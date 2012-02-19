#include <jni.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include "Core_pkts.h"
#include "pf_kernel.h"
#include<linux/pf_ring.h>


JNIEXPORT void JNICALL Java_Core_pkts_fill
  (JNIEnv * env,jobject obj)// include packages in name
{
	printf("\ni inc ");
	hs_pkt_hdr hs_pkt;

	jobject pkt_obj,ip_obj,tcp_obj,udp_obj,pfring_obj;
	jbyteArray jsh,jsh2;
	jfieldID F1;
	
	jstring str;
	int length=100;
	void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass,jclass);
	void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass,jclass);
	void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass,jclass);
	void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass,jclass);
	void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass,jclass);

	jclass cls_main=(*env)->GetObjectClass(env,obj);//get main class

	int i=0;
	jobjectArray pkt_obj_arr;
	int fp;
	fp=open("pkttrac2",O_RDONLY,0);
	if(fp==0)
	{
		printf("file error");
		return;
	}
	jclass cls_pkt=(*env)->FindClass(env,"Core/CompletePacket");
	pkt_obj_arr=(*env)->NewObjectArray(env,length,cls_pkt,NULL);
	jclass cls_eth=(*env)->FindClass(env,"Core/Protocols/l2Proto/Ethhdr");
	jclass cls_ip=(*env)->FindClass(env,"Core/Protocols/l3Proto/Iphdr");	
	jclass cls_tcp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Tcphdr");	
	jclass cls_pfr=(*env)->FindClass(env,"Core/PfringPktHdr");	
	while(i<3)
	{
		int ret,j;
		ret=read(fp,&hs_pkt.pkf,sizeof(struct pfring_pkthdr)); 
		ret=read(fp,hs_pkt.buffer,hs_pkt.pkf.caplen);				
		pkt_obj=(*env)->AllocObject(env,cls_pkt);
		fill_eth_hdr(env,&hs_pkt,pkt_obj,cls_pkt,cls_eth);


		switch(hs_pkt.pkf.extended_hdr.parsed_pkt.eth_type)
		{
			case 0x8100:
				    printf("\n IN C CODE : vlan at L2");
				    break;
			case 0x0800:
				    fill_ip_hdr(env,&hs_pkt,pkt_obj,cls_pkt,cls_ip);
				    break;	
			default:	
				break;
		}	

		switch(hs_pkt.pkf.extended_hdr.parsed_pkt.l3_proto)
		{
			case IPPROTO_TCP:
					fill_tcp_hdr(env,&hs_pkt,pkt_obj,cls_pkt,cls_tcp);
					break;
			case IPPROTO_UDP:
//					fill_udp_hdr(env,&hs_pkt,pkt_obj,cls_pkt);
					break;
			default:
					break;
		}	
		fill_pfring_hdr(env,&hs_pkt,pkt_obj,cls_pkt,cls_pfr);

		(*env)->SetObjectArrayElement(env,pkt_obj_arr,i,pkt_obj);		
		i++;
	}
	
	cls_main=(*env)->GetObjectClass(env,obj);
	F1 = (*env)->GetFieldID(env,cls_main,"packets","[LCore/CompletePacket;");//signature for data class.also it is an array of objects so [ is required
	(*env)->SetObjectField(env,obj,F1,pkt_obj_arr);
	
	close(fp);	
	
}



void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt,jclass cls_pfr)
{
	jobject pfring_obj=(*env)->AllocObject(env,cls_pfr);		
	
	jfieldID F1;
	jbyteArray jsh,jsh2;	
	jstring str;
	
	//get class which contains members of ethernet structure
	
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


	F1=(*env)->GetFieldID(env,cls_pkt,"pfpacket","LCore/PfringPktHdr;");
	(*env)->SetObjectField(env,pkt_obj,F1,pfring_obj);
}

void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt,jclass cls_tcp)
{
		
	jobject tcp_obj=(*env)->AllocObject(env,cls_tcp);		
	
	jfieldID F1;
	struct tcphdr *tcp = (struct tcphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	
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

	F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,tcp_obj);
}



void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt,jclass cls_ip)
{
		
	jobject ip_obj=(*env)->AllocObject(env,cls_ip);		

	struct iphdr *ip = (struct iphdr*)(hs_pkt->buffer+hs_pkt->pkf.extended_hdr.parsed_pkt.offset.l3_offset);
	jfieldID F1;
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	
	
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

	F1 = (*env)->GetFieldID(env,cls_pkt,"l3Packet","LCore/Protocols/l3Proto/L3proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,ip_obj);
}


void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt,jclass cls_eth)
{
	//jobject get_eth_hdr(JNIEnv *,hs_pkt_hdr *);	
	//jclass cls_eth=(*env)->FindClass(env,"Core/Protocols/l2Proto/Ethhdr");
	
	if(cls_eth==NULL)
	printf("no such class");	
	else
	printf("class found");

	jobject eth_obj=(*env)->AllocObject(env,cls_eth);
		struct eth_hdr *eh = (struct eth_hdr*)(hs_pkt->buffer);
	jfieldID F1;
	jbyteArray jsh;	
	//get class which contains members of ethernet structure
	//jclass cls_eth=(*env)->FindClass(env,"Core/Protocols/l2Proto/Ethhdr");
	
	//create object of this class
	//jobject eth_obj=(*env)->AllocObject(env,cls_eth);
	
	//byte array for destination mac	
	F1 = (*env)->GetFieldID(env,cls_eth,"h_dest","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 		
	//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	
	jsh=(*env)->NewByteArray(env,6);	
	(*env)->SetByteArrayRegion(env,jsh,0,6,eh->h_dest);
	(*env)->SetObjectField(env,eth_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield

	//byte array for source mac	
	F1 = (*env)->GetFieldID(env,cls_eth,"h_source","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 		//arrays
	jsh=(*env)->NewByteArray(env,6);//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	(*env)->SetByteArrayRegion(env,jsh,0,6,eh->h_source);//set values of fields in array through setbytearrayregion.It needs values in terms of the whole array and not individual fields.0 is start and 6 is the size.
	(*env)->SetObjectField(env,eth_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
	F1 = (*env)->GetFieldID(env,cls_eth,"h_proto","S");
	(*env)->SetShortField(env,eth_obj,F1,eh->h_proto);
	

	F1 = (*env)->GetFieldID(env,cls_pkt,"l2Packet","LCore/Protocols/l2Proto/L2proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,eth_obj);

}	




