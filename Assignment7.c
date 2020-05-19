#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// A struct for the node of BST
struct node
{
	// The numeral value
	int key;

	// Pointer to the adjacent nodes/relatives
	struct node *parent;
	struct node *left_child;
	struct node *right_child;

	int left_num;
	int right_num;
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

// Supplementary function to count a node and it meta-children below
int count_sub_nodes(struct node *curr_node)
{
	// Simple recursion

	// Base Case
	if(!curr_node)
	{
		return 0;
	}
	curr_node->left_num = count_sub_nodes(curr_node->left_child);
	curr_node->right_num = count_sub_nodes(curr_node->right_child);
	return ((curr_node->left_num)+(curr_node->right_num)+1);
}

// "Helps" insert a node, hence the name Insert-Utility.
// Split Function as described in the assignment pdf
/*
	Method:
	We know that left slicing(diagonally down) and
	right slicing(diagonally up) will show up alternatively.
	There is a previous parent pointer which had lost a child 
	during previous split gets a new child from one of the 
	children of that previously-split child.
	This occurs recursively.

*/
void insertUtil(int key, struct node *prev_parent, struct node *curr_node)
{
	if(!curr_node)
	{
		return;
	}

	// A split to the left
	if(curr_node->key > key)
	{
		if( (!(curr_node->left_child)) || ((curr_node->left_child->key) < key) )
		{
			if((prev_parent->key) == key)
			{
				prev_parent->left_child = curr_node->left_child;
			}
			else
			{
				prev_parent->right_child = curr_node->left_child;
			}
			if(curr_node->left_child)
				curr_node->left_child->parent = prev_parent;
			struct node *temp = curr_node->left_child;
			curr_node->left_child = NULL;
			prev_parent = curr_node;
			insertUtil(key, prev_parent, temp);
		}
		// Iteration to the next child.
		else
		{
			curr_node = curr_node->left_child;
			insertUtil(key, prev_parent,curr_node);
		}
	}
	// A split to the right
	else if(curr_node->key < key)
	{
		if( (!(curr_node->right_child)) || ((curr_node->right_child->key) > key) )
		{
			if((prev_parent->key) == key)
			{
				prev_parent->right_child = curr_node->right_child;
			}
			else
			{
				prev_parent->left_child = curr_node->right_child;
			}
			if(curr_node->right_child)
				curr_node->right_child->parent = prev_parent;
			struct node *temp = curr_node->right_child;
			curr_node->right_child = NULL;
			prev_parent = curr_node;
			insertUtil(key, prev_parent, temp);
		}
		// Iteration to the next child.
		else
		{
			curr_node = curr_node->right_child;
			insertUtil(key, prev_parent,curr_node);
		}
	}
}

// Inserts a node in the tree.
void insert(struct bin_search_tree *tree_ptr, int key)
{	
	// False if the node already exists.
	if (search(tree_ptr,key))
	{
		printf("false\n");
		return; 
	}

	// Condition of no nodes
	if(tree_ptr->num_nodes < 1)
	{
		struct node *newnode = (struct node *) malloc(sizeof(struct node));
		if (newnode == NULL)
		{
			printf("Memory allocation fail.\n");
			printf("false\n");
		   	return; 
		}

		// Update the struct node values
		tree_ptr->root = newnode;
		newnode->key = key;
		newnode->left_child = NULL;
		newnode->right_child =NULL;
		newnode->parent = NULL;
		count_sub_nodes(newnode);

		// Update the tree values
		tree_ptr->num_nodes += 1;
		printf("true\n");
		return;
	}

	// If nodes are present.
	struct node *curr_node = tree_ptr->root;
	bool added = false;
	while(!added)
	{
		// Probability calculation
		int root_prob = (curr_node->left_num)+(curr_node->right_num)+1;
		int ran_num = rand()%root_prob;


		// Insert at root Method.
		if(ran_num == 0)
		{
			struct node *newnode = (struct node *) malloc(sizeof(struct node));
			// Malloc Fail check.
			if (newnode == NULL)
			{
				printf("Memory allocation fail.\n");
				printf("false\n");
			   	return; 
			}

			/*
			Updating Links
			*/ 

			// Link to and from the parent
			if(curr_node->parent)
			{
				newnode->parent = curr_node->parent;
				if ((curr_node->parent->key) > key)
				{
					curr_node->parent->left_child = newnode;
				}
				else if ((curr_node->parent->key) < key)
				{
					curr_node->parent->right_child = newnode;
				}
			}
			// Link to/from root.
			else
			{
				newnode->parent = NULL;
				tree_ptr->root = newnode;
			}

			// Updating the key.
			newnode->key = key;
			
			// Link to the child
			if((curr_node->key) > key)
			{
				newnode->right_child = curr_node;
			}
			else if((curr_node->key) < key)
			{
				newnode->left_child = curr_node;
			}

			// Link from the child
			curr_node->parent = newnode;

			// Split function
			insertUtil(key,newnode,curr_node);

			// Updation of node values.
			count_sub_nodes(tree_ptr->root);

			// Updating tree values.
			tree_ptr->num_nodes += 1;
			added = true;
			printf("true\n");
			return;
		}

		// The regular insert method. 
		else
		{
			// Iteration to left and right child.

			// To Left.
			if((curr_node->key) > key)
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
					// Malloc Fail Check.
					if (newnode == NULL)
					{
						printf("Memory allocation fail.\n");
						printf("false\n");
				    	return; 
					}

					// Update the struct node values
					curr_node->left_child = newnode;
					newnode->key = key;
					newnode->left_child = NULL;
					newnode->right_child =NULL;
					newnode->parent = curr_node;
					count_sub_nodes(tree_ptr->root);

					added = true;
						
					// Update the tree values
					tree_ptr->num_nodes += 1;
					break;
				}
			}
			// To Right.
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
					// Malloc Fail Check.
					if (newnode == NULL)
					{
						printf("Memory allocation fail.\n");
						printf("false\n");
				    	return; 
					}

					// Update the struct node values
					curr_node->right_child = newnode;
					newnode->key = key;
					newnode->left_child = NULL;
					newnode->right_child =NULL;
					newnode->parent = curr_node;
					count_sub_nodes(tree_ptr->root);
				
					added = true;

					// Update the tree values
					tree_ptr->num_nodes += 1;
					break;
				}
			}	
		}
			
	}

	printf("true\n");
	return;
}

