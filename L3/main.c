#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

const int MAX_FILENAME_LENGTH = 100;	// used for malloc

void f_sync(char *dirA, char *dirB);
void f_copy(char *src, char *dst);
void f_del(char *dirA, char *dirB);
void dir_del(char *direct);


int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Invalid arguments\n");
		return EXIT_FAILURE;
	}
	else
	{
		char *dirA = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
		char *dirB = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
		
		sprintf(dirA, "./%s", argv[1]);	// appends "./" to main directory
		sprintf(dirB, "./%s", argv[2]);	// appends "./" to syncing directory
		
		f_sync(dirA, dirB);		// copies files directories
		f_del(dirA, dirB);		// deletes unwanted files
		free(dirA);				// frees memory
		free(dirB);
	}

	printf("\n");
	return EXIT_SUCCESS;
}

void f_sync(char *dirA, char *dirB)
{
	// based on code that lists all files in a directory, from
		// http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
	
	DIR *d;
	struct dirent *dir;

	d = opendir(dirA);
	while ((dir = readdir(d)) != NULL)	// looking at a file
	{
		if(strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0)	// ignore source and parent directories
		{
			char *sourcefile = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
			char *destfile = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);

			sprintf(sourcefile, "%s/%s", dirA, dir->d_name);	// appends filename to directory for complete path
			sprintf(destfile, "%s/%s", dirB, dir->d_name);

			DIR *checkSource = opendir(sourcefile);	// checks if source is a directory
			if(checkSource != NULL)
			{
				closedir(checkSource);
				DIR *checkDest = opendir(destfile);		// check if destination directory exists
				if(checkDest != NULL)
				{ closedir(checkDest); }
				else
				{ 
					printf("\nDirectory %s not found in %s, creating...\n", dir->d_name, dirB);
					mkdir(destfile, 0777);			// create it if not
					printf("%s created successfully\n", dir->d_name);
				}
				f_sync(sourcefile, destfile);		// recursively sync subdirectories
			}
			else	// file is not a directory
			{
				FILE *file;
				file = fopen(destfile, "r");	// checks if file exists in syncing directory
			
				// if file exists in syncing directory, check last modified dates
					// and copy the newer file to the other location
				if(file != NULL)
				{
					fclose(file);
					
					struct stat *s1 = (struct stat *) malloc(sizeof(struct stat));		// stat structure includes st_mtime,
					struct stat *s2 = (struct stat *) malloc(sizeof(struct stat));			// last modified time
					long t1, t2;
					
					if((stat(sourcefile, s1)) == 0)
					{ t1 = s1->st_mtime; }		// gets last modified times
					if((stat(destfile, s2)) == 0)
					{ t2 = s2->st_mtime; }
					
					if(t1 > t2)		// main directory file is newer
					{
						printf("\n%s is newer than %s, updating...\n", sourcefile, destfile);
						f_copy(sourcefile, destfile);
						printf("%s updated successfully\n", destfile);
					}
					else if(t2 > t1)	// syncing directory file is newer
					{
						printf("\n%s is newer than %s, updating...\n", destfile, sourcefile);
						f_copy(destfile, sourcefile);
						printf("%s updated successfully\n", destfile);
					}
					
					free(s1);
					free(s2);
				}
				
				// if file does not exist in syncing directory, copy it there
				else
				{
					printf("\n%s not found in %s, copying...\n", dir->d_name, dirB);
					f_copy(sourcefile, destfile);
					printf("%s copied successfully\n", dir->d_name);
				}
			}
			
			free(sourcefile);
			free(destfile);
		}
	}
	closedir(d);
}

// copies contents of file src to file dst (full paths given) 
void f_copy(char *src, char *dst)
{
	char c;
	FILE *source, *dest;
	source = fopen(src, "r");	// open for read
	dest = fopen(dst, "w");		// open for write, overwriting existing file or creating one new
	
	while ((c = fgetc(source)) != EOF)	// copies file one char at a time until EOF
	{ fputc(c, dest); }

	fclose(source);
	fclose(dest);
}

// delete files from syncing directory not in main directory
		// same code as above, but reversing main and syncing directories.  If
		// a file exists in syncing directory but not in main, delete it
void f_del(char *dirA, char *dirB)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(dirB);
	while ((dir = readdir(d)) != NULL)
	{
		if(strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0)
		{
			char *sourcefile = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
			char *destfile = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
			
			sprintf(sourcefile, "%s/%s", dirB, dir->d_name);
			sprintf(destfile, "%s/%s", dirA, dir->d_name);
			
			DIR *checkSource = opendir(sourcefile);	// checks if source is a directory
			if(checkSource != NULL)
			{
				closedir(checkSource);
				DIR *checkDest = opendir(destfile);
				if(checkDest != NULL)		// if destination directory also exists, recurse into it
				{
					closedir(checkDest);
					f_del(destfile, sourcefile);
				}
				else if(checkDest == NULL)	// otherwise recursively delete
				{
					printf("\nDirectory %s not found in %s, deleting...\n", dir->d_name, dirA);
					dir_del(sourcefile);
					printf("%s deleted successfully\n", dir->d_name);
				}
			}
			else	// source is not a directory, look for matching file in destination
			{
				FILE *file;
				file = fopen(destfile, "r");
				if(file != NULL)
				{ fclose(file); }
				else	// delete from source if not found
				{
					printf("\n%s not found in %s, deleting...\n", dir->d_name, dirA);
					remove(sourcefile);
					printf("%s deleted successfully\n", dir->d_name);
				}
			}
			
			free(sourcefile);
			free(destfile);
		}
	}
	closedir(d);
}

// recursively deletes directories
void dir_del(char *direct)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(direct);
	while ((dir = readdir(d)) != NULL)
	{
		if(strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0)
		{
			char *target = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
			sprintf(target, "%s/%s", direct, dir->d_name);
			
			DIR *checkTarget = opendir(target);	// checks if target file is a directory
			if(checkTarget != NULL)
			{
				closedir(checkTarget);
				dir_del(target);
			}
			else	// not a directory, normal remove
			{ remove(target); }
			
			free(target);
		}
	}
	closedir(d);
	rmdir(direct);		// finally, delete this empty directory
}
