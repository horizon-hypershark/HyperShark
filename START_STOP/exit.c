#include<netinet/in.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include<stdio.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include<fcntl.h> 
void exit_capture() 
{ 
	int sd,cd;  
	struct sockaddr_in ser; 
	sd=socket(AF_INET,SOCK_STREAM,0); 
	if(sd<0) 
		printf("SOCKET NOT CREATED\n"); 
	bzero(&ser,sizeof(struct sockaddr_in)); 
	ser.sin_family=AF_INET; 
	ser.sin_port=htons(4444); 
	inet_aton("localhost",&ser.sin_addr); 
	connect(sd,(struct sockaddr *)&ser,sizeof(ser));
	write(sd,"EXIT",5);
	close(sd); 
} 

int main()
{
	exit_capture();	
}