// "Helps" delete a node, hence the name Delete-Utility.
//  Works as the join function as described in the assignment
struct node* deleteUtil(struct node *left_node,struct node *right_node)
{
	// Calculation of Probability.
	int left_prob,right_prob,total;
	if(!left_node)
	{
		left_prob = 0;
	}
	else
	{
		left_prob = (left_node->left_num) + (left_node->right_num) + 1;
	}
	if(!right_node)
	{
		right_prob = 0;
	}
	else
	{
		right_prob = (right_node->left_num) + (right_node->right_num) + 1;
	}
	total = left_prob + right_prob;
	if (total == 0)
	{
		return NULL;
	}

	// Random Number For Choosing the path of joining
	int ran_num = rand()%total;
	if(ran_num < left_prob)
	{
		left_node->right_child = deleteUtil(left_node->right_child,right_node);
		if(left_node->right_child)
			left_node->right_child->parent = left_node;
		return left_node;
	}
	else
	{
		right_node->left_child = deleteUtil(left_node,right_node->left_child);
		if(right_node->left_child)
			right_node->left_child->parent = right_node;
		return right_node;
	}
}

// The base delete-a-node function.
// [Uses deleteUtil() and search()]
void delete(struct bin_search_tree *tree_ptr, int key)
{
	// Finding the address of node to be deleted
	struct node *curr_node = search(tree_ptr, key);

	// If found.
	if(curr_node)
	{
		struct node *replace_node = deleteUtil(curr_node->left_child,curr_node->right_child);
		if(curr_node->parent)
		{
			// Placing the replace node on curr_node's place.
			if(replace_node)
			replace_node->parent = curr_node->parent;
			
			if(curr_node->parent->key > key)
			{
				curr_node->parent->left_child = replace_node; 
			}
			else if(curr_node->parent->key < key)
			{
				curr_node->parent->right_child = replace_node; 
			}
		}
		else
		{
			// Placing the replace node on curr_node's place.
			if(replace_node)
				replace_node->parent = NULL;
			tree_ptr->root = replace_node;
		}

		// Deleting the memory of curr_node/ required node.
		free(curr_node);
		tree_ptr->num_nodes -=1;
		count_sub_nodes(tree_ptr->root);
		printf("true\n");
		return;
		
	}
	printf("false\n");
	return;
}

