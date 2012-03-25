#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include "ret_query.h"
#include <jni.h>
#include "FileAccess_GetPacket.h"
#include<linux/ip.h>
#include<linux/udp.h>
#include<linux/tcp.h>
#include<linux/icmp.h>
#include<linux/if_arp.h>
#include "FileAccess_datatransfer.h"


mapstr icmp_type_map[] = 
{
  { ICMP_ECHOREPLY,	"echo reply"},
  { ICMP_DEST_UNREACH, 	"destination unreachable" },
  { ICMP_SOURCE_QUENCH,    "source quench" },
  { ICMP_REDIRECT,        "redirect" },
  { ICMP_ECHO,	     "echo request" },
  { ICMP_TIME_EXCEEDED,    "time exceeded" },
  { ICMP_PARAMETERPROB,    "parameter problem" },
  { ICMP_TIMESTAMP,       "timestamp" },
  { ICMP_TIMESTAMPREPLY,  "timestamp reply" },
  { ICMP_INFO_REQUEST,     "information request" },
  { ICMP_INFO_REPLY,       "information reply" },
  { ICMP_ADDRESS,     "mask request" },
  { ICMP_ADDRESSREPLY,       "mask reply" },
  { 0, "" }
};


mapstr icmp_du_map[] = 
{
  { ICMP_NET_UNREACH,               "network unreachable" },
  { ICMP_HOST_UNREACH,              "host unreachable" },
  { ICMP_PROT_UNREACH,          "protocol unreachable" },
  { ICMP_PORT_UNREACH,              "port unreachable"},
  { ICMP_FRAG_NEEDED,              "fragmentation needed"},
  { ICMP_SR_FAILED,        "source route failed"},
  { ICMP_NET_UNKNOWN,        "network unknown"},
  { ICMP_HOST_UNKNOWN,       "host unknown"},
  { ICMP_HOST_ISOLATED,       "source isolated"},
  { ICMP_NET_ANO,          "network admin prohibited"},
  { ICMP_HOST_ANO,         "host admin prohibited"},
  { ICMP_NET_UNR_TOS,            "network unreachable for TOS"},
  { ICMP_HOST_UNR_TOS,           "host unreachable for TOS"},
  { ICMP_PREC_VIOLATION,      "host precedence violation"},
  { ICMP_PREC_CUTOFF,        "precendence cutoff"},
  { ICMP_PKT_FILTERED,	     "packet filtered"},		
  { 0, "" }
};

mapstr icmp_timeexceeded_map[] = 
{
  { ICMP_EXC_TTL,               "TTL equals 0" },
  { ICMP_EXC_FRAGTIME,        "TTL expired in reassembly"}
};

mapstr icmp_rd_map[] =
{
	{ ICMP_REDIR_NET,	"Redirect Net"},
	{ ICMP_REDIR_HOST,	"Redirect Host"},
   	{ ICMP_REDIR_NETTOS,	"Redirect Net for TOS"},
	{ ICMP_REDIR_HOSTTOS,	"Redirect Host for TOS"},
};

char *map2str(int type,mapstr *mapping,int num)
{
	int i;
	char * type_string;
	for(i=0;i<num;i++)
	{
		if(type==mapping[i].type)
		{	
			return(mapping[i].typestr);
		}
	}
	type_string=malloc(10);
	strcpy(type_string,"INVALID");	
	return(type_string);
}


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


hs_pkt_hdr* get_hs_pkt(struct pfring_pkthdr *pkf,unsigned char *buf)
{
	hs_pkt_hdr *packet=(hs_pkt_hdr*)malloc(sizeof(hs_pkt_hdr));
	//Code to assign pfring header and skbuff	
	packet->pkf=*pkf;
	packet->buffer=(unsigned char*)malloc(pkf->caplen*sizeof(unsigned char));
	memcpy(packet->buffer,buf,pkf->caplen*sizeof(unsigned char));
	packet->next=0;
	//packet->buffer=buf;
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
		
	sprintf(fname,"%sBitMaps/%s",path,filename);
	printf("\n%s",fname);
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

void traverse_flow_list(flow_rec_nos *flow_list,char *path,int file_index,int get_packets)
{
	int index,fd_fr,fd_off=-1,fd_pkt=-1,i,cnt=0;
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
	system(command);
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
			if(get_packets){
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
					//Creating Link List of Read Packets
					if(packet_start==0)
					{
						packet_start=end=get_hs_pkt(&pkf,buffer);
					}
					else
					{
						end->next=get_hs_pkt(&pkf,buffer);
						end=end->next;
					}
					free(buffer);
				}
			//To Link to Packets to corresponding Flow Record
				flow_list->pkt_list=packet_start;
				packet_start=0;
			}
		}
		else
			flow_list->pkt_list=0;
		flow_list=flow_list->next;
		
	}

	close(fd_fr);
	if(fd_off > 0)
	{
		close(file_off);
	}
	if(fd_pkt > 0)
		close(fd_pkt);		
	sprintf(command,"%s%s%s%d","lzop -U ",f_path,"flowrec_",file_index);
	system(command);
}

