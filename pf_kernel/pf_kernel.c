#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include<linux/fs.h>
#include<linux/ip.h>
#include<linux/tcp.h>
#include<linux/udp.h>
#include<linux/icmp.h>
#include<linux/pf_ring.h>
#include<linux/unistd.h>
#include<linux/delay.h>
#include<linux/time.h>
#include"pf_kernel.h"

#define DOMAIN PF_RING
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)


//extern and global declarations

FlowSlotInfo *slots_info;
extern struct pf_ring_socket *hs_pfr;
struct socket *capture_sockfd;
char *slots;
struct sockaddr_in source,dest;
u_int16_t slot_header_len;


char *inet_ntoa(struct in_addr *in)
{
	char* str_ip = NULL;
	u_int32_t int_ip = 0;
	
	str_ip = kmalloc(16 * sizeof(char), GFP_KERNEL);
	if (!str_ip)
		return NULL;
	else
		memset(str_ip, 0, 16);

	int_ip = in->s_addr;
	
	sprintf(str_ip, "%d.%d.%d.%d",  (int_ip) & 0xFF,
									(int_ip >> 8 ) & 0xFF,
									(int_ip >> 16) & 0xFF,
									(int_ip >> 24) & 0xFF);
	return str_ip;
}



int parse_pkt(u_char *pkt, struct pfring_pkthdr *hdr)
{
	struct iphdr *ip;
	struct eth_hdr *eh = (struct eth_hdr*)pkt;
	u_int16_t displ;
	int i;
//	eh->h_dest[0];
//return 0;
	memset(&hdr->extended_hdr.parsed_pkt, 0, sizeof(struct pkt_parsing_info));
	for(i=0;i<=5;i++)
	{
		hdr->extended_hdr.parsed_pkt.dmac[i]=eh->h_dest[i];
		hdr->extended_hdr.parsed_pkt.smac[i]=eh->h_source[i];
	}
	
	hdr->extended_hdr.parsed_header_len = 0;
	hdr->extended_hdr.parsed_pkt.eth_type = ntohs(eh->h_proto);
	hdr->extended_hdr.parsed_pkt.offset.eth_offset = 0;

	if(hdr->extended_hdr.parsed_pkt.eth_type == 0x8100 /* 802.1q (VLAN) */)
	{
		hdr->extended_hdr.parsed_pkt.offset.vlan_offset = hdr->extended_hdr.parsed_pkt.offset.eth_offset + sizeof(struct eth_hdr);
		hdr->extended_hdr.parsed_pkt.vlan_id = (pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 14] & 15) * 256
		+ pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 15];
		hdr->extended_hdr.parsed_pkt.eth_type = (pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 16]) * 256
		+ pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 17];
		displ = 4;
	}
	else
	{
		displ = 0;
		hdr->extended_hdr.parsed_pkt.vlan_id = 0; /* Any VLAN */
	}

	if(hdr->extended_hdr.parsed_pkt.eth_type == 0x0800 /* IP */) {
		hdr->extended_hdr.parsed_pkt.offset.l3_offset = hdr->extended_hdr.parsed_pkt.offset.eth_offset+displ+sizeof(struct eth_hdr);
		ip = (struct iphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l3_offset);

		hdr->extended_hdr.parsed_pkt.ip_src.v4 = ntohl(ip->saddr), hdr->extended_hdr.parsed_pkt.ip_dst.v4 = ntohl(ip->daddr), hdr->extended_hdr.parsed_pkt.l3_proto = ip->protocol;
		hdr->extended_hdr.parsed_pkt.ip_tos = ip->tos;

		if((ip->protocol == IPPROTO_TCP) || (ip->protocol == IPPROTO_UDP))
		{
			u_int16_t ip_len = ip->ihl*4;

			hdr->extended_hdr.parsed_pkt.offset.l4_offset = hdr->extended_hdr.parsed_pkt.offset.l3_offset+ip_len;

			if(ip->protocol == IPPROTO_TCP) 
			{
				struct tcphdr *tcp = (struct tcphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l4_offset);
				hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(tcp->source), hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(tcp->dest);
				hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset+(tcp->doff * 4);
				hdr->extended_hdr.parsed_pkt.tcp.seq_num = ntohl(tcp->seq);
				hdr->extended_hdr.parsed_pkt.tcp.ack_num = ntohl(tcp->ack_seq);
				hdr->extended_hdr.parsed_pkt.tcp.flags = (tcp->fin * TH_FIN_MULTIPLIER) + (tcp->syn * TH_SYN_MULTIPLIER) + (tcp->rst * TH_RST_MULTIPLIER) +
				  (tcp->psh * TH_PUSH_MULTIPLIER) + (tcp->ack * TH_ACK_MULTIPLIER) + (tcp->urg * TH_URG_MULTIPLIER);
			} 
			else if(ip->protocol == IPPROTO_UDP) 
			{
				struct udphdr *udp = (struct udphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l4_offset);
				hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(udp->source), hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(udp->dest);
				hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct udphdr);
			} 
			else
				hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset;
		} 
		else
			hdr->extended_hdr.parsed_pkt.l4_src_port = hdr->extended_hdr.parsed_pkt.l4_dst_port = 0;

	return(1); /* IP */
  } /* TODO: handle IPv6 */

  return(0); /* No IP */
}

