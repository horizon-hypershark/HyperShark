/*******************************************************************************

  Intel(R) Gigabit Ethernet Linux driver
  Copyright(c) 2007-2010 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/netdevice.h>
#include <linux/tcp.h>
#ifdef NETIF_F_TSO
#include <net/checksum.h>
#ifdef NETIF_F_TSO6
#include <linux/ipv6.h>
#include <net/ip6_checksum.h>
#endif
#endif
#ifdef SIOCGMIIPHY
#include <linux/mii.h>
#endif
#ifdef SIOCETHTOOL
#include <linux/ethtool.h>
#endif
#include <linux/if_vlan.h>

#include "igb.h"
#include "igb_vmdq.h"

#ifdef HAVE_PF_RING
#include "../../../../../../kernel/linux/pf_ring.h"
#endif

#define DRV_DEBUG
#define DRV_HW_PERF
#define VERSION_SUFFIX

#define MAJ 3
#define MIN 0
#define BUILD 22
#define DRV_VERSION __stringify(MAJ) "." __stringify(MIN) "." __stringify(BUILD) VERSION_SUFFIX DRV_DEBUG DRV_HW_PERF

char igb_driver_name[] = "igb";
char igb_driver_version[] = DRV_VERSION;
static const char igb_driver_string[] =
                                "Intel(R) Gigabit Ethernet Network Driver";
static const char igb_copyright[] = "Copyright (c) 2007-2010 Intel Corporation.";

static struct pci_device_id igb_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_FIBER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_SGMII) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_FIBER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_SGMII) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_COPPER_DUAL) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_QUAD_FIBER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SGMII) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_BACKPLANE) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SFP) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_NS) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_NS_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_FIBER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_SERDES_QUAD) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_QUAD_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_QUAD_COPPER_ET2) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575EB_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575EB_FIBER_SERDES) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575GB_QUAD_COPPER) },
	/* required last entry */
	{0, }
};

MODULE_DEVICE_TABLE(pci, igb_pci_tbl);

void igb_reset(struct igb_adapter *);
static int igb_setup_all_tx_resources(struct igb_adapter *);
static int igb_setup_all_rx_resources(struct igb_adapter *);
static void igb_free_all_tx_resources(struct igb_adapter *);
static void igb_free_all_rx_resources(struct igb_adapter *);
static void igb_setup_mrqc(struct igb_adapter *);
void igb_update_stats(struct igb_adapter *);
static int igb_probe(struct pci_dev *, const struct pci_device_id *);
static void __devexit igb_remove(struct pci_dev *pdev);
static int igb_sw_init(struct igb_adapter *);
static int igb_open(struct net_device *);
static int igb_close(struct net_device *);
static void igb_configure_tx(struct igb_adapter *);
static void igb_configure_rx(struct igb_adapter *);
static void igb_clean_all_tx_rings(struct igb_adapter *);
static void igb_clean_all_rx_rings(struct igb_adapter *);
static void igb_clean_tx_ring(struct igb_ring *);
static void igb_set_rx_mode(struct net_device *);
static void igb_update_phy_info(unsigned long);
static void igb_watchdog(unsigned long);
static void igb_watchdog_task(struct work_struct *);
static netdev_tx_t igb_xmit_frame(struct sk_buff *skb, struct net_device *);
static struct net_device_stats *igb_get_stats(struct net_device *);
static int igb_change_mtu(struct net_device *, int);
void igb_full_sync_mac_table(struct igb_adapter *adapter);
static int igb_set_mac(struct net_device *, void *);
static void igb_set_uta(struct igb_adapter *adapter);
static irqreturn_t igb_intr(int irq, void *);
static irqreturn_t igb_intr_msi(int irq, void *);
static irqreturn_t igb_msix_other(int irq, void *);
static irqreturn_t igb_msix_ring(int irq, void *);
#ifdef IGB_DCA
static void igb_update_dca(struct igb_q_vector *);
static void igb_setup_dca(struct igb_adapter *);
#endif /* IGB_DCA */
static int igb_poll(struct napi_struct *, int);
static bool igb_clean_tx_irq(struct igb_q_vector *, int);
static void igb_clean_rx_irq(struct igb_q_vector *, int *, int);
static int igb_ioctl(struct net_device *, struct ifreq *, int cmd);
static void igb_tx_timeout(struct net_device *);
static void igb_reset_task(struct work_struct *);
static void igb_vlan_rx_register(struct net_device *, struct vlan_group *);
static void igb_vlan_rx_add_vid(struct net_device *, u16);
static void igb_vlan_rx_kill_vid(struct net_device *, u16);
static void igb_restore_vlan(struct igb_adapter *);
void igb_rar_set(struct igb_adapter *adapter, u32 index);
static void igb_ping_all_vfs(struct igb_adapter *);
static void igb_msg_task(struct igb_adapter *);
static void igb_vmm_control(struct igb_adapter *);
static int igb_set_vf_mac(struct igb_adapter *, int, unsigned char *);
static void igb_restore_vf_multicasts(struct igb_adapter *adapter);
#ifdef IFLA_VF_MAX
static int igb_ndo_set_vf_mac( struct net_device *netdev, int vf, u8 *mac);
static int igb_ndo_set_vf_vlan(struct net_device *netdev,
			       int vf, u16 vlan, u8 qos);
static int igb_ndo_set_vf_bw(struct net_device *netdev, int vf, int tx_rate);
static int igb_ndo_get_vf_config(struct net_device *netdev, int vf,
				 struct ifla_vf_info *ivi);
static void igb_check_vf_rate_limit(struct igb_adapter *);
#endif
#ifdef CONFIG_PM
static int igb_suspend(struct pci_dev *, pm_message_t);
static int igb_resume(struct pci_dev *);
#endif
#ifndef USE_REBOOT_NOTIFIER
static void igb_shutdown(struct pci_dev *);
#else
static int igb_notify_reboot(struct notifier_block *, unsigned long, void *);
static struct notifier_block igb_notifier_reboot = {
	.notifier_call	= igb_notify_reboot,
	.next		= NULL,
	.priority	= 0
};
#endif
#ifdef IGB_DCA
static int igb_notify_dca(struct notifier_block *, unsigned long, void *);
static struct notifier_block dca_notifier = {
	.notifier_call	= igb_notify_dca,
	.next		= NULL,
	.priority	= 0
};
#endif
#ifdef CONFIG_NET_POLL_CONTROLLER
/* for netdump / net console */
static void igb_netpoll(struct net_device *);
#endif

#ifdef HAVE_PCI_ERS
static pci_ers_result_t igb_io_error_detected(struct pci_dev *,
		     pci_channel_state_t);
static pci_ers_result_t igb_io_slot_reset(struct pci_dev *);
static void igb_io_resume(struct pci_dev *);

static struct pci_error_handlers igb_err_handler = {
	.error_detected = igb_io_error_detected,
	.slot_reset = igb_io_slot_reset,
	.resume = igb_io_resume,
};
#endif

static void igb_init_fw(struct igb_adapter *adapter);


static struct pci_driver igb_driver = {
	.name     = igb_driver_name,
	.id_table = igb_pci_tbl,
	.probe    = igb_probe,
	.remove   = __devexit_p(igb_remove),
#ifdef CONFIG_PM
	/* Power Managment Hooks */
	.suspend  = igb_suspend,
	.resume   = igb_resume,
#endif
#ifndef USE_REBOOT_NOTIFIER
	.shutdown = igb_shutdown,
#endif
#ifdef HAVE_PCI_ERS
	.err_handler = &igb_err_handler
#endif
};

MODULE_AUTHOR("Intel Corporation, <e1000-devel@lists.sourceforge.net>");
MODULE_DESCRIPTION("Intel(R) Gigabit Ethernet Network Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

static void igb_vfta_set(struct e1000_hw *hw, u32 vid, bool add)
{
	struct e1000_host_mng_dhcp_cookie *mng_cookie = &hw->mng_cookie;
	u32 index = (vid >> E1000_VFTA_ENTRY_SHIFT) & E1000_VFTA_ENTRY_MASK;
	u32 mask = 1 << (vid & E1000_VFTA_ENTRY_BIT_SHIFT_MASK);
	u32 vfta;

	/*
	 * if this is the management vlan the only option is to add it in so
	 * that the management pass through will continue to work
	 */
	if ((mng_cookie->status & E1000_MNG_DHCP_COOKIE_STATUS_VLAN) &&
	    (vid == mng_cookie->vlan_id))
		add = TRUE;

	vfta = E1000_READ_REG_ARRAY(hw, E1000_VFTA, index);
	if (add)
		vfta |= mask;
	else
		vfta &= ~mask;

	e1000_write_vfta(hw, index, vfta);
}

#ifdef HAVE_HW_TIME_STAMP
/**
 * igb_read_clock - read raw cycle counter (to be used by time counter)
 */
static cycle_t igb_read_clock(const struct cyclecounter *tc)
{
	struct igb_adapter *adapter =
		container_of(tc, struct igb_adapter, cycles);
	struct e1000_hw *hw = &adapter->hw;
	u64 stamp = 0;
	int shift = 0;

	/*
	 * The timestamp latches on lowest register read. For the 82580
	 * the lowest register is SYSTIMR instead of SYSTIML.  However we never
	 * adjusted TIMINCA so SYSTIMR will just read as all 0s so ignore it.
	 */
	if (hw->mac.type >= e1000_82580) {
		stamp = E1000_READ_REG(hw, E1000_SYSTIMR) >> 8;
		shift = IGB_82580_TSYNC_SHIFT;
	}

	stamp |= (u64)E1000_READ_REG(hw, E1000_SYSTIML) << shift;
	stamp |= (u64)E1000_READ_REG(hw, E1000_SYSTIML) << shift;
	stamp |= (u64)E1000_READ_REG(hw, E1000_SYSTIML) << shift;
	stamp |= (u64)E1000_READ_REG(hw, E1000_SYSTIML) << shift;
	stamp |= (u64)E1000_READ_REG(hw, E1000_SYSTIMH) << (shift + 32);
	return stamp;
}

#endif /* SIOCSHWTSTAMP */
static int debug = NETIF_MSG_DRV | NETIF_MSG_PROBE;
module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "Debug level (0=none, ..., 16=all)");

/**
 * igb_init_module - Driver Registration Routine
 *
 * igb_init_module is the first routine called when the driver is
 * loaded. All it does is register with the PCI subsystem.
 **/
static int __init igb_init_module(void)
{
	int ret;

	printk(KERN_INFO "%s - version %s\n",
	       igb_driver_string, igb_driver_version);

	printk(KERN_INFO "%s\n", igb_copyright);

#ifdef IGB_DCA
	dca_register_notify(&dca_notifier);
#endif
	ret = pci_register_driver(&igb_driver);
#ifdef USE_REBOOT_NOTIFIER
	if (ret >= 0) {
		register_reboot_notifier(&igb_notifier_reboot);
	}
#endif
	return ret;
}

module_init(igb_init_module);

/**
 * igb_exit_module - Driver Exit Cleanup Routine
 *
 * igb_exit_module is called just before the driver is removed
 * from memory.
 **/
static void __exit igb_exit_module(void)
{
#ifdef IGB_DCA
	dca_unregister_notify(&dca_notifier);
#endif
#ifdef USE_REBOOT_NOTIFIER
	unregister_reboot_notifier(&igb_notifier_reboot);
#endif
	pci_unregister_driver(&igb_driver);
}

module_exit(igb_exit_module);

#define Q_IDX_82576(i) (((i & 0x1) << 3) + (i >> 1))
/**
 * igb_cache_ring_register - Descriptor ring to register mapping
 * @adapter: board private structure to initialize
 *
 * Once we know the feature-set enabled for the device, we'll cache
 * the register offset the descriptor ring is assigned to.
 **/
static void igb_cache_ring_register(struct igb_adapter *adapter)
{
	int i = 0, j = 0;
	u32 rbase_offset = adapter->vfs_allocated_count;

	switch (adapter->hw.mac.type) {
	case e1000_82576:
		/* The queues are allocated for virtualization such that VF 0
		 * is allocated queues 0 and 8, VF 1 queues 1 and 9, etc.
		 * In order to avoid collision we start at the first free queue
		 * and continue consuming queues in the same sequence
		 */
		if ((adapter->rss_queues > 1) && adapter->vmdq_pools) {
			for (; i < adapter->rss_queues; i++)
				adapter->rx_ring[i]->reg_idx = rbase_offset +
				                               Q_IDX_82576(i);
		}
	case e1000_82575:
	case e1000_82580:
	case e1000_i350:
	default:
		for (; i < adapter->num_rx_queues; i++)
			adapter->rx_ring[i]->reg_idx = rbase_offset + i;
		for (; j < adapter->num_tx_queues; j++)
			adapter->tx_ring[j]->reg_idx = rbase_offset + j;
		break;
	}
}

static void igb_free_queues(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		kfree(adapter->tx_ring[i]);
		adapter->tx_ring[i] = NULL;
	}
	for (i = 0; i < adapter->num_rx_queues; i++) {
		kfree(adapter->rx_ring[i]);
		adapter->rx_ring[i] = NULL;
	}
	adapter->num_rx_queues = 0;
	adapter->num_tx_queues = 0;
}

/**
 * igb_alloc_queues - Allocate memory for all rings
 * @adapter: board private structure to initialize
 *
 * We allocate one ring per queue at run-time since we don't know the
 * number of queues at compile-time.
 **/
static int igb_alloc_queues(struct igb_adapter *adapter)
{
	struct igb_ring *ring;
	int i;
#ifdef HAVE_DEVICE_NUMA_NODE
	int orig_node = adapter->node;
#endif /* HAVE_DEVICE_NUMA_NODE */

	for (i = 0; i < adapter->num_tx_queues; i++) {
#ifdef HAVE_DEVICE_NUMA_NODE
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
#endif /* HAVE_DEVICE_NUMA_NODE */
		ring = kzalloc_node(sizeof(struct igb_ring), GFP_KERNEL,
		                    adapter->node);
		if (!ring)
			ring = kzalloc(sizeof(struct igb_ring), GFP_KERNEL);
		if (!ring)
			goto err;
		ring->count = adapter->tx_ring_count;
		ring->queue_index = i;
		ring->dev = pci_dev_to_dev(adapter->pdev);
		ring->netdev = adapter->netdev;
		ring->numa_node = adapter->node;
		/* For 82575, context index must be unique per ring. */
		if (adapter->hw.mac.type == e1000_82575)
			set_bit(IGB_RING_FLAG_TX_CTX_IDX, &ring->flags);
		adapter->tx_ring[i] = ring;
	}
#ifdef HAVE_DEVICE_NUMA_NODE
	/* Restore the adapter's original node */
	adapter->node = orig_node;
#endif /* HAVE_DEVICE_NUMA_NODE */

	for (i = 0; i < adapter->num_rx_queues; i++) {
#ifdef HAVE_DEVICE_NUMA_NODE
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
#endif /* HAVE_DEVICE_NUMA_NODE */
		ring = kzalloc_node(sizeof(struct igb_ring), GFP_KERNEL,
		                    adapter->node);
		if (!ring)
			ring = kzalloc(sizeof(struct igb_ring), GFP_KERNEL);
		if (!ring)
			goto err;
		ring->count = adapter->rx_ring_count;
		ring->queue_index = i;
		ring->dev = pci_dev_to_dev(adapter->pdev);
		ring->netdev = adapter->netdev;
		ring->numa_node = adapter->node;
#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
		ring->rx_buffer_len = MAXIMUM_ETHERNET_VLAN_SIZE;
#endif
		/* enable rx checksum */
		set_bit(IGB_RING_FLAG_RX_CSUM, &ring->flags);
		/* set flag indicating ring supports SCTP checksum offload */
		if (adapter->hw.mac.type >= e1000_82576)
			set_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags);
#ifdef IGB_LRO
		/* set flag enabling LRO */
		if (i < adapter->rss_queues)
			set_bit(IGB_RING_FLAG_RX_LRO, &ring->flags);
#endif
		/* On i350, loopback VLAN packets have the tag byte-swapped. */
		if (adapter->hw.mac.type == e1000_i350)
			set_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &ring->flags);

		adapter->rx_ring[i] = ring;
	}
#ifdef HAVE_DEVICE_NUMA_NODE
	/* Restore the adapter's original node */
	adapter->node = orig_node;
#endif /* HAVE_DEVICE_NUMA_NODE */

	igb_cache_ring_register(adapter);

	return E1000_SUCCESS;

err:
#ifdef HAVE_DEVICE_NUMA_NODE
	/* Restore the adapter's original node */
	adapter->node = orig_node;
#endif /* HAVE_DEVICE_NUMA_NODE */
	igb_free_queues(adapter);

	return -ENOMEM;
}

static void igb_configure_lli(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u16 port;

	/* LLI should only be enabled for MSI-X or MSI interrupts */
	if (!adapter->msix_entries && !(adapter->flags & IGB_FLAG_HAS_MSI))
		return;

	if (adapter->lli_port) {
		/* use filter 0 for port */
		port = htons((u16)adapter->lli_port);
		E1000_WRITE_REG(hw, E1000_IMIR(0),
			(port | E1000_IMIR_PORT_IM_EN));
		E1000_WRITE_REG(hw, E1000_IMIREXT(0),
			(E1000_IMIREXT_SIZE_BP | E1000_IMIREXT_CTRL_BP));
	}

	if (adapter->flags & IGB_FLAG_LLI_PUSH) {
		/* use filter 1 for push flag */
		E1000_WRITE_REG(hw, E1000_IMIR(1),
			(E1000_IMIR_PORT_BP | E1000_IMIR_PORT_IM_EN));
		E1000_WRITE_REG(hw, E1000_IMIREXT(1),
			(E1000_IMIREXT_SIZE_BP | E1000_IMIREXT_CTRL_PSH));
	}

	if (adapter->lli_size) {
		/* use filter 2 for size */
		E1000_WRITE_REG(hw, E1000_IMIR(2),
			(E1000_IMIR_PORT_BP | E1000_IMIR_PORT_IM_EN));
		E1000_WRITE_REG(hw, E1000_IMIREXT(2),
			(adapter->lli_size | E1000_IMIREXT_CTRL_BP));
	}

}

#define IGB_N0_QUEUE -1
static void igb_assign_vector(struct igb_q_vector *q_vector, int msix_vector)
{
	u32 msixbm = 0;
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	u32 ivar, index;
	int rx_queue = IGB_N0_QUEUE;
	int tx_queue = IGB_N0_QUEUE;

	if (q_vector->rx_ring)
		rx_queue = q_vector->rx_ring->reg_idx;
	if (q_vector->tx_ring)
		tx_queue = q_vector->tx_ring->reg_idx;

	switch (hw->mac.type) {
	case e1000_82575:
		/* The 82575 assigns vectors using a bitmask, which matches the
		   bitmask for the EICR/EIMS/EIMC registers.  To assign one
		   or more queues to a vector, we write the appropriate bits
		   into the MSIXBM register for that vector. */
		if (rx_queue > IGB_N0_QUEUE)
			msixbm = E1000_EICR_RX_QUEUE0 << rx_queue;
		if (tx_queue > IGB_N0_QUEUE)
			msixbm |= E1000_EICR_TX_QUEUE0 << tx_queue;
		if (!adapter->msix_entries && msix_vector == 0)
			msixbm |= E1000_EIMS_OTHER;
		E1000_WRITE_REG_ARRAY(hw, E1000_MSIXBM(0), msix_vector, msixbm);
		q_vector->eims_value = msixbm;
		break;
	case e1000_82576:
		/* 82576 uses a table-based method for assigning vectors.
		   Each queue has a single entry in the table to which we write
		   a vector number along with a "valid" bit.  Sadly, the layout
		   of the table is somewhat counterintuitive. */
		if (rx_queue > IGB_N0_QUEUE) {
			index = (rx_queue & 0x7);
			ivar = E1000_READ_REG_ARRAY(hw, E1000_IVAR0, index);
			if (rx_queue < 8) {
				/* vector goes into low byte of register */
				ivar = ivar & 0xFFFFFF00;
				ivar |= msix_vector | E1000_IVAR_VALID;
			} else {
				/* vector goes into third byte of register */
				ivar = ivar & 0xFF00FFFF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 16;
			}
			E1000_WRITE_REG_ARRAY(hw, E1000_IVAR0, index, ivar);
		}
		if (tx_queue > IGB_N0_QUEUE) {
			index = (tx_queue & 0x7);
			ivar = E1000_READ_REG_ARRAY(hw, E1000_IVAR0, index);
			if (tx_queue < 8) {
				/* vector goes into second byte of register */
				ivar = ivar & 0xFFFF00FF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 8;
			} else {
				/* vector goes into high byte of register */
				ivar = ivar & 0x00FFFFFF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 24;
			}
			E1000_WRITE_REG_ARRAY(hw, E1000_IVAR0, index, ivar);
		}
		q_vector->eims_value = 1 << msix_vector;
		break;
	case e1000_82580:
	case e1000_i350:
		/* 82580 uses the same table-based approach as 82576 but has fewer
		   entries as a result we carry over for queues greater than 4. */
		if (rx_queue > IGB_N0_QUEUE) {
			index = (rx_queue >> 1);
			ivar = E1000_READ_REG_ARRAY(hw, E1000_IVAR0, index);
			if (rx_queue & 0x1) {
				/* vector goes into third byte of register */
				ivar = ivar & 0xFF00FFFF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 16;
			} else {
				/* vector goes into low byte of register */
				ivar = ivar & 0xFFFFFF00;
				ivar |= msix_vector | E1000_IVAR_VALID;
			}
			E1000_WRITE_REG_ARRAY(hw, E1000_IVAR0, index, ivar);
		}
		if (tx_queue > IGB_N0_QUEUE) {
			index = (tx_queue >> 1);
			ivar = E1000_READ_REG_ARRAY(hw, E1000_IVAR0, index);
			if (tx_queue & 0x1) {
				/* vector goes into high byte of register */
				ivar = ivar & 0x00FFFFFF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 24;
			} else {
				/* vector goes into second byte of register */
				ivar = ivar & 0xFFFF00FF;
				ivar |= (msix_vector | E1000_IVAR_VALID) << 8;
			}
			E1000_WRITE_REG_ARRAY(hw, E1000_IVAR0, index, ivar);
		}
		q_vector->eims_value = 1 << msix_vector;
		break;
	default:
		BUG();
		break;
	}

	/* add q_vector eims value to global eims_enable_mask */
	adapter->eims_enable_mask |= q_vector->eims_value;

	/* configure q_vector to set itr on first interrupt */
	q_vector->set_itr = 1;
}

/**
 * igb_configure_msix - Configure MSI-X hardware
 *
 * igb_configure_msix sets up the hardware to properly
 * generate MSI-X interrupts.
 **/
static void igb_configure_msix(struct igb_adapter *adapter)
{
	u32 tmp;
	int i, vector = 0;
	struct e1000_hw *hw = &adapter->hw;

	adapter->eims_enable_mask = 0;

	/* set vector for other causes, i.e. link changes */
	switch (hw->mac.type) {
	case e1000_82575:
		tmp = E1000_READ_REG(hw, E1000_CTRL_EXT);
		/* enable MSI-X PBA support*/
		tmp |= E1000_CTRL_EXT_PBA_CLR;

		/* Auto-Mask interrupts upon ICR read. */
		tmp |= E1000_CTRL_EXT_EIAME;
		tmp |= E1000_CTRL_EXT_IRCA;

		E1000_WRITE_REG(hw, E1000_CTRL_EXT, tmp);

		/* enable msix_other interrupt */
		E1000_WRITE_REG_ARRAY(hw, E1000_MSIXBM(0), vector++,
		                      E1000_EIMS_OTHER);
		adapter->eims_other = E1000_EIMS_OTHER;

		break;

	case e1000_82576:
	case e1000_82580:
	case e1000_i350:
		/* Turn on MSI-X capability first, or our settings
		 * won't stick.  And it will take days to debug. */
		E1000_WRITE_REG(hw, E1000_GPIE, E1000_GPIE_MSIX_MODE |
		                E1000_GPIE_PBA | E1000_GPIE_EIAME |
		                E1000_GPIE_NSICR);

		/* enable msix_other interrupt */
		adapter->eims_other = 1 << vector;
		tmp = (vector++ | E1000_IVAR_VALID) << 8;

		E1000_WRITE_REG(hw, E1000_IVAR_MISC, tmp);
		break;
	default:
		/* do nothing, since nothing else supports MSI-X */
		break;
	} /* switch (hw->mac.type) */

	adapter->eims_enable_mask |= adapter->eims_other;

	for (i = 0; i < adapter->num_q_vectors; i++)
		igb_assign_vector(adapter->q_vector[i], vector++);

	E1000_WRITE_FLUSH(hw);
}

/**
 * igb_request_msix - Initialize MSI-X interrupts
 *
 * igb_request_msix allocates MSI-X vectors and requests interrupts from the
 * kernel.
 **/
static int igb_request_msix(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	int i, err = 0, vector = 0;

	err = request_irq(adapter->msix_entries[vector].vector,
	                  &igb_msix_other, 0, netdev->name, adapter);
	if (err)
		goto out;
	vector++;

	for (i = 0; i < adapter->num_q_vectors; i++) {
		struct igb_q_vector *q_vector = adapter->q_vector[i];

		q_vector->itr_register = hw->hw_addr + E1000_EITR(vector);

		if (q_vector->rx_ring && q_vector->tx_ring)
			sprintf(q_vector->name, "%s-TxRx-%u", netdev->name,
			        q_vector->rx_ring->queue_index);
		else if (q_vector->tx_ring)
			sprintf(q_vector->name, "%s-tx-%u", netdev->name,
			        q_vector->tx_ring->queue_index);
		else if (q_vector->rx_ring)
			sprintf(q_vector->name, "%s-rx-%u", netdev->name,
			        q_vector->rx_ring->queue_index);
		else
			sprintf(q_vector->name, "%s-unused", netdev->name);

		err = request_irq(adapter->msix_entries[vector].vector,
		                  igb_msix_ring, 0, q_vector->name,
		                  q_vector);
		if (err)
			goto out;
		vector++;
	}

	igb_configure_msix(adapter);
out:
	return err;
}

static void igb_reset_interrupt_capability(struct igb_adapter *adapter)
{
	if (adapter->msix_entries) {
		pci_disable_msix(adapter->pdev);
		kfree(adapter->msix_entries);
		adapter->msix_entries = NULL;
	} else if (adapter->flags & IGB_FLAG_HAS_MSI) {
		pci_disable_msi(adapter->pdev);
	}
}

#ifdef IGB_LRO
static void igb_lro_ring_exit(struct igb_lro_list *lrolist)
{
	struct hlist_node *node, *node2;
	struct igb_lro_desc *lrod;

	hlist_for_each_entry_safe(lrod, node, node2, &lrolist->active,
	                          lro_node) {
		hlist_del(&lrod->lro_node);
		kfree(lrod);
	}

	hlist_for_each_entry_safe(lrod, node, node2, &lrolist->free,
	                          lro_node) {
		hlist_del(&lrod->lro_node);
		kfree(lrod);
	}
}

static void igb_lro_ring_init(struct igb_lro_list *lrolist)
{
	int j, bytes;
	struct igb_lro_desc *lrod;

	bytes = sizeof(struct igb_lro_desc);

	INIT_HLIST_HEAD(&lrolist->free);
	INIT_HLIST_HEAD(&lrolist->active);

	for (j = 0; j < IGB_LRO_MAX; j++) {
		lrod = kzalloc(bytes, GFP_KERNEL);
		if (lrod != NULL) {
			INIT_HLIST_NODE(&lrod->lro_node);
			hlist_add_head(&lrod->lro_node, &lrolist->free);
		}
	}
}

#endif /* IGB_LRO */
/**
 * igb_free_q_vectors - Free memory allocated for interrupt vectors
 * @adapter: board private structure to initialize
 *
 * This function frees the memory allocated to the q_vectors.  In addition if
 * NAPI is enabled it will delete any references to the NAPI struct prior
 * to freeing the q_vector.
 **/
static void igb_free_q_vectors(struct igb_adapter *adapter)
{
	struct igb_q_vector *q_vector;
	int v_idx;

	for (v_idx = 0; v_idx < adapter->num_q_vectors; v_idx++) {
		q_vector = adapter->q_vector[v_idx];
		adapter->q_vector[v_idx] = NULL;
		if (!q_vector)
			continue;
		netif_napi_del(&q_vector->napi);
#ifdef IGB_LRO
		if (q_vector->lrolist) {
			igb_lro_ring_exit(q_vector->lrolist);
			vfree(q_vector->lrolist);
			q_vector->lrolist = NULL;
		}
#endif
		kfree(q_vector);
	}
	adapter->num_q_vectors = 0;
}

/**
 * igb_clear_interrupt_scheme - reset the device to a state of no interrupts
 *
 * This function resets the device so that it has 0 rx queues, tx queues, and
 * MSI-X interrupts allocated.
 */
static void igb_clear_interrupt_scheme(struct igb_adapter *adapter)
{
	igb_free_queues(adapter);
	igb_free_q_vectors(adapter);
	igb_reset_interrupt_capability(adapter);
}

/**
 * igb_reset_sriov_capability - disable SR-IOV if enabled
 *
 * Attempt to disable single root IO virtualization capabilites present in the
 * kernel.
 **/
static void igb_reset_sriov_capability(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;

	/* reclaim resources allocated to VFs */
	if (adapter->vf_data) {
		/* disable iov and allow time for transactions to clear */
		pci_disable_sriov(pdev);
		msleep(500);

		/* free vf data storage */
		kfree(adapter->vf_data);
		adapter->vf_data = NULL;

		/* switch rings back to PF ownership */
		E1000_WRITE_REG(hw, E1000_IOVCTL, E1000_IOVCTL_REUSE_VFQ);
		msleep(100);

		dev_info(pci_dev_to_dev(pdev), "IOV Disabled\n");
	}

	adapter->vfs_allocated_count = 0;
}

/**
 * igb_set_sriov_capability - setup SR-IOV if supported
 *
 * Attempt to enable single root IO virtualization capabilites present in the
 * kernel.
 **/
static void igb_set_sriov_capability(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	unsigned char mac_addr[ETH_ALEN];
	int i;

	/* no VFs requested, do nothing */
	if (!adapter->vfs_allocated_count)
		return;

	/* allocate vf data storage */
	adapter->vf_data = kcalloc(adapter->vfs_allocated_count,
	                           sizeof(struct vf_data_storage),
	                           GFP_KERNEL);

	if (adapter->vf_data) {
		for (i = 0; i < adapter->vfs_allocated_count; i++) {
			random_ether_addr(mac_addr);
			igb_set_vf_mac(adapter, i, mac_addr);
		}

		if (!pci_enable_sriov(pdev, adapter->vfs_allocated_count)) {
			/* DMA Coalescing is not supported in IOV mode. */
			if (adapter->flags & IGB_FLAG_DMAC)
				adapter->flags &= ~IGB_FLAG_DMAC;
			return;
		}

		kfree(adapter->vf_data);
		adapter->vf_data = NULL;
	}

	adapter->vfs_allocated_count = 0;
	dev_warn(pci_dev_to_dev(pdev), "Failed to initialize SR-IOV virtualization\n");
}

/**
 * igb_set_interrupt_capability - set MSI or MSI-X if supported
 *
 * Attempt to configure interrupts using the best available
 * capabilities of the hardware and kernel.
 **/
static void igb_set_interrupt_capability(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int err;
	int numvecs, i;

	/* Number of supported queues. */
	adapter->num_rx_queues = adapter->rss_queues;

	if (adapter->vmdq_pools > 1)
		adapter->num_rx_queues += adapter->vmdq_pools - 1;

#ifdef HAVE_TX_MQ
	if (adapter->vmdq_pools)
		adapter->num_tx_queues = adapter->vmdq_pools;
	else
		adapter->num_tx_queues = adapter->num_rx_queues;
#else
	adapter->num_tx_queues = max_t(u32, 1, adapter->vmdq_pools);
#endif

	switch (adapter->int_mode) {
	case IGB_INT_MODE_MSIX:
		/* start with one vector for every rx queue */
		numvecs = adapter->num_rx_queues;

		/* if tx handler is seperate add 1 for every tx queue */
		if (!(adapter->flags & IGB_FLAG_QUEUE_PAIRS))
			numvecs += adapter->num_tx_queues;

		/* store the number of vectors reserved for queues */
		adapter->num_q_vectors = numvecs;

		/* add 1 vector for link status interrupts */
		numvecs++;
		adapter->msix_entries = kcalloc(numvecs,
		                                sizeof(struct msix_entry),
		                                GFP_KERNEL);
		if (adapter->msix_entries) {
			for (i = 0; i < numvecs; i++)
				adapter->msix_entries[i].entry = i;

			err = pci_enable_msix(pdev,
			                      adapter->msix_entries, numvecs);
			if (err == 0)
				break;
		}
		/* MSI-X failed, so fall through and try MSI */
		dev_warn(pci_dev_to_dev(pdev), "Failed to initialize MSI-X interrupts. "
		         "Falling back to MSI interrupts.\n");
		igb_reset_interrupt_capability(adapter);
	case IGB_INT_MODE_MSI:
		if (!pci_enable_msi(pdev))
			adapter->flags |= IGB_FLAG_HAS_MSI;
		else
			dev_warn(pci_dev_to_dev(pdev), "Failed to initialize MSI "
			         "interrupts.  Falling back to legacy "
			         "interrupts.\n");
		/* Fall through */
	case IGB_INT_MODE_LEGACY:
		/* disable advanced features and set number of queues to 1 */
		igb_reset_sriov_capability(adapter);
		adapter->vmdq_pools = 0;
		adapter->rss_queues = 1;
		adapter->flags |= IGB_FLAG_QUEUE_PAIRS;
		adapter->num_rx_queues = 1;
		adapter->num_tx_queues = 1;
		adapter->num_q_vectors = 1;
		/* Don't do anything; this is system default */
		break;
	}

#ifdef HAVE_TX_MQ
	/* Notify the stack of the (possibly) reduced Tx Queue count. */
#ifdef CONFIG_NETDEVICES_MULTIQUEUE
	adapter->netdev->egress_subqueue_count = adapter->num_tx_queues;
#else
	adapter->netdev->real_num_tx_queues =
			(adapter->vmdq_pools ? 1 : adapter->num_tx_queues);
#endif
#endif
}

