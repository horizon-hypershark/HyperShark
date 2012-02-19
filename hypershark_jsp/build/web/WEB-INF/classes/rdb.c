#define ETH_ALEN 6
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<linux/pf_ring.h>
#include<fcntl.h>
#include "pf_kernel.h"

//STRUCTURES
typedef struct byte_list
{
	u_int8_t value;
	struct byte_list *next;
}byte_list;

typedef struct short_list
{
	u_int16_t value;
	struct short_list *next;
}short_list;

typedef struct display_rule
{
	int sip_oct[4];
	int dip_oct[4];
	int src_port;
	int dst_port;
	int protocol;
}display_rule;


typedef struct flow_rec_nos
{
	int no;
	flow_record fr; 
	hs_pkt_hdr *link;
	struct flow_rec_nos *next;
}flow_rec_nos;

typedef struct comp_flow_list
{
	flow_rec_nos *frec;
	struct comp_flow_list *down;		
}comp_flow_list;



typedef struct bit_map_info
{
	int remain;
	int curr_byte;
}bit_map_info;

//------------------------------------------------------------------------------------------------------//

flow_rec_nos* get_flow_nos_node(int no)
{
	flow_rec_nos *temp=(flow_rec_nos*)malloc(sizeof(flow_rec_nos));
	temp->no=no;
	temp->next=0;
	return temp;
}

comp_flow_list* get_comp_flow_list_node(flow_rec_nos *frn)
{
	comp_flow_list *t=(comp_flow_list*)malloc(sizeof(comp_flow_list));
	t->frec=frn;
	t->down=0;
	return t;	
}

void set_info_zero(bit_map_info *info)
{
	info->remain=0;
	info->curr_byte=0;
}

void and_word(unsigned char *map1,unsigned char *map2,unsigned char *result,int byte1,int byte2,int res)
{
	result[res] = map1[byte1] & map2[byte2];
	result[res+1] = map1[byte1+1] & map2[byte2+1];
	result[res+2] = map1[byte1+2] & map2[byte2+2];
	result[res+3] = map1[byte1+3] & map2[byte2+3];
}

void set_word(unsigned char *map,int byte1,int value)
{
	map[byte1]=map[byte1+1]=map[byte1+2]=map[byte1+3]=value;
}


void set_min_words(bit_map_info *info1,bit_map_info *info2,unsigned char *result,bit_map_info *res_info,int no_zero,int diff)
{
	info1->curr_byte+=4;
	info2->curr_byte+=4;
	info2->remain=diff;
	if((res_info->curr_byte-4>=0)&&(result[res_info->curr_byte-4]==0))
		result[res_info->curr_byte-1]+=no_zero;
	else
	{
		set_word(result,res_info->curr_byte,0);
		result[res_info->curr_byte+3]=no_zero;
		res_info->curr_byte+=4; 	
	}
}

int handle_map_remain(unsigned char *map,bit_map_info *info,unsigned char *result,bit_map_info *res_info,bit_map_info *info2)
{
	if(map[info->curr_byte]&0x01)
	{
		info->remain--;		
		info->curr_byte+=4;		
	}
	else
	{
		int no_zero=map[info->curr_byte+3];
		if(no_zero<=info->remain)
		{		
			info->remain-=no_zero;
			info->curr_byte+=4;			
		}
		else
		{
			int diff=no_zero-info->remain;
			info->remain=0;
			info->curr_byte+=4;
			info2->remain=diff;			
			if((res_info->curr_byte-4>=0)&&(result[res_info->curr_byte-4]==0))
				result[res_info->curr_byte-1]+=diff;
			else
			{
				set_word(result,res_info->curr_byte,0);
				result[res_info->curr_byte+3]=diff;
				res_info->curr_byte+=4; 	
			}
			return diff;
		}
	}
	return 0;
}

