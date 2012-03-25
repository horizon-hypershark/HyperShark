#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<linux/pf_ring.h>
#include"pf_kernel.h"
#define UNIT_SIZE 1024

#define LOG_FILE "daemon.log"
#define LOCK_FILE "compress.lockd"
//-----------------------------------------------------------------------------------------------------------------//
void log_message(char *filename,char *message)
{
  FILE *fp=fopen(filename,"a");
  if(!fp)
    return;
  fprintf(fp,"%s\n",message);
  fclose(fp);
}
//-----------------------------------------------------------------------------------------------------------------//
void signal_handler(int sig)
{
  switch(sig){
  case SIGHUP:
    log_message(LOG_FILE,"Hangup Signal Caught");
    break;
  case SIGTERM:
    log_message(LOG_FILE,"Terminate Signal Caught");
    exit(0);
    break;
  } 
}
//---------------------------------------------------------------------------------------------------------//
//Function to compute space acquired in unit GB
double compute_used_space(char *path)
{
	int i,reduction=0;
	double space_consumed;
	char command[200],value[15],unit;
	FILE *fp;

	sprintf(command,"%s%s%s","du -h ",path," | tail -n 1 | awk '{printf $1}' > SIZE");
	system(command);

	fp=fopen("SIZE","r");
	fscanf(fp,"%s",value);

	//Extracting unit of storage
	unit=value[strlen(value)-1];

	//Only value remains
	value[strlen(value)-1]='\0';

	//Converting it to float value
	sscanf(value,"%lf",&space_consumed);

	switch(unit)
	{
		case 'K':
		{
			reduction=2;
			break;
		}
		case 'M':
		{
			reduction=1;
			break;
		}
		default:
		{ 
			reduction=0;
		}
	}

	for(i=0;i<reduction;i++)
		space_consumed/=UNIT_SIZE;

	return space_consumed;
}
//--------------------------------------------------------------------------------------------------------//
//Function to execute shell command and obtain output
void exec_system_call(char *cmd,char *result) 
{
	FILE* pipe = popen(cmd, "r");
	if (!pipe) {printf("error");};
	char buffer[128];

	while(!feof(pipe)) 
	{
		if(fgets(buffer, 128, pipe) != NULL)
			strcat(result,buffer);
	}
	pclose(pipe);
}

//----------------------------------------------------------------------------------------------------------//
//Function to obtain Packet trace file that is the oldest
int obtain_pkt_file(char *path)
{
	char command[200],pkt_file[40]="",*index;
	int file_index,i;
	FILE *fp;

	sprintf(command,"%s%s%s","ls -tr ",path,"Packets/ | head -n 1 > PKTFILE");
	system(command);

	//Obtain file name of the last modified packet filename
	fp=fopen("PKTFILE","r");
	fscanf(fp,"%s",pkt_file);
	log_message(LOG_FILE,pkt_file);	
	fclose(fp);
	//exec_system_call(command,pkt_file);

	//Obtain the first occurance of '_' and increment to obtain the place form where the number begins
	index=strchr(pkt_file,'_');
	index++;
	log_message(LOG_FILE,index);
	//Obtain pkttrace index and return it
	file_index=atoi(index);

	return file_index;
}

