#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include<linux/tcp.h>
#include "ret_query.h"

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


hs_pkt_hdr* get_hs_pkt(struct pfring_pkthdr pkf,unsigned char *buf)
{
	hs_pkt_hdr *packet=(hs_pkt_hdr*)malloc(sizeof(hs_pkt_hdr));
	//Code to assign pfring header and skbuff	
	packet->pkf=pkf;
	packet->buffer=(unsigned char*)malloc(pkf.caplen);
	strcpy(packet->buffer,buf);
	return packet;
} 

int get_file_index(char *filename)
{
	int i,j=0,index;
	char flow_file_rev[20],flow_file[20];
	for(i=strlen(filename)-1;filename[i]!='_';i--)
	{
		flow_file_rev[j++]=filename[i];
	}
	flow_file_rev[j]='\0';
	j=0;
	for(i=strlen(flow_file_rev)-1;i>=0;i--)
		flow_file[j++]=flow_file_rev[i];
	flow_file[j]='\0';
	return atoi(flow_file);
}
unsigned char* get_final_bitmap(display_rule *dr,char *filename,char *path)
{
	int i,j;
	bitmap_start_offsets head;
	char fname[140];//(unsigned char*)malloc(140*sizeof(unsigned char));
	unsigned char *bitmap1=(unsigned char *)malloc(sizeof(unsigned char )*MAX_FLOW_BYTE);
	unsigned char *bitmap2=(unsigned char *)malloc(sizeof(unsigned char )*MAX_FLOW_BYTE);
	unsigned char *res=0;
	loff_t offsets[256];
	loff_t *offsets_port=(loff_t*)malloc(65536*sizeof(loff_t));
		
	printf("Bitmap Files %s\n",filename);
	sprintf(fname,"%sBitMaps/%s",path,filename);
	int fd = open(fname,O_RDONLY,0);

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
			res=and_bitmaps(bitmap1,bitmap2,MAX_FLOW_REC);
			for(j=0;j<MAX_FLOW_BYTE;j++)
				bitmap2[j]=res[j];
		}
	}

	//Reading Source Port
	if(dr->src_port >= 0)
	{
		lseek(fd,head.src_port_octet,0);
		read(fd,offsets_port,65536*sizeof(loff_t));
		lseek(fd,offsets_port[dr->src_port],0);
		if(dr->src_port < 65535)
			read(fd,bitmap1,offsets_port[dr->src_port + 1]-offsets_port[dr->src_port]);
		else
			read(fd,bitmap1,head.dst_port_octet - offsets_port[dr->src_port]);			

		res=and_bitmaps(bitmap1,bitmap2,MAX_FLOW_REC);
		for(j=0;j<MAX_FLOW_BYTE;j++)
			bitmap2[j]=res[j];
	}
	

	//Reading Dst Port
	if(dr->dst_port >= 0)
	{
		lseek(fd,head.dst_port_octet,0);
		read(fd,offsets_port,65536*sizeof(loff_t));
		lseek(fd,offsets_port[dr->dst_port],0);
		
		if(dr->dst_port < 65535)
			read(fd,bitmap1,offsets_port[dr->dst_port + 1]-offsets_port[dr->dst_port]);
		else
			read(fd,bitmap1,head.protocol_octet - offsets_port[dr->dst_port]);

		res=and_bitmaps(bitmap1,bitmap2,MAX_FLOW_REC);
		for(j=0;j<MAX_FLOW_BYTE;j++)
			bitmap2[j]=res[j];
	}

	//Reading Protocol
	if(dr->protocol >= 0)
	{
		lseek(fd,head.protocol_octet,0);
		read(fd,offsets,256*sizeof(loff_t));
		lseek(fd,offsets[dr->protocol],0);
		if(dr->protocol < 255)
			read(fd,bitmap1,offsets[dr->protocol + 1] - offsets[dr->protocol]);
		else
			read(fd,bitmap1,13);

		res=and_bitmaps(bitmap1,bitmap2,MAX_FLOW_REC);
		for(j=0;j<MAX_FLOW_BYTE;j++)
			bitmap2[j]=res[j];

	}

	return bitmap2;
}


