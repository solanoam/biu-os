#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#define READ 0
#define WRITE 1
#define EXIT_BUTTON 'q'

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

void exitGracfully(int pipe[], int pid, int status){
    close(pipe[WRITE]);
    if (pid != 0){kill(pid, SIGKILL);}
    exit(status);
}

int handleForkError(int pipe[], int pid){
    if (pid < 0){ //check if error happened
        exitGracfully(pipe, 0, -1);
    }
    return 1;
}

//function forks and runs draw proc
//TODO handling errors
int runGame(int pipe[]){
    int childId = fork();
    printf("%d", childId);
    if (childId == 0){
        close(pipe[WRITE]);
        dup2(pipe[READ],0);
        close(pipe[READ]);
        execl("draw.out", "draw.out", NULL);
    }
    else {
        close(pipe[READ]);
        dup2(pipe[WRITE],1);
        close(pipe[WRITE]);
    }
    handleForkError(pipe, childId);
    return childId;
}
void writeKey(char keyPressed){
    printf("%c", keyPressed);
}

//handeling key presses
void handleKeys(int pid, int* pipe){
    char keyPressed;
    while (true) {
        keyPressed = getch();
        if (keyPressed == EXIT_BUTTON) {exitGracfully(pipe, pid, 0); exit(-1);}
        writeKey(keyPressed);
        kill(pid, SIGUSR2);
    }
}

int main() {
    int fd_pipe[2];
    pipe(fd_pipe);
    int childId = runGame(fd_pipe);
    handleKeys(childId, fd_pipe);
    return 0;
}