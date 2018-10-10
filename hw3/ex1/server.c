#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 256

//return 1(true) if string has only number and alphabet
// return 0 (false) if string has special character 
int checkValidString(char* str){
	for(int i = 0 ; i<strlen(str);i++){
		if(!isdigit(str[i]) && !isalpha(str[i])) {
			return 0;
		}
	}
	return 1;
}

void appendCharToString(char *str,char a){
	str[strlen(str)] = a;
	str[strlen(str)+1] = '\0';
}

void appendStringToString(char *str1,char *str2){
	
	for(int i=0;i<strlen(str2);i++){
		appendCharToString(str1,str2[i]);
	}
}

//return string in two parts: number part and alpha part
char **splitStringInNumberAndAlpha(char *str){
	
	char **result = malloc(2 * sizeof(char*));
	for (int i =0 ; i < 2; ++i)
    	result[i] = malloc(BUFF_SIZE * sizeof(char));

	if(!checkValidString(str)){
		strcpy(result[0],"Error");
		return result;
	}
	//Split string in two array:number array and alphabet array
	for(int i =0 ;i<strlen(str);i++){
		if(isdigit(str[i])) appendCharToString(result[0],str[i]);
		else if(isalpha(str[i])) appendCharToString(result[1],str[i]);

	}
	return result;
}

//server create message to send to client
char *createSendResultMessage(char *buff){
	char **splitedString = splitStringInNumberAndAlpha(buff);
	char *result = malloc(sizeof(char)*(strlen(splitedString[0])+strlen(splitedString[1])));
	if(splitedString[0][0]!='\0' && splitedString[1][0]!='\0'){
		appendStringToString(result,splitedString[0]);
		appendCharToString(result,'\n');
		appendStringToString(result,splitedString[1]);
	}else if(splitedString[0][0] == '\0'){
		appendStringToString(result,splitedString[1]);
	}else if(splitedString[1][0]== '\0'){
		appendStringToString(result,splitedString[0]);
	}
	free(splitedString);
	return result;
}

//return string length of str1
int myStringCopy(char *str1,char *str2){
	int  i = 0;
	for(i = 0;i< strlen(str2);i++){
		str1[i] = str2[i];
	}
	str1[i] = '\0';
	return i;
}

int main(int argc,char *argv[]){
	
	int sockfd, rcvBytes, sendBytes;
	socklen_t len;
	char buff[BUFF_SIZE+1];
	struct sockaddr_in servaddr, cliaddr;
	if(argc != 2){
		printf("Invalid Argument!\n");
		exit(1);
	}

	int SERV_PORT = atoi(argv[1]);
	//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Error: ");
		return 0;
	}

	//Step 2: Bind address to socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))){
		perror("Error: ");
		return 0;
	}
	printf("Server started.\n");
	//Step 3: Communicate with client
	;
	for ( ; ; ) {
		len = sizeof(cliaddr);
		rcvBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr, &len);	
		if(rcvBytes < 0){
			perror("Error: ");
			return 0;
		}	

		//IMPORTANT LINE!!!!!!!!!!!!!!!!!!!!!
		buff[rcvBytes]='\0';

		int recvBytes =  myStringCopy(buff,createSendResultMessage(buff));

		char *mesg = malloc(sizeof(char)*256);
		printf("[%s:%d]: %s", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port), mesg);
		free(mesg);
		sendBytes = sendto(sockfd, buff, recvBytes, 0,(struct sockaddr *) &cliaddr, len);

		if(sendBytes < 0){
			perror("Error: ");
			return 0;
		}
	}
}