 
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
    struct sockaddr_in cl_address; 
    struct sockaddr_in server_addr;
    int soc = 0; 
    char *hello = "Hello from client"; 

    //MY edit
    char outmsg[5] = {0};
    char outmsg1[10] = {0};
    int port; char ip[15] = {0};
    port = atoi(argv[2]);
    sscanf(argv[1], "%s", ip);

    fflush(stdin);

    //
    int valread;

    char buffer[1024] = {0}; 
    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&server_addr, '0', sizeof(server_addr)); 
   
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(soc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

     
    valread = read( soc , buffer, 1024); 
    printf("%s\n\n", buffer ); 
    fflush(stdin);

    while(1){
    	printf("ftp> ");
    	fflush(stdin);
    	memset(outmsg, 0, sizeof(outmsg)); memset(outmsg1, 0, sizeof(outmsg1));
    	scanf("%s", outmsg);
    	// authenticate User
    	if (strncmp(outmsg, "USER", 4)==0){ 
    		scanf("%s", outmsg);
    		printf("ftp> Thanks, now please include password\n\n");
    		printf("ftp> ");
    		scanf("%s", outmsg1);
    		if (strncmp(outmsg1, "PASS", 4)==0){ 
    			// get the passowrd and send both user and pass to server
				scanf("%s", outmsg1);
				send(soc, outmsg, strlen(outmsg) , 0 );
  		  		send(soc, outmsg1, strlen(outmsg1) , 0 );
				
				// read the server response out to screen
				memset(buffer, 0, sizeof(buffer));
			    valread = read( soc , buffer, 1024); 
			    printf("About to print buffer content\n"); 
			    fflush(stdin);
			    printf("%s\n\n",buffer ); 
			    fflush(stdin);

			    // given user name and passowrd, should be able to run all other commands
				while(1){
					printf("ftp> ");
			    	fflush(stdin);
			    	memset(outmsg, 0, sizeof(outmsg)); memset(outmsg1, 0, sizeof(outmsg1));
			    	scanf("%s", outmsg);
					// handle the put command to put a file to remote from local
					if (strncmp(outmsg, "PUT", 3)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the get command to get a file from remote
					}else if (strncmp(outmsg, "GET", 3)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the remote cd command
					}else if (strncmp(outmsg, "CD", 2)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the remote ls command
					}else if (strncmp(outmsg, "LS", 2)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the remote pwd command to display current directory
					}else if (strncmp(outmsg, "PWD", 3)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc , outmsg, strlen(outmsg) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the local ls command 
					}else if (strncmp(outmsg, "!LS", 3)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the local pwd command to display current directory
					}else if (strncmp(outmsg, "!PWD", 4)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc , outmsg, strlen(outmsg) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the local cd command to change current directory
					}else if (strncmp(outmsg, "!CD", 3)==0){ 
			    		scanf("%s", outmsg1);
			    		send(soc, outmsg1, strlen(outmsg1) , 0 );
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024); 
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// quit the ftp process
					}else if (strncmp(outmsg, "QUIT", 4)==0){ 
			    		printf("%s\n\n", "Goodbye!" ); 
					    fflush(stdin);
			    		close(soc);
			    		exit(1);
			    	}
				}
			}

		// if we couldnt autheticate user, only allow to quit or ask to authenticate first
		}else if (strncmp(outmsg, "QUIT", 4)==0){ 
    		printf("%s\n\n", "Goodbye!" ); 
		    fflush(stdin);
    		close(soc);
    		exit(1);
		
		// ask to authenticate first if not done
		}else{
			printf("Please authenticate Username and Password\n"); 
		    fflush(stdin);
		}
	}

	// break into cases for USER
	// PASS
	// PUT
	// GET
	// CD
	// LS PWD
	// !CD !LS !PWD
	// QUIT

	return 0;
} 