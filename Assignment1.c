#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Structures use for Linked lists.
// nucleotide is a node
struct nucleotide
{
	char n_base;
	struct nucleotide *next_nucleotide;
};
struct chain
{
	// Head Pointer of nodes
	struct nucleotide *head;
	// Points the current position / node we are dealing with.
	struct nucleotide *current;
};

// DNA molecule is a node
struct DNA_molecule
{
	int molecule_id;
	 // alpha and beta chains of DNA 
	struct chain *alpha;
	struct chain *beta;
	int length;
	struct DNA_molecule *next_molecule;
};

struct molecule_list
{
	int number_of_molecules;
	// Head Pointer of nodes
	struct DNA_molecule *list_head;
	// Points the current position / node we are dealing with.
	struct DNA_molecule *list_current;
}mlist;
// mlist is a global list of all modeinitialised at the start of main function.


// compliment function- Gives a compliment n_base for another. 
char compliment(char n_base)
{
	switch(n_base)
	{
		case 'A':
			return 'T';
		case 'T':
			return 'A';
		case 'G':
			return 'C';
		case 'C':
			return 'G';
		default :
			return '\0';	
	}
}


// valid_n_base function- Checks for a valid n_base
bool valid_n_base(char n_base)
{
	bool valid = true;
	
	if(!(n_base =='A'|| n_base =='T'||n_base =='G'||n_base =='C'))
	{
		valid = false;
	}
	return valid;
}


// Inserts nucleotide at given position in a molecule
void insert_nucleotide(int mol_id, int ch_id, char nctd, int position)
{
	// Checks for validity of n_base
	if (!valid_n_base(nctd))
	{
		printf("Invalid Choice of Nucleotides\n");
		return;	
	}
	// checks for vallidity of molecule id
	if (mol_id<1||mol_id>mlist.number_of_molecules)
	{
		printf("Invalid Molecule Id\n");
		return;
	}

	// Re-initialise the current to head.
	mlist.list_current=mlist.list_head;

	// Search the correct molecule
	while(mlist.list_current->molecule_id!=mol_id)
	{
		mlist.list_current= mlist.list_current->next_molecule;
	}

	 //check for validity of position 
	if (position<1||position>((mlist.list_current->length)+1))
	{
		printf("Invalid Position %d\n",position);
		return;
	}

	// Allocatng memory for the chains 
	if (mlist.list_current->length==0)
	{
		mlist.list_current->alpha= (struct chain *) malloc(sizeof(struct chain));
		mlist.list_current->beta= (struct chain *) malloc(sizeof(struct chain));
	}

	// Allocating memory for nucleotides
	struct nucleotide *temp_alpha = (struct nucleotide *) malloc(sizeof(struct nucleotide));
	struct nucleotide *temp_beta = (struct nucleotide *) malloc(sizeof(struct nucleotide));
	temp_alpha->next_nucleotide=NULL;
	temp_beta->next_nucleotide=NULL;

	if (mlist.list_current->length==0)
	{
		// head pointing to new nodes
		mlist.list_current->alpha->head= temp_alpha;
		mlist.list_current->beta->head= temp_beta;
		mlist.list_current->length+=1;
	}
	else
	{
		// Re-initialize current at head
		mlist.list_current->alpha->current=mlist.list_current->alpha->head;
		mlist.list_current->beta->current=mlist.list_current->beta->head;

		// search for (position-1)th node
		for (int i = 0; i < position-2; ++i)
		{
			mlist.list_current->alpha->current=mlist.list_current->alpha->current->next_nucleotide;
			mlist.list_current->beta->current=mlist.list_current->beta->current->next_nucleotide;
		}
		// saving the location of these nodes
		struct nucleotide *previous_alpha=mlist.list_current->alpha->current;
		struct nucleotide *previous_beta=mlist.list_current->beta->current;

		// Increment in position of current
		mlist.list_current->alpha->current = mlist.list_current->alpha->current->next_nucleotide;
		mlist.list_current->beta->current = mlist.list_current->beta->current->next_nucleotide;

		// new node pointing to original (position)th node
		temp_alpha->next_nucleotide = mlist.list_current->alpha->current;
		temp_beta->next_nucleotide = mlist.list_current->beta->current;

		//Prev node point to new node 
		previous_alpha->next_nucleotide = temp_alpha;
		previous_beta->next_nucleotide = temp_beta;
		mlist.list_current->length+=1;

		//Node inserted 

	}

	// choice for addition of nucleotide(nctd) at alpha/beta
	if (ch_id==1)
	{
		temp_alpha->n_base = nctd;
		temp_beta->n_base = compliment(nctd);
	}

	else if (ch_id==2)
	{
		temp_beta->n_base = nctd;
		temp_alpha->n_base = compliment(nctd);
	}
}

