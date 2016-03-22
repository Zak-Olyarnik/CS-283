#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

const int MAX_LENGTH = 2000;		// easier than constantly reallocating memory
char *str_replace(char *src, char *pattern, char *replace);
void f_search(char *pattern, char *replace, char *action, char *fnames, char *directory);

int main(int argc, char *argv[])
{
	int c;
	int err = 0;	// count to make sure all mandatory arguments are present
	extern char *optarg;		// used by getopt, value of next argument
	char *pattern, *replace, *action, *fnames = NULL;	// arguments

	while((c = getopt(argc, argv, "p:r:a:f::")) != -1)	// parses argumets, : means a value follows and :: means optional
	{
		switch(c)
		{
			case 'p':
				pattern = strdup(optarg);		// pattern to look for
				err += 1;
				break;
			case 'r':
				replace = strdup(optarg);		// pattern to replace or put replacement before
				err += 1;
				break;
			case 'a':
				action = strdup(optarg);		// either "filenames" or "filecontents"
				err += 1;
				break;
			case 'f':
				if(optarg)
				{ fnames = strdup(optarg); }	// if fnames parameter is given
				break;
			default:
				break;
		}
	}

	if(err < 3)		// check for all mandatory arguments
	{ printf("invalid arguments\n"); }
	else
	{
		printf("\n");
		f_search(pattern, replace, action, fnames, "sub");
		printf("\n");
	
		free(pattern);		// free memory
		free(replace);
		free(action);
		if(fnames != NULL)
		{ free(fnames); }
	}	
	return 0;
}

void f_search(char *pattern, char *replace, char *action, char *fnames, char *directory)
{
	DIR *d;
	struct dirent *dir;
	char *direc = strdup(directory);		// no idea why this is necessary either, but without it directory variable gets corrupted

	d = opendir(direc);
	while((dir = readdir(d)) != NULL)	// looking at a file
	{
		if(strcmp(dir->d_name, ".." ) != 0 && strcmp(dir->d_name, ".") != 0) // ignore source and parent directories
		{
			char src[MAX_LENGTH];
			sprintf(src, "%s/%s", direc, dir->d_name);	// full path of current file
			char *new = strdup(dir->d_name);	// file (name only) to be changed if necessary
			char full[MAX_LENGTH];
			sprintf(full,"%s/%s", direc, dir->d_name);	// full path to be changed if necessary


			DIR *checksrc = opendir(src);	// checks if src is a directory
			if(checksrc != NULL)
			{
				closedir(checksrc);
				
				if(strcmp(action, "filenames") == 0)	// rename
				{
					if(fnames == NULL || (fnames != NULL && strstr(new, fnames)))	// check for optional fnames parameter
					{
						while(strstr(new, pattern))	// loop to look for multiple occurrences of pattern
						{ new = str_replace(new, pattern, replace); }	// replace pattern
				  		memset(full, 0, sizeof(full));			// clear path to file
						sprintf(full,"%s/%s", direc, new);			// and replace it with new path and name
						if(strcmp(src, full) != 0)		// only do rename if filename has changed
						{
							printf("renaming directory %s to %s\n", src, full);
							rename(src, full);		// library function to actually rename file
						}
					}
				}
				f_search(pattern, replace, action, fnames, full);	// recursively search subdirectories
			}
			else	// file is not a directory
			{
				if(fnames == NULL || (fnames != NULL && strstr(new, fnames)))	// check for optional fnames parameter
				{
					if(strcmp(action, "filenames") == 0)	// rename
					{
						while(strstr(new, pattern))   // loop to look for multiple occurrences of pattern
						{ new = str_replace(new, pattern, replace); }	// replace pattern
						memset(full, 0, sizeof(full));			// clear path to file
						sprintf(full,"%s/%s", direc, new);			// and replace it with new path and name
						if(strcmp(src, full) != 0)		// only do rename if filename has changed
						{
							printf("renaming %s to %s\n", src, full);
							rename(src, full);		// library function to actually rename file
						}
					}
					else if(strcmp(action, "filecontents") == 0)	// search within file
					{
						FILE *in;
						char buffer[MAX_LENGTH];
						int foundflag = 0;

						in = fopen(full, "r");	// open source file
						while(fgets(buffer, MAX_LENGTH, in) && foundflag == 0)	// short circuits search
						{
							if(strstr(buffer, pattern))	// pattern is found, quit
							{ foundflag = 1; }
						}
						if(foundflag == 0)	// only continue if pattern is not already present
						{ 
							rewind(in);	// return to beginning of file

							while(fgets(buffer, MAX_LENGTH, in) && foundflag == 0)	// look for word to append in front of
							{
								if(strstr(buffer, replace))	// word is found, quit
								{ foundflag = 1; }
							}
							if(foundflag == 1)	// only continue if word is present
							{
								printf("inserting pattern '%s' into file %s\n", pattern, full);
								rewind(in);
								
								// based on http://stackoverflow.com/questions/5754536/replacing-strings-in-files-c
									// but edited to append rather than replace
								char *buffptr, *foundptr;
								FILE *out;
								char copy[MAX_LENGTH];
								sprintf(copy, "%s/copy.txt", direc);	// define temp file
								out = fopen(copy, "w");
								while(fgets(buffer, MAX_LENGTH, in))	// parse file line by line
								{
									buffptr = buffer;
									while((foundptr = strstr(buffptr, replace)))	// position of word
									{
										while(buffptr < foundptr)	// copy characters up to match
										{ fputc((int)*buffptr++, out); }
										fputs(pattern, out);		// then copy pattern to append
										fputs(" ", out);			// add a space between the words
										fputs(replace, out);		// continue with the original word
										buffptr += strlen(replace);	// adjust buffptr to continue parsing file
									}
									fputs(buffptr, out);	// when there are no more matches, just copy the rest of the line
								}
								fclose(out);
								fclose(in);
								in = NULL;

								remove(full);			// delete original file
								rename(copy, full);	// rename new file
							}
						}
						if(in)
						{ fclose(in); }
					}
				}
			}
		}
	}
	closedir(d);
	free(direc);
}


// heavily influenced by
// http://www.linuxquestions.org/questions/programming-9/replace-a-substring-with-another-string-in-c-170076/
char *str_replace(char *src, char *pattern, char *replace)
{
	static char buffer[2000];	// won't let me use my MAX_LENGTH constant here
	char *p;

	p = strstr(src, pattern);		// find location of pattern match
	strncpy(buffer, src, p-src);	// copy chars up to pattern into buffer
	char rest[MAX_LENGTH];			// trying to do sprintf in one step failed for some reason, so an extra array 
												// is necessary
	sprintf(rest, "%s%s", replace, p+strlen(pattern));	// concatenate the replacement pattern and the rest of the
																			// original string
	sprintf(buffer+(p-src), "%s", rest);			// concatenate them into the buffer
	return buffer;
}
