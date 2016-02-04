# UDP-server-with-select-
UDP server with select() function synchronous I/O multiplexing 
Home Assignment 4: UDP sockets with Select
----------------------------------------

Info
----
login: 			alisara
personal details: 	Alisa Rahlina, ID: 327153466
exercise name: 	communication applications, exercise No.4
submitted files:	server.c, slist.c, slist.h, README 

Submitted Files
---------------
server.c - implementation (in C) the main function for UDP server that get requests from the clients with select function.
slist.c - implementation(s) of functions of slist.h file (in C) to crate and initialize the linked list that we use to save read messages
slist.h - the header file for slist.c 
README - This file.

Special Cases
-------------
For testing the function in this program you may use command line with to running a server.c and client.c files. The server should be run like this:
./serverPORT <S> <>
s terminating) to the client. 
<port> - the port number should be a between 1-65535.
The client reads a message from the command line, takes a required argument <erver-Host-Name> and port number(<port>):
erver-Host-Name> - the address of the UDP server that the client send requests. You can assume that the maximum length of a message is4096.
How to compile
------------
You can compile the server.c file by using the makefile with lines:
all:
	gcc -o server server.c -Wall
