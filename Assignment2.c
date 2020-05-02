#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct matrix_element
{
	int data;
	struct matrix_element *right_element;
	struct matrix_element *down_element;

};
struct matrix_row
{
	struct matrix_element *head;
	struct matrix_element *current;
	struct matrix_row *next_row;
};

struct matrix
{
	struct matrix_row *head_row;
	struct matrix_row *current_row;
	int row;
	int column;
}matrix_a,matrix_b,matrix_output;

bool is_square(struct matrix *matrix_num, FILE* fileptr)
{
	if (matrix_num->row==matrix_num->column)
	{
		fprintf(fileptr,"Is Square.\n");
		fprintf(fileptr, "%d\n", matrix_num->row);
		return true;
	}
	else
	{
		fprintf(fileptr,"Is NOT Square\n");
		return false;
	}
}
void print_matrix(struct matrix *matrix_num, FILE* fileptr)
{
	matrix_num->current_row = matrix_num->head_row;
	matrix_num->current_row->current = matrix_num->current_row->head;
	while(matrix_num->current_row)
	{
		while(matrix_num->current_row->current)
		{
			printf("%d",matrix_num->current_row->current->data);
			matrix_num->current_row->current = matrix_num->current_row->current->right_element;
		}
		printf("\n");
		
		matrix_num->current_row = matrix_num->current_row->next_row;
		if(matrix_num->current_row)
		{
			matrix_num->current_row->current = matrix_num->current_row->head;
		}
	}
}
void multiply_matrix(struct matrix *matrix_1,struct matrix *matrix_2,struct matrix *matrix_prod)
{
	if (matrix_1->column!=matrix_2->row)
	{
		printf("Multiplication not possible\n");
		return;
	}
	else
	{
		matrix_prod->row = matrix_1->row;
		matrix_prod->column = matrix_2->column;

		matrix_1->current_row = matrix_1->head_row;
		matrix_1->current_row->current = matrix_1->current_row->head;
		matrix_2->current_row = matrix_2->head_row;
		matrix_2->current_row->current = matrix_2->current_row->head;

		struct matrix_element *data1, *data2, *previous;
		struct matrix_row *previous_row;

		data1 = matrix_1->current_row->head;
		data2 = matrix_2->current_row->head;
		int element_data = 0;
		for (int i = 0; i < matrix_prod->row; ++i)
		{
			// printf("ITER%d\n",i+1 );
			matrix_prod->current_row = (struct matrix_row *) malloc(sizeof(struct matrix_row));
			if(i==0)
			{
				matrix_prod->head_row = matrix_prod->current_row;
			}
			else
			{
				previous_row->next_row = matrix_prod->current_row;
			}
				printf("hii\n");

			for (int j = 0; j < matrix_prod->column; ++j)
			{
				matrix_prod->current_row->current= (struct matrix_element *) malloc(sizeof(struct matrix_element));
				if(j==0)
				{
					matrix_prod->current_row->head = matrix_prod->current_row->current;
				}
				else
				{
					previous->right_element = matrix_prod->current_row->current;
				}
				for (int k = 0; k < matrix_1->column; ++k)
				{
					element_data+= ((data1->data)*(data2->data));
					if(data1)
					{
						printf("%d %d\n",data1->data,data2->data);
						data1 = data1->right_element;
						data2 = data2->down_element;
						// printf("%d\n",element_data);
					}
				}
				printf("\n%d\n\n",element_data);
				matrix_prod->current_row->current->data = element_data;
				element_data = 0;
				previous = matrix_prod->current_row->current;
				data1 = matrix_1->current_row->head;
				data2 = matrix_2->current_row->head;
				// printf("%d ",element_data);
				for (int k = 0; k <= j; ++k)
				{
					if(data2->right_element)
						data2 = data2->right_element;
				}

			}
			previous_row = matrix_prod->current_row; 
			// data1 = matrix_1->current_row->head;
			for (int j = 0; j <= i; ++j)
			{
				data1= data1->down_element;
			}

		}
		// printf("%d\n",matrix_prod->head_row->head->data );
	}
}
void is_symmetric(struct matrix *matrix_num, FILE* fileptr)
{	
	 
	struct matrix_element *temp_start = matrix_num->head_row->head;
	struct matrix_element *temp1 = temp_start;
	struct matrix_element *temp2 = temp_start;
	bool symmetry = true;

	for (int i = 0; i < matrix_num->row; ++i)
	{
		for (int j = 0; i+j< matrix_num->row; ++j)
		{
			if(temp1->data!=temp2->data)
			{
				symmetry = false;
				goto outer;
			}
			temp1 = temp1->right_element;
			temp2 = temp2->down_element;
		}
		if (temp_start->down_element)
		{
			temp_start = temp_start->right_element;
			temp_start = temp_start->down_element;
			temp1 = temp_start;
			temp2 = temp_start;
		}
	}
	outer: ;
	if (symmetry)
	{
		fprintf(fileptr, "Is Symmetric\n");
	}
	else
	{
		fprintf(fileptr, "Is NOT Symmetric\n");
	}

}

