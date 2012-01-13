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
char stri[]="HELLO WORLD";
jstring strin;
jfieldID F1;
jclass cls=(*env)->GetObjectClass(env,jobj);

F1 = (*env)->GetFieldID(env,cls,"str","Ljava/lang/String;");
        strin = (*env)->NewStringUTF(env,stri);
	(*env)->SetObjectField(env,jobj,F1,strin);
}








