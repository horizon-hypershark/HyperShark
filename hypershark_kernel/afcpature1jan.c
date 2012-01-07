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
#include"afcapture.h"


#define DOMAIN AF_PACKET
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)
#define BUFSIZE 65536
#define AFCAPTURE_NAME "afcapture"
#define AFCAPTURE_AUTHOR "horizon"
#define AFCAPTURE_DESC "a module to capture all packets"
#define PATH_TO_CONFIG "Mapping.config"


MODULE_AUTHOR(AFCAPTURE_AUTHOR);
MODULE_DESCRIPTION(AFCAPTURE_NAME"\n"AFCAPTURE_DESC);
MODULE_LICENSE("Dual BSD/GPL");


struct socket *capture_sockfd;
struct sockaddr_in source,dest;
int pktid=1;

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

offset_node *get_offset_node(loff_t offset)
{
	offset_node *t=kmalloc(sizeof(offset_node),GFP_KERNEL);
	t->offset=offset;
	t->next=NULL;
	return t;
}

//----------------------------------------FUNCTION TO OPEN FILE----------------------------------------//
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

  /*If Error in file open catch the catch and return else return the 
    File Descriptor returned*/
  if(IS_ERR(filp)){
    err=PTR_ERR(filp);
    return NULL;
  }
  printk(KERN_INFO "File opened Successfully");
  return filp;
}
//----------------------------------------------------------------------------------------------------//

//----------------------------------------FUNCTION TO CLOSE FILE--------------------------------------//
//Generic function to close a file
/*I/P:File Descriptor of the file opened*/
void file_close(struct file* filp){
  filp_close(filp,NULL);
}

/*****************************HASH FUNCTION FOR FINDING PATHNAME***********************************/
/*
Computes the hash based on the Customer and VM Id to obtain index
into the array of cached Cust and VM id's or to lookup into the file
I/P:-Customer Id and VM Id
O/P:-Hash Value                  
*/
u_int32_t dir_hash(u_int32_t cust_id,vm_id *vmid)
{
	u_int32_t hash=0;
	u_int8_t i;
	hash=cust_id;
	for(i=0;i<4;i++)
		hash+=vmid->v_id[i];
       	return hash%256;
}

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

/***************************************************************************************************/
void get_path_cache_entry(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,u_int32_t map_location,maprecord *path_cache)
{
	int ret;
	loff_t offset=0;
	mm_segment_t oldfs;
	struct file *filp;
//	maprecord *map=kmalloc(sizeof(maprecord),GFP_KERNEL);
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
		//	memcpy(&path_cache[map_location],map,sizeof(maprecord));
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
//	kfree(map);
	file_close(filp);
}

