#include<netinet/in.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include<stdio.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include<fcntl.h>
#include<stdlib.h>
#include"changeStatus.h"
 
start_data * fill_start_data(const char *path,const char *interface,u_int32_t GFL,u_int16_t hash,u_int16_t gpkt)
{
	start_data *to_send = (start_data*)malloc(sizeof(start_data));
	strcpy(to_send->option,"START");
	strcpy(to_send->path,path);
	strcpy(to_send->interface,interface);
	to_send->GFL=GFL;
	to_send->hash=hash;
	to_send->gpkt=gpkt;
	return to_send;
}

start_recv* start_capture(start_data *to_send) 
{ 
	int sd,cd,no_char; 
	char buf[1000]=""; 
	start_recv *to_recv=(start_recv*)malloc(sizeof(start_recv)); 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("localhost",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	memcpy(buf,to_send,sizeof(start_data));
	write(sd,buf,1000);		
		//receive recomputed hash in case of collision
	read(sd,buf,1000);	
	free(to_send);
	memcpy(to_recv,buf,sizeof(start_recv));
	printf("\nReceived hash%u",to_recv->hash);
	close(sd);
	return to_recv; 
} 

int main()
{
	start_recv *to_recv=start_capture(fill_start_data("media/STORAGE/storage/hs1234/","wlan0",0,36,0));
	printf("\nNew hash is:%d",to_recv->hash);
}
