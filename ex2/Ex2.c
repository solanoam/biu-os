#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAX_NAME 32

int CompFiles(char* const correctOut){
    int status;
    int fd;
    pid_t ChildId;
    fd = OpenFile("comp.out");
    if (fd == -1) {
        ChildId = fork();
        if (ChildId == 0){execlp("gcc", "gcc", "comp.c", "-o", "comp.out", NULL);}
        else {wait(&status);}
    }
    else {close(fd);}
    ChildId = fork();
    if (ChildId == 0){execl("comp.out", "temp.txt", correctOut, NULL);}
    else {wait(&status);}
    if (WEXITSTATUS(status) == 2){return 100;}
    else {return 0;}
}

void GetOutput(char *file, char* input){
    int status;
    pid_t ChildId = fork();
    if (ChildId == 0){execl(file,"<",input," >", "temp.txt", NULL);}
    else {wait(&status);}
}

void WriteToFile(int* fdin, int grade){
    char enterChar[1];
    enterChar[0] = (char) 10;
    switch(grade){
        case -1:
            write(*(fdin),": Compilation error",19);
            write(*(fdin),enterChar,1);
            break;
        case 0:
            write(*(fdin),": 0",3);
            write(*(fdin),enterChar,1);
            break;
        case 100:
            write(*(fdin),": 100",5);
            write(*(fdin),enterChar,1);
            break;
        default:
            return;
    }
}

int writeGrades(int* fdin, char* file, int compilationStatus, char* input, char* correctOut){
    int grade = -1;
    if (compilationStatus == 0){
        WriteToFile(fdin, grade);
        return 0;
    }
    else{
        GetOutput(file, input);
        grade = CompFiles(correctOut);
        WriteToFile(fdin, grade);
        return 1;
    }
}

int OpenFile(char* fileName){
    int fd = open(fileName,O_RDONLY);
    if (fd < 0){return -1;}
    else{return fd;}
}

int CompileFile(char* in,int nameLength){
    int status;
    char out[MAX_NAME];
    strcpy(out, in);
    strcpy(in+nameLength,".c\0");
    strcpy(out+nameLength,".out\0");
    pid_t ChildId = fork();
    if (ChildId == 0){execlp("gcc","gcc",in,"-o",out, NULL);}
    else {wait(&status);}
    int fdNewFile = open(out, O_RDONLY);
    if(fdNewFile == -1){
        return 0;
    }
    else{
        close(fdNewFile);
        strcpy(in,out);
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
    if (fdin1 == -1){return 0;}
    while ((read(fdin1,&buffer,1)) != 0){
        cFile[nameLength] = buffer;
        //found space
        if ((int) buffer == 10){
            compilationStatus = CompileFile(cFile,nameLength);
            writeGrades(&fdin1, cFile, compilationStatus, argv[2], argv[3]);
            nameLength = -1;
        }
        nameLength++;
    }
}
