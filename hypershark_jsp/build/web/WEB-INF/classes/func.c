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
#include "FileAccess_GetPacket.h"

#include "ret_query.h"


/*typedef struct disp_filter
{

       int *sip_oct;
       int *dip_oct;
       loff_t src_port;
       loff_t dst_port;
       int protocol;
}disp_filter;

typedef struct time_filter
{
	int month;
	int day;
	int start_hr,start_min;
	int end_hr,end_min;
}time_filter;
*/

JNIEXPORT void JNICALL Java_FileAccess_GetPacket_fillpackets
  (JNIEnv * env,jobject obj)// include packages in name
{
	hs_pkt_hdr hs_pkt;

/*	Rule rule;*/
	display_rule filter;

	filter.sip_oct=(short *)malloc(4*sizeof(short));
	filter.dip_oct=(short *)malloc(4*sizeof(short));
	//time structure
	capture_time tim_fil;


	jobject pkt_obj,ip_obj,tcp_obj,udp_obj,pfring_obj;
	jbyteArray jsh,jsh2;
	jfieldID F1;
	struct iphdr *ip;	
	const char *path;
	jstring str;
	int length=100;
	void fill_eth_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_ip_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_tcp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);
	void fill_pfring_hdr(JNIEnv *env,hs_pkt_hdr *,jobject,jclass);	
	void disp_fil(JNIEnv *,jobject ,display_rule *,jclass);
	void time_fil(JNIEnv *,jobject ,capture_time *,jclass);
	void readflows(JNIEnv *,jobject ,jclass);
	//void fill_rules(JNIEnv *,jobject ,Rule,jclass);
	//fill_rules(env,obj,rule,cls_main);//call fill_rules and pass main class reference	

	printf("\n in jni");
	jclass cls_main=(*env)->GetObjectClass(env,obj);//get main class

	disp_fil(env,obj,&filter,cls_main);
	
	time_fil(env,obj,&tim_fil,cls_main);
	
	printf("\n in c before cal path");
	
	F1 = (*env)->GetFieldID(env,cls_main,"path","Ljava/lang/String;");
	str = (*env)->GetObjectField(env,obj,F1);
	if(str==NULL)
	printf("\n error in geting string");
	else
	printf("\n no error in string java");

	path = (*env)->GetStringUTFChars(env,str,NULL);
	
	printf("\n in c path is %s",path);
	//call ret query function
	fill_flow_packets(&filter,path,&tim_fil,env,obj,cls_main);	

	//readflows(env,obj,cls_main);


	/*int i=0;
	jobjectArray pkt_obj_arr;
	int fp;
	fp=open("pkttrac2",O_RDONLY,0);
	if(fp==0)
	{
		printf("file error");
		return;
	}

	
	//L2 PROTOCOL
	//create an object array for ethernet packets
	jclass cls_pkt=(*env)->FindClass(env,"Core/CompletePacket");
	pkt_obj_arr=(*env)->NewObjectArray(env,length,cls_pkt,NULL);
	//for ethernet
	while(i<3)
	{
		int ret,j;
		ret=read(fp,&hs_pkt.pkf,sizeof(struct pfr-ing_pkthdr)); 
		ret=read(fp,hs_pkt.buffer,hs_pkt.pkf.caplen);				
		pkt_obj=(*env)->AllocObject(env,cls_pkt);		
		//find reference to class completepacket 
		//create object of this class
		fill_eth_hdr(env,&hs_pkt,pkt_obj,cls_pkt);

		//check for l3 protocol
		switch(hs_pkt.pkf.extended_hdr.parsed_pkt.eth_type)
		{
			case 0x8100:
				    printf("\n IN C CODE : vlan at L2");
				    break;
			case 0x0800:
				    fill_ip_hdr(env,&hs_pkt,pkt_obj,cls_pkt);
				    break;	
			default:	
				break;
		}	
		
		switch(hs_pkt.pkf.extended_hdr.parsed_pkt.l3_proto)
		{
			case IPPROTO_TCP:
					fill_tcp_hdr(env,&hs_pkt,pkt_obj,cls_pkt);
					break;
			case IPPROTO_UDP:
					fill_udp_hdr(env,&hs_pkt,pkt_obj,cls_pkt);
					break;
			default:
					break;
		}
		//fill pfring header
		fill_pfring_hdr(env,&hs_pkt,pkt_obj,cls_pkt);
		(*env)->SetObjectArrayElement(env,pkt_obj_arr,i,pkt_obj);		
		i++;	
	}
        cls_main=(*env)->GetObjectClass(env,obj);
	F1 = (*env)->GetFieldID(env,cls_main,"packets","[LCore/CompletePacket;");//signature for data class.also it is an array of objects so [ is required
	(*env)->SetObjectField(env,obj,F1,pkt_obj_arr);
	close(fp);	
	*/
}

