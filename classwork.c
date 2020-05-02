#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
typedef long long ll;

void random_num_array(int *array, int n)
{
	srand(time(0));
	for(int i=0;i<n;i++)
	{
		array[i]= rand()%100;
		// Done to make the range of random numbers from 0 to 99;
	}
}

/*
	IMPORTANT:
	-------------
	All the sorting functions used below are not created by me,
	rather taken as a reference from given Google Classroom post

	It was stated in the assignment that we are allowed to use code online.


*/

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

int main()
{
	// Variable Declaration
	ll t1,t2,bub_t,ins_t,qck_t,mer_t,sel_t;
	struct timeval begin,end;
	int p;
	// Input Size
	scanf("%d",&p);

	int arr1[p];
	random_num_array(arr1,p);
	int arr2[p];
	int arr3[p];
	int arr4[p];
	int arr5[p];

	// Making multiple copies of same array.
	for (int i = 0; i < p; ++i)
	{
		arr2[i]=arr1[i];
		arr3[i]=arr1[i];
		arr4[i]=arr1[i];
		arr5[i]=arr1[i];
	}

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

	printf("\n %lld %lld %lld %lld %lld \n",bub_t,ins_t,qck_t,mer_t,sel_t);
	return 0;
}