#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#define SIZE 2048

int main()
{
  int s,i;
	char str[]="originally";
	char *sharestr;
	int shmid;

    shmid=shmget (IPC_PRIVATE ,SIZE , 0600 );
    sharestr=(char *)shmat ( shmid ,0 ,0600 );
    strcpy ( sharestr , str );
	printf ( "before fork: %s\n" , sharestr );
    if ( fork()==0 )
	{
        strcpy ( sharestr , "son changed memory" );
		shmdt ( sharestr );
	}
	else
    {
        wait ( &s );
        printf ( "father prints: %s\n" , sharestr );
        shmdt ( sharestr );
    }
}