/**
 * igb_alloc_q_vectors - Allocate memory for interrupt vectors
 * @adapter: board private structure to initialize
 *
 * We allocate one q_vector per queue interrupt.  If allocation fails we
 * return -ENOMEM.
 **/
static int igb_alloc_q_vectors(struct igb_adapter *adapter)
{
	struct igb_q_vector *q_vector;
	struct e1000_hw *hw = &adapter->hw;
	int v_idx;
#ifdef HAVE_DEVICE_NUMA_NODE
	int orig_node = adapter->node;
#endif /* HAVE_DEVICE_NUMA_NODE */

	for (v_idx = 0; v_idx < adapter->num_q_vectors; v_idx++) {
#ifdef HAVE_DEVICE_NUMA_NODE
		if ((adapter->num_q_vectors == (adapter->num_rx_queues + adapter->num_tx_queues)) &&
		    (adapter->num_rx_queues == v_idx))
			adapter->node = orig_node;
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
#endif /* HAVE_DEVICE_NUMA_NODE */
		q_vector = kzalloc_node(sizeof(struct igb_q_vector), GFP_KERNEL,
		                        adapter->node);
		if (!q_vector)
			q_vector = kzalloc(sizeof(struct igb_q_vector), GFP_KERNEL);
		if (!q_vector)
			goto err_out;
		q_vector->adapter = adapter;
		q_vector->itr_register = hw->hw_addr + E1000_EITR(0);
		q_vector->itr_val = IGB_START_ITR;
		netif_napi_add(adapter->netdev, &q_vector->napi, igb_poll, 64);
		adapter->q_vector[v_idx] = q_vector;
#ifdef IGB_LRO
		if (v_idx < adapter->num_rx_queues) {
			int size = sizeof(struct igb_lro_list);
			q_vector->lrolist = vzalloc_node(size, q_vector->numa_node);
			if (!q_vector->lrolist)
				q_vector->lrolist = vzalloc(size);
			if (!q_vector->lrolist)
				goto err_out;
			igb_lro_ring_init(q_vector->lrolist);
		}
#endif
	}
#ifdef HAVE_DEVICE_NUMA_NODE
	/* Restore the adapter's original node */
	adapter->node = orig_node;
#endif /* HAVE_DEVICE_NUMA_NODE */

	return 0;

err_out:
#ifdef HAVE_DEVICE_NUMA_NODE
	/* Restore the adapter's original node */
	adapter->node = orig_node;
#endif /* HAVE_DEVICE_NUMA_NODE */
	igb_free_q_vectors(adapter);
	return -ENOMEM;
}

static void igb_map_rx_ring_to_vector(struct igb_adapter *adapter,
                                      int ring_idx, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	q_vector->rx_ring = adapter->rx_ring[ring_idx];
	q_vector->rx_ring->q_vector = q_vector;
	q_vector->itr_val = adapter->rx_itr_setting;
	if (q_vector->itr_val && q_vector->itr_val <= 3)
		q_vector->itr_val = IGB_START_ITR;
}

static void igb_map_tx_ring_to_vector(struct igb_adapter *adapter,
                                      int ring_idx, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	q_vector->tx_ring = adapter->tx_ring[ring_idx];
	q_vector->tx_ring->q_vector = q_vector;
	q_vector->itr_val = adapter->tx_itr_setting;
	if (q_vector->itr_val && q_vector->itr_val <= 3)
		q_vector->itr_val = IGB_START_ITR;
}

/**
 * igb_map_ring_to_vector - maps allocated queues to vectors
 *
 * This function maps the recently allocated queues to vectors.
 **/
static int igb_map_ring_to_vector(struct igb_adapter *adapter)
{
	int i;
	int v_idx = 0;

	if ((adapter->num_q_vectors < adapter->num_rx_queues) ||
	    (adapter->num_q_vectors < adapter->num_tx_queues))
		return -ENOMEM;

	if (adapter->num_q_vectors >=
	    (adapter->num_rx_queues + adapter->num_tx_queues)) {
		for (i = 0; i < adapter->num_rx_queues; i++)
			igb_map_rx_ring_to_vector(adapter, i, v_idx++);
		for (i = 0; i < adapter->num_tx_queues; i++)
			igb_map_tx_ring_to_vector(adapter, i, v_idx++);
	} else {
		for (i = 0; i < adapter->num_rx_queues; i++) {
			if (i < adapter->num_tx_queues)
				igb_map_tx_ring_to_vector(adapter, i, v_idx);
			igb_map_rx_ring_to_vector(adapter, i, v_idx++);
		}
		for (; i < adapter->num_tx_queues; i++)
			igb_map_tx_ring_to_vector(adapter, i, v_idx++);
	}
	return 0;
}

/**
 * igb_init_interrupt_scheme - initialize interrupts, allocate queues/vectors
 *
 * This function initializes the interrupts and allocates all of the queues.
 **/
static int igb_init_interrupt_scheme(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int err;

	igb_set_interrupt_capability(adapter);

	err = igb_alloc_q_vectors(adapter);
	if (err) {
		dev_err(pci_dev_to_dev(pdev), "Unable to allocate memory for vectors\n");
		goto err_alloc_q_vectors;
	}

	err = igb_alloc_queues(adapter);
	if (err) {
		dev_err(pci_dev_to_dev(pdev), "Unable to allocate memory for queues\n");
		goto err_alloc_queues;
	}

	err = igb_map_ring_to_vector(adapter);
	if (err) {
		dev_err(pci_dev_to_dev(pdev), "Invalid q_vector to ring mapping\n");
		goto err_map_queues;
	}


	return 0;
err_map_queues:
	igb_free_queues(adapter);
err_alloc_queues:
	igb_free_q_vectors(adapter);
err_alloc_q_vectors:
	igb_reset_interrupt_capability(adapter);
	return err;
}

/**
 * igb_request_irq - initialize interrupts
 *
 * Attempts to configure interrupts using the best available
 * capabilities of the hardware and kernel.
 **/
static int igb_request_irq(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	int err = 0;

	if (adapter->msix_entries) {
		err = igb_request_msix(adapter);
		if (!err)
			goto request_done;
		/* fall back to MSI */
		igb_clear_interrupt_scheme(adapter);
		igb_reset_sriov_capability(adapter);
		if (!pci_enable_msi(pdev))
			adapter->flags |= IGB_FLAG_HAS_MSI;
		igb_free_all_tx_resources(adapter);
		igb_free_all_rx_resources(adapter);
		adapter->num_tx_queues = 1;
		adapter->num_rx_queues = 1;
		adapter->num_q_vectors = 1;
		err = igb_alloc_q_vectors(adapter);
		if (err) {
			dev_err(pci_dev_to_dev(pdev),
			        "Unable to allocate memory for vectors\n");
			goto request_done;
		}
		err = igb_alloc_queues(adapter);
		if (err) {
			dev_err(pci_dev_to_dev(pdev),
			        "Unable to allocate memory for queues\n");
			igb_free_q_vectors(adapter);
			goto request_done;
		}
		igb_setup_all_tx_resources(adapter);
		igb_setup_all_rx_resources(adapter);
	}

	igb_assign_vector(adapter->q_vector[0], 0);

	if (adapter->flags & IGB_FLAG_HAS_MSI) {
		err = request_irq(pdev->irq, &igb_intr_msi, 0,
				  netdev->name, adapter);
		if (!err)
			goto request_done;

		/* fall back to legacy interrupts */
		igb_reset_interrupt_capability(adapter);
		adapter->flags &= ~IGB_FLAG_HAS_MSI;
	}

	err = request_irq(pdev->irq, &igb_intr, IRQF_SHARED,
			  netdev->name, adapter);

	if (err)
		dev_err(pci_dev_to_dev(pdev), "Error %d getting interrupt\n",
			err);

request_done:
	return err;
}

static void igb_free_irq(struct igb_adapter *adapter)
{
	if (adapter->msix_entries) {
		int vector = 0, i;

		free_irq(adapter->msix_entries[vector++].vector, adapter);

		for (i = 0; i < adapter->num_q_vectors; i++)
			free_irq(adapter->msix_entries[vector++].vector,
			         adapter->q_vector[i]);
	} else {
		free_irq(adapter->pdev->irq, adapter);
	}
}

/**
 * igb_irq_disable - Mask off interrupt generation on the NIC
 * @adapter: board private structure
 **/
static void igb_irq_disable(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;

	/*
	 * we need to be careful when disabling interrupts.  The VFs are also
	 * mapped into these registers and so clearing the bits can cause
	 * issues on the VF drivers so we only need to clear what we set
	 */
	if (adapter->msix_entries) {
		u32 regval = E1000_READ_REG(hw, E1000_EIAM);
		E1000_WRITE_REG(hw, E1000_EIAM, regval & ~adapter->eims_enable_mask);
		E1000_WRITE_REG(hw, E1000_EIMC, adapter->eims_enable_mask);
		regval = E1000_READ_REG(hw, E1000_EIAC);
		E1000_WRITE_REG(hw, E1000_EIAC, regval & ~adapter->eims_enable_mask);
	}

	E1000_WRITE_REG(hw, E1000_IAM, 0);
	E1000_WRITE_REG(hw, E1000_IMC, ~0);
	E1000_WRITE_FLUSH(hw);
	if (adapter->msix_entries) {
		int i;
		for (i = 0; i < adapter->num_q_vectors; i++)
			synchronize_irq(adapter->msix_entries[i].vector);
	} else {
		synchronize_irq(adapter->pdev->irq);
	}
}

/**
 * igb_irq_enable - Enable default interrupt generation settings
 * @adapter: board private structure
 **/
static void igb_irq_enable(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;

	if (adapter->msix_entries) {
		u32 ims = E1000_IMS_LSC | E1000_IMS_DOUTSYNC | E1000_IMS_DRSTA;
		u32 regval = E1000_READ_REG(hw, E1000_EIAC);
		E1000_WRITE_REG(hw, E1000_EIAC, regval | adapter->eims_enable_mask);
		regval = E1000_READ_REG(hw, E1000_EIAM);
		E1000_WRITE_REG(hw, E1000_EIAM, regval | adapter->eims_enable_mask);
		E1000_WRITE_REG(hw, E1000_EIMS, adapter->eims_enable_mask);
		if (adapter->vfs_allocated_count) {
			E1000_WRITE_REG(hw, E1000_MBVFIMR, 0xFF);
			ims |= E1000_IMS_VMMB;
		}
		E1000_WRITE_REG(hw, E1000_IMS, ims);
	} else {
		E1000_WRITE_REG(hw, E1000_IMS, IMS_ENABLE_MASK |
				E1000_IMS_DRSTA);
		E1000_WRITE_REG(hw, E1000_IAM, IMS_ENABLE_MASK |
				E1000_IMS_DRSTA);
	}
}

static void igb_update_mng_vlan(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u16 vid = adapter->hw.mng_cookie.vlan_id;
	u16 old_vid = adapter->mng_vlan_id;

	if (hw->mng_cookie.status & E1000_MNG_DHCP_COOKIE_STATUS_VLAN) {
		/* add VID to filter table */
		igb_vfta_set(hw, vid, TRUE);
		adapter->mng_vlan_id = vid;
	} else {
		adapter->mng_vlan_id = IGB_MNG_VLAN_NONE;
	}

	if ((old_vid != (u16)IGB_MNG_VLAN_NONE) &&
	    (vid != old_vid) &&
	    !vlan_group_get_device(adapter->vlgrp, old_vid)) {
		/* remove VID from filter table */
		igb_vfta_set(hw, old_vid, FALSE);
	}
}

/**
 * igb_release_hw_control - release control of the h/w to f/w
 * @adapter: address of board private structure
 *
 * igb_release_hw_control resets CTRL_EXT:DRV_LOAD bit.
 * For ASF and Pass Through versions of f/w this means that the
 * driver is no longer loaded.
 *
 **/
static void igb_release_hw_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl_ext;

	/* Let firmware take over control of h/w */
	ctrl_ext = E1000_READ_REG(hw, E1000_CTRL_EXT);
	E1000_WRITE_REG(hw, E1000_CTRL_EXT,
			ctrl_ext & ~E1000_CTRL_EXT_DRV_LOAD);
}

/**
 * igb_get_hw_control - get control of the h/w from f/w
 * @adapter: address of board private structure
 *
 * igb_get_hw_control sets CTRL_EXT:DRV_LOAD bit.
 * For ASF and Pass Through versions of f/w this means that
 * the driver is loaded.
 *
 **/
static void igb_get_hw_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl_ext;

	/* Let firmware know the driver has taken over */
	ctrl_ext = E1000_READ_REG(hw, E1000_CTRL_EXT);
	E1000_WRITE_REG(hw, E1000_CTRL_EXT,
			ctrl_ext | E1000_CTRL_EXT_DRV_LOAD);
}

/**
 * igb_configure - configure the hardware for RX and TX
 * @adapter: private board structure
 **/
static void igb_configure(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	int i;

	igb_get_hw_control(adapter);
	igb_set_rx_mode(netdev);

	igb_restore_vlan(adapter);

	igb_setup_tctl(adapter);
	igb_setup_mrqc(adapter);
	igb_setup_rctl(adapter);

	igb_configure_tx(adapter);
	igb_configure_rx(adapter);

	e1000_rx_fifo_flush_82575(&adapter->hw);
#ifdef CONFIG_NETDEVICES_MULTIQUEUE

	if (adapter->num_tx_queues > 1)
		netdev->features |= NETIF_F_MULTI_QUEUE;
	else
		netdev->features &= ~NETIF_F_MULTI_QUEUE;
#endif

	/* call igb_desc_unused which always leaves
	 * at least 1 descriptor unused to make sure
	 * next_to_use != next_to_clean */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *ring = adapter->rx_ring[i];
		igb_alloc_rx_buffers(ring, igb_desc_unused(ring));
	}
}

/**
 * igb_power_up_link - Power up the phy/serdes link
 * @adapter: address of board private structure
 **/
void igb_power_up_link(struct igb_adapter *adapter)
{
	if (adapter->hw.phy.media_type == e1000_media_type_copper)
		e1000_power_up_phy(&adapter->hw);
	else
		e1000_power_up_fiber_serdes_link(&adapter->hw);
}

/**
 * igb_power_down_link - Power down the phy/serdes link
 * @adapter: address of board private structure
 */
static void igb_power_down_link(struct igb_adapter *adapter)
{
	if (adapter->hw.phy.media_type == e1000_media_type_copper)
		e1000_power_down_phy(&adapter->hw);
	else
		e1000_shutdown_fiber_serdes_link(&adapter->hw);
}

/**
 * igb_up - Open the interface and prepare it to handle traffic
 * @adapter: board private structure
 **/
int igb_up(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	/* hardware has been reset, we need to reload some things */
	igb_configure(adapter);

	clear_bit(__IGB_DOWN, &adapter->state);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_enable(&(adapter->q_vector[i]->napi));

	if (adapter->msix_entries)
		igb_configure_msix(adapter);
	else
		igb_assign_vector(adapter->q_vector[0], 0);

	igb_configure_lli(adapter);

	/* Clear any pending interrupts. */
	E1000_READ_REG(hw, E1000_ICR);
	igb_irq_enable(adapter);

	/* notify VFs that reset has been completed */
	if (adapter->vfs_allocated_count) {
		u32 reg_data = E1000_READ_REG(hw, E1000_CTRL_EXT);
		reg_data |= E1000_CTRL_EXT_PFRSTD;
		E1000_WRITE_REG(hw, E1000_CTRL_EXT, reg_data);
	}

	netif_tx_start_all_queues(adapter->netdev);

	/* start the watchdog. */
	hw->mac.get_link_status = 1;
	schedule_work(&adapter->watchdog_task);

	return 0;
}

void igb_down(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	u32 tctl, rctl;
	int i;

	/* signal that we're down so the interrupt handler does not
	 * reschedule our watchdog timer */
	set_bit(__IGB_DOWN, &adapter->state);

	/* disable receives in the hardware */
	rctl = E1000_READ_REG(hw, E1000_RCTL);
	E1000_WRITE_REG(hw, E1000_RCTL, rctl & ~E1000_RCTL_EN);
	/* flush and sleep below */

	netif_tx_stop_all_queues(netdev);

	/* disable transmits in the hardware */
	tctl = E1000_READ_REG(hw, E1000_TCTL);
	tctl &= ~E1000_TCTL_EN;
	E1000_WRITE_REG(hw, E1000_TCTL, tctl);
	/* flush both disables and wait for them to finish */
	E1000_WRITE_FLUSH(hw);
	msleep(10);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_disable(&(adapter->q_vector[i]->napi));

	igb_irq_disable(adapter);

	del_timer_sync(&adapter->watchdog_timer);
	del_timer_sync(&adapter->phy_info_timer);

	netif_carrier_off(netdev);

	/* record the stats before reset*/
	igb_update_stats(adapter);

	adapter->link_speed = 0;
	adapter->link_duplex = 0;

#ifdef HAVE_PCI_ERS
	if (!pci_channel_offline(adapter->pdev))
		igb_reset(adapter);
#else
	igb_reset(adapter);
#endif
	igb_clean_all_tx_rings(adapter);
	igb_clean_all_rx_rings(adapter);
#ifdef IGB_DCA

	/* since we reset the hardware DCA settings were cleared */
	igb_setup_dca(adapter);
#endif
}

void igb_reinit_locked(struct igb_adapter *adapter)
{
	WARN_ON(in_interrupt());
	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		msleep(1);
	igb_down(adapter);
	igb_up(adapter);
	clear_bit(__IGB_RESETTING, &adapter->state);
}

void igb_reset(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;
	struct e1000_mac_info *mac = &hw->mac;
	struct e1000_fc_info *fc = &hw->fc;
	u32 pba = 0, tx_space, min_tx_space, min_rx_space;
	u16 hwm;

	/* Repartition Pba for greater than 9k mtu
	 * To take effect CTRL.RST is required.
	 */
	switch (mac->type) {
	case e1000_i350:
	case e1000_82580:
		pba = E1000_READ_REG(hw, E1000_RXPBS);
		pba = e1000_rxpbs_adjust_82580(pba);
		break;
	case e1000_82576:
		pba = E1000_READ_REG(hw, E1000_RXPBS);
		pba &= E1000_RXPBS_SIZE_MASK_82576;
		break;
	case e1000_82575:
	default:
		pba = E1000_PBA_34K;
		break;
	}

	if ((adapter->max_frame_size > ETH_FRAME_LEN + ETH_FCS_LEN) &&
	    (mac->type < e1000_82576)) {
		/* adjust PBA for jumbo frames */
		E1000_WRITE_REG(hw, E1000_PBA, pba);

		/* To maintain wire speed transmits, the Tx FIFO should be
		 * large enough to accommodate two full transmit packets,
		 * rounded up to the next 1KB and expressed in KB.  Likewise,
		 * the Rx FIFO should be large enough to accommodate at least
		 * one full receive packet and is similarly rounded up and
		 * expressed in KB. */
		pba = E1000_READ_REG(hw, E1000_PBA);
		/* upper 16 bits has Tx packet buffer allocation size in KB */
		tx_space = pba >> 16;
		/* lower 16 bits has Rx packet buffer allocation size in KB */
		pba &= 0xffff;
		/* the tx fifo also stores 16 bytes of information about the tx
		 * but don't include ethernet FCS because hardware appends it */
		min_tx_space = (adapter->max_frame_size +
				sizeof(union e1000_adv_tx_desc) -
				ETH_FCS_LEN) * 2;
		min_tx_space = ALIGN(min_tx_space, 1024);
		min_tx_space >>= 10;
		/* software strips receive CRC, so leave room for it */
		min_rx_space = adapter->max_frame_size;
		min_rx_space = ALIGN(min_rx_space, 1024);
		min_rx_space >>= 10;

		/* If current Tx allocation is less than the min Tx FIFO size,
		 * and the min Tx FIFO size is less than the current Rx FIFO
		 * allocation, take space away from current Rx allocation */
		if (tx_space < min_tx_space &&
		    ((min_tx_space - tx_space) < pba)) {
			pba = pba - (min_tx_space - tx_space);

			/* if short on rx space, rx wins and must trump tx
			 * adjustment */
			if (pba < min_rx_space)
				pba = min_rx_space;
		}
		E1000_WRITE_REG(hw, E1000_PBA, pba);
	}

	/* flow control settings */
	/* The high water mark must be low enough to fit one full frame
	 * (or the size used for early receive) above it in the Rx FIFO.
	 * Set it to the lower of:
	 * - 90% of the Rx FIFO size, or
	 * - the full Rx FIFO size minus one full frame */
	hwm = min(((pba << 10) * 9 / 10),
			((pba << 10) - 2 * adapter->max_frame_size));

	fc->high_water = hwm & 0xFFF0;	/* 16-byte granularity */
	fc->low_water = fc->high_water - 16;
	fc->pause_time = 0xFFFF;
	fc->send_xon = 1;
	fc->current_mode = fc->requested_mode;

	/* disable receive for all VFs and wait one second */
	if (adapter->vfs_allocated_count) {
		int i;
		/*
		 * Clear all flags except indication that the PF has set
		 * the VF MAC addresses administratively
		 */
		for (i = 0 ; i < adapter->vfs_allocated_count; i++)
			adapter->vf_data[i].flags &= IGB_VF_FLAG_PF_SET_MAC;

		/* ping all the active vfs to let them know we are going down */
		igb_ping_all_vfs(adapter);

		/* disable transmits and receives */
		E1000_WRITE_REG(hw, E1000_VFRE, 0);
		E1000_WRITE_REG(hw, E1000_VFTE, 0);
	}

	/* Allow time for pending master requests to run */
	e1000_reset_hw(hw);
	E1000_WRITE_REG(hw, E1000_WUC, 0);

	if (e1000_init_hw(hw))
		dev_err(pci_dev_to_dev(pdev), "Hardware Error\n");

	if (hw->mac.type > e1000_82580) {
		if (adapter->flags & IGB_FLAG_DMAC) {
			u32 reg;

			/*
			 * DMA Coalescing high water mark needs to be higher than the
			 * RX threshold.  The RX threshold is currently pba - 6, so we
			 * should use a high water mark of pba - 4.
			 */
			hwm = (pba - 4) << 10;

			reg = (((pba-6) << E1000_DMACR_DMACTHR_SHIFT)
			       & E1000_DMACR_DMACTHR_MASK);

			/* transition to L0x or L1 if available..*/
			reg |= (E1000_DMACR_DMAC_EN | E1000_DMACR_DMAC_LX_MASK);

			/* watchdog timer= +-1000 usec in 32usec intervals */
			reg |= (1000 >> 5);
			E1000_WRITE_REG(hw, E1000_DMACR, reg);

			/* no lower threshold to disable coalescing(smart fifb)-UTRESH=0*/
			E1000_WRITE_REG(hw, E1000_DMCRTRH, 0);

			/* set hwm to PBA -  2 * max frame size */
			E1000_WRITE_REG(hw, E1000_FCRTC, hwm);

			/*
			 * This sets the time to wait before requesting transition to
			 * low power state to number of usecs needed to receive 1 512
			 * byte frame at gigabit line rate
			 */
			reg = E1000_READ_REG(hw, E1000_DMCTLX);
			reg |= IGB_DMCTLX_DCFLUSH_DIS;

			
			/* Delay 255 usec before entering Lx state. */
			reg |= 0xFF;
			E1000_WRITE_REG(hw, E1000_DMCTLX, reg);

			/* free space in tx packet buffer to wake from DMA coal */
			E1000_WRITE_REG(hw, E1000_DMCTXTH,
		                (IGB_MIN_TXPBSIZE -
				(IGB_TX_BUF_4096 + adapter->max_frame_size)) >> 6);

			/* make low power state decision controlled by DMA coal */
			reg = E1000_READ_REG(hw, E1000_PCIEMISC);
			reg |= E1000_PCIEMISC_LX_DECISION;
			E1000_WRITE_REG(hw, E1000_PCIEMISC, reg);
		} /* end if IGB_FLAG_DMAC set */
	}
	if (!netif_running(adapter->netdev))
		igb_power_down_link(adapter);

	igb_update_mng_vlan(adapter);

	/* Enable h/w to recognize an 802.1Q VLAN Ethernet packet */
	E1000_WRITE_REG(hw, E1000_VET, ETHERNET_IEEE_VLAN_TYPE);

	e1000_get_phy_info(hw);
}

#ifdef HAVE_NET_DEVICE_OPS
static const struct net_device_ops igb_netdev_ops = {
	.ndo_open		= igb_open,
	.ndo_stop		= igb_close,
	.ndo_start_xmit		= igb_xmit_frame,
	.ndo_get_stats		= igb_get_stats,
	.ndo_set_rx_mode	= igb_set_rx_mode,
	.ndo_set_multicast_list	= igb_set_rx_mode,
	.ndo_set_mac_address	= igb_set_mac,
	.ndo_change_mtu		= igb_change_mtu,
	.ndo_do_ioctl		= igb_ioctl,
	.ndo_tx_timeout		= igb_tx_timeout,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_vlan_rx_register	= igb_vlan_rx_register,
	.ndo_vlan_rx_add_vid	= igb_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= igb_vlan_rx_kill_vid,
#ifdef IFLA_VF_MAX
	.ndo_set_vf_mac		= igb_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= igb_ndo_set_vf_vlan,
	.ndo_set_vf_tx_rate	= igb_ndo_set_vf_bw,
	.ndo_get_vf_config	= igb_ndo_get_vf_config,
#endif
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= igb_netpoll,
#endif
};
#ifdef CONFIG_IGB_VMDQ_NETDEV
static const struct net_device_ops igb_vmdq_ops = {
	.ndo_open		= &igb_vmdq_open,
	.ndo_stop		= &igb_vmdq_close,
	.ndo_start_xmit		= &igb_vmdq_xmit_frame,
	.ndo_get_stats		= &igb_vmdq_get_stats,
	.ndo_set_rx_mode	= &igb_vmdq_set_rx_mode,
	.ndo_set_multicast_list	= &igb_vmdq_set_rx_mode,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= &igb_vmdq_set_mac,
	.ndo_change_mtu		= &igb_vmdq_change_mtu,
	.ndo_tx_timeout		= &igb_vmdq_tx_timeout,
	.ndo_vlan_rx_register	= &igb_vmdq_vlan_rx_register,
	.ndo_vlan_rx_add_vid	= &igb_vmdq_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= &igb_vmdq_vlan_rx_kill_vid,
};
#endif /* CONFIG_IGB_VMDQ_NETDEV */

#endif /* HAVE_NET_DEVICE_OPS */

#ifdef CONFIG_IGB_VMDQ_NETDEV
void igb_assign_vmdq_netdev_ops(struct net_device *vnetdev)
{
#ifdef HAVE_NET_DEVICE_OPS
	vnetdev->netdev_ops = &igb_vmdq_ops;
#else
	dev->open = &igb_vmdq_open;
	dev->stop = &igb_vmdq_close;
	dev->hard_start_xmit = &igb_vmdq_xmit_frame;
	dev->get_stats = &igb_vmdq_get_stats;
#ifdef HAVE_SET_RX_MODE
	dev->set_rx_mode = &igb_vmdq_set_rx_mode;
#endif
	dev->set_multicast_list = &igb_vmdq_set_rx_mode;
	dev->set_mac_address = &igb_vmdq_set_mac;
	dev->change_mtu = &igb_vmdq_change_mtu;
#ifdef HAVE_TX_TIMEOUT
	dev->tx_timeout = &igb_vmdq_tx_timeout;
#endif
#ifdef NETIF_F_HW_VLAN_TX
	dev->vlan_rx_register = &igb_vmdq_vlan_rx_register;
	dev->vlan_rx_add_vid = &igb_vmdq_vlan_rx_add_vid;
	dev->vlan_rx_kill_vid = &igb_vmdq_vlan_rx_kill_vid;
#endif
#endif
	igb_vmdq_set_ethtool_ops(vnetdev);
	vnetdev->watchdog_timeo = 5 * HZ;

}

int igb_init_vmdq_netdevs(struct igb_adapter *adapter)
{
	int pool, err = 0, base_queue;
	struct net_device *vnetdev;
	struct igb_vmdq_adapter *vmdq_adapter;

	for (pool = 1; pool < adapter->vmdq_pools; pool++) {
		int qpp = (!adapter->rss_queues ? 1 : adapter->rss_queues);
		base_queue = pool * qpp;
		vnetdev = alloc_etherdev(sizeof(struct igb_vmdq_adapter));
		if (!vnetdev) {
			err = -ENOMEM;
			break;
		}
		vmdq_adapter = netdev_priv(vnetdev);
		vmdq_adapter->vnetdev = vnetdev;
		vmdq_adapter->real_adapter = adapter;
		vmdq_adapter->rx_ring = adapter->rx_ring[base_queue];
		vmdq_adapter->tx_ring = adapter->tx_ring[base_queue];
		igb_assign_vmdq_netdev_ops(vnetdev);
		snprintf(vnetdev->name, IFNAMSIZ, "%sv%d",
			 adapter->netdev->name, pool);
		vnetdev->features = adapter->netdev->features;
#ifdef HAVE_NETDEV_VLAN_FEATURES
		vnetdev->vlan_features = adapter->netdev->vlan_features;
#endif
		adapter->vmdq_netdev[pool-1] = vnetdev;
		err = register_netdev(vnetdev);
		if (err)
			break;
	}
	return err;
}

int igb_remove_vmdq_netdevs(struct igb_adapter *adapter)
{
	int pool, err = 0;

	for (pool = 1; pool < adapter->vmdq_pools; pool++) {
		unregister_netdev(adapter->vmdq_netdev[pool-1]);
		free_netdev(adapter->vmdq_netdev[pool-1]);
		adapter->vmdq_netdev[pool-1] = NULL;
	}
	return err;
}
#endif /* CONFIG_IGB_VMDQ_NETDEV */

/**
 * igb_probe - Device Initialization Routine
 * @pdev: PCI device information struct
 * @ent: entry in igb_pci_tbl
 *
 * Returns 0 on success, negative on failure
 *
 * igb_probe initializes an adapter identified by a pci_dev structure.
 * The OS initialization, configuring of the adapter private structure,
 * and a hardware reset occur.
 **/
