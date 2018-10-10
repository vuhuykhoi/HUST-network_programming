#include <stdio.h>
#include "result_msg.h"
#include <stdlib.h>
#include <string.h>

char *getErrorMessage(){
	int error_code;
	char *msg = malloc(sizeof(char)*100);
	switch(error_code){
		case FILE_IS_EXISTED:
			strcpy(msg,"ERROR: File is existed on server");

		case FILE_CAN_UPLOAD:
			strcpy(msg,"file can upload");
			
		case RECEIVE_FILE_FAIL:
			strcpy(msg,"ERROR: Recevive file fail");

		case WRITE_FILE_FAIL:
			strcpy(msg,"ERROR: Write file fail");

		case FILE_IS_NOT_EXISTED:
			strcpy(msg,"ERROR: File not found");

		case FILE_TRANFER_INTERUPTED:
			strcpy(msg,"ERROR: File tranfering is  interrupted");
	
		case INVALID_ARGUMENT:
			strcpy(msg,"ERROR: Invalid argument!");
	}
	return msg;
}

void exit_error(int error_code){
	perror(getErrorMessage(error_code));
	exit(1);
}

void error(int error_code){
	perror(getErrorMessage(error_code));
} 