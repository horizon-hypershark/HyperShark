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


int start_capture(const char *path,const char *interface,u_int32_t GFL,u_int16_t hash,u_int16_t gpkt) 
{ 
	int sd,cd,no_char; 
	char buf[1000]="",str[100]=""; 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("192.168.1.6",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	no_char=sprintf(str,"%s %u %u %s %s %u","START",hash,GFL,interface,path,gpkt);
	//printf("\nSending Buffer :%s",str);
		//send start signal
	//write(sd,"START",6); 		
		//send hash+GFL+interface+path value
	write(sd,str,no_char+1);		
		//receive recomputed hash in case of collision
	read(sd,buf,1000);	
	printf("\nReceived hash%s",buf);
	close(sd);
	return atoi(buf); 
} 

u_int32_t stop_capture(u_int16_t hash,u_int32_t *gfl,u_int16_t *gpkt) 
{ 
	int sd,cd,no_char; 
	char buf[1000]="",str[100]=""; 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("192.168.1.6",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	//write(sd,"STOP",5); 
	//sleep(1);
	no_char=sprintf(buf,"%s %d","STOP",hash);
	printf("\nIn stop Hash value is:%s",buf);
	write(sd,buf,no_char+1);
	read(sd,buf,1000);
	close(sd); 
	sscanf(buf,"%u%u",&gfl,&gpkt);
	return 0;
} 



JNIEXPORT void JNICALL Java_FileAccess_ChangeStatus_startCapture
  (JNIEnv * env,jobject obj)// include packages in name
{
	int GFL,hashval;	
	jstring str,vm_id;
	const char *cstr,*elementstr,*vmstr;
	jobject vm_obj;
	jstring element;
	jboolean iscopy;


	
	jclass cls_vm=(*env)->FindClass(env,"Core/VirtualMachine");	
	jclass cls_main=(*env)->GetObjectClass(env,obj);	
	
	//create vm object
	vm_obj=(*env)->AllocObject(env,cls_vm);
	printf("\nin c function startcapture\n");

	
	jfieldID F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	vm_obj=(*env)->GetObjectField(env,obj,F1); 

	F1 = (*env)->GetFieldID(env,cls_vm,"vmId","Ljava/lang/String;");
	vm_id=(*env)->GetObjectField(env,vm_obj,F1);
	if(vm_id==NULL)
	vmstr=NULL;	
	else	
	vmstr =(*env)->GetStringUTFChars(env,vm_id,&iscopy);
		


	
	//printf("\n in c vm id =%d\n",vm_id);
	F1 = (*env)->GetFieldID(env,cls_vm,"dirPath","Ljava/lang/String;");
	str=(*env)->GetObjectField(env,vm_obj,F1);
	
	if(str==NULL)
	cstr=NULL;	
	else	
	cstr = (*env)->GetStringUTFChars(env,str,NULL);
		
	printf("\n IN C CODE\n");	
	printf("\n in c PATH  is =%s\n",cstr);
	
	//accessing arraylist	
	//F1 = (*env)->GetFieldID(env,cls_vm,"vifs","Ljava/util/ArrayList;");
	

	//accessing method getvif
	jmethodID mid =(*env)->GetMethodID(env,cls_vm,"getVif","(I)Ljava/lang/String;");
	element=(*env)->CallObjectMethod(env,vm_obj,mid,0);
	if(element==NULL)
	elementstr=NULL;	
	else	
	elementstr = (*env)->GetStringUTFChars(env,element,NULL);
	
	//printf("\n in c code vifs=%s",elementstr);
	F1 = (*env)->GetFieldID(env,cls_vm,"globalFlowCount","I");
	GFL=(*env)->GetIntField(env,vm_obj,F1);
	
	//accessing hashval
	
	F1 = (*env)->GetFieldID(env,cls_vm,"hashVal","I");
	hashval=(*env)->GetIntField(env,vm_obj,F1);
		

	//printing thevalues
	printf("\n in c vm id =%s\n",vmstr);
	printf("\n in c PATH  is =%s\n",cstr);
	printf(" in c vif   is =%s\n",elementstr);
	printf("\n in c hashval is is =%d\n",hashval);

	start_capture(cstr,elementstr,GFL,hashval,0);
	//change 
	//daemon_compress("/storage/hs1234/");//for daemon	
	

	//release strings
	(*env)->ReleaseStringUTFChars(env,str,cstr);
	(*env)->ReleaseStringUTFChars(env,vm_id,vmstr);
	(*env)->ReleaseStringUTFChars(env,element,elementstr);

	
	
}


JNIEXPORT void JNICALL Java_FileAccess_ChangeStatus_stopCapture(JNIEnv * env,jobject obj)// include packages in name
{
	jclass cls_vm=(*env)->FindClass(env,"Core/VirtualMachine");	
	jclass cls_main=(*env)->GetObjectClass(env,obj);
	unsigned int hashval,GFL,GPKT;	

	jobject vm_obj=(*env)->AllocObject(env,cls_vm);
	jobject main_obj=(*env)->AllocObject(env,cls_main);
		
	jfieldID F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	vm_obj=(*env)->GetObjectField(env,obj,F1);
	printf("in c function stopcapture");	


	F1 = (*env)->GetFieldID(env,cls_vm,"hashVal","I");
	hashval=(*env)->GetIntField(env,vm_obj,F1);
	
	printf("\n in c code hashaval is =%d",hashval);


	stop_capture(hashval,&GFL,&GPKT);	
	F1 = (*env)->GetFieldID(env,cls_vm,"globalFlowCount","I");
	(*env)->SetIntField(env,vm_obj,F1,GFL);

	
	F1 = (*env)->GetFieldID(env,cls_main,"vm","LCore/VirtualMachine;");
	(*env)->SetObjectField(env,obj,F1,vm_obj);
		
				
	
	
}

