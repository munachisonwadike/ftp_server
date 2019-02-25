 
Brief description of Client-Server architecture:

I started the eserver using the select system call,
The client was done using the regular approach whereby it creates a socket over tcp and sends content to the socket. The server creates its own socket and reads from the sockets. Both the client and the server can read and write to the socket using the send and read calls. The default backlog of incoming connections to the server is 3 so in order to handle multiple connections, I had to use the select system call. The master socket was bound to the different ports on the server so that if there is a change in the the master socket, we know there is an incoming connection. 

...

