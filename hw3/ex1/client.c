#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 256

int main(int argc ,char*argv[]){
	int sockfd, rcvBytes, sendBytes;
	socklen_t len;
	char buff[BUFF_SIZE+1];
	struct sockaddr_in servaddr;

	if(argc != 3){
		printf("Invalid Argument!\n");
		exit(1);
	}

	int SERV_PORT = atoi(argv[2]);
	char SERV_ADDR[20] ;
	strcpy(SERV_ADDR,argv[1]);

	//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
	perror("Error: ");
	return 0;
	}
	//Step 2: Define the address of the server
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr = 
	inet_aton(SERV_ADDR, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	//Step 3: Communicate with server
	printf("Send to server: ");
	while(1){
		fgets(buff, BUFF_SIZE,stdin);
		if(buff[strlen(buff)-1]='\n'){
			buff[strlen(buff)-1] ='\0';
		}
		if(buff[0]=='\0') break;
		len = sizeof(servaddr);
		
		sendBytes = sendto(sockfd, buff, strlen(buff), 0,(struct sockaddr *) &servaddr, len);
		
		if(sendBytes < 0){
			perror("Error: ");
			return 0;
		}

		rcvBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0,(struct sockaddr *) &servaddr, &len);
		if(rcvBytes < 0){
			perror("Error: ");
			return 0;
		}
		buff[rcvBytes] = '\0';
		printf("Reply from server:\n%s\n", buff);
		printf("Send to server: ");
	}
}