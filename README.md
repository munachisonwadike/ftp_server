need to inlcude the test cases I used to test the server to be sure it works along with putting the test cases in the folder
The IP addresses are ipv4
https://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t

The main difference between sockets and files is that you allow for connections- the socket is just like a real sucket so the data "plugs" in there and gest sent over the internet...

Make sure to use the "get protobyname" to get the TCP protocol in the setsockopt functions...

Outline for the day- socket part, the file change part, then documentation and cleaning.

Test case for FTP client

1)
ftp> USER user
ftp> -user USER
ftp> USER user

- This is to be sure that the after a good username a bad password would cause you to have to start again with the username

2)

Test cases for FTP server


<!-- 
git commit -m ""
git add *

git push

 -->