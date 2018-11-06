#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void recv_msg(int sockfd){

	char buffer[256];
	sleep(5); //Why does this line create problems?

	while(recv(sockfd,buffer,256,0)>0){
		printf("%s",buffer);
	}
	printf("No more data\n");
}

int main(int argc, char** argv){

	int sockfd, port;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc != 2){
		printf("Usage: ./client [port_num]\n");
		return -1;
	}

	port = atoi(argv[1]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Failed to open socket\n");
		return -1;
	}

	server = gethostbyname("localhost");
	if (!server) {
		printf("Couldn't find localhost");
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){ 
		perror ("Error connecting to server\n");
		return -1;
	}

	recv_msg(sockfd);

	printf("Done\n");

	return 0;
}
