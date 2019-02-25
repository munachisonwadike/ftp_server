 
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
    char cmd[5] = {0};
    char arg[10] = {0};
    char outmsg[20] = {0};
    
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

    int x = -1;
    while(1){
    	x++;
    	printf("ftp> ");
    	fflush(stdin);
    	memset(cmd, 0, sizeof(cmd)); 
    	memset(arg, 0, sizeof(arg));
    	memset(outmsg, 0, sizeof(outmsg)); 
    	scanf("%s", cmd);
    	// authenticate User
    	if (strncmp(cmd, "USER", 4)==0){ 
    		scanf("%s", arg);
    		sprintf(outmsg, "USER %s", arg);
    		// send username to server
	  		send(soc, outmsg, strlen(outmsg), 0 );
    		
    		// check what server said
    		memset(buffer, 0, sizeof(buffer));
		    valread = read( soc , buffer, 1024); 

		    // if says the username not okay, then ask for them again
		    if(strncmp("User", buffer, 4)!=0){
		    	continue;
		    }
		    // otherwise, display what it said on the screen (should be Username ok pass required)
		    printf("%s\n\n", buffer ); 
		    fflush(stdin);

		    // ask for password
		    printf("ftp> ");
		    fflush(stdin);
		    memset(cmd, 0, sizeof(cmd)); 
	    	memset(arg, 0, sizeof(arg));
	    	memset(outmsg, 0, sizeof(outmsg)); 
	    	scanf("%s", cmd);
   			// get the password 
	    	scanf("%s", arg);
	    	// if the user gives password
    		if (strncmp(cmd, "PASS", 4)==0){ 
    			
				// scanf("%s", arg);

	    		// send password to server
		  		sprintf(outmsg, "PASS %s", arg);
				send(soc, outmsg, strlen(outmsg), 0 );

				// read the server response 
				memset(buffer, 0, sizeof(buffer));
			    valread = read( soc , buffer, 1024); 

			    // if says the password not okay, then ask for them again
			    if(strncmp("Pass", buffer, 4)!=0){
			    	printf("Password need\n"); 
			    	continue;
			    }
			    // otherwise, display what is said on the screen (should be Password also okay)
			    printf("%s\n\n\n", buffer ); 
			    fflush(stdin);

			    // given user name and passowrd, should be able to run all other commands
				while(1){
					printf("ftp> ");
			    	fflush(stdin);
			    	memset(cmd, 0, sizeof(cmd)); 
			    	memset(arg, 0, sizeof(arg));
		    		memset(outmsg, 0, sizeof(outmsg)); 
			    	scanf("%s", cmd);
					// handle the put command to put a file to remote from local
					if (strncmp(cmd, "PUT", 3)==0){ 
						// get the put file 
						scanf("%s", arg);
						sprintf(outmsg, "PUT %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the get command to get a file from remote
					}else if (strncmp(cmd, "GET", 3)==0){ 
			    		// get the get file 
						scanf("%s", arg);
						sprintf(outmsg, "GET %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the remote cd command
					}else if (strncmp(cmd, "CD", 2)==0){ 
			    		// get the cd file 
						scanf("%s", arg);
						sprintf(outmsg, "CD %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the remote ls command
					}else if (strncmp(cmd, "LS", 2)==0){ 
			    		// get the ls file 
						scanf("%s", arg);
						sprintf(outmsg, "LS %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the remote pwd command to display current directory
					}else if (strncmp(cmd, "PWD", 3)==0){ 
			    		// get the pwd file 
						scanf("%s", arg);
						sprintf(outmsg, "PWD %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the local ls command 
					}else if (strncmp(cmd, "!LS", 3)==0){ 
			    		// get the !ls file 
						scanf("%s", arg);
						sprintf(outmsg, "!LS %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
				    // handle the local pwd command to display current directory
					}else if (strncmp(cmd, "!PWD", 4)==0){ 
			    		// get the pwd file 
						scanf("%s", arg);
						sprintf(outmsg, "PWD %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// handle the local cd command to change current directory
					}else if (strncmp(cmd, "!CD", 3)==0){ 
			    		// get the !cd file 
						scanf("%s", arg);
						sprintf(outmsg, "!CD %s", arg);

			    		// send put to server
				  		send(soc, outmsg, strlen(outmsg), 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
					    valread = read( soc , buffer, 1024);  
					    printf("%s\n\n",buffer ); 
					    fflush(stdin);
					// quit the ftp process
					}else if (strncmp(cmd, "QUIT", 4)==0){ 
			    		printf("%s\n\n", "Goodbye!" ); 
					    fflush(stdin);
			    		close(soc);
			    		exit(1);
			    	} else{
						printf("Command not valid\n"); 
						continue;
			    	}
				}
			// otherwise ask for password
			}else{
				printf("Need to provide both Username & Password! %d\n\n", x);
				fflush(stdin); 
				continue;
			}

		// if we couldnt autheticate user, only allow to quit or ask to authenticate first
		}else if (strncmp(cmd, "QUIT", 4)==0){ 
    		printf("%s\n\n", "Goodbye!" ); 
		    fflush(stdin);
    		close(soc);
    		exit(1);
		
		// ask to authenticate first if not done
		}else{
			printf("Please authenticate\n"); 
			continue;
		}
	}
 

	return 0;
} 