static int __devinit igb_probe(struct pci_dev *pdev,
			       const struct pci_device_id *ent)
{
	struct net_device *netdev;
	struct igb_adapter *adapter;
	struct e1000_hw *hw;
	u16 eeprom_data = 0;
	u8 pba_str[E1000_PBANUM_LENGTH];
	s32 ret_val;
	static int global_quad_port_a; /* global quad port a indication */
	int i, err, pci_using_dac;
	static int cards_found;

	err = pci_enable_device_mem(pdev);
	if (err)
		return err;

	pci_using_dac = 0;
	err = dma_set_mask(pci_dev_to_dev(pdev), DMA_BIT_MASK(64));
	if (!err) {
		err = dma_set_coherent_mask(pci_dev_to_dev(pdev), DMA_BIT_MASK(64));
		if (!err)
			pci_using_dac = 1;
	} else {
		err = dma_set_mask(pci_dev_to_dev(pdev), DMA_BIT_MASK(32));
		if (err) {
			err = dma_set_coherent_mask(pci_dev_to_dev(pdev), DMA_BIT_MASK(32));
			if (err) {
				IGB_ERR("No usable DMA configuration, "
				        "aborting\n");
				goto err_dma;
			}
		}
	}

#ifndef HAVE_ASPM_QUIRKS
	/* 82575 requires that the pci-e link partner disable the L0s state */
	switch (pdev->device) {
	case E1000_DEV_ID_82575EB_COPPER:
	case E1000_DEV_ID_82575EB_FIBER_SERDES:
	case E1000_DEV_ID_82575GB_QUAD_COPPER:
		pci_disable_link_state(pdev, PCIE_LINK_STATE_L0S);
	default:
		break;
	}

#endif /* HAVE_ASPM_QUIRKS */
	err = pci_request_selected_regions(pdev,
	                                   pci_select_bars(pdev,
                                                           IORESOURCE_MEM),
	                                   igb_driver_name);
	if (err)
		goto err_pci_reg;

	pci_enable_pcie_error_reporting(pdev);

	pci_set_master(pdev);

	err = -ENOMEM;
#ifdef HAVE_TX_MQ
	netdev = alloc_etherdev_mq(sizeof(struct igb_adapter),
	                           IGB_MAX_TX_QUEUES);
#else
	netdev = alloc_etherdev(sizeof(struct igb_adapter));
#endif /* HAVE_TX_MQ */
	if (!netdev)
		goto err_alloc_etherdev;

	SET_MODULE_OWNER(netdev);
	SET_NETDEV_DEV(netdev, &pdev->dev);

	pci_set_drvdata(pdev, netdev);
	adapter = netdev_priv(netdev);
	adapter->netdev = netdev;
	adapter->pdev = pdev;
	hw = &adapter->hw;
	hw->back = adapter;
	adapter->msg_enable = (1 << debug) - 1;

#ifdef HAVE_PCI_ERS
	err = pci_save_state(pdev);
	if (err)
		goto err_ioremap;
#endif
	err = -EIO;
	hw->hw_addr = ioremap(pci_resource_start(pdev, 0),
	                      pci_resource_len(pdev, 0));
	if (!hw->hw_addr)
		goto err_ioremap;

#ifdef HAVE_NET_DEVICE_OPS
	netdev->netdev_ops = &igb_netdev_ops;
#else /* HAVE_NET_DEVICE_OPS */
	netdev->open = &igb_open;
	netdev->stop = &igb_close;
	netdev->get_stats = &igb_get_stats;
#ifdef HAVE_SET_RX_MODE
	netdev->set_rx_mode = &igb_set_rx_mode;
#endif
	netdev->set_multicast_list = &igb_set_rx_mode;
	netdev->set_mac_address = &igb_set_mac;
	netdev->change_mtu = &igb_change_mtu;
	netdev->do_ioctl = &igb_ioctl;
#ifdef HAVE_TX_TIMEOUT
	netdev->tx_timeout = &igb_tx_timeout;
#endif
	netdev->vlan_rx_register = igb_vlan_rx_register;
	netdev->vlan_rx_add_vid = igb_vlan_rx_add_vid;
	netdev->vlan_rx_kill_vid = igb_vlan_rx_kill_vid;
#ifdef CONFIG_NET_POLL_CONTROLLER
	netdev->poll_controller = igb_netpoll;
#endif
	netdev->hard_start_xmit = &igb_xmit_frame;
#endif /* HAVE_NET_DEVICE_OPS */
	igb_set_ethtool_ops(netdev);
#ifdef HAVE_TX_TIMEOUT
	netdev->watchdog_timeo = 5 * HZ;
#endif

	strncpy(netdev->name, pci_name(pdev), sizeof(netdev->name) - 1);

	adapter->bd_number = cards_found;

	/* setup the private structure */
	err = igb_sw_init(adapter);
	if (err)
		goto err_sw_init;

	e1000_get_bus_info(hw);

	hw->phy.autoneg_wait_to_complete = FALSE;
	hw->mac.adaptive_ifs = FALSE;

	/* Copper options */
	if (hw->phy.media_type == e1000_media_type_copper) {
#ifdef ETH_TP_MDI_X
		hw->phy.mdix = ETH_TP_MDI_INVALID;
#else
		hw->phy.mdix = AUTO_ALL_MODES;
#endif /* ETH_TP_MDI_X */
		hw->phy.disable_polarity_correction = FALSE;
		hw->phy.ms_type = e1000_ms_hw_default;
	}

	if (e1000_check_reset_block(hw))
		dev_info(pci_dev_to_dev(pdev),
			"PHY reset is blocked due to SOL/IDER session.\n");

	netdev->features = NETIF_F_SG |
			   NETIF_F_IP_CSUM |
			   NETIF_F_HW_VLAN_TX |
			   NETIF_F_HW_VLAN_RX |
			   NETIF_F_HW_VLAN_FILTER;

#ifdef NETIF_F_IPV6_CSUM
	netdev->features |= NETIF_F_IPV6_CSUM;
#endif
#ifdef NETIF_F_TSO
	netdev->features |= NETIF_F_TSO;
#ifdef NETIF_F_TSO6
	netdev->features |= NETIF_F_TSO6;
#endif
#endif /* NETIF_F_TSO */
#ifdef IGB_LRO
	netdev->features |= NETIF_F_LRO;

#endif
#ifdef NETIF_F_GRO
	netdev->features |= NETIF_F_GRO;
#endif

#ifdef HAVE_NETDEV_VLAN_FEATURES
	netdev->vlan_features |= NETIF_F_TSO;
	netdev->vlan_features |= NETIF_F_TSO6;
	netdev->vlan_features |= NETIF_F_IP_CSUM;
	netdev->vlan_features |= NETIF_F_IPV6_CSUM;
	netdev->vlan_features |= NETIF_F_SG;

#endif
	if (pci_using_dac)
		netdev->features |= NETIF_F_HIGHDMA;

	if (hw->mac.type >= e1000_82576)
		netdev->features |= NETIF_F_SCTP_CSUM;

	adapter->en_mng_pt = e1000_enable_mng_pass_thru(hw);

	/* before reading the NVM, reset the controller to put the device in a
	 * known good starting state */
	e1000_reset_hw(hw);

	/* make sure the NVM is good */
	if (e1000_validate_nvm_checksum(hw) < 0) {
		dev_err(pci_dev_to_dev(pdev), "The NVM Checksum Is Not"
		        " Valid\n");
		err = -EIO;
		goto err_eeprom;
	}

	/* copy the MAC address out of the NVM */
	if (e1000_read_mac_addr(hw))
		dev_err(pci_dev_to_dev(pdev), "NVM Read Error\n");
	memcpy(netdev->dev_addr, hw->mac.addr, netdev->addr_len);
#ifdef ETHTOOL_GPERMADDR
	memcpy(netdev->perm_addr, hw->mac.addr, netdev->addr_len);

	if (!is_valid_ether_addr(netdev->perm_addr)) {
#else
	if (!is_valid_ether_addr(netdev->dev_addr)) {
#endif
		dev_err(pci_dev_to_dev(pdev), "Invalid MAC Address\n");
		err = -EIO;
		goto err_eeprom;
	}

	memcpy(&adapter->mac_table[0].addr, hw->mac.addr, netdev->addr_len);
	adapter->mac_table[0].queue = adapter->vfs_allocated_count;
	adapter->mac_table[0].state = (IGB_MAC_STATE_DEFAULT | IGB_MAC_STATE_IN_USE);
	igb_rar_set(adapter, 0);

	/* get firmware version for ethtool -i */
	e1000_read_nvm(&adapter->hw, 5, 1, &adapter->fw_version);
	setup_timer(&adapter->watchdog_timer, &igb_watchdog,
	            (unsigned long) adapter);
	setup_timer(&adapter->phy_info_timer, &igb_update_phy_info,
	            (unsigned long) adapter);

	INIT_WORK(&adapter->reset_task, igb_reset_task);
	INIT_WORK(&adapter->watchdog_task, igb_watchdog_task);

	/* Initialize link properties that are user-changeable */
	adapter->fc_autoneg = true;
	hw->mac.autoneg = true;
	hw->phy.autoneg_advertised = 0x2f;

	hw->fc.requested_mode = e1000_fc_default;
	hw->fc.current_mode = e1000_fc_default;

	e1000_validate_mdi_setting(hw);

	/* Initial Wake on LAN setting If APM wake is enabled in the EEPROM,
	 * enable the ACPI Magic Packet filter
	 */

	if (hw->bus.func == 0)
		e1000_read_nvm(hw, NVM_INIT_CONTROL3_PORT_A, 1, &eeprom_data);
	else if (hw->mac.type >= e1000_82580)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_A +
		                 NVM_82580_LAN_FUNC_OFFSET(hw->bus.func), 1,
		                 &eeprom_data);
	else if (hw->bus.func == 1)
		e1000_read_nvm(hw, NVM_INIT_CONTROL3_PORT_B, 1, &eeprom_data);

	if (eeprom_data & IGB_EEPROM_APME)
		adapter->eeprom_wol |= E1000_WUFC_MAG;

	/* now that we have the eeprom settings, apply the special cases where
	 * the eeprom may be wrong or the board simply won't support wake on
	 * lan on a particular port */
	switch (pdev->device) {
	case E1000_DEV_ID_82575GB_QUAD_COPPER:
		adapter->eeprom_wol = 0;
		break;
	case E1000_DEV_ID_82575EB_FIBER_SERDES:
	case E1000_DEV_ID_82576_FIBER:
	case E1000_DEV_ID_82576_SERDES:
		/* Wake events only supported on port A for dual fiber
		 * regardless of eeprom setting */
		if (E1000_READ_REG(hw, E1000_STATUS) & E1000_STATUS_FUNC_1)
			adapter->eeprom_wol = 0;
		break;
	case E1000_DEV_ID_82576_QUAD_COPPER:
	case E1000_DEV_ID_82576_QUAD_COPPER_ET2:
		/* if quad port adapter, disable WoL on all but port A */
		if (global_quad_port_a != 0)
			adapter->eeprom_wol = 0;
		else
			adapter->flags |= IGB_FLAG_QUAD_PORT_A;
		/* Reset for multiple quad port adapters */
		if (++global_quad_port_a == 4)
			global_quad_port_a = 0;
		break;
	}

	/* initialize the wol settings based on the eeprom settings */
	adapter->wol = adapter->eeprom_wol;
	device_set_wakeup_enable(pci_dev_to_dev(adapter->pdev), adapter->wol);

	/* reset the hardware with the new settings */
	igb_reset(adapter);

	/* let the f/w know that the h/w is now under the control of the
	 * driver. */
	igb_get_hw_control(adapter);

	strncpy(netdev->name, "eth%d", IFNAMSIZ);
	err = register_netdev(netdev);
	if (err)
		goto err_register;

#ifdef CONFIG_IGB_VMDQ_NETDEV
	err = igb_init_vmdq_netdevs(adapter);
	if (err)
		goto err_register;
#endif
	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(netdev);

#ifdef IGB_DCA
	if (dca_add_requester(&pdev->dev) == E1000_SUCCESS) {
		adapter->flags |= IGB_FLAG_DCA_ENABLED;
		dev_info(pci_dev_to_dev(pdev), "DCA enabled\n");
		igb_setup_dca(adapter);
	}

#endif
#ifdef HAVE_HW_TIME_STAMP
	switch (hw->mac.type) {
	case e1000_i350:
	case e1000_82580:
		memset(&adapter->cycles, 0, sizeof(adapter->cycles));
		adapter->cycles.read = igb_read_clock;
		adapter->cycles.mask = CLOCKSOURCE_MASK(64);
		adapter->cycles.mult = 1;
		/*
		 * The 82580 timesync updates the system timer every 8ns by 8ns
		 * and the value cannot be shifted.  Instead we need to shift
		 * the registers to generate a 64bit timer value.  As a result
		 * SYSTIMR/L/H, TXSTMPL/H, RXSTMPL/H all have to be shifted by
		 * 24 in order to generate a larger value for synchronization.
		 */
		adapter->cycles.shift = IGB_82580_TSYNC_SHIFT;
		/* disable system timer temporarily by setting bit 31 */
		E1000_WRITE_REG(hw, E1000_TSAUXC, 0x80000000);
		E1000_WRITE_FLUSH(hw);

		/* Set registers so that rollover occurs soon to test this. */
		E1000_WRITE_REG(hw, E1000_SYSTIMR, 0x00000000);
		E1000_WRITE_REG(hw, E1000_SYSTIML, 0x80000000);
		E1000_WRITE_REG(hw, E1000_SYSTIMH, 0x000000FF);
		E1000_WRITE_FLUSH(hw);

		/* enable system timer by clearing bit 31 */
		E1000_WRITE_REG(hw, E1000_TSAUXC, 0x0);
		E1000_WRITE_FLUSH(hw);

		timecounter_init(&adapter->clock,
				 &adapter->cycles,
				 ktime_to_ns(ktime_get_real()));
		/*
		 * Synchronize our NIC clock against system wall clock. NIC
		 * time stamp reading requires ~3us per sample, each sample
		 * was pretty stable even under load => only require 10
		 * samples for each offset comparison.
		 */
		memset(&adapter->compare, 0, sizeof(adapter->compare));
		adapter->compare.source = &adapter->clock;
		adapter->compare.target = ktime_get_real;
		adapter->compare.num_samples = 10;
		timecompare_update(&adapter->compare, 0);
		break;
	case e1000_82576:
		/*
		 * Initialize hardware timer: we keep it running just in case
		 * that some program needs it later on.
		 */
		memset(&adapter->cycles, 0, sizeof(adapter->cycles));
		adapter->cycles.read = igb_read_clock;
		adapter->cycles.mask = CLOCKSOURCE_MASK(64);
		adapter->cycles.mult = 1;
		/**
		 * Scale the NIC clock cycle by a large factor so that
		 * relatively small clock corrections can be added or
		 * substracted at each clock tick. The drawbacks of a large
		 * factor are a) that the clock register overflows more quickly
		 * (not such a big deal) and b) that the increment per tick has
		 * to fit into 24 bits.  As a result we need to use a shift of
		 * 19 so we can fit a value of 16 into the TIMINCA register.
		 */
		adapter->cycles.shift = IGB_82576_TSYNC_SHIFT;
		E1000_WRITE_REG(hw, E1000_TIMINCA,
		                (1 << E1000_TIMINCA_16NS_SHIFT) |
		                (16 << IGB_82576_TSYNC_SHIFT));

		/* Set registers so that rollover occurs soon to test this. */
		E1000_WRITE_REG(hw, E1000_SYSTIML, 0x00000000);
		E1000_WRITE_REG(hw, E1000_SYSTIMH, 0xFF800000);
		E1000_WRITE_FLUSH(hw);

		timecounter_init(&adapter->clock,
				 &adapter->cycles,
				 ktime_to_ns(ktime_get_real()));
		/*
		 * Synchronize our NIC clock against system wall clock. NIC
		 * time stamp reading requires ~3us per sample, each sample
		 * was pretty stable even under load => only require 10
		 * samples for each offset comparison.
		 */
		memset(&adapter->compare, 0, sizeof(adapter->compare));
		adapter->compare.source = &adapter->clock;
		adapter->compare.target = ktime_get_real;
		adapter->compare.num_samples = 10;
		timecompare_update(&adapter->compare, 0);
		break;
	case e1000_82575:
		/* 82575 does not support timesync */
	default:
		break;
	}

#endif /* SIOCSHWTSTAMP */
	dev_info(pci_dev_to_dev(pdev), "Intel(R) Gigabit Ethernet Network Connection\n");
	/* print bus type/speed/width info */
	dev_info(pci_dev_to_dev(pdev), "%s: (PCIe:%s:%s) ",
	         netdev->name,
	         ((hw->bus.speed == e1000_bus_speed_2500) ? "2.5GT/s" :
	          (hw->bus.speed == e1000_bus_speed_5000) ? "5.0GT/s" :
	                                                    "unknown"),
	         ((hw->bus.width == e1000_bus_width_pcie_x4) ? "Width x4" :
	          (hw->bus.width == e1000_bus_width_pcie_x2) ? "Width x2" :
	          (hw->bus.width == e1000_bus_width_pcie_x1) ? "Width x1" :
	           "unknown"));

	for (i = 0; i < 6; i++)
		printk("%2.2x%c", netdev->dev_addr[i], i == 5 ? '\n' : ':');

	ret_val = e1000_read_pba_string(hw, pba_str, E1000_PBANUM_LENGTH);
	if (ret_val)
		strcpy(pba_str, "Unknown");
	dev_info(pci_dev_to_dev(pdev), "%s: PBA No: %s\n", netdev->name,
		 pba_str);


	switch (hw->mac.type) {
	case e1000_i350:
		/* Enable EEE */
		e1000_set_eee_i350(hw);

		/* send driver version info to firmware */
		igb_init_fw(adapter);
		break;
	default:
		break;
	}
#ifdef IGB_LRO
	if (test_bit(IGB_RING_FLAG_RX_LRO, &adapter->rx_ring[0]->flags))
		dev_info(pci_dev_to_dev(pdev), "Internal LRO is enabled \n");
	else
		dev_info(pci_dev_to_dev(pdev), "LRO is disabled \n");
#endif
	dev_info(pci_dev_to_dev(pdev),
	         "Using %s interrupts. %d rx queue(s), %d tx queue(s)\n",
	         adapter->msix_entries ? "MSI-X" :
	         (adapter->flags & IGB_FLAG_HAS_MSI) ? "MSI" : "legacy",
	         adapter->num_rx_queues, adapter->num_tx_queues);

	cards_found++;
	return 0;

err_register:
	igb_release_hw_control(adapter);
err_eeprom:
	if (!e1000_check_reset_block(hw))
		e1000_phy_hw_reset(hw);

	if (hw->flash_address)
		iounmap(hw->flash_address);
err_sw_init:
	igb_clear_interrupt_scheme(adapter);
	igb_reset_sriov_capability(adapter);
	iounmap(hw->hw_addr);
err_ioremap:
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_selected_regions(pdev,
	                             pci_select_bars(pdev, IORESOURCE_MEM));
err_pci_reg:
err_dma:
	pci_disable_device(pdev);
	return err;
}

/**
 * igb_remove - Device Removal Routine
 * @pdev: PCI device information struct
 *
 * igb_remove is called by the PCI subsystem to alert the driver
 * that it should release a PCI device.  The could be caused by a
 * Hot-Plug event, or because the driver is going to be removed from
 * memory.
 **/
static void __devexit igb_remove(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;

	/* flush_scheduled work may reschedule our watchdog task, so
	 * explicitly disable watchdog tasks from being rescheduled  */
	set_bit(__IGB_DOWN, &adapter->state);
	del_timer_sync(&adapter->watchdog_timer);
	del_timer_sync(&adapter->phy_info_timer);

	flush_scheduled_work();

#ifdef IGB_DCA
	if (adapter->flags & IGB_FLAG_DCA_ENABLED) {
		dev_info(pci_dev_to_dev(pdev), "DCA disabled\n");
		dca_remove_requester(&pdev->dev);
		adapter->flags &= ~IGB_FLAG_DCA_ENABLED;
		E1000_WRITE_REG(hw, E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_DISABLE);
	}
#endif

	/* Release control of h/w to f/w.  If f/w is AMT enabled, this
	 * would have already happened in close and is redundant. */
	igb_release_hw_control(adapter);

	unregister_netdev(netdev);
#ifdef CONFIG_IGB_VMDQ_NETDEV
	igb_remove_vmdq_netdevs(adapter);
#endif

	igb_clear_interrupt_scheme(adapter);
	igb_reset_sriov_capability(adapter);

	iounmap(hw->hw_addr);
	if (hw->flash_address)
		iounmap(hw->flash_address);
	pci_release_selected_regions(pdev,
	                             pci_select_bars(pdev, IORESOURCE_MEM));

	kfree(adapter->mac_table);
	free_netdev(netdev);

	pci_disable_pcie_error_reporting(pdev);

	pci_disable_device(pdev);
}

/**
 * igb_sw_init - Initialize general software structures (struct igb_adapter)
 * @adapter: board private structure to initialize
 *
 * igb_sw_init initializes the Adapter private data structure.
 * Fields are initialized based on PCI device information and
 * OS network device settings (MTU size).
 **/
static int __devinit igb_sw_init(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;

	/* PCI config space info */

	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;

	pci_read_config_byte(pdev, PCI_REVISION_ID, &hw->revision_id);

	pci_read_config_word(pdev, PCI_COMMAND, &hw->bus.pci_cmd_word);

	adapter->tx_ring_count = IGB_DEFAULT_TXD;
	adapter->rx_ring_count = IGB_DEFAULT_RXD;
	adapter->max_frame_size = netdev->mtu + ETH_HLEN + ETH_FCS_LEN +
					      VLAN_TAG_SIZE;

	/* Initialize the hardware-specific values */
	if (e1000_setup_init_funcs(hw, TRUE)) {
		dev_err(pci_dev_to_dev(pdev), "Hardware Initialization Failure\n");
		return -EIO;
	}

	igb_check_options(adapter);

	adapter->mac_table = kzalloc(sizeof(struct igb_mac_addr) *
				     hw->mac.rar_entry_count, 
				     GFP_ATOMIC);

	/* These calls may decrease the number of queues */
	igb_set_sriov_capability(adapter);

	if (igb_init_interrupt_scheme(adapter)) {
		dev_err(pci_dev_to_dev(pdev), "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

	/* Explicitly disable IRQ since the NIC can be in any state. */
	igb_irq_disable(adapter);

	set_bit(__IGB_DOWN, &adapter->state);
	return 0;
}

/**
 * igb_open - Called when a network interface is made active
 * @netdev: network interface device structure
 *
 * Returns 0 on success, negative value on failure
 *
 * The open entry point is called when a network interface is made
 * active by the system (IFF_UP).  At this point all resources needed
 * for transmit and receive operations are allocated, the interrupt
 * handler is registered with the OS, the watchdog timer is started,
 * and the stack is notified that the interface is ready.
 **/
static int igb_open(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int err;
	int i;

	/* disallow open during test */
	if (test_bit(__IGB_TESTING, &adapter->state))
		return -EBUSY;

	netif_carrier_off(netdev);

	/* allocate transmit descriptors */
	err = igb_setup_all_tx_resources(adapter);
	if (err)
		goto err_setup_tx;

	/* allocate receive descriptors */
	err = igb_setup_all_rx_resources(adapter);
	if (err)
		goto err_setup_rx;

	igb_power_up_link(adapter);

	/* before we allocate an interrupt, we must be ready to handle it.
	 * Setting DEBUG_SHIRQ in the kernel makes it fire an interrupt
	 * as soon as we call pci_request_irq, so we have to setup our
	 * clean_rx handler before we do so.  */
	igb_configure(adapter);

	err = igb_request_irq(adapter);
	if (err)
		goto err_req_irq;

	/* From here on the code is the same as igb_up() */
	clear_bit(__IGB_DOWN, &adapter->state);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_enable(&(adapter->q_vector[i]->napi));
	igb_configure_lli(adapter);

	/* Clear any pending interrupts. */
	E1000_READ_REG(hw, E1000_ICR);

	igb_irq_enable(adapter);

	/* notify VFs that reset has been completed */
	if (adapter->vfs_allocated_count) {
		u32 reg_data = E1000_READ_REG(hw, E1000_CTRL_EXT);
		reg_data |= E1000_CTRL_EXT_PFRSTD;
		E1000_WRITE_REG(hw, E1000_CTRL_EXT, reg_data);
	}

	netif_tx_start_all_queues(netdev);

	/* start the watchdog. */
	hw->mac.get_link_status = 1;
	schedule_work(&adapter->watchdog_task);

	return E1000_SUCCESS;

err_req_irq:
	igb_release_hw_control(adapter);
	igb_power_down_link(adapter);
	igb_free_all_rx_resources(adapter);
err_setup_rx:
	igb_free_all_tx_resources(adapter);
err_setup_tx:
	igb_reset(adapter);

	return err;
}

/**
 * igb_close - Disables a network interface
 * @netdev: network interface device structure
 *
 * Returns 0, this is not allowed to fail
 *
 * The close entry point is called when an interface is de-activated
 * by the OS.  The hardware is still under the driver's control, but
 * needs to be disabled.  A global MAC reset is issued to stop the
 * hardware, and all transmit and receive resources are freed.
 **/
static int igb_close(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	WARN_ON(test_bit(__IGB_RESETTING, &adapter->state));
	igb_down(adapter);

	igb_free_irq(adapter);

	igb_free_all_tx_resources(adapter);
	igb_free_all_rx_resources(adapter);

	return 0;
}

/**
 * igb_setup_tx_resources - allocate Tx resources (Descriptors)
 * @tx_ring: tx descriptor ring (for a specific queue) to setup
 *
 * Return 0 on success, negative on failure
 **/
int igb_setup_tx_resources(struct igb_ring *tx_ring)
{
	struct device *dev = tx_ring->dev;
	int orig_node = dev_to_node(dev);
	int size;

	size = sizeof(struct igb_buffer) * tx_ring->count;
	tx_ring->buffer_info = vzalloc_node(size, tx_ring->numa_node);
	if (!tx_ring->buffer_info)
		tx_ring->buffer_info = vzalloc(size);
	if (!tx_ring->buffer_info)
		goto err;

	/* round up to nearest 4K */
	tx_ring->size = tx_ring->count * sizeof(union e1000_adv_tx_desc);
	tx_ring->size = ALIGN(tx_ring->size, 4096);

	set_dev_node(dev, tx_ring->numa_node);
	tx_ring->desc = dma_alloc_coherent(dev,
					   tx_ring->size,
					   &tx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!tx_ring->desc)
		tx_ring->desc = dma_alloc_coherent(dev,
						   tx_ring->size,
						   &tx_ring->dma,
						   GFP_KERNEL);

	if (!tx_ring->desc)
		goto err;

	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;

	return 0;

err:
	vfree(tx_ring->buffer_info);
	dev_err(dev,
		"Unable to allocate memory for the transmit descriptor ring\n");
	return -ENOMEM;
}

/**
 * igb_setup_all_tx_resources - wrapper to allocate Tx resources
 *				  (Descriptors) for all queues
 * @adapter: board private structure
 *
 * Return 0 on success, negative on failure
 **/
static int igb_setup_all_tx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		err = igb_setup_tx_resources(adapter->tx_ring[i]);
		if (err) {
			dev_err(pci_dev_to_dev(pdev),
				"Allocation for Tx Queue %u failed\n", i);
			for (i--; i >= 0; i--)
				igb_free_tx_resources(adapter->tx_ring[i]);
			break;
		}
	}

	return err;
}

/**
 * igb_setup_tctl - configure the transmit control registers
 * @adapter: Board private structure
 **/
void igb_setup_tctl(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 tctl;

	/* disable queue 0 which is enabled by default on 82575 and 82576 */
	E1000_WRITE_REG(hw, E1000_TXDCTL(0), 0);

	/* Program the Transmit Control Register */
	tctl = E1000_READ_REG(hw, E1000_TCTL);
	tctl &= ~E1000_TCTL_CT;
	tctl |= E1000_TCTL_PSP | E1000_TCTL_RTLC |
		(E1000_COLLISION_THRESHOLD << E1000_CT_SHIFT);

	e1000_config_collision_dist(hw);

	/* Enable transmits */
	tctl |= E1000_TCTL_EN;

	E1000_WRITE_REG(hw, E1000_TCTL, tctl);
}

/**
 * igb_configure_tx_ring - Configure transmit ring after Reset
 * @adapter: board private structure
 * @ring: tx ring to configure
 *
 * Configure a transmit ring after a reset.
 **/
void igb_configure_tx_ring(struct igb_adapter *adapter,
                           struct igb_ring *ring)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 txdctl = 0;
	u64 tdba = ring->dma;
	int reg_idx = ring->reg_idx;

	/* disable the queue */
	E1000_WRITE_REG(hw, E1000_TXDCTL(reg_idx), 0);
	E1000_WRITE_FLUSH(hw);
	mdelay(10);

	E1000_WRITE_REG(hw, E1000_TDLEN(reg_idx),
	                ring->count * sizeof(union e1000_adv_tx_desc));
	E1000_WRITE_REG(hw, E1000_TDBAL(reg_idx),
	                tdba & 0x00000000ffffffffULL);
	E1000_WRITE_REG(hw, E1000_TDBAH(reg_idx), tdba >> 32);

	ring->tail = hw->hw_addr + E1000_TDT(reg_idx);
	E1000_WRITE_REG(hw, E1000_TDH(reg_idx), 0);
	writel(0, ring->tail);

	txdctl |= IGB_TX_PTHRESH;
	txdctl |= IGB_TX_HTHRESH << 8;
	txdctl |= IGB_TX_WTHRESH << 16;

	txdctl |= E1000_TXDCTL_QUEUE_ENABLE;
	E1000_WRITE_REG(hw, E1000_TXDCTL(reg_idx), txdctl);
}

/**
 * igb_configure_tx - Configure transmit Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Tx unit of the MAC after a reset.
 **/
static void igb_configure_tx(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
		igb_configure_tx_ring(adapter, adapter->tx_ring[i]);
}

/**
 * igb_setup_rx_resources - allocate Rx resources (Descriptors)
 * @rx_ring:    rx descriptor ring (for a specific queue) to setup
 *
 * Returns 0 on success, negative on failure
 **/
int igb_setup_rx_resources(struct igb_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int orig_node = dev_to_node(dev);
	int size, desc_len;

	size = sizeof(struct igb_buffer) * rx_ring->count;
	rx_ring->buffer_info = vzalloc_node(size, rx_ring->numa_node);
	if (!rx_ring->buffer_info)
		rx_ring->buffer_info = vzalloc(size);
	if (!rx_ring->buffer_info)
		goto err;

	desc_len = sizeof(union e1000_adv_rx_desc);

	/* Round up to nearest 4K */
	rx_ring->size = rx_ring->count * desc_len;
	rx_ring->size = ALIGN(rx_ring->size, 4096);

	set_dev_node(dev, rx_ring->numa_node);
	rx_ring->desc = dma_alloc_coherent(dev,
					   rx_ring->size,
					   &rx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!rx_ring->desc)
		rx_ring->desc = dma_alloc_coherent(dev,
						   rx_ring->size,
						   &rx_ring->dma,
						   GFP_KERNEL);

	if (!rx_ring->desc)
		goto err;

	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;

	return 0;

err:
	vfree(rx_ring->buffer_info);
	rx_ring->buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the receive descriptor"
		" ring\n");
	return -ENOMEM;
}

/**
 * igb_setup_all_rx_resources - wrapper to allocate Rx resources
 *				  (Descriptors) for all queues
 * @adapter: board private structure
 *
 * Return 0 on success, negative on failure
 **/
static int igb_setup_all_rx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_rx_queues; i++) {
		err = igb_setup_rx_resources(adapter->rx_ring[i]);
		if (err) {
			dev_err(pci_dev_to_dev(pdev),
				"Allocation for Rx Queue %u failed\n", i);
			for (i--; i >= 0; i--)
				igb_free_rx_resources(adapter->rx_ring[i]);
			break;
		}
	}

	return err;
}

/**
 * igb_setup_mrqc - configure the multiple receive queue control registers
 * @adapter: Board private structure
 **/
static void igb_setup_mrqc(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 mrqc, rxcsum;
	u32 j, num_rx_queues, shift = 0, shift2 = 0;
	union e1000_reta {
		u32 dword;
		u8  bytes[4];
	} reta;
	static const u8 rsshash[40] = {
		0x6d, 0x5a, 0x56, 0xda, 0x25, 0x5b, 0x0e, 0xc2, 0x41, 0x67,
		0x25, 0x3d, 0x43, 0xa3, 0x8f, 0xb0, 0xd0, 0xca, 0x2b, 0xcb,
		0xae, 0x7b, 0x30, 0xb4,	0x77, 0xcb, 0x2d, 0xa3, 0x80, 0x30,
		0xf2, 0x0c, 0x6a, 0x42, 0xb7, 0x3b, 0xbe, 0xac, 0x01, 0xfa };

	/* Fill out hash function seeds */
	for (j = 0; j < 10; j++) {
		u32 rsskey = rsshash[(j * 4)];
		rsskey |= rsshash[(j * 4) + 1] << 8;
		rsskey |= rsshash[(j * 4) + 2] << 16;
		rsskey |= rsshash[(j * 4) + 3] << 24;
		E1000_WRITE_REG_ARRAY(hw, E1000_RSSRK(0), j, rsskey);
	}

	num_rx_queues = adapter->rss_queues;

	if (adapter->vfs_allocated_count || adapter->vmdq_pools) {
		/* 82575 and 82576 supports 2 RSS queues for VMDq */
		switch (hw->mac.type) {
		case e1000_i350:
		case e1000_82580:
			num_rx_queues = 1;
			shift = 0;
			break;
		case e1000_82576:
			shift = 3;
			num_rx_queues = 2;
			break;
		case e1000_82575:
			shift = 2;
			shift2 = 6;
		default:
			break;
		}
	} else {
		if (hw->mac.type == e1000_82575)
			shift = 6;
	}

	for (j = 0; j < (32 * 4); j++) {
		reta.bytes[j & 3] = (j % num_rx_queues) << shift;
		if (shift2)
			reta.bytes[j & 3] |= num_rx_queues << shift2;
		if ((j & 3) == 3)
			E1000_WRITE_REG(hw, E1000_RETA(j >> 2), reta.dword);
	}

	/*
	 * Disable raw packet checksumming so that RSS hash is placed in
	 * descriptor on writeback.  No need to enable TCP/UDP/IP checksum
	 * offloads as they are enabled by default
	 */
	rxcsum = E1000_READ_REG(hw, E1000_RXCSUM);
	rxcsum |= E1000_RXCSUM_PCSD;

	if (adapter->hw.mac.type >= e1000_82576)
		/* Enable Receive Checksum Offload for SCTP */
		rxcsum |= E1000_RXCSUM_CRCOFL;

	/* Don't need to set TUOFL or IPOFL, they default to 1 */
	E1000_WRITE_REG(hw, E1000_RXCSUM, rxcsum);

	/* If VMDq is enabled then we set the appropriate mode for that, else
	 * we default to RSS so that an RSS hash is calculated per packet even
	 * if we are only using one queue */
	if (adapter->vfs_allocated_count || adapter->vmdq_pools) {
		if (hw->mac.type > e1000_82575) {
			/* Set the default pool for the PF's first queue */
			u32 vtctl = E1000_READ_REG(hw, E1000_VT_CTL);
			vtctl &= ~(E1000_VT_CTL_DEFAULT_POOL_MASK |
				   E1000_VT_CTL_DISABLE_DEF_POOL);
			vtctl |= adapter->vfs_allocated_count <<
				E1000_VT_CTL_DEFAULT_POOL_SHIFT;
			E1000_WRITE_REG(hw, E1000_VT_CTL, vtctl);
		} else if (adapter->rss_queues > 1) {
			/* set default queue for pool 1 to queue 2 */
			E1000_WRITE_REG(hw, E1000_VT_CTL,
				        adapter->rss_queues << 7);
		}
		if (adapter->rss_queues > 1)
			mrqc = E1000_MRQC_ENABLE_VMDQ_RSS_2Q;
		else
			mrqc = E1000_MRQC_ENABLE_VMDQ;
	} else {
		mrqc = E1000_MRQC_ENABLE_RSS_4Q;
	}

	igb_vmm_control(adapter);

	/*
	 * Generate RSS hash based on TCP port numbers and/or
	 * IPv4/v6 src and dst addresses since UDP cannot be
	 * hashed reliably due to IP fragmentation
	 */
	mrqc |= E1000_MRQC_RSS_FIELD_IPV4 |
		E1000_MRQC_RSS_FIELD_IPV4_TCP |
		E1000_MRQC_RSS_FIELD_IPV6 |
		E1000_MRQC_RSS_FIELD_IPV6_TCP |
		E1000_MRQC_RSS_FIELD_IPV6_TCP_EX;

	E1000_WRITE_REG(hw, E1000_MRQC, mrqc);
}

