#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAX_NAME 32

void closeAll(int fd1 = 0, int fd2 = 0, int fd3 = 0){
    if (fdin1 != 0) {close(fd1)}
    if (fdin2 != 0) {close(fd2)}
    if (fdin3 != 0) {close(fd3)}
}

int EvaluateCode(char* file, int input, int output){
    //code for compering output
}

void WriteToFile(int fdin, int grade){
    //code for writing to file

}

int writeGrades(int fdin, char* file, int compiledStatus, int input, int correctOut){
    int grade = -1;
    if (compileStatus == 0){
        WriteToFile(fdin, grade);
        return 0;
    }
    else{
        grade = EvaluateCode(file, input, output);
        WriteToFile(fdin, grade);
        return 1;
    }
}

int OpenFile(char* fdin){

    fdin1 = open(fdin,O_RDONLY);
    if (fdin1 < 0){
        return -1;
    }
    else{
        return fdin1;
    }
}

int CompileFile(char* in,int nameLength){
    char out[MAX_NAME] = in;
    strcpy(in+nameLength,".c\0");
    strcpy(out+nameLength,".out\0");
    printf("%s",in);
    printf("\n");
    printf("%s",out);
    printf("\n");
    execlp("gcc","gcc","%s","-o","%s",in,out);
    if((open(out, O_RDONLY)) == -1){
        return 0;
    }
    else{
        close(out);
        in = out;
        return 1;
    }
}

int main(int argc, char** argv) {
    int fdin1, fdin2, fdin3;
    char cFile[MAX_NAME];
    char outFile[MAX_NAME];
    char buffer;
    int nameLength = 0;
    int compilationStatus;

    if (argc != 4){
        printf("Argument count is not correct. terminating...");
        return 0;
    }
    fdin1 = OpenFile(argv[1]);
    if (fdin1 == -1){return 0}
    fdin2 = OpenFile(argv[2]);
    if (fdin2 == -1){return 0}
    fdin3 = OpenFile(argv[3]);
    if (fdin3 == -1){return 0}
    while ((read(fdin1,&buffer,1)) != 0){
        cFile[nameLength] = buffer;
        //found space
        if ((int) buffer == 10){
            compileStatus = CompileFile(cFile,nameLength);
            writeGrades(fdin1, cFile, compileStatus, fdin2, fdin3);
            namelength = -1;
        }
        nameLength++;
    }
}
