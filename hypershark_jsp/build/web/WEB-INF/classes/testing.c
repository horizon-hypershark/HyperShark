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
#include "testpackage_testing.h"
#include "afcapture.h"


JNIEXPORT void JNICALL Java_testpackage_testing_loadFile
  (JNIEnv * env,jobject jobj)
{
	jfieldID F1;
	char strin[100]="hello world";
	jstring str;	
	jclass cls=(*env)->GetObjectClass(env,jobj);
	F1 = (*env)->GetFieldID(env,cls,"s","Ljava/lang/String;");
        str = (*env)->NewStringUTF(env,strin);
	(*env)->SetObjectField(env,jobj,F1,str);
}