/**
 * igb_setup_rctl - configure the receive control registers
 * @adapter: Board private structure
 **/
void igb_setup_rctl(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 rctl;

	rctl = E1000_READ_REG(hw, E1000_RCTL);

	rctl &= ~(3 << E1000_RCTL_MO_SHIFT);
	rctl &= ~(E1000_RCTL_LBM_TCVR | E1000_RCTL_LBM_MAC);

	rctl |= E1000_RCTL_EN | E1000_RCTL_BAM | E1000_RCTL_RDMTS_HALF |
		(hw->mac.mc_filter_type << E1000_RCTL_MO_SHIFT);

	/*
	 * enable stripping of CRC. It's unlikely this will break BMC
	 * redirection as it did with e1000. Newer features require
	 * that the HW strips the CRC.
	 */
	rctl |= E1000_RCTL_SECRC;

	/* disable store bad packets and clear size bits. */
	rctl &= ~(E1000_RCTL_SBP | E1000_RCTL_SZ_256);

	/* enable LPE to prevent packets larger than max_frame_size */
	rctl |= E1000_RCTL_LPE;

	/* disable queue 0 to prevent tail write w/o re-config */
	E1000_WRITE_REG(hw, E1000_RXDCTL(0), 0);

	/* Attention!!!  For SR-IOV PF driver operations you must enable
	 * queue drop for all VF and PF queues to prevent head of line blocking
	 * if an un-trusted VF does not provide descriptors to hardware.
	 */
	if (adapter->vfs_allocated_count) {
		/* set all queue drop enable bits */
		E1000_WRITE_REG(hw, E1000_QDE, ALL_QUEUES);
	}

	E1000_WRITE_REG(hw, E1000_RCTL, rctl);
}

static inline int igb_set_vf_rlpml(struct igb_adapter *adapter, int size,
                                   int vfn)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr;

	/* if it isn't the PF check to see if VFs are enabled and
	 * increase the size to support vlan tags */
	if (vfn < adapter->vfs_allocated_count &&
	    adapter->vf_data[vfn].vlans_enabled)
		size += VLAN_TAG_SIZE;

#ifdef CONFIG_IGB_VMDQ_NETDEV
	if (vfn >= adapter->vfs_allocated_count) {
		int queue = vfn - adapter->vfs_allocated_count;
		struct igb_vmdq_adapter *vadapter;

		vadapter = netdev_priv(adapter->vmdq_netdev[queue-1]);
		if (vadapter->vlgrp)
			size += VLAN_TAG_SIZE;
	}
#endif
	vmolr = E1000_READ_REG(hw, E1000_VMOLR(vfn));
	vmolr &= ~E1000_VMOLR_RLPML_MASK;
	vmolr |= size | E1000_VMOLR_LPE;
	E1000_WRITE_REG(hw, E1000_VMOLR(vfn), vmolr);

	return 0;
}

/**
 * igb_rlpml_set - set maximum receive packet size
 * @adapter: board private structure
 *
 * Configure maximum receivable packet size.
 **/
static void igb_rlpml_set(struct igb_adapter *adapter)
{
	u32 max_frame_size = adapter->max_frame_size;
	struct e1000_hw *hw = &adapter->hw;
	u16 pf_id = adapter->vfs_allocated_count;

	if (adapter->vmdq_pools && hw->mac.type != e1000_82575) {
		int i;
		for (i = 1; i < adapter->vmdq_pools; i++)
			igb_set_vf_rlpml(adapter, max_frame_size, pf_id + i);
		/*
		 * If we're in VMDQ or SR-IOV mode, then set global RLPML
		 * to our max jumbo frame size, in case we need to enable
		 * jumbo frames on one of the rings later.
		 * This will not pass over-length frames into the default
		 * queue because it's gated by the VMOLR.RLPML.
		 */
		max_frame_size = MAX_JUMBO_FRAME_SIZE;
	}
	/* Set VF RLPML for the PF device. */
	if (adapter->vfs_allocated_count)
		igb_set_vf_rlpml(adapter, max_frame_size, pf_id);

	E1000_WRITE_REG(hw, E1000_RLPML, max_frame_size);
}

static inline void igb_set_vf_vlan_strip(struct igb_adapter *adapter,
					int vfn, bool enable)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 val;
	void __iomem *reg;

	if (hw->mac.type < e1000_82576)
		return;

		if (hw->mac.type == e1000_i350)
		reg = hw->hw_addr + E1000_DVMOLR(vfn);
	else
		reg = hw->hw_addr + E1000_VMOLR(vfn);

	val = readl(reg);
	if (enable)
		val |= E1000_VMOLR_STRVLAN;
	else
		val &= ~(E1000_VMOLR_STRVLAN);
	writel(val, reg);
}
static inline void igb_set_vmolr(struct igb_adapter *adapter,
				 int vfn, bool aupe)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr;

	/*
	 * This register exists only on 82576 and newer so if we are older then
	 * we should exit and do nothing
	 */
	if (hw->mac.type < e1000_82576)
		return;

	vmolr = E1000_READ_REG(hw, E1000_VMOLR(vfn));

	if (aupe)
		vmolr |= E1000_VMOLR_AUPE;        /* Accept untagged packets */
	else
		vmolr &= ~(E1000_VMOLR_AUPE); /* Tagged packets ONLY */

	/* clear all bits that might not be set */
	vmolr &= ~E1000_VMOLR_RSSE;

	if (adapter->rss_queues > 1 && vfn == adapter->vfs_allocated_count)
		vmolr |= E1000_VMOLR_RSSE; /* enable RSS */

	vmolr |= E1000_VMOLR_BAM;	   /* Accept broadcast */
	vmolr |= E1000_VMOLR_LPE; 	   /* Accept long packets */

	E1000_WRITE_REG(hw, E1000_VMOLR(vfn), vmolr);
}

/**
 * igb_configure_rx_ring - Configure a receive ring after Reset
 * @adapter: board private structure
 * @ring: receive ring to be configured
 *
 * Configure the Rx unit of the MAC after a reset.
 **/
void igb_configure_rx_ring(struct igb_adapter *adapter,
                           struct igb_ring *ring)
{
	struct e1000_hw *hw = &adapter->hw;
	u64 rdba = ring->dma;
	int reg_idx = ring->reg_idx;
	u32 srrctl = 0, rxdctl = 0;

	/* disable the queue */
	E1000_WRITE_REG(hw, E1000_RXDCTL(reg_idx), 0);

	/* Set DMA base address registers */
	E1000_WRITE_REG(hw, E1000_RDBAL(reg_idx),
	                rdba & 0x00000000ffffffffULL);
	E1000_WRITE_REG(hw, E1000_RDBAH(reg_idx), rdba >> 32);
	E1000_WRITE_REG(hw, E1000_RDLEN(reg_idx),
	               ring->count * sizeof(union e1000_adv_rx_desc));

	/* initialize head and tail */
	ring->tail = hw->hw_addr + E1000_RDT(reg_idx);
	E1000_WRITE_REG(hw, E1000_RDH(reg_idx), 0);
	writel(0, ring->tail);

	/* set descriptor configuration */
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
	srrctl = IGB_RX_HDR_LEN << E1000_SRRCTL_BSIZEHDRSIZE_SHIFT;
#if (PAGE_SIZE / 2) > IGB_RXBUFFER_16384
	srrctl |= IGB_RXBUFFER_16384 >> E1000_SRRCTL_BSIZEPKT_SHIFT;
#else
	srrctl |= (PAGE_SIZE / 2) >> E1000_SRRCTL_BSIZEPKT_SHIFT;
#endif
	srrctl |= E1000_SRRCTL_DESCTYPE_HDR_SPLIT_ALWAYS;
#else /* CONFIG_IGB_DISABLE_PACKET_SPLIT */
	srrctl = ALIGN(ring->rx_buffer_len, 1024) >>
	         E1000_SRRCTL_BSIZEPKT_SHIFT;
	srrctl |= E1000_SRRCTL_DESCTYPE_ADV_ONEBUF;
#endif /* CONFIG_IGB_DISABLE_PACKET_SPLIT */
#ifdef IGB_PER_PKT_TIMESTAMP
	if (hw->mac.type >= e1000_82580)
		srrctl |= E1000_SRRCTL_TIMESTAMP;
#endif
	/* Set Drop Enable if we are supporting SR-IOV */
	if (adapter->vfs_allocated_count)
		srrctl |= E1000_SRRCTL_DROP_EN;

	E1000_WRITE_REG(hw, E1000_SRRCTL(reg_idx), srrctl);

	/* set filtering for VMDQ pools */
	igb_set_vmolr(adapter, reg_idx & 0x7, true);

	rxdctl |= IGB_RX_PTHRESH;
	rxdctl |= IGB_RX_HTHRESH << 8;
	rxdctl |= IGB_RX_WTHRESH << 16;

	/* enable receive descriptor fetching */
	rxdctl |= E1000_RXDCTL_QUEUE_ENABLE;
	E1000_WRITE_REG(hw, E1000_RXDCTL(reg_idx), rxdctl);
}

/**
 * igb_configure_rx - Configure receive Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Rx unit of the MAC after a reset.
 **/
static void igb_configure_rx(struct igb_adapter *adapter)
{
	int i;

	/* set UTA to appropriate mode */
	igb_set_uta(adapter);

	igb_full_sync_mac_table(adapter);
	/* Setup the HW Rx Head and Tail Descriptor Pointers and
	 * the Base and Length of the Rx Descriptor Ring */
	for (i = 0; i < adapter->num_rx_queues; i++)
		igb_configure_rx_ring(adapter, adapter->rx_ring[i]);
}

/**
 * igb_free_tx_resources - Free Tx Resources per Queue
 * @tx_ring: Tx descriptor ring for a specific queue
 *
 * Free all transmit software resources
 **/
void igb_free_tx_resources(struct igb_ring *tx_ring)
{
	igb_clean_tx_ring(tx_ring);

	vfree(tx_ring->buffer_info);
	tx_ring->buffer_info = NULL;

	/* if not set, then don't free */
	if (!tx_ring->desc)
		return;

	dma_free_coherent(tx_ring->dev, tx_ring->size,
			  tx_ring->desc, tx_ring->dma);

	tx_ring->desc = NULL;
}

/**
 * igb_free_all_tx_resources - Free Tx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all transmit software resources
 **/
static void igb_free_all_tx_resources(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
		igb_free_tx_resources(adapter->tx_ring[i]);
}

void igb_unmap_and_free_tx_resource(struct igb_ring *tx_ring,
				    struct igb_buffer *buffer_info)
{
	if (buffer_info->dma) {
		if (buffer_info->mapped_as_page)
			dma_unmap_page(tx_ring->dev,
					buffer_info->dma,
					buffer_info->length,
					DMA_TO_DEVICE);
		else
			dma_unmap_single(tx_ring->dev,
					buffer_info->dma,
					buffer_info->length,
					DMA_TO_DEVICE);
	}
	if (buffer_info->skb)
		dev_kfree_skb_any(buffer_info->skb);
	/* buffer_info must be completely set up in the transmit path */
	memset(buffer_info, 0, sizeof(struct igb_buffer));
}

/**
 * igb_clean_tx_ring - Free Tx Buffers
 * @tx_ring: ring to be cleaned
 **/
static void igb_clean_tx_ring(struct igb_ring *tx_ring)
{
	struct igb_buffer *buffer_info;
	unsigned long size;
	u16 i;

	if (!tx_ring->buffer_info)
		return;
	/* Free all the Tx ring sk_buffs */

	for (i = 0; i < tx_ring->count; i++) {
		buffer_info = &tx_ring->buffer_info[i];
		igb_unmap_and_free_tx_resource(tx_ring, buffer_info);
	}

	size = sizeof(struct igb_buffer) * tx_ring->count;
	memset(tx_ring->buffer_info, 0, size);

	/* Zero out the descriptor ring */
	memset(tx_ring->desc, 0, tx_ring->size);

	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;
}

/**
 * igb_clean_all_tx_rings - Free Tx Buffers for all queues
 * @adapter: board private structure
 **/
static void igb_clean_all_tx_rings(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
		igb_clean_tx_ring(adapter->tx_ring[i]);
}

/**
 * igb_free_rx_resources - Free Rx Resources
 * @rx_ring: ring to clean the resources from
 *
 * Free all receive software resources
 **/
void igb_free_rx_resources(struct igb_ring *rx_ring)
{
	igb_clean_rx_ring(rx_ring);

	vfree(rx_ring->buffer_info);
	rx_ring->buffer_info = NULL;

	/* if not set, then don't free */
	if (!rx_ring->desc)
		return;

	dma_free_coherent(rx_ring->dev, rx_ring->size,
			  rx_ring->desc, rx_ring->dma);

	rx_ring->desc = NULL;
}

/**
 * igb_free_all_rx_resources - Free Rx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all receive software resources
 **/
static void igb_free_all_rx_resources(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_rx_queues; i++)
		igb_free_rx_resources(adapter->rx_ring[i]);
}

/**
 * igb_clean_rx_ring - Free Rx Buffers per Queue
 * @rx_ring: ring to free buffers from
 **/
void igb_clean_rx_ring(struct igb_ring *rx_ring)
{
	unsigned long size;
#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
	const int bufsz = rx_ring->rx_buffer_len;
#else
	const int bufsz = IGB_RX_HDR_LEN;
#endif
	u16 i;

	if (!rx_ring->buffer_info)
		return;

	/* Free all the Rx ring sk_buffs */
	for (i = 0; i < rx_ring->count; i++) {
		struct igb_buffer *buffer_info = &rx_ring->buffer_info[i];
		if (buffer_info->dma) {
			dma_unmap_single(rx_ring->dev,
			                 buffer_info->dma,
					 bufsz,
					 DMA_FROM_DEVICE);
			buffer_info->dma = 0;
		}

		if (buffer_info->skb) {
			dev_kfree_skb(buffer_info->skb);
			buffer_info->skb = NULL;
		}
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
		if (buffer_info->page_dma) {
			dma_unmap_page(rx_ring->dev,
			               buffer_info->page_dma,
				       PAGE_SIZE / 2,
				       DMA_FROM_DEVICE);
			buffer_info->page_dma = 0;
		}
		if (buffer_info->page) {
			put_page(buffer_info->page);
			buffer_info->page = NULL;
			buffer_info->page_offset = 0;
		}
#endif
	}

	size = sizeof(struct igb_buffer) * rx_ring->count;
	memset(rx_ring->buffer_info, 0, size);

	/* Zero out the descriptor ring */
	memset(rx_ring->desc, 0, rx_ring->size);

	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;
}

/**
 * igb_clean_all_rx_rings - Free Rx Buffers for all queues
 * @adapter: board private structure
 **/
static void igb_clean_all_rx_rings(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_rx_queues; i++)
		igb_clean_rx_ring(adapter->rx_ring[i]);
}

/**
 * igb_set_mac - Change the Ethernet Address of the NIC
 * @netdev: network interface device structure
 * @p: pointer to an address structure
 *
 * Returns 0 on success, negative on failure
 **/
static int igb_set_mac(struct net_device *netdev, void *p)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	igb_del_mac_filter(adapter, hw->mac.addr,
			   adapter->vfs_allocated_count);
	memcpy(netdev->dev_addr, addr->sa_data, netdev->addr_len);
	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);

	/* set the correct pool for the new PF MAC address in entry 0 */
	return igb_add_mac_filter(adapter, hw->mac.addr,
	                   adapter->vfs_allocated_count);
}

/**
 * igb_write_mc_addr_list - write multicast addresses to MTA
 * @netdev: network interface device structure
 *
 * Writes multicast address list to the MTA hash table.
 * Returns: -ENOMEM on failure
 *                0 on no addresses written
 *                X on writing X addresses to MTA
 **/
int igb_write_mc_addr_list(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
#ifdef NETDEV_HW_ADDR_T_MULTICAST
	struct netdev_hw_addr *ha;
#else
	struct dev_mc_list *ha;
#endif
	u8  *mta_list;
	int i, count;
#ifdef CONFIG_IGB_VMDQ_NETDEV
	int vm;
#endif
	count = netdev_mc_count(netdev);
#ifdef CONFIG_IGB_VMDQ_NETDEV
	for (vm = 1; vm < adapter->vmdq_pools; vm++) {
		if (!adapter->vmdq_netdev[vm])
			break;
		if (!netif_running(adapter->vmdq_netdev[vm]))
			continue;
		count += netdev_mc_count(adapter->vmdq_netdev[vm]);
	}
#endif

	if (!count) {
		e1000_update_mc_addr_list(hw, NULL, 0);
		return 0;
	}
	mta_list = kzalloc(count * 6, GFP_ATOMIC);
	if (!mta_list)
		return -ENOMEM;

	/* The shared function expects a packed array of only addresses. */
	i = 0;
	netdev_for_each_mc_addr(ha, netdev)
#ifdef NETDEV_HW_ADDR_T_MULTICAST
		memcpy(mta_list + (i++ * ETH_ALEN), ha->addr, ETH_ALEN);
#else
		memcpy(mta_list + (i++ * ETH_ALEN), ha->dmi_addr, ETH_ALEN);
#endif
#ifdef CONFIG_IGB_VMDQ_NETDEV
	for (vm = 1; vm < adapter->vmdq_pools; vm++) {
		if (!adapter->vmdq_netdev[vm])
			break;
		if (!netif_running(adapter->vmdq_netdev[vm]) ||
		    !netdev_mc_count(adapter->vmdq_netdev[vm]))
			continue;
		netdev_for_each_mc_addr(ha, adapter->vmdq_netdev[vm])
#ifdef NETDEV_HW_ADDR_T_MULTICAST
			memcpy(mta_list + (i++ * ETH_ALEN),
			       ha->addr, ETH_ALEN);
#else
			memcpy(mta_list + (i++ * ETH_ALEN),
			       ha->dmi_addr, ETH_ALEN);
#endif
	}
#endif
	e1000_update_mc_addr_list(hw, mta_list, i);
	kfree(mta_list);

	return count;
}

void igb_rar_set(struct igb_adapter *adapter, u32 index)
{
	u32 rar_low, rar_high;
	struct e1000_hw *hw = &adapter->hw;
	u8 *addr = adapter->mac_table[index].addr;
	/* HW expects these in little endian so we reverse the byte order
	 * from network order (big endian) to little endian
	 */
	rar_low = ((u32) addr[0] | ((u32) addr[1] << 8) |
	          ((u32) addr[2] << 16) | ((u32) addr[3] << 24));
	rar_high = ((u32) addr[4] | ((u32) addr[5] << 8));

	/* Indicate to hardware the Address is Valid. */
	if (adapter->mac_table[index].state & IGB_MAC_STATE_IN_USE)
		rar_high |= E1000_RAH_AV;

	if (hw->mac.type == e1000_82575)
		rar_high |= E1000_RAH_POOL_1 * adapter->mac_table[index].queue;
	else
		rar_high |= E1000_RAH_POOL_1 << adapter->mac_table[index].queue;

	E1000_WRITE_REG(hw, E1000_RAL(index), rar_low);
	E1000_WRITE_FLUSH(hw);
	E1000_WRITE_REG(hw, E1000_RAH(index), rar_high);
	E1000_WRITE_FLUSH(hw);
}

void igb_full_sync_mac_table(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;
	for (i = 0; i < hw->mac.rar_entry_count; i++) {
			igb_rar_set(adapter, i);
	}
}

void igb_sync_mac_table(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;
	for (i = 0; i < hw->mac.rar_entry_count; i++) {
		if (adapter->mac_table[i].state & IGB_MAC_STATE_MODIFIED)
			igb_rar_set(adapter, i);
		adapter->mac_table[i].state &= ~(IGB_MAC_STATE_MODIFIED);
	}
}

int igb_available_rars(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i, count = 0;

	for (i = 0; i < hw->mac.rar_entry_count; i++) {
		if (adapter->mac_table[i].state == 0)
			count++;
	}
	return count;
}

#ifdef HAVE_SET_RX_MODE
/**
 * igb_write_uc_addr_list - write unicast addresses to RAR table
 * @netdev: network interface device structure
 *
 * Writes unicast address list to the RAR table.
 * Returns: -ENOMEM on failure/insufficient address space
 *                0 on no addresses written
 *                X on writing X addresses to the RAR table
 **/
static int igb_write_uc_addr_list(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	unsigned int vfn = adapter->vfs_allocated_count;
	int count = 0;

	/* return ENOMEM indicating insufficient memory for addresses */
	if (netdev_uc_count(netdev) > igb_available_rars(adapter))
		return -ENOMEM;
	if (!netdev_uc_empty(netdev)) {
#ifdef NETDEV_HW_ADDR_T_UNICAST
		struct netdev_hw_addr *ha;
#else
		struct dev_mc_list *ha;
#endif
		netdev_for_each_uc_addr(ha, netdev) {
#ifdef NETDEV_HW_ADDR_T_UNICAST
			igb_del_mac_filter(adapter, ha->addr, vfn);
			igb_add_mac_filter(adapter, ha->addr, vfn);
#else
			igb_del_mac_filter(adapter, ha->da_addr, vfn);
			igb_add_mac_filter(adapter, ha->da_addr, vfn);
#endif
			count++;
		}
	}
	return count;
}

#endif
/**
 * igb_set_rx_mode - Secondary Unicast, Multicast and Promiscuous mode set
 * @netdev: network interface device structure
 *
 * The set_rx_mode entry point is called whenever the unicast or multicast
 * address lists or the network interface flags are updated.  This routine is
 * responsible for configuring the hardware for proper unicast, multicast,
 * promiscuous mode, and all-multi behavior.
 **/
static void igb_set_rx_mode(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	unsigned int vfn = adapter->vfs_allocated_count;
	u32 rctl, vmolr = 0;
	int count;

	/* Check for Promiscuous and All Multicast modes */
	rctl = E1000_READ_REG(hw, E1000_RCTL);

	/* clear the effected bits */
	rctl &= ~(E1000_RCTL_UPE | E1000_RCTL_MPE | E1000_RCTL_VFE);

	if (netdev->flags & IFF_PROMISC) {
		rctl |= (E1000_RCTL_UPE | E1000_RCTL_MPE);
		vmolr |= (E1000_VMOLR_ROPE | E1000_VMOLR_MPME);
	} else {
		if (netdev->flags & IFF_ALLMULTI) {
			rctl |= E1000_RCTL_MPE;
			vmolr |= E1000_VMOLR_MPME;
		} else {
			/*
			 * Write addresses to the MTA, if the attempt fails
			 * then we should just turn on promiscous mode so
			 * that we can at least receive multicast traffic
			 */
			count = igb_write_mc_addr_list(netdev);
			if (count < 0) {
				rctl |= E1000_RCTL_MPE;
				vmolr |= E1000_VMOLR_MPME;
			} else if (count) {
				vmolr |= E1000_VMOLR_ROMPE;
			}
		}
#ifdef HAVE_SET_RX_MODE
		/*
		 * Write addresses to available RAR registers, if there is not
		 * sufficient space to store all the addresses then enable
		 * unicast promiscous mode
		 */
		count = igb_write_uc_addr_list(netdev);
		if (count < 0) {
			rctl |= E1000_RCTL_UPE;
			vmolr |= E1000_VMOLR_ROPE;
		}
#endif
		rctl |= E1000_RCTL_VFE;
	}
	E1000_WRITE_REG(hw, E1000_RCTL, rctl);

	/*
	 * In order to support SR-IOV and eventually VMDq it is necessary to set
	 * the VMOLR to enable the appropriate modes.  Without this workaround
	 * we will have issues with VLAN tag stripping not being done for frames
	 * that are only arriving because we are the default pool
	 */
	if (hw->mac.type < e1000_82576)
		return;

	vmolr |= E1000_READ_REG(hw, E1000_VMOLR(vfn)) &
	         ~(E1000_VMOLR_ROPE | E1000_VMOLR_MPME | E1000_VMOLR_ROMPE);
	E1000_WRITE_REG(hw, E1000_VMOLR(vfn), vmolr);
	igb_restore_vf_multicasts(adapter);
}

static void igb_check_wvbr(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 wvbr = 0;

	switch (hw->mac.type) {
	case e1000_82576:
	case e1000_i350:
		if (!(wvbr = E1000_READ_REG(hw, E1000_WVBR)))
			return;
		break;
	default:
		break;
	}

	adapter->wvbr |= wvbr;
}

#define IGB_STAGGERED_QUEUE_OFFSET 8

static void igb_spoof_check(struct igb_adapter *adapter)
{
	int j;

	if (!adapter->wvbr)
		return;

	for(j = 0; j < adapter->vfs_allocated_count; j++) {
		if (adapter->wvbr & (1 << j) ||
		    adapter->wvbr & (1 << (j + IGB_STAGGERED_QUEUE_OFFSET))) {
			DPRINTK(DRV, WARNING,
				"Spoof event(s) detected on VF %d\n", j);
			adapter->wvbr &=
				~((1 << j) |
				  (1 << (j + IGB_STAGGERED_QUEUE_OFFSET)));
		}
	}
}

/* Need to wait a few seconds after link up to get diagnostic information from
 * the phy */
static void igb_update_phy_info(unsigned long data)
{
	struct igb_adapter *adapter = (struct igb_adapter *) data;
	e1000_get_phy_info(&adapter->hw);
}

/**
 * igb_has_link - check shared code for link and determine up/down
 * @adapter: pointer to driver private info
 **/
bool igb_has_link(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	bool link_active = FALSE;

	/* get_link_status is set on LSC (link status) interrupt or
	 * rx sequence error interrupt.  get_link_status will stay
	 * false until the e1000_check_for_link establishes link
	 * for copper adapters ONLY
	 */
	switch (hw->phy.media_type) {
	case e1000_media_type_copper:
		if (!hw->mac.get_link_status)
			return true;
	case e1000_media_type_internal_serdes:
		e1000_check_for_link(hw);
		link_active = !hw->mac.get_link_status;
		break;
	case e1000_media_type_unknown:
	default:
		break;
	}

	return link_active;
}

/**
 * igb_watchdog - Timer Call-back
 * @data: pointer to adapter cast into an unsigned long
 **/
static void igb_watchdog(unsigned long data)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
	/* Do the rest outside of interrupt context */
	schedule_work(&adapter->watchdog_task);
}

static void igb_watchdog_task(struct work_struct *work)
{
	struct igb_adapter *adapter = container_of(work,
	                                           struct igb_adapter,
                                                   watchdog_task);
	struct e1000_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	u32 link;
	int i;
	u32 thstat, ctrl_ext;


	link = igb_has_link(adapter);
	if (link) {
		if (!netif_carrier_ok(netdev)) {
			u32 ctrl;
			e1000_get_speed_and_duplex(hw,
			                           &adapter->link_speed,
			                           &adapter->link_duplex);

			ctrl = E1000_READ_REG(hw, E1000_CTRL);
			/* Links status message must follow this format */
			printk(KERN_INFO "igb: %s NIC Link is Up %d Mbps %s, "
				 "Flow Control: %s\n",
			       netdev->name,
			       adapter->link_speed,
			       adapter->link_duplex == FULL_DUPLEX ?
				 "Full Duplex" : "Half Duplex",
			       ((ctrl & E1000_CTRL_TFCE) &&
			        (ctrl & E1000_CTRL_RFCE)) ? "RX/TX":
			       ((ctrl & E1000_CTRL_RFCE) ?  "RX" :
			       ((ctrl & E1000_CTRL_TFCE) ?  "TX" : "None")));
			/* adjust timeout factor according to speed/duplex */
			adapter->tx_timeout_factor = 1;
			switch (adapter->link_speed) {
			case SPEED_10:
				adapter->tx_timeout_factor = 14;
				break;
			case SPEED_100:
				/* maybe add some timeout factor ? */
				break;
			}

			netif_carrier_on(netdev);
			netif_tx_wake_all_queues(netdev);

			igb_ping_all_vfs(adapter);
#ifdef IFLA_VF_MAX
			igb_check_vf_rate_limit(adapter);
#endif /* IFLA_VF_MAX */

			/* link state has changed, schedule phy info update */
			if (!test_bit(__IGB_DOWN, &adapter->state))
				mod_timer(&adapter->phy_info_timer,
					  round_jiffies(jiffies + 2 * HZ));
		}
	} else {
		if (netif_carrier_ok(netdev)) {
			adapter->link_speed = 0;
			adapter->link_duplex = 0;
			/* check for thermal sensor event on i350 */
			if (hw->mac.type == e1000_i350) {
				thstat = E1000_READ_REG(hw, E1000_THSTAT);
				ctrl_ext = E1000_READ_REG(hw, E1000_CTRL_EXT);
				if ((hw->phy.media_type ==
					e1000_media_type_copper) &&
					!(ctrl_ext &
					E1000_CTRL_EXT_LINK_MODE_SGMII)) {
					if (thstat & E1000_THSTAT_PWR_DOWN) {
						printk(KERN_ERR "igb: %s The "
						"network adapter was stopped "
						"because it overheated.\n",
						netdev->name);
					}
					if (thstat & E1000_THSTAT_LINK_THROTTLE) {
						printk(KERN_INFO 
							"igb: %s The network "
							"adapter supported "
							"link speed "
							"was downshifted "
							"because it "
							"overheated.\n",
							netdev->name);
					}
				}
			}

			/* Links status message must follow this format */
			printk(KERN_INFO "igb: %s NIC Link is Down\n",
			       netdev->name);
			netif_carrier_off(netdev);
			netif_tx_stop_all_queues(netdev);

			igb_ping_all_vfs(adapter);

			/* link state has changed, schedule phy info update */
			if (!test_bit(__IGB_DOWN, &adapter->state))
				mod_timer(&adapter->phy_info_timer,
					  round_jiffies(jiffies + 2 * HZ));
		}
	}

	igb_update_stats(adapter);

	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct igb_ring *tx_ring = adapter->tx_ring[i];
		if (!netif_carrier_ok(netdev)) {
			/* We've lost link, so the controller stops DMA,
			 * but we've got queued Tx work that's never going
			 * to get done, so reset controller to flush Tx.
			 * (Do the reset outside of interrupt context). */
			if (igb_desc_unused(tx_ring) + 1 < tx_ring->count) {
				adapter->tx_timeout_count++;
				schedule_work(&adapter->reset_task);
				/* return immediately since reset is imminent */
				return;
			}
		}

		/* Force detection of hung controller every watchdog period */
		set_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags);
	}

	/* Cause software interrupt to ensure rx ring is cleaned */
	if (adapter->msix_entries) {
		u32 eics = 0;
		for (i = 0; i < adapter->num_q_vectors; i++)
			eics |= adapter->q_vector[i]->eims_value;
		E1000_WRITE_REG(hw, E1000_EICS, eics);
	} else {
		E1000_WRITE_REG(hw, E1000_ICS, E1000_ICS_RXDMT0);
	}

	igb_spoof_check(adapter);

	/* Reset the timer */
	if (!test_bit(__IGB_DOWN, &adapter->state))
		mod_timer(&adapter->watchdog_timer,
			  round_jiffies(jiffies + 2 * HZ));
}

enum latency_range {
	lowest_latency = 0,
	low_latency = 1,
	bulk_latency = 2,
	latency_invalid = 255
};

/**
 * igb_update_ring_itr - update the dynamic ITR value based on packet size
 *
 *      Stores a new ITR value based on strictly on packet size.  This
 *      algorithm is less sophisticated than that used in igb_update_itr,
 *      due to the difficulty of synchronizing statistics across multiple
 *      receive rings.  The divisors and thresholds used by this fuction
 *      were determined based on theoretical maximum wire speed and testing
 *      data, in order to minimize response time while increasing bulk
 *      throughput.
 *      This functionality is controlled by the InterruptThrottleRate module
 *      parameter (see igb_param.c)
 *      NOTE:  This function is called only when operating in a multiqueue
 *             receive environment.
 * @q_vector: pointer to q_vector
 **/
