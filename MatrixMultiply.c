/* MatrixMultiply.c : 
*
*  Author:	Kate Wang(kate.wang.p@gmail.com)
*  Date:	2018-02-18		
*  Version: 1.0
*
*
*	It can be copied into Visual Studio 2017 for C compile mode and run as console application
*	
*	For simplification, there is no human I/O designed, instead all test cases are hard 
*	coded in test functions, though some values are generated automatically.
*
*	there will be some printout of each test cases to show the result of each question
*
*   Assumptions:
*	1. Overflow during multiplication or add will not happen
*	2. usable memeory is abundent so malloc() always return successfull, then NO checking 
*	   to what malloc() returns, neither NULL checking before free()
*/

#include "stdafx.h"// uncomment this line if you want to run in Visual Studio 2017

#include <stdio.h>
#include <stdlib.h>

void printArray(int *arr_ptr, int arr_sz);
void print2Darray(int **array, int r, int c);


void quick_sort(int *arr_ptr, int arr_sz);
void insertion_sort(int *arr_ptr, int arr_sz);
void sort(int *arr_ptr, int arr_sz);


void matrix_brutal_force_multiply(int** res,
	int** X, int** Y,
	int x_rows, int x_cols,
	int y_rows, int y_cols);

void matrix_condense_multiply(int** resD,
	int** X, int** Y,
	int x_rows, int x_cols,
	int y_rows, int y_cols);

int ** matrix_multiply(int** X, int** Y,
	int x_rows, int x_cols,
	int y_rows, int y_cols);

void test_array_sort();
void test_matrix_multiplication();

int main()
{
	// Q1:
	// please see answer sheet

	// Q2:
	test_array_sort();

	// Q3:
	test_matrix_multiplication();

	return 0;

}

/************************************************
* quick_sort function
*************************************************/
void quick_sort(int *arr_ptr, int arr_sz) {
	
	//sorted sub array
	if (arr_sz <= 1)
		return;

	int i, j, pivot, tmp;

	//use leftmost of each sub array as pivot
	pivot = 0;
	i = 0;
	j = arr_sz - 1;

	while (i < j) {

		// search the first bigger than pivot from the right of the array
		while (*(arr_ptr + i) <= *(arr_ptr + pivot) && i < arr_sz - 1) {
			i++;
		}

		// search the first less than pivot from the left of the array
		while (*(arr_ptr + j) > *(arr_ptr + pivot)) {
			j--;
		}

		// exchange i, j and continue search the array until i and j meet
		if (i < j) {
			tmp = *(arr_ptr + i);
			*(arr_ptr + i) = *(arr_ptr + j);
			*(arr_ptr + j) = tmp;
		}

	}

	// array had been traversed, 
	// put pivot into the right place 
	// then partition the array into two subarray and do quick sort to each
	tmp = *(arr_ptr + pivot);
	*(arr_ptr + pivot) = *(arr_ptr + j);
	*(arr_ptr + j) = tmp;

	// sort the left sub array
	quick_sort(arr_ptr, j);

	// sort the right sub array
	quick_sort(arr_ptr + j + 1, arr_sz - j - 1);
}
	

/************************************************
* insertion sort function
*/
void insertion_sort(int *arr_ptr, int arr_sz) {
	int current, i, j;
	// compare each number with all numbers before it
	// start from the second one
	for ( i = 1; i<arr_sz; i++)
	{
		current = arr_ptr[i];

		j = i - 1;
		while (current < arr_ptr[j] && j >= 0)
		{
			//shift larger number one position to the right
			arr_ptr [j + 1] = arr_ptr[j];
			--j;
		}
		// put current ahead of all numbers larger
		arr_ptr[j + 1] = current;
	}
}
	


/************************************************
* entry function for array sort
* ***********************************************
* sort an array by using
* 1. quicksort when partition > 10
* or
* 2. insertion sort when partition <=10
*/
void sort(int *arr_ptr, int arr_sz) {

	//sorted sub array
	if (arr_sz<=1)
		return; 

	// quick sort if partition > 10
	if (arr_sz > 10) {
		printf("size: %d >10, \t quick sort ... \n\n", arr_sz);
		quick_sort(arr_ptr, arr_sz);
	}
	else// insertion sort if partition <= 10
	{
		printf("size: %d <=10 \t insertion sort ... \n\n", arr_sz);
		insertion_sort(arr_ptr, arr_sz);
	}
}


