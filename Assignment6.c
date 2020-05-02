#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A struct for the node of BST
struct node
{
	// The numeral value
	int key;

	// Pointer to the adjacent nodes/relatives
	struct node *parent;
	struct node *left_child;
	struct node *right_child;
};


// A struct for the BST
struct bin_search_tree
{
	// Pointer to the root node
	struct node *root;

	// Number of nodes in the tree.
	int num_nodes;
}
// Declaration of the Global BST for the program
tree;


// A function which helps search the address of a node with its key
// (Works similar to find(), but different return value/type)
struct node* search(struct bin_search_tree *tree_ptr, int key)
{
	// No node check
	if(tree_ptr->num_nodes<1)
	{
		return NULL;
	}

	// Node pointer to traverse/search
	struct node *curr_node = tree_ptr->root;

	// Logic to go either left/right or stop.
	while((curr_node->key)!= key)
	{
		if((curr_node->key) == key)
		{
				break;
		}
		else if((curr_node->key) > key)
		{
			if(curr_node->left_child)
			{
				curr_node = curr_node->left_child;
				continue;
			} 
			else
			{
				return NULL;
			}
		}

		else if((curr_node->key) < key)
		{
			if(curr_node->right_child)
			{
				curr_node = curr_node->right_child;
				continue;
			}
			else
			{
				return NULL;
			}
		}
	}

	// NULL means the node couldn't be found.
	return curr_node;
}


// Inserts a node in the tree.
void insert(struct bin_search_tree *tree_ptr, int key, FILE *output_file)
{
	bool added = false;
	if(tree_ptr->num_nodes > 0)
	{	
		// Initialising the searcher node
		struct node *curr_node = tree_ptr->root;
	
		// logic to find the coorect place to add.
		while(!added){
			if((curr_node->key) == key) 
			{
				fprintf(output_file,"false\n");
				return;
			}
			else if((curr_node->key) > key)
			{
				if(curr_node->left_child)
				{
					curr_node = curr_node->left_child;
					continue;
				}
				// addtion spot
				else
				{
					struct node *newnode = (struct node *) malloc(sizeof(struct node));
					if (newnode == NULL)
					{
						printf("Memory allocation fail.\n");
						fprintf(output_file,"false\n");
				    	return; 
					}

					// Update the struct node values
					curr_node->left_child = newnode;
					newnode->key = key;
					newnode->left_child = NULL;
					newnode->right_child =NULL;
					newnode->parent = curr_node;

					added = true;
					
					// Update the tree values
					tree_ptr->num_nodes += 1;
					break;
				}

			}
			else if((curr_node->key) < key)
			{
				if(curr_node->right_child)
				{
					curr_node = curr_node->right_child;
					continue;
				}

				// addtion spot
				else
				{
					struct node *newnode = (struct node *) malloc(sizeof(struct node));
					if (newnode == NULL)
					{
						printf("Memory allocation fail.\n");
						fprintf(output_file,"false\n");
				    	return; 
					}

					// Update the struct node values
					curr_node->right_child = newnode;
					newnode->key = key;
					newnode->left_child = NULL;
					newnode->right_child =NULL;
					newnode->parent = curr_node;
					
					added = true;

					// Update the tree values
					tree_ptr->num_nodes += 1;
					break;
				}

			}
		}
		
		fprintf(output_file,"true\n");
	}

	else
	{
		struct node *newnode = (struct node *) malloc(sizeof(struct node));
		if (newnode == NULL)
		{
			printf("Memory allocation fail.\n");
			fprintf(output_file,"false\n");
		   	return; 
		}

		// Update the struct node values
		tree_ptr->root = newnode;
		newnode->key = key;
		newnode->left_child = NULL;
		newnode->right_child =NULL;
		newnode->parent = NULL;
		
		added = true;

		// Update the tree values
		tree_ptr->num_nodes += 1;
		fprintf(output_file,"true\n");
	}
}

// "Helps" delete a node, hence the name Delete-Utility.
// (Keeps track for the recursion of replacements happening)
bool deleteUtil(struct bin_search_tree *tree_ptr,struct node *curr_node)
{
	bool deleted = false;

	// If the node couldn't be found
	if(!(curr_node))
	{
		return deleted;
	}
	else
	{
		
		// Case 1: (Predecessor / Left Child) does NOT exist
		if( !(curr_node->left_child) )
		{
			if(!(curr_node->parent))
			{
				tree_ptr->root == curr_node->right_child;
				if(curr_node->right_child)
				curr_node->right_child->parent == curr_node->parent;
			}
			else if((curr_node->parent->key) > curr_node->key)
			{
				curr_node->parent->left_child = curr_node->right_child;
				if(curr_node->right_child)
				curr_node->right_child->parent == curr_node->parent;
			}
			else if((curr_node->parent->key) < curr_node->key)
			{
				curr_node->parent->right_child = curr_node->right_child;
				if(curr_node->right_child)
				curr_node->right_child->parent == curr_node->parent;
			}
			free(curr_node);
			tree_ptr->num_nodes -= 1;
			deleted = true;
		}

		// Case 2: (Predecessor / Left Child) exists
		else
		{
			struct node* replace_node = curr_node->left_child;
			while(replace_node->right_child)
			{
				replace_node = replace_node->right_child; 
			}
			int temp_key = replace_node->key;
			deleted = deleteUtil(tree_ptr, replace_node);
			curr_node->key = temp_key;
		}
	}
	return deleted;
}

// The base delete-a-node function.
// [Uses deleteUtil() and search()]
void delete(struct bin_search_tree *tree_ptr, int key, FILE *output_file)
{
	// Finding the address of node to be deleted
	struct node *curr_node = search(tree_ptr, key);

	// If found.
	if(curr_node)
	{
		// If deleted.
		if(deleteUtil(tree_ptr, curr_node))
		{
			fprintf(output_file,"true\n");
			return;
		}
	}
	fprintf(output_file,"false\n");
	return;
}

