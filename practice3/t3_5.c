#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int pid, fd_in, fd_out, status;
	
	fd_in = open("t3_5_in.txt", O_RDWR);
	fd_out = open("t3_5_out.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);

	printf("This will go to stdout\n");
	
	// This 2 dups could be in the son's code, if we need the std I/O for the father
	dup2(fd_in, 0);
	dup2(fd_out, 1);

	pid = fork();
	if (pid > 0) // father
	{
		waitpid(pid, &status, 0);
		printf("Son exited with code: %d\n", WEXITSTATUS(status));
	}
	else // son
	{
		// dup2(fd_in, 0);
		// dup2(fd_out, 1);
		execl("./t3_5_ex.out", "t3_5_ex.out", NULL); // t3_5_ex.c has to be compiled manually before running this code (t3_5.out)
	}
	return 0;
}
