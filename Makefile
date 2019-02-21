
# Makefile

# Type  make    to compile all the programs
# in the folder


program: 	 FTPclient \
	FTPserver  

clean: 
	rm -f 	FTPclient \
	FTPserver  


FTPclient: FTPclient.c
	gcc -g FTPclient.c -o FTPclient

FTPserver: FTPserver.c
	gcc -g FTPserver.c -o FTPserver
