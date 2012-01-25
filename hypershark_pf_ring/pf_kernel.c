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
#include"socket_ops.h"

#define DOMAIN PF_RING
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)
#define HYPERSHARK_NAME "HyperShark"
#define HYPERSHARK_AUTHOR "horizon"
#define HYPERSHARK_DESC "a module to capture all packets"
#define MAX_FLOW_REC 100
#define PORT 4444

int count=0;
u_int16_t slot_header_len;


int add_filtering_rule(struct socket *capture_sockfd,filtering_rule* rule_to_add)
{
	int ret=capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_ADD_FILTERING_RULE, rule_to_add, sizeof(filtering_rule));
	return ret;
}

int set_filtering_rule(struct socket *capture_sockfd,u_int16_t id,u_int16_t proto)
{
	filtering_rule rule;

	memset(&rule, 0, sizeof(rule));

	rule.rule_id = id;
	//rule.rule_action = forward_packet_and_stop_rule_evaluation;
	rule.rule_action = dont_forward_packet_and_stop_rule_evaluation;
	//rule.core_fields.port_low = 80, rule.core_fields.port_high = 80;
	rule.core_fields.proto = proto;//6 /* tcp */;
	if(add_filtering_rule(capture_sockfd, &rule) < 0)
		printk("pfring_add_hash_filtering_rule(2) failed\n");
	else
		printk("Rule added successfully...\n");
}

int atoi( char* pStr ) 
{ 
	int iRetVal = 0; 
	int iTens = 1;

	if ( pStr )
	{
		char* pCur = pStr; 
		while (*pCur) 
			pCur++;

		pCur--;

		while ( pCur >= pStr && *pCur <= '9' && *pCur >= '0' ) 
		{ 
			iRetVal += ((*pCur - '0') * iTens);
			pCur--; 
			iTens *= 10; 
		}
	} 
	return iRetVal; 
} 

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
	printk(KERN_ALERT "   |-Protocol            : %u \n",hdr->extended_hdr.parsed_pkt.eth_type);
	//printk(KERN_ALERT "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
	printk(KERN_ALERT "   |-l3 Protocol            : %u \n",hdr->extended_hdr.parsed_pkt.l3_proto);	
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
	
	kfree(eth);
}



int recv_pack(u_char** buffer, u_int buffer_len,
		    struct pfring_pkthdr *hdr,char *slots,FlowSlotInfo *slots_info) {
  

	if(buffer == NULL)
		return(-1);

	if(slots_info->tot_insert != slots_info->tot_read) 
	{
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



/********************FUNCTION TO OPEN FILE********************/
//Generic function to open a file
/*I/P:Path of Filename,Flags and Permissions
  O/P:The File Descriptor after opening the file*/

struct file* file_open(const char *path,int flags,int rights)
{
  struct file* filp=NULL;
  mm_segment_t oldfs;
  int err=0;
  printk(KERN_INFO "Attempting to open File");

  oldfs=get_fs();
  set_fs(get_ds());
  filp=filp_open(path,flags,rights);
  set_fs(oldfs);

  /*If Error in file open catch the catch and return else return the File Descriptor*/
  if(IS_ERR(filp)){
    err=PTR_ERR(filp);
    printk("Error COde is : %d",err);
    return NULL;
  }
  printk(KERN_INFO "File opened Successfully");
  return filp;
}

/********************FUNCTION TO CLOSE FILE********************/
//Generic function to close a file
/*I/P:File Descriptor of the file opened*/

void file_close(struct file* filp){
  filp_close(filp,NULL);
}

/**********Function to Compare VM id's***************/

int cmp_vmids(vm_id *vm1,vm_id *vm2)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(vm1->v_id[i]!=vm2->v_id[i])
			return 1;
	}
	return 0;
}

/****************Function to allocate Offset Node****************/

offset_node *get_offset_node(loff_t offset)
{
    offset_node *t=kmalloc(sizeof(offset_node),GFP_KERNEL);
    t->offset=offset;
    t->next=NULL;
    return t;
}

/********************Free Offset Nodes********************/