/************************************************
* brutal force matrix multiplication function
* **********************************************/
void matrix_brutal_force_multiply(int** res,
	int** X, int** Y,
	int x_rows, int x_cols,
	int y_rows, int y_cols) {

	int i, j, k;
	// start to simply muliply row and column, O(x_rows * x_rows * x_cols)
	for (i = 0; i < x_rows; i++) {
		for (j = 0; j < x_rows; j++) {
			res[i][j] = 0;
			for (k = 0; k < x_cols; k++) {
				res[i][j] += X[i][k] * Y[k][j];
			}
		}
	}

}
/************************************************
* sparse matrix multiplication function
* **********************************************/
void matrix_condense_multiply(int** resD,
	int** X, int** Y,
	int x_rows, int x_cols,
	int y_rows, int y_cols) {

	//condese X and Y to A and B if they are sparse, O(x_rows*x_cols)
	int i, j;
	int sizeA = 0, sizeB = 0;
	for (i = 0; i < x_rows; i++) {
		for (j = 0; j < x_cols; j++) {
			if (X[i][j]) sizeA++;
			if (Y[j][i]) sizeB++;
		}
	}

	// prepare memeory for int[3][] matrix A and B to store all non-zero values
	int ** A = (int**)malloc(3 * sizeof(int));
	for (i = 0; i<3; i++)
	{
		A[i] = (int *)malloc(sizeA * sizeof(int));
	}

	int ** B = (int**)malloc(3 * sizeof(int));
	for (i = 0; i<3; i++)
	{
		B[i] = (int *)malloc(sizeB * sizeof(int));
	}


	// traverse matrix to fetch all non-zero values
	// O(x_rows * x_cols)
	int a = 0, b = 0;
	for (i = 0; i < x_rows; i++) {
		for (j = 0; j < x_cols; j++) {
			if (X[i][j]) {
				A[0][a] = i;
				A[1][a] = j;
				A[2][a] = X[i][j];
				a++;
			}

			if (Y[j][i]) {

				B[0][b] = i;
				B[1][b] = j;
				B[2][b] = Y[j][i];
				b++;
			}
		}
	}

	printf("\n Condensed matrix X->A:");
	print2Darray(A, 3, sizeA);
	printf("\n Condensed matrix Y->B:");
	print2Darray(B, 3, sizeB);


	// start from the smaller condensed matrix
	// for each value in A, multiple it with corresponding column
	// O( number of non-zero-in-X * number of non-zero-in-Y)
	int y, z;
	for (i = 0; i < sizeA; i++) {
		for (j = 0; j < sizeB; j++) {

			if (A[1][i] == B[1][j]) {
				y = A[0][i];
				z = B[0][j];
				resD[y][z] = resD[y][z] + (A[2][i] * B[2][j]);
			}
		}
	}

	// clean up
	for (i = 0; i<3; i++)
	{
		free(A[i]);
		free(B[i]);
	}
	free(A);
	free(B);

}

/************************************************
* entry function for martix multiplication
**************************************************
* NOTE1:
*	 if (non-zero-in-X * non-zero-in-Y )< (x_rows -2) * x_rows*y_cols
*	     it is worthy to use condensed matrix
*	 else
*	     simply use brutal force to do matrix multiplicationd
*
* NOTE2: 
*	 to simplify, overflow during multiplication or add is NOT considered
*	 
* NOTE3:
*	 it is possible to calculate whether it is worthy to use condensed 
*    matrix according to NOTE1, but it is not implemented in this version.
*	 instead, one static global variable "SPARSE" is used as switcher.
*/
static int SPARSE = 0; // if SPARSE=1, go for sparse matrix multiplication 
int ** matrix_multiply(int** X, int** Y, 
						int x_rows, int x_cols, 
						int y_rows, int y_cols) {

	//pointer address check
	if (!X || !Y) {
		printf("Failed, X or Y is NULL!\n");
		return NULL;

	}

	// no rows or columns checking
	if (!x_rows || !x_cols || !y_rows || !y_cols) {
		printf("Failed, X(%d*%d) and Y(%d*%d) contains zero rows or colums\n",
			x_rows, x_cols, y_rows, y_cols);
		return NULL;
	}

	//size matching check
	if (x_rows != y_cols || x_cols != y_rows){
		printf("Failed, X(%d*%d) and Y(%d*%d) dosen't match in size of (m*n and n*m) to have multiplation\n", 
			x_rows, x_cols, y_rows, y_cols);
		return NULL;
	}


	// multipy started
	int m = x_rows;
	int i = 0;
	// prepare memory space for output
	int **res = (int**) malloc(m * sizeof(int));
	for (i = 0; i < m; i++) {
		res[i] = (int *)malloc(m * sizeof(int));
		// set matrix to 0 for sparse matrix
		if (SPARSE)
			memset(res[i], 0, m * sizeof(int));
	}


	if (SPARSE) {
		//Big O(2*x_rows*x_cols + non-zero-in-X * non-zero-in-Y)
		matrix_condense_multiply(res, X, Y, x_rows, x_cols, y_rows, y_cols);
	}
	else { 
		// Big O(x_rows * x_rows * y_cols)
		matrix_brutal_force_multiply(res, X, Y, x_rows, x_cols, y_rows, y_cols);
	}

	return res;
}