// Supplementary Print function 1
void print_inorder(struct bin_search_tree *tree_ptr,struct node *curr_node)
{
	if(!curr_node)
	{
		return;
	}

	print_inorder(tree_ptr,curr_node->left_child);

	printf("%d ",curr_node->key);

	print_inorder(tree_ptr,curr_node->right_child);
}

// Supplementary Print function 2
void print_preorder(struct bin_search_tree *tree_ptr,struct node *curr_node)
{
	if(!curr_node)
	{
		return;
	}

	printf("%d ",curr_node->key);

	print_preorder(tree_ptr,curr_node->left_child);

	print_preorder(tree_ptr,curr_node->right_child);
}

// Supplementary Print function 3
void print_postorder(struct bin_search_tree *tree_ptr,struct node *curr_node)
{
	if(!curr_node)
	{
		return;
	}

	print_postorder(tree_ptr,curr_node->left_child);

	print_postorder(tree_ptr,curr_node->right_child);

	printf("%d ",curr_node->key);
}

// Print function for the user query
// [Uses supplementary print functions]
void print_subtree(struct bin_search_tree *tree_ptr,int key)
{
	// Finding the root of sub-tree
	struct node *curr_node = search(tree_ptr,key);
	if(!curr_node)
	{
		printf("false\n");
		return;
	}

	// Print Format.
	printf("Inorder: ");
	print_inorder(tree_ptr,curr_node);
	printf("\nPreorder: ");
	print_preorder(tree_ptr,curr_node);
	printf("\nPostorder: ");
	print_postorder(tree_ptr,curr_node);
	printf("\n");
}

// Print function for the user query
// [Uses supplementary print functions]
void print_tree(struct bin_search_tree *tree_ptr)
{
	struct node *curr_node = tree_ptr->root;
	if(!curr_node)
	{
		printf("false\n");
		return;
	}

	// Print Format.
	printf("Inorder: ");
	print_inorder(tree_ptr,curr_node);
	printf("\nPreorder: ");
	print_preorder(tree_ptr,curr_node);
	printf("\nPostorder: ");
	print_postorder(tree_ptr,curr_node);
	printf("\n");
}

// Find a node and its prints its depth.
void find(struct bin_search_tree *tree_ptr, int key)
{
	int depth = 0;

	// No node check
	if(tree_ptr->num_nodes<1)
	{
		printf("false\n");
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
				printf("false\n");
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
				printf("false\n");
				return;
			}
		}
	}

	printf("true, depth=%d\n", depth);
	return;
}

// Function to calculate the imbalance of sub-tree
// [Uses the count function]
void calculate_imbalance(struct bin_search_tree *tree_ptr,int key)
{
	// Search for the address of node.
	struct node *curr_node = search(tree_ptr, key);

	// If not found.
	if(!curr_node)
	{
		printf("false\n");
		return;
	}

	// Simple arithmetical logic.
	int left = count_sub_nodes(curr_node->left_child);
	int right = count_sub_nodes(curr_node->right_child);
	printf("%d\n", (left-right) );
}

// The main function.
int main()
{
	srand(time(NULL));
	// Initialisation of struct values.
	tree.num_nodes=0;
	tree.root = NULL;

	// Number of queries
	int t;
	scanf("%d",&t);
	while(t--)
	{
		// Query type
		int q;
		int key;
		scanf("%d",&q);
		
		// Conditional queries.
		switch(q)
		{
			case 1:
				scanf("%d",&key);
				insert(&tree,key);
				break;
			case 2:
				scanf("%d",&key);
				delete(&tree,key);
				break;
			case 3:
				scanf("%d",&key);
				find(&tree,key);
				break;
			case 4:
				print_tree(&tree);
				break;
			case 5:
				scanf("%d",&key);
				print_subtree(&tree,key);
				break;
			case 6:
				scanf("%d",&key);
				calculate_imbalance(&tree,key);
				break;
			default:
				printf("false\n");
				break;
		}
	}

	return 0;
}
