#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
int in_fd;
int counter=0;
int curr_write=0, curr_read=0;
void my_read()
{
	in_fd=open("in.txt",O_RDWR | O_CREAT | O_SYNC,0666);
	char buf[10];
	int count=0;
	count=read(in_fd,buf,10);
	curr_read+=count;
	if(count!=0){
		buf[count-1]='\0';
		printf("I read now: %s, the num of bytes= %d\n",buf,count);
		exit(0);
	}
	else
	{
		printf("read nothing - just wasting CPU time\n");
		close(in_fd);
	}
}
void my_write()
{
	char buf[8]="hello";
	int count=0;
	buf[5]=(char)(counter+'0');
	buf[6]='\n';
	count=write(in_fd,buf,7);
	curr_write+=count;
	counter++;
}

int main()
{
	pid_t child_id;
	child_id=fork();
	if(child_id>0)
	{	//father
		in_fd=open("in.txt",O_RDWR |O_CREAT | O_TRUNC | O_SYNC,0666);
		sleep(1);
		my_write();
		exit(0);

	}else{
		while(1)
		{
			my_read();
		}

	}

}