void matrix_entry(char string[], int length, struct matrix *matrix_num)
{
	int element_data = 0;
	struct matrix_row  *previous_row = NULL;
	matrix_num->head_row = (struct matrix_row *) malloc(sizeof(struct matrix_row));
	matrix_num->current_row = matrix_num->head_row;
	matrix_num->current_row->head= (struct matrix_element *) malloc(sizeof(struct matrix_element));
	matrix_num->current_row->current = matrix_num->current_row->head;
	matrix_num->row=1;
	matrix_num->column=1;
	bool is_negative = false;
	for (int i=0;i<length;++i) 
	{
	
		if(string[i]==',')
		{
			struct matrix_element *temp = (struct matrix_element *) malloc(sizeof(struct matrix_element));
			if (is_negative) element_data*=-1;
			matrix_num->current_row->current->data = element_data;	
			// printf("%d ",matrix_num->current_row->current->data );		
			matrix_num->current_row->current->right_element = temp;
			matrix_num->current_row->current = matrix_num->current_row->current->right_element;
			matrix_num->current_row->current->right_element = NULL;
			matrix_num->current_row->current->down_element = NULL;			
			element_data=0;
			matrix_num->column++;
			if (previous_row)
			{
				previous_row->current = previous_row->current->right_element;
				previous_row->current->down_element = matrix_num->current_row->current;
			}
		}
		else if(string[i]==';')
		{
			if (is_negative) element_data*=-1;
			matrix_num->current_row->current->data = element_data;
			// printf("%d\n",matrix_num->current_row->current->data );	
			struct matrix_row *temp_row = (struct matrix_row *) malloc(sizeof(struct matrix_row));
			matrix_num->current_row->next_row = temp_row;
			previous_row = matrix_num->current_row;
			matrix_num->current_row = matrix_num->current_row->next_row;
			matrix_num->current_row->head = (struct matrix_element *) malloc(sizeof(struct matrix_element));
			matrix_num->current_row->current = matrix_num->current_row->head;
			matrix_num->current_row->current->right_element = NULL;
			matrix_num->current_row->current->down_element = NULL;
			previous_row->head->down_element = matrix_num->current_row->head;
			previous_row->current=previous_row->head;
			element_data=0;
			matrix_num->column++;
			matrix_num->row++;

		} 
		else if(string[i]>='0'&&string[i]<='9')
		{
			element_data*=10;
			element_data+= (string[i]-'0');
		}
		else if (string[i]='-')
		{
			is_negative = true;
		}
		else
		{
			printf("%d\n", element_data);
			if (is_negative) 
			{
				element_data*=-1;
				is_negative = false;
			}
			matrix_num->current_row->current->data = element_data;
			element_data=0;
		}

		// printf("%d ",matrix_num->current_row->current->data);
		// printf("%d %c\n",i,string[i]);
		
	}
	matrix_num->column/=matrix_num->row;
	// printf("%d %d\n",matrix_num->row,matrix_num->column);
}

int main(int argc, char const *argv[])
{

	//  Take care of negative elements
	//  DO SO
	FILE *input1_file,*input2_file,*output_file;

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
		char matrix_input_1[100];
		char matrix_input_2[100];
		fscanf(input1_file,"%s",matrix_input_1);
		fscanf(input2_file,"%s",matrix_input_2);
		strcat(matrix_input_1," ");
		strcat(matrix_input_2," ");
		matrix_entry(matrix_input_1,strlen(matrix_input_1), &matrix_a);
		matrix_entry(matrix_input_2,strlen(matrix_input_2), &matrix_b);

		fprintf(output_file,"\nMatrix A\n");
		print_matrix(&matrix_a,output_file);
		printf("\n");
		print_matrix(&matrix_b,output_file);
		// print_matrix(&matrix_output,output_file);
		if(is_square(&matrix_a,output_file))
		{
			// is_symmetric(&matrix_a,output_file);
		}

		fprintf(output_file,"\nMatrix B\n");
		if(is_square(&matrix_b,output_file))
		{
			// is_symmetric(&matrix_b,output_file);
		}
		multiply_matrix(&matrix_a,&matrix_b,&matrix_output);
		// printf("%d\n",matrix_output.head_row->next_row->head->data);
	}
	return 0;
}