unsigned char* and_bitmaps(unsigned char *map1,unsigned char *map2,int max_bits)//breaking condition not written
{
	int tot_words=max_bits/31,res_words=0,i;
	//unsigned char *result=(unsigned char *)malloc(max_bits/8+(max_bits/31)/8+2+3);//3 bytes extra
	unsigned char *result=(unsigned char *)malloc(MAX_FLOW_BYTE*sizeof(unsigned char));
	bit_map_info info1,info2,res_info;
	int extra_bytes=((max_bits%31)/8)+1;
	set_info_zero(&info1);	
	set_info_zero(&info2);
	set_info_zero(&res_info);
	while(1)
	{
		if(info1.remain==0 && info2.remain==0)
		{
			if((map1[info1.curr_byte]&0x01) && (map2[info2.curr_byte]&0x01))//check for zeroth bit
			{
				and_word(map1,map2,result,info1.curr_byte,info2.curr_byte,res_info.curr_byte);
				res_info.curr_byte+=4;
				info1.curr_byte+=4;
				info2.curr_byte+=4;
				res_words+=1;
			}
			else if(!((map1[info1.curr_byte]&0x01) || (map2[info2.curr_byte]&0x01)))
			{				
				int no_zero1=map1[info1.curr_byte+3],no_zero2=map2[info2.curr_byte+3];				
				if(no_zero1>no_zero2)
				{
					set_min_words(&info1,&info2,result,&res_info,no_zero1,no_zero1-no_zero2);
					res_words+=no_zero1;
				}
				else
				{
					set_min_words(&info2,&info1,result,&res_info,no_zero2,no_zero2-no_zero1);
					res_words+=no_zero2;								
				}
			}
			else
			{
				int no_zero=0;
				if(map2[info2.curr_byte]&0x01)
				{
					no_zero=map1[info1.curr_byte+3];
					if((res_info.curr_byte-4>=0)&&(result[res_info.curr_byte-4]==0))
						result[res_info.curr_byte-1]+=no_zero;
					else
					{
						set_word(result,res_info.curr_byte,0);
						result[res_info.curr_byte+3]=no_zero;
						res_info.curr_byte+=4; 	
					}									
					info1.curr_byte+=4;
					info2.curr_byte+=4;
					info2.remain=no_zero-1;
				}
				else
				{
					no_zero=map2[info2.curr_byte+3];	
					if((res_info.curr_byte-4>=0)&&(result[res_info.curr_byte-4]==0))
						result[res_info.curr_byte-1]+=no_zero;
					else
					{
						set_word(result,res_info.curr_byte,0);
						result[res_info.curr_byte+3]=no_zero;
						res_info.curr_byte+=4; 	
					}										
					info1.curr_byte+=4;
					info2.curr_byte+=4;
					info1.remain=no_zero-1;
				}	
				res_words+=no_zero;			
			}
		}
		else if(info1.remain)
		{
			res_words+=handle_map_remain(map1,&info1,result,&res_info,&info2);
		}
		else
		{
			res_words+=handle_map_remain(map2,&info2,result,&res_info,&info1);
		}
		if(res_words==tot_words)
			break;		
	}
//TODO last 31
	for(i=0;i<extra_bytes;i++)
	{		
		result[res_info.curr_byte+i] = map1[info1.curr_byte+i] & map2[info2.curr_byte+i];
	}
	return result;
}



