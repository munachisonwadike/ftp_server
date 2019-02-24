 
// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
    
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 

    //MY edit
    char outmsg[100] = {0};
    int port; char ip[15] = {0};
    port = atoi(argv[2]);
    sscanf(argv[1], "%s", ip);

    fflush(stdin);

    //
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

     
	send(sock , hello, strlen(hello) , 0 ); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n\n",buffer ); 
    fflush(stdin);

    while(1){
    	printf("ftp> ");
    	memset(outmsg, 0, sizeof(outmsg));
    	scanf("%s", outmsg);
	    send(sock , outmsg, strlen(outmsg) , 0 ); 
		memset(buffer, 0, sizeof(buffer));
	    valread = read( sock , buffer, 1024); 
	    printf("%s\n\n",buffer ); 
	    fflush(stdin);
	}
	return 0;
} 