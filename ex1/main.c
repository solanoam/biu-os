#include <stdio.h>
#include <sys/types.h>
#define MAX_NUM_FILES 3
#define MAX_FILE_NAME 20

int main(int argc, char **argv) {
    int fdin1;
    int fdin2;
    //char[MAX_FILE_NAME] tempName;
    if (argc != MAX_NUM_FILES) {
        printf("Please pass two files as arguments. Try again");
        return 0;
    }
//
    return 0;
}
