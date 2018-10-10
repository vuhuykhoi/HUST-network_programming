

typedef struct account
{
	char *acc;
	char *pass;
	int sts;
	int blockCountTime;
	int isLogging;
}val;


typedef struct node{
	val val;
	struct node *next;
}node;

typedef struct pairNode{
	node *first;
	node *last;
}linkList;

linkList *newLinkList();
void addToLastOfList(linkList *list,val El);
void printList(linkList *list);
node* newNode(val El);

val newVal(char *acc,char *pass,int status);
void printVal(val El);

int checkExistedNode(linkList *list,char *acc);