void free_offset_nodes(maprecord* path_cache)
{		
       int j;				
       for(j=0;j<MAX_FLOW_REC;j++)
	{
		offset_node *start=path_cache->off_table[j].start;
		while(start)
		{
			offset_node *tmp=start->next;
			kfree(start);
			start=tmp;
		}
	}
	
}

//******************************Freeing all the Memory allocated********************//
void free_path_cache(maprecord *path_cache)
{	
      	vfree(path_cache->flow_start);
      	vfree(path_cache->off_table);
	kfree(path_cache->bit_map->src_ip);
	kfree(path_cache->bit_map->dst_ip);
	vfree(path_cache->bit_map->src_port);
	vfree(path_cache->bit_map->dst_port);
	kfree(path_cache->bit_map->protocol);
	kfree(path_cache->bit_map);
	kfree(path_cache);
}

/********************************************************************************************************************************/
//BITMAP CREATION COMPRESSION AND WRITING CODE
void set_bit_no(char *arr,u_int32_t row_no)
{
	char tmp=1;
	tmp=tmp<<row_no%8;	
	arr[row_no/8]|=tmp;
}
/********************************************************************************************************************************/
void make_bitmap_entry(flow_record *fr,bitmap *bit_map,u_int32_t flowhash)
{
	set_bit_no(bit_map->src_ip[(*(int*)&fr->ip_src) & 0xFF].octet_4,flowhash);
	set_bit_no(bit_map->src_ip[(*(int*)&fr->ip_src >> 8 ) & 0xFF].octet_3,flowhash);
	set_bit_no(bit_map->src_ip[(*(int*)&fr->ip_src >> 16) & 0xFF].octet_2,flowhash);
	set_bit_no(bit_map->src_ip[(*(int*)&fr->ip_src >> 24) & 0xFF].octet_1,flowhash);

	set_bit_no(bit_map->dst_ip[(*(int*)&fr->ip_dst) & 0xFF].octet_4,flowhash);//check if the order is right
	set_bit_no(bit_map->dst_ip[(*(int*)&fr->ip_dst >> 8 ) & 0xFF].octet_3,flowhash);
	set_bit_no(bit_map->dst_ip[(*(int*)&fr->ip_dst >> 16) & 0xFF].octet_2,flowhash);
	set_bit_no(bit_map->dst_ip[(*(int*)&fr->ip_dst >> 24) & 0xFF].octet_1,flowhash);

	set_bit_no(bit_map->src_port[fr->src_port].octet_1,flowhash);//check if ntohs is done or not
	set_bit_no(bit_map->protocol[fr->protocol].octet_1,flowhash);
}

/********************************************************************************************************************************/
int shift_1_bit_right(unsigned char *bit_map_col,u_int32_t start_bit_no,u_int32_t bit_len)//changes required
{
	u_int32_t i;
	u_int8_t valid_bits_in_last_byte=bit_len%8;//handle condition when this comes zero
	u_int32_t start_index=(bit_len-valid_bits_in_last_byte)/8;	
	u_int32_t last_index=start_bit_no/8;//last_index<start_index	
	
	//u_int32_t no_bytes_to_traverse=((bit_len-valid_bits_in_last_byte)-start_bit_no)/8;//from end
	//no_bytes_to_traverse++;//specific to our case where start_bit_no always indicates 1st bit of any byte i.e. start_bit_no%8=1
	if(valid_bits_in_last_byte==0)
	{
		unsigned char tmp=bit_map_col[start_index-1]&0x01;
		bit_map_col[start_index]=tmp<<7;		
	}
	bit_map_col[start_index]>>=1;//check this for worst case
	start_index--;
	for(i=start_index;i<=last_index;i--)
	{
		bit_map_col[i+1]|=(bit_map_col[i]&0x01)<<7;//shift last bit
		bit_map_col[i]>>=1;
		bit_map_col[i]&=0x7F;
	}
	return 0;//Do not know why
}
/********************************************************************************************************************************/
int shift_4_bytes(unsigned char *bit_map_col,u_int32_t start_index,u_int32_t bit_len)
{
	u_int32_t end_index=bit_len/8,i;
	for(i=start_index;i<end_index-1;i++)
	{
		bit_map_col[i]=bit_map_col[i+1];
	}
	if((bit_len%8)!=0)
		bit_map_col[end_index-1]=bit_map_col[end_index];
	return 0;
}
/********************************************************************************************************************************/
int compress_bit_map(unsigned char * bit_map_col,u_int32_t bit_len)//make everything unsigned char
{
	u_int32_t no_loop=bit_len/31;
	//u_int32_t remain_bits=bit_len%31;
	u_int32_t j=0,was_last_zero=0,i;
	for(i=0;i<no_loop;i++)
	{		
		u_int8_t byte1=bit_map_col[j]&0xFF;
		u_int8_t byte2=bit_map_col[j+1]&0xFF;
		u_int8_t byte3=bit_map_col[j+2]&0xFF;
		u_int8_t byte4=bit_map_col[j+3]&0xF7;
		
		shift_1_bit_right(bit_map_col,j*8,bit_len);
		bit_len++;
		if(byte1 || byte2 || byte3 ||byte4)
		{
			bit_map_col[j]|=0x80;				
		}
		else if(!was_last_zero)
		{
			was_last_zero=1;
			bit_map_col[j+3]|=0x01;
		}
		else
		{
			if(!(++bit_map_col[j-1]))
			{
				++bit_map_col[j-2];//this restricts no of flow records to approx 256*256
			}
			shift_4_bytes(bit_map_col,j,bit_len);
			bit_len-=32;
		}
		j+=4;
	}
	return bit_len;
}