/**************************FUNCTION to ALLOCATE SPACE for FLOW RECORD*****************************/
void create_flow_space(maprecord *path_cache,u_int32_t dirhash)
{
	path_cache[dirhash].flow_start=vmalloc(100 * sizeof(flow_record));
	memset(path_cache[dirhash].flow_start,0,(100*sizeof(flow_record)));
	path_cache[dirhash].off_table=vmalloc(100*sizeof(offset_table));
	memset(path_cache[dirhash].off_table,0,(100*sizeof(offset_table)));	
//	path_cache[dirhash].bit_map=kmalloc(sizeof(bitmap),GFP_KERNEL);	
//	path_cache[dirhash].bit_map->src_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
//	path_cache[dirhash].bit_map->dst_ip=kmalloc(256 * sizeof(ip_bits),GFP_KERNEL);
//	path_cache[dirhash].bit_map->src_port=vmalloc(65536*sizeof(port_bits));
//	path_cache[dirhash].bit_map->dst_port=vmalloc(65536*sizeof(port_bits));	
//	path_cache[dirhash].bit_map->protocol=kmalloc(256*sizeof(protocol_bits),GFP_KERNEL);
}
/*void set_bit_no(char *arr,u_int32_t row_no)
{
	char tmp=1;
	tmp=tmp<<row_no%8;	
	arr[row_no/8]|=tmp;
}
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

/***************************FUNCTION TO CREATE NEW FLOW RECORD************************/
/*
Function to create Flow record from Pfring Pktheader
I/P:PF_Ring pkthdr 
O/P:Flow Record created
*/
void create_flow_record(flow_record *fr,struct pfring_pkthdr *pkf,u_int32_t dirhash,u_int32_t flowhash,maprecord *path_cache)
{
	      fr->flow_id=path_cache[dirhash].GFL + flowhash;
              fr->nop=1;
              fr->src_port=pkf->extended_hdr.parsed_pkt.l4_src_port;
              fr->dst_port=pkf->extended_hdr.parsed_pkt.l4_dst_port;
              fr->ip_src=pkf->extended_hdr.parsed_pkt.ip_src;
              fr->ip_dst=pkf->extended_hdr.parsed_pkt.ip_dst;
              fr->protocol=pkf->extended_hdr.parsed_pkt.l3_proto;
	      //fr->offset_table[fr->nop]=path_cache[dirhash].pkt_offset;
	      //fr->timestamp_ns=
	      path_cache[dirhash].off_table[flowhash].start=path_cache[dirhash].off_table[flowhash].end=get_offset_node(path_cache[dirhash].pkt_offset);	
	      fr->nxtfr=0;
              //Updation of Offset Field in path_cache
	      fr->start_pkt_no=path_cache[dirhash].GPC;
	      path_cache[dirhash].GPC++;	
              path_cache[dirhash].pkt_offset+=sizeof(hs_pkt_hdr);
	      //make_bitmap_entry(fr,path_cache[dirhash].bit_map,flowhash);	      
	      //To Verify --> Printing of Fields
              printk("\nSrc IP:%d.%d.%d.%d\n",(*(int*)&fr->ip_src) & 0xFF,
                                              (*(int*)&fr->ip_src >> 8 ) & 0xFF,
                                              (*(int*)&fr->ip_src >> 16) & 0xFF,
		                              (*(int*)&fr->ip_src >> 24) & 0xFF);
 }


void write_offset_table(maprecord *path_cache,u_int32_t dirhash)
{
	char *off_path=kmalloc(30,GFP_KERNEL);
	char *off_name=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i,ret;
	strcpy(off_path,path_cache[dirhash].path);
	strcat(off_path,"FlowRecords/");
	sprintf(off_name,"%d_%d_off",path_cache[dirhash].GFL,path_cache[dirhash].GFL + (100-1));
	strcat(off_path,off_name);
	filp=file_open(off_path,O_WRONLY|O_APPEND|O_CREAT,0664);
	//Fixing the Address Space Issue
	printk("\nNo problem in Opening offset file");
	oldfs=get_fs();
	set_fs(get_ds());

	for(i=0;i<100;i++)
	{
		offset_node *start=path_cache[dirhash].off_table[i].start;
		while(start)
		{
			vfs_write(filp,&start->offset,sizeof(loff_t),&offset);//Writing Flow Records to File
			start=start->next;
		}			
	}

	printk("\nNo Problem in Writing");
	set_fs(oldfs);
	file_close(filp);
	//Clean Up and Updation Tasks	
	kfree(off_name);
	kfree(off_path);	
}

int write_flow_rec(maprecord *path_cache,u_int32_t dirhash)
{
	char *frpath=kmalloc(30,GFP_KERNEL);
	char *fname=kmalloc(20,GFP_KERNEL);
	struct file *filp;
	mm_segment_t oldfs;
	loff_t offset=0;
	int i,ret;
	strcpy(frpath,path_cache[dirhash].path);
	strcat(frpath,"FlowRecords/");
	sprintf(fname,"%d_%d",path_cache[dirhash].GFL,path_cache[dirhash].GFL + (100-1));
	strcat(frpath,fname);
	filp=file_open(frpath,O_WRONLY|O_APPEND|O_CREAT,0664);
	//Fixing the Address Space Issue
	printk("\nNo problem in Opening File");
	oldfs=get_fs();
	set_fs(get_ds());

	for(i=0;i<100;i++)
	{
		path_cache[dirhash].flow_start[i].flow_id=path_cache[dirhash].GFL+i;
		ret=vfs_write(filp,&path_cache[dirhash].flow_start[i],sizeof(flow_record),&offset);//Writing Flow Records to File
	}
	printk("\nNo Problem in Writing");
	set_fs(oldfs);
	file_close(filp);
	//Clean Up and Updation Tasks	
	kfree(fname);
	kfree(frpath);
	return ret;	
}

