#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
typedef long long ll;

void swap(int *a,int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void random_shuffle (int *array,int n)
{
	srand(time(0));
	for (int i = n-1; i > 0; --i)
	{
		int j = rand()%(i+1);
		swap(&array[i],&array[j]);
	}
}
/*
	IMPORTANT:
	-------------
	All the sorting functions used below are not created by me,
	rather taken as a reference from given Google Classroom post

	It was stated in the assignment that we are allowed to use code online.


*/
// Function to join left and right subarray 
int join(int arr[], int left[], int right[], 
          int l, int m, int r) 
{ 
    int i; // Used in second loop 
    for (i = 0; i <= m - l; i++) 
        arr[i] = left[i]; 
  
    for (int j = 0; j < r - m; j++) 
        arr[i + j] = right[j]; 
} 

int split(int arr[], int left[], int right[], 
          int l, int m, int r) 
{ 
    for (int i = 0; i <= m - l; i++) 
        left[i] = arr[i * 2]; 
  
    for (int i = 0; i < r - m; i++) 
        right[i] = arr[i * 2 + 1]; 
} 
  
// Function to generate Median -Ending subarrays  
int generate_M_arr(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l + (r - l) / 2; 
  
        // create two auxillary arrays 
        int left[m - l + 1]; 
        int right[r - m]; 
  
        // Store alternate array elements in left 
        // and right subarray 
        split(arr, left, right, l, m, r); 
  
        // Recurse first and second halves 
        generate_M_arr(left, l, m); 
        generate_M_arr(right, m + 1, r); 
  
        // join left and right subarray 
        join(arr, left, right, l, m, r); 
    } 
} 

