#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Constant Parameters
#define MAX_VERTICES 10000
#define MAX_EDGES 3000
#define DAYS 300
#define TAU 0.5
#define GAMMA 0.2


// Some variables as global parameters for the simulation.
int curr_day = 0;
int susceptible;
int infected;
int recovered;

/*
	Plague Inc. 
	(Low-Cost Version)
*/

// A boolean matrix to store adjacent nodes
bool neighbour[MAX_VERTICES][MAX_VERTICES];

// An event node.
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

// A Linked List of the event nodes.
struct queue
{
	int length;

	struct q_node *front;
}
events;


// Structure of a graph node
struct vertex
{
	int id;
	char status;
	int pred_inf_time; // Pedicted infection time
	int rec_time;  // Recovery time

	struct l_node *adjacent;
}
graph[MAX_VERTICES];


/*

	Functions start here.

*/


// Function to add event to the priority queue.
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
	
	// Initializing Values Of the Event (Node)
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

// Function to remove event from the front of queue
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


// Function to create the graph.
void create_graph(int vertices)
{
	// Making the nodes disjoint.
	for (int i = 0; i < vertices; ++i)
	{
		for (int j = 0; j < vertices; ++j)
		{
			neighbour[i][j] = false;
		}
	}


	for (int i = 0; i < vertices; ++i)
	{
		// Initialising the node values 
		graph[i].id = i;
		graph[i].status = 'S';
		graph[i].pred_inf_time = DAYS+1;
		graph[i].rec_time = DAYS+1;
		graph[i].adjacent = NULL;
	
		// Making a directed graph by joining the links
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

	// Global Variable Initialised
	susceptible = vertices;

	// Converting directed graph to undirected.
	// Using symmetry.
	for (int i = 0; i < vertices; ++i)
	{
		for (int j = i; j < vertices; ++j)
		{
			neighbour[i][j] = neighbour[j][i];
		}
	}
}

// Prints the graph
// ##Not a mandatory funtion as per the project, but for user convenience.
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

// The Coin-Flip Experiment.
int exponent_variate(float lambda)
{
	int tries = 1;
	int norm_lam = (int)(lambda*10);
	int ran = rand()%10;
	while(ran >= norm_lam)
	{
		tries++;
		ran = rand()%10;
	}
	return tries;
}

// Funtion as Given in Project PDF
// Function for predicting the infection time of targets
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

// Funtion as Given in Project PDF
// Function for infecting the targets.
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

// Funtion as Given in Project PDF
// Function for recovering the infected people.
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

// Funtion as Given in Project PDF
// Processing the event queue/ running the simulation.
void simulation(struct queue *event_queue, int vertices)
{
	// The first persont to be infected.
	int first_infect = rand()%vertices;
	enqueue(event_queue, first_infect, 1, 'T');
	graph[first_infect].pred_inf_time = 1;
	curr_day++;

	// Event Processing
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
	// Printing the parameters of remaining days.
	while(curr_day < (DAYS+1))
	{
		printf("%d,%d,%d,%d\n",curr_day,susceptible,infected,recovered);
		curr_day++;
	}
}

int main()
{
	// Seeding the rand();
	srand(time(NULL));
	// Random number of vertices.
	int num_nodes = rand()%10000;
	create_graph(num_nodes);
	simulation(&events,num_nodes);
	return 0;
}