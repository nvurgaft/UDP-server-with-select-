#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/time.h>
#include "slist.c"
#define MAXLEN 4096

char* toUpper(char*);

int main(int argc, char **argv)
{
	slist_t *list = (slist_t*)malloc(sizeof(slist_t));
	slist_init(list);
	fd_set read_fds, write_fds;				// temp file descriptor list for select()
	int sockfd;								// socket descriptor
	struct sockaddr_in srv, cli_addr;		// used by bind()
	socklen_t clilen = sizeof(cli_addr);
	int nbytes, portno;	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0; 
	//check command line arguments 
	if (argc != 2) {
		fprintf(stderr, "Usage: ./server %s <port>\n", argv[1]);
		exit(1);
	}
	portno = atoi(argv[1]);
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		perror("ERROR opening socket");
		exit(1);
	}
	else
		printf("Server : Socket() successful\n");
	//bzero(&srv, sizeof(srv));
    
	/* bind: use the Internet address family */

	/* create the socket */
	srv.sin_family = AF_INET;
	/* bind: socket ‘sockfd’ to port portno*/
	srv.sin_port = htons(portno);
	/* bind: a client may connect to any of my addresses */
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
     
	if(bind(sockfd, (struct sockaddr*) &srv, sizeof(srv)) < 0){
		perror("ERROR on binding");
		close(sockfd);
		exit(1);
	}
	else
		printf("Server : bind() successful\n");
	//Now the UDP server is ready to accept packets…
    while(1){ 			
		fflush(stdout);
		char* buf = (char*)malloc(MAXLEN);						// message buf
		char* copybuf = (char*)malloc(MAXLEN);
		memset(buf, 0, strlen(buf));
		bzero(buf, MAXLEN);
		FD_ZERO(&read_fds);
		FD_SET(sockfd, &read_fds);
		int n = select(sockfd+1, &read_fds, &write_fds, 0, 0);
		if(n < 0)
		{
			perror("ERROR Server : select()\n");
			close(sockfd);
			exit(1);
		}
		if(FD_ISSET(sockfd, &read_fds)) {
			printf("Server is ready to read\n");
			nbytes = recvfrom(sockfd, buf, MAXLEN, 0, (struct sockaddr*)&cli_addr, &clilen);
			if (nbytes < 0){
				perror("ERROR in recvfrom()");
				close(sockfd);
				exit(1);
			}
		//	printf("Received packet from %s : %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		//	printf("Data: %s", buf);
			FD_CLR(sockfd, &read_fds);
		}
		if(strlen(buf) != 0){
			slist_append(list, buf);
			//printlist(list);				//you may see the list
			memcpy(copybuf, buf, strlen(buf));
			if(list == NULL){
				perror("List is empty. Server is not ready to write.");
				close(sockfd);
				exit(1);
			}
			else{
				FD_ZERO(&write_fds);
				FD_SET(sockfd, &write_fds);
				if(FD_ISSET(sockfd, &write_fds)){
					toUpper(copybuf);
					nbytes = sendto(sockfd, copybuf, MAXLEN, 0, (struct sockaddr*) &cli_addr, sizeof(cli_addr));
					if(nbytes < 0) {
						perror("ERROR in sendto");
						close(sockfd);
						exit(1);
					}
					printf("Server is ready to write\n");
					//printf("Send to client: %s", copybuf);
					//printf("==========================\n");
					printf("\n");
					memset(&copybuf, 0, sizeof(copybuf));
					FD_CLR(sockfd, &write_fds);
				}
			}
		}
		memset(&buf, 0, sizeof(buf));
	}
	close(sockfd);
	return 0;
}
char* toUpper(char* str){
	if(str != NULL){	
		int i;
		for(i = 0; i<strlen(str); i++)
		{
			if( str[i] <= 'z' && str[i] >= 'a')
			{
				str[i] += 'A' - 'a';
			}
		}
		return str;
	}
	return NULL;
}
