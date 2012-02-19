#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "filelist.h"

file_list* get_file_node(char *filename)
{
	file_list *temp=(file_list*)malloc(sizeof(file_list));
	strcpy(temp->filename,filename);
	temp->next=0;
	return temp;
}

int cmp_time(int hr1,int min1,int hr2,int min2)
{
	int time1=hr1*60+min1;
	int time2=hr2*60+min2;
	if(time1<time2)
		return -1;
	else if(time1>time2)
		return 1;
	else
		return 0;
}

file_list* get_filename(capture_time *time_cap,char *path)
{
	int index;
	char command[150];
	char filename[20],open_path[100];
	int start_hr,end_hr,start_min,end_min;
	FILE *fp;
	file_list *file_start=0,*end=0,*tmp;

	sprintf(open_path,"%s%s",path,"BitMaps/");

	sprintf(command,"%s%s%s%d_%d%s","ls -t ",open_path," | grep bit",time_cap->month,time_cap->day," | cut -d '_' -f 3,4,5,6 --output-delimiter ' ' > tempp.txt");
//	printf("%s",command);
	system(command);
	fp=fopen("tempp.txt","r");
	while(fscanf(fp,"%d%d%d%d",&start_hr,&start_min,&end_hr,&end_min)==4)
	{
		if(!((cmp_time(start_hr,start_min,time_cap->start_hr,time_cap->start_min)==-1 && cmp_time(end_hr,end_min,time_cap->start_hr,time_cap->start_min)==-1) || (cmp_time(start_hr,start_min,time_cap->end_hr,time_cap->end_min)==1 && cmp_time(end_hr,end_min,time_cap->start_hr,time_cap->start_min)==1)))
		{
			sprintf(filename,"bit%d_%d_%d_%d_%d_%d",time_cap->month,time_cap->day,start_hr,start_min,end_hr,end_min);
			printf("\nfilename::%s is here",filename);
			if(file_start==0)
			{
				file_start=end=get_file_node(filename);
			}
			else
			{
				end->next=get_file_node(filename);
				end=end->next;
			}
		}	
	}
	fclose(fp);
	system("rm -f tempp.txt");	
 	//printf("%s","Filenames Extracted");
	return file_start;
}
