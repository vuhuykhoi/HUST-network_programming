#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 8192

//argv[1]:SERVER_ADDR argv[2]:SERVER_PORT
int main(int argc,char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;

	if(argc!=3){
		printf("Invalid Argument!\n");
		exit(1);
	}
	char SERVER_ADDR[20];
	strcpy(SERVER_ADDR,argv[1]);
	int SERVER_PORT = atoi(argv[2]);

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	
	//Step 4: Communicate with server			
	while(1){
		//send message
		printf("\nInsert string to send:");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		if(buff[strlen(buff)-1]=='\n'){
			buff[strlen(buff)-1] ='\0';
		}		
		msg_len = strlen(buff);
		if (msg_len == 0) break;
		
		bytes_sent = send(client_sock, buff, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
			break;
		}
	
		bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);

		if(bytes_received <= 0){
			printf("\nError!Cannot receive data from sever!\n");
			break;
		}
			
		buff[bytes_received] = '\0';
		printf("Reply from server:\n%s", buff);
	
	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}