static void igb_update_ring_itr(struct igb_q_vector *q_vector)
{
	int new_val = q_vector->itr_val;
	int avg_wire_size = 0;
	struct igb_adapter *adapter = q_vector->adapter;

	/* For non-gigabit speeds, just fix the interrupt rate at 4000
	 * ints/sec - ITR timer value of 120 ticks.
	 */
	if (adapter->link_speed != SPEED_1000) {
		new_val = IGB_4K_ITR;
		goto set_itr_val;
	}

	if (q_vector->rx_ring && q_vector->rx_ring->total_packets) {
		struct igb_ring *ring = q_vector->rx_ring;
		avg_wire_size = ring->total_bytes / ring->total_packets;
	}

	if (q_vector->tx_ring && q_vector->tx_ring->total_packets) {
		struct igb_ring *ring = q_vector->tx_ring;
		avg_wire_size = max_t(u32, avg_wire_size,
		                      (ring->total_bytes /
		                       ring->total_packets));
	}

	/* if avg_wire_size isn't set no work was done */
	if (!avg_wire_size)
		goto clear_counts;

	/* Add 24 bytes to size to account for CRC, preamble, and gap */
	avg_wire_size += 24;

	/* Don't starve jumbo frames */
	avg_wire_size = min(avg_wire_size, 3000);

	/* Give a little boost to mid-size frames */
	if ((avg_wire_size > 300) && (avg_wire_size < 1200))
		new_val = avg_wire_size / 3;
	else
		new_val = avg_wire_size / 2;

	/* conservative mode (itr 3) eliminates the lowest_latency setting */
	if (new_val < IGB_20K_ITR &&
	    ((q_vector->rx_ring && adapter->rx_itr_setting == 3) ||
	     (!q_vector->rx_ring && adapter->tx_itr_setting == 3)))
		new_val = IGB_20K_ITR;

set_itr_val:
	if (new_val != q_vector->itr_val) {
		q_vector->itr_val = new_val;
		q_vector->set_itr = 1;
	}
clear_counts:
	if (q_vector->rx_ring) {
		q_vector->rx_ring->total_bytes = 0;
		q_vector->rx_ring->total_packets = 0;
	}
	if (q_vector->tx_ring) {
		q_vector->tx_ring->total_bytes = 0;
		q_vector->tx_ring->total_packets = 0;
	}
}

/**
 * igb_update_itr - update the dynamic ITR value based on statistics
 *      Stores a new ITR value based on packets and byte
 *      counts during the last interrupt.  The advantage of per interrupt
 *      computation is faster updates and more accurate ITR for the current
 *      traffic pattern.  Constants in this function were computed
 *      based on theoretical maximum wire speed and thresholds were set based
 *      on testing data as well as attempting to minimize response time
 *      while increasing bulk throughput.
 *      this functionality is controlled by the InterruptThrottleRate module
 *      parameter (see igb_param.c)
 *      NOTE:  These calculations are only valid when operating in a single-
 *             queue environment.
 * @ring: ring to update the itr for
 **/
static u16 igb_update_itr(struct igb_ring *ring)
{
	unsigned int packets = ring->total_packets;
	unsigned int bytes = ring->total_bytes;
	u16 itrval = ring->itr;

	/* no packets, exit with status unchanged */
	if (packets == 0)
		return itrval;

	switch (itrval) {
	case lowest_latency:
		/* handle TSO and jumbo frames */
		if (bytes/packets > 8000)
			itrval = bulk_latency;
		else if ((packets < 5) && (bytes > 512))
			itrval = low_latency;
		break;
	case low_latency:  /* 50 usec aka 20000 ints/s */
		if (bytes > 10000) {
			/* this if handles the TSO accounting */
			if (bytes/packets > 8000) {
				itrval = bulk_latency;
			} else if ((packets < 10) || ((bytes/packets) > 1200)) {
				itrval = bulk_latency;
			} else if ((packets > 35)) {
				itrval = lowest_latency;
			}
		} else if (bytes/packets > 2000) {
			itrval = bulk_latency;
		} else if (packets <= 2 && bytes < 512) {
			itrval = lowest_latency;
		}
		break;
	case bulk_latency: /* 250 usec aka 4000 ints/s */
		if (bytes > 25000) {
			if (packets > 35)
				itrval = low_latency;
		} else if (bytes < 1500) {
			itrval = low_latency;
		}
		break;
	}

	ring->total_bytes = 0;
	ring->total_packets = 0;
	ring->itr = itrval;
	return itrval;
}

static void igb_set_itr(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	u16 current_itr = 0;
	u32 new_itr = q_vector->itr_val;

	/* for non-gigabit speeds, just fix the interrupt rate at 4000 */
	if (adapter->link_speed != SPEED_1000) {
		current_itr = 0;
		new_itr = IGB_4K_ITR;
		goto set_itr_now;
	}

	if (q_vector->rx_ring) 
		current_itr = igb_update_itr(q_vector->rx_ring);
	if (q_vector->tx_ring) {
		igb_update_itr(q_vector->tx_ring);
		current_itr = max(current_itr, q_vector->tx_ring->itr);
	}

	/* conservative mode (itr 3) eliminates the lowest_latency setting */
	if (current_itr == lowest_latency &&
	    ((q_vector->rx_ring && adapter->rx_itr_setting == 3) ||
	     (!q_vector->rx_ring && adapter->tx_itr_setting == 3)))
		current_itr = low_latency;

	switch (current_itr) {
	/* counts and packets in update_itr are dependent on these numbers */
	case lowest_latency:
		new_itr = IGB_70K_ITR; /* 70,000 ints/sec */
		break;
	case low_latency:
		new_itr = IGB_20K_ITR; /* 20,000 ints/sec */
		break;
	case bulk_latency:
		new_itr = IGB_4K_ITR;  /* 4,000 ints/sec */
		break;
	default:
		break;
	}

set_itr_now:
	if (new_itr != q_vector->itr_val) {
		/* this attempts to bias the interrupt rate towards Bulk
		 * by adding intermediate steps when interrupt rate is
		 * increasing */
		new_itr = new_itr > q_vector->itr_val ?
		             max((new_itr * q_vector->itr_val) /
		                 (new_itr + (q_vector->itr_val >> 2)),
				 new_itr) :
			     new_itr;
		/* Don't write the value here; it resets the adapter's
		 * internal timer, and causes us to delay far longer than
		 * we should between interrupts.  Instead, we write the ITR
		 * value at the beginning of the next interrupt so the timing
		 * ends up being correct.
		 */
		q_vector->itr_val = new_itr;
		q_vector->set_itr = 1;
	}
}

#define IGB_TX_FLAGS_CSUM		0x00000001
#define IGB_TX_FLAGS_VLAN		0x00000002
#define IGB_TX_FLAGS_TSO		0x00000004
#define IGB_TX_FLAGS_IPV4		0x00000008
#define IGB_TX_FLAGS_TSTAMP		0x00000010
#define IGB_TX_FLAGS_VLAN_MASK		0xffff0000
#define IGB_TX_FLAGS_VLAN_SHIFT		        16

static inline struct e1000_adv_tx_context_desc *igb_tx_ctxtdesc(struct igb_ring *tx_ring)
{
	struct e1000_adv_tx_context_desc *context_desc;
	struct igb_buffer *buffer_info;
	u16 i = tx_ring->next_to_use;

	buffer_info = &tx_ring->buffer_info[i];
	buffer_info->time_stamp = jiffies;
	buffer_info->next_to_watch = i;

	context_desc = IGB_TX_CTXTDESC(tx_ring, i);

	i++;
	if (i == tx_ring->count)
		tx_ring->next_to_use = 0;
	else
		tx_ring->next_to_use = i;

	return context_desc;
}


static inline int igb_tso(struct igb_ring *tx_ring,
                          struct sk_buff *skb, u32 tx_flags, u8 *hdr_len)
{
#ifdef NETIF_F_TSO
	struct e1000_adv_tx_context_desc *context_desc;
	int err;
	u32 info = 0, tu_cmd = 0;
	u32 mss_l4len_idx;
	u8 l4len;

	if (skb_header_cloned(skb)) {
		err = pskb_expand_head(skb, 0, 0, GFP_ATOMIC);
		if (err)
			return err;
	}

	context_desc = igb_tx_ctxtdesc(tx_ring);
	l4len = tcp_hdrlen(skb);
	*hdr_len += l4len;

	if (skb->protocol == htons(ETH_P_IP)) {
		struct iphdr *iph = ip_hdr(skb);
		iph->tot_len = 0;
		iph->check = 0;
		tcp_hdr(skb)->check = ~csum_tcpudp_magic(iph->saddr,
							 iph->daddr, 0,
							 IPPROTO_TCP,
							 0);
#ifdef NETIF_F_TSO6
	} else if (skb_is_gso_v6(skb)) {
		ipv6_hdr(skb)->payload_len = 0;
		tcp_hdr(skb)->check = ~csum_ipv6_magic(&ipv6_hdr(skb)->saddr,
						       &ipv6_hdr(skb)->daddr,
						       0, IPPROTO_TCP, 0);
#endif
	}

	/* VLAN MACLEN IPLEN */
	if (tx_flags & IGB_TX_FLAGS_VLAN)
		info |= (tx_flags & IGB_TX_FLAGS_VLAN_MASK);
	info |= (skb_network_offset(skb) << E1000_ADVTXD_MACLEN_SHIFT);
	*hdr_len += skb_network_offset(skb);
	info |= skb_network_header_len(skb);
	*hdr_len += skb_network_header_len(skb);
	context_desc->vlan_macip_lens = cpu_to_le32(info);

	/* ADV DTYP TUCMD MKRLOC/ISCSIHEDLEN */
	tu_cmd |= (E1000_TXD_CMD_DEXT | E1000_ADVTXD_DTYP_CTXT);

	if (skb->protocol == htons(ETH_P_IP))
		tu_cmd |= E1000_ADVTXD_TUCMD_IPV4;
	tu_cmd |= E1000_ADVTXD_TUCMD_L4T_TCP;

	context_desc->type_tucmd_mlhl = cpu_to_le32(tu_cmd);

	/* MSS L4LEN IDX */
	mss_l4len_idx = (skb_shinfo(skb)->gso_size << E1000_ADVTXD_MSS_SHIFT);
	mss_l4len_idx |= (l4len << E1000_ADVTXD_L4LEN_SHIFT);

	/* For 82575, context index must be unique per ring. */
	if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
		mss_l4len_idx |= tx_ring->reg_idx << 4;

	context_desc->mss_l4len_idx = cpu_to_le32(mss_l4len_idx);
	context_desc->seqnum_seed = 0;

	return TRUE;
#else
	return FALSE;
#endif  /* NETIF_F_TSO */
}

static inline bool igb_tx_csum(struct igb_ring *tx_ring,
			       struct sk_buff *skb, u32 tx_flags)
{
	if ((skb->ip_summed == CHECKSUM_PARTIAL) ||
	    (tx_flags & IGB_TX_FLAGS_VLAN)) {
		struct e1000_adv_tx_context_desc *context_desc;
		u32 info = 0, tu_cmd = 0;

		context_desc = igb_tx_ctxtdesc(tx_ring);

		if (tx_flags & IGB_TX_FLAGS_VLAN)
			info |= (tx_flags & IGB_TX_FLAGS_VLAN_MASK);

		info |= (skb_network_offset(skb) << E1000_ADVTXD_MACLEN_SHIFT);
		if (skb->ip_summed == CHECKSUM_PARTIAL)
			info |= skb_network_header_len(skb);

		context_desc->vlan_macip_lens = cpu_to_le32(info);

		tu_cmd |= (E1000_TXD_CMD_DEXT | E1000_ADVTXD_DTYP_CTXT);

		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			__be16 protocol;

			if (skb->protocol == cpu_to_be16(ETH_P_8021Q)) {
				const struct vlan_ethhdr *vhdr =
				          (const struct vlan_ethhdr*)skb->data;

				protocol = vhdr->h_vlan_encapsulated_proto;
			} else {
				protocol = skb->protocol;
			}

			switch (protocol) {
			case __constant_htons(ETH_P_IP):
				tu_cmd |= E1000_ADVTXD_TUCMD_IPV4;
				if (ip_hdr(skb)->protocol == IPPROTO_TCP)
					tu_cmd |= E1000_ADVTXD_TUCMD_L4T_TCP;
				else if (ip_hdr(skb)->protocol == IPPROTO_SCTP)
					tu_cmd |= E1000_ADVTXD_TUCMD_L4T_SCTP;
				break;
#ifdef NETIF_F_IPV6_CSUM
			case __constant_htons(ETH_P_IPV6):
				/* XXX what about other V6 headers?? */
				if (ipv6_hdr(skb)->nexthdr == IPPROTO_TCP)
					tu_cmd |= E1000_ADVTXD_TUCMD_L4T_TCP;
				else if (ipv6_hdr(skb)->nexthdr == IPPROTO_SCTP)
					tu_cmd |= E1000_ADVTXD_TUCMD_L4T_SCTP;
				break;
#endif
			default:
				if (unlikely(net_ratelimit()))
					dev_warn(tx_ring->dev,
					    "partial checksum but proto=%x!\n",
					    skb->protocol);
				break;
			}
		}

		context_desc->type_tucmd_mlhl = cpu_to_le32(tu_cmd);
		context_desc->seqnum_seed = 0;
		if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
			context_desc->mss_l4len_idx =
				cpu_to_le32(tx_ring->reg_idx << 4);

		return TRUE;
	}
	return FALSE;
}

#define IGB_MAX_TXD_PWR	16
#define IGB_MAX_DATA_PER_TXD	(1<<IGB_MAX_TXD_PWR)

static inline int igb_tx_map(struct igb_ring *tx_ring, struct sk_buff *skb,
			     unsigned int first)
{
	struct igb_buffer *buffer_info;
	unsigned int hlen = skb_headlen(skb);
	unsigned int f;
	int count = 0;
	u16 i = tx_ring->next_to_use;
#ifdef NETIF_F_TSO
	u16 gso_segs = skb_shinfo(skb)->gso_segs ?: 1;
#endif

	buffer_info = &tx_ring->buffer_info[i];
	buffer_info->length = hlen;
	/* set time_stamp *before* dma to help avoid a possible race */
	buffer_info->time_stamp = jiffies;
	buffer_info->next_to_watch = i;
	buffer_info->dma = dma_map_single(tx_ring->dev, skb->data, hlen,
					  DMA_TO_DEVICE);
	if (dma_mapping_error(tx_ring->dev, buffer_info->dma))
		goto dma_error;

	for (f = 0; f < skb_shinfo(skb)->nr_frags; f++) {
		struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[f];
		unsigned int len = frag->size;

		count++;
		i++;
		if (i == tx_ring->count)
			i = 0;

		buffer_info = &tx_ring->buffer_info[i];
		buffer_info->length = len;
		buffer_info->time_stamp = jiffies;
		buffer_info->next_to_watch = i;
		buffer_info->mapped_as_page = true;
		buffer_info->dma = dma_map_page(tx_ring->dev,
						frag->page,
						frag->page_offset,
						len,
						DMA_TO_DEVICE);
		if (dma_mapping_error(tx_ring->dev, buffer_info->dma))
			goto dma_error;
	}

	tx_ring->buffer_info[i].skb = skb;
#ifdef HAVE_HW_TIME_STAMP
#ifdef SKB_SHARED_TX_IS_UNION
	tx_ring->buffer_info[i].shtx = skb_shinfo(skb)->tx_flags.flags;
#else
	tx_ring->buffer_info[i].shtx = skb_shinfo(skb)->tx_flags;
#endif
#endif
#ifdef NETIF_F_TSO
	/* multiply data chunks by size of headers */
	tx_ring->buffer_info[i].bytecount = ((gso_segs - 1) * hlen) + skb->len;
	tx_ring->buffer_info[i].gso_segs = gso_segs;
#else
	tx_ring->buffer_info[i].bytecount = skb->len;
#endif
	tx_ring->buffer_info[first].next_to_watch = i;

	return ++count;

dma_error:
	dev_err(tx_ring->dev, "TX DMA map failed\n");

	/* clear timestamp and dma mappings for failed buffer_info mapping */
	memset(buffer_info, 0, sizeof(struct igb_buffer));

	/* clear timestamp and dma mappings for remaining portion of packet */
	while (count--) {
		if (i == 0)
			i = tx_ring->count;
		i--;
		buffer_info = &tx_ring->buffer_info[i];
		igb_unmap_and_free_tx_resource(tx_ring, buffer_info);
	}

	return 0;
}

static inline void igb_tx_queue(struct igb_ring *tx_ring,
				u32 tx_flags, int count, u32 paylen,
				u8 hdr_len)
{
	union e1000_adv_tx_desc *tx_desc;
	u32 olinfo_status = 0, cmd_type_len;
	u16 i;

	cmd_type_len = (E1000_ADVTXD_DTYP_DATA | E1000_ADVTXD_DCMD_IFCS |
			E1000_ADVTXD_DCMD_DEXT);

	if (tx_flags & IGB_TX_FLAGS_VLAN)
		cmd_type_len |= E1000_ADVTXD_DCMD_VLE;

	if (tx_flags & IGB_TX_FLAGS_TSTAMP)
		cmd_type_len |= E1000_ADVTXD_MAC_TSTAMP;

	if (tx_flags & IGB_TX_FLAGS_TSO) {
		cmd_type_len |= E1000_ADVTXD_DCMD_TSE;

		/* insert tcp checksum */
		olinfo_status |= E1000_TXD_POPTS_TXSM << 8;

		/* insert ip checksum */
		if (tx_flags & IGB_TX_FLAGS_IPV4)
			olinfo_status |= E1000_TXD_POPTS_IXSM << 8;

	} else if (tx_flags & IGB_TX_FLAGS_CSUM) {
		olinfo_status |= E1000_TXD_POPTS_TXSM << 8;
	}

	if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags) &&
	    (tx_flags & (IGB_TX_FLAGS_CSUM |
	                 IGB_TX_FLAGS_TSO |
			 IGB_TX_FLAGS_VLAN)))
		olinfo_status |= tx_ring->reg_idx << 4;

	olinfo_status |= ((paylen - hdr_len) << E1000_ADVTXD_PAYLEN_SHIFT);

	i = tx_ring->next_to_use;
	do {
		struct igb_buffer *buffer_info = &tx_ring->buffer_info[i];
		tx_desc = IGB_TX_DESC(tx_ring, i);
		tx_desc->read.buffer_addr = cpu_to_le64(buffer_info->dma);
		tx_desc->read.cmd_type_len =
			cpu_to_le32(cmd_type_len | buffer_info->length);
		tx_desc->read.olinfo_status = cpu_to_le32(olinfo_status);
		count--;
		i++;
		if (i == tx_ring->count)
			i = 0;
	} while (count > 0);
	tx_ring->next_to_use = i;

	tx_desc->read.cmd_type_len |= cpu_to_le32(IGB_TXD_DCMD);
	/* Force memory writes to complete before letting h/w
	 * know there are new descriptors to fetch.  (Only
	 * applicable for weak-ordered memory model archs,
	 * such as IA-64). */
	wmb();

	writel(i, tx_ring->tail);
	/* we need this if more than one processor can write to our tail
	 * at a time, it syncronizes IO on IA64/Altix systems */
	mmiowb();
}

static int __igb_maybe_stop_tx(struct igb_ring *tx_ring, const u16 size)
{
	struct net_device *netdev = netdev_ring(tx_ring);

	if (netif_is_multiqueue(netdev))
		netif_stop_subqueue(netdev, ring_queue_index(tx_ring));
	else
		netif_stop_queue(netdev);

	/* Herbert's original patch had:
	 *  smp_mb__after_netif_stop_queue();
	 * but since that doesn't exist yet, just open code it. */
	smp_mb();

	/* We need to check again in a case another CPU has just
	 * made room available. */
	if (igb_desc_unused(tx_ring) < size)
		return -EBUSY;

	/* A reprieve! */
	if (netif_is_multiqueue(netdev))
		netif_wake_subqueue(netdev, ring_queue_index(tx_ring));
	else
		netif_wake_queue(netdev);

	tx_ring->tx_stats.restart_queue++;

	return 0;
}

static inline int igb_maybe_stop_tx(struct igb_ring *tx_ring, const u16 size)
{
	if (igb_desc_unused(tx_ring) >= size)
		return 0;
	return __igb_maybe_stop_tx(tx_ring, size);
}

netdev_tx_t igb_xmit_frame_ring(struct sk_buff *skb,
				struct igb_ring *tx_ring)
{
	int tso = 0, count;
	u32 tx_flags = 0;
	u16 first;
	u8 hdr_len = 0;

	/* need: 1 descriptor per page,
	 *       + 2 desc gap to keep tail from touching head,
	 *       + 1 desc for skb->data,
	 *       + 1 desc for context descriptor,
	 * otherwise try next time */
	if (igb_maybe_stop_tx(tx_ring, skb_shinfo(skb)->nr_frags + 4)) {
		/* this is a hard error */
		return NETDEV_TX_BUSY;
	}

#ifdef HAVE_HW_TIME_STAMP
#ifdef SKB_SHARED_TX_IS_UNION
	if (unlikely(skb_shinfo(skb)->tx_flags.flags & SKBTX_HW_TSTAMP)) {
		skb_shinfo(skb)->tx_flags.flags |= SKBTX_IN_PROGRESS;
		tx_flags |= IGB_TX_FLAGS_TSTAMP;
	}
#else
	if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP)) {
		skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;
		tx_flags |= IGB_TX_FLAGS_TSTAMP;
	}
#endif

#endif
	if (vlan_tx_tag_present(skb)) {
		tx_flags |= IGB_TX_FLAGS_VLAN;
		tx_flags |= (vlan_tx_tag_get(skb) << IGB_TX_FLAGS_VLAN_SHIFT);
	}

	if (skb->protocol == htons(ETH_P_IP))
		tx_flags |= IGB_TX_FLAGS_IPV4;

	first = tx_ring->next_to_use;
#ifdef NETIF_F_TSO
	if (skb_is_gso(skb)) {
		tso = igb_tso(tx_ring, skb, tx_flags, &hdr_len);

		if (tso < 0) {
			dev_kfree_skb_any(skb);
			return NETDEV_TX_OK;
		}
	}

#endif
	if (tso)
		tx_flags |= IGB_TX_FLAGS_TSO;
	else if (igb_tx_csum(tx_ring, skb, tx_flags) &&
	         (skb->ip_summed == CHECKSUM_PARTIAL))
		tx_flags |= IGB_TX_FLAGS_CSUM;

	/*
	 * count reflects descriptors mapped, if 0 or less then mapping error
	 * has occured and we need to rewind the descriptor queue
	 */
	count = igb_tx_map(tx_ring, skb, first);
	if (!count) {
		dev_kfree_skb_any(skb);
		tx_ring->buffer_info[first].time_stamp = 0;
		tx_ring->next_to_use = first;
		return NETDEV_TX_OK;
	}

	igb_tx_queue(tx_ring, tx_flags, count, skb->len, hdr_len);

#ifndef HAVE_TRANS_START_IN_QUEUE
	netdev_ring(tx_ring)->trans_start = jiffies;

#endif
	/* Make sure there is space in the ring for the next send. */
	igb_maybe_stop_tx(tx_ring, MAX_SKB_FRAGS + 4);

	return NETDEV_TX_OK;
}

#ifdef HAVE_TX_MQ
static inline struct igb_ring *igb_tx_queue_mapping(struct igb_adapter *adapter,
                                                    struct sk_buff *skb)
{
	unsigned int r_idx = skb->queue_mapping & (IGB_MAX_TX_QUEUES - 1);

	if (r_idx >= adapter->num_tx_queues)
		r_idx = r_idx % adapter->num_tx_queues;

	return adapter->tx_ring[r_idx];
}
#else
#define igb_tx_queue_mapping(_adapter, _skb) (_adapter)->tx_ring[0]
#endif

static netdev_tx_t igb_xmit_frame(struct sk_buff *skb,
                                      struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	if (test_bit(__IGB_DOWN, &adapter->state)) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	if (skb->len <= 0) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	/*
	 * The minimum packet size with TCTL.PSP set is 17 so pad the skb
	 * in order to meet this minimum size requirement.
	 */
	if (skb->len < 17) {
		if (skb_padto(skb, 17))
			return NETDEV_TX_OK;
		skb->len = 17;
	}

	/* This goes back to the question of how to logically map a tx queue
	 * to a flow.  Right now, performance is impacted slightly negatively
	 * if using multiple tx queues.  If the stack breaks away from a
	 * single qdisc implementation, we can look at this again. */
	return igb_xmit_frame_ring(skb, igb_tx_queue_mapping(adapter, skb));
}

/**
 * igb_tx_timeout - Respond to a Tx Hang
 * @netdev: network interface device structure
 **/
static void igb_tx_timeout(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;

	/* Do the reset outside of interrupt context */
	adapter->tx_timeout_count++;

	if (hw->mac.type >= e1000_82580)
		hw->dev_spec._82575.global_device_reset = true;

	schedule_work(&adapter->reset_task);
	E1000_WRITE_REG(hw, E1000_EICS,
			(adapter->eims_enable_mask & ~adapter->eims_other));
}

static void igb_reset_task(struct work_struct *work)
{
	struct igb_adapter *adapter;
	adapter = container_of(work, struct igb_adapter, reset_task);

	igb_reinit_locked(adapter);
}

/**
 * igb_get_stats - Get System Network Statistics
 * @netdev: network interface device structure
 *
 * Returns the address of the device statistics structure.
 * The statistics are actually updated from the timer callback.
 **/
static struct net_device_stats *igb_get_stats(struct net_device *netdev)
{
#ifdef HAVE_NETDEV_STATS_IN_NETDEV
	/* only return the current stats */
	return &netdev->stats;
#else
	struct igb_adapter *adapter = netdev_priv(netdev);

	/* only return the current stats */
	return &adapter->net_stats;
#endif /* HAVE_NETDEV_STATS_IN_NETDEV */
}

/**
 * igb_change_mtu - Change the Maximum Transfer Unit
 * @netdev: network interface device structure
 * @new_mtu: new value for maximum frame size
 *
 * Returns 0 on success, negative on failure
 **/
static int igb_change_mtu(struct net_device *netdev, int new_mtu)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct pci_dev *pdev = adapter->pdev;
	int max_frame = new_mtu + ETH_HLEN + ETH_FCS_LEN + VLAN_TAG_SIZE;
#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
	u32 rx_buffer_len, i;
#endif

	if ((new_mtu < 68) || (max_frame > MAX_JUMBO_FRAME_SIZE)) {
		dev_err(pci_dev_to_dev(pdev), "Invalid MTU setting\n");
		return -EINVAL;
	}

#define MAX_STD_JUMBO_FRAME_SIZE 9238
	if (max_frame > MAX_STD_JUMBO_FRAME_SIZE) {
		dev_err(pci_dev_to_dev(pdev), "MTU > 9216 not supported.\n");
		return -EINVAL;
	}

	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		msleep(1);

	/* igb_down has a dependency on max_frame_size */
	adapter->max_frame_size = max_frame;

#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
#ifdef IGB_PER_PKT_TIMESTAMP
	if (adapter->hw.mac.type >= e1000_82580)
		max_frame += IGB_TS_HDR_LEN;

#endif
	/*
	 * RLPML prevents us from receiving a frame larger than max_frame so
	 * it is safe to just set the rx_buffer_len to max_frame without the
	 * risk of an skb over panic.
	 */
	if (max_frame <= MAXIMUM_ETHERNET_VLAN_SIZE)
		rx_buffer_len = MAXIMUM_ETHERNET_VLAN_SIZE;
	else
		rx_buffer_len = max_frame;

#endif
	if (netif_running(netdev))
		igb_down(adapter);

	dev_info(pci_dev_to_dev(pdev), "changing MTU from %d to %d\n",
	        netdev->mtu, new_mtu);
	netdev->mtu = new_mtu;

#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
	for (i = 0; i < adapter->num_rx_queues; i++)
		adapter->rx_ring[i]->rx_buffer_len = rx_buffer_len;

#endif
	if (netif_running(netdev))
		igb_up(adapter);
	else
		igb_reset(adapter);

	clear_bit(__IGB_RESETTING, &adapter->state);

	return 0;
}

/**
 * igb_update_stats - Update the board statistics counters
 * @adapter: board private structure
 **/

void igb_update_stats(struct igb_adapter *adapter)
{
	struct net_device_stats *net_stats = igb_get_stats(adapter->netdev);
	struct e1000_hw *hw = &adapter->hw;
#ifdef HAVE_PCI_ERS
	struct pci_dev *pdev = adapter->pdev;
#endif
	u32 reg, mpc;
	u16 phy_tmp;
	int i;
	u64 bytes, packets;
#ifdef IGB_LRO
	u32 flushed = 0, coal = 0, recycled = 0;
	struct igb_q_vector *q_vector;
#endif

#define PHY_IDLE_ERROR_COUNT_MASK 0x00FF

	/*
	 * Prevent stats update while adapter is being reset, or if the pci
	 * connection is down.
	 */
	if (adapter->link_speed == 0)
		return;
#ifdef HAVE_PCI_ERS
	if (pci_channel_offline(pdev))
		return;

#endif
#ifdef IGB_LRO
	for (i = 0; i < adapter->num_q_vectors; i++) {
		q_vector = adapter->q_vector[i];
		if (!q_vector || !q_vector->lrolist)
			continue;
		flushed += q_vector->lrolist->stats.flushed;
		coal += q_vector->lrolist->stats.coal;
		recycled += q_vector->lrolist->stats.recycled;
	}
	adapter->lro_stats.flushed = flushed;
	adapter->lro_stats.coal = coal;
	adapter->lro_stats.recycled = recycled;

#endif
	bytes = 0;
	packets = 0;
	for (i = 0; i < adapter->num_rx_queues; i++) {
		u32 rqdpc_tmp = E1000_READ_REG(hw, E1000_RQDPC(i)) & 0x0FFF;
		struct igb_ring *ring = adapter->rx_ring[i];
		ring->rx_stats.drops += rqdpc_tmp;
		net_stats->rx_fifo_errors += rqdpc_tmp;
#ifdef CONFIG_IGB_VMDQ_NETDEV
		if (!ring->vmdq_netdev) {
			bytes += ring->rx_stats.bytes;
			packets += ring->rx_stats.packets;
		}
#else
		bytes += ring->rx_stats.bytes;
		packets += ring->rx_stats.packets;
#endif
	}

	net_stats->rx_bytes = bytes;
	net_stats->rx_packets = packets;

	bytes = 0;
	packets = 0;
	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct igb_ring *ring = adapter->tx_ring[i];
#ifdef CONFIG_IGB_VMDQ_NETDEV
		if (!ring->vmdq_netdev) {
			bytes += ring->tx_stats.bytes;
			packets += ring->tx_stats.packets;
		}
#else
		bytes += ring->tx_stats.bytes;
		packets += ring->tx_stats.packets;
#endif
	}
	net_stats->tx_bytes = bytes;
	net_stats->tx_packets = packets;

	/* read stats registers */
	adapter->stats.crcerrs += E1000_READ_REG(hw, E1000_CRCERRS);
	adapter->stats.gprc += E1000_READ_REG(hw, E1000_GPRC);
	adapter->stats.gorc += E1000_READ_REG(hw, E1000_GORCL);
	E1000_READ_REG(hw, E1000_GORCH); /* clear GORCL */
	adapter->stats.bprc += E1000_READ_REG(hw, E1000_BPRC);
	adapter->stats.mprc += E1000_READ_REG(hw, E1000_MPRC);
	adapter->stats.roc += E1000_READ_REG(hw, E1000_ROC);

	adapter->stats.prc64 += E1000_READ_REG(hw, E1000_PRC64);
	adapter->stats.prc127 += E1000_READ_REG(hw, E1000_PRC127);
	adapter->stats.prc255 += E1000_READ_REG(hw, E1000_PRC255);
	adapter->stats.prc511 += E1000_READ_REG(hw, E1000_PRC511);
	adapter->stats.prc1023 += E1000_READ_REG(hw, E1000_PRC1023);
	adapter->stats.prc1522 += E1000_READ_REG(hw, E1000_PRC1522);
	adapter->stats.symerrs += E1000_READ_REG(hw, E1000_SYMERRS);
	adapter->stats.sec += E1000_READ_REG(hw, E1000_SEC);

	mpc = E1000_READ_REG(hw, E1000_MPC);
	adapter->stats.mpc += mpc;
	net_stats->rx_fifo_errors += mpc;
	adapter->stats.scc += E1000_READ_REG(hw, E1000_SCC);
	adapter->stats.ecol += E1000_READ_REG(hw, E1000_ECOL);
	adapter->stats.mcc += E1000_READ_REG(hw, E1000_MCC);
	adapter->stats.latecol += E1000_READ_REG(hw, E1000_LATECOL);
	adapter->stats.dc += E1000_READ_REG(hw, E1000_DC);
	adapter->stats.rlec += E1000_READ_REG(hw, E1000_RLEC);
	adapter->stats.xonrxc += E1000_READ_REG(hw, E1000_XONRXC);
	adapter->stats.xontxc += E1000_READ_REG(hw, E1000_XONTXC);
	adapter->stats.xoffrxc += E1000_READ_REG(hw, E1000_XOFFRXC);
	adapter->stats.xofftxc += E1000_READ_REG(hw, E1000_XOFFTXC);
	adapter->stats.fcruc += E1000_READ_REG(hw, E1000_FCRUC);
	adapter->stats.gptc += E1000_READ_REG(hw, E1000_GPTC);
	adapter->stats.gotc += E1000_READ_REG(hw, E1000_GOTCL);
	E1000_READ_REG(hw, E1000_GOTCH); /* clear GOTCL */
	adapter->stats.rnbc += E1000_READ_REG(hw, E1000_RNBC);
	adapter->stats.ruc += E1000_READ_REG(hw, E1000_RUC);
	adapter->stats.rfc += E1000_READ_REG(hw, E1000_RFC);
	adapter->stats.rjc += E1000_READ_REG(hw, E1000_RJC);
	adapter->stats.tor += E1000_READ_REG(hw, E1000_TORH);
	adapter->stats.tot += E1000_READ_REG(hw, E1000_TOTH);
	adapter->stats.tpr += E1000_READ_REG(hw, E1000_TPR);

	adapter->stats.ptc64 += E1000_READ_REG(hw, E1000_PTC64);
	adapter->stats.ptc127 += E1000_READ_REG(hw, E1000_PTC127);
	adapter->stats.ptc255 += E1000_READ_REG(hw, E1000_PTC255);
	adapter->stats.ptc511 += E1000_READ_REG(hw, E1000_PTC511);
	adapter->stats.ptc1023 += E1000_READ_REG(hw, E1000_PTC1023);
	adapter->stats.ptc1522 += E1000_READ_REG(hw, E1000_PTC1522);

	adapter->stats.mptc += E1000_READ_REG(hw, E1000_MPTC);
	adapter->stats.bptc += E1000_READ_REG(hw, E1000_BPTC);

	adapter->stats.tpt += E1000_READ_REG(hw, E1000_TPT);
	adapter->stats.colc += E1000_READ_REG(hw, E1000_COLC);

	adapter->stats.algnerrc += E1000_READ_REG(hw, E1000_ALGNERRC);
	/* read internal phy sepecific stats */
	reg = E1000_READ_REG(hw, E1000_CTRL_EXT);
	if (!(reg & E1000_CTRL_EXT_LINK_MODE_MASK)) {
		adapter->stats.rxerrc += E1000_READ_REG(hw, E1000_RXERRC);
		adapter->stats.tncrs += E1000_READ_REG(hw, E1000_TNCRS);
	}

	adapter->stats.tsctc += E1000_READ_REG(hw, E1000_TSCTC);
	adapter->stats.tsctfc += E1000_READ_REG(hw, E1000_TSCTFC);

	adapter->stats.iac += E1000_READ_REG(hw, E1000_IAC);
	adapter->stats.icrxoc += E1000_READ_REG(hw, E1000_ICRXOC);
	adapter->stats.icrxptc += E1000_READ_REG(hw, E1000_ICRXPTC);
	adapter->stats.icrxatc += E1000_READ_REG(hw, E1000_ICRXATC);
	adapter->stats.ictxptc += E1000_READ_REG(hw, E1000_ICTXPTC);
	adapter->stats.ictxatc += E1000_READ_REG(hw, E1000_ICTXATC);
	adapter->stats.ictxqec += E1000_READ_REG(hw, E1000_ICTXQEC);
	adapter->stats.ictxqmtc += E1000_READ_REG(hw, E1000_ICTXQMTC);
	adapter->stats.icrxdmtc += E1000_READ_REG(hw, E1000_ICRXDMTC);

	/* Fill out the OS statistics structure */
	net_stats->multicast = adapter->stats.mprc;
	net_stats->collisions = adapter->stats.colc;

	/* Rx Errors */

	/* RLEC on some newer hardware can be incorrect so build
	 * our own version based on RUC and ROC */
	net_stats->rx_errors = adapter->stats.rxerrc +
		adapter->stats.crcerrs + adapter->stats.algnerrc +
		adapter->stats.ruc + adapter->stats.roc +
		adapter->stats.cexterr;
	net_stats->rx_length_errors = adapter->stats.ruc +
				      adapter->stats.roc;
	net_stats->rx_crc_errors = adapter->stats.crcerrs;
	net_stats->rx_frame_errors = adapter->stats.algnerrc;
	net_stats->rx_missed_errors = adapter->stats.mpc;

	/* Tx Errors */
	net_stats->tx_errors = adapter->stats.ecol +
			       adapter->stats.latecol;
	net_stats->tx_aborted_errors = adapter->stats.ecol;
	net_stats->tx_window_errors = adapter->stats.latecol;
	net_stats->tx_carrier_errors = adapter->stats.tncrs;

	/* Tx Dropped needs to be maintained elsewhere */

	/* Phy Stats */
	if (hw->phy.media_type == e1000_media_type_copper) {
		if ((adapter->link_speed == SPEED_1000) &&
		   (!e1000_read_phy_reg(hw, PHY_1000T_STATUS, &phy_tmp))) {
			phy_tmp &= PHY_IDLE_ERROR_COUNT_MASK;
			adapter->phy_stats.idle_errors += phy_tmp;
		}
	}

	/* Management Stats */
	adapter->stats.mgptc += E1000_READ_REG(hw, E1000_MGTPTC);
	adapter->stats.mgprc += E1000_READ_REG(hw, E1000_MGTPRC);
	adapter->stats.mgpdc += E1000_READ_REG(hw, E1000_MGTPDC);
	adapter->stats.o2bgptc += E1000_READ_REG(hw, E1000_O2BGPTC);
	adapter->stats.o2bspc += E1000_READ_REG(hw, E1000_O2BSPC);
	adapter->stats.b2ospc += E1000_READ_REG(hw, E1000_B2OSPC);
	adapter->stats.b2ogprc += E1000_READ_REG(hw, E1000_B2OGPRC);
}

