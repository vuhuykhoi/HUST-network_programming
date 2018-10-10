#include "filehelper.h"

//-------------------------------------------------------------

char *getFileName(char *fURL){
	int idx = 0;
	for(int i =strlen(fURL);i>=0 ;i--){
		if(fURL[i]=='/'){
			idx = i +1;
			break;
		}
	}
	char *result =malloc(sizeof(char)*100);
	strncpy(result,fURL+idx,strlen(fURL)+1-idx);
	
	return result;
}

FILE *createEmptyFile(char *fURLin,char *serverfolderURL){
	char *fname = getFileName(fURLin);
	char *fURL = (char*)malloc(sizeof(char)*(strlen(fname)+strlen(serverfolderURL)+1)); 
	fURL[0]= '\0';
	strcat(fURL,serverfolderURL);
	strcat(fURL,fname);
	if(isExsistedFile(fURL)){
		 return NULL;
	}
	FILE *fout = fopen(fURL,"wr");
	return fout;
}

int isExsistedFile(char *fname){
	FILE *file;
    if (file = fopen(fname, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
/*
int main(int argc , char*argv[1]){
	makeFile(getFileName(argv[1]),readFile(argv[1]),getSizeOfFile(argv[1]));
	return 0;
}*/