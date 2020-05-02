#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// A Node Structure for the Stack
struct onode
{
	// A character
	char o;
	// An address to the next node (implementation of singly-linked list)
	struct onode *next;
};

// A Stack Structure holding the Nodes 
struct ostack
{
	// Length of Stack
	int length;

	// A pointer to the Top Node of Stack
	struct onode *top;
};

// A Node Structure for the Stack
struct nnode
{
	// A number
	double num;
	// An address to the next node (implementation of singly-linked list)
	struct nnode *next;
};

// A Stack Structure holding the Nodes 
struct nstack
{
	// Length of Stack
	int length;

	// A pointer to the Top Node of Stack
	struct nnode *top;
};

// Categorising the character
char categorize(char c)
{
	// Left Bracket
	if(c=='('||c=='{'||c=='[') return 'l';
	// Right bracket
	else if(c==')'||c=='}'||c==']') return 'r';
	// Operators
	else if(c=='+'||c=='-'||c=='*'||c=='/'||c=='^') return 'o';
	// Numbers
	else if(c>='0'&&c<='9') return 'n';
	// Space
	else if(c==' ') return 's';
	// Invalid
	else return 'i';
}

// Returning the complementary braket
char bracket_inverse(char c)
{
	switch(c)
	{
		case '(': return ')';
		case ')': return '(';
		case '{': return '}';
		case '}': return '{';
		case '[': return ']';
		case ']': return '[';
		default : return 'i';
	}
}

// Priority according to BODMAS + Exp
int priority(char c)
{
	if(c=='+'||c=='-') return 1;
	else if(c=='*'||c=='/') return 2;
	else if(c=='^') return 3;
	else return (-1);
}


// Push Function - Used to add a node
void opush(struct ostack *stack_ptr,char o)
{
	// Memory allocation
	struct onode *temp = (struct onode *) malloc(sizeof(struct onode));
	
	// Allocation fail check.
	if (temp == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(0); 
	}

	// Updating other value
	temp->o = o;
	
	// Linking the new node to current top
	temp->next = stack_ptr->top;

	// Re-initialise the top pointer to new node
	stack_ptr->top = temp;

	// Update stack detail
	stack_ptr->length +=1;

}

// Pop Function - Used to remove a node (from top)
void opop(struct ostack *stack_ptr)
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
		struct onode *temp = stack_ptr->top;

		// Updating the stack top to the next node.
		stack_ptr->top = stack_ptr->top->next;

		// Deleting the original node.
		free(temp);

		// Updating value.
		stack_ptr->length -=1;
	}
}


// Push Function - Used to add a node
void npush(struct nstack *stack_ptr,double num)
{
	// Memory allocation
	struct nnode *temp = (struct nnode *) malloc(sizeof(struct nnode));
	
	// Allocation fail check.
	if (temp == NULL)
	{
		printf("Memory allocation fail.\n"); 
    	exit(0); 
	}

	// Updating value
	temp->num = num;
	
	// Linking the new node to current top
	temp->next = stack_ptr->top;

	// Re-initialise the top pointer to new node
	stack_ptr->top = temp;

	// Update stack detail
	stack_ptr->length +=1;
}

// Pop Function - Used to remove a node (from top)
void npop(struct nstack *stack_ptr)
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
		struct nnode *temp = stack_ptr->top;

		// Updating the stack top to the next node.
		stack_ptr->top = stack_ptr->top->next;

		// Deleting the original node.
		free(temp);

		// Updating value.
		stack_ptr->length -=1;
	}
}

// Checking if the expression is valid
bool is_valid(char* infix_expression, int length)
{
	bool valid = true;

	/*

	Category 1
	// (Non parenthesial order related)

	*/

	char last_char=categorize(infix_expression[0]);
	char curr_char=categorize(infix_expression[0]);

	// All the statements in printf are explanatory
	if(curr_char=='r'||curr_char=='o')
	{
		printf("Incorrect Start of expression at position :0\n");
		valid = false;
		return valid;
	}
	if(curr_char=='i')
	{
		printf("Unknown expression %c at position :1\n", infix_expression[0]);
		valid = false;
		return valid;
	}
	for (int i = 1; i < length; i++)
	{
		if(infix_expression[i]==' ') continue;

		

		curr_char = categorize(infix_expression[i]);

		if(curr_char=='i')
		{
			printf("Unknown expression %c at position :%d\n", infix_expression[i],i+1);
			valid = false;
			return valid;
		}

		if(curr_char=='n');
		else if(curr_char=='o')
		{
			if(last_char=='o') 
			{	
				printf("Multiple operator usage at position :%d\n", i+1);
				valid = false;
				return valid;
			}
			else if(last_char=='l')
			{
				printf("No operand found at position :%d\n", i);
				valid = false;
				return valid;
			}
		}
		else if(curr_char=='l')
		{
			if(last_char=='n'||last_char=='r') 
			{	
				printf("No operator found at position :%d\n", i);
				valid = false;
				return valid;
			}
		}
		else if(curr_char=='r')
		{
			if(last_char=='l'||last_char=='o') 
			{	
				printf("No operand found at position :%d\n", i);
				valid = false;
				return valid;
			}
		}
		last_char = curr_char;
	}


	/*

	Category 2
	// (Parenthesial order related)
	
	*/


	// Stack for parenthesis
	struct ostack parenthesis;
	parenthesis.length=0;
	parenthesis.top = NULL;

	// All the printf statements are explanatory.
	for(int i=0;i<length;i++)
	{
		char bracket = infix_expression[i];
		if(bracket =='('||bracket =='['||bracket =='{')
		{
			opush(&parenthesis,bracket);
		}
		else if(bracket == ')')
		{
			if(parenthesis.length == 0||parenthesis.top->o != '(')
			{	
				printf("No matching '(' found for ')' at position :%d\n", i);
				valid = false;
				return valid;
			}
			else
			{
				opop(&parenthesis);
			}
		}
		else if(bracket == '}')
		{
			if(parenthesis.length == 0||parenthesis.top->o != '{')
			{	
				printf("No matching '{' found for '}' at position :%d\n", i);
				valid = false;
				return valid;
			}
			else
			{
				opop(&parenthesis);
			}
		}
		else if(bracket == ']')
		{
			if(parenthesis.length == 0||parenthesis.top->o != '[')
			{	
				printf("No matching '[' found for ']' at position :%d\n", i);
				valid = false;
				return valid;
			}
			else
			{
				opop(&parenthesis);
			}
		}
	
	}

	// If there are parenthesis unclosed
	if((parenthesis.length)>0)
	{
		printf("%d Closing parenthesis missing at position :%d\n", parenthesis.length,length);
		valid = false;
		return valid;
	}

	return valid;
}

