#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "filehelper.h"
#include "result_msg.h"

#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024

#define serverfolderURL "sf/"


int myStringCopy(char *str1,char *str2)
{
	int  i = 0;
	for(i = 0;i< strlen(str2);i++){
		str1[i] = str2[i];
	}
	str1[i] = '\0';
	return i;
}

int main(int argc , char* argv[])
{
	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE+1],send_data[BUFF_SIZE+1];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	if(argc != 2){
		printf("INVALID ARGUMENT!\n");
		exit(1);
	}

	int PORT = atoi(argv[1]);
	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 2: Bind address to socket
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}     
	
	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}	


	//Step 4: Communicate with client
	while(1)
	{
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size)) == -1) 
			perror("\nError: ");
  
		printf("\nYou got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		//start conversation
		while(1){
			//receives file name message from client
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
			if (bytes_received <= 0){
				printf("\nConnection closed");
				break;
			}
			else{
				recv_data[bytes_received] = '\0';
				printf("\nReceive file:%s\n", recv_data);
			}
			
			//check existed file and create if not exsisted and echo ressult to client 
				//check 
			FILE *fout = createEmptyFile(getFileName(recv_data),serverfolderURL);
			if(fout == NULL){
				bytes_received = myStringCopy(recv_data,"File is exsisted");
			}
			else{
				bytes_received = myStringCopy(recv_data,"Ready to upload");
			}

			//send mmsg
			bytes_sent = send(conn_sock, recv_data, bytes_received, 0); 
			if (bytes_sent <= 0){
				printf("\nConnection closed");
				break;
			}
			bzero(recv_data,BUFF_SIZE);

			//receive file content
			while(1){	
				bytes_received = recv(conn_sock, recv_data,BUFF_SIZE, 0);
				if (bytes_received <= 0){
					break;
				}
				/////////////////////////////////////////////////////////
				//fwrite function  write bytes MUST EQUAL byte_received//
				/////////////////////////////////////////////////////////
				int write_sz = fwrite(recv_data,sizeof(char),bytes_received,fout);
				if(write_sz < bytes_received){
		            exit_error(WRITE_FILE_FAIL);
		        }	
				bzero(recv_data,BUFF_SIZE);
			}
			if(bytes_received < 0){
	            exit_error(RECEIVE_FILE_FAIL);
	        }	

	        printf("Ok received from client!\n");
			fclose(fout);
		}
		close(conn_sock);	
	}
	close(listen_sock);
	return 0;
}
	
