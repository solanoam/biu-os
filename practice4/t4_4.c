#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>

void boom(int signum)
{
	printf("Now we know how to use kill command. \nLearning operating systems is pretty fun. \nLet's continue!\n");
	exit(1);
}
int main()
{
	signal(SIGUSR1, boom);
	while(1);
}