// Changing infix to postfix
void infix_to_postfix(char *infix_expression, int length, char output[])
{
	char char_type;
	char space = ' ';

	// Operator stack declared
	struct ostack operators;
	operators.length = 0;
	operators.top = NULL;

	for (int i=0;i<length;i++)
	{
		char_type = categorize(infix_expression[i]);

		// Add all the nums to string
		if(char_type == 'n')
		{
			strncat(output,infix_expression+i,1);
		}

		// On reaching Left Bracket push it in the stack
		else if(char_type == 'l')
		{
			opush(&operators,infix_expression[i]);
		}

		// On reaching Righ Bracket pop the stack and print the operators till
		// corresponding left bracket is found
		else if(char_type == 'r')
		{
			while((operators.length)>0 && operators.top->o != bracket_inverse(infix_expression[i]))
            {
            	char c = operators.top->o;
                opop(&operators); 
                strncat(output,&space,1);
                strncat(output,&c,1);
            }
            // Pop the left bracket
            opop(&operators); 
		}

		// Push all operators of higher precedence than previous
		// Else pop and print till the correct precendence is reached.
		// And then push it in the stack
		else if(char_type == 'o')
		{
			strncat(output,&space,1);

			while ((operators.length)>0 && priority(infix_expression[i]) <= priority(operators.top->o)) 
            {
            	char c = operators.top->o;
                opop(&operators); 
                strncat(output,&c,1);
                strncat(output,&space,1);
			}
			opush(&operators,infix_expression[i]);	
		}
	}

	// Pop and print all the operators left in the stack
	while ((operators.length)>0) 
    {
		char c = operators.top->o;
		opop(&operators); 
		strncat(output,&space,1);
		strncat(output,&c,1);
	}
}

// Calculating the postfix expression
void postfix_calculation(char *postfix_expression, int length)
{
	// Creating a number stack
	struct nstack operands;
	operands.length = 0;
	operands.top = NULL;

	// Char type
	char curr_char;
	char last_char;

	//  A temporary number holder
	int temp=0;
	for (int i = 0; i < length; ++i)
	{
		curr_char = categorize(postfix_expression[i]);

		// If num is encountered save it in temp as 10times num + this num
		if(curr_char == 'n') 
		{
			// Converting digits to number
			temp = (temp*10) + (postfix_expression[i]-'0');
		}

		// If the number is finished push it in the stack
		else if(curr_char == 's'&& last_char == 'n')
		{
			npush(&operands,temp);
			temp = 0;
		}

		// Calculating in case of operators.
		else if(curr_char == 'o')
		{
			// Pop the numbers from stack
			double num1,num2;
			num2 = operands.top->num;
			npop(&operands);
			num1 = operands.top->num;
			npop(&operands);

			switch(postfix_expression[i])
			{
				case '+': 	npush(&operands,num1+num2);
							break;
				case '-':	npush(&operands,num1-num2);
							break;
				case '*':	npush(&operands,num1*num2);
							break;
				case '/':	if(num2!=0)
							{
								npush(&operands,num1/num2);
								break;
							}
							else
							{
								// Errors handling
								printf("Dividend is zero\n");
								printf("Program being terminated\n");
								exit(2);
							}
				case '^':	if(num1==0&&num2<=0)
							{
								// Errors handling
								printf("Only positive numbers are allowed for Zero's exponent\n");
								printf("Program being terminated\n");
								exit(3);
							}
							else if(num1<=0 && (num2!=(int)num2))
							{
								// Errors handling
								printf("Fractional exponents for negative is not allowed\n");
								printf("Program being terminated\n");
								exit(3);
							}
							else
							{
								npush(&operands,pow(num1,num2));
								break;
							}
			}
		}

		last_char = curr_char;
	}
	printf("Value = %.2lf (Rounded to 2 decimal places)\n",operands.top->num);
	npop(&operands);
}

int main()
{
	// Input char array
	char infix_expression[100];

	char space = ' ';
	scanf(" %s",infix_expression);
	int input_length = strlen(infix_expression);

	// Check validity
	if(!is_valid(infix_expression,input_length))
	{
		printf("INVALID\n");;
	}
	else
	{
		// Postfix char array
		char postfix_expression[100] = "";

		// Changing expression
		infix_to_postfix(infix_expression, input_length, postfix_expression);

		strncat(postfix_expression,&space,1);
		printf("\nPostfix Expression:\n%s\n",postfix_expression );

		// Calculation
		postfix_calculation(postfix_expression,strlen(postfix_expression));
	} 
	return 0;
}