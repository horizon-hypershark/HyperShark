#include <stdio.h>
#include "pf_kernel.h"
#include <stdlib.h>
#include <jni.h>
#include <string.h>
#include "FileAccess_GraphData.h"

typedef struct sip
{
	ip_addr ip_src;
	u_int32_t nop;
	struct sip *next;
	int check;
}sip;

typedef struct ip_list
{
	ip_addr ip_src;
	u_int32_t nop;
}ip_list;

//for source port
typedef struct srcpt
{
	u_int16_t port;
	u_int32_t nop;
	struct srcpt *next;
	int check;
}srcpt;

typedef struct srcpt_list
{
	u_int16_t port;
	u_int32_t nop;
}srcpt_list;
//src port structure ends



sip* get_node(flow_record fp)
{
	sip *temp;
	temp=(sip*)malloc(1*sizeof(sip));
	temp->ip_src=fp.ip_src;
	temp->nop=fp.nop;
	temp->next=NULL;
	temp->check=0;     
	return temp;
} // end of get_node

int check(flow_record fp,sip **head)
{
	int ret=0;
	while((*head)!=NULL)
	{
		if((*head)->ip_src.v4==fp.ip_src.v4)
		{
			(*head)->nop+=fp.nop;
			ret=1;
			break;
		}
		(*head)=(*head)->next;
	}
	return ret;
} // end of check

JNIEXPORT void JNICALL Java_FileAccess_GraphData_fillips
  (JNIEnv * env,jobject obj)// include packages in name
{

	ip_list top[5],t;
	int chk=0,count=1,i=1,j=1,num; 
	char *fname1,*fname2;
	char command[200];
	sip *s_ip=0,*temp=0,*temp1=0;
	flow_record flow_struct;
	void fillgraphobject(jclass,ip_list[],JNIEnv *,jobject);
	fname1=(char*)malloc(20*sizeof(char));
	fname2=(char*)malloc(70*sizeof(char));
		
	FILE *fp1,*fp; 

	system("ls /storage/hs1234/Flowrecords/ | grep flow.* > temp.txt");
	fp1=fopen("temp.txt","r");
	if(fp1==NULL)
	{
		printf("\nerror in opening file");
		return;
	}
	while(fscanf(fp1,"%s",fname1)!=EOF)
	{
		strcpy(fname2,"/storage/hs1234/Flowrecords/");
		strcat(fname2,fname1);
		sprintf(command,"%s%s","lzop -dU ",fname2);
		system(command);
		fname2[strlen(fname2)-4]='\0';		
		fp=fopen(fname2,"rb");
		if(fp==NULL)
		{		
			printf("\n error in opening file");
			return;
		}
		//printf("\nReading File %s\n",fname1);

		while((fread(&flow_struct,sizeof(flow_record),1,fp))==1)
		{
			if(s_ip==0)
			{
				s_ip=temp=get_node(flow_struct);
			}
			else
			{
				temp1=s_ip;
				chk=check(flow_struct,&temp1);
				if(chk==0)
				{        
					temp->next=get_node(flow_struct);
					temp=temp->next;
				}  
			}
					    
		} // end of while
		fclose(fp);
		sprintf(command,"%s%s","lzop -U ",fname2);
		system(command);		
	} // end of while
	fclose(fp1);

// sort the linked list to get top 5 ips

	temp=s_ip;
	i=0;
	while(i<5)
	{
		top[i].nop=-1;
		i++;
	}

	num=5;
	//SORTING OF THE LIST
	while(temp!=NULL)
	{
		for(i=0;i<num;i++)
		{
			if(temp->nop > top[i].nop)
			{
				for(j=num-1;j>i;j--)
				{
					top[j]=top[j-1];	
				}
				top[i].nop=temp->nop;
				top[i].ip_src=temp->ip_src;
				break;
			}
			if(top[i].nop==-1)
			{
				top[i].nop=temp->nop;
				top[i].ip_src=temp->ip_src;
				break;
			}			
		}
		temp=temp->next;
	}
	i=0;
	//printf("\nSorted\n");
	while(i<5)
	{
	   
	   //printf("\nIn c code SRC_IP=%u.%u.%u.%u",(top[i].ip_src.v4 >> 24) & 0xFF,(top[i].ip_src.v4 >> 16) & 0xFF,(top[i].ip_src.v4 >> 8) & 0xFF,(top[i].ip_src.v4) & 0xFF);   
	   //printf(" nop = %d",top[i].nop);
	   i++;
	}

	//jni code starts here	
	jclass cls_main=(*env)->GetObjectClass(env,obj);

	jfieldID F1;
	jintArray jint;	
	int iparr[5],freqarr[5];
	int k=0,countparams;

	F1 = (*env)->GetFieldID(env,cls_main,"totPoints","I");
	countparams=(*env)->GetIntField(env,obj,F1);
	while(k<countparams)
	{
		jmethodID M1=(*env)->GetMethodID(env,cls_main,"addStat","(II)V");
		(*env)->CallVoidMethod(env,obj,M1,top[k].ip_src.v4,top[k].nop);
		k++;
	}
	


} // end of main

