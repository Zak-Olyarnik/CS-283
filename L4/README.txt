Jinu Jacob
CS 283-001
L4: File I/O

SUMMARY
--------------------------------------------------------------------------
client.c connects to a server using sockets and sends an HTTP request. It
then reads the result and displays it on the screen. Its input parameters 
are HOST, PORT, and FILE.

server.c starts a HTTP server. It accepts HTTP GET requests and serves
static content.

--------------------------------------------------------------------------


HOW TO RUN:
--------------------------------------------------------------------------
client.c and server.c can be compiled and run using the provided makefile.
The necessary source files have been provided including csapp.h and csapp.c
To run client use the following command:

HOST=www.google.com PORT=80 FILE=index.html make client

To run server use the following command:

PORT=80 make server

WARNING: the server program has run into infinite loops when trying to
connect to it from the client program. I was not able to reproduce this 
intentionally and debug in time. 

--------------------------------------------------------------------------

Compiled in VIM on tux


TESTING
--------------------------------------------------------------------------
To test client.c I used arguments such as HOST=www.google.com and
HOST=www.wikipedia.com and recieved 404's and 301's. PORT=80 and
FILE=index.html were used for all the tests. client was tested on tux as
well as another computer running Ubuntu 14.04

To test server, I started the server on PORT=8080. I then pasted the host 
ip address into my browser and server printed the correct response. When I
tried to use client to send an HTTP request to the server, the server began
to print blank lines infinitely. Also if I closed the browser while it was
still trying to connect to the server it would do the same thing. I had to
ctrl-c multiple times to stop it.


DESIGN/IMPLEMENTATION
--------------------------------------------------------------------------
For client.c I followed the example of echoclient.c. For sending 
the HTTP request I looked online. The program opens a socket and connects
to the server. If information is passed back and forth by reading and
writing to the socket file descriptor.

For server.c I followed the echoserver.c and tiny.c examples. I wrote out
most of the code that open_listenfd() who have done to get better
understanding of the socket programming. The helper functions are similar to
those in tiny.c but not as robust. server creates a socket descriptor and
socket address on the server. The bind function is used to associate the
two. In the main loop, the server waits for a connection request. It handles
the request appropriately using helper functions. It assumes only requests
for static content are sent.

