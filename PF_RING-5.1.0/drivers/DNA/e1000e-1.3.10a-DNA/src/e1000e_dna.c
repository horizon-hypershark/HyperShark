/*
 *
 * (C) 2008-11 - Luca Deri <deri@ntop.org>
 * (C) 2011    - Alfredo Cardigliano <cardigliano@ntop.org>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#define MAX_NUM_ADAPTERS       8

static char *adapters_to_enable[MAX_NUM_ADAPTERS] = { 0 };
module_param_array(adapters_to_enable, charp, NULL, 0444);
MODULE_PARM_DESC(adapters_to_enable,
		 "Comma separated list of adapters where DNA "
		 "will be enabled");

static unsigned int enable_debug = 0;
module_param(enable_debug, uint, 0644);
MODULE_PARM_DESC(enable_debug, "Set to 1 to enable DNA debug tracing into the syslog");

/* Forward */
static void e1000_irq_enable(struct e1000_adapter *adapter);
static void e1000_irq_disable(struct e1000_adapter *adapter);

/* ****************************** */

void dna_check_enable_adapter(struct e1000_adapter *adapter) {
  adapter->dna.dna_enabled = 0; /* Default */
  
  if(strcmp(adapter->netdev->name, "eth0") == 0)
    return; /*
	      We never enable DNA on eth0 as this might be
	      the management interface
	    */

  if(adapters_to_enable[0] == NULL) {
    /* We enable all the adapters */
    adapter->dna.dna_enabled = 1;
  } else {
    int i = 0;

    while((i < MAX_NUM_ADAPTERS) && (adapters_to_enable[i] != NULL)) {
      if(!strcmp(adapters_to_enable[i], adapter->netdev->name)) {
	adapter->dna.dna_enabled = 1;
	break;
      }
      
      i++;
    } /* while */
  }
}

/* ****************************** */

void reserve_memory(unsigned long base, unsigned long len) {
  struct page *page, *page_end;

  page_end = virt_to_page(base + len - 1);
  for(page = virt_to_page(base); page <= page_end; page++)
    SetPageReserved(page);
}

/* ****************************** */

void unreserve_memory(unsigned long base, unsigned long len) {
  struct page *page, *page_end;

  page_end = virt_to_page(base + len - 1);
  for(page = virt_to_page(base); page <= page_end; page++)
    ClearPageReserved(page);
}

/* ********************************** */

static unsigned long alloc_contiguous_memory(u_int *tot_mem_len,
					     u_int *mem_order) {
  unsigned long mem;

  if(unlikely(enable_debug)) printk("[DNA] alloc_contiguous_memory(%d)\n", *tot_mem_len);

  *mem_order = get_order(*tot_mem_len);
  *tot_mem_len = PAGE_SIZE << *mem_order;

  mem = __get_free_pages(GFP_ATOMIC, *mem_order);

  if(mem) {
    if(unlikely(enable_debug))
      printk("[DNA] alloc_contiguous_memory: success (%d,%lu,%d)\n",
	     *tot_mem_len, mem, *mem_order);
    reserve_memory(mem, *tot_mem_len);
  } else {
    if(unlikely(enable_debug))
      printk("[DNA] alloc_contiguous_memory: failure (len=%d,order=%d)\n",
	     *tot_mem_len, *mem_order);
  }

  return(mem);
}

/* ********************************** */

static void free_contiguous_memory(unsigned long mem,
				   u_int tot_mem_len,
				   u_int mem_order) {
  if(unlikely(enable_debug))
    printk("[DNA] free_contiguous_memory(%lu,%d,%d)\n",
	   mem, tot_mem_len, mem_order);

  if(mem != 0) {
    unreserve_memory(mem, tot_mem_len);
    free_pages(mem, mem_order);
  }
}

/* ********************************** */

void init_dna(struct e1000_adapter *adapter) {
  init_waitqueue_head(&adapter->dna.packet_waitqueue);
}

/* ********************************** */

void notify_function_ptr(void *data, u_int8_t device_in_use) {
  /* struct e1000_adapter *adapter = (struct e1000_adapter*)data; */ /* Just in case */

  if(device_in_use)
    try_module_get(THIS_MODULE); /* ++ */
  else
    module_put(THIS_MODULE);     /* -- */
}