int compress_write_octet(u_int64_t no,port_bits *bitmap,u_int16_t start_offsets,struct file *filp)//start offsets refers to all except first two elements of bitmap_start_offsets
{
	u_int64_t i;
	int ret;
	mm_segment_t oldfs;
	u_int16_t *col_offsets=vmalloc(no*sizeof(u_int16_t));	
	loff_t curr_offset=start_offsets+sizeof(col_offsets);
	
	col_offsets[0]=curr_offset;
	for(i=0;i<no;i++)		
	{
		u_int16_t bit_len;

		bit_len=compress_bit_map(bitmap[i].octet_1,MAX_FLOW_REC);
      
		u_int16_t no_bytes=bit_len/8+((bit_len%8)?1:0);
		if(i!=no-1)
			col_offsets[i+1]=col_offsets[i]+no_bytes;
		oldfs=get_fs();
		set_fs(get_ds());
			ret=vfs_write(filp,bitmap[i].octet_1,no_bytes,&curr_offset);		
		set_fs(oldfs);
		curr_offset+=no_bytes;
	}	
	oldfs=get_fs();
	set_fs(get_ds());
		ret=vfs_write(filp,col_offsets,sizeof(col_offsets),&start_offsets);
	set_fs(oldfs);
	vfree(col_offsets);
	return curr_offset;
}


int compress_write_src_dst_ip(ip_bits *ip_bitmap,u_int16_t *start_offsets,struct file *filp)//start offsets refers to first two elements of bitmap_start_offsets
{
	int no=256,j,i,ret;
	mm_segment_t oldfs;
	loff_t curr_offset=0;
	u_int16_t *col_offsets=vmalloc(no*sizeof(u_int16_t));
	for(j=0;j<4;j++)
	{
		curr_offset=start_offsets[j]+sizeof(col_offsets);
		col_offsets[0]=curr_offset;
		for(i=0;i<no;i++)		
		{
			u_int16_t bit_len;
			switch(j)
			{
				case 0:
					bit_len=compress_bit_map(ip_bitmap[i].octet_1,MAX_FLOW_REC);
					break;
				case 1:
					bit_len=compress_bit_map(ip_bitmap[i].octet_2,MAX_FLOW_REC);
					break;
				case 2:
					bit_len=compress_bit_map(ip_bitmap[i].octet_3,MAX_FLOW_REC);
					break;
				case 3:
					bit_len=compress_bit_map(ip_bitmap[i].octet_4,MAX_FLOW_REC);
			}
			u_int16_t no_bytes=bit_len/8+((bit_len%8)?1:0);
			if(i!=no-1)
				col_offsets[i+1]=col_offsets[i]+no_bytes;
	
			//Switching to kernel_ds
			oldfs=get_fs();
			set_fs(get_ds());
			switch(j)
			{
				case 0:
					ret=vfs_write(filp,ip_bitmap[i].octet_1,no_bytes,&curr_offset);
					break;
				case 1:
					ret=vfs_write(filp,ip_bitmap[i].octet_2,no_bytes,&curr_offset);
					break;
				case 2:
					ret=vfs_write(filp,ip_bitmap[i].octet_3,no_bytes,&curr_offset);
					break;
				case 3:
					ret=vfs_write(filp,ip_bitmap[i].octet_4,no_bytes,&curr_offset);
			}			
			set_fs(oldfs);		
			curr_offset+=no_bytes;
		}	

		oldfs=get_fs();
		set_fs(get_ds());
			ret=vfs_write(filp,col_offsets,sizeof(col_offsets),&start_offsets[j]);
		set_fs(oldfs);		
		if(j!=3)
			start_offsets[j+1]=curr_offset;
	}
	vfree(col_offsets);
	return curr_offset;
}

