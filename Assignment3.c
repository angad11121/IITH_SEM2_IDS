#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// A Node Structure for the Stack
struct node
{
	// The three parameters defing the functions currently in the stack.
	int low;
	int high;
	int line_number;

	// An address to the next node (implementation of singly-linked list)
	struct node *next;
};

// A Stack Structure holding the Nodes 
struct stack
{
	// Length of Stack
	int length;

	// A pointer to the Top Node of Stack
	struct node *top;
}
// A global stack declared
qsort_status;

// Function to display all the nodes in stack
void display(struct stack *stack_ptr)
{
	// Initializing a pointer to the top of stack
	struct node *temp = stack_ptr->top;

	// Indentation for output. :)
	for (int i = 0; i < (stack_ptr->length); ++i)
	{
		printf("\t");
	}
	// Print format
	printf("{");

	// For each node in stack
	for (int i = 0; i < (stack_ptr->length); ++i)
	{
		// Print the required values
		printf(" (%d,%d,%d)",temp->low,temp->high,temp->line_number);
		if (i < (stack_ptr->length)-1)
		{
			printf(",");
		}

		// Node Iteration Update Expression
		temp= temp->next;
	}
	// Print Format
	printf("}\n");
}

// Push Function - Used to add a node
void push(struct stack *stack_ptr,int low,int high,int line_number)
{
	// Memory allocation
	struct node *temp = (struct node *) malloc(sizeof(struct node));
	
	// Allocation fail check.
	if (temp == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(0); 
	}

	// Updating other values
	temp->low = low;
	temp->high = high;
	temp->line_number = line_number;

	// Linking the new node to current top
	temp->next = stack_ptr->top;

	// Re-initialise the top pointer to new node
	stack_ptr->top = temp;

	// Update stack detail
	stack_ptr->length +=1;

	// Display after every stack update
	display(stack_ptr);
}

// Pop Function - Used to remove a node (from top)
void pop(struct stack *stack_ptr)
{
	// Error Handling.
	if(stack_ptr->length==0)
	{
		printf("Stack Length already 0\n");
	}

	// Removing the node
	else
	{
		// Initialising temporary pointer at top.
		struct node *temp = stack_ptr->top;

		// Updating the stack top to the next node.
		stack_ptr->top = stack_ptr->top->next;

		// Deleting the original node.
		free(temp);

		// Updating value.
		stack_ptr->length -=1;
	}

	// Display after every stack update
	display(stack_ptr);
}

// Function to swap values.
void swap(int* a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Partition Function to sort a partion 
int partition(int array[],int low ,int high)
{
	// Randomizer for the *Randomised* Quick Sort Line (133-137)
	srand(time(NULL));
	// Transformation for random number b/w low and high.
	int random_pivot = (rand()%(high-low)) + low;
	// Swapping the random pivot position to high value.
	swap(&array[random_pivot],&array[high]);
	
	// Sorting
	int pivot = array[high];
	int i =low-1;
	for(int j=low;j<high;j++)
	{
		if (array[j]<pivot)
		{
			i++;
			swap(&array[i],&array[j]);
		}
	}
	swap(&array[i+1],&array[high]);

	// Retuning Pivot Position
	return (i+1);
}

// Recursive Function
void quick_sort(int array[],int low ,int high,int line_number,struct stack *stack_ptr)
{
	// Pushing the function in the stack.
	push(stack_ptr,low,high,line_number);
	
	if(low<high)
	{
		int pivot = partition(array,low,high);

		quick_sort(array,low,pivot-1,__LINE__,stack_ptr);
		quick_sort(array,pivot+1,high,__LINE__,stack_ptr);
	}

	// Popping Function out of the stack. 
	pop(stack_ptr);
}

// The main function
int main()
{
	// Pointer for the stack (qsort_status)
	struct stack *func_status = &qsort_status;
	// Stack values initialisation
	qsort_status.length=0;
	qsort_status.top = NULL;

	/*User Input*/
	int n;
	printf("Enter the length of array\n");
	scanf("%d",&n);
	// Error Handling
	if(n<1)
	{
		printf("Array Length is invalid/short\n");
	}

	else
	{
		// Input Array
		int arr[n];
		printf("Enter the array members\n");
		for (int i = 0; i < n; ++i)
		{
			scanf("%d",&arr[i]);
		}

		// Output Format
		printf("\nStack:\n");

		// Stack Display (before starting)
		display(func_status);

		// Calling the sort function
		quick_sort(arr,0,n-1,__LINE__,func_status);

		// Output Format
		printf("\nThe Sorted Array:\n");
		// Printing the Array.
		for (int i = 0; i < n; ++i)
		{
		 	printf("%d ",arr[i]);
		} 
		printf("\n");
	}
	return 0;
}