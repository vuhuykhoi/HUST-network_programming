#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"
#include <string.h>

#define ACCOUNT_IS_NOT_EXSIST  -3
#define ACCOUNT_IS_BEING_BLOCKED  0
#define ACCOUNT_IS_ACTIVE 1
#define INVALID_PASSWORD -2
#define LOGGIN_SUCCESSFUL 1
#define BLOCK_THREE_TIME_FAIL -1
#define NOT_SIGN_IN -4
#define IS_SIGN_IN 1

val *currentLoggingAccount = NULL;
//----------LOADING DATA--------------------------------------
linkList* getListUserFromFile(FILE*);	

//------------REGISTER----------------------------------------
int registerAccount(linkList *list,char *);
void getInputAccFromUser(char *acc,char *pass);
void updateAccountToFile(linkList *list,char *filename);

//-----------SIGN IN------------------------------------------
int identifyValidAccount(linkList *list,char *acc,char *pass);
void signIn(linkList *list,char *filename);

//-----------LOG OUT------------------------------------------
int checkCurrentSession(char *);
void getAccNameFromUser(char *acc);
int signOut(linkList *list);

//-----------SEARCH-------------------------------------------
int seach(linkList *list);






//-----------MAIN---------------------------------------------

int main(int argc,char* argv[] ){
	int menuSelected = 0;
	linkList *accList = NULL;	
	
	FILE  *file = NULL;
	if(argc !=2){
		printf("ERROR: Wrong argument!Miss File name!\n");
		exit(-1);
	}

	if((file = fopen(argv[1],"r+")) == NULL){
		printf("ERROR:File can not be found!\n");
		exit(-1);
	}
	
	accList =  getListUserFromFile(file);	
	fclose(file);
	printList(accList);
	
	do{
		printf("-------------------------------\n");
		printf("USER MANAGEMENT PROGRAM\n");
		printf("1.Register\n");
		printf("2.Sign in\n");
		printf("3.Search\n");
		printf("4.Sign out\n");
		printf("Your choice(1-4,other to quit):");
		scanf("%d",&menuSelected);
		printf("-------------------------------\n");
		switch(menuSelected){
			case 1:	
				if(registerAccount(accList,argv[1])){
					printf("Successful registration!\n");
				}else{
					printf("Account existed\n");
				}
			break;
			case 2:
				signIn(accList,argv[1]);
			break;
			case 3:
				switch(seach(accList)){
					case NOT_SIGN_IN:
					printf("Account is not sign in\n");
					break;
					case ACCOUNT_IS_ACTIVE:
					printf("Account is active\n");
					break;
					case ACCOUNT_IS_BEING_BLOCKED:
					printf("Account is blocked");
					break;
					case ACCOUNT_IS_NOT_EXSIST:
					printf("Cannot find account\n");
					break;

				}
			break;
			case 4:
				switch(signOut(accList)){
					case IS_SIGN_IN:
					printf("Goodbye,%s\n",currentLoggingAccount->acc);
					currentLoggingAccount = NULL;
					break;
					case NOT_SIGN_IN:
					printf("Account is not sign in\n");
					break;
					case ACCOUNT_IS_NOT_EXSIST:
					printf("Cannot find account\n");
					break;
				}
			break;
			default:
			break;
		}
	}while(menuSelected==1||menuSelected==2||menuSelected==3||menuSelected==4);
	return 0;
}

//----------LOADING DATA-----------------------------------
linkList* getListUserFromFile(FILE *file){
	char *line = malloc(sizeof(char)*50);
	linkList *tmpLL = newLinkList();
	val El;
	while(fgets(line,50,file) != NULL ){
		int i = 0;
		char *p = strtok (line, " ");
    	char *array[3];

    	while (p != NULL)
    	{
        	array[i++] = p;
        	p = strtok (NULL, " ");
    	}
		El = newVal(array[0],array[1],atoi(array[2]));
		if(tmpLL!=NULL) addToLastOfList(tmpLL,El);
	}
	return tmpLL;
}
//------------REGISTER-------------------------------------
int registerAccount(linkList *list,char *filename){
	char *acc = malloc(sizeof(char)*20);
	char *pass = malloc(sizeof(char)*20);
	getInputAccFromUser(acc,pass);
	if(checkExistedNode(list,acc)) return 0;
	else{
		val El = newVal(acc,pass,1);
		addToLastOfList(list,El);
		updateAccountToFile(list,filename);
	}
	return 1;
}

