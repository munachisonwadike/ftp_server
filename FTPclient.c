#include <arpa/inet.h>
#include <dirent.h>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>

    
int main(int argc, char const *argv[]) 
{ 

    char arg[10] = {0};
    char buffer[1024] = {0}; 
    char cmd[5] = {0};
	char *hello = "Hello from client"; 
	char ip[15] = {0};
    char outmsg[20] = {0};

    DIR *currdir;

    int port;
    int reclen;
    int retval;
    int sendlen;
    int soc = 0;
    int valread;

    struct dirent* currdirent;  
    struct sockaddr_in cl_address; 
    struct sockaddr_in server_addr;
     
   
    port = atoi(argv[2]);
    sscanf(argv[1], "%s", ip);

 

    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n Socket creation error \n"); 
        fflush(stdin);
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
    	sendlen=0; reclen=0;
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
    		sendlen = (unsigned)strlen(outmsg);
    		send(soc, &sendlen, 4, 0  );
	  		send(soc, outmsg, sendlen, 0);
    		
    		// check what server said
    		memset(buffer, 0, sizeof(buffer));
		    valread = read( soc , buffer, 30); 

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
	    		sprintf(outmsg, "PASS %s", arg);

	    		// send username to server
	    		sendlen = (unsigned)strlen(outmsg);
	    		send(soc, &sendlen, 4, 0  );
		  		send(soc, outmsg, sendlen, 0);
	    			    	
				// read the server response 
				memset(buffer, 0, sizeof(buffer));
			    valread = read( soc , buffer, 13); 

			    // if says the password not okay, then ask for them again
			    if(strncmp("Pass", buffer, 4)!=0){
			    	printf("Password needed\n"); 
			    	continue;
			    }
			    // otherwise, display what is said on the screen (should be Password also okay)
			    printf("%s\n\n\n", buffer ); 
			    fflush(stdin);

			    // given user name and passowrd, should be able to run all other commands
 				while(1){
 					retval = 0;
 					sendlen=0; reclen=0;
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

 				  		scanf("%s", arg); 
						sprintf(outmsg, "CD %s\n", arg);

			    		// send cd command to server
			    		sendlen = (int)strlen(outmsg);

			    		send(soc, &sendlen, 4, 0  );
				  		send(soc, outmsg, sendlen, 0);  

				  		// read the server response 
						memset(buffer, 0, sizeof(buffer));
					    valread = read(soc , buffer, 4); 


					    // if says dir doesn't exist (-1), point this out
					    if(*buffer==-1){
					    	printf("Directory, '%s' does not exist.\n\n", arg); 
					    }else{
					    	printf("\n");
					    }
					// handle the remote ls command
					}else if (strncmp(cmd, "LS", 2)==0){ 
			    		// send ls to server 
 			    		sendlen = 2;
 			    		send(soc, &sendlen, 4, 0  );
				  		send(soc, "LS", sendlen, 0 );

						// read the server response to screen
						while( (valread = recv(soc, &reclen, 4, 0 )) != 0 ){
							if (reclen==0) break;
							memset(buffer, 0, sizeof(buffer));
						    valread = recv(soc, buffer, reclen, 0 );  
						    printf("~ /%s", buffer ); 
						    fflush(stdin);
						}
						printf("\n\n");
				    // handle the remote pwd command to display current directory
					}else if (strncmp(cmd, "PWD", 3)==0){ 
 			    		// send pwd to server 
 			    		sendlen = 3;
 			    		send(soc, &sendlen, 4, 0  );
				  		send(soc, "PWD", sendlen, 0 );

						// read the server response to screen
						memset(buffer, 0, sizeof(buffer));
						valread = recv(soc, &reclen, 4, 0 );  
					    valread = recv(soc, buffer, reclen, 0 );  
					    printf("%s\n\n", buffer ); 
					    fflush(stdin);
				    // handle the local ls command 
					}else if (strncmp(cmd, "!LS", 3)==0){ 
			    		// use pointer to needed directory
				    	currdir = opendir(".");

			    		if (currdir == NULL){
			    			printf("Cannot open directory\n\n");
			    		}

			    		// use readdir to send out the directory, clear buffer each time
			    		while(1){
			    			memset(buffer, 0, sizeof(buffer));

							if ( (currdirent= readdir(currdir)) != NULL){ 
					    		printf("%s\n", currdirent->d_name);
 				    		}else{
 				    			printf("\n\n");
					    		break;
				    		}

			    		} 
 			    		closedir(currdir);
 				    		
				    // handle the local pwd command to display current directory
					}else if (strncmp(cmd, "!PWD", 4)==0){ 
			    		// get the pwd  
			    		memset(buffer, 0, sizeof(buffer));
			    		getcwd(buffer, sizeof(buffer));
			    		printf("%s\n\n", buffer);   
	                    fflush(stdin);
					    continue;
					// handle the local cd command to change current directory
					}else if (strncmp(cmd, "!CD", 3)==0){ 
			    		// get the cd  
			    		scanf("%s", arg); retval = chdir(arg);
						if (retval!=0){
							printf( "Directory, '%s' does not exist.\n\n", arg );
						}else{
							printf("\n");
						}		 

					// quit the ftp process
					}else if (strncmp(cmd, "QUIT", 4)==0){ 
			    		printf("%s\n\n", "Goodbye!" ); 
					    fflush(stdin);
			    		close(soc);
			    		exit(1);
			    	} else{
						printf("Command not valid. \n\n"); 
						fflush(stdin);
						continue;
			    	}
				}
			// otherwise ask for password
			}else{
				printf("Need to provide both Username & Password!\n\n");
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
			scanf("%s", arg);
			printf("Please authenticate. \n\n"); 
			fflush(stdin);
			continue;
		}
	}
 

	return 0;
} 