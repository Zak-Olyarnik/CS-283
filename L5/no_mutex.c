#include<stdio.h>
#include "csapp.h"

const int NUM_THREADS = 100;

typedef struct
{ int *count; } arg_struct;	// structure used to pass arguments to thread


// no mutex version
void *countThread(void *arg)
{
	int *count = ((arg_struct *) arg)->count;		// reads count out of struct
	int i;
	for(i=0; i<1000; i++)
	{ (*count)++; }		// increments count directly
	Pthread_exit(0);
}

int main()
{
	volatile int count = 0;			// this throws a compile warning
	pthread_t tid[NUM_THREADS];	// create array to hold pthreads
	arg_struct thread_args;
	thread_args.count = &count;	// give struct access to count

	int i, j;
	clock_t start, end;
	double total, average = 0;
	
	printf("No mutex runs:\n\n");
	for(j=0; j<10; j++)	// ten runs for analytics
	{
		count = 0;
		start = clock();		// start timer

		for(i=0; i<NUM_THREADS; i++)	// create 1000 threads and pass in struct
		{ Pthread_create(&tid[i], NULL, countThread, &thread_args); }

		for(i=0; i<NUM_THREADS; i++)	// reap all 1000 threads
		{ Pthread_join(tid[i], NULL); }

		end = clock();		// end timer
		total = (double)(end-start)/CLOCKS_PER_SEC;
		average += total;

		printf("final count = %d\n", count);
		printf("time taken = %f\n\n", total);
	}

	average = average/10;
	printf("average time per run = %f\n\n", average);
	Pthread_exit(0);
}
