#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// stdbool used for boolean operations
// string library used for string operations


// Structures Defined here

// Element of any matrix
struct matrix_element
{
	// Data Values 
	int data;

	// Pointers to store adjacent addresses
	struct matrix_element *right_element;
	struct matrix_element *down_element;

};

// Row for any matrix
struct matrix_row
{
	// Pointers to store element addresses
	struct matrix_element *head;
	struct matrix_element *current;

	// Pointer for next row
	struct matrix_row *next_row;
};

// Any Matrix
struct matrix
{
	struct matrix_row *head_row;
	struct matrix_row *current_row;

	// Count of number of rows/columns.
	int row;
	int column;
}matrix_a,matrix_b,matrix_output;


// Function to check for square matrices.
bool is_square(struct matrix *matrix_num, FILE* fileptr)
{
	// Simple binary condition using if-else
	if (matrix_num->row==matrix_num->column)
	{
		fprintf(fileptr,"Is Square.\n");
		return true;
	}
	else
	{
		fprintf(fileptr,"Is NOT a Square.\n");
		return false;
	}
}

// Function to print any matrix
void print_matrix(struct matrix *matrix_num, FILE* fileptr)
{
	// Re-initialize pointers to start
	matrix_num->current_row = matrix_num->head_row;
	matrix_num->current_row->current = matrix_num->current_row->head;

	// Loop through the rows
	while(matrix_num->current_row)
	{
		// Loop throught the elements of each row
		while(matrix_num->current_row->current)
		{
			// Printing elements in a file
			fprintf(fileptr,"%d ",matrix_num->current_row->current->data);

			// Iteration update expression in pointers (to the next element)
			matrix_num->current_row->current = matrix_num->current_row->current->right_element;
		}
		fprintf(fileptr,"\n");
	
		// Iteration update expression in pointers (to the next row)
		matrix_num->current_row = matrix_num->current_row->next_row;
		if(matrix_num->current_row)
		{
			// Re-initialize element pointer to the row-head
			matrix_num->current_row->current = matrix_num->current_row->head;
		}
	}

}

// Function for multiplying matrices
bool multiply_matrix(struct matrix *matrix_1,struct matrix *matrix_2,struct matrix *matrix_prod, FILE* fileptr)
{

	// Validation of posibility of multiplication
	if (matrix_1->column!=matrix_2->row)
	{
		fprintf(fileptr,"Multiplication not possible\n");
		return false;
	}

	// Calculating the row/column count
	matrix_prod->row=matrix_1->row;
	matrix_prod->column=matrix_2->column;

	// Initialized dummy variable to store data for elements of new array
	int element_data = 0;

	// New pointers to store multiplication row/column heads
	struct matrix_element *multiply1_head = matrix_1->head_row->head;
	struct matrix_element *multiply2_head = matrix_2->head_row->head;

	// Pointers for storing the previous element/row in prod matrix (to form a linked list-like structure)
	struct matrix_element *previous = NULL;
	struct matrix_row *previous_row = NULL;

	// Iterating thro' each row of new matrix
	for (int i = 0; i < (matrix_prod->row); ++i)
	{
		// Updating of multiply heads
		if(i!=0)
		multiply1_head = multiply1_head->down_element;

		multiply2_head = matrix_2->head_row->head;

		// Allocation of memory for new matrix (rows)
		struct matrix_row *temp_row = (struct matrix_row*) malloc(sizeof(struct matrix_row));
		temp_row->next_row = NULL;

		// Head pointer update (i==0)
		if (i==0)
		{
			matrix_prod->head_row = temp_row;
		}

		// Prev. row pointer update
		else
		{
			previous_row->next_row = temp_row;
		}

		// Iterate the current row pointer to newly allocated memory.
		matrix_prod->current_row = temp_row;

		// Iterating thro' each element of a row in new matrix
		for (int j = 0; j < (matrix_prod->column); ++j)
		{
			// Updating of multiply heads
			if(j!=0)
			{
				multiply2_head = multiply2_head->right_element;
			}

			//  Allocation of memory for new matrix (elements)
			struct matrix_element *temp = (struct matrix_element*) malloc(sizeof(struct matrix_element));
			
			// Head pointer update
			if (j==0)
			{
				matrix_prod->current_row->head = temp;
			}

			// Prev. element pointer update.
			else
			{
				previous->right_element = temp;
			}
			
			// Iterate the current pointer to newly allocated memory.
			matrix_prod->current_row->current = temp;

			// New pointers to iterate thro' multiply row/column
			struct matrix_element *multiply1 = multiply1_head;
			struct matrix_element *multiply2 = multiply2_head;

			// Iteration for each element to get sum of products
			for (int k = 0; k <(matrix_1->column); ++k)
			{
				element_data+=((multiply1->data)*(multiply2->data));

				// Updating position of pointers 
				if(k+1<(matrix_1->column))
				{
					multiply1=multiply1->right_element;
					multiply2=multiply2->down_element;
				}
			}

			// Updating data
			temp->data = element_data;
			element_data=0;

			// Saving the element address in prev pointer
			previous = temp;

		}
		
		// Saving the row address in prev pointer
		previous_row=temp_row;
	}
		
	return true;
}

