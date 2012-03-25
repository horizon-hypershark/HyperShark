#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

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
	FILE *fp=fopen("/media/STORAGE/storage/VM_DETAILS","r");
	
	temp=(vm_info*)malloc(sizeof(vm_info));
	while(fread(temp,sizeof(vm_info),1,fp)==1)
	{	
		j=0;
		if(start==0)
		{
			start=end=temp;		
		}
		else
		{
			end->next=temp;
			end=end->next;
		}
		temp->next=0;
		temp=(vm_info*)malloc(sizeof(vm_info));
	}
	while(start!=NULL)
	{
		printf("%d %s %s %s %s",start->vm_id,start->vm_name,start->vm_type,start->vm_state,start->vm_uuid);
		for(i=0;i<start->vif_count;i++)
			printf(" %s  ",start->vm_vif[i]);
		printf("\n");
		start=start->next;
	}
	return start;
}


main()
{
	vm_info *head;
	head=read_vm_info();	
	return 0;
}