comp_flow_list* get_flow_packets(display_rule *dr,char *path,capture_time *time_cap,int get_packets)
{
	file_list* file_names;
	comp_flow_list *start_flow=0,*end_flow=0;
	file_names=get_filename(time_cap,path);
	while(file_names)
	{
		int file_index=get_file_index(file_names->filename);
		flow_rec_nos* t=decode_bitmap(get_final_bitmap(dr,file_names->filename,path),MAX_FLOW_REC);
		traverse_flow_list(t,path,file_index,get_packets);
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


//fillflowfunction

void fillFlowRecord(JNIEnv *env,jobject flow_obj,jclass cls_flow,flow_record fr)
{
	jfieldID F1;	
		
	F1 = (*env)->GetFieldID(env,cls_flow,"flow_id","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.flow_id);
	 
	F1 = (*env)->GetFieldID(env,cls_flow,"src_port","S");
	(*env)->SetShortField(env,flow_obj,F1,fr.src_port);
			
	F1 = (*env)->GetFieldID(env,cls_flow,"dst_port","S");
	(*env)->SetShortField(env,flow_obj,F1,fr.dst_port);

	F1 = (*env)->GetFieldID(env,cls_flow,"ip_src","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.ip_src.v4);

	F1 = (*env)->GetFieldID(env,cls_flow,"ip_dst","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.ip_dst.v4);

	F1 = (*env)->GetFieldID(env,cls_flow,"protocol","B");
	(*env)->SetByteField(env,flow_obj,F1,fr.protocol);

	F1 = (*env)->GetFieldID(env,cls_flow,"nop","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.nop);

	F1 = (*env)->GetFieldID(env,cls_flow,"start_pkt_no","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.start_pkt_no);

	F1 = (*env)->GetFieldID(env,cls_flow,"nxtfr","I");
	(*env)->SetIntField(env,flow_obj,F1,fr.nxtfr);
			
	F1 = (*env)->GetFieldID(env,cls_flow,"bytes_transfer","J");
	(*env)->SetLongField(env,flow_obj,F1,fr.bytes_transfer);

	F1 = (*env)->GetFieldID(env,cls_flow,"start_tv_sec","J");
	(*env)->SetLongField(env,flow_obj,F1,fr.start_time.tv_sec);

	F1 = (*env)->GetFieldID(env,cls_flow,"start_tv_usec","J");
	(*env)->SetLongField(env,flow_obj,F1,fr.start_time.tv_usec);

	F1 = (*env)->GetFieldID(env,cls_flow,"end_tv_sec","J");
	(*env)->SetLongField(env,flow_obj,F1,fr.end_time.tv_sec);

	F1 = (*env)->GetFieldID(env,cls_flow,"end_tv_usec","J");
	(*env)->SetLongField(env,flow_obj,F1,fr.end_time.tv_usec);
}



void fillpackets(JNIEnv *env,jobject pkt_obj,jclass cls_pkt,hs_pkt_hdr *hs_hdr)
{
	
	void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_icmp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_arp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	
	fill_eth_hdr(env,hs_hdr,pkt_obj,cls_pkt);
	//check for l3 protocol
	printf("\n in c ethernet type is %x",hs_hdr->pkf.extended_hdr.parsed_pkt.eth_type);
	switch(hs_hdr->pkf.extended_hdr.parsed_pkt.eth_type)
	{
		case 0x8100:
			    printf("\n IN C CODE : vlan at L2");
			    break;
		case 0x0800:
			    fill_ip_hdr(env,hs_hdr,pkt_obj,cls_pkt);
			    break;
		case 0x0806:
			    printf("\n IN C FOR ARP PKT");
			    fill_arp_hdr(env,hs_hdr,pkt_obj,cls_pkt);
			    printf("\n after call to fill_arp_hdr");
			    break;		
		default:	
			    break;
	}	
		
	switch(hs_hdr->pkf.extended_hdr.parsed_pkt.l3_proto)
	{
		case IPPROTO_TCP:
				fill_tcp_hdr(env,hs_hdr,pkt_obj,cls_pkt);
				break;
		case IPPROTO_UDP:
				fill_udp_hdr(env,hs_hdr,pkt_obj,cls_pkt);
				break;
		case IPPROTO_ICMP:
				fill_icmp_hdr(env,hs_hdr,pkt_obj,cls_pkt);
		default:
				break;
	}
					

	fill_pfring_hdr(env,hs_hdr,pkt_obj,cls_pkt);
}


jobject get_arp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	struct arphdr *arp = (struct arphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset);
	jfieldID F1;
	jbyteArray jsh;	
	short int temp;	

	//get class which contains members of ethernet structure
	jclass cls_arp=(*env)->FindClass(env,"Core/Protocols/l3Proto/ArpHdr");
	//create object of this class
	jobject arp_obj=(*env)->AllocObject(env,cls_arp);
	
	F1 = (*env)->GetFieldID(env,cls_arp,"ar_hdr","S");
	printf("\n in c ar_hdr is %d",arp->ar_hrd);
	(*env)->SetShortField(env,arp_obj,F1,arp->ar_hrd);

	F1 = (*env)->GetFieldID(env,cls_arp,"ar_pro","S");
	printf("\n in c ar_pro is %d",arp->ar_pro);
	(*env)->SetShortField(env,arp_obj,F1,arp->ar_pro);
	
	F1 = (*env)->GetFieldID(env,cls_arp,"ar_hln","S");
	temp=(short int)arp->ar_hln;	
	printf("\n in c ar_hln is %d",temp);	
	(*env)->SetShortField(env,arp_obj,F1,temp);
		
	/*if(temp==6)
	{
		jsh=(*env)->NewByteArray(env,6);	
		(*env)->SetByteArrayRegion(env,jsh,0,6,(jbyte *)arp->ar_sha);
		F1 = (*env)->GetFieldID(env,cls_arp,"h_source","[S");
		(*env)->SetObjectField(env,arp_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
		jsh=(*env)->NewByteArray(env,6);	
		(*env)->SetByteArrayRegion(env,jsh,0,6,(jbyte *)arp->ar_tha);
		F1 = (*env)->GetFieldID(env,cls_arp,"h_dest","[S");
		(*env)->SetObjectField(env,arp_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
	}*/		
	
	F1 = (*env)->GetFieldID(env,cls_arp,"ar_pln","S");
	temp=(short int)arp->ar_pln;	
	printf("\n in c ar_pln is %d",temp);
	(*env)->SetShortField(env,arp_obj,F1,temp);
		
		
	/*if(temp==4)
	{
		jsh=(*env)->NewByteArray(env,6);	
		(*env)->SetByteArrayRegion(env,jsh,0,6,(jbyte *)arp->ar_sip);
		F1 = (*env)->GetFieldID(env,cls_arp,"h_source","[S");
		(*env)->SetObjectField(env,arp_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
		jsh=(*env)->NewByteArray(env,6);	
		(*env)->SetByteArrayRegion(env,jsh,0,6,(jbyte *)arp->ar_tha);
		F1 = (*env)->GetFieldID(env,cls_arp,"h_dest","[S");
		(*env)->SetObjectField(env,arp_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
	}*/
	
	F1 = (*env)->GetFieldID(env,cls_arp,"ar_op","S");
	printf("\n in c ar_op is %d",arp->ar_op);
	(*env)->SetShortField(env,arp_obj,F1,arp->ar_op);
	
	return(arp_obj);

}

void fill_arp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	printf("\n in c fill_arp_hdr");	
	jobject get_arp_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_arp=(*env)->FindClass(env,"Core/Protocols/l3Proto/ArpHdr");
	jobject arp_obj=(*env)->AllocObject(env,cls_arp);		
	arp_obj=get_arp_hdr(env,hs_pkt);
	if(arp_obj==NULL)	
	printf("\n in c arp_obj is null");
	else
	printf("\n in c arp_obj is not null");
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l3Packet","LCore/Protocols/l3Proto/L3proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,arp_obj);
}



void fill_flow_packets(display_rule *dr,char *path,capture_time *time_cap,JNIEnv *env,jobject obj,jclass cls_main)
{
	
	//JNI DECLARATIONS

	jclass cls_flow;
	jobjectArray flow_obj_arr,pkt_obj_arr,main_obj_arr;
	jsize length=3000;//size of array
	jobject flow_obj,pkt_obj;
	jfieldID F1;
	int i=0,index;
	jint integ;
	int get_packets=1;

	/*void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);*/	
	

	cls_flow=(*env)->FindClass(env,"Core/FlowRecord");//find data class			
	if(cls_flow==NULL)
		printf("\nerror in getting class");
	//make an array of objects
	//flow_obj_arr=(*env)->NewObjectArray(env,length,cls_flow,NULL);

	//main_obj_arr=(*env)->NewObjectArray(env,length,cls_main,NULL);
	//main_obj=(*env)->AllocObject(env,cls_main);//create object	

	
	comp_flow_list* flow_list=get_flow_packets(dr,path,time_cap,get_packets);

	while(flow_list)
	{
	
		flow_rec_nos *frec=flow_list->frec;
		while(frec)
		{
			
			//create object
			jmethodID M1=(*env)->GetMethodID(env,cls_flow,"<init>","()V");
			flow_obj=(*env)->NewObject(env,cls_flow,M1);

			fillFlowRecord(env,flow_obj,cls_flow,frec->fr);
		
			jclass cls_pkt=(*env)->FindClass(env,"Core/CompletePacket");
			pkt_obj_arr=(*env)->NewObjectArray(env,length,cls_pkt,NULL);
			

			//jni ends
			hs_pkt_hdr *hs_hdr=frec->pkt_list;
			
			index=0;			
			while(hs_hdr)
			{								
				//if(M1==NULL)
				//printf("\nerror in getting method");						
								
				pkt_obj=(*env)->AllocObject(env,cls_pkt);
			
				fillpackets(env,pkt_obj,cls_pkt,hs_hdr);
				
				M1=(*env)->GetMethodID(env,cls_flow,"addPacket","(LCore/CompletePacket;)V");
				(*env)->CallVoidMethod(env,flow_obj,M1,pkt_obj);
				
				hs_hdr=hs_hdr->next;
			}
		
			jmethodID M=(*env)->GetMethodID(env,cls_main,"addFlowRecord","(LCore/FlowRecord;)V");
			(*env)->CallVoidMethod(env,obj,M,flow_obj);

			frec=frec->next;
		}
			
			
		flow_list=flow_list->down;
	
	}
	//jni code


	cls_main=(*env)->GetObjectClass(env,obj);
	
	//F1 = (*env)->GetFieldID(env,cls_main,"flow","[LCore/FlowRecord;");//signature for data class.also it is an array of objects so [ is required
	//(*env)->SetObjectField(env,obj,F1,flow_obj_arr);

	//ends
	printf("\n");
}



jobject get_icmp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	jfieldID F1;
	
	struct iphdr *ip = (struct iphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset);
	struct icmphdr *icmp = (struct icmphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset+(ip->ihl*4));
	jbyteArray jsh;
	jstring str;
	
	//get class which contains members of ethernet structure
	jclass cls_icmp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Icmphdr");
	//create object of this class
	jobject icmp_obj=(*env)->AllocObject(env,cls_icmp);
	
	F1 = (*env)->GetFieldID(env,cls_icmp,"type","B");
	printf("type in c is %d",icmp->type);	
	(*env)->SetByteField(env,icmp_obj,F1,icmp->type);
			
	F1 = (*env)->GetFieldID(env,cls_icmp,"code","B");
	printf("code in c is %d",icmp->code);
	(*env)->SetByteField(env,icmp_obj,F1,icmp->code);

	F1 = (*env)->GetFieldID(env,cls_icmp,"checksum","S");
	printf("checksum in c is %d",icmp->checksum);	
	(*env)->SetShortField(env,icmp_obj,F1,icmp->checksum);
	
	F1 = (*env)->GetFieldID(env,cls_icmp,"idOrUnused","I");
	printf("id in c is %d",icmp->un.echo.id);	
	(*env)->SetIntField(env,icmp_obj,F1,icmp->un.echo.id);
		
	F1 = (*env)->GetFieldID(env,cls_icmp,"sequenceOrMtu","I");
	printf("sequence in c is %d",icmp->un.echo.sequence);	
	(*env)->SetIntField(env,icmp_obj,F1,icmp->un.echo.sequence);
	
	F1 = (*env)->GetFieldID(env,cls_icmp,"typeStr","Ljava/lang/String;");
	str = (*env)->NewStringUTF(env,map2str(icmp->type,icmp_type_map,14));//creating a string in format specified in jni
	(*env)->SetObjectField(env,icmp_obj,F1,str);

	switch(icmp->type)
	{
		case ICMP_DEST_UNREACH:	
					F1 = (*env)->GetFieldID(env,cls_icmp,"codeStr","Ljava/lang/String;");
					str = (*env)->NewStringUTF(env,map2str(icmp->code,icmp_du_map,17));
					(*env)->SetObjectField(env,icmp_obj,F1,str);
					break;
		case ICMP_TIME_EXCEEDED:
					F1 = (*env)->GetFieldID(env,cls_icmp,"codeStr","Ljava/lang/String;");
					str = (*env)->NewStringUTF(env,map2str(icmp->code,icmp_timeexceeded_map,2));
					(*env)->SetObjectField(env,icmp_obj,F1,str);
					break;
		case ICMP_REDIRECT:	
					F1 = (*env)->GetFieldID(env,cls_icmp,"codeStr","Ljava/lang/String;");
					str = (*env)->NewStringUTF(env,map2str(icmp->code,icmp_rd_map,4));
					(*env)->SetObjectField(env,icmp_obj,F1,str);
					break;
	}	
	return(icmp_obj);	

}



void fill_icmp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{

	u_int32_t offset;	
	
	jobject get_icmp_hdr(JNIEnv *,hs_pkt_hdr *);	

	jclass cls_icmp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Icmphdr");
	jobject icmp_obj=(*env)->AllocObject(env,cls_icmp);		
	icmp_obj=get_icmp_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,icmp_obj);

	//change
	offset = hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct icmphdr);
	//fill_buffer(env,hs_pkt,pkt_obj,cls_pkt,offset);
	//change ends
}




jobject get_pfring_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	jfieldID F1;
	jbyteArray jsh,jsh2;	
	jstring str;
	
	//get class which contains members of ethernet structure
	jclass cls_pfr=(*env)->FindClass(env,"Core/PfringPktHdr");
	//create object of this class
	jobject pfring_obj=(*env)->AllocObject(env,cls_pfr);
	

	F1 = (*env)->GetFieldID(env,cls_pfr,"caplen","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.caplen);

	F1 = (*env)->GetFieldID(env,cls_pfr,"len","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.len);

	F1 = (*env)->GetFieldID(env,cls_pfr,"timestamp_ns","J");
	(*env)->SetLongField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.timestamp_ns);

	F1 = (*env)->GetFieldID(env,cls_pfr,"rx_direction","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.rx_direction);

	F1 = (*env)->GetFieldID(env,cls_pfr,"if_index","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.if_index);

	F1 = (*env)->GetFieldID(env,cls_pfr,"pkt_hash","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.pkt_hash);

	F1 = (*env)->GetFieldID(env,cls_pfr,"parsed_header_len","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_header_len);  
	      
	F1 = (*env)->GetFieldID(env,cls_pfr,"dmac","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 			//arrays
	jsh=(*env)->NewByteArray(env,7);//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	(*env)->SetByteArrayRegion(env,jsh,0,6,hs_pkt->pkf.extended_hdr.parsed_pkt.dmac);//set values of fields in array through setbytearrayregion.It needs 			//values in terms of the whole array and not individual fields.0 is start and 6 is the size.
	(*env)->SetObjectField(env,pfring_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
	jsh2=(*env)->NewByteArray(env,7);
	F1 = (*env)->GetFieldID(env,cls_pfr,"smac","[B");
	(*env)->SetByteArrayRegion(env,jsh2,0,6,hs_pkt->pkf.extended_hdr.parsed_pkt.smac);
	(*env)->SetObjectField(env,pfring_obj,F1,jsh2);
		
	F1 = (*env)->GetFieldID(env,cls_pfr,"eth_type","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.eth_type);

	F1 = (*env)->GetFieldID(env,cls_pfr,"vlan_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.vlan_id);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_version","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.ip_version);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l3_proto","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.l3_proto);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_tos","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.ip_tos);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_src","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.ip_src.v4);
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_dst","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.ip_dst.v4);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_src_port","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.l4_src_port);	

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_dst_port","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.l4_dst_port);	

	F1 = (*env)->GetFieldID(env,cls_pfr,"seq_num","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.tcp.seq_num);	

	F1 = (*env)->GetFieldID(env,cls_pfr,"ack_num","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.tcp.ack_num);

	F1 = (*env)->GetFieldID(env,cls_pfr,"last_matched_plugin_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.last_matched_plugin_id);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"last_matched_rule_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.last_matched_rule_id);

	F1 = (*env)->GetFieldID(env,cls_pfr,"eth_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.offset.eth_offset);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"vlan_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.offset.vlan_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l3_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"payload_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.offset.payload_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"in_iface","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.pkt_detail.flow.in_iface);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"out_iface","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.pkt_detail.flow.out_iface);

	F1 = (*env)->GetFieldID(env,cls_pfr,"samplingPopulation","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.pkt_detail.flow.samplingPopulation);

	F1 = (*env)->GetFieldID(env,cls_pfr,"flow_sequence","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt->pkf.extended_hdr.parsed_pkt.pkt_detail.flow.flow_sequence);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"buffer","Ljava/lang/String;");//string field check signature if required through javap -s classname
	str = (*env)->NewStringUTF(env,hs_pkt->buffer);//creating a string in format specified in jni
	(*env)->SetObjectField(env,pfring_obj,F1,str);
	return(pfring_obj);
}


void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	jobject get_pfring_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_pfring=(*env)->FindClass(env,"Core/PfringPktHdr");
	jobject pfring_obj=(*env)->AllocObject(env,cls_pfring);		
	pfring_obj=get_pfring_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"pfpacket","LCore/PfringPktHdr;");
	(*env)->SetObjectField(env,pkt_obj,F1,pfring_obj);
	
}




jobject get_udp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	jfieldID F1;
	
	struct udphdr *udp = (struct udphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	jclass cls_udp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Udphdr");
	//create object of this class
	jobject udp_obj=(*env)->AllocObject(env,cls_udp);
	
	F1 = (*env)->GetFieldID(env,cls_udp,"source","S");
	(*env)->SetShortField(env,udp_obj,F1,udp->source);
			
	F1 = (*env)->GetFieldID(env,cls_udp,"dest","S");
	(*env)->SetShortField(env,udp_obj,F1,udp->dest);

	F1 = (*env)->GetFieldID(env,cls_udp,"len","S");
	(*env)->SetShortField(env,udp_obj,F1,udp->len);

	F1 = (*env)->GetFieldID(env,cls_udp,"check","S");
	(*env)->SetShortField(env,udp_obj,F1,udp->check);
	return(udp_obj);	
}

void fill_buffer(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt,int offset)
{
	jfieldID F1;
	jstring jstr;
	const char *str;
	//change
	//struct tcphdr *tcp = (struct tcphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct tcphdr));
	//char *appold=(char *)(tcp);
	jbyteArray jsh;
	char *p;	
	
	jsh=(*env)->NewByteArray(env,hs_pkt->pkf.caplen-offset);	
	jbyte *app=(jbyte *)(hs_pkt->buffer+offset);	
	p=(char *)(hs_pkt->buffer+offset);	
	if(strstr(p,"HTTP")==p || strstr(p,"GET")==p || strstr(p,"POST")==p)
	{	
		F1 = (*env)->GetFieldID(env,cls_pkt,"buffer","[B");
		(*env)->SetByteArrayRegion(env,jsh,0,hs_pkt->pkf.caplen-offset,app);
		(*env)->SetObjectField(env,pkt_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
		F1 = (*env)->GetFieldID(env,cls_pkt,"appProto","Ljava/lang/String;");
		jstr = (*env)->NewStringUTF(env,"HTTP");//creating a string in format specified in jni
		(*env)->SetObjectField(env,pkt_obj,F1,jstr);
		//check
		//str = (*env)->GetStringUTFChars(env, jstr, NULL);
		//printf("value stored in proto=%s\n",str);		
		
	}
	if(strstr(p,"M-SEARCH")==p)
	{	
		printf("\nIN C FOR SSDP PROTOCOL");
		F1 = (*env)->GetFieldID(env,cls_pkt,"buffer","[B");
		(*env)->SetByteArrayRegion(env,jsh,0,hs_pkt->pkf.caplen-offset,app);
		(*env)->SetObjectField(env,pkt_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
		F1 = (*env)->GetFieldID(env,cls_pkt,"appProto","Ljava/lang/String;");
		jstr = (*env)->NewStringUTF(env,"SSDP");//creating a string in format specified in jni
		(*env)->SetObjectField(env,pkt_obj,F1,jstr);
		//check
		//str = (*env)->GetStringUTFChars(env, jstr, NULL);
		//printf("value stored in proto=%s\n",str);		
		
	}

	//jbyte *app=(jbyte *)(hs_pkt->buffer+offset);	
	//printf("\n in c code %s ends here",app);			
	//change ends
	//F1 = (*env)->GetFieldID(env,cls_pkt,"buffer","[B");
		
	//str = (*env)->NewStringUTF(env,app);//creating a string in format specified in jni
	//(*env)->SetObjectField(env,pkt_obj,F1,str);
}



void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{

	u_int32_t offset;	
	jobject get_udp_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_udp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Udphdr");
	jobject udp_obj=(*env)->AllocObject(env,cls_udp);		
	udp_obj=get_udp_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,udp_obj);

	//change
	offset = hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct udphdr);
	//fill_buffer(env,hs_pkt,pkt_obj,cls_pkt,offset);
	//change ends
}




jobject get_tcp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	jfieldID F1;
	struct tcphdr *tcp = (struct tcphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	jclass cls_tcp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Tcphdr");
	//create object of this class
	jobject tcp_obj=(*env)->AllocObject(env,cls_tcp);
	F1 = (*env)->GetFieldID(env,cls_tcp,"source","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->source);

	F1 = (*env)->GetFieldID(env,cls_tcp,"dest","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->dest);
	
	F1 = (*env)->GetFieldID(env,cls_tcp,"seq","I");
	(*env)->SetIntField(env,tcp_obj,F1,tcp->seq);

	F1 = (*env)->GetFieldID(env,cls_tcp,"ack_seq","I");
	(*env)->SetIntField(env,tcp_obj,F1,tcp->ack_seq);

	
	F1 = (*env)->GetFieldID(env,cls_tcp,"window","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->window);

	F1 = (*env)->GetFieldID(env,cls_tcp,"check","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->check);

	F1 = (*env)->GetFieldID(env,cls_tcp,"urg_ptr","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->urg_ptr);
	
	return(tcp_obj);	
}




void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	u_int32_t offset;	
	jobject get_tcp_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_tcp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Tcphdr");
	jobject tcp_obj=(*env)->AllocObject(env,cls_tcp);		
	tcp_obj=get_tcp_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,tcp_obj);
		
	//change
	struct tcphdr *tcp = (struct tcphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	offset = hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset+(4*tcp->doff);
	fill_buffer(env,hs_pkt,pkt_obj,cls_pkt,offset);
	//change ends
}


jobject get_ip_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
	struct iphdr *ip = (struct iphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset);
	jfieldID F1;
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	jclass cls_ip=(*env)->FindClass(env,"Core/Protocols/l3Proto/Iphdr");
	//create object of this class
	jobject ip_obj=(*env)->AllocObject(env,cls_ip);
	
	F1 = (*env)->GetFieldID(env,cls_ip,"ihl","B");
	(*env)->SetByteField(env,ip_obj,F1,ip->ihl);

	F1 = (*env)->GetFieldID(env,cls_ip,"version","B");
	(*env)->SetByteField(env,ip_obj,F1,ip->version);

	F1 = (*env)->GetFieldID(env,cls_ip,"tos","B");
	(*env)->SetByteField(env,ip_obj,F1,ip->tos);

	F1 = (*env)->GetFieldID(env,cls_ip,"tot_len","S");
	(*env)->SetShortField(env,ip_obj,F1,ip->tot_len);

	F1 = (*env)->GetFieldID(env,cls_ip,"id","S");
	(*env)->SetShortField(env,ip_obj,F1,ip->id);

	F1 = (*env)->GetFieldID(env,cls_ip,"frag_off","S");
	(*env)->SetShortField(env,ip_obj,F1,ip->frag_off);

	F1 = (*env)->GetFieldID(env,cls_ip,"ttl","B");
	(*env)->SetByteField(env,ip_obj,F1,ip->ttl);

	F1 = (*env)->GetFieldID(env,cls_ip,"protocol","B");
	(*env)->SetByteField(env,ip_obj,F1,ip->protocol);

	F1 = (*env)->GetFieldID(env,cls_ip,"check","S");
	(*env)->SetShortField(env,ip_obj,F1,ip->check);

	F1 = (*env)->GetFieldID(env,cls_ip,"saddr","I");
	(*env)->SetIntField(env,ip_obj,F1,ip->saddr);

	F1 = (*env)->GetFieldID(env,cls_ip,"daddr","I");
	(*env)->SetIntField(env,ip_obj,F1,ip->daddr);
	
	return(ip_obj);

}

void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	jobject get_ip_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_ip=(*env)->FindClass(env,"Core/Protocols/l3Proto/Iphdr");
	jobject ip_obj=(*env)->AllocObject(env,cls_ip);		
	ip_obj=get_ip_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l3Packet","LCore/Protocols/l3Proto/L3proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,ip_obj);
}



jobject get_eth_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt)
{
		
	struct eth_hdr *eh = (struct eth_hdr*)(hs_pkt->buffer);
	jfieldID F1;
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	jclass cls_eth=(*env)->FindClass(env,"Core/Protocols/l2Proto/Ethhdr");
	
	//create object of this class
	jobject eth_obj=(*env)->AllocObject(env,cls_eth);
	
	//byte array for destination mac	
	F1 = (*env)->GetFieldID(env,cls_eth,"h_dest","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 		
	//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	
	jsh=(*env)->NewByteArray(env,6);	
	(*env)->SetByteArrayRegion(env,jsh,0,6,eh->h_dest);
	(*env)->SetObjectField(env,eth_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield

	//byte array for source mac	
	F1 = (*env)->GetFieldID(env,cls_eth,"h_source","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 		//arrays
	jsh=(*env)->NewByteArray(env,7);//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	(*env)->SetByteArrayRegion(env,jsh,0,6,eh->h_source);//set values of fields in array through setbytearrayregion.It needs values in terms of the whole array and not individual fields.0 is start and 6 is the size.
	(*env)->SetObjectField(env,eth_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
	F1 = (*env)->GetFieldID(env,cls_eth,"h_proto","S");
	(*env)->SetShortField(env,eth_obj,F1,eh->h_proto);
	//printf("protocol is %d",eh->h_proto);


	return(eth_obj);
}



void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	jobject get_eth_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_eth=(*env)->FindClass(env,"Core/Protocols/l2Proto/Ethhdr");
	jobject eth_obj=(*env)->AllocObject(env,cls_eth);		
	eth_obj=get_eth_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l2Packet","LCore/Protocols/l2Proto/L2proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,eth_obj);
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

void increase_size(struct pfring_pkthdr pkf,u_int64_t *arr)
{
  struct tm *time_val;
  time_t tmk;
  tmk=pkf.ts.tv_sec;
  time_val=localtime(&tmk);
  //printf("\n Time is %d:%d:%d",time_val->tm_mday,time_val->tm_hour,time_val->tm_min);
  arr[time_val->tm_hour]+=pkf.caplen;
}

void set_rule(display_rule *dr,int val)
{
	int i;
	dr->sip_oct=(short *)malloc(sizeof(short)*4);
	dr->dip_oct=(short *)malloc(sizeof(short)*4);	
	for(i=0;i<4;i++)
	{
		dr->sip_oct[i]=val;
		dr->dip_oct[i]=val;
	}
	dr->src_port=val;
	dr->dst_port=val;
	dr->protocol=val;
}

u_int64_t* get_hourly_transfer(const char *path,capture_time *time_cap)
{
	int i;
	display_rule dr;
	char path1[100];
  	u_int64_t *cap_size=(u_int64_t*)calloc(24,sizeof(u_int64_t));	
	comp_flow_list* flow_list;	
	set_rule(&dr,-1);	
	/*time_cap->month=2;
	time_cap->day=20;*/
	time_cap->start_hr=0;
	time_cap->start_min=0;
	time_cap->end_hr=23;
	time_cap->end_min=59;
	flow_list=get_flow_packets(&dr,path,time_cap,1);

	while(flow_list)
	{
		flow_rec_nos *frec=flow_list->frec;
		while(frec)
		{
		        hs_pkt_hdr *hs_hdr=frec->pkt_list;
			while(hs_hdr)
			{
				//Change begins(Code to calculate trafficof each hour in a day)
				increase_size(hs_hdr->pkf,cap_size);
				//Change Ends
				hs_hdr=hs_hdr->next;
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

JNIEXPORT void JNICALL Java_FileAccess_datatransfer_fillData
  (JNIEnv * env,jobject obj)// include packages in name
{

	jlongArray jint;
	capture_time *cpt=(capture_time *)malloc(sizeof(capture_time));
	jstring jpath;
	const char *path;
	u_int64_t *data;
	jboolean iscopy;

	
	printf("in c");

	jclass cls_data=(*env)->GetObjectClass(env,obj);
	
	jclass cls_cpt=(*env)->FindClass(env,"Core/CaptureTime");	
	jobject cpt_obj=(*env)->AllocObject(env,cls_cpt);//create object	

	jfieldID F1=(*env)->GetFieldID(env,cls_data,"cpt","LCore/CaptureTime;");
	cpt_obj=(*env)->GetObjectField(env,obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	F1=(*env)->GetFieldID(env,cls_cpt,"month","I");
	cpt->month=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
		
	F1=(*env)->GetFieldID(env,cls_cpt,"day","I");
	cpt->day=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	F1=(*env)->GetFieldID(env,cls_cpt,"start_hr","I");
	cpt->start_hr=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	F1=(*env)->GetFieldID(env,cls_cpt,"start_min","I");
	cpt->start_min=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	F1=(*env)->GetFieldID(env,cls_cpt,"end_hr","I");
	cpt->end_hr=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	F1=(*env)->GetFieldID(env,cls_cpt,"end_min","I");
	cpt->end_min=(*env)->GetIntField(env,cpt_obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
		
	//set path
	F1=(*env)->GetFieldID(env,cls_data,"path","Ljava/lang/String;");
	jpath=(*env)->GetObjectField(env,obj,F1);//similar to setintfield but as array is a reference type it is setobjectfield
	
	path =(*env)->GetStringUTFChars(env,jpath,&iscopy);
	/*printf("\n in c path is %s",path);
	printf("\n in c month is %d",cpt->month);
	printf("\n in c day is %d",cpt->day);
	printf("\n in c start_hr is %d",cpt->start_hr);
	printf("\n in c start_min is %d",cpt->start_min);
	printf("\n in c end_hr is %d",cpt->end_hr);
	printf("\n in c end_min is %d",cpt->end_min);*/

			
	
		//function call
	data=get_hourly_transfer(path,cpt);
	
	F1 = (*env)->GetFieldID(env,cls_data,"bytes_transfer","[J");
	jint=(*env)->NewLongArray(env,24);	
	(*env)->SetLongArrayRegion(env,jint,0,24,data);
	(*env)->SetObjectField(env,obj,F1,jint);//similar to setintfield but as array is a reference type it is setobjectfield
	
}


/*
int main()
{
	capture_time ct;
	display_rule dr;
	
	
	dr.sip_oct[0]=74;
	dr.sip_oct[1]=-1;
	dr.sip_oct[2]=-1;
	dr.sip_oct[3]=-1;

	dr.dip_oct[0]=-1;
	dr.dip_oct[1]=-1;
	dr.dip_oct[2]=-1;
	dr.dip_oct[3]=-1;

	dr.src_port=80;
	dr.dst_port=-1;
	dr.protocol=-1;

	ct.month=0;
	ct.day=31;
	ct.start_hr=9,ct.start_min=20;
	ct.end_hr=9,ct.end_min=29;

	//comp_flow_list *flow=get_flow_packets();
	
	//fill_flow_packets(&dr,"/storage/hs1234/",&ct);
	return 0;

}*/


