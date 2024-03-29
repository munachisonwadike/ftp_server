#include <arpa/inet.h> 
#include <dirent.h>
#include <errno.h> 
#include <netinet/in.h>
#include <netdb.h> 
#include <stdio.h>  
#include <stdlib.h>      
#include <string.h>    
#include <sys/socket.h>  
#include <sys/types.h>  
#include <sys/time.h>  
#include <unistd.h>     
 
     
int main(int argc , char *argv[]){   
    char a;
    char buffer[1025];  //for storing incoming data  
    char cmd[5]; 	//for storing file name
	char filenm[40]; 	//for storing file name
    char filenm2[40] = {0};
	char *message = "Munachiso's FTP Server 0.1 \r\n";  //welcome message 

    DIR *currdir;
    FILE *fp;

	int activity, addrlen, strmaddrlen;
    int clsocs[30];
    int mastsoc;
    int maxsd;
    int newsoc;
    int reclen;
    int retval; 
    int sd; 
    int valread;  
    
    int opt = 1;
    int port = 8888;
    int port2 = 8080; 
	int maxcls = 30;
    
    struct sockaddr_in address; 
    struct sockaddr_in strmaddr; 
    struct dirent* currdirent;  
    unsigned int buflen = 0;
         

    
    //set of socket descriptors and in an array as well so we can loop through them
    fd_set readfds;    
    for (int i = 0; i < maxcls; i++){   

        clsocs[i] = 0;   
    }    
      
         
    //master socket  
    if( (mastsoc = socket(AF_INET , SOCK_STREAM , getprotobyname("TCP")->p_proto)) == 0){   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //master socket should allow multiple connections 
    if( setsockopt(mastsoc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){ 

        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //setting address
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons(port);   
         
    //binding socket to localhost port 8888  
    if (bind(mastsoc, (struct sockaddr *)&address, sizeof(address))<0){   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listening on port %d \n", port);   
         
    //max of three pending connections for master socket
    if (listen(mastsoc, 3) < 0){   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
    addrlen = sizeof(address);   
    strmaddrlen = addrlen;
    //accept the incoming connections
    puts("Waiting for connections ...");       
    while(1){   
    	reclen = 0; retval =0;
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(mastsoc, &readfds);   
        maxsd = mastsoc;   
             
        //add child sockets to set  
        for (int i = 0; i < maxcls; i++){   
            //socket descriptor  
            sd = clsocs[i];   
                 
            //if valid socket descriptor, add to read list  
            if(sd > 0)   
                FD_SET(sd, &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > maxsd)   
                maxsd = sd;   
        }   
     
        //wait indefinitely for an activity on one of the sockets (timeout==NULL here) 
        activity = select( maxsd + 1 , &readfds , NULL , NULL , NULL);   
        if ((activity < 0) && (errno!=EINTR)){   
            printf("select error");   
        }   
             
        //if something happened on the master socket, then its an incoming connection so create new socket
        

        if (FD_ISSET(mastsoc, &readfds)){   

	        if ((newsoc = accept(mastsoc, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){   
	            perror("accept");   
	            exit(EXIT_FAILURE);   
	        }   
	         
	        //inform user of new socket number    
	        printf("New connection, socket fd is %d, ip is: %s, port: %d\n" ,
	        		newsoc , inet_ntoa(address.sin_addr) , ntohs 
	              (address.sin_port));   
	       
	        //send new client a greeting message  
	        if( send(newsoc, message, strlen(message), 0) != strlen(message) ){   
	            perror("send");   
	        }   
	        puts("Welcome message sent successfully");   
	             
	        //add the new socket to the client socket array  
	        for (int i = 0; i < maxcls; i++){   
	            //if position is empty  
	            if( clsocs[i] == 0 ){   
	                clsocs[i] = newsoc;   
	                printf("Adding to list of sockets as %d\n" , i);   
	                     
	                break;   
	            }   
	        }   

	       
        }else{
	 	    //else IO on another socket
	        for (int i = 0; i < maxcls; i++){   
	        	reclen=0; retval=0;
	            sd = clsocs[i];   
	                 
	            if (FD_ISSET(sd, &readfds)){   
   	                //kill two birds with one stone by reading the message and checking if it was a close request
	                if ((valread = read(sd, &reclen, 4)) == 0){   
	                    //Somebody disconnected , get his details and print  
	                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);   
	                    printf("Host disconnected, ip %s, port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));   
	                    fflush(stdin);
	                    //Close the socket and mark as 0 in list for reuse  
	                    close(sd);   
	                    clsocs[i] = 0;   
	                //if it wasnt to close, check what it was and respond appropriately
	                }else{   	    
		                valread = read(sd, buffer, reclen);   
	                    if (strncmp(buffer, "USER", 4)==0){ 
				    		send(sd, "Username OK, password required", 30 , 0 );
				    	}else if (strncmp(buffer, "PWD", 3)==0){
				    		//clear buffer and use getcwd to put the current directory there
					    	memset(buffer, 0, sizeof(buffer)); 
				    		getcwd(buffer, sizeof(buffer));

				    		buflen = (int)strlen(buffer);
				    		buflen++; 
				    		//make sure buffer terminates in NULL char
				    		buffer[buflen] = '\0';
				    		send(sd, &buflen, 4, 0 );	
				    		send(sd, buffer, buflen, 0 );

				    	}else if (strncmp(buffer, "PUT", 3)==0){

 							//get the PUT file
							memset(cmd, 0, sizeof(cmd)); 
					    	memset(filenm, 0, sizeof(filenm));
							sscanf(buffer,"%s %s", cmd, filenm);

 							if(strncmp(filenm, "-1", 2)==0){
								continue;
							}
							//open socket to use
							if ((newsoc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
					        	printf("\n Put-Socket creation error \n"); 
					        }
						    
						    //get the details of the client socket
					        getpeername(sd, (struct sockaddr*)&strmaddr, (socklen_t*)&strmaddrlen);   

					        //change the port to the one to send to
					        strmaddr.sin_port = htons(port2); 

 					        //open up the connection
					        while( (connect(newsoc, (struct sockaddr *)&strmaddr, sizeof(strmaddr)) < 0) ){ 
						        printf("\n Starting put datastream connection... \n"); 
 
							} 
							printf("Connected\n");
								

							//read the content of data stream using a while loop and send to file
							sprintf(filenm2, "%s%s\n", filenm, "(1)");
							fp = fopen(filenm2, "w"); 
							while(1){

								valread = read(newsoc, &a, 4); 
								if(a==-1) break;
   							    putc((char)a, fp);
   							    fflush(fp);    																
							
							}	
							//indicate the end of the file transfer and close socket
							close(newsoc);

							continue;
					

							
				    	}else if (strncmp(buffer, "LS", 2)==0){
				    		//use pointer to needed directory
				    		currdir = opendir(".");

				    		if (currdir == NULL){
				    			printf("Cannot open directory\n\n");
				    		}

				    		//use readdir to send out the directory, clear buffer each time
				    		while(1){
				    			memset(buffer, 0, sizeof(buffer));

								if ( (currdirent= readdir(currdir)) != NULL){ 
 					    			sprintf(buffer, "%s\n", currdirent->d_name);
					    			buflen = (int)strlen(buffer);
					    			
					    			buflen++; 

					    			buffer[buflen] = '\0';
					    			send(sd, &buflen, 4, 0 );	
					    			send(sd, buffer, buflen, 0 );	

  								//if we reach the end of the directory, send a zero bit so the client knows
					    		}else{
					    			buflen = 0;
					    			send(sd, &buflen, 4, 0 );
 					    			break;
					    		}

				    		}  
				    		//close the dir pointer
				    		closedir(currdir);
 				    		
 						}else if (strncmp(buffer, "GET", 3)==0){
				    		 

 							//get the GET command and file name
 							//put the file name into a path
							memset(cmd, 0, sizeof(cmd)); 
					    	memset(filenm, 0, sizeof(filenm));
							sscanf(buffer,"%s %s", cmd, filenm);

							//check if the file exists

							fp = fopen(filenm, "r"); 

					    	fflush(stdin);
							//if doesnt exist, let the client know
							if(fp==NULL){
								retval = -1;
								memset(buffer, 0, sizeof(buffer)); 
 								send(sd, &retval, 4, 0 );
 								continue;
 							//if it does, open a new socket and connect to client to send data
							}else{
								//let client know of incoming connection
								retval = 0;
								memset(buffer, 0, sizeof(buffer)); 
 								send(sd, &retval, 4, 0 );

 								//open socket to use
 								if ((newsoc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
							        printf("\n Get-Socket creation error \n"); 
 
							    } 
							    //get the details of the client socket
						        getpeername(sd, (struct sockaddr*)&strmaddr, (socklen_t*)&strmaddrlen);   

						        //change the port to the one to send to
						        strmaddr.sin_port = htons(port2); 

						        //connect to the port
						        while( (connect(newsoc, (struct sockaddr *)&strmaddr, sizeof(strmaddr)) < 0) ){ 
							        printf("\n Starting datastream connection... \n"); 
     
    							} 
    							printf("Connected\n");
   								
   								//write a simple test sentence to the port
   								while(1){
   									a =  getc(fp);
   									if(a==-1){
   										send(newsoc, &a, 4, 0);
   										break;
   									}
   									send(newsoc, &a, 4, 0);
   									
								}	
   								//indicate the end of the file transfer and close socket
   								close(newsoc);

 								continue;
							}

 						}else if (strncmp(buffer, "CD", 2)==0){ 
 					    	memset(cmd, 0, sizeof(cmd)); 
					    	memset(filenm, 0, sizeof(filenm)); 

							//put the command into a path and change dir to that path
							sscanf(buffer,"%s %s", cmd, filenm);
 							retval = chdir(filenm);
 							if(retval==0){
 								printf("\nSwitched to directory /%s\n\n", filenm);
 								fflush(stdin);
 								send(sd, &retval, 4, 0);
 							}
 							else{
 								memset(buffer, 0, sizeof(buffer)); 
 								send(sd, &retval, 4, 0 );	 
 							}
 							
				    	}else if (strncmp(buffer, "PASS", 4)==0){ 
				    		send(sd, "Password okay", 13 , 0 );
				    	}

	                       
	                }   
	            }   

	        }

	    }


    }   
         
    return 0;   
}   