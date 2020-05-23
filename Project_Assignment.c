#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 3000
#define DAYS 300
#define TAU 0.5
#define GAMMA 0.2

int curr_day = 0;
int susceptible;
int infected;
int recovered;

/*
	Plague Inc. 
	(Low-Cost Version)
*/
bool neighbour[MAX_VERTICES][MAX_VERTICES];

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


struct vertex
{
	int id;
	char status;
	int pred_inf_time;
	int rec_time;

	struct l_node *adjacent;
}
graph[MAX_VERTICES];


/*

	Functions start here.

*/


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
			while( (temp->next) && (temp->next->day < day))
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


void create_graph(int vertices)
{
	for (int i = 0; i < vertices; ++i)
	{
		for (int j = 0; j < vertices; ++j)
		{
			neighbour[i][j] = false;
		}
	}

	for (int i = 0; i < vertices; ++i)
	{
		graph[i].id = i;
		graph[i].status = 'S';
		graph[i].pred_inf_time = DAYS+1;
		graph[i].rec_time = DAYS+1;
		graph[i].adjacent = NULL;
	
		int node_neighbours[MAX_EDGES];
		for (int j = 0; j < MAX_EDGES; ++j)
		{
			node_neighbours[j] = rand() % (vertices);
			if(node_neighbours[j] == i) j--;
		}
		for (int j = 0; j < MAX_EDGES; ++j)
		{
			neighbour[i][node_neighbours[j]] =true;
		}
	}

	susceptible = vertices;

	for (int i = 0; i < vertices; ++i)
	{
		for (int j = i; j < vertices; ++j)
		{
			neighbour[i][j] = neighbour[j][i];
		}
	}
}

void printgraph(int vertices)
{
	for (int i = 0; i < vertices; ++i)
	{
		printf("\n%d %c %d %d:", i ,graph[i].status,graph[i].pred_inf_time,graph[i].rec_time);
		
		for (int j = 0; j < vertices; ++j)
		{
			if(neighbour[i][j])
			printf(" %d",j);
		}
	}
}

// Minimum Function
int min(int a, int b)
{
	if(a>b) return b;
	else return a;
}


int exponent_variate(float lambda)
{
	int tries = 1;
	int norm_lam = (int)(lambda*10);
	// printf("%d\n", norm_lam);
	int ran = rand()%10;
	while(ran >= norm_lam)
	{
		// printf("%d\n",tries );
		tries++;
		ran = rand()%10;
	}
	return tries;
}

void find_trans_SIR(struct queue *event_queue, struct vertex *target, struct vertex *source)
{
	if((target->status) == 'S')
	{
		int inf_time = curr_day + exponent_variate(TAU);
		if(inf_time <  min( (source->rec_time), min((target->pred_inf_time),DAYS) ) )
		{
			enqueue(event_queue, (target->id), inf_time, 'T');
			target->pred_inf_time = inf_time;
		}
	}
}

void proc_trans_SIR(struct queue *event_queue, struct vertex *target, int vertices)
{
	if((target->status) == 'S')
	{
		susceptible--;
		infected++;
		target->status = 'I';
		target->rec_time = curr_day + exponent_variate(GAMMA);
		if((target->rec_time) < DAYS)
		{
			enqueue(event_queue, (target->id), (target->rec_time),'R');
		}

		for (int i = 0; i < vertices; ++i)
		{
			if(neighbour[(target->id)][i])
			{
				find_trans_SIR(event_queue, &graph[i], target);
			}
		}
	}
	dequeue(event_queue);
}

void proc_rec_SIR(struct queue *event_queue,struct vertex *target)
{
	if((target->status) == 'I')
	{
		infected--;
		recovered++;
		target->status = 'R';
	}
	dequeue(event_queue);
}

void simulation(struct queue *event_queue, int vertices)
{
	int first_infect = rand()%vertices;
	enqueue(event_queue, first_infect, 1, 'T');
	graph[first_infect].pred_inf_time = 1;
	curr_day++;

	while((event_queue->length) && curr_day < (DAYS+1))
	{
		if((event_queue->front->day) > curr_day)
		{
			printf("%d,%d,%d,%d\n",curr_day,susceptible,infected,recovered);
			curr_day++;
		}
		else if((event_queue->front->action) == 'T')
		{
			proc_trans_SIR(event_queue,&graph[(event_queue->front->id)], vertices);
		}
		else if((event_queue->front->action) == 'R')
		{
			proc_rec_SIR(event_queue,&graph[(event_queue->front->id)]);
		}
	}
	while(curr_day < (DAYS+1))
	{
		printf("%d,%d,%d,%d\n",curr_day,susceptible,infected,recovered);
		curr_day++;
	}
}

int main()
{
	srand(time(NULL));
	int num_nodes = rand()%10000;
	create_graph(num_nodes);
	simulation(&events,num_nodes);
	return 0;
}