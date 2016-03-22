#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include "csapp.h"

// This program is a combination of echoclient.c, openlistenfd(), and an
// added GET request
int main(int argc, char *argv[])
{
	char *host;
	int port;
	char *page;

	if(argc != 4)
	{
		fprintf(stderr, "usage: %s <host> <port> <file>\n", argv[0]);
		exit(0);
	}

	host = argv[1];
	port = atoi(argv[2]);
	page = argv[3];
	
	int clientfd;
	int n;
	struct sockaddr_in serveraddr;
	struct hostent *hostp;
	
	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{ return -1; }
	
	if ((hostp = gethostbyname(host)) == NULL)
	{
		fprintf(stderr, "no such host\n");
		exit(0);
	}
	
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)hostp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hostp->h_length);
	serveraddr.sin_port = htons(port);
	if (connect(clientfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{ printf("Error connecting\n");}    
	
	// this is the new part to send the GET request:
	char sent[500];	// buffers for sent and received messages
	char rec[10000];
	
	// builds and sends message
	sprintf(sent, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page, host);
	write(clientfd, sent, strlen(sent));
	
	// receives and prints message
	while( (n = read(clientfd, rec, sizeof(rec)-1)) > 0)
	{ printf("\n%s\n", rec); }
	
	return 0;    
}
