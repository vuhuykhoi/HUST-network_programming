#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int isNumber(char *str){
	for(int i = 0 ;i <strlen(str);i++){
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}

int isIPAddress(char *addr){
	int count = 0;
    char *str = malloc(sizeof(char)*16);
    strcpy(str,addr);
    char *token = strtok(str, "."); 
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

void convertIPAddressToHostName(char *str){
	struct in_addr test2;
	struct hostent *test;
	
	char *hostAddr = malloc(sizeof(char)*50);
	strcpy(hostAddr,str);

	inet_pton(AF_INET,hostAddr,&test2);

	test = gethostbyaddr(&test2,4,AF_INET);
	
	if(test !=NULL){
		if(test->h_name!= NULL)
			printf("Official name:%s\n",test->h_name);
		int i = 0;
		while(test->h_aliases[i]!= NULL){
			if(i==0) printf("Alias name:\n");
			printf("%s\n",test->h_aliases[i]);
			i++;
		}
	}else{
		printf("Not found information\n");
	}
}

void convertHostNameToIP(char *IPAddr){
	struct hostent *test;
	test =gethostbyname(IPAddr);
	if(test !=NULL){
		char str[INET_ADDRSTRLEN];
		int i = 0;
		while(test->h_addr_list[i]!=NULL){
			inet_ntop(AF_INET, test->h_addr_list[i], str, INET_ADDRSTRLEN);
			//first element is Official IP
			if(i==0) printf("Official IP:\n");
			else if(i ==1) printf("Alias IP:\n");
			printf("%s\n", str);
			strcpy(str,"");
			i++;
		}
	}else{
		printf("Not found information\n");
	}
}

int main(int argc ,char *argv[]){
	int menu =0;
	if(argc!=2){
		printf("Missing argument!\n");
		exit(1);
	}
	menu = isIPAddress(argv[1]);

	switch(menu){
		case 1:
			convertIPAddressToHostName(argv[1]);
			break;
		case 0:
			convertHostNameToIP(argv[1]);
			break;
	}
	return 0;

}