int wait_packet_function_ptr(void *data, int mode) {
  struct e1000_adapter *adapter = (struct e1000_adapter*)data;

  if(unlikely(enable_debug)) printk("[wait_packet_function_ptr] called [mode=%d]\n", mode);

  if(mode == 1) {
    struct e1000_ring *rx_ring = adapter->rx_ring;
    struct e1000_rx_desc *rx_desc;

    rx_ring->next_to_clean = E1000_READ_REG(&adapter->hw, E1000_RDT(0));
    rx_desc = E1000_RX_DESC(*rx_ring, rx_ring->next_to_clean);
    if(unlikely(enable_debug)) printk("[wait_packet_function_ptr] Check if a packet is arrived\n");

    prefetch(rx_desc);

    if(!(rx_desc->status & E1000_RXD_STAT_DD)) {
      adapter->dna.interrupt_received = 0;

#if 0
      if(!adapter->dna.interrupt_enabled) {
	e1000_irq_enable(adapter), adapter->dna.interrupt_enabled = 1;
	if(unlikely(enable_debug)) printk("[wait_packet_function_ptr] Packet not arrived yet: enabling interrupts\n");
      }
#endif
    } else
      adapter->dna.interrupt_received = 1;

    return(rx_desc->status & E1000_RXD_STAT_DD);
  } else {
    if(adapter->dna.interrupt_enabled) {
      e1000_irq_disable(adapter);
      adapter->dna.interrupt_enabled = 0;
      if(unlikely(enable_debug)) printk("[wait_packet_function_ptr] Disabled interrupts\n");
    }
    return(0);
  }
}