void looper(const struct pfring_pkthdr *hdr, const u_char *p) {
	if(hdr->ts.tv_sec == 0) {
	//	gettimeofday((struct timeval*)&hdr->ts, NULL);
		printk("\nInside PARSE_PKT");
		parse_pkt((u_char*)p, (struct pfring_pkthdr*)hdr);
	}
	source.sin_addr.s_addr = hdr->extended_hdr.parsed_pkt.ip_src.v4;
	dest.sin_addr.s_addr =hdr->extended_hdr.parsed_pkt.ip_dst.v4;
	printk(KERN_ALERT "   |-Source IP        : %s\n",inet_ntoa(&source.sin_addr));
	printk(KERN_ALERT "   |-Destination IP   : %s\n",inet_ntoa(&dest.sin_addr));
	if(hdr->extended_hdr.parsed_pkt.l4_dst_port && hdr->extended_hdr.parsed_pkt.l4_src_port)
	{
		printk(KERN_ALERT "   |-Source Port        : %d\n",hdr->extended_hdr.parsed_pkt.l4_src_port);
		printk(KERN_ALERT "   |-Destination Port   : %d\n",hdr->extended_hdr.parsed_pkt.l4_dst_port);
	}
}


void print_ethernet_header(struct pfring_pkthdr *hdr)
{
	struct ethhdr *eth = kmalloc(sizeof (struct ethhdr),GFP_KERNEL);
	int i;
	for(i=0;i<=5;i++)
	{
		eth->h_dest[i]=hdr->extended_hdr.parsed_pkt.dmac[i];
		eth->h_source[i]=hdr->extended_hdr.parsed_pkt.smac[i];
	}
	printk(KERN_ALERT "\n");
	printk(KERN_ALERT "Ethernet Header\n");
	printk(KERN_ALERT "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] ,eth->h_dest[4] , eth->h_dest[5] );
	printk(KERN_ALERT "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
	printk(KERN_ALERT "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}



int recv_pack(u_char** buffer, u_int buffer_len,
		    struct pfring_pkthdr *hdr) {
  int rc = 0;
  

  if(buffer == NULL)
    return(-1);

  do_pfring_recv:
    //rmb();

    if(slots_info->tot_insert != slots_info->tot_read) {
      char *bucket = &slots[slots_info->remove_off];
      u_int32_t next_off, real_slot_len, insert_off, bktLen;

      memcpy(hdr, bucket, slot_header_len);

      if(slot_header_len != sizeof(struct pfring_pkthdr))
	bktLen = hdr->caplen;
      else
	bktLen = hdr->caplen+hdr->extended_hdr.parsed_header_len;

      real_slot_len = slot_header_len + bktLen;
      insert_off = slots_info->insert_off;
      if(bktLen > buffer_len) bktLen = buffer_len;

      if(buffer_len == 0)
	*buffer = (u_char*)&bucket[slot_header_len];
      else
	memcpy(*buffer, &bucket[slot_header_len], bktLen);

      next_off = slots_info->remove_off + real_slot_len;
      if((next_off + slots_info->slot_len) > (slots_info->tot_mem - sizeof(FlowSlotInfo))) {
        next_off = 0;
      }

#ifdef USE_MB
      /* This prevents the compiler from reordering instructions.
       * http://en.wikipedia.org/wiki/Memory_ordering#Compiler_memory_barrier */
      gcc_mb();
#endif

      slots_info->tot_read++;
      slots_info->remove_off = next_off;

      /* Ugly safety check */
      if((slots_info->tot_insert == slots_info->tot_read)
	 && (slots_info->remove_off > slots_info->insert_off)) {
	slots_info->remove_off = slots_info->insert_off;
      }
/*
      if(ring->reentrant) pthread_spin_unlock(&ring->spinlock);
      return(1);*/
	return 1;
    }

    /* Nothing to do: we need to wait */
    //if(ring->reentrant) pthread_spin_unlock(&ring->spinlock);

      return(0); /* non-blocking, no packet */
}


int capture_thread(void *arg)
{
	int ret,caplen=128;
	struct sockaddr sa;
	u_int memSlotsLen;
	char device_name[]="eth0";
	int len = sizeof(slot_header_len);
	packet_direction direction=0;
	u_char *actual_buffer = NULL;
	struct pfring_pkthdr hdr;
	char dummy;
	int i;
	
	printk("in function capture_thread");	
	ret = sock_create(DOMAIN, TYPE , PROTOCOL , &capture_sockfd); 
	if (ret <0)
	{
		printk("socket creation failed\n");
		return -1;
	}
	else
	{
		printk("socket created successfully\n");
	}

	//setsockopt    :::::::
	//capture_sockfd->ops->release(capture_sockfd);
	
	//if (SOL_SOCKET==0)
		//ret = sock_setsockopt(capture_sockfd, SOL_SOCKET, SO_RING_BUCKET_LEN, (void *)&caplen, sizeof(caplen));
	//else
		ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_RING_BUCKET_LEN,&caplen, sizeof(caplen));

	//t = capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_RING_BUCKET_LEN, &caplen, sizeof(caplen));	
	if (ret != 0)
	{
		printk("setsockopt failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("setsockopt success\n");
	}	
		
	//bind ::::::::

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_RING;
  	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", "wlan0");

  	ret = capture_sockfd->ops->bind(capture_sockfd, (struct sockaddr *)&sa, sizeof(sa));
  	if(ret != 0) {
		printk(KERN_ALERT "failed to bind....\n");
		sock_release(capture_sockfd);
		return -1;
	}
	else
	{
		printk(KERN_ALERT "bind successful\n");	
	}

	//mmap ::::::::::
	//printk(KERN_ALERT "pkap_pfr=%u",pkap_pfr);
	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_HS_RING, 1, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("SO_SET_PKAP_RING setsockopt success :%u \n",hs_pfr);
	}	
	//mmap done
	printk("\nhs_pfr->ring_mem:%u",hs_pfr->ring_memory);
	slots_info = (FlowSlotInfo *)(hs_pfr->ring_memory);

	if(slots_info->version != RING_FLOWSLOT_VERSION)
	 {
	printk("\nWrong RING version: "
	   "kernel is %i, libpfring was compiled with %i\n",
	   slots_info->version, RING_FLOWSLOT_VERSION);
	sock_release(capture_sockfd);
	return -1;
	}
	else
	{
		printk("\nSlot info success");
	}	

	slots = (char *)(hs_pfr->ring_memory+sizeof(FlowSlotInfo));
	//getsockopt call: last para in declaration int __user *optlen
	
	ret = capture_sockfd->ops->getsockopt(capture_sockfd, 0, SO_GET_PKT_HEADER_LEN, &slot_header_len, &len);
	if(ret!=0)
	{
		printk("\nerror in getting socket header len is:%d",slot_header_len);
		sock_release(capture_sockfd);
		return -1;
	}
	else
	{
		printk("\nSuccess in getting header lenn: %d",slot_header_len);
	}	

	ret=capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_SET_PACKET_DIRECTION, &direction, sizeof(direction));
	if(ret!=0)
	{
		printk("\nError in setting direction");
		sock_release(capture_sockfd);
		return -1;

	}
	else
	{
		printk("\nDirection set successfully");
	}

	dummy = 0;
  	
	//setscokopt ::::activate ring

	ret=capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_ACTIVATE_RING, &dummy, sizeof(dummy));
	
	if(ret!=0)
	{
		printk("\nError in enableing ring\n");		
		sock_release(capture_sockfd);
		return -1;
	}
	else
		printk("\nRing Activated Successfully");
	// ring activation done

	while(1) {
	//for(i=0;i<20;i++){
	    ret = recv_pack(&actual_buffer, 0, &hdr);
	    if(ret < 0)
	      break;
	    else if(ret == 0)
	    {	
	      looper(&hdr, actual_buffer);
	      print_ethernet_header(&hdr); 	
	    }	
	    else {
	      msleep(10);
	    }
	    //break;	
	  }


	sock_release(capture_sockfd);
	return 0;
}



static int ker_pf_ring_init(void)
{
	printk(KERN_ALERT "Initializing PF_RING kernel module");
	kernel_thread(capture_thread, NULL, 0);
	return 0;
}



static void ker_pf_ring_exit(void)
{
	printk(KERN_ALERT "Exiting PF_RING kernel module");
}



module_init(ker_pf_ring_init);
module_exit(ker_pf_ring_exit);
MODULE_LICENSE("Dual BSD/GPL");
