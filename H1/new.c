#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int MAX_WORD_LENGTH = 128;		// suggested by given file I/O code
const int MAX_BUCKETS = 260;			// arbitrary, could hold up to "zzzzzzzzzz"
typedef struct HashBucket HashBucket;
typedef struct HashTable HashTable;

struct HashBucket	// HashtTable nodes, each holding a word
{
	char *data;
	HashBucket *next;
};

struct HashTable	// each HashTable contains all words of a given length
{
	HashBucket **buckets;
};

int hash(char *word);
void hash_alloc(HashTable **arr);
void hash_dealloc(HashTable *arr);
void hash_insert(HashTable **arr, char *word);
void anagram(HashTable *arr, char *word, char let, int pos);
void str_sort(char *word);


int main(int argc, const char *argv[])
{
	HashTable **arr = (HashTable**) malloc(sizeof(HashTable *) * MAX_WORD_LENGTH);	// creates array of HashTables
	int i;
	for(i = 0; i < MAX_WORD_LENGTH; i++)	// creates HashTables and HashBuckets
		{hash_alloc(&arr[i]);}

	FILE *dict = fopen("/usr/share/dict/words", "r");	// reads dictionary
	char *word = (char *) malloc(MAX_WORD_LENGTH * sizeof(char));
	while(fgets(word, MAX_WORD_LENGTH, dict) != NULL)	// inserts word into correct HashTable
	{
		word[strlen(word)-1] = '\0';	// trims off newline character at end of word
		hash_insert(arr, word);
	}

	free(word);
	fclose(dict);	// closes dictionary

	char *ana = strdup(argv[2]);	// word to look for anagrams for
	char let;		// desired letter for scrabble functionality
	int pos;			// desired position for scrabble functionality

	switch (argc) {
		case 3:		// find anagrams
			let = '\0';
			pos = -1;
			printf("\nanagrams of '%s':\n", ana);
			anagram(arr[strlen(ana)], ana, let, pos);
			free(ana);
			break;
		
		case 5:	// find scrabble
			ana = strdup(argv[2]);
			let = argv[3][0];
			let = tolower(let);  // adjusts capital letters to lowercase
			pos = atoi(argv[4]);
			
			if(let < 97 || let > 122)	// error check for letter being a letter
			{
				printf("scrabble character must be a letter\n");
				break;
			}
			if(pos < 0 || pos > strlen(ana))	// error check for position being in range
			{
				printf("scrabble position must be in range\n");
				break;
			}
			
			printf("\nanagrams of '%s' with letter '%c' at position %i:\n", ana, let, pos);
			anagram(arr[strlen(ana)], ana, let, pos);
			free(ana);
			break;
		
		default:
			printf("\ninvalid arguments\n");
			break;
	}
	printf("\n");

	for(i = 0; i < MAX_WORD_LENGTH; i++)	// frees HashTable memory
		{hash_dealloc(arr[i]);}
	free(arr);	// frees array memory
	return 0;
}

int hash(char *word)	// hashes by summing the word's ASCII characters
{
	int i;
	char let;
	int sum = 0;
	for (i = 0; i < strlen(word); i++)
	{
		let = word[i];
		let = tolower(let);	// converts to lowercase
		word[i] = tolower(word[i]);
		if(let < 97 || let > 122)	// ignores non-letter characters
			{continue;}
		let = let - 96;	// adjusts so a=1, b=2...
		sum += let;		// tallies the sum of the word
	}
	if(sum > MAX_BUCKETS - 1)	// words which sum to larger than the largets HashBucket are placed
		{sum = MAX_BUCKETS - 1;}		// in that largest HashBucket
	return sum;
}

void hash_alloc(HashTable** arr) // creates HashTable and HashBuckets
{
	*arr = (HashTable *) malloc(sizeof(HashTable));
	(*arr)->buckets = (HashBucket **) malloc(sizeof(HashBucket*) * MAX_BUCKETS);
}

void hash_dealloc(HashTable *arr)	// deallocates HashTable and HashBuckets
{
	HashBucket *bucket;
	int i;
	for(i = 0; i < MAX_BUCKETS; i++) {
		bucket = arr->buckets[i];
		while(bucket != NULL)	// frees data and moves to the next bucket
		{
			free(bucket->data);
			bucket = bucket->next;
		}
		free(bucket);	// frees the bucket pointer once the whole list has been freed
	}
	free(arr->buckets);	// frees the HashTable's pointer to its (now empty) HashBuckets
	free(arr);	// frees the HashTable itself
}


void hash_insert(HashTable **arr, char *word)	// inserts word into correct HashTable
{
	int sum = hash(word);	// hashes by summing the word's ASCII characters
	HashTable *table = arr[strlen(word)];	// accesses the correct HashTable based on word length
	HashBucket *newBucket = (HashBucket*) malloc(sizeof(HashBucket));
	newBucket->data = strdup(word);
	newBucket->next = table->buckets[sum];		// adjusts the list
	table->buckets[sum] = newBucket;
}

void anagram(HashTable *arr, char *word, char let, int pos)	// finds anagrams
{
	int sum = hash(word);
	char* w = NULL;
	str_sort(word);	// simple sort
	HashBucket *bucket = arr->buckets[sum];	// accesses the correct HashBucket
	while(bucket != NULL) 
	{
		if(bucket->data) 
		{
			w = strdup(bucket->data);	// copies word to sort (but keeps original to print if it's a match)
			str_sort(w);
			if(strcmp(word, w) == 0)	// match
			{
				if(let == '\0' && pos == -1)	// simple anagram
					{printf("%s\n", bucket->data);}
				else if((bucket->data)[pos] == let)	// scrabble, meeting letter position requirement 
					{printf("%s\n", bucket->data);}
			}
			free(w);
		}
		bucket = bucket->next;	// moves to the next word in the list
	}
}

void str_sort(char *word)	// simple sort
{
	char temp;
	int i;
	int j;
	int len = strlen(word);
	for(i=0; i<len - 1; i++)
	{
		for(j=i+1; j<len; j++)
		{
			if(word[i] > word[j])
			{
				temp = word[i];
				word[i] = word[j];
				word[j] = temp;
			}
		}
	}
}