// Deletes nctd from position
void delete_nucleotide(int mol_id, int position)
{
	// Check for validity of Molecule ID
	if (mol_id<1||mol_id>mlist.number_of_molecules)
	{
		printf("Invalid Molecule Id\n");
		return;
	}

	// Re-initialize current at head
	mlist.list_current=mlist.list_head;

	// pointer for prev node
	struct DNA_molecule *previous_mol=NULL;

	// searching for required molecule
	while(mlist.list_current->molecule_id!=mol_id)
	{
		previous_mol=mlist.list_current;
		mlist.list_current= mlist.list_current->next_molecule;
	}

	// Checks validity of position
	if (position<1||position>(mlist.list_current->length))
	{
		printf("Invalid Position %d\n",position);
		return;
	}

	// Re-initialize current at head for chains
	mlist.list_current->alpha->current=mlist.list_current->alpha->head;
	mlist.list_current->beta->current=mlist.list_current->beta->head;

	// Deletion of node.
	if (position==1)
	{
		mlist.list_current->alpha->head=mlist.list_current->alpha->current->next_nucleotide;
		mlist.list_current->beta->head=mlist.list_current->beta->current->next_nucleotide;
		free(mlist.list_current->alpha->current);
		free(mlist.list_current->beta->current);
	}

	// Deletion of node
	else
	{
		// searching for req. node
		for (int i = 0; i < position-2; ++i)
		{
			mlist.list_current->alpha->current=mlist.list_current->alpha->current->next_nucleotide;
			mlist.list_current->beta->current=mlist.list_current->beta->current->next_nucleotide;
		}

		// previous nodes
		struct nucleotide *previous_alpha = mlist.list_current->alpha->current;
		struct nucleotide *previous_beta = mlist.list_current->beta->current;

		// (position)th nodes
		mlist.list_current->alpha->current = mlist.list_current->alpha->current->next_nucleotide;
		mlist.list_current->beta->current = mlist.list_current->beta->current->next_nucleotide;

		// previous point to (position+1)
		previous_alpha->next_nucleotide = mlist.list_current->alpha->current->next_nucleotide;
		previous_beta->next_nucleotide = mlist.list_current->beta->current->next_nucleotide;

		// (Position) is freed.
		free(mlist.list_current->alpha->current);
		free(mlist.list_current->beta->current);
	}

	mlist.list_current->length-=1;

	// If no nodes are left
	if (mlist.list_current->length==0)
	{

		// deletion of molecule
		if (mlist.list_current->molecule_id==1)
		{
			mlist.list_head = mlist.list_current->next_molecule;
			free(mlist.list_current);
			mlist.list_current = mlist.list_head;
		}
		else
		{
			previous_mol->next_molecule=mlist.list_current->next_molecule;
			free(mlist.list_current);
			mlist.list_current = previous_mol->next_molecule;
		}

		// Updating the values of molecule id 
		while(mlist.list_current)
		{
			mlist.list_current->molecule_id-=1;
			mlist.list_current=mlist.list_current->next_molecule;
		}
		
		// Updating the number of molecules
		mlist.number_of_molecules-=1;
	}

}

// Function to add molecules
void add_molecule(char* string)
{
	// Check for validity of nucleotides
	int l = strlen(string);
	for (int i = 0; i < l; ++i)
	{
		string[i]=toupper(string[i]);
		printf("%c",string[i]);
		if(!valid_n_base(string[i]))
		{
			printf("Invalid Choice of Nucleotides\n");
			return;
		}
	}
	
	// allocating memory to molecule
	if(mlist.number_of_molecules==0)
	{
		mlist.list_head = (struct DNA_molecule *) malloc(sizeof(struct DNA_molecule));
		mlist.list_current=mlist.list_head;
	}
	else
	{
		struct DNA_molecule *temp = (struct DNA_molecule *) malloc(sizeof(struct DNA_molecule));
		while(mlist.list_current->next_molecule)
		{
			mlist.list_current = mlist.list_current->next_molecule;
		}
		mlist.list_current->next_molecule = temp;
		mlist.list_current = temp;
	}

	// Updating other values
	mlist.number_of_molecules+=1;
	mlist.list_current->molecule_id = mlist.number_of_molecules;
	mlist.list_current->length=0;
	mlist.list_current->next_molecule= NULL;

	// calling insert function to fill in the values
	for (int i = 0; i < l; ++i)
	{
		insert_nucleotide(mlist.list_current->molecule_id,1,string[i],i+1);
	}	
}


