#include<stdlib.h>
#include<stdio.h>
#include<string.h>


const int MAX_WORD_LENGTH = 128;	// suggested by given file I/O code
const int MAX_BUCKETS = 260;		// arbitrary, could hold "zzzzzzzzzz"
typedef struct HashBucket HashBucket;
typedef struct HashTable HashTable;

struct HashBucket	// HashTable nodes, each holding a word
{
	char *data;
	HashBucket *next;
};

struct HashTable	// each HashTable contains all words of a given length
{
	HashBucket **buckets;
};


void hash_alloc(HashTable **arr);
void hash_insert(HashTable **arr, char *word);
void print_list(HashBucket *first);

int main(int argc, const char *argv[])
{
	HashTable **arr = (HashTable **) malloc(sizeof(HashTable *) * MAX_WORD_LENGTH);	// creates array of HashTables
	int i;
	for(i=0; i < MAX_WORD_LENGTH; i++)	// creates HashTables and HashBuckets
		{hash_alloc(&arr[i]);}
	
	FILE *dict = fopen("/usr/share/dict/words", "r");	// reads dictionary
	char *word = (char *) malloc(MAX_WORD_LENGTH * sizeof(char));
	while(fgets(word, MAX_WORD_LENGTH, dict) != NULL)	// inserts word into correct HashTable
	{
		//printf("%s", word);
		hash_insert(arr, word);
	}
	free(word);
	fclose(dict);

	print_list(arr[0]->buckets[1]);


	printf("Hello World\n");
	return(0);
}

int hash(char *word)
{
	int sum = 0;
	int letter;
	int i;

	for(i=0; i<strlen(word); i++)
	{
		letter = word[i];
		if(letter >= 65 && letter <= 90)
			{letter += 32;}	// turns capital letters to lowercase
		letter = letter - 96;	// adjusts so a=1,b=2...
		sum += letter;	// tallies the sum of the word
	}
	if(sum > MAX_BUCKETS - 1)	// words that sum to larger than the largest HashBucket are placed in the
		{sum = MAX_BUCKETS - 1;}		// largest HashBucket
	
	return sum;
}



void hash_alloc(HashTable **arr)	// creates HashTable and HashBuckets
{
	*arr = (HashTable *) malloc(sizeof(HashTable));
	(*arr)->buckets = (HashBucket **) malloc(sizeof(HashBucket *) * MAX_BUCKETS);
}

void hash_insert(HashTable **arr, char *word)	// inserts word into correct HashTable
{
	int sum = hash(word);
	HashTable *table = arr[strlen(word)];	// accesses the correct HashTable based on word length
	HashBucket *newBucket = (HashBucket *) malloc(sizeof(HashBucket));
	newBucket->data = strdup(word);
	newBucket->next = table->buckets[sum];	// adjusts the list
	table->buckets[sum] = newBucket;
}

void print_list(HashBucket* first) {
	  HashBucket* current = first;
	    while(current != NULL) {
			     if(current->data != NULL) {
					        printf("%s\n", current->data);
							      }
				      current = current->next;
						  }
		   printf("\n");
}