/*******************************************************************************************************************************/
int compress_write_bitmap(maprecord *path_cache,struct file *filp)
{
        int ret=0;
	u_int64_t max_ports=65536;
	loff_t offset=0;
	mm_segment_t oldfs;//Shifting Segment get and setfs remaining
	bitmap_start_offsets start_off;

	start_off.src_ip_octet[0]=sizeof(bitmap_start_offsets);
	start_off.dst_ip_octet[0]=compress_write_src_dst_ip(path_cache->bit_map->src_ip,start_off.src_ip_octet,filp);
	start_off.src_port_octet=compress_write_src_dst_ip(path_cache->bit_map->dst_ip,start_off.dst_ip_octet,filp);
	start_off.dst_port_octet=compress_write_octet(max_ports,path_cache->bit_map->src_port,start_off.src_port_octet,filp);
	start_off.protocol_octet=compress_write_octet(max_ports,path_cache->bit_map->dst_port,start_off.dst_port_octet,filp);
	compress_write_octet(256,path_cache->bit_map->protocol,start_off.protocol_octet,filp);//typecasting required 
	
	oldfs=get_fs();
	set_fs(get_ds());
		ret=vfs_write(filp,&start_off,sizeof(bitmap_start_offsets),&offset);
	set_fs(oldfs);
	printk(KERN_ALERT "No problem in writing(compress_bitmap)");		
	return 0;
}

/*******************************************************************************************************************************/
int write_bit_map(maprecord *path_cache)
{
	char *bmpath=kmalloc(60,GFP_KERNEL);
	char *bmname=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	
	strcpy(bmpath,path_cache->path);
	strcat(bmpath,"BitMaps/");
	sprintf(bmname,"bitmap_%d",path_cache->GFL);
	strcat(bmpath,bmname);

	filp=file_open(bmpath,O_WRONLY|O_APPEND|O_CREAT,0777);
		compress_write_bitmap(path_cache,filp);	
	file_close(filp);

	//Clean Up and Updation Tasks	
	kfree(bmname);
	kfree(bmpath);
	printk("Completed Bitmap Code");
	return 0;	
}



/***************************Function to write offset table to File********************/
void write_offset_table(maprecord *path_cache)
{
	char *off_path=kmalloc(60,GFP_KERNEL);
	char *off_name=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	u_int32_t pktcount=0;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i;
	strcpy(off_path,path_cache->path);
	strcat(off_path,"Flowrecords/");
	sprintf(off_name,"offset_%d",path_cache->GFL);
	strcat(off_path,off_name);
	filp=file_open(off_path,O_WRONLY|O_CREAT,0664);
	//Fixing the Address Space Issue
	if(filp!=NULL)
        {
	    printk("\nNo problem in Opening offset file");
	    oldfs=get_fs();
	    set_fs(get_ds());

	    printk("\nValue of Offsets:");
	    for(i=0;i<MAX_FLOW_REC;i++)
	      {
		offset_node *start=path_cache->off_table[i].start;
		//Changes made
		path_cache->flow_start[i].start_pkt_no=pktcount;
	       	while(start)
	        {
		   if(i==0)
			printk("\nOffsets of Flow rec 0 are :%llu",start->offset);
		    filp->f_op->write(filp,&start->offset,sizeof(loff_t),&offset);//Writing Flow Records to File//Change made &start to start in write
		    start=start->next;
		}			
		pktcount+=path_cache->flow_start[i].nop;
	      }
	    set_fs(oldfs);
	    printk("\nNo Problem in Writing");
	  }
	 else
	 {
	    printk("Problem in opening filw for writing offsets");
	 }
	file_close(filp);
	//Clean Up and Updation Tasks	
	kfree(off_name);
	kfree(off_path);	
}