static irqreturn_t igb_msix_other(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct e1000_hw *hw = &adapter->hw;
	u32 icr = E1000_READ_REG(hw, E1000_ICR);
	/* reading ICR causes bit 31 of EICR to be cleared */

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
		/* The DMA Out of Sync is also indication of a spoof event
		 * in IOV mode. Check the Wrong VM Behavior register to
		 * see if it is really a spoof event. */
		igb_check_wvbr(adapter);
	}

	/* Check for a mailbox event */
	if (icr & E1000_ICR_VMMB)
		igb_msg_task(adapter);

	if (icr & E1000_ICR_LSC) {
		hw->mac.get_link_status = 1;
		/* guard against interrupt when we're going down */
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

	E1000_WRITE_REG(hw, E1000_EIMS, adapter->eims_other);

	return IRQ_HANDLED;
}

static void igb_write_itr(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	u32 itr_val = q_vector->itr_val & 0x7FFC;

	if (!q_vector->set_itr)
		return;

	if (!itr_val)
		itr_val = 0x4;

	if (adapter->hw.mac.type == e1000_82575)
		itr_val |= itr_val << 16;
	else
		itr_val |= E1000_EITR_CNT_IGNR;

	writel(itr_val, q_vector->itr_register);
	q_vector->set_itr = 0;
}

static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	/* Write the ITR value calculated from the previous interrupt. */
	igb_write_itr(q_vector);

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

#ifdef IGB_DCA
static void igb_update_dca(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	int cpu = get_cpu();

	if (q_vector->cpu == cpu)
		goto out_no_update;

	if (q_vector->tx_ring) {
		int q = q_vector->tx_ring->reg_idx;
		u32 dca_txctrl = E1000_READ_REG(hw, E1000_DCA_TXCTRL(q));
		if (hw->mac.type == e1000_82575) {
			dca_txctrl &= ~E1000_DCA_TXCTRL_CPUID_MASK;
			dca_txctrl |= dca3_get_tag(&adapter->pdev->dev, cpu);
		} else {
			dca_txctrl &= ~E1000_DCA_TXCTRL_CPUID_MASK_82576;
			dca_txctrl |= dca3_get_tag(&adapter->pdev->dev, cpu) <<
			              E1000_DCA_TXCTRL_CPUID_SHIFT_82576;
		}
		dca_txctrl |= E1000_DCA_TXCTRL_DESC_DCA_EN;
		E1000_WRITE_REG(hw, E1000_DCA_TXCTRL(q), dca_txctrl);
	}
	if (q_vector->rx_ring) {
		int q = q_vector->rx_ring->reg_idx;
		u32 dca_rxctrl = E1000_READ_REG(hw, E1000_DCA_RXCTRL(q));
		if (hw->mac.type == e1000_82575) {
			dca_rxctrl &= ~E1000_DCA_RXCTRL_CPUID_MASK;
			dca_rxctrl |= dca3_get_tag(&adapter->pdev->dev, cpu);
		} else {
			dca_rxctrl &= ~E1000_DCA_RXCTRL_CPUID_MASK_82576;
			dca_rxctrl |= dca3_get_tag(&adapter->pdev->dev, cpu) <<
			              E1000_DCA_RXCTRL_CPUID_SHIFT_82576;
		}
		dca_rxctrl |= E1000_DCA_RXCTRL_DESC_DCA_EN;
		dca_rxctrl |= E1000_DCA_RXCTRL_HEAD_DCA_EN;
		dca_rxctrl |= E1000_DCA_RXCTRL_DATA_DCA_EN;
		E1000_WRITE_REG(hw, E1000_DCA_RXCTRL(q), dca_rxctrl);
	}
	q_vector->cpu = cpu;
out_no_update:
	put_cpu();
}

static void igb_setup_dca(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	if (!(adapter->flags & IGB_FLAG_DCA_ENABLED))
		return;

	/* Always use CB2 mode, difference is masked in the CB driver. */
	E1000_WRITE_REG(hw, E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_MODE_CB2);

	for (i = 0; i < adapter->num_q_vectors; i++) {
		adapter->q_vector[i]->cpu = -1;
		igb_update_dca(adapter->q_vector[i]);
	}
}

static int __igb_notify_dca(struct device *dev, void *data)
{
	struct net_device *netdev = dev_get_drvdata(dev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;
	unsigned long event = *(unsigned long *)data;

	switch (event) {
	case DCA_PROVIDER_ADD:
		/* if already enabled, don't do it again */
		if (adapter->flags & IGB_FLAG_DCA_ENABLED)
			break;
		if (dca_add_requester(dev) == E1000_SUCCESS) {
			adapter->flags |= IGB_FLAG_DCA_ENABLED;
			dev_info(pci_dev_to_dev(pdev), "DCA enabled\n");
			igb_setup_dca(adapter);
			break;
		}
		/* Fall Through since DCA is disabled. */
	case DCA_PROVIDER_REMOVE:
		if (adapter->flags & IGB_FLAG_DCA_ENABLED) {
			/* without this a class_device is left
			 * hanging around in the sysfs model */
			dca_remove_requester(dev);
			dev_info(pci_dev_to_dev(pdev), "DCA disabled\n");
			adapter->flags &= ~IGB_FLAG_DCA_ENABLED;
			E1000_WRITE_REG(hw, E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_DISABLE);
		}
		break;
	}

	return E1000_SUCCESS;
}

static int igb_notify_dca(struct notifier_block *nb, unsigned long event,
                          void *p)
{
	int ret_val;

	ret_val = driver_for_each_device(&igb_driver.driver, NULL, &event,
	                                 __igb_notify_dca);

	return ret_val ? NOTIFY_BAD : NOTIFY_DONE;
}
#endif /* IGB_DCA */

static void igb_ping_all_vfs(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ping;
	int i;

	for (i = 0 ; i < adapter->vfs_allocated_count; i++) {
		ping = E1000_PF_CONTROL_MSG;
		if (adapter->vf_data[i].flags & IGB_VF_FLAG_CTS)
			ping |= E1000_VT_MSGTYPE_CTS;
		e1000_write_mbx(hw, &ping, 1, i);
	}
}

/**
 *  igb_mta_set_ - Set multicast filter table address
 *  @adapter: pointer to the adapter structure
 *  @hash_value: determines the MTA register and bit to set
 *
 *  The multicast table address is a register array of 32-bit registers.
 *  The hash_value is used to determine what register the bit is in, the
 *  current value is read, the new bit is OR'd in and the new value is
 *  written back into the register.
 **/
void igb_mta_set(struct igb_adapter *adapter, u32 hash_value)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 hash_bit, hash_reg, mta;

	/*
	 * The MTA is a register array of 32-bit registers. It is
	 * treated like an array of (32*mta_reg_count) bits.  We want to
	 * set bit BitArray[hash_value]. So we figure out what register
	 * the bit is in, read it, OR in the new bit, then write
	 * back the new value.  The (hw->mac.mta_reg_count - 1) serves as a
	 * mask to bits 31:5 of the hash value which gives us the
	 * register we're modifying.  The hash bit within that register
	 * is determined by the lower 5 bits of the hash value.
	 */
	hash_reg = (hash_value >> 5) & (hw->mac.mta_reg_count - 1);
	hash_bit = hash_value & 0x1F;

	mta = E1000_READ_REG_ARRAY(hw, E1000_MTA, hash_reg);

	mta |= (1 << hash_bit);

	E1000_WRITE_REG_ARRAY(hw, E1000_MTA, hash_reg, mta);
	E1000_WRITE_FLUSH(hw);
}

static int igb_set_vf_promisc(struct igb_adapter *adapter, u32 *msgbuf, u32 vf)
{

	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr = E1000_READ_REG(hw, E1000_VMOLR(vf));
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];

	vf_data->flags &= ~(IGB_VF_FLAG_UNI_PROMISC |
	                    IGB_VF_FLAG_MULTI_PROMISC);
	vmolr &= ~(E1000_VMOLR_ROPE | E1000_VMOLR_ROMPE | E1000_VMOLR_MPME);

#ifdef IGB_ENABLE_VF_PROMISC
	if (*msgbuf & E1000_VF_SET_PROMISC_UNICAST) {
		vmolr |= E1000_VMOLR_ROPE;
		vf_data->flags |= IGB_VF_FLAG_UNI_PROMISC;
		*msgbuf &= ~E1000_VF_SET_PROMISC_UNICAST;
	}
#endif
	if (*msgbuf & E1000_VF_SET_PROMISC_MULTICAST) {
		vmolr |= E1000_VMOLR_MPME;
		vf_data->flags |= IGB_VF_FLAG_MULTI_PROMISC;
		*msgbuf &= ~E1000_VF_SET_PROMISC_MULTICAST;
	} else {
		/*
		 * if we have hashes and we are clearing a multicast promisc
		 * flag we need to write the hashes to the MTA as this step
		 * was previously skipped
		 */
		if (vf_data->num_vf_mc_hashes > 30) {
			vmolr |= E1000_VMOLR_MPME;
		} else if (vf_data->num_vf_mc_hashes) {
			int j;
			vmolr |= E1000_VMOLR_ROMPE;
			for (j = 0; j < vf_data->num_vf_mc_hashes; j++)
				igb_mta_set(adapter, vf_data->vf_mc_hashes[j]);
		}
	}

	E1000_WRITE_REG(hw, E1000_VMOLR(vf), vmolr);

	/* there are flags left unprocessed, likely not supported */
	if (*msgbuf & E1000_VT_MSGINFO_MASK)
		return -EINVAL;

	return 0;

}

static int igb_set_vf_multicasts(struct igb_adapter *adapter,
				  u32 *msgbuf, u32 vf)
{
	int n = (msgbuf[0] & E1000_VT_MSGINFO_MASK) >> E1000_VT_MSGINFO_SHIFT;
	u16 *hash_list = (u16 *)&msgbuf[1];
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	int i;

	/* salt away the number of multicast addresses assigned
	 * to this VF for later use to restore when the PF multi cast
	 * list changes
	 */
	vf_data->num_vf_mc_hashes = n;

	/* only up to 30 hash values supported */
	if (n > 30)
		n = 30;

	/* store the hashes for later use */
	for (i = 0; i < n; i++)
		vf_data->vf_mc_hashes[i] = hash_list[i];

	/* Flush and reset the mta with the new values */
	igb_set_rx_mode(adapter->netdev);

	return 0;
}

static void igb_restore_vf_multicasts(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data;
	int i, j;

	for (i = 0; i < adapter->vfs_allocated_count; i++) {
		u32 vmolr = E1000_READ_REG(hw, E1000_VMOLR(i));
		vmolr &= ~(E1000_VMOLR_ROMPE | E1000_VMOLR_MPME);

		vf_data = &adapter->vf_data[i];

		if ((vf_data->num_vf_mc_hashes > 30) ||
		    (vf_data->flags & IGB_VF_FLAG_MULTI_PROMISC)) {
			vmolr |= E1000_VMOLR_MPME;
		} else if (vf_data->num_vf_mc_hashes) {
			vmolr |= E1000_VMOLR_ROMPE;
			for (j = 0; j < vf_data->num_vf_mc_hashes; j++)
				igb_mta_set(adapter, vf_data->vf_mc_hashes[j]);
		}
		E1000_WRITE_REG(hw, E1000_VMOLR(i), vmolr);
	}
}

static void igb_clear_vf_vfta(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 pool_mask, reg, vid;
	u16 vlan_default;
	int i;

	pool_mask = 1 << (E1000_VLVF_POOLSEL_SHIFT + vf);

	/* Find the vlan filter for this id */
	for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
		reg = E1000_READ_REG(hw, E1000_VLVF(i));

		/* remove the vf from the pool */
		reg &= ~pool_mask;

		/* if pool is empty then remove entry from vfta */
		if (!(reg & E1000_VLVF_POOLSEL_MASK) &&
		    (reg & E1000_VLVF_VLANID_ENABLE)) {
			reg = 0;
			vid = reg & E1000_VLVF_VLANID_MASK;
			igb_vfta_set(hw, vid, FALSE);
		}

		E1000_WRITE_REG(hw, E1000_VLVF(i), reg);
	}

	adapter->vf_data[vf].vlans_enabled = 0;

	vlan_default = adapter->vf_data[vf].default_vf_vlan_id;
	if (vlan_default)
		igb_vlvf_set(adapter, vlan_default, true, vf);
}

s32 igb_vlvf_set(struct igb_adapter *adapter, u32 vid, bool add, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 reg, i;

	/* The vlvf table only exists on 82576 hardware and newer */
	if (hw->mac.type < e1000_82576)
		return -1;

	/* we only need to do this if VMDq is enabled */
	if (!adapter->vmdq_pools)
		return -1;

	/* Find the vlan filter for this id */
	for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
		reg = E1000_READ_REG(hw, E1000_VLVF(i));
		if ((reg & E1000_VLVF_VLANID_ENABLE) &&
		    vid == (reg & E1000_VLVF_VLANID_MASK))
			break;
	}

	if (add) {
		if (i == E1000_VLVF_ARRAY_SIZE) {
			/* Did not find a matching VLAN ID entry that was
			 * enabled.  Search for a free filter entry, i.e.
			 * one without the enable bit set
			 */
			for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
				reg = E1000_READ_REG(hw, E1000_VLVF(i));
				if (!(reg & E1000_VLVF_VLANID_ENABLE))
					break;
			}
		}
		if (i < E1000_VLVF_ARRAY_SIZE) {
			/* Found an enabled/available entry */
			reg |= 1 << (E1000_VLVF_POOLSEL_SHIFT + vf);

			/* if !enabled we need to set this up in vfta */
			if (!(reg & E1000_VLVF_VLANID_ENABLE)) {
				/* add VID to filter table */
				igb_vfta_set(hw, vid, TRUE);
				reg |= E1000_VLVF_VLANID_ENABLE;
			}
			reg &= ~E1000_VLVF_VLANID_MASK;
			reg |= vid;
			E1000_WRITE_REG(hw, E1000_VLVF(i), reg);

			/* do not modify RLPML for PF devices */
			if (vf >= adapter->vfs_allocated_count)
				return E1000_SUCCESS;

			if (!adapter->vf_data[vf].vlans_enabled) {
				u32 size;
				reg = E1000_READ_REG(hw, E1000_VMOLR(vf));
				size = reg & E1000_VMOLR_RLPML_MASK;
				size += 4;
				reg &= ~E1000_VMOLR_RLPML_MASK;
				reg |= size;
				E1000_WRITE_REG(hw, E1000_VMOLR(vf), reg);
			}

			adapter->vf_data[vf].vlans_enabled++;
			return E1000_SUCCESS;
		}
	} else {
		if (i < E1000_VLVF_ARRAY_SIZE) {
			/* remove vf from the pool */
			reg &= ~(1 << (E1000_VLVF_POOLSEL_SHIFT + vf));
			/* if pool is empty then remove entry from vfta */
			if (!(reg & E1000_VLVF_POOLSEL_MASK)) {
				reg = 0;
				igb_vfta_set(hw, vid, FALSE);
			}
			E1000_WRITE_REG(hw, E1000_VLVF(i), reg);

			/* do not modify RLPML for PF devices */
			if (vf >= adapter->vfs_allocated_count)
				return E1000_SUCCESS;

			adapter->vf_data[vf].vlans_enabled--;
			if (!adapter->vf_data[vf].vlans_enabled) {
				u32 size;
				reg = E1000_READ_REG(hw, E1000_VMOLR(vf));
				size = reg & E1000_VMOLR_RLPML_MASK;
				size -= 4;
				reg &= ~E1000_VMOLR_RLPML_MASK;
				reg |= size;
				E1000_WRITE_REG(hw, E1000_VMOLR(vf), reg);
			}
			return E1000_SUCCESS;
		}
	}
	return E1000_SUCCESS;
}

#ifdef IFLA_VF_MAX
static void igb_set_vmvir(struct igb_adapter *adapter, u32 vid, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;

	if (vid)
		E1000_WRITE_REG(hw, E1000_VMVIR(vf), (vid | E1000_VMVIR_VLANA_DEFAULT));
	else
		E1000_WRITE_REG(hw, E1000_VMVIR(vf), 0);
}

static int igb_ndo_set_vf_vlan(struct net_device *netdev,
			       int vf, u16 vlan, u8 qos)
{
	int err = 0;
	struct igb_adapter *adapter = netdev_priv(netdev);

	/* VLAN IDs accepted range 0-4094 */
	if ((vf >= adapter->vfs_allocated_count) || (vlan > VLAN_VID_MASK-1) || (qos > 7))
		return -EINVAL;
	if (vlan || qos) {
		err = igb_vlvf_set(adapter, vlan, !!vlan, vf);
		if (err)
			goto out;
		igb_set_vmvir(adapter, vlan | (qos << VLAN_PRIO_SHIFT), vf);
		igb_set_vmolr(adapter, vf, !vlan);
		adapter->vf_data[vf].pf_vlan = vlan;
		adapter->vf_data[vf].pf_qos = qos;
		igb_set_vf_vlan_strip(adapter, vf, true); 
		dev_info(&adapter->pdev->dev,
			 "Setting VLAN %d, QOS 0x%x on VF %d\n", vlan, qos, vf);
		if (test_bit(__IGB_DOWN, &adapter->state)) {
			dev_warn(&adapter->pdev->dev,
				 "The VF VLAN has been set,"
				 " but the PF device is not up.\n");
			dev_warn(&adapter->pdev->dev,
				 "Bring the PF device up before"
				 " attempting to use the VF device.\n");
		}
	} else {
		igb_vlvf_set(adapter, adapter->vf_data[vf].pf_vlan,
				   false, vf);
		igb_set_vmvir(adapter, vlan, vf);
		igb_set_vmolr(adapter, vf, true);
		igb_set_vf_vlan_strip(adapter, vf, false); 
		adapter->vf_data[vf].pf_vlan = 0;
		adapter->vf_data[vf].pf_qos = 0;
       }
out:
       return err;
}
#endif

static int igb_set_vf_vlan(struct igb_adapter *adapter, u32 *msgbuf, u32 vf)
{
	int add = (msgbuf[0] & E1000_VT_MSGINFO_MASK) >> E1000_VT_MSGINFO_SHIFT;
	int vid = (msgbuf[1] & E1000_VLVF_VLANID_MASK);
	
	if (vid)
		igb_set_vf_vlan_strip(adapter, vf, true); 
	else
		igb_set_vf_vlan_strip(adapter, vf, false); 
	return igb_vlvf_set(adapter, vid, add, vf);
}

static inline void igb_vf_reset(struct igb_adapter *adapter, u32 vf)
{
	/* clear flags except flag that the PF has set the MAC */
	adapter->vf_data[vf].flags &= IGB_VF_FLAG_PF_SET_MAC;
	adapter->vf_data[vf].last_nack = jiffies;

	/* reset offloads to defaults */
	igb_set_vmolr(adapter, vf, true);

	/* reset vlans for device */
	igb_clear_vf_vfta(adapter, vf);
#ifdef IFLA_VF_MAX
	if (adapter->vf_data[vf].pf_vlan)
		igb_ndo_set_vf_vlan(adapter->netdev, vf,
				    adapter->vf_data[vf].pf_vlan,
				    adapter->vf_data[vf].pf_qos);
	else
		igb_clear_vf_vfta(adapter, vf);
#endif

	/* reset multicast table array for vf */
	adapter->vf_data[vf].num_vf_mc_hashes = 0;

	/* Flush and reset the mta with the new values */
	igb_set_rx_mode(adapter->netdev);
}

static void igb_vf_reset_event(struct igb_adapter *adapter, u32 vf)
{
	unsigned char *vf_mac = adapter->vf_data[vf].vf_mac_addresses;

	/* generate a new mac address as we were hotplug removed/added */
	if (!(adapter->vf_data[vf].flags & IGB_VF_FLAG_PF_SET_MAC))
		random_ether_addr(vf_mac);

	/* process remaining reset events */
	igb_vf_reset(adapter, vf);
}

static void igb_vf_reset_msg(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	unsigned char *vf_mac = adapter->vf_data[vf].vf_mac_addresses;
	u32 reg, msgbuf[3];
	u8 *addr = (u8 *)(&msgbuf[1]);

	/* process all the same items cleared in a function level reset */
	igb_vf_reset(adapter, vf);

	/* set vf mac address */
	igb_del_mac_filter(adapter, vf_mac, vf);
	igb_add_mac_filter(adapter, vf_mac, vf);

	/* enable transmit and receive for vf */
	reg = E1000_READ_REG(hw, E1000_VFTE);
	E1000_WRITE_REG(hw, E1000_VFTE, reg | (1 << vf));
	reg = E1000_READ_REG(hw, E1000_VFRE);
	E1000_WRITE_REG(hw, E1000_VFRE, reg | (1 << vf));

	adapter->vf_data[vf].flags |= IGB_VF_FLAG_CTS;

	/* reply to reset with ack and vf mac address */
	msgbuf[0] = E1000_VF_RESET | E1000_VT_MSGTYPE_ACK;
	memcpy(addr, vf_mac, 6);
	e1000_write_mbx(hw, msgbuf, 3, vf);
}

static int igb_set_vf_mac_addr(struct igb_adapter *adapter, u32 *msg, int vf)
{
	/*
	 * The VF MAC Address is stored in a packed array of bytes
	 * starting at the second 32 bit word of the msg array
	 */
	unsigned char *addr = (unsigned char *)&msg[1];
	int err = -1;

	if (is_valid_ether_addr(addr))
		err = igb_set_vf_mac(adapter, vf, addr);

	return err;
}

static void igb_rcv_ack_from_vf(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	u32 msg = E1000_VT_MSGTYPE_NACK;

	/* if device isn't clear to send it shouldn't be reading either */
	if (!(vf_data->flags & IGB_VF_FLAG_CTS) &&
	    time_after(jiffies, vf_data->last_nack + (2 * HZ))) {
		e1000_write_mbx(hw, &msg, 1, vf);
		vf_data->last_nack = jiffies;
	}
}

static void igb_rcv_msg_from_vf(struct igb_adapter *adapter, u32 vf)
{
	struct pci_dev *pdev = adapter->pdev;
	u32 msgbuf[E1000_VFMAILBOX_SIZE];
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	s32 retval;

	retval = e1000_read_mbx(hw, msgbuf, E1000_VFMAILBOX_SIZE, vf);

	if (retval) {
		dev_err(pci_dev_to_dev(pdev), "Error receiving message from VF\n");
		return;
	}

	/* this is a message we already processed, do nothing */
	if (msgbuf[0] & (E1000_VT_MSGTYPE_ACK | E1000_VT_MSGTYPE_NACK))
		return;

	/*
	 * until the vf completes a reset it should not be
	 * allowed to start any configuration.
	 */

	if (msgbuf[0] == E1000_VF_RESET) {
		igb_vf_reset_msg(adapter, vf);
		return;
	}

	if (!(vf_data->flags & IGB_VF_FLAG_CTS)) {
		msgbuf[0] = E1000_VT_MSGTYPE_NACK;
		if (time_after(jiffies, vf_data->last_nack + (2 * HZ))) {
			e1000_write_mbx(hw, msgbuf, 1, vf);
			vf_data->last_nack = jiffies;
		}
		return;
	}

	switch ((msgbuf[0] & 0xFFFF)) {
	case E1000_VF_SET_MAC_ADDR:
		retval = -EINVAL;
#ifndef IGB_DISABLE_VF_MAC_SET
		if (!(vf_data->flags & IGB_VF_FLAG_PF_SET_MAC))
			retval = igb_set_vf_mac_addr(adapter, msgbuf, vf);
		else
			DPRINTK(DRV, INFO,
				"VF %d attempted to override administratively "
				"set MAC address\nReload the VF driver to "
				"resume operations\n", vf);
#endif
		break;
	case E1000_VF_SET_PROMISC:
		retval = igb_set_vf_promisc(adapter, msgbuf, vf);
		break;
	case E1000_VF_SET_MULTICAST:
		retval = igb_set_vf_multicasts(adapter, msgbuf, vf);
		break;
	case E1000_VF_SET_LPE:
		retval = igb_set_vf_rlpml(adapter, msgbuf[1], vf);
		break;
	case E1000_VF_SET_VLAN:
		retval = -1;
#ifdef IFLA_VF_MAX
		if (vf_data->pf_vlan)
			DPRINTK(DRV, INFO,
				"VF %d attempted to override administratively "
				"set VLAN tag\nReload the VF driver to "
				"resume operations\n", vf);
		else
#endif
			retval = igb_set_vf_vlan(adapter, msgbuf, vf);
		break;
	default:
		dev_err(pci_dev_to_dev(pdev), "Unhandled Msg %08x\n", msgbuf[0]);
		retval = -E1000_ERR_MBX;
		break;
	}

	/* notify the VF of the results of what it sent us */
	if (retval)
		msgbuf[0] |= E1000_VT_MSGTYPE_NACK;
	else
		msgbuf[0] |= E1000_VT_MSGTYPE_ACK;

	msgbuf[0] |= E1000_VT_MSGTYPE_CTS;

	e1000_write_mbx(hw, msgbuf, 1, vf);
}

static void igb_msg_task(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vf;

	for (vf = 0; vf < adapter->vfs_allocated_count; vf++) {
		/* process any reset requests */
		if (!e1000_check_for_rst(hw, vf))
			igb_vf_reset_event(adapter, vf);

		/* process any messages pending */
		if (!e1000_check_for_msg(hw, vf))
			igb_rcv_msg_from_vf(adapter, vf);

		/* process any acks */
		if (!e1000_check_for_ack(hw, vf))
			igb_rcv_ack_from_vf(adapter, vf);
	}
}

/**
 *  igb_set_uta - Set unicast filter table address
 *  @adapter: board private structure
 *
 *  The unicast table address is a register array of 32-bit registers.
 *  The table is meant to be used in a way similar to how the MTA is used
 *  however due to certain limitations in the hardware it is necessary to
 *  set all the hash bits to 1 and use the VMOLR ROPE bit as a promiscous
 *  enable bit to allow vlan tag stripping when promiscous mode is enabled
 **/
static void igb_set_uta(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	/* The UTA table only exists on 82576 hardware and newer */
	if (hw->mac.type < e1000_82576)
		return;

	/* we only need to do this if VMDq is enabled */
	if (!adapter->vmdq_pools)
		return;

	for (i = 0; i < hw->mac.uta_reg_count; i++)
		E1000_WRITE_REG_ARRAY(hw, E1000_UTA, i, ~0);
}

/**
 * igb_intr_msi - Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a network interface device structure
 **/