void traverse_flow_list(flow_rec_nos *flow_list,char *path,int file_index)
{
	int index,fd_fr,fd_off,fd_pkt,i,cnt=0;
	unsigned char *buffer;
	char file_fr[100],file_off[100],command[100],f_path[100];
	loff_t offset;
	struct pfring_pkthdr pkf;
	hs_pkt_hdr *packet_start=0,*end=0;

	if(!flow_list)
			return;

	strcpy(f_path,path);
	strcat(f_path,"Flowrecords/");

	sprintf(command,"%s%s%s%d%s","lzop -dU ",f_path,"flowrec_",file_index,".lzo");
	//system(command);
	sprintf(file_fr,"%s%s%d",f_path,"flowrec_",file_index);
	sprintf(file_off,"%s%s%d",f_path,"offset_",file_index);
	fd_fr=open(file_fr,O_RDONLY,0);
	fd_off=open(file_off,O_RDONLY,0);

	//To reach required flow record and read it
	while(flow_list!=NULL)
	{
		char pkt_trace_file[80];
		lseek(fd_fr,(flow_list->no-1) * sizeof(flow_record),0);
		read(fd_fr,&flow_list->fr,sizeof(flow_record));
		
		//Seek To Flow Record's Start Packet Number in the Offset File
		if(flow_list->fr.pkt_file_no!=-1)
		{
			lseek(fd_off,(flow_list->fr.start_pkt_no) * sizeof(loff_t),0);
			if(cnt==0)		
			{
				sprintf(pkt_trace_file,"%s%s%d",path,"Packets/pkttrace_",flow_list->fr.pkt_file_no);
				fd_pkt=open(pkt_trace_file,O_RDONLY,0);
				cnt=1;
			}
			//Read the offset NOP times and fetch corresponding Packet from Packet Trace File
			for(i=0;i<flow_list->fr.nop;i++)
			{
				read(fd_off,(loff_t*)&offset,sizeof(loff_t));
				lseek(fd_pkt,offset,0);
				read(fd_pkt,&pkf,sizeof(struct pfring_pkthdr));
				buffer=(unsigned char*)malloc(pkf.caplen);
				read(fd_pkt,buffer,pkf.caplen);
				if(pkf.extended_hdr.parsed_pkt.l3_proto==6)
				{
					char* buff=(buffer+pkf.extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct tcphdr));
					//printf("l4_offset is %u",pkf.extended_hdr.parsed_pkt.offset.l4_offset);
					//printf("\nTCP packets::%s",buff);
				}
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
		}
	        //To Link to Packets to corresponding Flow Record
		flow_list->pkt_list=packet_start;
		flow_list=flow_list->next;
		packet_start=0;
	}

	close(fd_fr);
	close(fd_off);
	close(fd_pkt);		
	sprintf(command,"%s%s%s%d","lzop -U ",f_path,"flowrec_",file_index);
	//system(command);

}


comp_flow_list* get_flow_packets(display_rule *dr,char *path,capture_time *time_cap)
{
	file_list* file_names;
	comp_flow_list *start_flow=0,*end_flow=0;
	file_names=get_filename(time_cap,path);
	while(file_names)
	{
		int file_index=get_file_index(file_names->filename);
		flow_rec_nos* t=decode_bitmap(get_final_bitmap(dr,file_names->filename,path),MAX_FLOW_REC);
		traverse_flow_list(t,path,file_index);
		printf("\nFilename is:%s",file_names->filename);
		if(start_flow==0)
			start_flow=end_flow=get_comp_flow_list_node(t);
		else
	{
			end_flow->down=get_comp_flow_list_node(t);
			end_flow=end_flow->down;
		}
		file_names=file_names->next;
	}
	return start_flow;
}


void update_packet_count(struct pfring_pkthdr pkf,u_int64_t *arr)
{
  struct tm *time_val;
  time_t tmk;
  tmk=pkf.ts.tv_sec;
  time_val=localtime(&tmk);
  //printf("\n Time is %d:%d:%d",time_val->tm_mday,time_val->tm_hour,time_val->tm_min);
  arr[time_val->tm_hour]+=1;
}

void set_rule(display_rule *dr,int val)
{
	int i;
	for(i=0;i<4;i++)
	{
		dr->sip_oct[i]=val;
		dr->dip_oct[i]=val;
	}
	dr->src_port=val;
	dr->dst_port=val;
	dr->protocol=val;
}


u_int64_t* get_hourly_packets(char *path,capture_time *time_cap)
{
	int i,tot_pkt=0;
	display_rule dr1;
  	u_int64_t *cap_size=(u_int64_t*)calloc(24,sizeof(u_int64_t));

	
	set_rule(&dr1,-1);	
	time_cap->month=2;
	time_cap->day=26;
	time_cap->start_hr=0;
	time_cap->start_min=0;
	time_cap->end_hr=23;
	time_cap->end_min=59;

	comp_flow_list* flow_list=get_flow_packets(&dr1,path,time_cap);
	while(flow_list)
	{
		flow_rec_nos *frec=flow_list->frec;
		while(frec)
		{
			//use frec->fr to get flow_rec structure
			hs_pkt_hdr *hs_hdr=frec->pkt_list;
			while(hs_hdr)
			{
				//use hs_hdr to get packet details 
				update_packet_count(hs_hdr->pkf,cap_size);
				hs_hdr=hs_hdr->next;
				//break;			
			}			
			frec=frec->next;
		}
		flow_list=flow_list->down;
	}
	printf("\n");
	for(i=0;i<24;i++)
		printf("Hour %d == %llu Bytes\n",i,cap_size[i]); 
	return cap_size;
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



int main()
{
	capture_time ct;
	display_rule dr;
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

	ct.month=2;
	ct.day=26;
	ct.start_hr=0,ct.start_min=0;
	ct.end_hr=23,ct.end_min=59;

	//comp_flow_list *flow=get_flow_packets();
	
//	fill_flow_packets(&dr,"/media/STORAGE/storage/hs1234/",&ct);
	get_hourly_packets("/media/STORAGE/storage/hs1234/",&ct);
	return 0;

}


