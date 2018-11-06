#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void send_msg(int sockfd, int newport, int pid){
	char* buffer = (char*)calloc(128,sizeof(char));

	for(int i=0; i<5; i++){
		snprintf(buffer,128,"[%d] PID %d Iteration #%d\n",newport,pid,i);
		printf("Server sending: %s",buffer);
		if(write(sockfd,buffer,strlen(buffer)+1) < 0){
		//if(send(sockfd,buffer,strlen(buffer)+1,0) < 0){
		  perror("Failed to write to socket\n");
		}
		

		sleep(2);
	}
	if(shutdown(sockfd,2) != 0){
		perror("Failed to close socket\n");
	}
	else{
		printf("Closed the socket!\n");
	}
	free(buffer);
}

int main(int argc, char** argv){
	int sockfd, newsockfd, port, clilen, pid;
	struct sockaddr_in serv_addr, cli_addr;

	if(argc != 2){
		printf("Usage: ./server [port_num]\n");
		return -1;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(!sockfd){
		perror("Couldn't create a socket\n");
	}

	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&(int){1},sizeof(int));

	bzero((char *) &serv_addr, sizeof(serv_addr)); //Initialize the server address

	port = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("Failed to bind socket\n");
		return -1;
	}

	listen(sockfd,2);
	clilen = sizeof(cli_addr);
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0){
			perror("Failed to accept\n");
			return -1;
		}

		pid = fork();

		if(pid){
			send_msg(newsockfd,ntohs(((struct sockaddr_in*)&cli_addr)->sin_port),pid);
		}
	}

	return 0;
}