flow_rec_nos* decode_bitmap(unsigned char *map,int max_bits)
{
	int tot_words=max_bits/31,curr_words=0,curr_byte=0,i,j;
	flow_rec_nos *flow_list=0,*end=0;
	int extra_bits=max_bits%31,tmp2,start_flow=0;
	start_flow=(curr_words*31)+1;
	while(1)
	{
		if(map[curr_byte]&0x01)
		{					
			for(i=0;i<4;i++)
			{
				unsigned char tmp=map[curr_byte+i];tmp2=0x01;
				if(i==0)
				{
					tmp&=0xFE;
					start_flow--;
				}
				for(j=0;j<8;j++)
				{
					if(tmp & tmp2)
					{
						if(flow_list==0)
						{
							flow_list=end=get_flow_nos_node(start_flow);
						}
						else
						{
							end->next=get_flow_nos_node(start_flow);
							end=end->next;
						}
					}
					start_flow++;
					tmp2<<=1;	
				}
			}
			curr_words++;			
		}
		else
		{
			curr_words+=map[curr_byte+3];			
		}
		curr_byte+=4;
		start_flow=(curr_words*31)+1;
		if(curr_words==tot_words)
			break;
	}
	tmp2=0x01;
	for(i=0;i<extra_bits;i++)
	{
		
		if((i/8!=0) && (i%8==0))
		{
			tmp2=0x01;
			curr_byte++;
		}
		if(map[curr_byte] & tmp2)
		{
			if(flow_list==0)
			{
				flow_list=end=get_flow_nos_node(start_flow);
			}
			else
			{
				end->next=get_flow_nos_node(start_flow);
				end=end->next;
			}													
		}	
		tmp2<<=1;
		start_flow++;			
	}
	return flow_list;
}


hs_pkt_hdr* get_hs_pkt(struct pfring_pkthdr pkf,unsigned char *buf)
{
	hs_pkt_hdr *packet=(hs_pkt_hdr*)malloc(sizeof(hs_pkt_hdr));

	//Code to assign pfring header and skbuff	
	packet->pkf=pkf;
	packet->buffer=(unsigned char*)malloc(pkf.caplen);
	strcpy(packet->buffer,buf);
	return packet;
} 


