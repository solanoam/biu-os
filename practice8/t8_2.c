#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int initsem(key_t key, int nsems)  /* key from ftok() */
{
	int i;
	union semun arg;
	struct sembuf sb;
	int semid;

	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

	if (semid >= 0)
	{ /* we got it first */
		sb.sem_op = 1;
		sb.sem_flg = 0;

		for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++)
		{
			/* do a semop() to "free" the semaphores. */
			if (semop(semid, &sb, 1) == -1)
			{
				int e = errno;
				semctl(semid, 0, IPC_RMID, NULL); /* clean up */
				errno = e;
				return -1; /* error, check errno */
			}
		}
	}
	else if (errno == EEXIST) /* someone else created it first */
	{
		int ready = 0;
		printf("someone else created it\n");

		semid = semget(key, nsems, 0); /* get the id */
		if (semid < 0)
			return -1; /* error, check errno */
	}

	return semid;
}

int main(void)
{
	key_t key;
	int semid;
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = -1;  /* set to allocate resource */
	sb.sem_flg = 0;

	if ((key = ftok("blabla", 'O')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	/* grab the semaphore set created by seminit.c: */
	if ((semid = initsem(key, 1)) == -1)
	{
		perror("initsem");
		exit(1);
	}

	printf("Press return to lock: ");
	getchar();
	printf("Trying to lock...\n");

	if (semop(semid, &sb, 1) == -1)
	{
		perror("semop");
		exit(1);
	}

	printf("Locked.\n");
	printf("Press return to unlock: ");
	getchar();

	sb.sem_op = 1; /* free resource */
	if (semop(semid, &sb, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
	printf("Unlocked\n");

	return 0;
}
