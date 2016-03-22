#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct ListNode
{
	int data;
	struct ListNode *next;
};

// function definitions
void p1(void);
void p2(void);
void p3(void);
void sort(int *a, int size);
void p4(void);
struct ListNode *listsort(struct ListNode *root);
struct ListNode *ptr_swap(struct ListNode *item1, struct ListNode *item2);
void p5(int cap);
int* simple_add(int *arr, int *cap, int *n, int new);
int* double_add(int *arr, int *cap, int *n, int new);
void rem(int *arr, int *n, int pos);
int get(int *arr, int pos);


int main(void)
{
	p1();
	p2();
	p3();
	p4();
	p5(10);
	printf("\nDone!\n");
	return 0;
}

// creates and prints integer array using malloc
void p1(void)
{
	printf("\nProblem 1:\n");
	
	int *ptr;		// declares integer pointer
	ptr = (int *)malloc(10 * sizeof(int));		// creates a 10 integer array
	
	int i;
	for (i=0; i<10; i++)		// assigns values to the array
	{ptr[i] = i;}

	for (i=0; i<10; i++)		// prints values in the array
	{printf("%d\n", ptr[i]);}

	free(ptr);		// frees memory
}

// creates and prints array of character arrays
void p2(void)
{
	printf("\nProblem 2:\n");

	char **ptr;			// declares pointer to pointer of chars
	ptr = (char **)malloc(10 * sizeof(char*));	// creates 10 char pointers

	int i;
	for (i=0; i<10; i++)
	{
		char arr[15] =		// initializes and assigns char pointers
		{'i','m','p','l','e','m','e','n','t','a','t','i','o','n','\0'};
		ptr[i] = arr;
	}
	
	for (i=0; i<10; i++)    // prints values
		{printf("%s\n", ptr[i]);}
	
	free(ptr);		// frees memory
}

// sorts an integer array using pointer arithmetic
void p3(void)
{
	printf("\nProblem 3:\n");

	int arr[10] = {9,2,0,4,3,5,6,7,1,8};	// declares and prints unsorted array
	printf("Original array: ");
	int i;
	for (i=0; i<10; i++)
		{printf("%d ", arr[i]);}

	sort(arr, 10);		// helper function
	printf("\nSorted array: ");	// prints sorted array
	for (i=0; i<10; i++)
		{printf("%d ", arr[i]);}
}

// p3 helper function
void sort(int *a, int size)
{
	int i;
	int j;
	int temp;

	for(i=0; i<size-1; i++)		// simple bubble sort, but using pointer arithmetic
	{
		for(j=0; j<size-i-1; j++)
		{
			if(*(a+j)>*(a+j+1))
			{
				temp = *(a+j);
				*(a+j) = *(a+j+1);
				*(a+j+1) = temp;
			}
		}
	}
}

// sorts a linked list by modifying the pointers
void p4(void)
{
	printf("\n\nProblem 4:\n");

	struct ListNode *root;
	struct ListNode *current;
	root = NULL;
	root = (struct ListNode *)malloc(sizeof(struct ListNode));	// creates root node
	root->data = 9;
	root->next = NULL;
	current = root;

	int i;
	for(i=0; i<9; i++)	// creates the rest of the linked list, numbers 0-9 in reverse order
	{
		current->next = (struct ListNode *)malloc(sizeof(struct ListNode));
		current->next->data = 8-i;
		current->next->next = NULL;
		current = current->next;
	}

	printf("Original list: ");
	current = root;
	while(current)
	{
		printf("%d ", current->data);	// prints unsorted list
		current = current->next;
	}

	root = listsort(root);	// helper function
	
	printf("\nSorted list: ");
	current = root;
	while(current)
	{
		printf("%d ", current->data);   // prints sorted list
		current = current->next;
	}
}

