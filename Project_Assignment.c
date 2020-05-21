#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int MAX_VERTICES = 10000;
int MAX_EDGES = 3000;

int susceptible[300];
int infected[300];
int recovered[300];

struct q_node
{
	int id;
	int day;

	struct q_node *next;
};

struct queue
{
	int length;

	struct q_node *front, *rear;
}
events;

struct l_node
{
	int id;

	struct l_node *next;
};


struct g_node
{
	char status;
	struct l_node *adjacent;
};


// Function to add jobs to the end of queue.
void enqueue(struct queue *job_queue, int id, int day)
{
	// Memory Allocation to a node
	struct q_node *temp = (struct q_node *) malloc(sizeof(struct q_node));
	// Malloc Fail Check
	if (temp == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(0); 
	}
	
	// Initializing Values Of the Job (Node)
	temp->id = id;
	temp->day = day;
	temp->next = NULL;

	// Placing the node in the queue
	if ((job_queue->length) == 0)
	{
		job_queue->front = temp;
	}
	else
	{
		job_queue->rear->next = temp;	
	}

	// Node pointer to the last node
	job_queue->rear = temp;
	
	// Updating Value for queue
	job_queue->length +=1;
}

// Function to remove job from the front of queue
void dequeue(struct queue *job_queue)
{
	// Error handling
	if(job_queue->length == 0)
	{
		printf("Length of queue is already zero.\n");
	}

	else
	{
		// Node pointer for node to be removed
		struct q_node *temp = job_queue->front;

		// Updating the new front/rear for queue
		job_queue->front = job_queue->front->next;
		if(job_queue->length == 1)
		{
			job_queue->rear = NULL;
		}

		// Freeing the memory of the node not required
		free(temp);

		// Updating value for queue
		job_queue->length-=1;
	}
}