//-------------------------------------------------------------------------------------------------------------//
void remove_files(char *path,int file_index)
{
	char command[200],flow_file[40]="",file_path[100];
	char off_file[100],bitmap_file[100]="",*index;
	flow_record fr;
	FILE *fp;
	int fd_fr,flow_index;

	sprintf(command,"%s%s%s","ls -tr ",path,"Flowrecords/ | grep flow* | head -n 1 > FLOWFILE");

	fp=fopen("FLOWFILE","r");
	fscanf(fp,"%s",flow_file);
	log_message(LOG_FILE,flow_file);	
	fclose(fp);

	//Obtain the first occurance of '_' and increment to obtain the place form where the number begins
	index=strchr(flow_file,'_');
	index++;

	//Obtain pkttrace index and return it
	flow_index=atoi(index);

	while(1)
	{
		strcpy(bitmap_file,"");
		memset(&fr,0,sizeof(flow_record));
		//To form the file name to be opened and possibly deleted
		sprintf(file_path,"%s%s%d",path,"Flowrecords/flowrec_",flow_index);
		sprintf(off_file,"%s%s%d",path,"Flowrecords/offset_",flow_index);

		//To for the system command to decompress the file
		sprintf(command,"%s%s%s","lzop -dU ",file_path,".lzo");
		system(command);

		fd_fr=open(file_path,O_RDONLY,0);
		if(fd_fr!=-1)//File does not exist
		{
			read(fd_fr,&fr,sizeof(flow_record));
			if(fr.nop!=0)
			{
				if(fr.pkt_file_no == file_index)
				{
					close(fd_fr);
					sprintf(command,"%s%s","rm -rf ",file_path);//For the flowrec file
					log_message(LOG_FILE,command);
					system(command);
					sprintf(command,"%s%s","rm -rf ",off_file);//For the offset file
					log_message(LOG_FILE,command);
					system(command);
					sprintf(command,"%s%s%s%d%s","ls -tr ",path,"BitMaps/ | grep '\\<.*_",flow_index,"\\>' > BITFILE");
					log_message(LOG_FILE,command);
					fp=fopen("BITFILE","r");
					fscanf(fp,"%s",bitmap_file);
					log_message(LOG_FILE,bitmap_file);	
					fclose(fp);
//					exec_system_call(command,bitmap_file);
					log_message(LOG_FILE,bitmap_file);
					sprintf(command,"%s%s%s%s","rm -rf ",path,"BitMaps/",bitmap_file);
					system(command);
					flow_index++;
				}
				else
				{
					close(fd_fr);
					sprintf(command,"%s%s","lzop -U ",file_path);
					log_message(LOG_FILE,command);
					system(command);
					break;
				}	
			}
			else
			{
				log_message(LOG_FILE,"Code Must Exit now");
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------//
//Function to compress files
void compress(char *path)
{
	FILE *fp;
	char command[200],filename[50];
      	sprintf(command,"%s%s%s","ls ",path,"Flowrecords/ | grep \"flow\" |grep -v \".lzo\" > COMPRESSLIST");
	system(command);
	fp=fopen("COMPRESSLIST","r");
	while(fscanf(fp,"%s",filename)==1)
	{
		sprintf(command,"%s%s%s%s","lzop -U ",path,"Flowrecords/",filename);
	        log_message(LOG_FILE,command);
		system(command);
	}
	fclose(fp);  
}
//-----------------------------------------------------------------------------------------//

main(int argc, char *argv[])
{
  int i,lock;
  char *filename=(char*)malloc(150*sizeof(char));
  char *filename1=(char*)malloc(150*sizeof(char));
  char *path=(char*)malloc(150*sizeof(char));
  char pid_file_name[200];
  char str[20];
  int GFL=0,fd,file_index,MAX_LIMIT;
  double space;

  if(getppid()==1) return;//Already a daemon
  i=fork();
  if(i<0)exit(1);//Fork Error
  if(i>0)exit(0);//Parent exiting

  setsid();//Obtaining a New Process Group

  sprintf(pid_file_name,"%s%s%s","compress_",argv[3],".lockd");

  for(i=getdtablesize();i>=0;i--)//Closing all File Descriptors
    close(i);
  
  i=open("/dev/null",O_RDWR);//STDin
  dup(i);//STDout
  dup(i);//STDerr

  umask(027);

  chdir("/");

  lock=open(pid_file_name,O_RDWR|O_CREAT,0640);

  if(lock<0){exit(1);}

  if(lockf(lock,F_TLOCK,0)<0)exit(0);

  sprintf(str,"%d\n",getpid());
  write(lock,str,strlen(str));

  log_message(LOG_FILE,argv[1]);

  signal(SIGCHLD,SIG_IGN); /* ignore child */
  signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  signal(SIGHUP,signal_handler); /* catch hangup signal */
  signal(SIGTERM,signal_handler);

  strcpy(path,argv[1]);
  MAX_LIMIT=atoi(argv[2]);
 
  while(1)
  {
      log_message(LOG_FILE,"Entered Loop");

      //Daemon to compress files
      compress(path);	

      //Daemon for deleting files when exceeded limit
      space=compute_used_space(path);
      log_message(LOG_FILE,path);
      if(space >= MAX_LIMIT)
      {
		//Obtain the Packet Trace file which is the oldest in terms of modification time
		file_index=obtain_pkt_file(path);
		//To identify flow,offset and bitmap files based on pkt file index and remove them
		remove_files(path,file_index);
		log_message(LOG_FILE,"reached here and next gonna continue in loop");
		sleep(5);
      }
      else
      {	
		sleep(5);
      }
  }
}


      