/*****************Function to write Flow Records********************/

int write_flow_rec(maprecord *path_cache)
{
	char *frpath=kmalloc(60,GFP_KERNEL);
	char *fname=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i,ret=0;
        printk(KERN_INFO "\nFlow Records Buffer Full !!!!");
	strcpy(frpath,path_cache->path);
	strcat(frpath,"Flowrecords/");
	sprintf(fname,"flowrec_%d",path_cache->GFL);
	strcat(frpath,fname);
	printk("\nFlowFileName : %s",frpath);
	filp=file_open(frpath,O_WRONLY|O_APPEND|O_CREAT,0777);
	if(filp!=NULL)
	{
		//Fixing the Address Space Issue
		printk("\nNo problem in Opening File(FLowRecWriting)");
		oldfs=get_fs();
		set_fs(get_ds());
		for(i=0;i<MAX_FLOW_REC;i++)
		{
			 ret=filp->f_op->write(filp,&path_cache->flow_start[i],sizeof(flow_record),&offset);//Writing Flow Records to File
		}
		set_fs(oldfs);
		printk("\nNo Problem in Writing");
	}
	else
	{
		printk(KERN_INFO "File Could not be opened to write Flow Records");
	}
	file_close(filp);
	//Clean Up and Updation Tasks	
	kfree(fname);
	kfree(frpath);
	return ret;	
}

//******************************Function to write Packet to Disks******************************//
int write_packet(struct pfring_pkthdr *pkthdr,unsigned char *buf,maprecord *path_cache)
{
	char *pktpath=kmalloc(50,GFP_KERNEL);
	struct file *filp;
	u_int32_t ret=0;
	loff_t offset=0;
	mm_segment_t oldfs;	
	//printk("\nInitial path is::%s",path_cache->path);
	strcpy(pktpath,path_cache->path);
	strcat(pktpath,"Packets/");
	strcat(pktpath,"pkttrace");	

	//Dumping this packet to disk in Packet Trace File
	filp=file_open(pktpath,O_WRONLY|O_APPEND|O_CREAT,0777);
	if(filp!=NULL)
	{
	//Fixing the Address Space Issue
		printk("\nNo problem in Opening File");
		oldfs=get_fs();
		set_fs(get_ds());
		ret=filp->f_op->write(filp,pkthdr,sizeof(struct pfring_pkthdr),&offset);//Replace vfs_write with file operations write function
		ret=filp->f_op->write(filp,buf,pkthdr->caplen,&offset);
		set_fs(oldfs);
		printk("\nNo Problem in Writing");
	}
	else
	{
		printk("\nProblem in writing packet");
	}
	file_close(filp);
	//Freeing the memory allocated
	kfree(pktpath);
	return ret;
}

/***************************FUNCTION TO CREATE NEW FLOW RECORD************************/
/*Function to create Flow record from Pfring Pktheader 
  I/P:PF_Ring pkthdr 
  O/P:Flow Record created*/

void create_flow_record(flow_record *fr,struct pfring_pkthdr *pkf,u_int32_t flowhash,maprecord *path_cache)
{
	fr->flow_id=path_cache->GFL + flowhash;
	fr->nop=1;
	fr->src_port=pkf->extended_hdr.parsed_pkt.l4_src_port;
	fr->dst_port=pkf->extended_hdr.parsed_pkt.l4_dst_port;
	fr->ip_src=pkf->extended_hdr.parsed_pkt.ip_src;
	fr->ip_dst=pkf->extended_hdr.parsed_pkt.ip_dst;
	fr->protocol=pkf->extended_hdr.parsed_pkt.l3_proto;
	//fr->timestamp_ns=
	fr->nxtfr=0;
	//fr->start_pkt_no=path_cache->GPC;
	//Updation of Offset Field in path_cache	
	printk("PKT offset when assigning is :%llu",path_cache->pkt_offset);
	path_cache->off_table[flowhash].start=path_cache->off_table[flowhash].end=get_offset_node(path_cache->pkt_offset);
	path_cache->pkt_offset+=(sizeof(struct pfring_pkthdr) + pkf->caplen);
	path_cache->GPC++;		
	make_bitmap_entry(fr,path_cache->bit_map,flowhash);
	count++;
	printk("\nFR count is :%d",count);	      
}