int update_path_cache(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,maprecord *path_cache)
{
	u_int32_t temp;
	while(1)
	{
		if(path_cache[dirhash].cust_id==0)
		{
			//printk("cust_id==0 cust_id =%u",path_cache[dirhash].cust_id);
			get_path_cache_entry(cust_id,vmid,dirhash,dirhash,path_cache);
			create_flow_space(path_cache,dirhash);
			//printk("path_cache.custid=%u\n",path_cache[dirhash].cust_id);
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

/**************************FUNCTION TO UPDATE FLOW RECORD*******************************************/

void update_flow_rec(u_int32_t cust_id,vm_id *vmid,u_int32_t dirhash,maprecord *path_cache,struct pfring_pkthdr *pkthdr)
{
	u_int32_t flowhash,org_flowhash;
	//offset_node *start;

	dirhash=update_path_cache(cust_id,vmid,dirhash,path_cache);

	org_flowhash=flowhash=calc_hash(pkthdr);                  //Input is the pfring_pkthdr which has all the five fields needed to compute hash
    	flowhash%=100;
	printk("\nFlow hash is :%u",flowhash);
	if(path_cache[dirhash].flow_start[flowhash].nop==0)
	{
		create_flow_record(&path_cache[dirhash].flow_start[flowhash],pkthdr,dirhash,flowhash,path_cache);
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
				  //path_cache[dirhash].flow_start[flowhash].offset_table[path_cache[dirhash].flow_start[flowhash].nop]= path_cache[dirhash].pkt_offset;
				  path_cache[dirhash].off_table[flowhash].end->next=get_offset_node(path_cache[dirhash].pkt_offset);
				  path_cache[dirhash].off_table[flowhash].end=path_cache[dirhash].off_table[flowhash].end->next;
				  path_cache[dirhash].GPC++;	
				  path_cache[dirhash].pkt_offset+=sizeof(hs_pkt_hdr);
				  break;			  
			 }
			else
			{
				if(flowhash=path_cache[dirhash].flow_start[flowhash].nxtfr);
				else
				{		
					int i;		
					for(i=flowhash+1;i!=flowhash;i=(i+1)%100)
					{
						if(path_cache[dirhash].flow_start[i].nop==0)
							break;
					}
					if(i!=flowhash)
					{
//						create_flow_record(&path_cache[dirhash].flow_start[i],pkthdr,dirhash,i,path_cache,bit_map);
		 			        path_cache[dirhash].flow_start[flowhash].nxtfr=i;
						break;
					}
					else
					{
						//flush the flow records to disk
						write_flow_rec(path_cache,dirhash);
						write_offset_table(path_cache,dirhash);
						memset(path_cache[dirhash].flow_start,0,(100*sizeof(flow_record)));
						path_cache[dirhash].GFL+=100;
						flowhash=org_flowhash;			
					}	
				}
				
			}		
		}
	}
	//start=path_cache[36].off_table[flowhash].start;
	/*while(start)
	{
		printk("\noffset:%llu",start->offset);
		start=start->next;
	}*/
}

int write_packet(struct pfring_pkthdr *pkthdr,char *buf,maprecord *path_cache,u_int32_t dirhash)
{
	char *pktpath=kmalloc(30,GFP_KERNEL);
	struct file *filp;
	u_int32_t ret;
	loff_t offset=10;
	mm_segment_t oldfs;
	hs_pkt_hdr *hspkt=kmalloc(sizeof(hs_pkt_hdr),GFP_KERNEL);
	hspkt->pkt_id=1;
	hspkt->pkf=*pkthdr;
	strcpy(hspkt->buffer,buf);
	hspkt->buffer_len=strlen(buf);

	strcpy(pktpath,path_cache[dirhash].path);
	strcat(pktpath,"Packets/");
	strcat(pktpath,"pkttrace");
	printk(KERN_INFO "Packet Storage Path is:%s",pktpath);


	//Dumping this packet to disk in Packet Trace File
	filp=file_open(pktpath,O_WRONLY|O_APPEND|O_CREAT,0777);
	//Fixing the Address Space Issue
	printk("\nNo problem in Opening File");
	oldfs=get_fs();
	set_fs(get_ds());
	ret=vfs_write(filp,hspkt,sizeof(hs_pkt_hdr),&offset);//Replace vfs_write with file operations write function
	printk("\nNo Problem in Writing");
	set_fs(oldfs);
	file_close(filp);
	//--------------------------------------------------------------------------------------//
	//Freeing the memory allocated
	kfree(pktpath);
	kfree(hspkt);
	return ret;
}


void free_path_cache(maprecord *path_cache)
{
	int i;
	for(i=0;i<256;i++)
	{
		if(path_cache[i].flow_start)
		{		
			int j;				
			vfree(path_cache[i].flow_start);
			for(j=0;j<100;j++)
			{
				offset_node *start=path_cache[i].off_table[j].start;
				while(start)
				{
					offset_node *tmp=start->next;
					kfree(start);
					start=tmp;
				}
			}
			vfree(path_cache[i].off_table);
		/*	kfree(path_cache->bit_map->src_ip);
			kfree(path_cache->bit_map->dst_ip);
			vfree(path_cache->bit_map->src_port);
			vfree(path_cache->bit_map->dst_port);
			kfree(path_cache->bit_map->protocol);*/
		}
	}
	kfree(path_cache);
}



/**********************FUNCTION TO HANDLE PACKET AFTER CAPTURE PROCESS******************************/

void handle_storage_pkt(u_int32_t cust_id,vm_id *vmid,struct pfring_pkthdr *pkthdr,char *buf,maprecord *path_cache)
{
	u_int32_t dirhash;
	dirhash=dir_hash(cust_id,vmid);		
	printk("Now Hash is:%u",dirhash);
	update_flow_rec(cust_id,vmid,dirhash,path_cache,pkthdr);	
	write_packet(pkthdr,buf,path_cache,dirhash);	
}


int capture_thread(void *arg)
{
	struct pfring_pkthdr pkthdr;
	u_int32_t cust_id=1234;//Temporary
	vm_id *vmid=kmalloc(sizeof(vm_id),GFP_KERNEL);
	maprecord *path_cache=kmalloc(256*sizeof(maprecord),GFP_KERNEL);//Cache Block to hold Customer-Path Records
	unsigned char *buf=(unsigned char *)kmalloc(BUFSIZE,GFP_KERNEL);
	int len=0,i=0;
	int ret = sock_create(DOMAIN, TYPE , PROTOCOL , &capture_sockfd);
	memset(path_cache,0,(256*sizeof(maprecord)));	
	//Temprory to run the code
	vmid->v_id[0]=1234;vmid->v_id[1]=2345;vmid->v_id[2]=3456;vmid->v_id[3]=4567;
	if (ret <0)
	{
		printk("socket creation failed\n");
		return -1;
	}
	memset(&pkthdr, 0, sizeof(struct pfring_pkthdr));
	printk("capture_sockfd = 0x%p\n", capture_sockfd);
        lookupcreate();
	printk(KERN_INFO "Lookup Table Created");
	for(i=0;i<3;i++)
	{
		len=pack_recv(capture_sockfd, buf, BUFSIZE, 0);
		if(len>0)
		{
			parse_pkt(buf, &pkthdr);
			looper(&pkthdr, buf);
			print_ethernet_header(&pkthdr);
			handle_storage_pkt(cust_id,vmid,&pkthdr,buf,path_cache);			
			//dirhash=dirpath(cust_id,vm_id,path_cache);	         	//Call to return directory path of incoming Packet
			//printk(KERN_INFO "Directory Path is %s",path_cache[dirhash].path);
			//dump_pkt(path_cache,buf,&pkthdr,dirhash);	         	//Use Returned Directory Path To Route the Packets
			//break;
		}
	}
	kfree(buf);
        kfree(vmid);
	free_path_cache(path_cache);	
	return 0;
}

static int af_capture_init(void)
{
	printk(KERN_ALERT "\nInserted module: %s\nauthor: %s\ndesc: %s\n",AFCAPTURE_NAME,AFCAPTURE_AUTHOR,AFCAPTURE_DESC);
	kernel_thread(capture_thread, NULL, 0);
	return 0;
}

static void af_capture_exit(void)
{
	capture_sockfd->ops->release(capture_sockfd);
	sock_release(capture_sockfd);
	printk(KERN_ALERT "\nRemoving module: %s\n",AFCAPTURE_NAME);
}

module_init(af_capture_init);
module_exit(af_capture_exit);
