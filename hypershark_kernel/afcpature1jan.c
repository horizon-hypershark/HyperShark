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
#include<linux/vmalloc.h>
#include<linux/string.h>
#include"afcapture.h"
/********************************************************************************************************************************/

#define DOMAIN AF_PACKET
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)
#define BUFSIZE 65536
#define AFCAPTURE_NAME "afcapture"
#define AFCAPTURE_AUTHOR "horizon"
#define AFCAPTURE_DESC "a module to capture all packets"
#define PATH_TO_CONFIG "Mapping.config"
#define MAX_FLOW_REC 100

/********************************************************************************************************************************/

MODULE_AUTHOR(AFCAPTURE_AUTHOR);
MODULE_DESCRIPTION(AFCAPTURE_NAME"\n"AFCAPTURE_DESC);
MODULE_LICENSE("Dual BSD/GPL");

/********************************************************************************************************************************/

struct socket *capture_sockfd;
struct sockaddr_in source,dest;
u_int32_t count=0;
u_int32_t pktid=1;

/********************************************************************************************************************************/

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
	sprintf(str_ip, "%d.%d.%d.%d",  (int_ip) & 0xFF,(int_ip >> 8 ) & 0xFF,(int_ip >> 16) & 0xFF,(int_ip >> 24) & 0xFF);
	return str_ip;
}

/********************************************************************************************************************************/

