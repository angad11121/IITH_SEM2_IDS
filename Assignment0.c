#include <stdio.h>
#include <stdlib.h>

struct node
{
	int data;
	struct node *next;
};

struct linkedlist 
{
	struct node *head;
};

struct linkedlist *listcreate()
{
	return malloc(sizeof(struct linkedlist));
}

void delete(struct linkedlist *l, int position)
{

}
void insert(struct linkedlist *l, int position, struct node* the_node)
{

}
void find(struct linkedlist *l, int rollno)
{

}
void findanddelete(struct linkedlist *l, int rollno)
{

}

int main()
{
	struct linkedlist* CSE;
	CSE=listcreate();
	CSE->head=NULL;
	// CSE->head->data=1;
	// printf("%d\n",CSE->head->data);
	return 0;
}