/************************************************
* test function for martix multiplication
* ***********************************************/
void test_matrix_multiplication() {

	// test maxtrix multiplication with 3*2 matrix and 2*3 matrix
	int x_rows = 3, x_cols = 2;
	int y_rows = 2, y_cols = 3;
	int i;

	// allocate mem for X, fill with random value
	int** X = (int**)malloc(x_rows * sizeof(int));
	for (i = 0; i < x_rows; i++)
	{
		X[i] = (int *)malloc(x_cols * sizeof(int));
	}

	// allocate mem for Y, fill with random value
	int** Y = (int**)malloc(y_rows * sizeof(int));
	for (i = 0; i < y_rows; i++)
	{
		Y[i] = (int *)malloc(y_cols * sizeof(int));
	}



	printf("\n\nChecking error cases begin ... \n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");

	matrix_multiply(X, NULL, 2, 2, 2, 2);
	matrix_multiply(NULL, Y, 2, 2, 2, 2);

	matrix_multiply(X, Y, 2, 2, 3, 2);
	matrix_multiply(X, Y, 2, 2, 2, 3);

	matrix_multiply(X, Y, 0, 2, 2, 2);
	matrix_multiply(X, Y, 2, 0, 2, 2);
	matrix_multiply(X, Y, 2, 2, 0, 2);
	matrix_multiply(X, Y, 2, 2, 2, 0);
	printf("\n\nChecking error cases end ! \n");


	// create matrix 
	for (i = 0; i < x_rows; i++) {
		for (int j = 0;j < x_cols;j++) {
				X[i][j] = ((i + 1) * (j + 1) + j);
				Y[j][i] = ((i + 1) * (j + 1) + j);
		}
	}
	printf("\n\nTest for Matrix muliplication begin...\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("X is:\n");
	print2Darray(X, x_rows, x_cols);
	printf("Y is:\n");
	print2Darray(Y, y_rows, y_cols);

	// test multiply
	int** res = matrix_multiply(X, Y, x_rows, x_cols, y_rows, y_cols);
	printf("\n------------------------\nX * Y is:\n");
	print2Darray(res, x_rows, y_cols);
	printf("\n\nTest for Matrix muliplication end\n");

	printf("\n\n");
	for (i = 0; i < x_rows; i++) {
		free(res[i]);
	}
	free(res);

	//create sparse matrix
	for (i = 0; i < x_rows; i++) {
		for (int j = 0;j < x_cols;j++) {
			if (((i + 1) * (j + 1) + i) % 2) {
				X[i][j] = 0;
				Y[j][i] = 0;
			}
			else {
				X[i][j] = ((i + 1) * (j + 1) + i);
				Y[j][i] = ((i + 1) * (j + 1) + j);
			}
		}
	}
	printf("\nTest for Sparse muliplication begin...\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("X is:\n");
	print2Darray(X, x_rows, x_cols);
	printf("\n------------- \n");
	printf("Y is:\n");
	print2Darray(Y, y_rows, y_cols);
	printf("\n");

	SPARSE = 1;
	res = matrix_multiply(X, Y, x_rows, x_cols, y_rows, y_cols);
	printf("\n------------------------\nX * Y is:\n");
	print2Darray(res, x_rows, y_cols);
	printf("\n\nTest for Sparse muliplication end !\n\n");

	// clean up 
	for (i = 0; i < x_rows; i++) {
		free(X[i]);
	}
	free(X);

	for (i = 0; i < y_rows; i++) {
		free(Y[i]);
	}
	free(Y);

	// free the 2D array memory used as return 
	// which is allocated inside the called function
	for (i = 0; i < x_rows; i++) {
		free(res[i]);
	}
	free(res);
	printf("clean up !\n\n\n");

}

/************************************************
* test function for array sort
* ***********************************************/
void test_array_sort() {
	// insertion sort test
	int arr[] = { 8,7,6,5,4,3,2,1 };
	printf("Insertion Sort Test\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
	printArray(arr, 8);
	sort(arr, 8);
	printArray(arr, 8);

	// quick sort test
	printf("Quick Sort Test\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
	int arr10[] = { 10,9, 9, 8,9,7,6,5,4,3,2,1 };
	printArray(arr10, 12);
	sort(arr10, 12);
	printArray(arr10, 12);

}



/*
* Helper function
*/
void printArray(int *arr_ptr, int arr_sz) {
	printf("array[] = {");
	for (int i = 0; i < arr_sz; i++) {
		printf("%3d ", *arr_ptr);
		arr_ptr++;
	}
	printf(" }\n\n");

}

/*
* Helper function
*/
void print2Darray(int **array, int r, int c) {
	int i;
	int j;

	for (j = 0; j < r; j++) {
		printf("\n");
		for (i = 0; i < c; i++) {
			printf("%3d ", array[j][i]);
		}
	}
	printf("\n\n");
}