void getInputAccFromUser(char *acc,char *pass){
	printf("account:");
	scanf("%s",acc);
	printf("password:");
	scanf("%s",pass);
}

void updateAccountToFile(linkList *list,char *filename){
	FILE *fp =fopen(filename,"r+");
	node *tmp = list->first;
	while(tmp!=NULL){
		fprintf(fp,"%s %s %d\n",tmp->val.acc,tmp->val.pass,tmp->val.sts);
		tmp = tmp->next;
	}
	fclose(fp);
}
//-------------SIGN IN-------------------------------------
int identifyValidAccount(linkList *list,char *acc,char *pass){
	node *tmp = list->first;
	//checking account id:
	while(tmp !=NULL){
		if(!strcmp(tmp->val.acc,acc)) break;
		tmp = tmp->next;
	}
	//checking account password:
	if(tmp !=NULL){
		if(!strcmp(tmp->val.pass,pass)){
			//return 0:is blocking,1:is not blockking
			if(tmp->val.sts == 1){
				tmp->val.blockCountTime = 0 ;
				tmp->val.isLogging = 1;
				currentLoggingAccount = &(tmp->val);
				return LOGGIN_SUCCESSFUL;
			}else{
				return ACCOUNT_IS_BEING_BLOCKED;
			}
		}else{
			if(tmp->val.blockCountTime < 3) {
				printf("%d",tmp->val.blockCountTime++);
				if(tmp->val.blockCountTime == 3){
					tmp->val.sts = 0 ;
					return BLOCK_THREE_TIME_FAIL;
				} 
			return INVALID_PASSWORD;//password not correct
			}
		}
	}
	return ACCOUNT_IS_NOT_EXSIST;//account not exsist
}

void signIn(linkList *list,char *filename){
	char *acc = malloc(sizeof(char)*20);
	char *pass = malloc(sizeof(char)*20);
	getInputAccFromUser(acc,pass);
	switch(identifyValidAccount(list,acc,pass)){
		case ACCOUNT_IS_NOT_EXSIST:
			printf("Cannot find account\n");
			break;
		case INVALID_PASSWORD:
			printf("Password is incorrect\n");
			break;
		case BLOCK_THREE_TIME_FAIL:
			printf("Password is incorrect. Account is blocked\n");
			updateAccountToFile(list,filename);
			break;
		case ACCOUNT_IS_BEING_BLOCKED:
			printf("Account is blocked\n");
			break;
		case LOGGIN_SUCCESSFUL:
			printf("\nHello,%s\n\n",currentLoggingAccount->acc);
			break;
	}
}
//---------------SIGN OUT------------------------------------
void getAccNameFromUser(char *acc){
	printf("account:");
	scanf("%s",acc);
}
int checkCurrentSession(char *acc){
	if(currentLoggingAccount == NULL || strcmp(currentLoggingAccount->acc,acc)) return NOT_SIGN_IN; 
	return IS_SIGN_IN;
}

int signOut(linkList *list){
	char *acc = malloc(sizeof(char)*20);
	getAccNameFromUser(acc);
	if(checkExistedNode(list,acc)) return checkCurrentSession(acc);
	return ACCOUNT_IS_NOT_EXSIST;
}

//---------------SEARCH--------------------------------------
int seach(linkList *list){
	if(currentLoggingAccount!=NULL){
		char *acc = malloc(sizeof(char)*20);
		getAccNameFromUser(acc);
		node *tmp = list->first;
		while(tmp != NULL){
			if(!strcmp(tmp->val.acc,acc)){
				return tmp->val.sts==0? ACCOUNT_IS_BEING_BLOCKED:ACCOUNT_IS_ACTIVE;
			}
			tmp = tmp->next;
		}
		if(tmp == NULL) return ACCOUNT_IS_NOT_EXSIST;  
		
	}
	return NOT_SIGN_IN;
}