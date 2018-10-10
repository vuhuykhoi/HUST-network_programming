
#include <stdlib.h>
#include <string.h>
#include "linklist.h"
#include <stdio.h>

val newVal(char *acc,char *pass,int sts){
	val tmp ;
	tmp.acc = malloc(sizeof(char)*20);
	tmp.pass = malloc(sizeof(char)*20);
	strcpy(tmp.acc,acc);
	strcpy(tmp.pass,pass);
	tmp.sts = sts;
	tmp.blockCountTime = 0;
	tmp.isLogging = 0;
	return tmp;
}

void printVal(val El){
	printf("%s %s %d\n",El.acc,El.pass,El.sts);
}

node* newNode(val El){
	node *tmp = malloc(sizeof(node));
	if(tmp ==NULL){
		printf("cannot malloc");
		exit(1);
	}
	tmp->val = El;
	tmp->next = NULL;
	return tmp;
}

linkList *newLinkList(){
	linkList *tmp = malloc(sizeof(linkList));
	return tmp;
}

void addToLastOfList(linkList *list,val El){
	if(list->first == NULL){
		list->first = newNode(El);
		list->last = list->first;
	}else{
		list->last->next =  newNode(El);
		list->last = list->last->next;
	}
}

void printList(linkList *list){
	node *tmp = list->first;
	while(tmp !=NULL){
		printVal(tmp->val);
		tmp = tmp->next;
	}
}

int checkExistedNode(linkList *list,char *acc){
	node *tmp = list->first;
	while(tmp !=NULL){
		if(strcmp(tmp->val.acc,acc)==0) return 1;
		tmp = tmp->next;
	}	
	return 0;
}