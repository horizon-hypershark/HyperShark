#include <jni.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "testpackage_test3.h"

JNIEXPORT void JNICALL Java_testpackage_test3_loadFile
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
	//char cCurrentPath[100];	
	int errno=0;
	FILE *fp;
	a.a=10;
	a.b=12;
	a.c='A';

	/*if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(char)))
     	{
     		return errno;
     	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required 
	//printf ("The current working directory is %s", cCurrentPathFILE *fp=fopen("test.txt","w");
	fwrite(&a,sizeof(s),1,fp);
	fclose(fp);*/
	fp=fopen("test.txt","rb");
 	fread(&a,sizeof(s),1,fp);
	jclass cls=(*env)->GetObjectClass(env,jobj);
	F1 = (*env)->GetFieldID(env,cls,"a","I");
	(*env)->SetIntField(env,jobj,F1,a.a);
	F1 = (*env)->GetFieldID(env,cls,"b","F");
	(*env)->SetFloatField(env,jobj,F1,a.b);
	F1 = (*env)->GetFieldID(env,cls,"c","C");
	(*env)->SetCharField(env,jobj,F1,a.c);
	fclose(fp);
	
}

