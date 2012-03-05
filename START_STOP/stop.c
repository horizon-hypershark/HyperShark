#include<netinet/in.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include<stdio.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include<fcntl.h> 
#include<stdlib.h>
#include"changeStatus.h"

stop_data * fill_stop_data(u_int16_t hash)
{
	stop_data *to_send = (stop_data*) malloc(sizeof(stop_data));
	strcpy(to_send->option,"STOP");
	to_send->hash=hash;
	return to_send;
}

stop_recv* stop_capture(stop_data *to_send) 
{ 
	int sd,cd,no_char; 
	char buf[1000]="";
	stop_recv *to_recv=(stop_recv*)malloc(sizeof(stop_recv)); 
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("localhost",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));	
	memcpy(buf,to_send,sizeof(stop_data));
	write(sd,buf,1000);
	read(sd,buf,1000);
	close(sd); 
	memcpy(to_recv,buf,sizeof(stop_recv));
	free(to_send);
	return to_recv;
} 


int main()
{
	stop_recv *recv=stop_capture(fill_stop_data(36));
	printf("\nGFL is::%u gpkt is %u",recv->GFL,recv->gpkt);
}
