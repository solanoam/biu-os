#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_ARG 3


//closing all open fd
void closeAll(int fd1, int fd2){
    close(fd1);
    close(fd2);
}

int main(int argc, char **argv) {
    int fdin1;
    int fdin2;
    int buf1 = 0;
    int buf2 = 0;
    int readStatus1 = 0;
    int readStatus2 = 0;
    if (argc != MAX_ARG) {
        printf("Please pass two files as arguments. Try again\n");
        return 0;
    }
    fdin1 = open(argv[1], O_RDONLY);
    if (fdin1 == -1){
        printf("Error while opening the first argument. closing...\n");
        return -1;
    }
    fdin2 = open(argv[2], O_RDONLY);
    if (fdin2 == -1){
        printf("Error while opening the second argument. closing...\n");
        //closing fd1 that was openned.
        close(fdin1);
        return -1;
    }
    readStatus1 = (int) read(fdin1, &buf1, 1);
    if (readStatus1 == -1){
        printf("Error reading fd 1. closing\n");
        closeAll(fdin1, fdin2);
        return -1;
    }
    while (readStatus1 != 0){
        readStatus2 = (int) read(fdin2,&buf2,1);
        if (readStatus2 == -1){
            printf("Error reading fd 2. closing\n");
            closeAll(fdin1, fdin2);
            return -1;
        }
        if (readStatus2 == 0){
            //files are not the same - file 2 is shorter
            printf("1\n");
            closeAll(fdin1,fdin2);
            return 1;
        }
        //check if bytes are the same
        if (buf1 != buf2){
            //bytes are not the same, files are diffrent
            printf("1\n");
            closeAll(fdin1,fdin2);
            return 1;
        }
        //moving to the next byte
        readStatus1 = (int) (read(fdin1, &buf1, 1));
        if (readStatus1 == -1){
            printf("Error reading fd 1. closing\n");
            closeAll(fdin1,fdin2);
            return -1;
        }
    }
    //checking that file 2 isn't longer
    readStatus2 = (int) read(fdin2,&buf2,1);

    if (readStatus2 != 0){
        //file 2 is longer then file 1
        printf("1\n");
        closeAll(fdin1,fdin2);
        return 1;
    }
    printf("2\n");
    closeAll(fdin1,fdin2);
    return 2;
}