void bubbleSort(int* array, int N){

	for (int i = 0 ; i < N - 1 ; ++i)
		for (int j = 0 ; j < N - i - 1; ++j)
			if (array[j] > array[j + 1]){
				/*swap array[j] and array[j + 1]*/
				int temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
}

void insertionSort(int* array, int N){

	for (int i = 1; i < N; ++i){
		/*take a copy of element to be inserted in sorted array[0...i]*/
		int temp = array[i];

		/*search for the correct position of temp in sorted array[0...i-1]*/
		int j = i - 1;
		while (j >=0 && array[j] > temp){
			/*copy (j)th element to (j+1)th index*/
			array[j+1] = array[j];
			/*decrease the value of j by 1*/
			j--;
		}

		/*paste temp at (j+1)th index*/
		array[j+1] = temp;
	}
}

int partition(int* array, int start, int end){
	/*keep last element at its correct position*/
	int l = start - 1;
	for (int i = start; i < end; ++i)
		if (array[i] < array[end]){
			/*current element is less than pivot element - swap A[l+1] and A[i]*/
			int temp = array[l + 1];
			array[l+1] = array[i];
			array[i] = temp;

			/*increment l*/
			l++;
		}
	/*swap A[l+1] and A[end]*/
	int temp = array[l + 1];
	array[l+1] = array[end];
	array[end] = temp;

	/*return the index of pivot element*/
	return (l + 1);
}

/*sorts the given array of size N using quick sort algorithm*/
void quickSort(int* array, int start, int end){
	/*check if array contains at least two elements to sort*/
	if (start < end){
		/*partition the array and get the pivot index*/
		int pivot_index = partition(array, start, end);
		/*apply quick sort recursively on two parts of the array*/
		quickSort(array, start, pivot_index - 1);
		quickSort(array, pivot_index + 1, end);
	}
}

void merge(int* array, int start, int mid, int end){
	/*take copies of two sorted arrays*/
	int size1 = mid - start + 1;
	int size2 = end - mid;
	int A[size1], B[size2];

	/*copy one part to array A*/
	for (int i = 0 ; i < size1; ++i)
		A[i] = array[start + i];

	/*copy another part to array B*/
	for (int i = 0 ; i < size2; ++i)
		B[i] = array[mid + 1 + i];

	/*indices to iterate through arrays*/
	int a_index = 0, b_index = 0, combinded_index = start;

	/*combine A and B to store in original array*/
	while (a_index  < size1 && b_index < size2){
		if (A[a_index] < B[b_index])
			array[combinded_index++] = A[a_index++];
		else
			array[combinded_index++] = B[b_index++];			
	}

	/*if A is exhausted then copy B entirely*/
	if (a_index == size1)
		while (b_index < size2)
			array[combinded_index++] = B[b_index++];

	/*if B is exhausted then copy A entirely*/
	if (b_index == size2)
		while (a_index < size1)
			array[combinded_index++] = A[a_index++];
}

/*sorts the given array of size N using merge sort algorithm*/
void mergeSort(int* array, int start, int end){
	/*check if array contains at least two elements to be sorted*/
	if (end > start){
		/*divide the array into two equal halves*/
		int mid = (start + end)/2;

		/*sort two halves recursively*/
		mergeSort(array, start, mid);
		mergeSort(array, mid + 1, end);

		/*merge two small sorted array to get a big sorted array*/
		merge(array, start, mid, end);
	}
}

void selectionSort(int* array, int N){

	for (int i = 0 ; i < N - 1; ++i){
		/*get the minimum element in array[i...N]*/
		/*assume the first element in array[i...N] is minimum*/
		int min_element = array[i];
		int min_index = i;
		/*search for minimum in array[i+1...N]*/
		for (int j = i + 1; j < N; ++j)
			if (array[j] < min_element){
				min_element = array[j];
				min_index = j;
			}
		/*swap array[i] with array[min_index]*/
		int temp = array[i];
		array[i] = array[min_index];
		array[min_index] = temp;
	}

}

// IMPORTANT NOTICE ENDS HERE

void print_avg_time(int p,int n)
{
	// Variable Declaration
	ll t1,t2,bub_t,ins_t,qck_t,mer_t,sel_t;
	ll bub_t_sum=0,ins_t_sum=0,qck_t_sum=0,mer_t_sum=0,sel_t_sum=0;
	struct timeval begin,end;

	for(int j=0;j<n;j++)
	{
		int random_array[p];
		for (int i = 0; i < p; ++i)
		{
			random_array[i]=i+1;
		}
		random_shuffle(random_array,p);
		int bub_rand[p];
		int ins_rand[p];
		int qck_rand[p];
		int mer_rand[p];
		int sel_rand[p];

		// Making multiple copies of same array.
		for (int i = 0; i < p; ++i)
		{
			bub_rand[i]=random_array[i];
			ins_rand[i]=random_array[i];
			qck_rand[i]=random_array[i];
			mer_rand[i]=random_array[i];
			sel_rand[i]=random_array[i];
		}

		gettimeofday(&begin,NULL);
		bubbleSort(bub_rand,p);
		gettimeofday(&end,NULL);
		t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
		t2 = (end.tv_sec * 1000000 + end.tv_usec);
		bub_t=t2-t1;
		bub_t_sum+=bub_t;

		gettimeofday(&begin,NULL);
		insertionSort(ins_rand,p);
		gettimeofday(&end,NULL);
		t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
		t2 = (end.tv_sec * 1000000 + end.tv_usec);
		ins_t=t2-t1;
		ins_t_sum+=ins_t;

		gettimeofday(&begin,NULL);
		quickSort(qck_rand,0,p-1);
		gettimeofday(&end,NULL);
		t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
		t2 = (end.tv_sec * 1000000 + end.tv_usec);
		qck_t=t2-t1;
		qck_t_sum+=qck_t;

		gettimeofday(&begin,NULL);
		mergeSort(mer_rand,0,p-1);
		gettimeofday(&end,NULL);
		t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
		t2 = (end.tv_sec * 1000000 + end.tv_usec);
		mer_t=t2-t1;
		mer_t_sum+=mer_t;

		gettimeofday(&begin,NULL);
		selectionSort(sel_rand,p);
		gettimeofday(&end,NULL);
		t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
		t2 = (end.tv_sec * 1000000 + end.tv_usec);
		sel_t=t2-t1;
		sel_t_sum+=sel_t;
	}

	printf("\n %10lld %10lld %10lld %10lld %10lld \n",bub_t_sum/10,ins_t_sum/10,qck_t_sum/10,mer_t_sum/10,sel_t_sum/10);
}

void print_time(int p, int* arr1, int* arr2, int* arr3, int* arr4, int* arr5)
{
	// Variable Declaration
	ll t1,t2,bub_t,ins_t,qck_t,mer_t,sel_t;
	struct timeval begin,end;

	gettimeofday(&begin,NULL);
	bubbleSort(arr1,p);
	gettimeofday(&end,NULL);
	t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
	t2 = (end.tv_sec * 1000000 + end.tv_usec);
	bub_t=t2-t1;

	gettimeofday(&begin,NULL);
	insertionSort(arr2,p);
	gettimeofday(&end,NULL);
	t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
	t2 = (end.tv_sec * 1000000 + end.tv_usec);
	ins_t=t2-t1;

	gettimeofday(&begin,NULL);
	quickSort(arr3,0,p-1);
	gettimeofday(&end,NULL);
	t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
	t2 = (end.tv_sec * 1000000 + end.tv_usec);
	qck_t=t2-t1;

	gettimeofday(&begin,NULL);
	mergeSort(arr4,0,p-1);
	gettimeofday(&end,NULL);
	t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
	t2 = (end.tv_sec * 1000000 + end.tv_usec);
	mer_t=t2-t1;

	gettimeofday(&begin,NULL);
	selectionSort(arr5,p);
	gettimeofday(&end,NULL);
	t1 = (begin.tv_sec * 1000000 + begin.tv_usec);
	t2 = (end.tv_sec * 1000000 + end.tv_usec);
	sel_t=t2-t1;

	printf("\n %10lld %10lld %10lld %10lld %10lld \n",bub_t,ins_t,qck_t,mer_t,sel_t);
}

int main()
{
	int p;
	// Input Size
	scanf("%d",&p);

	int sorted_array[p];
	int reverse_array[p];
	int random_array[p];
	int median_array[p];
	generate_M_arr(median_array,0,p-1);
	for (int i = 0; i < p; ++i)
	{
		sorted_array[i]=i+1;
		reverse_array[p-i-1]=i+1;
		random_array[i]=i+1;
	}
	random_shuffle(random_array,p);
	int bub_best[p];
	int ins_best[p];
	int qck_best[p];
	int mer_best[p];
	int sel_best[p];
	int bub_worst[p];
	int ins_worst[p];
	int qck_worst[p];
	int mer_worst[p];
	int sel_worst[p];

	// Making multiple copies of same array.
	for (int i = 0; i < p; ++i)
	{
		bub_best[i]=sorted_array[i];
		ins_best[i]=sorted_array[i];
		qck_best[i]=random_array[i];
		mer_best[i]=sorted_array[i];
		sel_best[i]=sorted_array[i];

		bub_worst[i]=reverse_array[i];
		ins_worst[i]=reverse_array[i];
		qck_worst[i]=sorted_array[i];
		mer_worst[i]=median_array[i];
		sel_worst[i]=reverse_array[i];

	}

	printf("\n     Bubble  Insertion\t    Quick      Merge  Selection\n");
	printf("Worst\n");
	print_time(p,bub_worst,ins_worst,qck_worst,mer_worst,sel_worst);
	printf("Best\n");
	print_time(p,bub_best,ins_best,qck_best,mer_best,sel_best);
	printf("Average\n");
	print_avg_time(p,10);

	return 0;
}