#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAX_NAME 32

int CompileFile(char* in,int nameLength){
    char out[MAX_NAME];
    strcpy(in+nameLength,".c\0");
    strcpy(out+nameLength,".out\0");
    printf("%s",in);
    printf("\n");
    printf("%s",out);
    execlp("gcc","gcc","%s","-o","%s",in,out);
}

int main(int argc, char** argv) {
    int fdin1, fdin2, fdin3;
    char cFile[MAX_NAME];
    char outFile[MAX_NAME];
    char buffer;
    int nameLength = 0;

    if (argc != 4){
        printf("Argument count is not correct. terminating...");
        return 0;
    }
    fdin1 = open("users.txt",O_RDONLY);
    if (fdin1 < 0){
        printf("Error, the file could not be open. terminating...");
        return 0;
    }
    while ((read(fdin1,&buffer,1)) != 0){
        if ((int) buffer == 10){
            CompileFile(cFile,nameLength);
        }
        cFile[nameLength] = buffer;
        nameLength++;
        //found space
    }
}