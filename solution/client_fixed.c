//Partially stolen from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <netdb.h>

#define SYS_read_lines_array 335

void do_client(int sockfd,FILE* logfd){
	char buffer[256];
	ssize_t nbytes = 0;
	sleep(5); //Why does this line create problems? Can we get around it?

	while(syscall(SYS_read_lines,sockfd,buffer,256)>0){
		fprintf(logfd,"%s",buffer);
	}
	printf("No more data\n");
}

int main(int argc, char** argv){

	int sockfd, port;
	FILE* logfd;

	if(argc != 3){
		printf("Correct usage is %s [port] [logfile]\n",argv[0]);
		return -1;
	}

	logfd = fopen(argv[2],"w");
	if(logfd <0){
		perror("Failed to open logfile for writing.\n");
		return -1;
	}

	struct sockaddr_in serv_addr;
	struct hostent *server;

	port = atoi(argv[1]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Failed to open socket\n");
		fclose(logfd);
		return -1;
	}

	server = gethostbyname("localhost");
	if (!server) {
		printf("Couldn't find localhost");
		fclose(logfd);
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){ 
		perror ("Error connecting to server\n");
		fclose(logfd);
		return -1;
	}

	do_client(sockfd,logfd);

	fclose(logfd);

	printf("Transmission completed\n");

	return 0;
}