void write_all(maprecord *path_cache)
{
	write_offset_table(path_cache);
	free_offset_nodes(path_cache);
	write_flow_rec(path_cache);
	write_bit_map(path_cache);
}

void set_zero_all(maprecord *path_cache)
{
	memset(path_cache->flow_start,0,(MAX_FLOW_REC*sizeof(flow_record)));
	memset(path_cache->bit_map->src_ip,0,256 * sizeof(ip_bits));
	memset(path_cache->bit_map->dst_ip,0,256 * sizeof(ip_bits));
	memset(path_cache->bit_map->src_port,0,65536*sizeof(port_bits));
	memset(path_cache->bit_map->dst_port,0,65536*sizeof(port_bits));	
	memset(path_cache->bit_map->protocol,0,256*sizeof(protocol_bits));
}

/**************************FUNCTION TO UPDATE FLOW RECORD*******************************************/
void update_flow_rec(maprecord *path_cache,struct pfring_pkthdr *pkthdr)
{
        u_int32_t flowhash,org_flowhash,chklink;
	
	org_flowhash=flowhash=calc_hash(pkthdr);                       //Input is the pfring_pkthdr which has all the five fields needed to compute hash
    	flowhash%=MAX_FLOW_REC;
	org_flowhash%=MAX_FLOW_REC;

	printk("\nFlow hash is :%u",flowhash);

		if(path_cache->flow_start[flowhash].nop==0)
	    	{
		          create_flow_record(&path_cache->flow_start[flowhash],pkthdr,flowhash,path_cache);
	    	}	
	  	else
	    	{
	      		struct pkt_parsing_info *pkt_data=&pkthdr->extended_hdr.parsed_pkt;
	      		while(1)
			{
				  flow_record *tempfr=&path_cache->flow_start[flowhash];
		  		  if(*((u_int32_t*)&tempfr->ip_src)==*((u_int32_t*)&pkt_data->ip_src) && *((u_int32_t*)&tempfr->ip_dst)==*((u_int32_t*)&pkt_data->ip_dst)  && tempfr->src_port==pkt_data->l4_src_port && tempfr->dst_port==pkt_data->l4_dst_port && tempfr->protocol==pkt_data->l3_proto)
		    		{
				      //Packet belong to existing flow, update the flow record i.e.No. of Packets and Offset Table
				      printk(KERN_INFO "\nAll Five Fields are Equal ");
				      path_cache->flow_start[flowhash].nop+=1;
				      path_cache->off_table[flowhash].end->next=get_offset_node(path_cache->pkt_offset);
				      path_cache->off_table[flowhash].end=path_cache->off_table[flowhash].end->next;
				      path_cache->GPC++;	
				      path_cache->pkt_offset+=(sizeof(struct pfring_pkthdr) + pkthdr->caplen);
				      break;			  
				  }
				  else
				  {
					      if((chklink=path_cache->flow_start[flowhash].nxtfr))//
					      {
							   flowhash=chklink;
					      } 
					      else
					      {		
							  int i;		
							  for(i=flowhash+1;i!=flowhash;i=(i+1)%MAX_FLOW_REC)
							  {
								      if(path_cache->flow_start[i].nop==0)
									      break;
							  }
							  if(i!=flowhash)
							  {
								      create_flow_record(&path_cache->flow_start[i],pkthdr,i,path_cache);
								      path_cache->flow_start[flowhash].nxtfr=i;					 
							  }
							  else
							  {
								      //flush the flow records to disk
								      write_all(path_cache);
								      set_zero_all(path_cache);				      
								      path_cache->GFL+=MAX_FLOW_REC;
								      flowhash=org_flowhash;
							              create_flow_record(&path_cache->flow_start[flowhash],pkthdr,flowhash,path_cache);
							    }	
							  break;
						}
				    }		
			}
	     }
	
}