void split_molecule(int mol_id)
{
	// Checking validity of molecule id
	if (mol_id<1||mol_id>mlist.number_of_molecules)
	{
		printf("Invalid Molecule Id\n");
		return;
	}

	// searching the required molecule
	struct DNA_molecule *initial = mlist.list_head;
	while(initial->molecule_id!=mol_id)
	{
		initial=initial->next_molecule;
	}

	// pointer at end of list
	mlist.list_current=mlist.list_head;
	while(mlist.list_current->next_molecule)
	{
		mlist.list_current=mlist.list_current->next_molecule;
	}

	// forming a new node.
	struct DNA_molecule *new_mol = (struct DNA_molecule *) malloc(sizeof(struct DNA_molecule));
	mlist.list_current->next_molecule = new_mol;
	mlist.list_current = new_mol;

	// updating concerned values
	mlist.number_of_molecules += 1;
	mlist.list_current->molecule_id = mlist.number_of_molecules;
	mlist.list_current->length = initial->length;
	mlist.list_current->next_molecule= NULL;

	// IMPORTANT: new node takes up the beta chain of initial
	mlist.list_current->beta=initial->beta;
	initial->beta= NULL;

	// New beta chain for initial molecule and alpha chain for the new molecule
	mlist.list_current->alpha= (struct chain *) malloc(sizeof(struct chain));
	initial->beta= (struct chain *) malloc(sizeof(struct chain));
	
	// Nodes in chain (Nucleotides) added and concerned values updated
	struct nucleotide *temp_alpha_head = (struct nucleotide *) malloc(sizeof(struct nucleotide));
	struct nucleotide *temp_beta_head = (struct nucleotide *) malloc(sizeof(struct nucleotide));
	temp_alpha_head->next_nucleotide=NULL;
	temp_beta_head->next_nucleotide=NULL;

	// Head pointers of respective chains point to the newly formed nodes
	mlist.list_current->alpha->head= temp_alpha_head;
	initial->beta->head= temp_beta_head;

	// n_base updated using compliment function for n_bases
	temp_alpha_head->n_base = compliment(mlist.list_current->beta->head->n_base);
	temp_beta_head->n_base = compliment(initial->alpha->head->n_base);

	// The four alpha/beta lists' current pointers are initialised to head 
	mlist.list_current->alpha->current=mlist.list_current->alpha->head;
	mlist.list_current->beta->current=mlist.list_current->beta->head;
	initial->alpha->current=initial->alpha->head;
	initial->beta->current=initial->beta->head;

	// iteration for each nucleotide after the first (similar as above)
	for (int i = 0; i < initial->length-1; ++i)
	{
		// Nodes in chain (Nucleotides) added and concerned values updated
		struct nucleotide *temp_alpha = (struct nucleotide *) malloc(sizeof(struct nucleotide));
		struct nucleotide *temp_beta = (struct nucleotide *) malloc(sizeof(struct nucleotide));
		temp_alpha->next_nucleotide=NULL;
		temp_beta->next_nucleotide=NULL;
		mlist.list_current->alpha->current->next_nucleotide = temp_alpha;
		initial->beta->current->next_nucleotide = temp_beta;
		
		// pointer iteration
		mlist.list_current->alpha->current= mlist.list_current->alpha->current->next_nucleotide;
		mlist.list_current->beta->current= mlist.list_current->beta->current->next_nucleotide;
		initial->alpha->current = initial->alpha->current->next_nucleotide;
		initial->beta->current = initial->beta->current->next_nucleotide;

		// n_bases updated
		temp_alpha->n_base = compliment(mlist.list_current->beta->current->n_base);
		temp_beta->n_base = compliment(initial->alpha->current->n_base);
	}


}

