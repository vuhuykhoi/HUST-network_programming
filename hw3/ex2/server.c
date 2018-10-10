#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/unistd.h>

#define BUFF_SIZE 256

//append a char to string .
//Input:String != NULL
void appendCharToString(char *str,char a){
	str[strlen(str)] = a;
	str[strlen(str)+1] = '\0';
}

//append str2 to str1
//Input str1 !=NULL
void appendStringToString(char *str1,char *str2){
	
	for(int i=0;i<strlen(str2);i++){
		appendCharToString(str1,str2[i]);
	}
}

//copy str2 to str1
//Str1 is empty.
//return string len of str1
int myStringCopy(char *str1,char *str2){
	int  i = 0;
	for(i = 0;i< strlen(str2);i++){
		str1[i] = str2[i];
	}
	str1[i] = '\0';
	return i;
}

//check if string of character is number?
//return 0:false ,1:true
int isNumber(char *str){
	for(int i = 0 ;i <strlen(str);i++){
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}

//check string is IP address?
//return 0:false,return 1 :true
int isIPAddress(char *addr){
	int count = 0;
    char *str = malloc(sizeof(char)*16);
    strcpy(str,addr);
    char *token = strtok(str, "."); 
   //split string in numbers
    while (token != NULL) 
    { 
        if(!isNumber(token)) return 0;
		if(atoi(token)<0 ||atoi(token)>255) return 0; 
        token = strtok(NULL, ".");
        count++; 
    }
    free(str); 
    //IP address has for number
    if(count !=4) return 0;
    return 1; 
}

char* convertHostNameToIP(char *hostname){
	char* result = malloc(sizeof(char)*256); 
	struct hostent *test;
	test =gethostbyname(hostname);
	if(test !=NULL){
		char str[INET_ADDRSTRLEN];
		int i = 0;
		while(test->h_addr_list[i]!=NULL){
			inet_ntop(AF_INET, test->h_addr_list[i], str, INET_ADDRSTRLEN);
			//first element is Official IP
			if(i==0) myStringCopy(result,"Official IP:\n");
			else if(i ==1) appendStringToString(result,"Alias IP:\n");
			appendStringToString(result,str);
			appendStringToString(result,"\n");
			strcpy(str,"");
			i++;
		}
	}else{
		myStringCopy(result,"Not found information\n");
	}
	return result;
}

char *convertIPAddressToHostName(char *IPAddr){
	
	struct in_addr test2;
	struct hostent *test;

	char *result = malloc(sizeof(char)*256);
	char *hostAddr = malloc(sizeof(char)*50);
	
	strcpy(hostAddr,IPAddr);
	inet_pton(AF_INET,hostAddr,&test2);
	test = gethostbyaddr(&test2,4,AF_INET);
	
	if(test !=NULL){
		if(test->h_name!= NULL)
			myStringCopy(result,"Official name:\n");
			appendStringToString(result,test->h_name);
			appendStringToString(result,"\n");
		int i = 0;
		while(test->h_aliases[i]!= NULL){
			if(i==0) appendStringToString(result,"Alias name:\n");
			appendStringToString(result,test->h_aliases[i]);
			i++;
		}
	}else{
		myStringCopy(result,"IP Address is invalid\n");
	}
	return result;
}

//Create message to send to client
char *createSendResultMessage(char *str){
	char *result;
	if(isIPAddress(str)) result = convertIPAddressToHostName(str);
	else result = convertHostNameToIP(str);
	
	return result;
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