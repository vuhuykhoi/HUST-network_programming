#include <stdlib.h>
#include <string.h>
#include <ctype.h>

   
#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024

int checkValidString(char* str);

void appendCharToString(char *str,char a);
void appendStringToString(char *str1,char *str2);
char **splitStringInNumberAndAlpha(char *str);
char *createSendResultMessage(char *buff);
int myStringCopy(char *str1,char *str2);