void alloc_dna_memory(struct e1000_adapter *adapter) {
  struct net_device *netdev = adapter->netdev;
  struct pci_dev *pdev = adapter->pdev;
  struct e1000_ring *rx_ring = adapter->rx_ring;
  struct e1000_ring *tx_ring = adapter->tx_ring;
  struct e1000_tx_desc *tx_desc;
  struct pfring_hooks *hook = (struct pfring_hooks*)netdev->pfring_ptr;
  struct e1000_rx_desc *rx_desc;
  struct e1000_buffer *buffer_info;
  u16 cache_line_size;
  struct sk_buff *skb;
  unsigned int i;
  int cleaned_count = rx_ring->count; /* Allocate all slots in one shot */
  unsigned int bufsz = adapter->rx_buffer_len;
  mem_ring_info         rx_info = {0}; 
  mem_ring_info         tx_info = {0}; 

  /* Buffers are allocated all in one shot so we'll pass here once */
#if 0
  printk("[DNA] e1000_alloc_rx_buffers(cleaned_count=%d)[%s][slot len=%u/%u]\n",
	 cleaned_count, adapter->netdev->name,
	 bufsz, adapter->max_hw_frame_size);
#endif

  if(hook && (hook->magic == PF_RING)) {
    if(adapter->dna.rx_packet_memory[0] == 0) {
      pci_read_config_word(adapter->pdev,
			   0x0C /* Conf. Space Cache Line Size offset */,
			   &cache_line_size);
      cache_line_size *= 2; /* word (2-byte) to bytes */

      if (0)
	printk("[DNA] Cache line size is %u bytes\n", cache_line_size);

      adapter->dna.packet_slot_len  = ALIGN(bufsz, cache_line_size);
      adapter->dna.packet_num_slots = cleaned_count;

      /* Align the slots to MAX_NUM_SLOTS_PER_PAGE */
      adapter->dna.packet_num_slots += (adapter->dna.packet_num_slots % MAX_NUM_SLOTS_PER_PAGE);
      adapter->dna.num_memory_pages  = adapter->dna.packet_num_slots / MAX_NUM_SLOTS_PER_PAGE;
      adapter->dna.tot_packet_memory = adapter->dna.packet_slot_len * MAX_NUM_SLOTS_PER_PAGE;

      if(0)
	printk("[DNA] Allocating memory [%u slots][%u memory pages][tot_packet_memory %u bytes]\n",
	       adapter->dna.packet_num_slots, adapter->dna.num_memory_pages, adapter->dna.tot_packet_memory);

      for(i=0; i<adapter->dna.num_memory_pages; i++) {
	adapter->dna.rx_packet_memory[i] =
	  alloc_contiguous_memory(&adapter->dna.tot_packet_memory, &adapter->dna.mem_order);

	if(adapter->dna.rx_packet_memory[i] != 0) {
	  if(0)
	    printk("[DNA]  Successfully allocated %lu bytes at "
		   "0x%08lx [slot_len=%d]\n",
		   (unsigned long) adapter->dna.tot_packet_memory,
		   (unsigned long) adapter->dna.rx_packet_memory[i],
		   adapter->dna.packet_slot_len);
	} else {
	  printk("[DNA]  ERROR: not enough memory for DMA ring\n");
	  return;
	}
      }

      for(i=0; i<cleaned_count; i++) {
	u_int page_index, offset;

	page_index = i / MAX_NUM_SLOTS_PER_PAGE;
	offset = (i % MAX_NUM_SLOTS_PER_PAGE) * adapter->dna.packet_slot_len;
	skb = (struct sk_buff *)(adapter->dna.rx_packet_memory[page_index] + offset);

	if(0)
	  printk("[DNA] Allocating slot %d of %d [addr=%p][page_index=%u][offset=%u]\n",
		 i, adapter->dna.packet_num_slots, skb, page_index, offset);

	buffer_info = &rx_ring->buffer_info[i];
	buffer_info->skb = skb;
	buffer_info->length = adapter->rx_buffer_len;
	buffer_info->dma = pci_map_single(pdev, skb,
					  buffer_info->length,
					  PCI_DMA_FROMDEVICE);

#if 0
	printk("[DNA] Mapping buffer %d [ptr=%p][len=%d]\n",
	       i, (void*)buffer_info->dma, adapter->dna.packet_slot_len);
#endif

	rx_desc = E1000_RX_DESC(*rx_ring, i);
	rx_desc->buffer_addr = cpu_to_le64(buffer_info->dma);
      }

      wmb();

      /* Tell the card that we have allocated all buckets */
      writel(rx_ring->count-1, adapter->hw.hw_addr + rx_ring->tail);

      /* The statement below syncs the value of next_to_clean with
	 the corresponding register, hence sync the kernel with
	 userland
      */
      E1000_WRITE_REG(&adapter->hw, E1000_RDT(0), rx_ring->next_to_clean);

      e1000_irq_disable(adapter);
      //e1000_irq_enable(adapter);

      /* TX */

      if(adapter->dna.tx_packet_memory[0] == 0) {

	for(i=0; i<adapter->dna.num_memory_pages; i++) {
	  adapter->dna.tx_packet_memory[i] =
	    alloc_contiguous_memory(&adapter->dna.tot_packet_memory, &adapter->dna.mem_order);

	  if(adapter->dna.tx_packet_memory[i] != 0) {
	    if(0)
	      printk("[DNA] [TX] Successfully allocated %lu bytes at "
		     "0x%08lx [slot_len=%d]\n",
		     (unsigned long) adapter->dna.tot_packet_memory,
		     (unsigned long) adapter->dna.rx_packet_memory[i],
		     adapter->dna.packet_slot_len);
	  } else {
	    printk("[DNA]  ERROR: not enough memory for DMA ring\n");
	    return;
	  }
	}

	for(i=0; i<cleaned_count; i++) {
	  u_int page_index, offset;

	  page_index = i / MAX_NUM_SLOTS_PER_PAGE;
	  offset = (i % MAX_NUM_SLOTS_PER_PAGE) * adapter->dna.packet_slot_len;
	  skb = (struct sk_buff *)(adapter->dna.tx_packet_memory[page_index] + offset);

	  if(0)
	    printk("[DNA] [TX] Allocating slot %d of %d [addr=%p][page_index=%u][offset=%u]\n",
		   i, adapter->dna.packet_num_slots, skb, page_index, offset);

	  buffer_info = &tx_ring->buffer_info[i];
	  buffer_info->skb = skb;
	  buffer_info->length = adapter->rx_buffer_len;
	  buffer_info->dma = pci_map_single(pdev, skb,
					    buffer_info->length,
					    PCI_DMA_TODEVICE);

#if 0
	  printk("[DNA] Mapping buffer %d [ptr=%p][len=%d]\n",
		 i, (void*)buffer_info->dma, adapter->dna.packet_slot_len);
#endif

	  tx_desc = E1000_TX_DESC(*tx_ring, i);
	  tx_desc->buffer_addr = cpu_to_le64(buffer_info->dma);
	}
      }

      rx_info.packet_memory_num_chunks    = adapter->dna.num_memory_pages; //1;
      rx_info.packet_memory_chunk_len     = adapter->dna.tot_packet_memory;
      rx_info.packet_memory_num_slots     = adapter->dna.packet_num_slots;
      rx_info.packet_memory_slot_len      = adapter->dna.packet_slot_len;
      rx_info.descr_packet_memory_tot_len = rx_ring->size;
  
      tx_info.packet_memory_num_chunks    = adapter->dna.num_memory_pages; //1;
      tx_info.packet_memory_chunk_len     = adapter->dna.tot_packet_memory;
      tx_info.packet_memory_num_slots     = adapter->dna.packet_num_slots;
      tx_info.packet_memory_slot_len      = adapter->dna.packet_slot_len;
      tx_info.descr_packet_memory_tot_len = rx_ring->size;

      /* Register with PF_RING */
      hook->ring_dna_device_handler(add_device_mapping,
				    dna_v1,
  				    &rx_info,
				    &tx_info,
				    adapter->dna.rx_packet_memory,
				    rx_ring->desc,
				    adapter->dna.tx_packet_memory,
				    tx_ring->desc, /* Packet descriptors */
				    (void*)netdev->mem_start,
				    netdev->mem_end-netdev->mem_start,
				    0, /* Channel Id */
				    netdev,
				    intel_e1000e,
				    adapter->netdev->dev_addr,
				    &adapter->dna.packet_waitqueue,
				    &adapter->dna.interrupt_received,
				    (void*)adapter,
				    wait_packet_function_ptr,
				    notify_function_ptr);

      if(1) printk("[DNA] Enabled DNA on %s (len=%u)\n", adapter->netdev->name, rx_ring->size);
    } else {
      printk("WARNING e1000_alloc_rx_buffers(cleaned_count=%d)"
	     "[%s][%lu] already allocated\n",
	     cleaned_count, adapter->netdev->name,
	     adapter->dna.rx_packet_memory[0]);

    }
  }
}
