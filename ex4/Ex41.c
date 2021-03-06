/***************************************************************
 * Author: Noam Solan
 * ID: 204484703
 * Course: 83-381-01 (Operating Systems)
 * Practice: 83-381-02 (Yoram)
 * ENV: Ubuntu 16.04 LTS & macOS Mojave - x86_64-apple-darwin18
 ***************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

//macros
#define READ 0
#define WRITE 1
#define EXIT_BUTTON 'q'
#define RIGHT_BUTTON 'd'
#define LEFT_BUTTON 'a'
#define DOWN_BUTTON 's'
#define SWAP_BUTTON 'w'

//given function getch
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

// exit gracfully
void exitGracfully(int pipe[], int pid, int status){
    close(pipe[WRITE]);
    if (pid != 0){kill(pid, SIGKILL);}
    exit(status);
}
//handling fork error
int handleForkError(int pipe[], int pid){
    if (pid < 0){ //check if error happened
        exitGracfully(pipe, 0, -1);
    }
    return 1;
}

//function forks and runs draw proc
int runGame(int pipe[]){
    int childId = fork(); //forking
    if (childId == 0){ //child
        close(pipe[WRITE]);
        dup2(pipe[READ],0);
        close(pipe[READ]);
        execl("draw.out", "draw.out", NULL);
    } 
    handleForkError(pipe, childId); //father
    return childId;
}

//handeling key presses
void handleKeys(int pid, int* pipe){
    char keyPressed;
    while (true) {
        keyPressed = getch(); //getting key press from user
        //if it's a relevant key
        if (keyPressed == RIGHT_BUTTON || keyPressed == LEFT_BUTTON || keyPressed == DOWN_BUTTON || keyPressed == SWAP_BUTTON || keyPressed == EXIT_BUTTON){
            write(pipe[WRITE], &keyPressed, 1);
            if (keyPressed == EXIT_BUTTON) {exitGracfully(pipe, pid, 0); exit(-1);}
            kill(pid, SIGUSR2);
        }
    }
}

int main() {
    int fd_pipe[2];
    pipe(fd_pipe); //create pipe
    int childId = runGame(fd_pipe); //start draw.out
    handleKeys(childId, fd_pipe);
    return 0;
}