/**************************FUNCTION to ALLOCATE SPACE for FLOW RECORD*****************************/

void create_flow_space(maprecord *path_cache)
{
        path_cache->flow_start=vmalloc(MAX_FLOW_REC * sizeof(flow_record));
	path_cache->off_table=vmalloc(MAX_FLOW_REC*sizeof(offset_table));
	memset(path_cache->off_table,0,(MAX_FLOW_REC*sizeof(offset_table)));	
	path_cache->bit_map=kmalloc(sizeof(bitmap),GFP_KERNEL);	
	path_cache->bit_map->src_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
	path_cache->bit_map->dst_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
	path_cache->bit_map->src_port=vmalloc(65536*sizeof(port_bits));
	path_cache->bit_map->dst_port=vmalloc(65536*sizeof(port_bits));	
	path_cache->bit_map->protocol=kmalloc(256*sizeof(protocol_bits),GFP_KERNEL);
	set_zero_all(path_cache);
}

/**********************FUNCTION TO HANDLE PACKET AFTER CAPTURE PROCESS******************************/
void handle_storage_pkt(struct pfring_pkthdr *pkthdr,unsigned char *buf,maprecord *path_cache)
{		
	update_flow_rec(path_cache,pkthdr);	
	write_packet(pkthdr,buf,path_cache);	
}

int capture_thread(void *arg)
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
	maprecord *path_cache=(maprecord*)arg;

	//create socket
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

	//set bucket length
	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_RING_BUCKET_LEN,&caplen, sizeof(caplen));
	if (ret != 0)
	{
		printk("setsockopt failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("setsockopt success\n");
	}	
		
	//bind

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_RING;
  	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", path_cache->interface_name);

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

	//mmap equivalent in kernel i.e no mmap
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

	//get header length
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

	//set direction
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
  	
	//activate ring

	ret=capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_ACTIVATE_RING, &dummy, sizeof(dummy));
	
	if(ret!=0)
	{
		printk("\nError in enableing ring\n");		
		sock_release(capture_sockfd);
		return -1;
	}
	else
		printk("\nRing Activated Successfully");
	//set filters
	//set_filtering_rule(capture_sockfd,5,6);
	//set_filtering_rule(capture_sockfd,6,17);
	while(1) {
		//for(i=0;i<100;i++){
		if (kthread_should_stop())
		{
			printk("\nIn kthread stop......");
			//write_all(path_cache);
			//free_path_cache(path_cache);	
			break;
		}
		ret = recv_pack(&actual_buffer, 0, &hdr,slots,slots_info);
		if(ret < 0)
			break;
		else if(ret != 0)
		{	
			looper(&hdr, actual_buffer);
			print_ethernet_header(&hdr); 
			handle_storage_pkt(&hdr,actual_buffer,path_cache);		
		}	
		else {
			msleep(1);
		}
		//msleep(10);
		//break;	
	  }
	

	sock_release(capture_sockfd);
	//sock_release(fd2);
	return 0;
}


maprecord **map_list;

