#include<stdio.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define LOG_FILE "daemon.log"
#define LOCK_FILE "compress.lockd"

void log_message(char *filename,char *message)
{
  FILE *fp=fopen(filename,"a");
  if(!fp)
    return;
  fprintf(fp,"%s\n",message);
  fclose(fp);
}

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

daemon_compress(char *path)
{
  int i,lock;
  char *filename=(char*)malloc(150*sizeof(char));
  char *command=(char*)malloc(150*sizeof(char));
  char str[20];
  int GFL=0,fd;


  if(getppid()==1) return;//Already a daemon
  i=fork();
  if(i<0)exit(1);//Fork Error
  if(i>0)exit(0);//Parent exiting

  setsid();//Obtaining a New Process Group

  for(i=getdtablesize();i>=0;i--)//Closing all File Descriptors
    close(i);
  
  i=open("/dev/null",O_RDWR);//STDin
  dup(i);//STDout
  dup(i);//STDerr

  umask(027);

  chdir("/");

  lock=open(LOCK_FILE,O_RDWR|O_CREAT,0640);

  if(lock<0){exit(1);}

  if(lockf(lock,F_TLOCK,0)<0)exit(0);

  sprintf(str,"%d\n",getpid());
  write(lock,str,strlen(str));

  log_message(LOG_FILE,path);
 //printf("\nPath Name is :%s",argv[1]);

  signal(SIGCHLD,SIG_IGN); /* ignore child */
  signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  signal(SIGHUP,signal_handler); /* catch hangup signal */
  signal(SIGTERM,signal_handler);

  sprintf(filename,"%s%s",path,"Flowrecords/flowrec_0");
  sprintf(command,"lzop -U %s",filename);
  log_message(LOG_FILE,filename);
  //printf("Filename %s",filename);
  while(1)
  {
      log_message(LOG_FILE,"Entered Loop");
      if(fd=open(filename,O_RDONLY,0) < 0)
      {
	      sleep(5);
      }
      else
      {
	      log_message(LOG_FILE,filename);
              system(command);
	      log_message(LOG_FILE,"Executed System call");
	      log_message(LOG_FILE,"Compressed");
	      GFL+=1;
	      sprintf(filename,"%s%s%d",path,"Flowrecords/flowrec_",GFL);
	      log_message(LOG_FILE,filename);    
	      sprintf(command,"lzop -U %s",filename);
	      log_message(LOG_FILE,command);	      
      }
  }
}


      
