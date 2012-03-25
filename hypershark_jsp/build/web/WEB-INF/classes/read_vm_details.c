#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include"FileAccess_VMInfo.h"


typedef struct vm_info
{
	u_int16_t vm_id;
	int vif_count;
	char vm_name[20];
	char vm_uuid[40];
        char vm_type[20];
        char vm_state[20];  
	char vm_vif[5][10];
	struct vm_info *next;
}vm_info;



vm_info* read_vm_info()
{
	int i,j;
	vm_info *temp,*start=0,*end=0,*new;	
	FILE *fp=fopen("/storage/VM_DETAILS","r");
	
	temp=(vm_info*)malloc(sizeof(vm_info));
	while(fread(temp,sizeof(vm_info),1,fp)==1)
	{	
		j=0;
		if(start==0)
		{
			start=end=temp;		
			for(i=0;i<36;i++)
			{
				if(temp->vm_uuid[i]>0)
				{
					start->vm_uuid[j]=end->vm_uuid[j]=temp->vm_uuid[i];
					//printf("Value of j is %d and char is %c\n",j,start->vm_uuid[j]);
					j++;
				}
			}	
			start->vm_uuid[j]=end->vm_uuid[j]='\0';
		}
		else
		{
			end->next=temp;
			end=end->next;
			for(i=0;i<36;i++)
			{
				if(temp->vm_uuid[i]>0)
				{
					end->vm_uuid[j++]=temp->vm_uuid[i];
				}
			}	
			end->vm_uuid[j]='\0';
		}
		temp->next=0;
		temp=(vm_info*)malloc(sizeof(vm_info));
	}
	/*while(start!=NULL)
	{
//		fwrite(start,sizeof(start),1,fp1);
		printf("%d %s %s %s %s",start->vm_id,start->vm_name,start->vm_type,start->vm_state,start->vm_uuid);
		//for(i=0;i<36;i++)
		//	printf("%c is %d\n",start->vm_uuid[i],start->vm_uuid[i]);
		for(i=0;i<start->vif_count;i++)
			printf(" %s  ",start->vm_vif[i]);
		printf("\n");
		start=start->next;
	}*/
	return start;
	
}



JNIEXPORT void JNICALL Java_FileAccess_VMInfo_fillInfo
  (JNIEnv * env,jobject obj)// include packages in name
{
	vm_info *head;
	jstring str;	
	int i;
	jmethodID M1;
	jfieldID F1;
	jobject vm_obj;

	
	head=read_vm_info();	
	
	while(head!=NULL)
	{	
		jclass cls_main=(*env)->GetObjectClass(env,obj);
		
		//jni 
		jclass cls_vm=(*env)->FindClass(env,"Core/VirtualMachine");//find data class	
		
		//vm_obj=(*env)->AllocObject(env,cls_vm);
					
		M1=(*env)->GetMethodID(env,cls_vm,"<init>","()V");
		vm_obj=(*env)->NewObject(env,cls_vm,M1);

		//M1=(*env)->GetMethodID(env,cls_vm,"<init>","()V");
		//vm_obj=(*env)->NewObject(env,cls_vm,M1);
		
		F1 = (*env)->GetFieldID(env,cls_vm,"vmName","Ljava/lang/String;");
		str = (*env)->NewStringUTF(env,head->vm_name);//creating a string in format specified in jni
		(*env)->SetObjectField(env,vm_obj,F1,str);
			
		F1 = (*env)->GetFieldID(env,cls_vm,"vmType","Ljava/lang/String;");
		str = (*env)->NewStringUTF(env,head->vm_type);//creating a string in format specified in jni
		(*env)->SetObjectField(env,vm_obj,F1,str);
			
		F1 = (*env)->GetFieldID(env,cls_vm,"state","Ljava/lang/String;");
		str = (*env)->NewStringUTF(env,head->vm_state);//creating a string in format specified in jni
		(*env)->SetObjectField(env,vm_obj,F1,str);
	
		F1 = (*env)->GetFieldID(env,cls_vm,"vmId","Ljava/lang/String;");
		str = (*env)->NewStringUTF(env,head->vm_uuid);//creating a string in format specified in jni
		for(i=0;i<36;i++)
			printf("%c",head->vm_uuid[i]);
		printf("\n in c vmid is %s",head->vm_uuid);			
		(*env)->SetObjectField(env,vm_obj,F1,str);
	
		//printf("\n in c %d",head->vif_count);
			
		for(i=0;i<(head->vif_count);i++)
		{		
			
			printf("\n in c %s",head->vm_vif[i]);			
			M1=(*env)->GetMethodID(env,cls_vm,"addVif","(Ljava/lang/String;)V");
			str = (*env)->NewStringUTF(env,head->vm_vif[i]);//creating a string in format specified in jni
			(*env)->CallVoidMethod(env,vm_obj,M1,str);
		}		
		
		M1=(*env)->GetMethodID(env,cls_main,"addVM","(LCore/VirtualMachine;)V");
		(*env)->CallVoidMethod(env,obj,M1,vm_obj);
		
		head=head->next;	
	}			
}