// Funtion to check for symmetric matrices
void is_symmetric(struct matrix *matrix_num, FILE* fileptr)
{	
	 // Pointer for diagonal elements
	struct matrix_element *temp_start = matrix_num->head_row->head;

	// Pointers going down and right w.r.t. diagonal element
	struct matrix_element *temp1 = temp_start;
	struct matrix_element *temp2 = temp_start;

	// Boolean value of condition 
	bool symmetry = true;

	// Iteration for diagonal elements
	for (int i = 0; i < matrix_num->row; ++i)
	{
		// Iteration for mirroring pointers (in diagonal)
		for (int j = 0; i+j< matrix_num->row; ++j)
		{
			// Checking for un-symmetry
			if(temp1->data!=temp2->data)
			{
				symmetry = false;

				// Attempt to break thro' the outer loop
				// Since C doesn't allow breaking nested loops
				goto outer;
			}

			// Iteration update expression in mirror pointers
			temp1 = temp1->right_element;
			temp2 = temp2->down_element;
		}
		// Iteration update expression in diagonal pointer
		if (temp_start->down_element)
		{
			temp_start = temp_start->right_element;
			temp_start = temp_start->down_element;

			// Re-initialize mirror pointers at diagonal
			temp1 = temp_start;
			temp2 = temp_start;
		}
	}
	// Label to break outer loop
	outer: ;

	// Printing of result (in file)
	if (symmetry)
	{
		fprintf(fileptr, "Is Symmetric.\n");
	}
	else
	{
		fprintf(fileptr, "Is NOT Symmetric.\n");
	}

}

