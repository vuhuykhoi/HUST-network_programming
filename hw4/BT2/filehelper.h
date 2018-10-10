#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELENGTH 512
#define FILEURLLENGTH 200

char *getFileName(char *fURL);
FILE *createEmptyFile(char *fname,char *serverfolderURL);
int isExsistedFile(char *fname);