static irqreturn_t igb_intr_msi(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct igb_q_vector *q_vector = adapter->q_vector[0];
	struct e1000_hw *hw = &adapter->hw;
	/* read ICR disables interrupts using IAM */
	u32 icr = E1000_READ_REG(hw, E1000_ICR);

	igb_write_itr(q_vector);

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
	}

	if (icr & (E1000_ICR_RXSEQ | E1000_ICR_LSC)) {
		hw->mac.get_link_status = 1;
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

/**
 * igb_intr - Legacy Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a network interface device structure
 **/
static irqreturn_t igb_intr(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct igb_q_vector *q_vector = adapter->q_vector[0];
	struct e1000_hw *hw = &adapter->hw;
	/* Interrupt Auto-Mask...upon reading ICR, interrupts are masked.  No
	 * need for the IMC write */
	u32 icr = E1000_READ_REG(hw, E1000_ICR);

	/* IMS will not auto-mask if INT_ASSERTED is not set, and if it is
	 * not set, then the adapter didn't send an interrupt */
	if (!(icr & E1000_ICR_INT_ASSERTED))
		return IRQ_NONE;

	igb_write_itr(q_vector);

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
	}

	if (icr & (E1000_ICR_RXSEQ | E1000_ICR_LSC)) {
		hw->mac.get_link_status = 1;
		/* guard against interrupt when we're going down */
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

void igb_ring_irq_enable(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;

	if ((q_vector->rx_ring && (adapter->rx_itr_setting & 3)) ||
	    (!q_vector->rx_ring && (adapter->tx_itr_setting & 3))) {
		if ((adapter->num_q_vectors == 1) && !adapter->vf_data)
			igb_set_itr(q_vector);
		else
			igb_update_ring_itr(q_vector);
	}

	if (!test_bit(__IGB_DOWN, &adapter->state)) {
		if (adapter->msix_entries)
			E1000_WRITE_REG(hw, E1000_EIMS, q_vector->eims_value);
		else
			igb_irq_enable(adapter);
	}
}

/**
 * igb_poll - NAPI Rx polling callback
 * @napi: napi polling structure
 * @budget: count of how many packets we should handle
 **/
static int igb_poll(struct napi_struct *napi, int budget)
{
	struct igb_q_vector *q_vector = container_of(napi, struct igb_q_vector, napi);
	int tx_clean_complete = 1, work_done = 0;

#ifdef IGB_DCA
	if (q_vector->adapter->flags & IGB_FLAG_DCA_ENABLED)
		igb_update_dca(q_vector);
#endif
	if (q_vector->tx_ring)
		tx_clean_complete = igb_clean_tx_irq(q_vector, budget);

	if (q_vector->rx_ring)
		igb_clean_rx_irq(q_vector, &work_done, budget);

	if (!tx_clean_complete)
		work_done = budget;

#ifndef HAVE_NETDEV_NAPI_LIST
	/* if netdev is disabled we need to stop polling */
	if (!netif_running(q_vector->adapter->netdev))
		work_done = 0;

#endif
	/* If not enough Rx work done, exit the polling mode */
	if (work_done < budget) {
		napi_complete(napi);
		igb_ring_irq_enable(q_vector);
	}

	return work_done;
}

#ifdef HAVE_HW_TIME_STAMP
/**
 * igb_systim_to_hwtstamp - convert system time value to hw timestamp
 * @adapter: board private structure
 * @shhwtstamps: timestamp structure to update
 * @regval: unsigned 64bit system time value.
 *
 * We need to convert the system time value stored in the RX/TXSTMP registers
 * into a hwtstamp which can be used by the upper level timestamping functions
 */
static void igb_systim_to_hwtstamp(struct igb_adapter *adapter,
                                   struct skb_shared_hwtstamps *shhwtstamps,
                                   u64 regval)
{
	u64 ns;

	/*
	 * The 82580 starts with 1ns at bit 0 in RX/TXSTMPL, shift this up to
	 * 24 to match clock shift we setup earlier.
	 */
	if (adapter->hw.mac.type >= e1000_82580)
		regval <<= IGB_82580_TSYNC_SHIFT;

	ns = timecounter_cyc2time(&adapter->clock, regval);
	timecompare_update(&adapter->compare, ns);
	memset(shhwtstamps, 0, sizeof(struct skb_shared_hwtstamps));
	shhwtstamps->hwtstamp = ns_to_ktime(ns);
	shhwtstamps->syststamp = timecompare_transform(&adapter->compare, ns);
}

/**
 * igb_tx_hwtstamp - utility function which checks for TX time stamp
 * @q_vector: pointer to q_vector containing needed info
 * @buffer: pointer to igb_buffer structure
 *
 * If we were asked to do hardware stamping and such a time stamp is
 * available, then it must have been for this skb here because we only
 * allow only one such packet into the queue.
 */
static void igb_tx_hwtstamp(struct igb_q_vector *q_vector, struct igb_buffer *buffer_info)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	struct skb_shared_hwtstamps shhwtstamps;
	u64 regval;

	/* if skb does not support hw timestamp or TX stamp not valid exit */
	if (likely(!buffer_info->shtx & SKBTX_HW_TSTAMP) ||
	    !(E1000_READ_REG(hw, E1000_TSYNCTXCTL) & E1000_TSYNCTXCTL_VALID))
		return;

	regval = E1000_READ_REG(hw, E1000_TXSTMPL);
	regval |= (u64)E1000_READ_REG(hw, E1000_TXSTMPH) << 32;

	igb_systim_to_hwtstamp(adapter, &shhwtstamps, regval);
	skb_tstamp_tx(buffer_info->skb, &shhwtstamps);
}

#endif
/**
 * igb_clean_tx_irq - Reclaim resources after transmit completes
 * @q_vector: pointer to q_vector containing needed info
 * returns TRUE if ring is completely cleaned
 **/
static bool igb_clean_tx_irq(struct igb_q_vector *q_vector, int budget)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct igb_ring *tx_ring = q_vector->tx_ring;
	union e1000_adv_tx_desc *eop_desc;
	unsigned int total_bytes = 0, total_packets = 0;
	int count = 0;
	u16 i = tx_ring->next_to_clean;
	u16 eop = tx_ring->buffer_info[i].next_to_watch;

	eop_desc = IGB_TX_DESC(tx_ring, eop);

	while (eop_desc->wb.status & __constant_cpu_to_le32(E1000_TXD_STAT_DD)) {
		bool cleaned = false;
		while (!cleaned) {
			struct igb_buffer *buffer_info = &tx_ring->buffer_info[i];
			union e1000_adv_tx_desc *tx_desc = IGB_TX_DESC(tx_ring, i);

			tx_desc->wb.status = 0;
			cleaned = (i == eop);

			i++;
			if (i == tx_ring->count)
				i = 0;

			if (buffer_info->skb) {
				total_bytes += buffer_info->bytecount;
#ifdef NETIF_F_TSO
				/* gso_segs is currently only valid for tcp */
				total_packets += buffer_info->gso_segs;
#else
				total_packets++;
#endif
#ifdef HAVE_HW_TIME_STAMP
				igb_tx_hwtstamp(q_vector, buffer_info);
#endif
			}

			igb_unmap_and_free_tx_resource(tx_ring, buffer_info);
		}
		count++;
		if (count >= budget)
			break;
		eop = tx_ring->buffer_info[i].next_to_watch;
		eop_desc = IGB_TX_DESC(tx_ring, eop);
	}

	tx_ring->next_to_clean = i;

	if (unlikely(count &&
		     netif_carrier_ok(netdev_ring(tx_ring)) &&
		     igb_desc_unused(tx_ring) >= IGB_TX_QUEUE_WAKE)) {
		/* Make sure that anybody stopping the queue after this
		 * sees the new next_to_clean.
		 */
		smp_mb();
		if (netif_is_multiqueue(netdev_ring(tx_ring))) {
			if (__netif_subqueue_stopped(netdev_ring(tx_ring), ring_queue_index(tx_ring)) &&
			    !(test_bit(__IGB_DOWN, &adapter->state))) {
				netif_wake_subqueue(netdev_ring(tx_ring), ring_queue_index(tx_ring));
				tx_ring->tx_stats.restart_queue++;
			}
		} else {
			if (netif_queue_stopped(netdev_ring(tx_ring)) &&
			    !(test_bit(__IGB_DOWN, &adapter->state))) {
				netif_wake_queue(netdev_ring(tx_ring));
				tx_ring->tx_stats.restart_queue++;
			}
		}
	}

	if (test_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags)) {
		struct e1000_hw *hw = &adapter->hw;

		/* Detect a transmit hang in hardware, this serializes the
		 * check with the clearing of time_stamp and movement of i */
		clear_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags);
		if (tx_ring->buffer_info[i].time_stamp &&
		    time_after(jiffies, tx_ring->buffer_info[i].time_stamp +
		               (adapter->tx_timeout_factor * HZ))
		    && !(E1000_READ_REG(hw, E1000_STATUS) &
		         E1000_STATUS_TXOFF)) {

			/* detected Tx unit hang */
			dev_err(tx_ring->dev,
				"Detected Tx Unit Hang\n"
				"  Tx Queue             <%d>\n"
				"  TDH                  <%x>\n"
				"  TDT                  <%x>\n"
				"  next_to_use          <%x>\n"
				"  next_to_clean        <%x>\n"
				"buffer_info[next_to_clean]\n"
				"  time_stamp           <%lx>\n"
				"  next_to_watch        <%x>\n"
				"  jiffies              <%lx>\n"
				"  desc.status          <%x>\n",
				tx_ring->queue_index,
				E1000_READ_REG(hw, E1000_TDH(tx_ring->reg_idx)),
				readl(tx_ring->tail),
				tx_ring->next_to_use,
				tx_ring->next_to_clean,
				tx_ring->buffer_info[eop].time_stamp,
				eop,
				jiffies,
				eop_desc->wb.status);
			if (netif_is_multiqueue(netdev_ring(tx_ring)))
				netif_stop_subqueue(netdev_ring(tx_ring),
						    ring_queue_index(tx_ring));
			else
				netif_stop_queue(netdev_ring(tx_ring));
		}
	}
	tx_ring->total_bytes += total_bytes;
	tx_ring->total_packets += total_packets;
	tx_ring->tx_stats.bytes += total_bytes;
	tx_ring->tx_stats.packets += total_packets;
	return (count < budget);
}

/**
 * igb_receive_skb - helper function to handle rx indications
 * @q_vector: structure containing interrupt and ring information
 * @skb: packet to send up
 * @vlan_tag: vlan tag for packet
 **/
static
#ifdef HAVE_PF_RING
int
#else
void
#endif
igb_receive_skb(struct igb_q_vector *q_vector,
                            struct sk_buff *skb,
                            u16 vlan_tag)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct vlan_group *vlgrp = adapter->vlgrp;

#ifdef HAVE_PF_RING
        {
          int debug = 0;
          struct pfring_hooks *hook = (struct pfring_hooks*)skb->dev->pfring_ptr;

          if(hook && (hook->magic == PF_RING)) {
            /* Wow: PF_RING is alive & kickin' ! */
            int rc;

            if(debug)
              printk(KERN_INFO "[PF_RING] alive [%s][len=%d]\n",
                     skb->dev->name, skb->len);

            // printk(KERN_INFO "[PF_RING] queue_index=%d\n", ring->queue_index);                                                                                        

            if(*hook->transparent_mode != standard_linux_path) {
              rc = hook->ring_handler(skb, 1, 1, q_vector->rx_ring->queue_index, adapter->num_rx_queues);

              if(rc > 0 /* Packet handled by PF_RING */) {
                if(*hook->transparent_mode == driver2pf_ring_non_transparent) {
                  /* PF_RING has already freed the memory */
                  return(rc);
                }
              }
            } else {
              if(debug) printk(KERN_INFO "[PF_RING] not present on %s\n",
                               skb->dev->name);
            }
          }
        }
#endif

#ifdef CONFIG_IGB_VMDQ_NETDEV
	if (q_vector->rx_ring->vmdq_netdev) {
		struct igb_vmdq_adapter *vadapt;
		vadapt = netdev_priv(q_vector->rx_ring->vmdq_netdev);
		vlgrp = vadapt->vlgrp;
	}
#endif
	if (vlan_tag && vlgrp)
		vlan_gro_receive(&q_vector->napi, vlgrp,
		                 vlan_tag, skb);
	else
		napi_gro_receive(&q_vector->napi, skb);

#ifdef HAVE_PF_RING
        return(0);
#endif
}

static inline void igb_rx_checksum(struct igb_ring *ring,
                                   u32 status_err, struct sk_buff *skb)
{
	/* Ignore Checksum bit is set or checksum is disabled through ethtool */
	if (!test_bit(IGB_RING_FLAG_RX_CSUM, &ring->flags) ||
	     (status_err & E1000_RXD_STAT_IXSM))
		return;

	/* TCP/UDP checksum error bit is set */
	if (status_err &
	    (E1000_RXDEXT_STATERR_TCPE | E1000_RXDEXT_STATERR_IPE)) {
		/*
		 * work around errata with sctp packets where the TCPE aka
		 * L4E bit is set incorrectly on 64 byte (60 byte w/o crc)
		 * packets, (aka let the stack check the crc32c)
		 */
		if (!((skb->len == 60) &&
		      test_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags)))
			ring->rx_stats.csum_err++;

		/* let the stack verify checksum errors */
		return;
	}
	/* It must be a TCP or UDP packet with a valid checksum */
	if (status_err & (E1000_RXD_STAT_TCPCS | E1000_RXD_STAT_UDPCS))
		skb->ip_summed = CHECKSUM_UNNECESSARY;
}

#ifdef HAVE_HW_TIME_STAMP
static void igb_rx_hwtstamp(struct igb_q_vector *q_vector, u32 staterr,
                                   struct sk_buff *skb)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	u64 regval;

	/*
	 * If this bit is set, then the RX registers contain the time stamp. No
	 * other packet will be time stamped until we read these registers, so
	 * read the registers to make them available again. Because only one
	 * packet can be time stamped at a time, we know that the register
	 * values must belong to this one here and therefore we don't need to
	 * compare any of the additional attributes stored for it.
	 *
	 * If nothing went wrong, then it should have a skb_shared_tx that we
	 * can turn into a skb_shared_hwtstamps.
	 */
	if (staterr & E1000_RXDADV_STAT_TSIP) {
		u32 *stamp = (u32 *)skb->data;
		regval = le32_to_cpu(*(stamp + 2));
		regval |= (u64)le32_to_cpu(*(stamp + 3)) << 32;
		skb_pull(skb, IGB_TS_HDR_LEN);
	} else {
		if(!(E1000_READ_REG(hw, E1000_TSYNCRXCTL) & E1000_TSYNCRXCTL_VALID))
			return;

		regval = E1000_READ_REG(hw, E1000_RXSTMPL);
		regval |= (u64)E1000_READ_REG(hw, E1000_RXSTMPH) << 32;
	}

	igb_systim_to_hwtstamp(adapter, skb_hwtstamps(skb), regval);
}
#endif
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
static inline u16 igb_get_hlen(union e1000_adv_rx_desc *rx_desc)
{
	/* HW will not DMA in data larger than the given buffer, even if it
	 * parses the (NFS, of course) header to be larger.  In that case, it
	 * fills the header buffer and spills the rest into the page.
	 */
	u16 hlen = (le16_to_cpu(rx_desc->wb.lower.lo_dword.hs_rss.hdr_info) &
	           E1000_RXDADV_HDRBUFLEN_MASK) >> E1000_RXDADV_HDRBUFLEN_SHIFT;
	if (hlen > IGB_RX_HDR_LEN)
		hlen = IGB_RX_HDR_LEN;
	return hlen;
}

#endif
#ifdef IGB_LRO
/**
 * igb_transform_rsc_queue - change rsc queue into a full packet
 * @skb: pointer to the last skb in the rsc queue
 *
 * This function changes a queue full of hw rsc buffers into a completed
 * packet.  It uses the ->prev pointers to find the first packet and then
 * turns it into the frag list owner.
 **/
static inline struct sk_buff *igb_transform_rsc_queue(struct sk_buff *skb)
{
	unsigned int frag_list_size = 0;

	while (skb->prev) {
		struct sk_buff *prev = skb->prev;
		frag_list_size += skb->len;
		skb->prev = NULL;
		skb = prev;
	}

	skb_shinfo(skb)->frag_list = skb->next;
	skb->next = NULL;
	skb->len += frag_list_size;
	skb->data_len += frag_list_size;
	skb->truesize += frag_list_size;
	return skb;
}

/**
 * igb_can_lro - returns true if packet is TCP/IPV4 and LRO is enabled
 * @adapter: board private structure
 * @rx_desc: pointer to the rx descriptor
 * @skb: pointer to the skb to be merged
 *
 **/
static inline bool igb_can_lro(struct igb_ring *rx_ring,
			       union e1000_adv_rx_desc *rx_desc,
			       struct sk_buff *skb)
{
	u16 pkt_info = le16_to_cpu(rx_desc->wb.lower.lo_dword.hs_rss.pkt_info);

	return (test_bit(IGB_RING_FLAG_RX_LRO, &rx_ring->flags) &&
		(skb->protocol == __constant_htons(ETH_P_IP)) &&
		(pkt_info & E1000_RXDADV_PKTTYPE_IPV4) &&
		(pkt_info & E1000_RXDADV_PKTTYPE_TCP));
}

/**
 * igb_lro_flush - Indicate packets to upper layer.
 *
 * Update IP and TCP header part of head skb if more than one
 * skb's chained and indicate packets to upper layer.
 **/
static void igb_lro_flush(struct igb_q_vector *q_vector,
                                 struct igb_lro_desc *lrod)
{
	struct igb_lro_list *lrolist = q_vector->lrolist;
	struct iphdr *iph;
	struct tcphdr *th;
	struct sk_buff *skb;
	u32 *ts_ptr;

	hlist_del(&lrod->lro_node);
	lrolist->active_cnt--;

	skb = lrod->skb;
	lrod->skb = NULL;

	if (lrod->append_cnt) {
		/* take the lro queue and convert to skb format */
		skb = igb_transform_rsc_queue(skb);

		/* incorporate ip header and re-calculate checksum */
		iph = (struct iphdr *)skb->data;
		iph->tot_len = ntohs(skb->len);
		iph->check = 0;
		iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

		/* incorporate the latest ack into the tcp header */
		th = (struct tcphdr *) ((char *)skb->data + sizeof(*iph));
		th->ack_seq = lrod->ack_seq;
		th->psh = lrod->psh;
		th->window = lrod->window;
		th->check = 0;

		/* incorporate latest timestamp into the tcp header */
		if (lrod->opt_bytes) {
			ts_ptr = (u32 *)(th + 1);
			ts_ptr[1] = htonl(lrod->tsval);
			ts_ptr[2] = lrod->tsecr;
		}
	}

#ifdef NETIF_F_TSO
	skb_shinfo(skb)->gso_size = lrod->mss;
#endif
	igb_receive_skb(q_vector, skb, lrod->vlan_tag);
	lrolist->stats.flushed++;


	hlist_add_head(&lrod->lro_node, &lrolist->free);
}

static void igb_lro_flush_all(struct igb_q_vector *q_vector)
{
	struct igb_lro_desc *lrod;
	struct hlist_node *node, *node2;
	struct igb_lro_list *lrolist = q_vector->lrolist;

	hlist_for_each_entry_safe(lrod, node, node2, &lrolist->active, lro_node)
		igb_lro_flush(q_vector, lrod);
}

/*
 * igb_lro_header_ok - Main LRO function.
 **/
static u16 igb_lro_header_ok(struct sk_buff *new_skb, struct iphdr *iph,
                               struct tcphdr *th)
{
	int opt_bytes, tcp_data_len;
	u32 *ts_ptr = NULL;

	/* If we see CE codepoint in IP header, packet is not mergeable */
	if (INET_ECN_is_ce(ipv4_get_dsfield(iph)))
		return -1;

	/* ensure there are no options */
	if ((iph->ihl << 2) != sizeof(*iph))
		return -1;

	/* .. and the packet is not fragmented */
	if (iph->frag_off & htons(IP_MF|IP_OFFSET))
		return -1;

	/* ensure no bits set besides ack or psh */
	if (th->fin || th->syn || th->rst ||
	    th->urg || th->ece || th->cwr || !th->ack)
		return -1;

	/* ensure that the checksum is valid */
	if (new_skb->ip_summed != CHECKSUM_UNNECESSARY)
		return -1;

	/*
	 * check for timestamps. Since the only option we handle are timestamps,
	 * we only have to handle the simple case of aligned timestamps
	 */

	opt_bytes = (th->doff << 2) - sizeof(*th);
	if (opt_bytes != 0) {
		ts_ptr = (u32 *)(th + 1);
		if ((opt_bytes != TCPOLEN_TSTAMP_ALIGNED) ||
			(*ts_ptr != ntohl((TCPOPT_NOP << 24) |
			(TCPOPT_NOP << 16) | (TCPOPT_TIMESTAMP << 8) |
			TCPOLEN_TIMESTAMP))) {
			return -1;
		}
	}

	tcp_data_len = ntohs(iph->tot_len) - (th->doff << 2) - sizeof(*iph);

	return tcp_data_len;
}

/**
 * igb_lro_queue - if able, queue skb into lro chain
 * @q_vector: structure containing interrupt and ring information
 * @new_skb: pointer to current skb being checked
 * @tag: vlan tag for skb
 *
 * Checks whether the skb given is eligible for LRO and if that's
 * fine chains it to the existing lro_skb based on flowid. If an LRO for
 * the flow doesn't exist create one.
 **/
static struct sk_buff *igb_lro_queue(struct igb_q_vector *q_vector,
                                       struct sk_buff *new_skb,
				       u16 tag)
{
	struct sk_buff *lro_skb;
	struct igb_lro_desc *lrod;
	struct hlist_node *node;
	struct skb_shared_info *new_skb_info = skb_shinfo(new_skb);
	struct igb_lro_list *lrolist = q_vector->lrolist;
	struct iphdr *iph = (struct iphdr *)new_skb->data;
	struct tcphdr *th = (struct tcphdr *)(iph + 1);
	int tcp_data_len = igb_lro_header_ok(new_skb, iph, th);
	u16  opt_bytes = (th->doff << 2) - sizeof(*th);
	u32 *ts_ptr = (opt_bytes ? (u32 *)(th + 1) : NULL);
	u32 seq = ntohl(th->seq);

	/*
	 * we have a packet that might be eligible for LRO,
	 * so see if it matches anything we might expect
	 */
	hlist_for_each_entry(lrod, node, &lrolist->active, lro_node) {
		if (lrod->source_port != th->source ||
			lrod->dest_port != th->dest ||
			lrod->source_ip != iph->saddr ||
			lrod->dest_ip != iph->daddr ||
			lrod->vlan_tag != tag)
			continue;

		/*
		 * malformed header, no tcp data, resultant packet would
		 * be too large, ack sequence numbers do not match, window
		 * size has changed, or new skb is larger than our current mss.
		 * If any of the above we should flush the lro descriptor and
		 * start over if possible
		 */
		if (tcp_data_len <= 0 || (tcp_data_len + lrod->len) > 65521 ||
		    lrod->ack_seq != th->ack_seq ||
		    lrod->window != th->window ||
		    lrod->mss < tcp_data_len) {
			igb_lro_flush(q_vector, lrod);
			break;
		}

		/* out of order packet */
		if (seq != lrod->next_seq) {
			igb_lro_flush(q_vector, lrod);
			tcp_data_len = -1;
			break;
		}

		/* packet without timestamp, or timestamp suddenly added to flow */
		if (lrod->opt_bytes != opt_bytes) {
			igb_lro_flush(q_vector, lrod);
			break;
		}

		if (opt_bytes) {
			u32 tsval = ntohl(*(ts_ptr + 1));
			/* make sure timestamp values are increasing */
			if (opt_bytes != lrod->opt_bytes ||
			    lrod->tsval > tsval || *(ts_ptr + 2) == 0) {
				igb_lro_flush(q_vector, lrod);
				tcp_data_len = -1;
				break;
			}

			lrod->tsval = tsval;
			lrod->tsecr = *(ts_ptr + 2);
		}

		/* remove any padding from the end of the skb */
		__pskb_trim(new_skb, ntohs(iph->tot_len));
		/* Remove IP and TCP header*/
		skb_pull(new_skb, ntohs(iph->tot_len) - tcp_data_len);

		lrod->next_seq += tcp_data_len;
		lrod->len += tcp_data_len;
		lrod->psh |= th->psh;
		lrod->append_cnt++;
		lrolist->stats.coal++;
		lro_skb = lrod->skb;

#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
		/* if header is empty pull pages into current skb */
		if (!skb_headlen(new_skb) &&
		    ((skb_shinfo(lro_skb)->nr_frags +
		      skb_shinfo(new_skb)->nr_frags) <= MAX_SKB_FRAGS )) {
			struct skb_shared_info *lro_skb_info = skb_shinfo(lro_skb);

			/* copy frags into the last skb */
			memcpy(lro_skb_info->frags + lro_skb_info->nr_frags,
			       new_skb_info->frags,
			       new_skb_info->nr_frags * sizeof(skb_frag_t));

			lro_skb_info->nr_frags += new_skb_info->nr_frags;
			lro_skb->len += tcp_data_len;
			lro_skb->data_len += tcp_data_len;
			lro_skb->truesize += tcp_data_len;

			new_skb_info->nr_frags = 0;
			new_skb->truesize -= tcp_data_len;
			new_skb->len = new_skb->data_len = 0;
			new_skb->data = skb_mac_header(new_skb);
			skb_reset_tail_pointer(new_skb);
			new_skb->protocol = 0;
			lrolist->stats.recycled++;
		} else {
#endif
			/* Chain this new skb in frag_list */
			new_skb->prev = lro_skb;
			lro_skb->next = new_skb;
			lrod->skb = new_skb ;
			new_skb = NULL;
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
		}
#endif

		if (lrod->psh || (tcp_data_len < lrod->mss))
			igb_lro_flush(q_vector, lrod);

		return new_skb;
	}

	/* start a new packet */
	if (tcp_data_len > 0 && !hlist_empty(&lrolist->free) && !th->psh) {
		lrod = hlist_entry(lrolist->free.first, struct igb_lro_desc,
		                   lro_node);

		/* remove any padding from the end of the skb */
		__pskb_trim(new_skb, ntohs(iph->tot_len));

		lrod->skb = new_skb;
		lrod->source_ip = iph->saddr;
		lrod->dest_ip = iph->daddr;
		lrod->source_port = th->source;
		lrod->dest_port = th->dest;
		lrod->vlan_tag = tag;
		lrod->len = new_skb->len;
		lrod->next_seq = seq + tcp_data_len;
		lrod->ack_seq = th->ack_seq;
		lrod->window = th->window;
		lrod->mss = tcp_data_len;
		lrod->opt_bytes = opt_bytes;
		lrod->psh = 0;
		lrod->append_cnt = 0;

		/* record timestamp if it is present */
		if (opt_bytes) {
			lrod->tsval = ntohl(*(ts_ptr + 1));
			lrod->tsecr = *(ts_ptr + 2);
		}
		/* remove first packet from freelist.. */
		hlist_del(&lrod->lro_node);
		/* .. and insert at the front of the active list */
		hlist_add_head(&lrod->lro_node, &lrolist->active);
		lrolist->active_cnt++;
		lrolist->stats.coal++;
		return NULL;
	}

	/* packet not handled by any of the above, pass it to the stack */
	igb_receive_skb(q_vector, new_skb, tag);
	return NULL;
}

#endif /* IGB_LRO */
static void igb_clean_rx_irq(struct igb_q_vector *q_vector,
                             int *work_done, int budget)
{
	struct igb_ring *rx_ring = q_vector->rx_ring;
	union e1000_adv_rx_desc *rx_desc;
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
	const int current_node = numa_node_id();
#endif
	unsigned int total_bytes = 0, total_packets = 0;
	u32 staterr;
	u16 cleaned_count = 0;
	u16 i = rx_ring->next_to_clean;

	rx_desc = IGB_RX_DESC(rx_ring, i);
	staterr = le32_to_cpu(rx_desc->wb.upper.status_error);

	while (staterr & E1000_RXD_STAT_DD) {
		struct igb_buffer *buffer_info = &rx_ring->buffer_info[i];
		struct sk_buff *skb = buffer_info->skb;
		union e1000_adv_rx_desc *next_rxd;
		u16 vlan_tag;

		rx_desc->wb.upper.status_error = 0;

		buffer_info->skb = NULL;
		prefetch(skb->data - NET_IP_ALIGN);

		i++;
		if (i == rx_ring->count)
			i = 0;

		next_rxd = IGB_RX_DESC(rx_ring, i);
		prefetch(next_rxd);

#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
		__skb_put(skb, le16_to_cpu(rx_desc->wb.upper.length));
		dma_unmap_single(rx_ring->dev, buffer_info->dma,
				 rx_ring->rx_buffer_len,
				 DMA_FROM_DEVICE);
		buffer_info->dma = 0;

#else
		if (!skb_is_nonlinear(skb)) {
			__skb_put(skb, igb_get_hlen(rx_desc));
			dma_unmap_single(rx_ring->dev, buffer_info->dma,
			                 IGB_RX_HDR_LEN,
					 DMA_FROM_DEVICE);
			buffer_info->dma = 0;
		}

		if (rx_desc->wb.upper.length) {
			u16 length = le16_to_cpu(rx_desc->wb.upper.length);

			skb_fill_page_desc(skb, skb_shinfo(skb)->nr_frags,
					   buffer_info->page,
					   buffer_info->page_offset,
					   length);

			skb->len += length;
			skb->data_len += length;
			skb->truesize += length;

			if ((page_count(buffer_info->page) != 1) ||
			    (page_to_nid(buffer_info->page) != current_node))
				buffer_info->page = NULL;
			else
				get_page(buffer_info->page);

			dma_unmap_page(rx_ring->dev, buffer_info->page_dma,
				       PAGE_SIZE / 2, DMA_FROM_DEVICE);
			buffer_info->page_dma = 0;
		}

		if (!(staterr & E1000_RXD_STAT_EOP)) {
			struct igb_buffer *next_buffer;
			next_buffer = &rx_ring->buffer_info[i];
			buffer_info->skb = next_buffer->skb;
			buffer_info->dma = next_buffer->dma;
			next_buffer->skb = skb;
			next_buffer->dma = 0;
			goto next_desc;
		}

#endif /* CONFIG_IGB_DISABLE_PACKET_SPLIT */
		if (staterr & E1000_RXDEXT_ERR_FRAME_ERR_MASK) {
			dev_kfree_skb_any(skb);
			goto next_desc;
		}

#ifdef HAVE_HW_TIME_STAMP
		if (staterr & (E1000_RXDADV_STAT_TSIP | E1000_RXDADV_STAT_TS))
			igb_rx_hwtstamp(q_vector, staterr, skb);
#endif
		total_bytes += skb->len;
		total_packets++;

		igb_rx_checksum(rx_ring, staterr, skb);

		skb->protocol = eth_type_trans(skb, netdev_ring(rx_ring));

		if (staterr & E1000_RXD_STAT_VP) {
			if (test_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &rx_ring->flags) &&
			    (staterr & E1000_RXDEXT_STATERR_LB))
				vlan_tag = be16_to_cpu(rx_desc->wb.upper.vlan);
			else
				vlan_tag = le16_to_cpu(rx_desc->wb.upper.vlan);
		} else
			vlan_tag = 0;
#ifdef IGB_LRO
		if (igb_can_lro(rx_ring, rx_desc, skb))
			buffer_info->skb = igb_lro_queue(q_vector, skb, vlan_tag);
		else
#endif
#ifdef HAVE_PF_RING
                  if(igb_receive_skb(q_vector, skb, vlan_tag) == 2) {
                    /* Force adapter to stop polling as we have no room for packets */
                    /* printk("[PF_RING] No room for packets\n"); */
                    *work_done = budget;
                    /* schedule(); */
                  }
#else
		igb_receive_skb(q_vector, skb, vlan_tag);
#endif


#ifndef NETIF_F_GRO
		netdev_ring(rx_ring)->last_rx = jiffies;

#endif
		(*work_done)++;

next_desc:
		if (*work_done >= budget)
			break;

		cleaned_count++;
		/* return some buffers to hardware, one at a time is too slow */
		if (cleaned_count >= IGB_RX_BUFFER_WRITE) {
			igb_alloc_rx_buffers(rx_ring, cleaned_count);
			cleaned_count = 0;
		}

		/* use prefetched values */
		rx_desc = next_rxd;
		staterr = le32_to_cpu(rx_desc->wb.upper.status_error);
	}

#ifdef IGB_LRO
	if (test_bit(IGB_RING_FLAG_RX_LRO, &rx_ring->flags))
		igb_lro_flush_all(q_vector);
#endif /* IGB_LRO */
	rx_ring->next_to_clean = i;
	cleaned_count = igb_desc_unused(rx_ring);

	if (cleaned_count)
		igb_alloc_rx_buffers(rx_ring, cleaned_count);

	rx_ring->total_packets += total_packets;
	rx_ring->total_bytes += total_bytes;
	rx_ring->rx_stats.packets += total_packets;
	rx_ring->rx_stats.bytes += total_bytes;
}

/**
 * igb_alloc_rx_buffers - Replace used receive buffers; packet split
 * @adapter: address of board private structure
 **/
