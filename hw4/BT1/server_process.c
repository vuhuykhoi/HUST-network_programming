#include "server_process.h"

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