#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//global variables
#define CORE 4 //const
#define SIZE 12

int matrix[SIZE][SIZE];// Matrix
int arr[SIZE * SIZE];// The numbers array
int index_odd = 1;// The running index for uneven numbers
int index_even = 0;// The running index for even numbers
int arr_Num_Threads[SIZE * SIZE];// Array to keep for each number which thread found him
int counter = 0;// Counter
int i;//variable for iteration 
int retcode;//variable for create a Thread
pthread_t th[CORE];// Threads array


//the function of the threads
void* mat_to_arr(void* arg)
{
	int remainder =  SIZE % CORE; // remainder to know how much rows left to go.
	int NewRemainder = remainder; // remainder for work.
	int ROW = (int)arg;// Convert the arg to int
	int NewROW=ROW;//Row variable for work.
	int check = SIZE / CORE;//variable for iteration 
	if (SIZE<CORE)//Check if the size smaller then the core the fall on the first thread.
	{
		while (NewRemainder > 0)
		{
			for (int k = 0; k < SIZE; k++)// Iteratre through the Matrix
			{
				if (matrix[NewROW][k] % 2 == 0)// Check if even
				{
					arr[index_even] = matrix[NewROW][k];// If so put in an even index
					index_even = index_even + 2;
				}
				else
				{
					arr[index_odd] = matrix[NewROW][k];// If not put in an odd index
					index_odd = index_odd + 2;
				}
				arr_Num_Threads[counter] = ROW;// For each group of numbers keep the thread number of the thread that found them.
				counter++;
			}
			NewRemainder = NewRemainder - 1;
			NewROW = NewROW + 1;
		}
		return ((void*)0);
	}
	if (remainder == 0 )// If the remainder Equal to zero 
	{
		for (int n = 0; n < SIZE; n++)// Iteratre through the Matrix
		{
			if (matrix[ROW][n] % 2 == 0)// Check if even
			{
				arr[index_even] = matrix[ROW][n];// If so put in an even index
				index_even = index_even + 2;
			}
			else
			{
				arr[index_odd] = matrix[ROW][n];// If not put in an odd index
				index_odd = index_odd + 2;
			}
			arr_Num_Threads[counter] = ROW;// For each group of numbers keep the thread number of the thread that found them.
			counter++;
		}
		check = check - 1;
		while (check>0)// Check if left work for the Thread
		{
			NewROW = NewROW + CORE;// Next row is the current plus core
			for (int k = 0; k < SIZE; k++)// Iteratre through the Matrix
			{
				if (matrix[NewROW][k] % 2 == 0)// Check if even
				{
					arr[index_even] = matrix[NewROW][k];// If so put in an even index
					index_even = index_even + 2;
				}
				else
				{
					arr[index_odd] = matrix[NewROW][k];// If not put in an odd index
					index_odd = index_odd + 2;
				}
				arr_Num_Threads[counter] = ROW;// For each group of numbers keep the thread number of the thread that found them.
				counter++;
			}
			check = check - 1;
		}

	}
	else if (remainder == 1 || 2 || 3)// If the remainder Equal to 1 or 2 or 3.
	{
		for (int n = 0; n < SIZE; n++)// Iteratre through the Matrix
		{
			if (matrix[ROW][n] % 2 == 0)// Check if even
			{
				arr[index_even] = matrix[ROW][n];// If so put in an even index
				index_even = index_even + 2;
			}
			else
			{
				arr[index_odd] = matrix[ROW][n];// If not put in an odd index
				index_odd = index_odd + 2;
			}
			arr_Num_Threads[counter] = ROW;// For each group of numbers keep the thread number of the thread that found them.
			counter++;
		}
		check = check - 1;
		while (check > 0)// Check if left work for the Thread
		{
			NewROW = NewROW + CORE;// Next row is the current plus core
			for (int k = 0; k < SIZE; k++)
			{
				if (matrix[NewROW][k] % 2 == 0)// Iteratre through the Matrix
				{
					arr[index_even] = matrix[NewROW][k];// Check if even
					index_even = index_even + 2;
				}
				else
				{
					arr[index_odd] = matrix[NewROW][k];// If so put in an even index
					index_odd = index_odd + 2;
				}
				arr_Num_Threads[counter] = ROW;// For each group of numbers keep the thread number of the thread that found them.
				counter++;
			}
			check = check - 1;
		}
		if (ROW == 3)// Check if we are on the last Thread.
		{
			while (NewRemainder>0)
			{
				NewROW = NewROW + 1;
				for (int k = 0; k < SIZE; k++)
				{
					if (matrix[NewROW][k] % 2 == 0)
					{
						arr[index_even] = matrix[NewROW][k];
						index_even = index_even + 2;
					}
					else
					{
						arr[index_odd] = matrix[NewROW][k];
						index_odd = index_odd + 2;
					}
					arr_Num_Threads[counter] = ROW;
					counter++;
				}
				NewRemainder = NewRemainder - 1;
			}
		}
	}
	return ((void*)0);
}




int main()
{
	// Fill the matrix
	for ( int k = 0; k < SIZE; k++) 
	{
		if (k == 0)
		{
			for (int l = 0; l < SIZE; l++)
			{
				matrix[k][l] = l + 1;
			}
			
		}
		else
		{
			for (int j = 0; j < SIZE; j++)
			{
				matrix[k][j] = matrix[k - 1][j] + SIZE;
			}
		}
		
	}
	//create the threads
	for (i = 0; i < CORE; i++) {
		retcode = pthread_create(&th[i], NULL, mat_to_arr, (void*)(i));
		if (retcode != 0)
			printf("Create thread failed with error %d\n", retcode);
		if (SIZE < CORE)//Check if the size smaller then the core the fall on the first thread.
		{
			break;
		}
	}

	//wait for the threads
	for (i = 0; i < CORE; i++) {
		pthread_join(th[i], NULL);
	} 

	//print the matrix
	printf("Matrix:\n");
	for (int y = 0; y < SIZE; y++)
	{
		for (int u = 0; u < SIZE; u++)
		{
			printf("%d\t", matrix[y][u]);
		}
		printf("\n");
	}
	//print the new array
	printf("The arranged array:\n");
	for (int z = 0; z < SIZE * SIZE; z++)
	{
		printf("%d,", arr[z]);
	}
	printf("\n");
	//print the numbers in the array and whice thread insert them
	for (int t = 0; t < SIZE * SIZE; t++)
	{
		printf("Num %d was found by thread %d\n", arr[t], arr_Num_Threads[t]);
	}

	exit(0);
	
}