unsigned char* get_final_bitmap(display_rule *dr,char *filename,char *path)
{
	int i,j;
	bitmap_start_offsets head;
	unsigned char *bitmap1=(unsigned char *)malloc(sizeof(unsigned char )*MAX_FLOW_BYTE);
	unsigned char *bitmap2=(unsigned char *)malloc(sizeof(unsigned char )*MAX_FLOW_BYTE);
	unsigned char *res=0;
	loff_t offsets[256];
	loff_t *offsets_port=(loff_t *)malloc(65536*sizeof(loff_t));
	int fd = open("/storage/hs1234/BitMaps/bitmap_0",O_RDONLY,0);

	for(i=0;i<MAX_FLOW_BYTE;i++)
		bitmap2[i]=255;

	read(fd,&head,sizeof(bitmap_start_offsets));


	//Reading Source IP
	for(i=0;i<4;i++)
	{
		if(dr->sip_oct[i] >=0)
		{
			lseek(fd,head.src_ip_octet[i],0);
			read(fd,offsets,256*sizeof(loff_t));
			printf("\nENtered in %dth OCtet",i);
			printf("\nDR SRC IP %d:%d",i,dr->sip_oct[i]);
			lseek(fd,offsets[dr->sip_oct[i]],0);
			if(dr->sip_oct[i]<255)
				read(fd,bitmap1,offsets[dr->sip_oct[i]+1]-offsets[dr->sip_oct[i]]);
			else
			{
				if(i<3)
					read(fd,bitmap1,head.src_ip_octet[i+1]-offsets[dr->sip_oct[i]]);
				else
					read(fd,bitmap1,head.dst_ip_octet[0]-offsets[dr->sip_oct[i]]);
			}
			res=and_bitmaps(bitmap1,bitmap2,MAX_FLOW_REC);
			for(j=0;j<MAX_FLOW_BYTE;j++)
				bitmap2[j]=res[j];
		}
	}
	//Reading Destination IP
	for(i=0;i<4;i++)
	{
		if(dr->dip_oct[i] >= 0)
		{
			lseek(fd,head.dst_ip_octet[i],0);
			read(fd,offsets,256*sizeof(loff_t));
			printf("\nEntered in %dth OCtet",i);
			printf("\nDR DST IP %d:%d",i,dr->dip_oct[i]);
			lseek(fd,offsets[dr->dip_oct[i]],0);
			if(dr->dip_oct[i] < 255)
				read(fd,bitmap1,offsets[dr->dip_oct[i]+1]-offsets[dr->dip_oct[i]]);
			else
			{
				if(i<3)
					read(fd,bitmap1,head.dst_ip_octet[i+1]-offsets[dr->dip_oct[i]]);
				else
					read(fd,bitmap1,head.src_port_octet - offsets[dr->dip_oct[i]]);
			}
			
		}
	}
/*
	//Reading Source Port
	if(dr->src_port >= 0)
	{
		printf("\nENtered in %dth OCtet",i);
		printf("\nDR SRC PORT %d:%llu",i,dr->src_port);			
		lseek(fd,head.src_port_octet,0);
		read(fd,offsets_port,65536*sizeof(loff_t));
		if(dr->src_port < 65535)
			read(fd,bitmap1,offsets_port[dr->src_port + 1]-offsets_port[dr->src_port]);
		else
			read(fd,bitmap1,head.dst_port_octet - offsets_port[dr->src_port]);			
	}
	
	//Reading Destination Port
	if(dr->dst_port >= 0)
	{
		lseek(fd,head.dst_port_octet,0);
		read(fd,offsets_port,65536*sizeof(loff_t));
		if(dr->dst_port < 65535)
			read(fd,bitmap1,offsets_port[dr->dst_port + 1]- offsets_port[dr->dst_port]);
		else
			read(fd,bitmap1,head.protocol_octet - offsets_port[dr->dst_port]);
	}
	
	//Reading Protocol
	if(dr->protocol >= 0)
	{
		lseek(fd,head.protocol_octet,0);
		read(fd,offsets,256*sizeof(loff_t));
		if(dr->protocol < 255)
			read(fd,bitmap1,offsets[dr->protocol + 1] - offsets[dr->protocol]);
		else
			read(fd,bitmap1,13);
	}*/
	
	printf("Bitmap is::");
	for(i=0;i<13;i++)
		printf("%d  ",bitmap2[i]);

	printf("\nread start offsets");
	printf("\nPrinting of Offsets :");
	printf("\nSRC IP OFFSETS :");
	for(i=0;i<4;i++)
		printf("\nOctet %d = %llu",i,head.src_ip_octet[i]);	
	printf("\nDST IP OFFSETS :");
	for(i=0;i<4;i++)
		printf("\nOctet %d = %llu",i,head.dst_ip_octet[i]);	
	printf("\nSRC PORT OFFSET :");
		printf("\nOctet %llu",head.src_port_octet);	
	printf("\nDST PORT OFFSET :");
		printf("\nOctet %llu",head.dst_port_octet);	
	printf("\nPROTOCOL OFFSET :");
		printf("\nOctet %llu\n",head.protocol_octet);	
		
	return bitmap2;
}



