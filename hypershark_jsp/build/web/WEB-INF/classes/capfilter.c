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


typedef struct capturefilter
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

}capturefilter;

JNIEXPORT void JNICALL Java_Core_capfilter_fillcapfilter
  (JNIEnv * env,jobject obj)
{
	int i=0,size;
	jfieldID F1;
	jclass cls_rule,cls_main;
	jobject rule_obj;
	jshortArray jsh;;
	jsize length=100;//size of array
	jboolean iscopy;

	short *srcHost;
	short *dstHost;
	short *srcHostMask;
	short *dstHostMask;
	int srcip;
	
	
	capturefilter filter; 
	
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
	filter.smac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
			
	F1 = (*env)->GetFieldID(env,cls_rule,"dmac","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	filter.dmac=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"srcHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	srcHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	

	//fill srchost
	srcip=srcHost[0];
	srcip<<=8;
	srcip=srcHost[1];
	srcip<<=8;
	srcip=srcHost[2];
	srcip<<=8;
	srcip=srcHost[3];
	srcip<<=8;

	filter.srcHost=srcip;

			
	F1 = (*env)->GetFieldID(env,cls_rule,"dstHost","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	dstHost=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
		
	F1 = (*env)->GetFieldID(env,cls_rule,"srcHostMask","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	srcHostMask=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
			
	F1 = (*env)->GetFieldID(env,cls_rule,"dstHostMask","[S");
	jsh=(*env)->GetObjectField(env,rule_obj,F1);
	dstHostMask=(*env)->GetShortArrayElements(env,jsh,&iscopy);	
	
	
	F1 = (*env)->GetFieldID(env,cls_rule,"lowSrcPort","I");
	filter.lowSrcPort=(*env)->GetIntField(env,rule_obj,F1);
		
	F1 = (*env)->GetFieldID(env,cls_rule,"highSrcPort","I");
	filter.highSrcPort=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"lowDstPort","I");
	filter.lowDstPort=(*env)->GetIntField(env,rule_obj,F1);
		
	F1 = (*env)->GetFieldID(env,cls_rule,"highDstPort","I");
	filter.highDstPort=(*env)->GetIntField(env,rule_obj,F1);
	
	F1 = (*env)->GetFieldID(env,cls_rule,"protocol","S");
	filter.protocol=(*env)->GetShortField(env,rule_obj,F1);

	printf("\nprinting values in c code\n");

	printf("smac=%d-%d-%d-%d-%d-%d\ndmac=%d-%d-%d-%d-%d-%d\nsrcip=%d.%d.%d.%d\n",filter.smac[0],filter.smac[1],filter.smac[2],filter.smac[3],filter.smac[4],filter.smac[5],filter.dmac[0],filter.dmac[1],filter.dmac[2],filter.dmac[3],filter.dmac[4],filter.dmac[5],((filter.srcHost>>24)& 0xFF),((filter.srcHost >> 16) & 0xFF),((filter.srcHost >> 8) & 0xFF),((filter.srcHost >> 0) & 0xFF));

	printf("\ndestip=%d.%d.%d.%d\nsourcemask=%d.%d.%d.%d\ndestmask=%d.%d.%d.%d\nlowsrcport=%d\nlowDstport=%d,highsrcport=%d,highdstport=%d,protocol=%d",filter.dstHost[0],filter.dstHost[1],filter.dstHost[2],filter.dstHost[3],filter.srcHostMask[0],filter.srcHostMask[1],filter.srcHostMask[2],filter.srcHostMask[3],filter.dstHostMask[0],filter.dstHostMask[1],filter.dstHostMask[2],filter.dstHostMask[3],filter.lowSrcPort,filter.lowDstPort,filter.highSrcPort,filter.highDstPort,filter.protocol);
	
}



