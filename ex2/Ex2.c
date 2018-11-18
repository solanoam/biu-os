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
    fd = open("comp.out", O_RDONLY);
    if (fd == -1) {
        ChildId = fork();
        if (ChildId == 0){execlp("gcc", "gcc", "comp.c", "-o", "comp.out", NULL);}
        else {wait(&status);}
    }
    else {close(fd);}
    ChildId = fork();
    if (ChildId == 0){execl("comp.out", "comp.out", "tempOut.txt", correctOut, NULL);}
    else {wait(&status);}
    if (WEXITSTATUS(status) == 2){return 100;}
    else {return 0;}
}

void GetOutput(char *file, char* input){
    int status;
    int fdin = open(input, O_RDONLY);
    if (fdin == -1) {printf("error with open arg[2]"); exit(-1);}
    int fdout = open("tempOut.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IWOTH | S_IROTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
    if (fdout == -1){printf("error with tempOut.txt");exit(-1);}
    pid_t ChildId = fork();
    if (ChildId == 0){
        dup2(fdin, 0);
        dup2(fdout, 1);
        execl(file, NULL);
    }
    else {wait(&status);}
    close(fdin);
    close(fdout);
}

void WriteToFile(int fdin, int grade){
    char enterChar[1];
    enterChar[0] = (char) 10;
    printf("%d\n",grade);
    switch(grade){
        case -1:
            write(fdin,": Compilation error",19);
            write(fdin,enterChar,1);
            break;
        case 0:
            write(fdin,": 0",3);
            //write(fdin,enterChar,1);
            break;
        case 100:
            write(fdin,": 100",5);
            //write(fdin,enterChar,1);
            break;
        default:
            return;
    }
}

int writeGrades(int fdin, char* file, int compilationStatus, char* input, char* correctOut){
    int grade = -1;
    if (compilationStatus == 0){return grade;}
    else{
        GetOutput(file, input);
        grade = CompFiles(correctOut);
        return grade;
    }
}

int CompileFile(char* in,int nameLength){
    int status;
    char out[MAX_NAME];
    strcpy(out, in);
    strcpy(in+nameLength,".c\0");
    strcpy(out+nameLength,".out\0");
    int fd = open("gccLog.txt", O_CREAT | O_WRONLY , S_IWOTH | S_IROTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
    if (fd == -1){printf("error creating logs for gcc"); exit(-1);}
    pid_t ChildId = fork();
    if (ChildId == 0){
        dup2(fd, 1);
        dup2(fd, 2);
        execlp("gcc","gcc",in,"-o",out, NULL);
    }
    else {wait(&status);close(fd);}
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
    char printedName[MAX_NAME];
    char buffer;
    int nameLength = 0;
    int compilationStatus;
    int grade;
    if (argc != 4){
        printf("Argument count is not correct. terminating...");
        return 0;
    }
    fdin1 = open(argv[1],O_RDWR);
    if (fdin1 == -1){return 0;}
    while ((read(fdin1,&buffer, sizeof(buffer))) != 0){
        if ((int) buffer == 10){
            cFile[nameLength] = '\0';
            printf("%s: ",cFile);
            compilationStatus = CompileFile(cFile,nameLength);
            grade = writeGrades(fdin1, cFile, compilationStatus, argv[2], argv[3]);
            if (grade == -1){printf(" Compilation error\n");}
            else {printf("%d\n", grade);}
            nameLength = -1;
        }
        cFile[nameLength] = buffer;
        nameLength++;
    }
}
