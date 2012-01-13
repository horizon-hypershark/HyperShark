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
#include "testpackage_test2.h"
//#include "afcapture.h"


JNIEXPORT void JNICALL Java_testpackage_test2_loadFile
  (JNIEnv * env,jobject jobj)
{
	typedef struct s
	{
		int a;
		float b;
		char c;
	}s;

	s a;
	jfieldID F1;
	
	FILE *fp=fopen("/home/varun/NetBeansProjects/WebApplication1/build/web/WEB-INF/classes/test.txt","rb");
 	fread(&a,sizeof(s),1,fp);
	jclass cls=(*env)->GetObjectClass(env,jobj);
	F1 = (*env)->GetFieldID(env,cls,"a","I");
	(*env)->SetIntField(env,jobj,F1,a.a);
	F1 = (*env)->GetFieldID(env,cls,"b","F");
	(*env)->SetFloatField(env,jobj,F1,a.b);
	F1 = (*env)->GetFieldID(env,cls,"c","C");
	(*env)->SetCharField(env,jobj,F1,a.c);
 
}