void traverse_flow_list(flow_rec_nos *flow_list,char *path)
{
	int index,fd_fr,fd_off,fd_pkt,i;
	unsigned char *buffer;
	char file_fr[100],file_off[100],command[100],f_path[100];
	loff_t offset;
	struct pfring_pkthdr pkf;
	hs_pkt_hdr *packet_start=0,*end=0;

	if(!flow_list)
			return;

	strcpy(f_path,path);
	strcat(f_path,"Flowrecords/");
	fd_pkt=open("/storage/hs1234/Packets/pkttrace",O_RDONLY,0);//Path is Problem here

	index=(flow_list->no)/100;
	
	sprintf(command,"%s%s%s%d%s","lzop -dU ",f_path,"flowrec_",index,".lzo");
	system(command);
	sprintf(file_fr,"%s%s%d",f_path,"flowrec_",index);
	sprintf(file_off,"%s%s%d",f_path,"offset_",index);
	fd_fr=open(file_fr,O_RDONLY,0);
	fd_off=open(file_off,O_RDONLY,0);

	//To reach required flow record and read it
	while(flow_list!=NULL)
	{
		lseek(fd_fr,(flow_list->no-1) * sizeof(flow_record),0);
		printf("\nFLow no :%d",flow_list->no);
		read(fd_fr,&flow_list->fr,sizeof(flow_record));

		printf("  FLOW_SRC_IP=%u.%u.%u.%u",(flow_list->fr.ip_dst.v4 >> 24) & 0xFF,
								   (flow_list->fr.ip_src.v4 >> 16) & 0xFF,
								   (flow_list->fr.ip_src.v4 >> 8) & 0xFF,
								   (flow_list->fr.ip_src.v4) & 0xFF);
		printf("  Port is : %u",flow_list->fr.src_port);
		//Seek To Flow Record's Start Packet Number in the Offset File
		lseek(fd_off,(flow_list->fr.start_pkt_no) * sizeof(loff_t),0);

		//Read the offset NOP times and fetch corresponding Packet from Packet Trace File
		for(i=0;i<flow_list->fr.nop;i++)
		{
			read(fd_off,(loff_t*)&offset,sizeof(loff_t));
			lseek(fd_pkt,offset,0);
			read(fd_pkt,&pkf,sizeof(struct pfring_pkthdr));
			buffer=(unsigned char*)malloc(pkf.caplen);
			read(fd_pkt,buffer,pkf.caplen);
			//Creating Link List of Read Packets
			if(packet_start==0)
			{
				packet_start=end=get_hs_pkt(pkf,buffer);
			}
			else
			{
				end->next=get_hs_pkt(pkf,buffer);
				end=end->next;
			}
			free(buffer);
		}
	        //To Link to Packets to corresponding Flow Record
		flow_list->link=packet_start;
		flow_list=flow_list->next;
	}

	close(file_fr);
	close(file_off);
	close(fd_pkt);		
	sprintf(command,"%s%s%s%d","lzop -U ",f_path,"flowrec_",index);
	system(command);
}


comp_flow_list* get_flow_packets(display_rule *dr,char *path,capture_time *time_cap)
{
	file_list* file_names;
	comp_flow_list *start_flow=0,*end_flow=0;
	file_names=get_filename(time_cap);
	while(file_names)
	{
		flow_rec_nos* t=decode_bitmap(get_final_bitmap(dr,file_names->filename,path),MAX_FLOW_REC);
		traverse_flow_list(t,path);
		if(start_flow==0)
			start_flow=end_flow=get_comp_flow_list_node(t);
		else
		{
			end_flow->down=get_comp_flow_list_node(t);
			end_flow=end_flow->down;
		}
		file_names=file_name->next;
	}
	return start_flow;
}

int main()
{
	display_rule dr;
	flow_record fr;
	unsigned char map1[15]={0},*map2,*result;
	int i=1,fd;
	flow_rec_nos *flow;
	
	dr.sip_oct[0]=-1;
	dr.sip_oct[1]=-1;
	dr.sip_oct[2]=-1;
	dr.sip_oct[3]=-1;
	dr.dip_oct[0]=-1;
	dr.dip_oct[1]=-1;
	dr.dip_oct[2]=-1;
	dr.dip_oct[3]=-1;
	dr.src_port=-1;
	dr.dst_port=-1;
	dr.protocol=-1;
	map1[0]=107;
	map1[1]=48;
	map1[2]=96;
	map1[3]=111;
	map1[4]=183;
	map1[5]=173;
	map1[6]=66;
	map1[7]=60;
	map1[8]=253;
	map1[9]=248;
	map1[10]=205;
	map1[11]=252;
	map1[12]=62;

	map2=get_final_bitmap(&dr,"bitmap_0","/storage/hs1234/BitMaps/");

	printf("Result is:");
	for(i=0;i<MAX_FLOW_BYTE;i++)
		printf("%d ",map2[i]);
	
	flow=decode_bitmap(map2,100);

	traverse_flow_list(flow,"/storage/hs1234/");

	printf("\n");
}