void readflows(JNIEnv *env,jobject obj,jclass cls_main)
{
	flow_record flow_struct;
	FILE *fp=fopen("flowrec","rb");
	jstring str;
	int i=0,size;
	jfieldID F1;
	jclass cls_flow;
	jbyteArray jsh;
	jobject flow_obj;
	jobjectArray flow_obj_arr;
	jsize length=100;//size of array

	
        
	cls_flow=(*env)->FindClass(env,"Core/FlowRecord");//find data class	
	
	//make an array of objects
	flow_obj_arr=(*env)->NewObjectArray(env,length,cls_flow,flow_obj);
	
	while(i<30)
	{
	
		fread(&flow_struct,sizeof(flow_record),1,fp);//read file
		flow_obj=(*env)->AllocObject(env,cls_flow);//create object	

		F1 = (*env)->GetFieldID(env,cls_flow,"flow_id","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.flow_id);
	 
		F1 = (*env)->GetFieldID(env,cls_flow,"src_port","S");
		(*env)->SetShortField(env,flow_obj,F1,flow_struct.src_port);


		F1 = (*env)->GetFieldID(env,cls_flow,"dst_port","S");
		(*env)->SetShortField(env,flow_obj,F1,flow_struct.dst_port);

		F1 = (*env)->GetFieldID(env,cls_flow,"ip_src","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.ip_src.v4);

		F1 = (*env)->GetFieldID(env,cls_flow,"ip_dst","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.ip_dst.v4);

		F1 = (*env)->GetFieldID(env,cls_flow,"protocol","B");
		(*env)->SetByteField(env,flow_obj,F1,flow_struct.protocol);

		/*F1 = (*env)->GetFieldID(env,cls_flow,"bytes_transfer","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.bytes_transfer);

		F1 = (*env)->GetFieldID(env,cls_flow,"start_tv_sec","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.start_time.tv_sec);

		F1 = (*env)->GetFieldID(env,cls_flow,"start_tv_usec","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.start_time.tv_usec);

		F1 = (*env)->GetFieldID(env,cls_flow,"end_tv_sec","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.end_time.tv_sec);

		F1 = (*env)->GetFieldID(env,cls_flow,"end_tv_usec","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.end_time.tv_usec);*/



		F1 = (*env)->GetFieldID(env,cls_flow,"nop","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.nop);

	   	F1 = (*env)->GetFieldID(env,cls_flow,"start_pkt_no","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.start_pkt_no);

		F1 = (*env)->GetFieldID(env,cls_flow,"nxtfr","I");
		(*env)->SetIntField(env,flow_obj,F1,flow_struct.nxtfr);
		//set object array element
		(*env)->SetObjectArrayElement(env,flow_obj_arr,i,flow_obj);
		i++;	
	}
		
	cls_main=(*env)->GetObjectClass(env,obj);
	F1 = (*env)->GetFieldID(env,cls_main,"flow","[LCore/FlowRecord;");//signature for data class.also it is an array of objects so [ is required
	(*env)->SetObjectField(env,obj,F1,flow_obj_arr);
	
	fclose(fp);
}





void time_fil(JNIEnv *env,jobject obj,capture_time *filter,jclass cls_main)
{
	
	
	jintArray jint;
	int i=0;
	jboolean iscopy;
 	jfieldID F1;
	
	//FILE *fp=fopen("checkdata.txt","w");

	jclass cls_rule=(*env)->FindClass(env,"Core/CaptureTime");
	
	F1 = (*env)->GetFieldID(env,cls_main,"cpt","LCore/CaptureTime;");//signature for data class.also it is an array of objects so [ is required
	
	jobject rule_obj=(*env)->AllocObject(env,cls_rule);
	
	//sjobject main_obj=(*env)->AllocObject(env,cls_main);

	rule_obj=(*env)->GetObjectField(env,obj,F1); 
	
	F1 = (*env)->GetFieldID(env,cls_rule,"month","I");
	filter->month=(*env)->GetIntField(env,rule_obj,F1);

	F1 = (*env)->GetFieldID(env,cls_rule,"day","I");
	filter->day=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"start_hr","I");
	filter->start_hr=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"start_min","I");
	filter->start_min=(*env)->GetIntField(env,rule_obj,F1);

	F1 = (*env)->GetFieldID(env,cls_rule,"end_hr","I");
	filter->end_hr=(*env)->GetIntField(env,rule_obj,F1);

	F1 = (*env)->GetFieldID(env,cls_rule,"end_min","I");
	filter->end_min=(*env)->GetIntField(env,rule_obj,F1);

	//fprintf(fp,"%d%d%d%d%d%d",filter->day,filter->month,filter->start_hr,filter->start_min,filter->end_hr,filter->end_min);
	//printf("\nmonth is =%d",filter->month);
	//fclose(fp);

}




void disp_fil(JNIEnv *env,jobject obj,display_rule *filter,jclass cls_main)
{
	
	
	jshortArray jint;
	int i=0;
	jboolean iscopy;
 	jfieldID F1;
	

	jclass cls_rule=(*env)->FindClass(env,"Core/DisplayPktRule");
	
	F1 = (*env)->GetFieldID(env,cls_main,"rules","LCore/DisplayPktRule;");//signature for data class.also it is an array of objects so [ is required
	
	jobject rule_obj=(*env)->AllocObject(env,cls_rule);
	rule_obj=(*env)->GetObjectField(env,obj,F1);//get the whole array into rule_obj_arr:it is an object array 
	
		
	jint=(*env)->NewShortArray(env,4);//create array
	
	//get individual fields from object of rules 	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcHost","[S");
	jint=(*env)->GetObjectField(env,rule_obj,F1);
	filter->sip_oct=(*env)->GetShortArrayElements(env,jint,&iscopy);	
			
	F1 = (*env)->GetFieldID(env,cls_rule,"dstHost","[S");
	jint=(*env)->GetObjectField(env,rule_obj,F1);
	filter->dip_oct=(*env)->GetShortArrayElements(env,jint,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"highPort","I");
	filter->dst_port=(*env)->GetIntField(env,rule_obj,F1);
		
	F1 = (*env)->GetFieldID(env,cls_rule,"lowPort","I");
	filter->src_port=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"protocol","S");
	filter->protocol=(*env)->GetShortField(env,rule_obj,F1);

		
}







/*void fill_rules(JNIEnv *env,jobject obj,Rule rule,jclass cls_main)
{
	
	jobjectArray rule_obj_arr;
	jbyteArray jb1;
	jshortArray jsh;
	//jbyte *srcOctet1,*srcOctet2,*srcOctet3,*srcOctet4,*dstOctet1,*dstOctet2,*dstOctet3,*dstOctet4,*protocol;
	//jshort *srcPort,dstPort;
	int length=100,i=0;
	jboolean iscopy;
 	jfieldID F1;
	jclass cls_rule=(*env)->FindClass(env,"Core/Rules");
	if(cls_rule==NULL)
	{
		printf("\nerror in getting class");
		return;	
	}	
	else
	{
		printf("\nno error in getting class");	
	}
	//rule_obj_arr=(*env)->NewObjectArray(env,length,cls_rule,NULL);
	
	



	F1 = (*env)->GetFieldID(env,cls_main,"rules","LCore/Rules;");//signature for data class.also it is an array of objects so [ is required
	jobject rule_obj=(*env)->AllocObject(env,cls_rule);
	rule_obj=(*env)->GetObjectField(env,obj,F1);//get the whole array into rule_obj_arr:it is an object array 
	
		
	jb1=(*env)->NewByteArray(env,100);//create byte array
	jsh=(*env)->NewShortArray(env,100);//create short array
	

	//get individual fields from object of rules 	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet1","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.srcOctet1=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
			
	/*length=(*env)->GetArrayLength(env,jb1);	
	printf("length is%d",length);*///for computing length of an array	
	
	//for printing individual values	
	/*for(i=0;i<20;i++)
	{
	printf("%d",rule.srcOctet1[i]);
	}*/			
	
/*	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet2","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.srcOctet2=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet3","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.srcOctet3=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet4","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.srcOctet4=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet1","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.dstOctet1=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet2","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.dstOctet2=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet3","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.dstOctet3=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet4","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.dstOctet4=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcPort","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	rule.srcPort=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstPort","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	rule.dstPort=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"protocol","[B");
	jb1=(*env)->GetObjectField(env,rule_obj,F1);
	rule.protocol=(*env)->GetByteArrayElements(env,jb1,&iscopy);	
	

*/


/*(rule+i)->srcOctet1=(*env)->GetByteField(env,rule_obj,F1);


		F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet2","B");
		(rule+i)->srcOctet2=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet3","B");
		(rule+i)->srcOctet3=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"srcOctet4","B");
		(rule+i)->srcOctet4=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet1","B");
		(rule+i)->dstOctet1=(*env)->GetByteField(env,rule_obj,F1);
		printf("dst octet1 is=%d",(rule+i)->dstOctet1);
		F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet2","B");
		(rule+i)->dstOctet2=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet3","B");
		(rule+i)->dstOctet3=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"dstOctet4","B");
		(rule+i)->dstOctet4=(*env)->GetByteField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"srcPort","S");
		(rule+i)->srcPort=(*env)->GetShortField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"dstPort","S");
		(rule+i)->dstPort=(*env)->GetShortField(env,rule_obj,F1);
		F1 = (*env)->GetFieldID(env,cls_rule,"protocol","B");
		(rule+i)->protocol=(*env)->GetByteField(env,rule_obj,F1);
	}
*/
/*
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
	jobject get_tcp_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_tcp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Tcphdr");
	jobject tcp_obj=(*env)->AllocObject(env,cls_tcp);		
	tcp_obj=get_tcp_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,tcp_obj);
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


void fill_udp_hdr(JNIEnv *env,hs_pkt_hdr *hs_pkt,jobject pkt_obj,jclass cls_pkt)
{
	jobject get_udp_hdr(JNIEnv *,hs_pkt_hdr *);	
	jclass cls_udp=(*env)->FindClass(env,"Core/Protocols/l4Proto/Udphdr");
	jobject udp_obj=(*env)->AllocObject(env,cls_udp);		
	udp_obj=get_udp_hdr(env,hs_pkt);
	jfieldID F1 = (*env)->GetFieldID(env,cls_pkt,"l4Packet","LCore/Protocols/l4Proto/L4proto;");
	(*env)->SetObjectField(env,pkt_obj,F1,udp_obj);
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



/*jobject fill_ip_hdr(JNIEnv *env,char *buf)
{

	jfieldID F1;
	struct iphdr *ip = (struct iphdr*)(buf+pkf.extended_hdr.parsed_pkt.offset.l3_offset);
	//get class which contains members of ip header structure
	jclass cls_ip=(*env)->FindClass(env,"Core/Protocols/l2Proto/Iphdr");
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


jobject fill_tcp_hdr(JNIEnv *env)
{
	
	jfieldID F1;
	struct tcphdr *tcp = (struct tcphdr*)(hs_pkt.buffer+hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	jclass cls_tcp=(*env)->FindClass(env,"Core/Protocols/l3Proto/Tcphdr");
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

	//F1 = (*env)->GetFieldID(env,cls_tcp,"flags","S");
	//(*env)->SetShortField(env,tcp_obj,F1,tcp->flags);
	//ask about these fields it is bit field in original structure	
	
	
	F1 = (*env)->GetFieldID(env,cls_tcp,"window","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->window);

	F1 = (*env)->GetFieldID(env,cls_tcp,"check","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->check);

	F1 = (*env)->GetFieldID(env,cls_tcp,"urg_ptr","S");
	(*env)->SetShortField(env,tcp_obj,F1,tcp->urg_ptr);
	
	return(tcp_obj);		
}

jobject fill_udp_hdr(JNIEnv *env)
{
	jfieldID F1;	
	struct udphdr *udp = (struct udphdr*)(hs_pkt.buffer+hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	jclass cls_udp=(*env)->FindClass(env,"Core/Protocols/l3Proto/Udphdr");
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
		

jobject fill_pfring_hdr(JNIEnv *env)
{	
	
	jbyteArray jsh,jsh2;
	jstring str;
	jclass cls_pfr=(*env)->FindClass(env,"Core/Pfring_Pkthdr");
	jobject pfring_obj=(*env)->AllocObject(env,cls_pfr);//allocate an object: setobjectarrayelement function needs this to set each element of array	
		
	jfieldID F1;	
	F1 = (*env)->GetFieldID(env,cls_pfr,"caplen","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.caplen);

	F1 = (*env)->GetFieldID(env,cls_pfr,"len","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.len);

	F1 = (*env)->GetFieldID(env,cls_pfr,"timestamp_ns","J");
	(*env)->SetLongField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.timestamp_ns);

	F1 = (*env)->GetFieldID(env,cls_pfr,"rx_direction","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.rx_direction);

	F1 = (*env)->GetFieldID(env,cls_pfr,"if_index","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.if_index);

	F1 = (*env)->GetFieldID(env,cls_pfr,"pkt_hash","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.pkt_hash);

	F1 = (*env)->GetFieldID(env,cls_pfr,"parsed_header_len","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_header_len);  
	      
	F1 = (*env)->GetFieldID(env,cls_pfr,"dmac","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 			//arrays
	jsh=(*env)->NewByteArray(env,7);//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
	(*env)->SetByteArrayRegion(env,jsh,0,6,hs_pkt.pkf.extended_hdr.parsed_pkt.dmac);//set values of fields in array through setbytearrayregion.It needs 			//values in terms of the whole array and not individual fields.0 is start and 6 is the size.
	(*env)->SetObjectField(env,pfring_obj,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
	jsh2=(*env)->NewByteArray(env,7);
	F1 = (*env)->GetFieldID(env,cls_pfr,"smac","[B");
	(*env)->SetByteArrayRegion(env,jsh2,0,6,hs_pkt.pkf.extended_hdr.parsed_pkt.smac);
	(*env)->SetObjectField(env,pfring_obj,F1,jsh2);
		
	F1 = (*env)->GetFieldID(env,cls_pfr,"eth_type","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.eth_type);

	F1 = (*env)->GetFieldID(env,cls_pfr,"vlan_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.vlan_id);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_version","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_version);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l3_proto","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l3_proto);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_tos","B");
	(*env)->SetByteField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_tos);

	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_src","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4);
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"ip_dst","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_src_port","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_src_port);	

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_dst_port","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_dst_port);	

	//F1 = (*env)->GetFieldID(env,cls_pfr,"flags","S");
	//(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.flags);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"seq_num","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.seq_num);	

	F1 = (*env)->GetFieldID(env,cls_pfr,"ack_num","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.ack_num);

	F1 = (*env)->GetFieldID(env,cls_pfr,"last_matched_plugin_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_plugin_id);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"last_matched_rule_id","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_rule_id);

	F1 = (*env)->GetFieldID(env,cls_pfr,"eth_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.eth_offset);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"vlan_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.vlan_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l3_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l3_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"l4_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l4_offset);
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"payload_offset","S");
	(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.payload_offset);

	F1 = (*env)->GetFieldID(env,cls_pfr,"in_iface","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.in_iface);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"out_iface","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.out_iface);

	F1 = (*env)->GetFieldID(env,cls_pfr,"samplingPopulation","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.samplingPopulation);

	F1 = (*env)->GetFieldID(env,cls_pfr,"flow_sequence","I");
	(*env)->SetIntField(env,pfring_obj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.flow_sequence);	
	
	F1 = (*env)->GetFieldID(env,cls_pfr,"buffer","Ljava/lang/String;");//string field check signature if required through javap -s classname
	str = (*env)->NewStringUTF(env,hs_pkt.buffer);//creating a string in format specified in jni
	(*env)->SetObjectField(env,pfring_obj,F1,str);
	
	//F1 = (*env)->GetFieldID(env,cls_pfr,"buffer_len","S");
	//(*env)->SetShortField(env,pfring_obj,F1,hs_pkt.buffer_len);
	
	return(pfring_obj);
}*/