// Supplementary Print function 1
void print_inorder(struct bin_search_tree *tree_ptr,struct node *curr_node, FILE *output_file)
{
	if(!curr_node)
	{
		return;
	}

	print_inorder(tree_ptr,curr_node->left_child,output_file);

	fprintf(output_file,"%d ",curr_node->key);

	print_inorder(tree_ptr,curr_node->right_child,output_file);
}

// Supplementary Print function 2
void print_preorder(struct bin_search_tree *tree_ptr,struct node *curr_node, FILE *output_file)
{
	if(!curr_node)
	{
		return;
	}

	fprintf(output_file,"%d ",curr_node->key);

	print_preorder(tree_ptr,curr_node->left_child,output_file);

	print_preorder(tree_ptr,curr_node->right_child,output_file);
}

// Supplementary Print function 3
void print_postorder(struct bin_search_tree *tree_ptr,struct node *curr_node, FILE *output_file)
{
	if(!curr_node)
	{
		return;
	}

	print_postorder(tree_ptr,curr_node->left_child,output_file);

	print_postorder(tree_ptr,curr_node->right_child,output_file);

	fprintf(output_file,"%d ",curr_node->key);
}

// Print function for the user query
// [Uses supplementary print functions]
void print_subtree(struct bin_search_tree *tree_ptr,int key, FILE *output_file)
{
	// Finding the root of sub-tree
	struct node *curr_node = search(tree_ptr,key);
	if(!curr_node)
	{
		fprintf(output_file,"false\n");
		return;
	}

	// Print Format.
	fprintf(output_file,"Inorder: ");
	print_inorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\nPreorder: ");
	print_preorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\nPostorder: ");
	print_postorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\n");
}

// Print function for the user query
// [Uses supplementary print functions]
void print_tree(struct bin_search_tree *tree_ptr, FILE *output_file)
{
	struct node *curr_node = tree_ptr->root;
	if(!curr_node)
	{
		fprintf(output_file,"false\n");
		return;
	}

	// Print Format.
	fprintf(output_file,"Inorder: ");
	print_inorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\nPreorder: ");
	print_preorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\nPostorder: ");
	print_postorder(tree_ptr,curr_node,output_file);
	fprintf(output_file,"\n");
}

// Find a node and its prints its depth.
void find(struct bin_search_tree *tree_ptr, int key, FILE *output_file)
{
	int depth = 0;

	// No node check
	if(tree_ptr->num_nodes<1)
	{
		fprintf(output_file,"false\n");
		return;
	}

	// Initialising the searcher node
	struct node *curr_node = tree_ptr->root;

	// Logic to go either left/right or stop.
	while((curr_node->key)!= key)
	{
		if((curr_node->key) == key)
		{
				break;
		}
		else if((curr_node->key) > key)
		{
			if(curr_node->left_child)
			{
				curr_node = curr_node->left_child;
				depth++;
				continue;
			}
			else
			{
				fprintf(output_file,"false\n");
				return;
			}
		}

		else if((curr_node->key) < key)
		{
			if(curr_node->right_child)
			{
				curr_node = curr_node->right_child;
				depth++;
				continue;
			}
			else
			{
				fprintf(output_file,"false\n");
				return;
			}
		}
	}

	fprintf(output_file,"true, depth=%d\n", depth);
	return;
}

// Supplementary function to count a node and it meta-children below
int count_sub_nodes(struct node *curr_node)
{
	// Simple recursion

	// Base Case
	if(!curr_node)
	{
		return 0;
	}

	return (count_sub_nodes(curr_node->left_child)+count_sub_nodes(curr_node->right_child)+1);
}

// Function to calculate the imbalance of sub-tree
// [Uses the count function]
void calculate_imbalance(struct bin_search_tree *tree_ptr,int key, FILE *output_file)
{
	// Search for the address of node.
	struct node *curr_node = search(tree_ptr, key);

	// If not found.
	if(!curr_node)
	{
		fprintf(output_file,"false\n");
		return;
	}

	// Simple arithmetical logic.
	int left = count_sub_nodes(curr_node->left_child);
	int right = count_sub_nodes(curr_node->right_child);
	fprintf(output_file,"%d\n", (left-right) );
}

// The main function.
int main()
{
	// Initialisation of struct values.
	tree.num_nodes=0;
	tree.root = NULL;

	// File operations
	FILE *input_file,*output_file;
	if ((input_file=fopen("input.txt","r"))==NULL)
	{
		printf("Input file couldn't be found\n");
		return 1;
	}
	output_file=fopen("output.txt","w+");

	// Number of queries
	int t;
	fscanf(input_file,"%d",&t);
	while(t--)
	{
		// Query type
		int q;
		int key;
		fscanf(input_file,"%d",&q);
		
		// Conditional queries.
		switch(q)
		{
			case 1:
				fscanf(input_file,"%d",&key);
				insert(&tree,key,output_file);
				break;
			case 2:
				fscanf(input_file,"%d",&key);
				delete(&tree,key,output_file);
				break;
			case 3:
				fscanf(input_file,"%d",&key);
				find(&tree,key,output_file);
				break;
			case 4:
				print_tree(&tree,output_file);
				break;
			case 5:
				fscanf(input_file,"%d",&key);
				print_subtree(&tree,key,output_file);
				break;
			case 6:
				fscanf(input_file,"%d",&key);
				calculate_imbalance(&tree,key,output_file);
				break;
			default:
				fprintf(output_file,"false\n");
				break;
		}
	}

	// File operations
	fclose(input_file);
	fclose(output_file);
	return 0;
}
