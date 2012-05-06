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
#include<netinet/in.h> 
#include <sys/socket.h>
#include<stdio.h>
#include <arpa/inet.h>
#include "FileAccess_ChangeStatus.h"
#include<linux/pf_ring.h>
#include"changeStatus.h"


start_data * fill_start_data(const char *path,const char *interface,u_int32_t GFL,u_int16_t hash,u_int16_t gpkt,u_int8_t store_pkt,filtering_rule *filter)
{
	start_data *to_send = (start_data*)malloc(sizeof(start_data));
	strcpy(to_send->option,"START");
	strcpy(to_send->path,path);
	strcpy(to_send->interface,interface);
	to_send->GFL=GFL;
	to_send->hash=hash;
	to_send->gpkt=gpkt;
	to_send->store_pkt=store_pkt;
	to_send->filter=*filter;
	return to_send;
}
stop_data * fill_stop_data(u_int16_t hash)
{
	stop_data *to_send = (stop_data*) malloc(sizeof(stop_data));
	strcpy(to_send->option,"STOP");
	to_send->hash=hash;
	return to_send;
}
start_recv* start_capture(start_data *to_send) 
{ 
	int sd,cd,no_char; 
	char buf[1000]="";
	start_recv *to_recv=(start_recv*)malloc(sizeof(start_recv)); 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("192.168.1.6",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	memcpy(buf,to_send,sizeof(start_data));
	write(sd,buf,1000);		
		//receive recomputed hash in case of collision
	read(sd,buf,1000);	
	free(to_send);
	memcpy(to_recv,buf,sizeof(start_recv));
	printf("\nReceived hash%u",to_recv->hash);
	close(sd);
	return to_recv; 
} 

stop_recv* stop_capture(stop_data *to_send) 
{ 
	int sd,cd,no_char; 
	char buf[1000]="";
	stop_recv *to_recv=(stop_recv*)malloc(sizeof(stop_recv)); 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("192.168.1.6",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	memcpy(buf,to_send,sizeof(stop_data));
	write(sd,buf,1000);
	read(sd,buf,1000);
	close(sd); 
	memcpy(to_recv,buf,sizeof(stop_recv));
	free(to_send);
	return to_recv;
} 

u_int32_t set_ip(int shift,short src_host)
{
	u_int32_t temp=0;
	temp=src_host;
	temp<<=shift;
	return temp;
}

filtering_rule* fillcapturefilter(JNIEnv *env,jobject rule_obj)
{
	filtering_rule *filter=(filtering_rule *)malloc(sizeof(filtering_rule));
	filtering_rule_core_fields filterr;
	jclass cls_rule;
	jfieldID F1;
	jshortArray jsh;
	short *smac;
	short *dmac;

	short *srcHost;
	short *dstHost;
	jboolean iscopy;
	u_int32_t ip=0;
	int i=0,shift,temp;

	memset(filter,0,sizeof(filtering_rule));
	filter->rule_action = dont_forward_packet_and_stop_rule_evaluation;
	cls_rule=(*env)->FindClass(env,"Core/CaptureRule");//find data class	

	jsh=(*env)->NewShortArray(env,6);//create array
	
	//get individual fields from object of rules 	
	
	if(rule_obj==NULL)
	printf("object is null in function ");	
	else
	printf("object is not null in function ");		
	

	F1 = (*env)->GetFieldID(env,cls_rule,"smac","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	if(jsh==NULL)
	printf("smac is null");	
	else	
	{
		smac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		//change
		if(smac[0]!=-1)
		{
			for(i=0;i<6;i++)
			{
			filter->core_fields.smac[i]=smac[i];	
			}
		}
	}
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dmac","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	if(jsh==NULL)
	printf("dmac is null");	
	else	
	{	
		dmac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		if(dmac[0]!=-1)
		{
			for(i=0;i<6;i++)
			{
			filter->core_fields.dmac[i]=dmac[i];	
			}
		}
	}
	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	if(jsh==NULL)
	printf("srchost is null");	
	else	
	{	
		srcHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		if(srcHost[0]!=-1)
		{
		//FOR source IP
			shift=24;
			for(i=0;i<4;i++)
			{
				ip=ip|set_ip(shift,srcHost[i]);
				shift-=8;
			}
			filter->core_fields.host_low.v4=ip;	
		}
	}
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	if(jsh==NULL)
	printf("dsthost is null");	
	else	
	{
		dstHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		if(dstHost[0]!=-1)
		{
			ip=0;	
			shift=24;
			for(i=0;i<4;i++)
			{
				ip=ip|set_ip(shift,dstHost[i]);
				shift-=8;
			}
	
			filter->core_fields.host_high.v4=ip;	
		}
	}
	
	F1 = (*env)->GetFieldID(env,cls_rule,"lowSrcPort","I");
	temp=(*env)->GetIntField(env,rule_obj,F1);
	if(temp==NULL)	
	printf("lowSrcPort is null");	
	else
	{	
		if(temp!=-1)		
		filter->core_fields.port_low=temp;
	}	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"highSrcPort","I");
	temp=(*env)->GetIntField(env,rule_obj,F1);
	if(temp==NULL)	
	printf("lowSrcPort is null");	
	else
	{	
		if(temp!=-1)		
		filter->core_fields.port_high=temp;
	}
	
	F1 = (*env)->GetFieldID(env,cls_rule,"protocol","S");
	temp=(*env)->GetShortField(env,rule_obj,F1);
	if(temp==NULL)	
	printf("lowSrcPort is null");	
	else
	{	
		if(temp!=-1)
		filter->core_fields.proto=temp;
	}	
	

	//change
	F1 = (*env)->GetFieldID(env,cls_rule,"ruleId","I");
	filter->rule_id=(*env)->GetIntField(env,rule_obj,F1);


	//change ends

	filterr=filter->core_fields;
	/*printf("\nprinting values in c code\n");

	printf("smac=%d-%d-%d-%d-%d-%d\ndmac=%d-%d-%d-%d-%d-%d\nsrcip=%d.%d.%d.%d",filterr.smac[0],filterr.smac[1],filterr.smac[2],filterr.smac[3],filterr.smac[4],filterr.smac[5],filterr.dmac[0],filterr.dmac[1],filterr.dmac[2],filterr.dmac[3],filterr.dmac[4],filterr.dmac[5],((filterr.host_low.v4>>24)& 0xFF),((filterr.host_low.v4 >> 16) & 0xFF),((filterr.host_low.v4 >> 8) & 0xFF),((filterr.host_low.v4 >> 0) & 0xFF));

	printf("\ndstipip=%d.%d.%d.%d\nlowsrcport=%d\nhighsrcport=%dprotocol=%d",((filterr.host_high.v4>>24)& 0xFF),((filterr.host_high.v4 >> 16) & 0xFF),((filterr.host_high.v4 >> 8) & 0xFF),((filterr.host_high.v4 >> 0) & 0xFF),filterr.port_low,filterr.port_high,filterr.proto);*/
		
	return filter;

}


JNIEXPORT void JNICALL Java_FileAccess_ChangeStatus_startCapture
  (JNIEnv * env,jobject obj)// include packages in name
{
	int GFL,hashval,keep_packets;	
	jstring str,vm_id;
	const char *cstr,*elementstr,*vmstr;
	jobject vm_obj;
	jstring element;
	jboolean iscopy;
	jmethodID M1;

	jclass cls_vm=(*env)->FindClass(env,"Core/VirtualMachine");	
	jclass cls_main=(*env)->GetObjectClass(env,obj);
	filtering_rule *filter;

	start_recv *to_recv;	

	//call function to fill capture filter
	jobject rule_obj;
	jclass cls_rule=(*env)->FindClass(env,"Core/CaptureRule");	
		
	rule_obj=(*env)->AllocObject(env,cls_rule);


	M1=(*env)->GetMethodID(env,cls_main,"getCaptureRule","(I)LCore/CaptureRule;");
	rule_obj=(*env)->CallObjectMethod(env,obj,M1,0);
	if(rule_obj==NULL)
	{
		filter=(filtering_rule*)malloc(sizeof(filtering_rule));
		memset(filter,0,sizeof(filtering_rule));
	}
	else
	{
		filter=fillcapturefilter(env,rule_obj);
 	}
	//create vm object
	vm_obj=(*env)->AllocObject(env,cls_vm);
	jfieldID F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	vm_obj=(*env)->GetObjectField(env,obj,F1); 

	F1 = (*env)->GetFieldID(env,cls_vm,"packets","I");
	keep_packets=(*env)->GetIntField(env,vm_obj,F1);
	F1 = (*env)->GetFieldID(env,cls_vm,"vmId","Ljava/lang/String;");
	vm_id=(*env)->GetObjectField(env,vm_obj,F1);
	if(vm_id==NULL)
	vmstr=NULL;	
	else	
	vmstr =(*env)->GetStringUTFChars(env,vm_id,&iscopy);
	F1 = (*env)->GetFieldID(env,cls_vm,"dirPath","Ljava/lang/String;");
	str=(*env)->GetObjectField(env,vm_obj,F1);
	
	if(str==NULL)
	cstr=NULL;	
	else	
	cstr = (*env)->GetStringUTFChars(env,str,NULL);
		
	jmethodID mid =(*env)->GetMethodID(env,cls_vm,"getVif","(I)Ljava/lang/String;");
	element=(*env)->CallObjectMethod(env,vm_obj,mid,0);
	if(element==NULL)
	elementstr=NULL;	
	else	
	elementstr = (*env)->GetStringUTFChars(env,element,NULL);
	
	F1 = (*env)->GetFieldID(env,cls_vm,"globalFlowCount","I");
	GFL=(*env)->GetIntField(env,vm_obj,F1);
	F1 = (*env)->GetFieldID(env,cls_vm,"hashVal","I");
	hashval=(*env)->GetIntField(env,vm_obj,F1);
	to_recv=start_capture(fill_start_data(cstr,elementstr,GFL,hashval,0,keep_packets,filter));
	//release strings
	(*env)->ReleaseStringUTFChars(env,str,cstr);
	(*env)->ReleaseStringUTFChars(env,vm_id,vmstr);
	(*env)->ReleaseStringUTFChars(env,element,elementstr);

}


JNIEXPORT void JNICALL Java_FileAccess_ChangeStatus_stopCapture(JNIEnv * env,jobject obj)// include packages in name
{
	jclass cls_vm=(*env)->FindClass(env,"Core/VirtualMachine");	
	jclass cls_main=(*env)->GetObjectClass(env,obj);
	unsigned int hashval;
	stop_recv *recv;	

	jobject vm_obj=(*env)->AllocObject(env,cls_vm);
	jobject main_obj=(*env)->AllocObject(env,cls_main);
		
	jfieldID F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	vm_obj=(*env)->GetObjectField(env,obj,F1);
	//printf("in c function stopcapture");	


	F1 = (*env)->GetFieldID(env,cls_vm,"hashVal","I");
	hashval=(*env)->GetIntField(env,vm_obj,F1);
	
	//printf("\n in c code hashaval is =%d",hashval);


	recv=stop_capture(fill_stop_data(hashval));	
	F1 = (*env)->GetFieldID(env,cls_vm,"globalFlowCount","I");
	(*env)->SetIntField(env,vm_obj,F1,recv->GFL);

	
	F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	(*env)->SetObjectField(env,obj,F1,vm_obj);
}