// Funtion fo input of matrix.
void matrix_entry(char string[], int length, struct matrix *matrix_num)
{
	// Dummy Variable to store marix data
	int element_data = 0;

	// Previous row pointer
	struct matrix_row  *previous_row = NULL;
	
	// Allocating memory for first row and pointing as current
	matrix_num->head_row = (struct matrix_row *) malloc(sizeof(struct matrix_row));
	matrix_num->current_row = matrix_num->head_row;

	// Allocating memory for first element of first row and pointing as current
	matrix_num->current_row->head= (struct matrix_element *) malloc(sizeof(struct matrix_element));
	matrix_num->current_row->current = matrix_num->current_row->head;

	// Initializing count for the row/column
	matrix_num->row=1;
	matrix_num->column=1;

	// Boolean for considering negative int
	bool is_negative = false;

	// Iterating thro' string
	for (int i=0;i<length;++i) 
	{
		// Check for ',' for new element
		if(string[i]==',')
		{
			// Allocating memory for new element
			struct matrix_element *temp = (struct matrix_element *) malloc(sizeof(struct matrix_element));
			
			// Updating the values for current element
			if (is_negative) 
			{
				element_data*=-1;
				is_negative = false;
			}
			matrix_num->current_row->current->data = element_data;	
			matrix_num->current_row->current->right_element = temp;

			// Pointer Iteration to next element and updating values
			matrix_num->current_row->current = matrix_num->current_row->current->right_element;
			matrix_num->current_row->current->right_element = NULL;
			matrix_num->current_row->current->down_element = NULL;			
			element_data=0;
			matrix_num->column++;

			// Updationg pointer values for prev. row and its elements
			if (previous_row)
			{
				previous_row->current = previous_row->current->right_element;
				previous_row->current->down_element = matrix_num->current_row->current;
			}
		}

		// Check for ';' for new rows
		else if(string[i]==';')
		{
			// Updating values for current element
			if (is_negative) 
			{
				element_data*=-1;
				is_negative = false;
			}
			matrix_num->current_row->current->data = element_data;

			// Allocating memory for new row and updating previous row values
			struct matrix_row *temp_row = (struct matrix_row *) malloc(sizeof(struct matrix_row));
			matrix_num->current_row->next_row = temp_row;
			previous_row = matrix_num->current_row;
			matrix_num->current_row = matrix_num->current_row->next_row;

			// Allocating memory for new element,updating pointers and re-initializing row head/current pointers
			matrix_num->current_row->head = (struct matrix_element *) malloc(sizeof(struct matrix_element));
			matrix_num->current_row->current = matrix_num->current_row->head;
			matrix_num->current_row->current->right_element = NULL;
			matrix_num->current_row->current->down_element = NULL;
			previous_row->head->down_element = matrix_num->current_row->head;
			previous_row->current=previous_row->head;

			// Updating data of the matrix
			element_data=0;
			matrix_num->column++;
			matrix_num->row++;

		} 

		// Check for numerical value to be stored
		else if(string[i]>='0'&&string[i]<='9')
		{
			element_data*=10;
			element_data+= (string[i]-'0');
		}

		// Check for negative values
		else if (string[i]=='-')
		{
			is_negative = true;
		}

		// Check for the last element of matrix
		else
		{
			// Updating values
			if (is_negative) 
			{
				element_data*=-1;
				is_negative = false;
			}
			matrix_num->current_row->current->data = element_data;
			element_data=0;
		}		
	}

	// Getting actual count of columns (no. of entries/rows)
	matrix_num->column/=matrix_num->row;

}

// The main function
int main(int argc, char const *argv[])
{
	// File pointers for file handling
	FILE *input1_file,*input2_file,*output_file;

	// Error handling for file pointers
	if ((input1_file=fopen("input1.txt","r"))==NULL)
	{
		printf("File 1 couldn't be found\n");
	}
	else if ((input2_file=fopen("input2.txt","r"))==NULL)
	{
		printf("File 2 couldn't be found\n");
	}


	else
	{	
		output_file=fopen("output.txt","w+");

		// input strings for matrices
		char matrix_input_1[1000];
		char matrix_input_2[1000];
		fscanf(input1_file,"%s",matrix_input_1);
		fscanf(input2_file,"%s",matrix_input_2);
		// Concatenation for extra iteration for last element.
		strcat(matrix_input_1," ");
		strcat(matrix_input_2," ");

		// Calling entry/input function
		matrix_entry(matrix_input_1,strlen(matrix_input_1), &matrix_a);
		matrix_entry(matrix_input_2,strlen(matrix_input_2), &matrix_b);

		// Printing for Matrix A
		fprintf(output_file,"\n-------------\nMatrix A\n-------------\n");
		print_matrix(&matrix_a,output_file);
		fprintf(output_file,"\n");
		if(is_square(&matrix_a,output_file))
		{
			is_symmetric(&matrix_a,output_file);
		}

		// Printing for Matrix B
		fprintf(output_file,"\n-------------\nMatrix B\n-------------\n");
		print_matrix(&matrix_b,output_file);
		fprintf(output_file,"\n");
		if(is_square(&matrix_b,output_file))
		{
			is_symmetric(&matrix_b,output_file);
		}

		// Prining Output Matrix
		fprintf(output_file,"\n---------------\nProduct Matrix\n---------------\n");
		if(multiply_matrix(&matrix_a,&matrix_b,&matrix_output,output_file))
		{
			print_matrix(&matrix_output,output_file);
		}

		fclose(output_file);
	}

	fclose(input1_file);
	fclose(input2_file);

	return 0;
}