void looper(const struct pfring_pkthdr *hdr, const u_char *p) {
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

/********************************************************************************************************************************/

int parse_pkt(u_char *pkt, struct pfring_pkthdr *hdr)
{
	struct iphdr *ip;
	struct eth_hdr *eh = (struct eth_hdr*)pkt;
	u_int16_t displ;
	int i;

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

/********************************************************************************************************************************/

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
	printk(KERN_ALERT "   |-Protocol            : %u \n",(unsigned short)ntohs(eth->h_proto));
}

/********************************************************************************************************************************/

ssize_t pack_recv(struct socket* sk, void *buffer, size_t length, int flags)
{
	struct msghdr msg;
	struct iovec iov;
	int ret=0;
	mm_segment_t old_fs;

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	ret = sock_recvmsg(sk, &msg, length, flags);
	set_fs(old_fs);
	if (ret < 0)
		goto out_krecv;
	
out_krecv:
	return ret;
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
int compress_write_bitmap(maprecord *path_cache,u_int32_t dirhash,struct file *filp)
{
        int ret=0;
	u_int64_t max_ports=65536;
	loff_t offset=0;
	mm_segment_t oldfs;//Shifting Segment get and setfs remaining
	bitmap_start_offsets start_off;

	start_off.src_ip_octet[0]=sizeof(bitmap_start_offsets);
	start_off.dst_ip_octet[0]=compress_write_src_dst_ip(path_cache[dirhash].bit_map->src_ip,start_off.src_ip_octet,filp);
	start_off.src_port_octet=compress_write_src_dst_ip(path_cache[dirhash].bit_map->dst_ip,start_off.dst_ip_octet,filp);
	start_off.dst_port_octet=compress_write_octet(max_ports,path_cache[dirhash].bit_map->src_port,start_off.src_port_octet,filp);
	start_off.protocol_octet=compress_write_octet(max_ports,path_cache[dirhash].bit_map->dst_port,start_off.dst_port_octet,filp);
	compress_write_octet(256,path_cache[dirhash].bit_map->protocol,start_off.protocol_octet,filp);//typecasting required 
	
	oldfs=get_fs();
	set_fs(get_ds());
		ret=vfs_write(filp,&start_off,sizeof(bitmap_start_offsets),&offset);
	set_fs(oldfs);
	printk(KERN_ALERT "No problem in writing(compress_bitmap)");		
	return 0;
}
/*******************************************************************************************************************************/
int write_bit_map(maprecord *path_cache,u_int32_t dirhash)
{
	char *bmpath=kmalloc(30,GFP_KERNEL);
	char *bmname=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	
	strcpy(bmpath,path_cache[dirhash].path);
	strcat(bmpath,"BitMaps/");
	sprintf(bmname,"bitmap_%d",path_cache[dirhash].GFL);
	strcat(bmpath,bmname);

	filp=file_open(bmpath,O_WRONLY|O_APPEND|O_CREAT,0777);
		compress_write_bitmap(path_cache,dirhash,filp);	
	file_close(filp);

	//Clean Up and Updation Tasks	
	kfree(bmname);
	kfree(bmpath);
	printk("Completed Bitmap Code");
	return 0;	
}
/******************************************************************************************************************************/

/*****************************HASH FUNCTION FOR FINDING PATHNAME***********************************/
/*Computes the hash based on the Customer and VM Id to obtain index
into the array of cached Cust and VM id's or to lookup into the file
I/P:-Customer Id and VM Id
O/P:-Hash Value*/
u_int32_t dir_hash(u_int32_t cust_id,vm_id *vmid)
{
	u_int32_t hash=0;
	u_int8_t i;
	hash=cust_id;
	for(i=0;i<4;i++)
		hash+=vmid->v_id[i];
       	return hash%256;
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

/*************************Function to retrieve Path of Cus + VM pair***************************************/

void get_path_cache_entry(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,u_int32_t map_location,maprecord *path_cache)
{
	int ret;
	loff_t offset=0;
	mm_segment_t oldfs;
	struct file *filp;
	maprecord *map=&path_cache[map_location];
	/*To fix the address space mismatch as the kernel
	expects the pointer to come from userspace and when
	we pass a kernal space pointer to the function 'BAD ADDRESS(-14)'
	error occurs. Hence the functions get_fs and set_fs modify the current
	address limits to whatever the caller wants
	1.] We save the current address limits
	2.] Set the kernel address limits and once work is done
	3.] We restore the previous address limits*/
        filp=file_open(PATH_TO_CONFIG,O_RDONLY,0664);                        //Opening a file
	oldfs=get_fs();
	set_fs(get_ds());
	while(1)
	{
		printk(KERN_INFO "Inside the File Read Loop");
		offset+=dirhash*sizeof(maprecord);
		ret=vfs_read(filp,map,sizeof(maprecord),&offset); 
		//to typecast the retrieved buffer into its corresponding structure
		printk(KERN_INFO "Customer Id :%d",map->cust_id);
		if(map->cust_id==cust_id && (!cmp_vmids(&map->vmid,vmid)))
		{
			map->link=0;
			printk(KERN_INFO "Finished Reading");
			break;
		}      
		else
		{
		     dirhash=map->link;
		     offset=0;
		     printk(KERN_INFO "Still Reading");
		}
	}
	set_fs(oldfs);
	printk(KERN_INFO "\nInside the read function Buffer= %s",map->path);
	printk(KERN_INFO "\nReturned Bytes:%d:",ret);
	file_close(filp);
}

/**************************FUNCTION to ALLOCATE SPACE for FLOW RECORD*****************************/

void create_flow_space(maprecord *path_cache,u_int32_t dirhash)
{
        path_cache[dirhash].flow_start=vmalloc(MAX_FLOW_REC * sizeof(flow_record));
	memset(path_cache[dirhash].flow_start,0,(MAX_FLOW_REC*sizeof(flow_record)));
	path_cache[dirhash].off_table=vmalloc(MAX_FLOW_REC*sizeof(offset_table));
	memset(path_cache[dirhash].off_table,0,(MAX_FLOW_REC*sizeof(offset_table)));	
	path_cache[dirhash].bit_map=kmalloc(sizeof(bitmap),GFP_KERNEL);	
	path_cache[dirhash].bit_map->src_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
	path_cache[dirhash].bit_map->dst_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
	path_cache[dirhash].bit_map->src_port=vmalloc(65536*sizeof(port_bits));
	path_cache[dirhash].bit_map->dst_port=vmalloc(65536*sizeof(port_bits));	
	path_cache[dirhash].bit_map->protocol=kmalloc(256*sizeof(protocol_bits),GFP_KERNEL);
}

/*****************Function to write Flow Records********************/

int write_flow_rec(maprecord *path_cache,u_int32_t dirhash)
{
	char *frpath=kmalloc(30,GFP_KERNEL);
	char *fname=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i,ret=0;
        printk("No. of FRRRRRRRR'sss: %u",count);
        printk(KERN_INFO "\nFlow Records Buffer Full !!!!");
	strcpy(frpath,path_cache[dirhash].path);
	strcat(frpath,"Flowrecords/");
	//sprintf(fname,"%d_%d",path_cache[dirhash].GFL,path_cache[dirhash].GFL + (MAX_FLOW_REC-1));
	sprintf(fname,"flowrec_%d",path_cache[dirhash].GFL);
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
	        //path_cache[dirhash].flow_start[i].flow_id=path_cache[dirhash].GFL+i;
	 	 ret=filp->f_op->write(filp,&path_cache[dirhash].flow_start[i],sizeof(flow_record),&offset);//Writing Flow Records to File
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

/***************************Function to write offset table to File********************/

void write_offset_table(maprecord *path_cache,u_int32_t dirhash)
{
	char *off_path=kmalloc(30,GFP_KERNEL);
	char *off_name=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i;
	strcpy(off_path,path_cache[dirhash].path);
	strcat(off_path,"Flowrecords/");
	//sprintf(off_name,"%d_%d_off",path_cache[dirhash].GFL,path_cache[dirhash].GFL + (MAX_FLOW_REC-1));
	sprintf(off_name,"offset_%d",path_cache[dirhash].GFL);
	strcat(off_path,off_name);
	filp=file_open(off_path,O_WRONLY|O_APPEND|O_CREAT,0664);
	//Fixing the Address Space Issue
	if(filp!=NULL)
	  {
	    printk("\nNo problem in Opening offset file");
	    oldfs=get_fs();
	    set_fs(get_ds());

	    printk("\nValue of Offsets:");
	    for(i=0;i<MAX_FLOW_REC;i++)
	      {
		offset_node *start=path_cache[dirhash].off_table[i].start;
	       	while(start)
		  {
		    filp->f_op->write(filp,&start->offset,sizeof(loff_t),&offset);//Writing Flow Records to File
		    /*		if(i==0)
				printk("%llu->",start->offset);
				temp=start;*/
		    start=start->next;
		    //kfree(temp);
		  }			
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

/***************************FUNCTION TO CREATE NEW FLOW RECORD************************/
/*Function to create Flow record from Pfring Pktheader 
  I/P:PF_Ring pkthdr 
  O/P:Flow Record created*/

void create_flow_record(flow_record *fr,struct pfring_pkthdr *pkf,u_int32_t dirhash,u_int32_t flowhash,maprecord *path_cache)
{
              fr->flow_id=path_cache[dirhash].GFL + flowhash;
              fr->nop=1;
              fr->src_port=pkf->extended_hdr.parsed_pkt.l4_src_port;
              fr->dst_port=pkf->extended_hdr.parsed_pkt.l4_dst_port;
              fr->ip_src=pkf->extended_hdr.parsed_pkt.ip_src;
              fr->ip_dst=pkf->extended_hdr.parsed_pkt.ip_dst;
              fr->protocol=pkf->extended_hdr.parsed_pkt.l3_proto;
	      //fr->timestamp_ns=
	      fr->nxtfr=0;
	      fr->start_pkt_no=path_cache[dirhash].GPC;
              //Updation of Offset Field in path_cache
	      path_cache[dirhash].off_table[flowhash].start=path_cache[dirhash].off_table[flowhash].end=get_offset_node(path_cache[dirhash].pkt_offset);
	      path_cache[dirhash].GPC++;	
              path_cache[dirhash].pkt_offset+=sizeof(hs_pkt_hdr);
	      make_bitmap_entry(fr,path_cache[dirhash].bit_map,flowhash);	      
}

//******************************Function to read directory Path******************************//

int update_path_cache(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,maprecord *path_cache)
{
	u_int32_t temp;
	while(1)
	{
		if(path_cache[dirhash].cust_id==0)
		{
			get_path_cache_entry(cust_id,vmid,dirhash,dirhash,path_cache);
			create_flow_space(path_cache,dirhash);
			break;
		}
		else if(path_cache[dirhash].cust_id==cust_id && (!cmp_vmids(&path_cache[dirhash].vmid,vmid)))
		{
			printk("\nPath_cache entry found");
			break;
		}
		else
		{
			if(path_cache[dirhash].link)
				dirhash=path_cache[dirhash].link;
			else
			{
				temp=(dirhash+1)%256;
				while(path_cache[temp].cust_id==0)
					temp=(temp+1)%256;
				path_cache[dirhash].link=temp;	
				get_path_cache_entry(cust_id,vmid,dirhash,temp,path_cache);
				create_flow_space(path_cache,temp);
				dirhash=temp;
				break;
			}
		}
	
	}
	return dirhash;
}

/********************Free Offset Nodes********************/

void free_offset_nodes(maprecord* path_cache,u_int32_t dirhash)
{
	printk(KERN_INFO "In function update_flow_rec");
	if(path_cache[dirhash].flow_start)
	{		
	       int j;				
	       for(j=0;j<MAX_FLOW_REC;j++)
		{
			offset_node *start=path_cache[dirhash].off_table[j].start;
			while(start)
			{
				offset_node *tmp=start->next;
				kfree(start);
				start=tmp;
			}
		}
	} 
}

/**************************FUNCTION TO UPDATE FLOW RECORD*******************************************/
void update_flow_rec(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,maprecord *path_cache,struct pfring_pkthdr *pkthdr)
{
        u_int32_t flowhash,org_flowhash,chklink;
	printk(KERN_INFO "In function update_flow_rec");
	dirhash=update_path_cache(cust_id,vmid,dirhash,path_cache);

	org_flowhash=flowhash=calc_hash(pkthdr);                       //Input is the pfring_pkthdr which has all the five fields needed to compute hash
    	flowhash%=MAX_FLOW_REC;
	org_flowhash%=MAX_FLOW_REC;

	printk("\nFlow hash is :%u",flowhash);

		if(path_cache[dirhash].flow_start[flowhash].nop==0)
	    	{
		          create_flow_record(&path_cache[dirhash].flow_start[flowhash],pkthdr,dirhash,flowhash,path_cache);
			  count++;
			  printk("No. of FRRRRRRRR'sss: %u",count);
	    	}	
	  	else
	    	{
	      		struct pkt_parsing_info *pkt_data=&pkthdr->extended_hdr.parsed_pkt;
	      		while(1)
			{
				  flow_record *tempfr=&path_cache[dirhash].flow_start[flowhash];
		  		  if(*((u_int32_t*)&tempfr->ip_src)==*((u_int32_t*)&pkt_data->ip_src) && *((u_int32_t*)&tempfr->ip_dst)==*((u_int32_t*)&pkt_data->ip_dst)  && tempfr->src_port==pkt_data->l4_src_port && tempfr->dst_port==pkt_data->l4_dst_port && tempfr->protocol==pkt_data->l3_proto)
		    		{
				      //Packet belong to existing flow, update the flow record i.e.No. of Packets and Offset Table
				      printk(KERN_INFO "\nAll Five Fields are Equal ");
				      path_cache[dirhash].flow_start[flowhash].nop+=1;
				      path_cache[dirhash].off_table[flowhash].end->next=get_offset_node(path_cache[dirhash].pkt_offset);
				      path_cache[dirhash].off_table[flowhash].end=path_cache[dirhash].off_table[flowhash].end->next;
				      path_cache[dirhash].GPC++;	
				      path_cache[dirhash].pkt_offset+=sizeof(hs_pkt_hdr);
                                      printk("No. of FRRRRRRRR'sss: %u",count);
				      break;			  
				  }
				  else
				  {
					      if((chklink=path_cache[dirhash].flow_start[flowhash].nxtfr))//
					      {
							   flowhash=chklink;
					      } 
					      else
					      {		
							  int i;		
							  for(i=flowhash+1;i!=flowhash;i=(i+1)%MAX_FLOW_REC)
							  {
								      if(path_cache[dirhash].flow_start[i].nop==0)
									      break;
							  }
							  if(i!=flowhash)
							  {
								      printk(KERN_ALERT "Reentry Found space remaining");
								      create_flow_record(&path_cache[dirhash].flow_start[i],pkthdr,dirhash,i,path_cache);
								      path_cache[dirhash].flow_start[flowhash].nxtfr=i;					 
								      count++;
                                                                      printk("No. of FRRRRRRRR'sss: %u",count);
							  }
							  else
							  {
                                                                      printk("No. of FRRRRRRRR'sss: %u",count);
								      printk(KERN_INFO"Flow Records Buffer Full !!!!");
								      //flush the flow records to disk
								      write_flow_rec(path_cache,dirhash);
								      write_offset_table(path_cache,dirhash);
								      printk(KERN_INFO "In function update_flow_rec before free");
								      free_offset_nodes(path_cache,dirhash);
								      printk(KERN_INFO "In function update_flow_rec after free");
								      write_bit_map(path_cache,dirhash);
								      memset(path_cache[dirhash].flow_start,0,(MAX_FLOW_REC*sizeof(flow_record)));
								      path_cache[dirhash].GFL+=MAX_FLOW_REC;
								      flowhash=org_flowhash;
							              create_flow_record(&path_cache[dirhash].flow_start[flowhash],pkthdr,dirhash,flowhash,path_cache);			                                          count++;
							    }	
							  break;
						}
				    }		
			}
	     }
	
}
//******************************Function to write Packet to Disks******************************//
int write_packet(struct pfring_pkthdr *pkthdr,unsigned char *buf,maprecord *path_cache,u_int32_t dirhash)
{
	char *pktpath=kmalloc(30,GFP_KERNEL);
	struct file *filp;
	u_int32_t ret=0;
	loff_t offset=0;
	mm_segment_t oldfs;
	hs_pkt_hdr *hspkt=kmalloc(sizeof(hs_pkt_hdr),GFP_KERNEL);
	hspkt->pkt_id=pktid++;
	hspkt->pkf=*pkthdr;
	//strcpy(hspkt->buffer,buf);
	memcpy(hspkt->buffer,buf,1500);        
	hspkt->buffer_len=strlen(buf);
	strcpy(pktpath,path_cache[dirhash].path);
	strcat(pktpath,"Packets/");
	strcat(pktpath,"pkttrace");
	printk(KERN_INFO "Packet Storage Path is:%s",pktpath);

	//Dumping this packet to disk in Packet Trace File
	filp=file_open(pktpath,O_WRONLY|O_APPEND|O_CREAT,0777);
	if(filp!=NULL)
	{
	//Fixing the Address Space Issue
		printk("\nNo problem in Opening File");
		oldfs=get_fs();
		set_fs(get_ds());
		ret=filp->f_op->write(filp,hspkt,sizeof(hs_pkt_hdr),&offset);//Replace vfs_write with file operations write function
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
	kfree(hspkt);
	return ret;
}

/**********************FUNCTION TO HANDLE PACKET AFTER CAPTURE PROCESS******************************/
void handle_storage_pkt(u_int32_t cust_id,vm_id *vmid,struct pfring_pkthdr *pkthdr,unsigned char *buf,maprecord *path_cache)
{
	u_int32_t dirhash;
	dirhash=dir_hash(cust_id,vmid);		
	printk("Now Hash is:%u",dirhash);
	update_flow_rec(cust_id,vmid,dirhash,path_cache,pkthdr);	
	write_packet(pkthdr,buf,path_cache,dirhash);	
}
//******************************Freeing all the Memory allocated********************//
void free_path_cache(maprecord *path_cache)
{
        int i;
	for(i=0;i<256;i++)
	{
		if(path_cache[i].flow_start)
		{		
		      	vfree(path_cache[i].flow_start);
		      	vfree(path_cache[i].off_table);
			kfree(path_cache[i].bit_map->src_ip);
			kfree(path_cache[i].bit_map->dst_ip);
			vfree(path_cache[i].bit_map->src_port);
			vfree(path_cache[i].bit_map->dst_port);
			kfree(path_cache[i].bit_map->protocol);
		}
	}
	kfree(path_cache);
}

//******************************Main Thread to Capture Packets********************//
int capture_thread(void *arg)
{
	struct pfring_pkthdr pkthdr;
	u_int32_t cust_id=1234;//Temporary
	vm_id *vmid=kmalloc(sizeof(vm_id),GFP_KERNEL);
	maprecord *path_cache=kmalloc(256*sizeof(maprecord),GFP_KERNEL);//Cache Block to hold Customer-Path Records
	unsigned char *buf=(unsigned char *)kmalloc(BUFSIZE,GFP_KERNEL);
	int len=0;//i=0;
	int ret = sock_create(DOMAIN, TYPE , PROTOCOL , &capture_sockfd);
	memset(path_cache,0,(256*sizeof(maprecord)));	
	//Temprory to run the code
	 vmid->v_id[0]=1234;vmid->v_id[1]=2345;vmid->v_id[2]=3456;vmid->v_id[3]=4567;
        if (ret<0)
	{
		printk("socket creation failed\n");
		return -1;
	}
	memset(&pkthdr, 0, sizeof(struct pfring_pkthdr));
	printk("capture_sockfd = 0x%p\n", capture_sockfd);
        lookupcreate();                                   //Lookup Table for hash of Flow Records
	printk(KERN_INFO "Lookup Table Created");
	while(count<=100)//
	{
		len=pack_recv(capture_sockfd, buf, BUFSIZE, 0);
		if(len>0)
		{
		        parse_pkt(buf, &pkthdr);
			looper(&pkthdr, buf);
			print_ethernet_header(&pkthdr);
			handle_storage_pkt(cust_id,vmid,&pkthdr,buf,path_cache);			
		}
	}
	kfree(buf);
        kfree(vmid);
	free_path_cache(path_cache);	
	return 0;
}

//********************Start Module Function******************//
static int af_capture_init(void)
{
	printk(KERN_ALERT "\nInserted module: %s\nauthor: %s\ndesc: %s\n",AFCAPTURE_NAME,AFCAPTURE_AUTHOR,AFCAPTURE_DESC);
	kernel_thread(capture_thread, NULL, 0);
	return 0;
}
//********************Exit Module Function******************//
static void af_capture_exit(void)
{
	capture_sockfd->ops->release(capture_sockfd);
	sock_release(capture_sockfd);
	printk(KERN_ALERT "\nRemoving module: %s\n",AFCAPTURE_NAME);
}
//******************Init and Exit Modules********************//
module_init(af_capture_init);
module_exit(af_capture_exit);
