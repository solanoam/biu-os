#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 5

int main (int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Number of arguments passed is wrong - not 2\n");
		return 1;
	}

	char *file1 = argv[1];
	char *file2 = argv[2];

	int fdin1 = open(file1, O_RDONLY);
	int fdin2 = open(file2, O_RDONLY);
	
	int readf1;
	int readf2;

	char  f1[SIZE + 1];
	char  f2[SIZE + 1];

	if (fdin1 < 0 || fdin2 < 0)
    {
        perror("ERROR: opening files");
		return 1;
    }

	do
	{
		readf1 = read(fdin1,f1,SIZE);
		readf2 = read(fdin2,f2,SIZE);
		if(readf1 == -1 || readf2 == -1)
		{
	        close(fdin1);
			close(fdin2);
			printf("ERROR: reading files");
			return 1;
		}
		f1[readf1] = '\0';
		f2[readf2] = '\0';
		
		if ((strcmp(f1,f2) != 0) && (readf1 > 0 || readf2 > 0))
		{
			close(fdin1);
			close(fdin2);
			return 1;
		}
	}
	while(readf1 == SIZE && readf2 == SIZE);
	
	close(fdin1);
	close(fdin2);
	return 2;
}