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
#include "Core_capfilter.h"
#include<linux/pf_ring.h>

/*typedef struct capturefilter
{
	short *smac;
	short *dmac;
	int srcHost;
	short *dstHost;
	short *srcHostMask;
	short *dstHostMask;
	u_int16_t lowSrcPort;
	u_int16_t highSrcPort;
	u_int16_t lowDstPort;
	u_int16_t highDstPort;
	u_int16_t protocol;

}capturefilter;*/

u_int32_t set_ip(int shift,short src_host)
{
	u_int32_t temp=0;
	temp=src_host;
	temp<<=shift;
	return temp;
}



JNIEXPORT void JNICALL Java_Core_capfilter_fillcapfilter
  (JNIEnv * env,jobject obj)
{
	int i=0,size;
	jfieldID F1;
	jclass cls_rule,cls_main;
	jobject rule_obj;
	jshortArray jsh;
	jsize length=100;//size of array
	jboolean iscopy;


	short *smac;
	short *dmac;

	short *srcHost;
	short *dstHost;
	u_int32_t ip=0;
	
	int shift;
	
	
	//capturefilter filter; 
	
	filtering_rule_core_fields filter;

	cls_main=(*env)->GetObjectClass(env,obj);
	cls_rule=(*env)->FindClass(env,"Core/CaptureRule");//find data class	

	F1 = (*env)->GetFieldID(env,cls_main,"cpt","LCore/CaptureRule;");//signature for data class.also it is an array of objects so [ is required
	rule_obj=(*env)->AllocObject(env,cls_rule);
	
	//get value in rule_obj
	rule_obj=(*env)->GetObjectField(env,obj,F1);	
	
	jsh=(*env)->NewShortArray(env,6);//create array
	
	//get individual fields from object of rules 	
	F1 = (*env)->GetFieldID(env,cls_rule,"smac","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	smac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	for(i=0;i<6;i++)
	{
		filter.smac[i]=smac[i];	
	}
		

	F1 = (*env)->GetFieldID(env,cls_rule,"dmac","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	dmac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	for(i=0;i<6;i++)
	{
		filter.dmac[i]=dmac[i];	
	}

	F1 = (*env)->GetFieldID(env,cls_rule,"srcHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	srcHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	

	

	//fill srchost
	/*temp=srcHost[0];
	temp<<=24;
	srcip=srcip|temp;
	
	temp=0;
	temp=srcHost[1];
	temp<<=16;
	srcip=srcip|temp;
	
	temp=0;
	temp=srcHost[2];
	temp<<=8;
	srcip=srcip|temp;
	
	temp=0;
	temp=srcHost[3];
	srcip=srcip|temp;*/
	
	
	//FOR source IP
	shift=24;
	for(i=0;i<4;i++)
	{
		ip=ip|set_ip(shift,srcHost[i]);
		shift-=8;
	}
	
		filter.host_low.v4=ip;	
	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"dstHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	dstHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		
	printf("%d%d%d%d",dstHost[0],dstHost[1],dstHost[2],dstHost[3]);	
	//FOR DESTINATION IP
	ip=0;	
	shift=24;
	for(i=0;i<4;i++)
	{
		ip=ip|set_ip(shift,dstHost[i]);
		shift-=8;
	}
	
		filter.host_high.v4=ip;	
	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"lowSrcPort","I");
	filter.port_low=(*env)->GetIntField(env,rule_obj,F1);
		
	F1 = (*env)->GetFieldID(env,cls_rule,"highSrcPort","I");
	filter.port_high=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"protocol","S");
	filter.proto=(*env)->GetShortField(env,rule_obj,F1);

	//change
	F1 = (*env)->GetFieldID(env,cls_rule,"ruleId","I");
	filter.rule_id=(*env)->GetIntField(env,rule_obj,F1);


	//change ends
	printf("\nprinting values in c code\n");

	printf("smac=%d-%d-%d-%d-%d-%d\ndmac=%d-%d-%d-%d-%d-%d\nsrcip=%d.%d.%d.%d",filter.smac[0],filter.smac[1],filter.smac[2],filter.smac[3],filter.smac[4],filter.smac[5],filter.dmac[0],filter.dmac[1],filter.dmac[2],filter.dmac[3],filter.dmac[4],filter.dmac[5],((filter.host_low.v4>>24)& 0xFF),((filter.host_low.v4 >> 16) & 0xFF),((filter.host_low.v4 >> 8) & 0xFF),((filter.host_low.v4 >> 0) & 0xFF));

	printf("\ndstipip=%d.%d.%d.%d\nlowsrcport=%d\nhighsrcport=%dprotocol=%d",((filter.host_high.v4>>24)& 0xFF),((filter.host_high.v4 >> 16) & 0xFF),((filter.host_high.v4 >> 8) & 0xFF),((filter.host_high.v4 >> 0) & 0xFF),filter.port_low,filter.port_high,filter.proto);
	
}