int create_capture_thread(void *arg)
{
	map_list=kmalloc(256*sizeof(maprecord *),GFP_KERNEL);	

	struct socket *sockfd_srv=NULL, *sockfd_cli=NULL;
	struct sockaddr_in addr_srv;
	struct sockaddr_in addr_cli;
	char buf[1024];
	int addr_len, len;
	int backlog = 10,tot_threads=0;	

	sockfd_srv = sockfd_cli = NULL;
	memset(&addr_cli, 0, sizeof(addr_cli));
	memset(&addr_srv, 0, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(PORT);
	addr_srv.sin_addr.s_addr = INADDR_ANY;
	addr_len = sizeof(struct sockaddr_in);
	lookupcreate();

	if(sock_create(AF_INET, SOCK_STREAM, 0, &sockfd_srv)<0)
	{
		printk("Socket creation failed");		
		return -1;
	}
	else
		printk("socket created successfully");
	if(sockfd_srv->ops->bind(sockfd_srv, (struct sockaddr *)&addr_srv, addr_len)<0)
	{
		printk("\nBinding failed");
		sock_release(sockfd_srv);
		return -1;
	}
	else
		printk("\nBinded successfully");
	if ((unsigned)backlog > SOMAXCONN)
		backlog = SOMAXCONN;
	if(sockfd_srv->ops->listen(sockfd_srv, backlog)<0)
	{
		printk("\nListening failed");
		sock_release(sockfd_srv);
		return -1;
	}
	else
		printk("\nListening successfully");
	while(1)
	{
		int len;
		vm_id *vmid=kmalloc(sizeof(vm_id),GFP_KERNEL);
		sockfd_cli = sock_accept(sockfd_srv, (struct sockaddr *)&addr_cli, &addr_len);
		if (sockfd_cli == NULL)
		{
			printk("\naccept failed\n");
			return -1;
		}
		else
			printk("sockfd_cli = 0x%p\n", sockfd_cli);
		len = sock_recv(sockfd_cli, buf, sizeof(buf), 0);
		if (len > 0)
		{
			printk("\ngot message : %s\n", buf);
		}
		if(!strcmp(buf,"START"))
		{
			u_int16_t map_count=0;
			len = sock_recv(sockfd_cli, buf, sizeof(buf), 0);
			printk("\nSelected Start");
			if (len > 0)
			{
				printk("\ngot message : %s\n", buf);
			}
			if(!(strstr(buf,"eth") || strstr(buf,"wlan")|| strstr(buf,"vif")))
				break;		
			vmid->v_id[0]=1234+tot_threads;vmid->v_id[1]=2345;vmid->v_id[2]=3456;vmid->v_id[3]=4567;
			map_count=map_hash(1234,vmid);						
			map_list[map_count]=kmalloc(sizeof(maprecord),GFP_KERNEL); 
			memset(map_list[map_count],0,sizeof(maprecord));
			printk("Offset is at start:%llu",map_list[map_count]->pkt_offset);
			strcpy(map_list[map_count]->path,"storage/hs1234/");
			strcpy(map_list[map_count]->interface_name,buf);
			printk("\nInterface :%s",map_list[map_count]->interface_name);
			map_list[map_count]->cust_id=1234;
			memcpy(&map_list[map_count]->vmid,vmid,sizeof(vm_id));
			create_flow_space(map_list[map_count]);
			len = sprintf(buf, "%d",map_count);
			printk("\nSent hash value:%s",buf);
			sock_send(sockfd_cli, buf, len+1, 0);
			map_list[map_count]->ts=kthread_run(capture_thread,(void *)map_list[map_count],"kthread");//incrementing
			kfree(vmid);	
			tot_threads++;
		}
		else if(!strcmp(buf,"STOP"))
		{
			int index;
			printk("\nYou selected STOP");
			//msleep(0);
			len = sock_recv(sockfd_cli, buf, sizeof(buf), 0);
			printk("\nLength of Message : %d",len);
			if (len > 0)
			{
				printk("\ngot message : %s\n", buf);
			}
			index=atoi(buf);
			printk("\nIndex is :%d",index);
			kthread_stop(map_list[index]->ts);
			//write_all(map_list[index]);
			if(map_list[index])
				free_path_cache(map_list[index]);
			map_list[index]=0;
			
		}
		else if(!strcmp(buf,"EXIT"))
		{
			int l;
			for(l=0;l<256;l++)
			{
				if(map_list[l])
				{
					kthread_stop(map_list[l]->ts);
					//write_all(map_list[l]);
					//free_path_cache(map_list[l]);					
				}				
			}
			sock_close(sockfd_cli);
			break;
		}
		sock_close(sockfd_cli);
	}	
	sock_close(sockfd_srv);	
	return 0;
}


static int ker_pf_ring_init(void)
{	
	printk(KERN_ALERT "Initializing PF_RING kernel module");
	kernel_thread(create_capture_thread, NULL, 0);
	return 0;
}



static void ker_pf_ring_exit(void)
{
	printk(KERN_ALERT "Exiting PF_RING kernel module");
	//kthread_stop(ts2);
}



module_init(ker_pf_ring_init);
module_exit(ker_pf_ring_exit);
MODULE_LICENSE("Dual BSD/GPL");
