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
#include <linux/ioport.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <asm/io.h>

#define DOMAIN PF_RING
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)


//extern and global declarations
struct task_struct *ts,*ts2;
u_int16_t slot_header_len;
//extern struct pf_ring_socket *hs_pfr;

char *inet_ntoa(struct in_addr *in,char *str_ip)
{

	u_int32_t int_ip = 0;
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

int looper(const struct pfring_pkthdr *hdr, const u_char *p) {
	struct sockaddr_in source,dest;
	char *str_ip = NULL;
	str_ip = kmalloc(16 * sizeof(char), GFP_KERNEL);
	if (!str_ip)
		return 0;
	else
		memset(str_ip, 0, 16);
	if(hdr->ts.tv_sec == 0) {
	//	gettimeofday((struct timeval*)&hdr->ts, NULL);
		printk("\nInside PARSE_PKT");
		parse_pkt((u_char*)p, (struct pfring_pkthdr*)hdr);
	}
	source.sin_addr.s_addr = hdr->extended_hdr.parsed_pkt.ip_src.v4;
	dest.sin_addr.s_addr =hdr->extended_hdr.parsed_pkt.ip_dst.v4;
	printk(KERN_ALERT "   |-Source IP        : %s\n",inet_ntoa(&source.sin_addr,str_ip));
	printk(KERN_ALERT "   |-Destination IP   : %s\n",inet_ntoa(&dest.sin_addr,str_ip));
	if(hdr->extended_hdr.parsed_pkt.l4_dst_port && hdr->extended_hdr.parsed_pkt.l4_src_port)
	{
		printk(KERN_ALERT "   |-Source Port        : %d\n",hdr->extended_hdr.parsed_pkt.l4_src_port);
		printk(KERN_ALERT "   |-Destination Port   : %d\n",hdr->extended_hdr.parsed_pkt.l4_dst_port);
	}
	kfree(str_ip);
	return 1;
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
	kfree(eth);
}



int recv_pack(u_char** buffer, u_int buffer_len,
		    struct pfring_pkthdr *hdr,char *slots,FlowSlotInfo *slots_info) {
  

  if(buffer == NULL)
    return(-1);

    if(slots_info->tot_insert != slots_info->tot_read) {
      char *bucket = &slots[slots_info->remove_off];
      u_int32_t next_off, real_slot_len, insert_off, bktLen;
      /*static int count=0;
      printk("\npkt count is:%u",count);
      count++;*/	
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


int capture_thread(void *device_name)
{
	int ret,caplen=128;
	struct sockaddr sa;
	packet_direction direction=0;
	u_char *actual_buffer = NULL;
	struct pfring_pkthdr hdr;
	char dummy;
	u_int32_t pfr_var;
	struct pf_ring_socket *pfr;
	struct socket *capture_sockfd;
	char *slots;
	FlowSlotInfo *slots_info;
	int len = sizeof(slot_header_len);

	printk("in function capture_thread");	
	ret = sock_create(DOMAIN, TYPE , PROTOCOL , &capture_sockfd);
	//ret = sock_create(DOMAIN, TYPE , PROTOCOL , &fd2);
	//printk("\ncapture_sockfd::%u  fd2::%u",capture_sockfd,fd2); 
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
		//ret = capture_sockfd->ops->setsockopt(fd2, 0, SO_RING_BUCKET_LEN,&caplen, sizeof(caplen));

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
  	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", ((char*)device_name));

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
	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_HS_RING,&pfr_var, sizeof(int));
	pfr=(struct pf_ring_socket *)pfr_var;
	if (ret != 0)
	{
		printk("setsockopt SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("SO_SET_PKAP_RING setsockopt success ");
	}	
	//mmap done
	//printk("\nhs_pfr is:: %u, pfr is ::%u",hs_pfr,pfr);
	//printk("\nhs_pfr->ring_mem:%u",hs_pfr->ring_memory);
	slots_info = (FlowSlotInfo *)(pfr->ring_memory);

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

	slots = (char *)(pfr->ring_memory+sizeof(FlowSlotInfo));
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
	//for(i=0;i<3000;i++){
	    if (kthread_should_stop())
	        break;
	    ret = recv_pack(&actual_buffer, 0, &hdr,slots,slots_info);
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
       	    //msleep(10);
	    //break;	
	  }
	

	sock_release(capture_sockfd);
	//sock_release(fd2);
	return 0;
}


char *device_name;
static int ker_pf_ring_init(void)
{	device_name=kmalloc(10*sizeof(char),GFP_KERNEL);
	strcpy(device_name,"wlan0");
	printk(KERN_ALERT "Initializing PF_RING kernel module");
	//kernel_thread(capture_thread, NULL, 0);
	ts=kthread_run(capture_thread,(void *)device_name,"kthread");
	msleep(10);
	strcpy(device_name,"eth0");
	ts2=kthread_run(capture_thread,(void *)device_name,"kthread");
	return 0;
}



static void ker_pf_ring_exit(void)
{
	kfree((char*)device_name);
	printk(KERN_ALERT "Exiting PF_RING kernel module");
	kthread_stop(ts);
	kthread_stop(ts2);
}



module_init(ker_pf_ring_init);
module_exit(ker_pf_ring_exit);
MODULE_LICENSE("Dual BSD/GPL");
