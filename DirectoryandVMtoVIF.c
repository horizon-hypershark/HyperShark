#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

typedef struct vm_info
{
	u_int16_t vm_id;
	char vm_name[20];
	char vm_uuid[40];
	char vm_vif[10];
	struct vm_info *next;
}vm_info;


vm_info* get_vm_node(int vm_id,char *vm_name,char *vm_uuid)
{
	int i;
	vm_info *new=(vm_info*)malloc(sizeof(vm_info));
	new->vm_id=vm_id;
	strcpy(new->vm_name,vm_name);
	for(i=0;i<36;i++)
		new->vm_uuid[i]=vm_uuid[i];
	sprintf(new->vm_vif,"%s%d%s","vif",vm_id,".0");
	new->next=NULL;
	return new;
}

void create_vm_nodes()//vm_info *create_vm_nodes()
{

	vm_info *start=0,*end=0;	
	int vm_id,i;
	char vm_name[20],vm_uuid[40];
	//system("xm list | awk 'NR!=1 {printf\"%10s %10s  \",$2,$1;cmd=\"echo `virsh domuuid \"$1\"` | cut -d'-' -f1,2,3,4,5 --output-delimiter=''\";system(cmd)}' > VMLIST");
	
	FILE *fp=fopen("VMLIST.txt","r");
	
	while(fscanf(fp,"%d %s %s",&vm_id,vm_name,vm_uuid)!=EOF)
	{
		if(start==0)
			start=end=get_vm_node(vm_id,vm_name,vm_uuid);
		else
		{
			end->next=get_vm_node(vm_id,vm_name,vm_uuid);
			end=end->next;
		}	
	}

	while(start!=NULL)
	{
		printf("%d %s %s ",start->vm_id,start->vm_name,start->vm_vif);
		for(i=0;i<36;i++)
			printf("%c",start->vm_uuid[i]);
		printf("\n");
		start=start->next;
	}
	//return start;
}

/*
void create_dir(u_int32_t cust_id,char v_id[][10],int no_of_vms)
{
	int i;
	char *path=(char*)malloc(sizeof(char)*200);
	//To create Head Directory

	sprintf(path,"%s%s%s%d","mkdir ","/media/85A5-1381/","hs",cust_id);
	system(path);

	//To create Directories for VM id and subsequent nested folders
	for(i=0;i<no_of_vms;i++)
	{
		//Create Dir for VM name
		sprintf(path,"%s%s%s%d%s%s","mkdir ","/media/85A5-1381/","hs",cust_id,"/",v_id[i]);
		system(path);
		//SubFolders
		sprintf(path,"%s%s%s%d%s%s%s","mkdir ","/media/85A5-1381/","hs",cust_id,"/",v_id[i],"/Packets");
		system(path);
		sprintf(path,"%s%s%s%d%s%s%s","mkdir ","/media/85A5-1381/","hs",cust_id,"/",v_id[i],"/Flowrecords");
		system(path);
		sprintf(path,"%s%s%s%d%s%s%s","mkdir ","/media/85A5-1381/","hs",cust_id,"/",v_id[i],"/BitMaps");
		system(path);
	}
}*/




main()
{
//	char vmnames[2][10]={"CentOS","Fedora"};
//	create_dir(1235,vmnames,2);
//	printf("Exited with 0 exit status");
	create_vm_nodes();
	return 0;
}







