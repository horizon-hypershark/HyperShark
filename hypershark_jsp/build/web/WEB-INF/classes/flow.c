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
#include "FileAccess_ReadFlow.h"
#include "afcapture.h"


JNIEXPORT void JNICALL Java_FileAccess_ReadFlow_readflow
  (JNIEnv * env,jobject obj)
{
	flow_record flow_struct;
	FILE *fp=fopen("file.txt","rb"); 
	jstring str;
	int i=0,size;
	jfieldID F1;
	jclass cls_flow,cls_main;
	jbyteArray jsh;
	jobject flow_obj;
	jobjectArray flow_obj_arr;
	jsize length=10;

	
        
	cls_flow=(*env)->FindClass(env,"FileAccess/FlowRecord");//find data class	
	
	//make an array of objects
	flow_obj_arr=(*env)->NewObjectArray(env,length,cls_flow,flow_obj);
	
	while(i<5)
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

		F1 = (*env)->GetFieldID(env,cls_flow,"timestamp_ns","J");
		(*env)->SetLongField(env,flow_obj,F1,flow_struct.timestamp_ns);

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
	F1 = (*env)->GetFieldID(env,cls_main,"flow","[LFileAccess/FlowRecord;");//signature for data class.also it is an array of objects so [ is required
	(*env)->SetObjectField(env,obj,F1,flow_obj_arr);
	fclose(fp);
}
