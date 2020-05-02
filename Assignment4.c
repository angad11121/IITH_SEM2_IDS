#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// A struct for nodes in the queue
struct q_node
{
	// Parameters containing job number and time required for job.
	int job_number;
	int time;

	// Parameter for time for which a job has been done before going to the rear of queue
	int time_per_processing;

	// A node pointer for the next node
	struct q_node *next;
};

// A struct for holding the queue
struct queue
{
	// Parameters for length of queue and total jobs distributed.
	int length;
	int total_jobs_seen;

	// Pointers to the front and rear node of queue
	struct q_node *front, *rear;

}
// Declaration of struct.
jobs;

// Minimum Function
int min(int a, int b)
{
	if(a>b) return b;
	else return a;
}

// Function to add jobs to the end of queue.
void enqueue(struct queue *job_queue, int job_number, int time)
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
	temp->job_number = job_number;
	temp->time = time;
	temp->time_per_processing = 0;
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

// Function to add multiple jobs in the queue
void bulkadd(struct queue *job_queue, int number_of_jobs)
{
	for (int i = 0; i < number_of_jobs; ++i)
	{
		job_queue->total_jobs_seen += 1;
		int time = rand()%300 + 1; 
		// I've excluded 0 since it wasn't mentioned for 0 to be inclusive.
		enqueue(job_queue,job_queue->total_jobs_seen,time);
	}
}

// Function to print the queue
void print_queue(struct queue *job_queue)
{
	if(job_queue->length == 0)
	{
		printf(" Empty\n");
	}

	else
	{
		// Node pointer to iterate over nodes
		struct q_node *temp = job_queue->front;

		// For : each node
		for (int i = 0; i < (job_queue->length); ++i)
		{
			// Printing the data
			printf(" J%d:%d;",temp->job_number,temp->time);
			
			// Iteration update expression
			temp = temp->next;
		}
		printf("\n");
	}
}

// Function to iterate over jobs
void job_processing(struct queue *job_queue)
{
	// Variable Declaration and initialisation
	int jobs_to_add = 100;
	int time_elapsed = 0;

	// Loop for the processes
	/*
		Design of this function:
		
		/Based on events/
		Event 1: Time has reached a multiple of 50
		Event 2: *Time per processing* of a particular job has reached 50
		Event 3: A job has been completed

		Total time time is updated till the occuarance of any event and then necessary
		actions are taken accordingly
	*/
	while(jobs_to_add||job_queue->length)
	{
		// Variable for time to the nearest event
		int time_in_loop = 0;

		if((job_queue->length) > 0)
		{
		 	// Time of nearest event 
		 	// Note that this will always be <= 50us.
			time_in_loop = min( min(50-(time_elapsed%50),50-(job_queue->front->time_per_processing)%50),job_queue->front->time);
			
			// Time values upadted in thr job (node)
			job_queue->front->time -= time_in_loop;
			job_queue->front->time_per_processing +=time_in_loop;
			
			// When Job is finished
			if(job_queue->front->time == 0)
			{
				dequeue(job_queue);
			}
			// When job has been alloted 50us.
			else if(job_queue->front->time_per_processing == 50)
			{
				enqueue(job_queue,job_queue->front->job_number,job_queue->front->time);
				dequeue(job_queue);
			}
		}

		// Only event 1 is valid when the queue is empty.
		else
		{
			time_in_loop = 50-(time_elapsed%50);
		}
		
		// Updating the total time
		time_elapsed += time_in_loop;

		// Printing Queue
		if(time_elapsed%50 == 0 && time_elapsed != 0)
		{
			printf("At t=%dus:",time_elapsed);
			print_queue(job_queue);
		}

		// Adding new jobs in the queue
		if(time_elapsed%200 == 0 && time_elapsed != 0 && jobs_to_add !=0 )
		{
			bulkadd(job_queue,jobs_to_add);
			printf("******** %dus passed added %d new jobs at the end of queue\n",time_elapsed,jobs_to_add);
			jobs_to_add /= 2;
			printf("After addition:");
			print_queue(job_queue);
		}
		
	}

	// For the final queue status.
	if(time_elapsed%50 !=0)
	{
		time_elapsed += 50-(time_elapsed%50);
		printf("At t=%dus:",time_elapsed);
		print_queue(job_queue);
	}
}

// The main function
int main()
{
	// Seeding the rand() function
	srand(time(NULL));

	// Initialising the values for queue
	jobs.length = 0;
	jobs.total_jobs_seen = 0;
	jobs.front = NULL;
	jobs.rear = NULL;
	// Pointer for queue
	struct queue *job_queue = &jobs;

	// Initialising the queue with some nodes (jobs)
	bulkadd(job_queue,3);

	// Print Format
	printf("Created 3 jobs:\n");
	printf("Initial Queue:");
	print_queue(job_queue);
	
	// Processing the jobs
	job_processing(job_queue);

	return 0;
}