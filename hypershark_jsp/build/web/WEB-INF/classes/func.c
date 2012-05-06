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
	
	jclass cls_main=(*env)->GetObjectClass(env,obj);//get main class

	disp_fil(env,obj,&filter,cls_main);
	
	time_fil(env,obj,&tim_fil,cls_main);
	
	F1 = (*env)->GetFieldID(env,cls_main,"path","Ljava/lang/String;");
	str = (*env)->GetObjectField(env,obj,F1);
	if(str==NULL)
	{
		printf("\n error in geting string");
		return;
	}	
	
	path = (*env)->GetStringUTFChars(env,str,NULL);
	//printf("\n in c path is %s",path);
	//call ret query function
	fill_flow_packets(&filter,path,&tim_fil,env,obj,cls_main);	
}



void time_fil(JNIEnv *env,jobject obj,capture_time *filter,jclass cls_main)
{
	
	
	jintArray jint;
	int i=0;
	jboolean iscopy;
 	jfieldID F1;

	jclass cls_rule=(*env)->FindClass(env,"Core/CaptureTime");
	
	F1 = (*env)->GetFieldID(env,cls_main,"cpt","LCore/CaptureTime;");//signature for data class.also it is an array of objects so [ is required
	
	jobject rule_obj=(*env)->AllocObject(env,cls_rule);

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