// p4 helper function
struct ListNode *listsort(struct ListNode *root)
{
	// heavily influenced by code found here:
		// http://faculty.salina.k-state.edu/tim/CMST302/study_guide/topic7/bubble.html
	struct ListNode *p;
	struct ListNode *q;
	struct ListNode *first;		// permanently marks the top of the list
	int swap = 1;					// denotes whether a swap has taken place each iteration

	first = (struct ListNode *)malloc(sizeof(struct ListNode));
	first->next = root;
	if(root != NULL && root->next != NULL)
	{
		while(swap)
		{
			swap = 0;
			q = first;
			p = first->next;	// q always follows p
			while(p->next != NULL)
			{
				if(p->data > p->next->data)
				{
					q->next = ptr_swap(p, p->next);	// helper function
					swap = 1;
				}
				q = p;
				if (p->next != NULL)
					{p = p->next;}
			}
		}	
	}
	p = first->next;	// returns p to the true root
	free(first);
	return p;
}

// p4 helper function
struct ListNode *ptr_swap(struct ListNode *item1, struct ListNode *item2)
{
	item1->next = item2->next;		// swaps pointers rather than just data
	item2->next = item1;
	return item2;
}

// does array operations
void p5(int cap)
{
	printf("\n\nProblem 5:\n");
	int n = 0;
	int cap2 = cap;
	int n2 = 0;

	int *arr = malloc(cap*sizeof(int));
	int *arr2 = malloc(cap*sizeof(int));

	int i;
	for(i=0; i<cap; i++)		// initial array of given size
		{
			arr = simple_add(arr, &cap, &n, i);
			arr2 = simple_add(arr2, &cap2, &n2, i);
		}
		
	printf("Original array: ");
	for(i=0; i<cap; i++)
		{printf("%d ", arr[i]);}
	
	int g = get(arr, 8); // retrieves an arbitrary number to prove get() works
   printf("\nRetrieving element number 8: %d", g);
	printf("\nRemoving element number 8...");
   rem(arr, &n, 8);     // demonstrates that removal works
	printf("\nNew array: ");
	for(i=0; i<n; i++)
		{printf("%d ", arr[i]);}
	g = get(arr, 8);
	printf("\nRetrieving new element number 8: %d\n", g);

	clock_t start;
   clock_t end;
	start = clock();	// timing simple add
	for(i=0; i<100000; i++)
		{arr = simple_add(arr, &cap, &n, 1);}
	end = clock();
	double msec = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
	printf("Adding 100000 elements...\nSimple add took %f msec\n", msec);

	start = clock();	// timing array doubling, usually runs 2-4 times quicker
	for(i=0; i<100000; i++)
		{arr2 = double_add(arr2, &cap2, &n2, 1);}
	end = clock();
	msec = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
	printf("Adding 100000 elements...\nArray doubling took %f msec\n",msec);
	
	free(arr);
	free(arr2);
}

// p5 helper function, increases array capacity by one
int *simple_add(int *arr, int *cap, int *n, int new)
{
	if(*n >= *cap)		// array full
	{
		arr = realloc(arr,(1+(*cap))*sizeof(int));
		*cap = 1+(*cap);
	}
	
	arr[*n] = new;		// adds in element
	*n = *n+1;
	return arr;
}

//p5 helper function, doubles array capacity
int* double_add(int *arr, int *cap, int *n, int new)
{
	if(*n >= *cap)		// array full
	{
		arr = realloc(arr,2*(*cap)*sizeof(int));
		*cap = 2*(*cap);
	}
	
	arr[*n] = new;		// adds in element
	*n = *n+1;
	return arr;
}

// p5 helper function, removes an element
void rem(int *arr, int *n, int pos)
{
	int i;
	for(i=pos; i<*n; i++)
		{arr[i] = arr[i+1];}		// shifts all elements after deleted one
	*n = *n-1;
}

// p5 helper function, indexes an element
int get(int *arr, int pos)
{
	return arr[pos];	// simply returns element in given position

}
