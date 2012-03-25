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


vm_info* get_vm_node(int vm_id,char *vm_name,char *vm_uuid,char *vm_type,char *vm_state,int vif_count,char vm_vif[][100])
{
	int i,j=0;
	vm_info *new=(vm_info*)malloc(sizeof(vm_info));

	new->vm_id=vm_id;
	strcpy(new->vm_name,vm_name);
	for(i=0;i<36;i++)
	{
		if(vm_uuid[i] > 0)
		{
			new->vm_uuid[j++]=vm_uuid[i];
		}
	}
	new->vm_uuid[j]='\0';
	//Change
	strcpy(new->vm_type,vm_type);
	strcpy(new->vm_state,vm_state);
	new->vif_count=vif_count;
	
	for(i=0;i<vif_count;i++)
	{
		strcpy(new->vm_vif[i],vm_vif[i]);
	}	
	//Change Ends
	new->next=NULL;
	return new;
}

void create_vm_nodes()
{
	vm_info *start=0,*end=0;	
       	int vm_id,i,vm_vif_count=0;
	char vm_name[20],vm_uuid[40],vm_type[30],vm_state[30],vm_vif[10][100];
	char vm_info_command[200],vm_vif_command[250];
	FILE *fp,*fp1,*fp2;

	//Retrieves the list of VM's running on the Hypervisor
	system("xm list | awk 'NR!=1 {printf\"%10s %10s  \",$2,$1;cmd=\"echo `virsh domuuid \"$1\"` | cut -d '-' -f1,2,3,4,5 --output-delimiter=''\";system(cmd)}' | sed -n '$!p' > VMLIST");
		
	fp=fopen("VMLIST","r");

		//Read one entry at a time and identify what to do
		while(fscanf(fp,"%d%s%s",&vm_id,vm_name,vm_uuid)!=EOF)
		{			
			vm_vif_count=0;
			//Code Change
			//Using the VM_NAME obtain info regarding state and ostype
			sprintf(vm_info_command,"%s%s%s","virsh dominfo ",vm_name," | sed -n '4,5p' | cut -d ':' -f2 | tr -d '\\n' > VM_DET");
			system(vm_info_command);
			fp1=fopen("VM_DET","r");
			
				fscanf(fp1,"%s%s",vm_type,vm_state);
				if(!strcmp(vm_state,"shut"))
					   vm_id=0;
				//Using VM_NAME to obtain the vif's belonging to it
				if(vm_id)
				{
					sprintf(vm_vif_command,"%s%s%s","xm network-list ",vm_name," | sed 1d | awk '{print $9}' | awk -F/ '{for(i=NF-2;i<=NF;i++){if(i==NF) printf(\".\"); printf(\"%s\",$i);} printf(\"\\n\");}'  > VM_VIF ");
	//				printf("%s\n",vm_vif_command);
					system(vm_vif_command);
					fp2=fopen("VM_VIF","r");

						vm_vif_count=0;
						while(fscanf(fp2,"%s",vm_vif[vm_vif_count])==1)
						{
								if(strcmp(vm_vif[vm_vif_count],"0")!=0)
									vm_vif_count++;						
						}
				
					fclose(fp2);
				}
				//Change Ends*/
			     	if(start==0)
				  start=end=get_vm_node(vm_id,vm_name,vm_uuid,vm_type,vm_state,vm_vif_count,vm_vif);
				else
				{
				  end->next=get_vm_node(vm_id,vm_name,vm_uuid,vm_type,vm_state,vm_vif_count,vm_vif);
		   		  end=end->next;
				}
				fclose(fp1);
			
			}
			fp1=fopen("/media/STORAGE/storage/VM_DETAILS","w");
			while(start!=NULL)
			{
				fwrite(start,sizeof(vm_info),1,fp1);
				start=start->next;
			}
			fclose(fp);
			fclose(fp1);
			system("rm -rf VMLIST");
			system("rm -rf VM_DET");
			system("rm -rf VM_VIF");
}


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
}



main()
{
	char vmnames[1][10]={"CentOS"};
	//create_dir(1235,vmnames,2);
	create_vm_nodes();
	//read_vm_info();
	printf("Exited with 0 exit status");
	return 0;
}