void igb_alloc_rx_buffers(struct igb_ring *rx_ring, u16 cleaned_count)
{
#ifdef CONFIG_IGB_DISABLE_PACKET_SPLIT
	const int bufsz = rx_ring->rx_buffer_len;
#else
	const int bufsz = IGB_RX_HDR_LEN;
#endif
	u16 i = rx_ring->next_to_use;

	while (cleaned_count--) {
		union e1000_adv_rx_desc *rx_desc = IGB_RX_DESC(rx_ring, i);
		struct igb_buffer *buffer_info = &rx_ring->buffer_info[i];
		struct sk_buff *skb = buffer_info->skb;

		if (likely(!skb)) {
			skb = netdev_alloc_skb_ip_align(netdev_ring(rx_ring),
							bufsz);
			buffer_info->skb = skb;
			if (!skb) {
				rx_ring->rx_stats.alloc_failed++;
				goto no_buffers;
			}
			/* initialize queue mapping */
			skb_record_rx_queue(skb, ring_queue_index(rx_ring));
		}

		if (!buffer_info->dma) {
			/* reset ip_summed value */
			skb->ip_summed = CHECKSUM_NONE;

			buffer_info->dma = dma_map_single(rx_ring->dev,
			                                  skb->data,
			                                  bufsz,
			                                  DMA_FROM_DEVICE);
			if (dma_mapping_error(rx_ring->dev, buffer_info->dma)) {
				buffer_info->dma = 0;
				rx_ring->rx_stats.alloc_failed++;
				goto no_buffers;
			}
		}

#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
		if (!buffer_info->page) {
			buffer_info->page =
				netdev_alloc_page(netdev_ring(rx_ring));
			if (!buffer_info->page) {
				rx_ring->rx_stats.alloc_failed++;
				goto no_buffers;
			}
		}

		if (!buffer_info->page_dma) {
			buffer_info->page_offset ^= PAGE_SIZE / 2;
			buffer_info->page_dma =
				dma_map_page(rx_ring->dev, buffer_info->page,
				             buffer_info->page_offset,
				             PAGE_SIZE / 2,
				             DMA_FROM_DEVICE);
			if (dma_mapping_error(rx_ring->dev, buffer_info->page_dma)) {
				buffer_info->page_dma = 0;
				rx_ring->rx_stats.alloc_failed++;
				goto no_buffers;
			}
		}

#endif /* CONFIG_IGB_DISABLE_PACKET_SPLIT */
		/* Refresh the desc even if buffer_addrs didn't change because
		 * each write-back erases this info. */
#ifndef CONFIG_IGB_DISABLE_PACKET_SPLIT
		rx_desc->read.pkt_addr = cpu_to_le64(buffer_info->page_dma);
		rx_desc->read.hdr_addr = cpu_to_le64(buffer_info->dma);
#else
		rx_desc->read.pkt_addr = cpu_to_le64(buffer_info->dma);
		rx_desc->read.hdr_addr = 0;
#endif /* CONFIG_IGB_DISABLE_PACKET_SPLIT */

		i++;
		if (i == rx_ring->count)
			i = 0;
	}

no_buffers:
	if (rx_ring->next_to_use != i) {
		rx_ring->next_to_use = i;

		/* Force memory writes to complete before letting h/w
		 * know there are new descriptors to fetch.  (Only
		 * applicable for weak-ordered memory model archs,
		 * such as IA-64). */
		wmb();
		writel(i, rx_ring->tail);
	}
}

#ifdef SIOCGMIIPHY
/**
 * igb_mii_ioctl -
 * @netdev:
 * @ifreq:
 * @cmd:
 **/
static int igb_mii_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct mii_ioctl_data *data = if_mii(ifr);

	if (adapter->hw.phy.media_type != e1000_media_type_copper)
		return -EOPNOTSUPP;

	switch (cmd) {
	case SIOCGMIIPHY:
		data->phy_id = adapter->hw.phy.addr;
		break;
	case SIOCGMIIREG:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		if (e1000_read_phy_reg(&adapter->hw, data->reg_num & 0x1F,
				   &data->val_out))
			return -EIO;
		break;
	case SIOCSMIIREG:
	default:
		return -EOPNOTSUPP;
	}
	return E1000_SUCCESS;
}

#endif
#ifdef HAVE_HW_TIME_STAMP
/**
 * igb_hwtstamp_ioctl - control hardware time stamping
 * @netdev:
 * @ifreq:
 * @cmd:
 *
 * Outgoing time stamping can be enabled and disabled. Play nice and
 * disable it when requested, although it shouldn't case any overhead
 * when no packet needs it. At most one packet in the queue may be
 * marked for time stamping, otherwise it would be impossible to tell
 * for sure to which packet the hardware time stamp belongs.
 *
 * Incoming time stamping has to be configured via the hardware
 * filters. Not all combinations are supported, in particular event
 * type has to be specified. Matching the kind of event packet is
 * not supported, with the exception of "all V2 events regardless of
 * level 2 or 4".
 *
 **/
static int igb_hwtstamp_ioctl(struct net_device *netdev,
			      struct ifreq *ifr, int cmd)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct hwtstamp_config config;
	u32 tsync_tx_ctl = E1000_TSYNCTXCTL_ENABLED;
	u32 tsync_rx_ctl = E1000_TSYNCRXCTL_ENABLED;
	u32 tsync_rx_cfg = 0;
	bool is_l4 = false;
	bool is_l2 = false;
	u32 regval;

	if (copy_from_user(&config, ifr->ifr_data, sizeof(config)))
		return -EFAULT;

	/* reserved for future extensions */
	if (config.flags)
		return -EINVAL;

	switch (config.tx_type) {
	case HWTSTAMP_TX_OFF:
		tsync_tx_ctl = 0;
	case HWTSTAMP_TX_ON:
		break;
	default:
		return -ERANGE;
	}

	switch (config.rx_filter) {
	case HWTSTAMP_FILTER_NONE:
		tsync_rx_ctl = 0;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
	case HWTSTAMP_FILTER_ALL:
		/*
		 * register TSYNCRXCFG must be set, therefore it is not
		 * possible to time stamp both Sync and Delay_Req messages
		 * => fall back to time stamping all packets
		 */
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_ALL;
		config.rx_filter = HWTSTAMP_FILTER_ALL;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_SYNC:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L4_V1;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V1_SYNC_MESSAGE;
		is_l4 = true;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L4_V1;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V1_DELAY_REQ_MESSAGE;
		is_l4 = true;
		break;
	case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L2_L4_V2;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V2_SYNC_MESSAGE;
		is_l2 = true;
		is_l4 = true;
		config.rx_filter = HWTSTAMP_FILTER_SOME;
		break;
	case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L2_L4_V2;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V2_DELAY_REQ_MESSAGE;
		is_l2 = true;
		is_l4 = true;
		config.rx_filter = HWTSTAMP_FILTER_SOME;
		break;
	case HWTSTAMP_FILTER_PTP_V2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_EVENT_V2;
		config.rx_filter = HWTSTAMP_FILTER_PTP_V2_EVENT;
		is_l2 = true;
		break;
	default:
		return -ERANGE;
	}

	if (hw->mac.type == e1000_82575) {
		if (tsync_rx_ctl | tsync_tx_ctl)
			return -EINVAL;
		return 0;
	}

#ifdef IGB_PER_PKT_TIMESTAMP
	/*
	 * Per-packet timestamping only works if all packets are
	 * timestamped, so enable timestamping in all packets as
	 * long as one rx filter was configured.
	 */
	if ((hw->mac.type == e1000_82580) && tsync_rx_ctl) {
		tsync_rx_ctl = E1000_TSYNCRXCTL_ENABLED;
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_ALL;
	}
#endif

	/* enable/disable TX */
	regval = E1000_READ_REG(hw, E1000_TSYNCTXCTL);
	regval &= ~E1000_TSYNCTXCTL_ENABLED;
	regval |= tsync_tx_ctl;
	E1000_WRITE_REG(hw, E1000_TSYNCTXCTL, regval);

	/* enable/disable RX */
	regval = E1000_READ_REG(hw, E1000_TSYNCRXCTL);
	regval &= ~(E1000_TSYNCRXCTL_ENABLED | E1000_TSYNCRXCTL_TYPE_MASK);
	regval |= tsync_rx_ctl;
	E1000_WRITE_REG(hw, E1000_TSYNCRXCTL, regval);

	/* define which PTP packets are time stamped */
	E1000_WRITE_REG(hw, E1000_TSYNCRXCFG, tsync_rx_cfg);

	/* define ethertype filter for timestamped packets */
	if (is_l2)
		E1000_WRITE_REG(hw, E1000_ETQF(3),
		                (E1000_ETQF_FILTER_ENABLE | /* enable filter */
		                 E1000_ETQF_1588 | /* enable timestamping */
		                 ETH_P_1588));     /* 1588 eth protocol type */
	else
		E1000_WRITE_REG(hw, E1000_ETQF(3), 0);

#define PTP_PORT 319
	/* L4 Queue Filter[3]: filter by destination port and protocol */
	if (is_l4) {
		u32 ftqf = (IPPROTO_UDP /* UDP */
			| E1000_FTQF_VF_BP /* VF not compared */
			| E1000_FTQF_1588_TIME_STAMP /* Enable Timestamping */
			| E1000_FTQF_MASK); /* mask all inputs */
		ftqf &= ~E1000_FTQF_MASK_PROTO_BP; /* enable protocol check */

		E1000_WRITE_REG(hw, E1000_IMIR(3), htons(PTP_PORT));
		E1000_WRITE_REG(hw, E1000_IMIREXT(3),
				(E1000_IMIREXT_SIZE_BP | E1000_IMIREXT_CTRL_BP));
		if (hw->mac.type == e1000_82576) {
			/* enable source port check */
			E1000_WRITE_REG(hw, E1000_SPQF(3), htons(PTP_PORT));
			ftqf &= ~E1000_FTQF_MASK_SOURCE_PORT_BP;
		}
		E1000_WRITE_REG(hw, E1000_FTQF(3), ftqf);
	} else {
		E1000_WRITE_REG(hw, E1000_FTQF(3), E1000_FTQF_MASK);
	}
	E1000_WRITE_FLUSH(hw);

	adapter->hwtstamp_config = config;

	/* clear TX/RX time stamp registers, just to be sure */
	regval = E1000_READ_REG(hw, E1000_TXSTMPH);
	regval = E1000_READ_REG(hw, E1000_RXSTMPH);

	return copy_to_user(ifr->ifr_data, &config, sizeof(config)) ?
		-EFAULT : 0;
}

#endif
/**
 * igb_ioctl -
 * @netdev:
 * @ifreq:
 * @cmd:
 **/
static int igb_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	switch (cmd) {
#ifdef SIOCGMIIPHY
	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSMIIREG:
		return igb_mii_ioctl(netdev, ifr, cmd);
#endif
#ifdef HAVE_HW_TIME_STAMP
	case SIOCSHWTSTAMP:
		return igb_hwtstamp_ioctl(netdev, ifr, cmd);
#endif
#ifdef ETHTOOL_OPS_COMPAT
	case SIOCETHTOOL:
		return ethtool_ioctl(ifr);
#endif
	default:
		return -EOPNOTSUPP;
	}
}

s32 e1000_read_pcie_cap_reg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;
	u16 cap_offset;

	cap_offset = pci_find_capability(adapter->pdev, PCI_CAP_ID_EXP);
	if (!cap_offset)
		return -E1000_ERR_CONFIG;

	pci_read_config_word(adapter->pdev, cap_offset + reg, value);

	return E1000_SUCCESS;
}

s32 e1000_write_pcie_cap_reg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;
	u16 cap_offset;

	cap_offset = pci_find_capability(adapter->pdev, PCI_CAP_ID_EXP);
	if (!cap_offset)
		return -E1000_ERR_CONFIG;

	pci_write_config_word(adapter->pdev, cap_offset + reg, *value);

	return E1000_SUCCESS;
}

void igb_enable_vlan_tags(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl, rctl;
	int enable = false;
#ifdef CONFIG_IGB_VMDQ_NETDEV
	struct igb_vmdq_adapter *vadapter;
	int i;

	for (i = 1; i < adapter->vmdq_pools; i++) {
		vadapter = netdev_priv(adapter->vmdq_netdev[i-1]);
		if (vadapter->vlgrp) {
			enable = true;
			igb_set_vf_vlan_strip(adapter, 
					      adapter->vfs_allocated_count + i,
					      true);
		} else
			igb_set_vf_vlan_strip(adapter, 
					      adapter->vfs_allocated_count + i,
					      false);
	}

#endif
	if (adapter->vlgrp)
		enable = true;

	if (enable) {
		/* enable VLAN tag insert/strip */
		ctrl = E1000_READ_REG(hw, E1000_CTRL);
		ctrl |= E1000_CTRL_VME;
		E1000_WRITE_REG(hw, E1000_CTRL, ctrl);

		/* Disable CFI check */
		rctl = E1000_READ_REG(hw, E1000_RCTL);
		rctl &= ~E1000_RCTL_CFIEN;
		E1000_WRITE_REG(hw, E1000_RCTL, rctl);
	} else {
		/* disable VLAN tag insert/strip */
		ctrl = E1000_READ_REG(hw, E1000_CTRL);
		ctrl &= ~E1000_CTRL_VME;
		E1000_WRITE_REG(hw, E1000_CTRL, ctrl);
	}
	igb_rlpml_set(adapter);
}
static void igb_vlan_rx_register(struct net_device *netdev,
				 struct vlan_group *grp)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	igb_irq_disable(adapter);
	adapter->vlgrp = grp;

	igb_enable_vlan_tags(adapter);

	if (!test_bit(__IGB_DOWN, &adapter->state))
		igb_irq_enable(adapter);
}

static void igb_vlan_rx_add_vid(struct net_device *netdev, u16 vid)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int pf_id = adapter->vfs_allocated_count;
#ifndef HAVE_NETDEV_VLAN_FEATURES
	struct net_device *v_netdev;
#endif

	/* attempt to add filter to vlvf array */
	igb_vlvf_set(adapter, vid, TRUE, pf_id);

	/* add the filter since PF can receive vlans w/o entry in vlvf */
	igb_vfta_set(hw, vid, TRUE);
#ifndef HAVE_NETDEV_VLAN_FEATURES

	/* Copy feature flags from netdev to the vlan netdev for this vid.
	 * This allows things like TSO to bubble down to our vlan device.
	 */
	v_netdev = vlan_group_get_device(adapter->vlgrp, vid);
	v_netdev->features |= adapter->netdev->features;
	vlan_group_set_device(adapter->vlgrp, vid, v_netdev);
#endif
}

static void igb_vlan_rx_kill_vid(struct net_device *netdev, u16 vid)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int pf_id = adapter->vfs_allocated_count;
	s32 err;

	igb_irq_disable(adapter);
	vlan_group_set_device(adapter->vlgrp, vid, NULL);

	if (!test_bit(__IGB_DOWN, &adapter->state))
		igb_irq_enable(adapter);

	/* remove vlan from VLVF table array */
	err = igb_vlvf_set(adapter, vid, FALSE, pf_id);

	/* if vid was not present in VLVF just remove it from table */
	if (err)
		igb_vfta_set(hw, vid, FALSE);
}

static void igb_restore_vlan(struct igb_adapter *adapter)
{
	igb_vlan_rx_register(adapter->netdev, adapter->vlgrp);

	if (adapter->vlgrp) {
		u16 vid;
		for (vid = 0; vid < VLAN_N_VID; vid++) {
			if (!vlan_group_get_device(adapter->vlgrp, vid))
				continue;
			igb_vlan_rx_add_vid(adapter->netdev, vid);
		}
	}
}

int igb_set_spd_dplx(struct igb_adapter *adapter, u16 spddplx)
{
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_mac_info *mac = &adapter->hw.mac;

	mac->autoneg = 0;

	/* Fiber NIC's only allow 1000 gbps Full duplex */
	if ((adapter->hw.phy.media_type == e1000_media_type_internal_serdes ) &&
		spddplx != (SPEED_1000 + DUPLEX_FULL)) {
		dev_err(pci_dev_to_dev(pdev),
		        "Unsupported Speed/Duplex configuration\n");
		return -EINVAL;
	}

	switch (spddplx) {
	case SPEED_10 + DUPLEX_HALF:
		mac->forced_speed_duplex = ADVERTISE_10_HALF;
		break;
	case SPEED_10 + DUPLEX_FULL:
		mac->forced_speed_duplex = ADVERTISE_10_FULL;
		break;
	case SPEED_100 + DUPLEX_HALF:
		mac->forced_speed_duplex = ADVERTISE_100_HALF;
		break;
	case SPEED_100 + DUPLEX_FULL:
		mac->forced_speed_duplex = ADVERTISE_100_FULL;
		break;
	case SPEED_1000 + DUPLEX_FULL:
		mac->autoneg = 1;
		adapter->hw.phy.autoneg_advertised = ADVERTISE_1000_FULL;
		break;
	case SPEED_1000 + DUPLEX_HALF: /* not supported */
	default:
		dev_err(pci_dev_to_dev(pdev), "Unsupported Speed/Duplex configuration\n");
		return -EINVAL;
	}
	return 0;
}

static int __igb_shutdown(struct pci_dev *pdev, bool *enable_wake)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl, rctl, status;
	u32 wufc = adapter->wol;
#ifdef CONFIG_PM
	int retval = 0;
#endif

	netif_device_detach(netdev);

	if (netif_running(netdev))
		igb_close(netdev);

	igb_clear_interrupt_scheme(adapter);

#ifdef CONFIG_PM
	retval = pci_save_state(pdev);
	if (retval)
		return retval;
#endif

	status = E1000_READ_REG(hw, E1000_STATUS);
	if (status & E1000_STATUS_LU)
		wufc &= ~E1000_WUFC_LNKC;

	if (wufc) {
		igb_setup_rctl(adapter);
		igb_set_rx_mode(netdev);

		/* turn on all-multi mode if wake on multicast is enabled */
		if (wufc & E1000_WUFC_MC) {
			rctl = E1000_READ_REG(hw, E1000_RCTL);
			rctl |= E1000_RCTL_MPE;
			E1000_WRITE_REG(hw, E1000_RCTL, rctl);
		}

		ctrl = E1000_READ_REG(hw, E1000_CTRL);
		/* phy power management enable */
		#define E1000_CTRL_EN_PHY_PWR_MGMT 0x00200000
		ctrl |= E1000_CTRL_ADVD3WUC;
		E1000_WRITE_REG(hw, E1000_CTRL, ctrl);

		/* Allow time for pending master requests to run */
		e1000_disable_pcie_master(hw);

		E1000_WRITE_REG(hw, E1000_WUC, E1000_WUC_PME_EN);
		E1000_WRITE_REG(hw, E1000_WUFC, wufc);
	} else {
		E1000_WRITE_REG(hw, E1000_WUC, 0);
		E1000_WRITE_REG(hw, E1000_WUFC, 0);
	}

	*enable_wake = wufc || adapter->en_mng_pt;
	if (!*enable_wake)
		igb_power_down_link(adapter);
	else
		igb_power_up_link(adapter);

	/* Release control of h/w to f/w.  If f/w is AMT enabled, this
	 * would have already happened in close and is redundant. */
	igb_release_hw_control(adapter);

	pci_disable_device(pdev);

	return 0;
}

#ifdef CONFIG_PM
static int igb_suspend(struct pci_dev *pdev, pm_message_t state)
{
#define E1000_PMCSR       0x0044
#define E1000_PMCSR_PS_D3 0x00000003
        struct net_device *netdev = NULL;
        struct igb_adapter *adapter = NULL;
        struct e1000_hw *hw = NULL;
        u32 pmcsr = 0;
	int retval;
	bool wake;

	retval = __igb_shutdown(pdev, &wake);
	if (retval)
		return retval;

	if (wake) {
		pci_prepare_to_sleep(pdev);
	} else {
		pci_wake_from_d3(pdev, false);
		pci_set_power_state(pdev, PCI_D3hot);
	}

        /* This is WoL workaround for DH89xxCC */
        switch (pdev->device) {
        case E1000_DEV_ID_DH89XXCC_SFP:
        case E1000_DEV_ID_DH89XXCC_BACKPLANE:
        case E1000_DEV_ID_DH89XXCC_SERDES:
        case E1000_DEV_ID_DH89XXCC_SGMII:
                netdev = pci_get_drvdata(pdev);
                adapter = netdev_priv(netdev);
                hw = &adapter->hw;
                pmcsr = E1000_READ_REG( hw, E1000_PMCSR );
                pmcsr |= E1000_PMCSR_PS_D3;
                E1000_WRITE_REG( hw, E1000_PMCSR, pmcsr );
                E1000_WRITE_REG( hw, E1000_PMCSR, pmcsr );
                break;
        default:
                break;
	}

	return 0;
}

static int igb_resume(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	u32 err;

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	pci_save_state(pdev);

	err = pci_enable_device_mem(pdev);
	if (err) {
		dev_err(pci_dev_to_dev(pdev),
			"igb: Cannot enable PCI device from suspend\n");
		return err;
	}
	pci_set_master(pdev);

	pci_enable_wake(pdev, PCI_D3hot, 0);
	pci_enable_wake(pdev, PCI_D3cold, 0);

	if (igb_init_interrupt_scheme(adapter)) {
		dev_err(pci_dev_to_dev(pdev), "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

	igb_reset(adapter);

	/* let the f/w know that the h/w is now under the control of the
	 * driver. */
	igb_get_hw_control(adapter);

	E1000_WRITE_REG(hw, E1000_WUS, ~0);

	if (netif_running(netdev)) {
		err = igb_open(netdev);
		if (err)
			return err;
	}

	netif_device_attach(netdev);

	return 0;
}
#endif

#ifdef USE_REBOOT_NOTIFIER
/* only want to do this for 2.4 kernels? */
static int igb_notify_reboot(struct notifier_block *nb, unsigned long event,
                             void *p)
{
	struct pci_dev *pdev = NULL;
	bool wake;

	switch (event) {
	case SYS_DOWN:
	case SYS_HALT:
	case SYS_POWER_OFF:
		while ((pdev = pci_find_device(PCI_ANY_ID, PCI_ANY_ID, pdev))) {
			if (pci_dev_driver(pdev) == &igb_driver) {
				__igb_shutdown(pdev, &wake);
				if (event == SYS_POWER_OFF) {
					pci_wake_from_d3(pdev, wake);
					pci_set_power_state(pdev, PCI_D3hot);
				}
			}
		}
	}
	return NOTIFY_DONE;
}
#else
static void igb_shutdown(struct pci_dev *pdev)
{
	bool wake;

	__igb_shutdown(pdev, &wake);

	if (system_state == SYSTEM_POWER_OFF) {
		pci_wake_from_d3(pdev, wake);
		pci_set_power_state(pdev, PCI_D3hot);
	}
}
#endif /* USE_REBOOT_NOTIFIER */

#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void igb_netpoll(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct igb_q_vector *q_vector;
	int i;

	for (i = 0; i < adapter->num_q_vectors; i++) {
		q_vector = adapter->q_vector[i];
		if (adapter->msix_entries)
			E1000_WRITE_REG(hw, E1000_EIMC, q_vector->eims_value);
		else
			igb_irq_disable(adapter);
		napi_schedule(&q_vector->napi);
	}
}
#endif /* CONFIG_NET_POLL_CONTROLLER */

#ifdef HAVE_PCI_ERS
/**
 * igb_io_error_detected - called when PCI error is detected
 * @pdev: Pointer to PCI device
 * @state: The current pci connection state
 *
 * This function is called after a PCI bus error affecting
 * this device has been detected.
 */
static pci_ers_result_t igb_io_error_detected(struct pci_dev *pdev,
					      pci_channel_state_t state)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);

	netif_device_detach(netdev);

	if (state == pci_channel_io_perm_failure)
		return PCI_ERS_RESULT_DISCONNECT;

	if (netif_running(netdev))
		igb_down(adapter);
	pci_disable_device(pdev);

	/* Request a slot slot reset. */
	return PCI_ERS_RESULT_NEED_RESET;
}

/**
 * igb_io_slot_reset - called after the pci bus has been reset.
 * @pdev: Pointer to PCI device
 *
 * Restart the card from scratch, as if from a cold-boot. Implementation
 * resembles the first-half of the igb_resume routine.
 */
static pci_ers_result_t igb_io_slot_reset(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	pci_ers_result_t result;

	if (pci_enable_device_mem(pdev)) {
		dev_err(pci_dev_to_dev(pdev),
			"Cannot re-enable PCI device after reset.\n");
		result = PCI_ERS_RESULT_DISCONNECT;
	} else {
		pci_set_master(pdev);
		pci_restore_state(pdev);
		pci_save_state(pdev);

		pci_enable_wake(pdev, PCI_D3hot, 0);
		pci_enable_wake(pdev, PCI_D3cold, 0);

		igb_reset(adapter);
		E1000_WRITE_REG(hw, E1000_WUS, ~0);
		result = PCI_ERS_RESULT_RECOVERED;
	}

	pci_cleanup_aer_uncorrect_error_status(pdev);

	return result;
}

/**
 * igb_io_resume - called when traffic can start flowing again.
 * @pdev: Pointer to PCI device
 *
 * This callback is called when the error recovery driver tells us that
 * its OK to resume normal operation. Implementation resembles the
 * second-half of the igb_resume routine.
 */
static void igb_io_resume(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);

	if (netif_running(netdev)) {
		if (igb_up(adapter)) {
			dev_err(pci_dev_to_dev(pdev), "igb_up failed after reset\n");
			return;
		}
	}

	netif_device_attach(netdev);

	/* let the f/w know that the h/w is now under the control of the
	 * driver. */
	igb_get_hw_control(adapter);
}

#endif /* HAVE_PCI_ERS */

int igb_add_mac_filter(struct igb_adapter *adapter, u8 *addr, u16 queue)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	if (is_zero_ether_addr(addr))
		return 0;

	for (i = 0; i < hw->mac.rar_entry_count; i++) {
		if (adapter->mac_table[i].state & IGB_MAC_STATE_IN_USE)
			continue;
		adapter->mac_table[i].state = (IGB_MAC_STATE_MODIFIED |
						   IGB_MAC_STATE_IN_USE);
		memcpy(adapter->mac_table[i].addr, addr, ETH_ALEN);
		adapter->mac_table[i].queue = queue;
		igb_sync_mac_table(adapter);
		return 0;
	}
	return -ENOMEM;
}
int igb_del_mac_filter(struct igb_adapter *adapter, u8* addr, u16 queue)
{
	/* search table for addr, if found, set to 0 and sync */
	int i;
	struct e1000_hw *hw = &adapter->hw;

	if (is_zero_ether_addr(addr))
		return 0;
	for (i = 0; i < hw->mac.rar_entry_count; i++) {
		if (!compare_ether_addr(addr, adapter->mac_table[i].addr) &&
		    adapter->mac_table[i].queue == queue) {
			adapter->mac_table[i].state = IGB_MAC_STATE_MODIFIED;
			memset(adapter->mac_table[i].addr, 0, ETH_ALEN);
			adapter->mac_table[i].queue = 0;
			igb_sync_mac_table(adapter);
			return 0;
		}
	}
	return -ENOMEM;
}
static int igb_set_vf_mac(struct igb_adapter *adapter,
                          int vf, unsigned char *mac_addr)
{
	igb_del_mac_filter(adapter, adapter->vf_data[vf].vf_mac_addresses, vf);
	memcpy(adapter->vf_data[vf].vf_mac_addresses, mac_addr, ETH_ALEN);

	igb_add_mac_filter(adapter, mac_addr, vf);

	return 0;
}

#ifdef IFLA_VF_MAX
static int igb_ndo_set_vf_mac(struct net_device *netdev, int vf, u8 *mac)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	if (!is_valid_ether_addr(mac) || (vf >= adapter->vfs_allocated_count))
		return -EINVAL;
	adapter->vf_data[vf].flags |= IGB_VF_FLAG_PF_SET_MAC;
	dev_info(&adapter->pdev->dev, "setting MAC %pM on VF %d\n", mac, vf);
	dev_info(&adapter->pdev->dev, "Reload the VF driver to make this"
				      " change effective.\n");
	if (test_bit(__IGB_DOWN, &adapter->state)) {
		dev_warn(&adapter->pdev->dev, "The VF MAC address has been set,"
			 " but the PF device is not up.\n");
		dev_warn(&adapter->pdev->dev, "Bring the PF device up before"
			 " attempting to use the VF device.\n");
	}
	return igb_set_vf_mac(adapter, vf, mac);
}

static int igb_link_mbps(int internal_link_speed)
{
	switch (internal_link_speed) {
	case SPEED_100:
		return 100;
	case SPEED_1000:
		return 1000;
	default:
		return 0;
	}
}

static void igb_set_vf_rate_limit(struct e1000_hw *hw, int vf, int tx_rate,
			int link_speed)
{
	int rf_dec, rf_int;
	u32 bcnrc_val;

	if (tx_rate != 0) {
		/* Calculate the rate factor values to set */
		rf_int = link_speed / tx_rate;
		rf_dec = (link_speed - (rf_int * tx_rate));
		rf_dec = (rf_dec * (1<<E1000_RTTBCNRC_RF_INT_SHIFT)) / tx_rate;

		bcnrc_val = E1000_RTTBCNRC_RS_ENA;
		bcnrc_val |= ((rf_int<<E1000_RTTBCNRC_RF_INT_SHIFT) &
				E1000_RTTBCNRC_RF_INT_MASK);
		bcnrc_val |= (rf_dec & E1000_RTTBCNRC_RF_DEC_MASK);
	} else {
		bcnrc_val = 0;
	}

	E1000_WRITE_REG(hw, E1000_RTTDQSEL, vf); /* vf X uses queue X */
	E1000_WRITE_REG(hw, E1000_RTTBCNRC, bcnrc_val);
}

static void igb_check_vf_rate_limit(struct igb_adapter *adapter)
{
	int actual_link_speed, i;
	bool reset_rate = false;

	/* VF TX rate limit was not set */
	if ((adapter->vf_rate_link_speed == 0) || 
		(adapter->hw.mac.type != e1000_82576))
		return;

	actual_link_speed = igb_link_mbps(adapter->link_speed);
	if (actual_link_speed != adapter->vf_rate_link_speed) {
		reset_rate = true;
		adapter->vf_rate_link_speed = 0;
		dev_info(&adapter->pdev->dev,
		"Link speed has been changed. VF Transmit rate is disabled\n");
	}

	for (i = 0; i < adapter->vfs_allocated_count; i++) {
		if (reset_rate)
			adapter->vf_data[i].tx_rate = 0;

		igb_set_vf_rate_limit(&adapter->hw, i,
			adapter->vf_data[i].tx_rate, actual_link_speed);
	}
}

static int igb_ndo_set_vf_bw(struct net_device *netdev, int vf, int tx_rate)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int actual_link_speed;
	
	if (hw->mac.type != e1000_82576)
		return -EOPNOTSUPP;

	actual_link_speed = igb_link_mbps(adapter->link_speed);
	if ((vf >= adapter->vfs_allocated_count) ||
		(!(E1000_READ_REG(hw, E1000_STATUS) & E1000_STATUS_LU)) ||
		(tx_rate < 0) || (tx_rate > actual_link_speed))
		return -EINVAL;

	adapter->vf_rate_link_speed = actual_link_speed;
	adapter->vf_data[vf].tx_rate = (u16)tx_rate;
	igb_set_vf_rate_limit(hw, vf, tx_rate, actual_link_speed);

	return 0;
}

static int igb_ndo_get_vf_config(struct net_device *netdev,
				 int vf, struct ifla_vf_info *ivi)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	if (vf >= adapter->vfs_allocated_count)
		return -EINVAL;
	ivi->vf = vf;
	memcpy(&ivi->mac, adapter->vf_data[vf].vf_mac_addresses, ETH_ALEN);
	ivi->tx_rate = adapter->vf_data[vf].tx_rate;
	ivi->vlan = adapter->vf_data[vf].pf_vlan;
	ivi->qos = adapter->vf_data[vf].pf_qos;
	return 0;
}
#endif
static void igb_vmm_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 reg;

	switch (hw->mac.type) {
	case e1000_82575:
	default:
		/* replication is not supported for 82575 */
		return;
	case e1000_82576:
		/* notify HW that the MAC is adding vlan tags */
		reg = E1000_READ_REG(hw, E1000_DTXCTL);
		reg |= (E1000_DTXCTL_VLAN_ADDED |
			E1000_DTXCTL_SPOOF_INT);
		E1000_WRITE_REG(hw, E1000_DTXCTL, reg);
	case e1000_82580:
		/* enable replication vlan tag stripping */
		reg = E1000_READ_REG(hw, E1000_RPLOLR);
		reg |= E1000_RPLOLR_STRVLAN;
		E1000_WRITE_REG(hw, E1000_RPLOLR, reg);
	case e1000_i350:
		/* none of the above registers are supported by i350 */
		break;
	}

	/* enable replication and loopback support */
	e1000_vmdq_set_loopback_pf(hw, adapter->vfs_allocated_count ||
				   adapter->vmdq_pools);

	e1000_vmdq_set_anti_spoofing_pf(hw, adapter->vfs_allocated_count ||
					adapter->vmdq_pools,
					adapter->vfs_allocated_count);
	e1000_vmdq_set_replication_pf(hw, adapter->vfs_allocated_count ||
				      adapter->vmdq_pools);
}

static void igb_init_fw(struct igb_adapter *adapter) 
{
	struct e1000_fw_drv_info fw_cmd;
	struct e1000_hw *hw = &adapter->hw;
	int maj = MAJ;
	int min = MIN;
	int build  = BUILD;
	int i;
	
	if (!e1000_get_hw_semaphore_generic(hw)) { 
		for (i = 0; i <= FW_MAX_RETRIES; i++) {
			E1000_WRITE_REG(hw, E1000_FWSTS, E1000_FWSTS_FWRI);
			fw_cmd.hdr.cmd = FW_CMD_DRV_INFO;
			fw_cmd.hdr.buf_len = FW_CMD_DRV_INFO_LEN;
			fw_cmd.hdr.cmd_or_resp.cmd_resv = FW_CMD_RESERVED;
			fw_cmd.port_num = hw->bus.func;
			fw_cmd.drv_version = (maj << 24) + (min << 16)
			                      + (build << 8) + FW_UNUSED_VER;
			fw_cmd.hdr.checksum = e1000_calculate_checksum((u8 *)&fw_cmd,
			                                           (FW_HDR_LEN +
			                                            fw_cmd.hdr.buf_len));
			 e1000_host_interface_command(hw, (u8*)&fw_cmd,
			                             sizeof(fw_cmd)); 
			if (fw_cmd.hdr.cmd_or_resp.ret_status == FW_STATUS_SUCCESS)
				break;
		}
	} else
		dev_warn(pci_dev_to_dev(adapter->pdev),
			 "Unable to get semaphore, firmware init failed.\n");
	e1000_put_hw_semaphore_generic(hw);
}
/* igb_main.c */