// Function to print all molecular chains
void print_all_chains()
{
	// list pointer re-initialised to head
	mlist.list_current=mlist.list_head;

	// pointer iterated till NULL
	while(mlist.list_current)
	{
		// for each molecule:
	
		// chain pointers re-initialised to head
		mlist.list_current->alpha->current=mlist.list_current->alpha->head;
		mlist.list_current->beta->current=mlist.list_current->beta->head;

		// Printing
		printf("Molecule ID %d\n",mlist.list_current->molecule_id);
		printf("\nα-β\n---\n");

		// pointer iterated till NULL
		while(mlist.list_current->alpha->current)
		{
			// Printing n_bases of nucleotides
			printf("%c-%c\n",mlist.list_current->alpha->current->n_base,mlist.list_current->beta->current->n_base);
			
			// Interation update expression
			mlist.list_current->alpha->current = mlist.list_current->alpha->current->next_nucleotide;
			mlist.list_current->beta->current = mlist.list_current->beta->current->next_nucleotide;
		}

		printf("\n");
		
		// Interation update expression
		mlist.list_current=mlist.list_current->next_molecule;
	}
}

void print_chain(int mol_id)
{
	// Checking validity of Molecule ID
	if (mol_id<1||mol_id>mlist.number_of_molecules)
	{
		printf("Invalid Molecule Id\n");
		return;
	}
	
	// list pointer re-initialised to head
	mlist.list_current=mlist.list_head;

	// searching for required Molecule
	while(mlist.list_current->molecule_id!=mol_id)
	{
		mlist.list_current= mlist.list_current->next_molecule;
	}

	// nucleotide pointer re-initialised to head
	mlist.list_current->alpha->current=mlist.list_current->alpha->head;
	mlist.list_current->beta->current=mlist.list_current->beta->head;

	printf("\nα-β\n---\n");
	while(mlist.list_current->alpha->current)
	{
		// printing
		printf("%c-%c\n",mlist.list_current->alpha->current->n_base,mlist.list_current->beta->current->n_base);
		
		// iteration update expression
		mlist.list_current->alpha->current = mlist.list_current->alpha->current->next_nucleotide;
		mlist.list_current->beta->current = mlist.list_current->beta->current->next_nucleotide;
	}
	printf("\n");
}
int main(int argc, char const *argv[])
{
	// Initialising the list values
	mlist.number_of_molecules=0;

	// Printing
	printf("Welcome to DNA-DBMS\n");
	printf("-------------------\n");
	
	// While loop for queries in the DATABASE
	int choice=-1;
	while(choice!=9)
	{
		printf("\n\nPlease select your choice.\n");
		printf("1.Add a Molecule\n");
		printf("2.Insert a Nucleotide\n");
		printf("3.Delete a Nucleotide\n");
		printf("4.Split a Molecule\n");
		printf("5.Print a Molecule Chain\n");
		printf("6.Print all Molecule Chains\n");
		printf("9.Quit\n\n");
		scanf("%d",&choice);

		// Variable declaraion (used as parameters in above-typed function)
		char *nctds;
		char nctd;
		int mol_id;
		int ch_id;
		int position;

		// Switch-case for the choice of functions 
		switch(choice)
		{
			case 1: printf("Enter the Sequence of Nucleotides in Alpha Chain.\n");
					scanf("%s",nctds);
					add_molecule(nctds);
					break;

			case 2: printf("Enter the Molecule ID.\n");
					scanf("%d",&mol_id);
					printf("Enter Chain ID (Alpha-1, Beta-2)\n");
					scanf("%d",&ch_id);
					if(!(ch_id==1||ch_id==2))
					{
						printf("Invalid Chain ID\n");
						break;
					}
					else
					{
						printf("Enter the Position of Nucleotide\n");
						scanf("%d",&position);
						fflush(stdin);
						printf("Enter the Nucleotide\n");
						scanf(" %c",&nctd);
						nctd=toupper(nctd);
						insert_nucleotide(mol_id,ch_id,nctd,position);
						break;
					}

			case 3: printf("Enter the Molecule ID.\n");
					scanf("%d",&mol_id);
					printf("Enter the Position of Nucleotide\n");
					scanf("%d",&position);
					delete_nucleotide(mol_id,position);
					break;
			
			case 4: printf("Enter the Molecule ID.\n");
					scanf("%d",&mol_id);
					split_molecule(mol_id);
					break;
			
			case 5: printf("Enter the Molecule ID.\n");
					scanf("%d",&mol_id);
					print_chain(mol_id);
					break;
			
			case 6: print_all_chains();
					break;
			
			case 9: break;
			
			default: printf("Invalid Choice\n");
		}	
	}
	return 0;
}