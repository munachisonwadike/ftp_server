//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
 
     
int main(int argc , char *argv[]){   
    int opt = 1;   
    int mastsoc , addrlen , newsoc , clsocs[30], maxcls = 30;
    int activity , valread , sd;   
    int maxsd;   
    int port = 8888;
    struct sockaddr_in address;   
         
    char buffer[1025];  //data buffer of 1K  
         
    //set of socket descriptors and in an array as well so we can loop through them

    fd_set readfds;    
    for (int i = 0; i < maxcls; i++){   
        clsocs[i] = 0;   
    }  

    // welcome message
    char *message = "Munachiso's FTP Server 0.1 \r\n";   
     
      
         
    //master socket  
    if( (mastsoc = socket(AF_INET , SOCK_STREAM , getprotobyname("TCP")->p_proto)) == 0){   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //master socket should allow multpile connections 
    if( setsockopt(mastsoc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 ){   
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
    printf("Listener on port %d \n", port);   
         
    //max of three pending connections for master socket
    if (listen(mastsoc, 3) < 0){   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
    addrlen = sizeof(address);   
    //accept the incoming connections
    puts("Waiting for connections ...");       
    while(1){   
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
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > maxsd)   
                maxsd = sd;   
        }   
     
        //wait indefinitely for an activity on one of the sockets (timeout==NULL here) 
        activity = select( maxsd + 1 , &readfds , NULL , NULL , NULL);   
        if ((activity < 0) && (errno!=EINTR)){   
            printf("select error");   
        }   
             
        //If something happened on the master socket, then its an incoming connection so create new socket
        

        if (FD_ISSET(mastsoc, &readfds)){   

	        if ((newsoc = accept(mastsoc, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){   
	            perror("accept");   
	            exit(EXIT_FAILURE);   
	        }   
	         
	        //inform user of new socket number    
	        printf("New connection , socket fd is %d , ip is : %s , port : %d\n" ,
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
	            sd = clsocs[i];   
	                 
	            if (FD_ISSET(sd, &readfds)){   
   	                //kill two birds with one stone by reading the message and checking if it was a close request
	                if ((valread = read(sd, buffer, 1024)) == 0){   
	                    //Somebody disconnected , get his details and print  
	                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);   
	                    printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));   
	                    //Close the socket and mark as 0 in list for reuse  
	                    close(sd);   
	                    clsocs[i] = 0;   
	                // if it wasnt to close, check what it was and respond appropriately
	                }else{   
	                    //set the string terminating NULL byte on the end  
	                    //of the data read  
	                    buffer[valread] = '\0';   
	                    if (strncmp(buffer, "USER", 4)==0){ 
				    		send(sd, "Username OK, password required", 30 , 0 );
				    	}else if (strncmp(buffer, "PWD", 3)==0){

					    	memset(buffer, 0, sizeof(buffer)); 
				    		getcwd(buffer, sizeof(buffer));
				    		printf("\n\nI got a PWD REQUEST @ path-%s\n\n", buffer );
				    		send(sd, buffer, sizeof(buffer) , 0 );
				    	}else if (strncmp(buffer, "PASS", 4)==0){ 
				    		send(sd, "Password okay", 30 , 0 );
				    	}

	                       
	                }   
	            }   

	        }

	    }


    }   
         
    return 0;   
}   