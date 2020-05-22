#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 3000
#define DAYS 300

int susceptible[MAX_EDGES];
int infected[300];
int recovered[300];

struct q_node
{
	int id;
	int day;

	char action;
	/*
		T - Transmit
		R - Recover
	*/
	struct q_node *next;
};

struct queue
{
	int length;

	struct q_node *front;
}
events;

struct l_node
{
	int id;

	struct l_node *next;
};


struct vertex
{
	char status;
	int pred_inf_time;
	int rec_time;

	struct l_node *adjacent;
}
graph[MAX_VERTICES];


// Function to add jobs to the end of queue.
void enqueue(struct queue *event_queue, int id, int day, char action)
{
	// Memory Allocation to a node
	struct q_node *newnode = (struct q_node *) malloc(sizeof(struct q_node));
	// Malloc Fail Check
	if (newnode == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(1); 
	}
	
	// Initializing Values Of the Job (Node)
	newnode->id = id;
	newnode->day = day;
	newnode->action = action;
	newnode->next = NULL;

	// Placing the node in the queue
	if ((event_queue->length) == 0)
	{
		event_queue->front = newnode;
	}
	else
	{
		if(event_queue->front->day > day)
		{
			newnode->next = event_queue->front;
			event_queue->front = newnode;
		}
		else
		{
			struct q_node *temp = event_queue->front;
			while( (temp->next) && (temp->day < day))
			{
				temp = temp->next;
			}
			newnode->next = temp->next;
			temp->next = newnode;
		}
	}
	
	// Updating Value for queue
	event_queue->length +=1;
}

// Function to remove job from the front of queue
void dequeue(struct queue *event_queue)
{
	// Error handling
	if(event_queue->length == 0)
	{
		printf("Length of queue is already zero.\n");
	}

	else
	{
		// Node pointer for node to be removed
		struct q_node *temp = event_queue->front;

		// Updating the new front/rear for queue
		event_queue->front = event_queue->front->next;

		// Freeing the memory of the node not required
		free(temp);

		// Updating value for queue
		event_queue->length-=1;
	}
}

void l_push(int id, struct vertex *vtx)
{
	// Memory Allocation to a node
	struct l_node *newnode = (struct l_node *) malloc(sizeof(struct l_node));
	// Malloc Fail Check
	if (newnode == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(1); 
	}
	
	// Initializing Values Of the Job (Node)
	newnode->id = id;
	newnode->next = NULL;

	// Placing the node in the queue
	if ((vtx->adjacent) == NULL)
	{
		vtx->adjacent = newnode;
	}
	else
	{
		if(vtx->adjacent->id > id)
		{
			newnode->next = vtx->adjacent;
			vtx->adjacent = newnode;
		}
		else
		{
			struct l_node *temp = vtx->adjacent;
			while( (temp->next) && (temp->id < id))
			{
				temp = temp->next;
			}
			newnode->next = temp->next;
			temp->next = newnode;
		}
	}
}

void create_graph(int vertices, int edges)
{
	for (int i = 0; i < vertices; ++i)
	{
		graph[i].status = 'S';
		graph[i].pred_inf_time = DAYS+1;
		graph[i].rec_time = DAYS+1;
		graph[i].adjacent = NULL;
	}
	for (int i = 0; i < edges; ++i)
	{
		int ran1 = rand()% (vertices+1);
		int ran2 = rand()% (vertices+1);
		if(ran1 != ran2)
		{
			l_push(ran1, &graph[ran2]);
			l_push(ran2, &graph[ran1]);
		}
	}
}
void printgraph(int vertices)
{
	for (int i = 0; i < vertices; ++i)
	{
		printf("\n%d %c %d %d:", i ,graph[i].status,graph[i].pred_inf_time,graph[i].rec_time);
		struct l_node *temp = graph[i].adjacent;
		while(temp)
		{
			printf(" %d",temp->id);
			temp= temp->next;
		}
	}
}

int main()
{
	create_graph(20, 50);
	printgraph(20);
	return 0;
}