void fillgraphobject(jclass cls_main,ip_list top[],JNIEnv *env,jobject obj)
{
	jfieldID F1;
	jintArray jint;	
	int *iparr,*freqarr;
	int i=0;

	while(i<5)
	{
		//*(iparr+i)=top[i].ip_src.v4;
		i++;
	}		
	i=0;
	while(i<5)
	{
		//*(freqarr+i)=top[i].nop;
		i++;
	}

}


//source port code



srcpt* get_node_src_port(flow_record fp)
{
	srcpt *temp;
	temp=(srcpt*)malloc(1*sizeof(srcpt));
	temp->port=fp.src_port;
	temp->nop=fp.nop;
	temp->next=NULL;
	temp->check=0;     
	return temp;
} // end of get_node

int check_src_port(flow_record fp,srcpt **head)
{
	int ret=0;
	while((*head)!=NULL)
	{
		if((*head)->port==fp.src_port)
		{
			(*head)->nop+=fp.nop;
			ret=1;
			break;
		}
		(*head)=(*head)->next;
	}
	return ret;
} // end of check

JNIEXPORT void JNICALL Java_FileAccess_GraphData_fillSrcPorts
  (JNIEnv * env,jobject obj)// include packages in name
{

	srcpt_list top[5],t;
	int chk=0,count=1,i=1,j=1,num; 
	char *fname1;
	srcpt *s_port=0,*temp=0,*temp1=0;
	flow_record flow_struct;
	//void fillgraphobject(jclass,ip_list[],JNIEnv *,jobject);
	fname1=(char*)malloc(20*sizeof(char));
	FILE *fp1,*fp; 

	system("ls | grep flow.* > srcport.txt");
	fp1=fopen("srcport.txt","r");
	if(fp1==NULL)
	{
		printf("\nerror in opening file");
		return;
	}
	
	while(fscanf(fp1,"%s",fname1)!=EOF)
	{
		fp=fopen(fname1,"rb");
		if(fp==NULL)
		{
			printf("\n error in opening file");
			return;
		}
		//printf("\nReading File %s\n",fname1);

		while((fread(&flow_struct,sizeof(flow_record),1,fp))==1)
		{
			if(s_port==0)
			{
				s_port=temp=get_node_src_port(flow_struct);
			}
			else
			{
				temp1=s_port;
				chk=check_src_port(flow_struct,&temp1);
				if(chk==0)
				{        
					temp->next=get_node_src_port(flow_struct);
					temp=temp->next;
				}  
			}
					    
		} // end of while
		fclose(fp);
	} // end of while
	fclose(fp1);

// sort the linked list to get top 5 ips

	temp=s_port;
	i=0;
	while(i<5)
	{
		top[i].nop=-1;
		i++;
	}

	num=5;
	//SORTING OF THE LIST
	while(temp!=NULL)
	{
		for(i=0;i<num;i++)
		{
			if(temp->nop > top[i].nop)
			{
				for(j=num-1;j>i;j--)
				{
					top[j]=top[j-1];	
				}
				top[i].nop=temp->nop;
				top[i].port=temp->port;
				break;
			}
			if(top[i].nop==-1)
			{
				top[i].nop=temp->nop;
				top[i].port=temp->port;
				break;
			}			
		}
		temp=temp->next;
	}
	i=0;
	//printf("\nSorted\n");
	while(i<5)
	{
	   
	   //printf("\nIn c code port is %d",top[i].port);
	   //printf(" nop = %d",top[i].nop);
	   i++;
	}

	//jni code starts here	
	jclass cls_main=(*env)->GetObjectClass(env,obj);

	jfieldID F1;
	jintArray jint;	
	int portarr[5],freqarr[5];
	int k=0,countparams;

	F1 = (*env)->GetFieldID(env,cls_main,"totPoints","I");
	countparams=(*env)->GetIntField(env,obj,F1);
	while(k<countparams)
	{
		jmethodID M1=(*env)->GetMethodID(env,cls_main,"addStat","(II)V");
		(*env)->CallVoidMethod(env,obj,M1,top[k].port,top[k].nop);
		k++;
	}

} // end of main

