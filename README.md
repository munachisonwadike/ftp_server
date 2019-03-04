 
The server must be running before the client in order for the client to be able to connect to it. 

The client initiates a connection via a tcp socket to the port the server is listening on. When the server recieved the requests to connect on its master socket, it creates another socket child to handle the request. This is the control connection.

The default maximum backlog of incoming connections to the server is three so in order to handle multiple connections, we implement the select system call. 

The LS, PWD, CD, and USER and PASS commands and their outputs are communicated over the control connection. However, When there is a request for a file transfer using either GET or POST, the server initiates another connection for the data stream, and the client accepts. When the data stream is ended, a small notification is provided.


At present, becuase the server is running only on the local host, you would need to start the client using "./FTPclient 127.0.0.1 8888". Where 8888 is the port number the server is listening on.
 

