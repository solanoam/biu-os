#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAX_NAME 32 //max name for user

//prints user and grade to CLI
void PrintGrade(char* cFile, int nameLength, int grade){
    cFile[nameLength] = '\0';
    printf("%s: ",cFile);
    if (grade == -1){printf("Compilation error\n");} //grade isn't a number
    else {printf("%d\n", grade);} //print grade
}
//compering files return grade
int CompFiles(char* const correctOut){
    int status;
    int fd;
    pid_t childId;
    fd = open("comp.out", O_RDONLY); //checking if .out already exists
    if (fd == -1) {
        childId = fork(); //forking for exec
        if (childId == 0){execlp("gcc", "gcc", "comp.c", "-o", "comp.out", NULL);} //child
        else {wait(&status);} //father
    }
    else {close(fd);} //already created
    childId = fork(); //forking for exec
    if (childId == 0){execl("comp.out", "comp.out", "tempOut.txt", correctOut, NULL);} //child
    else {wait(&status);} //father
    if (WEXITSTATUS(status) == 2){return 100;} //get return value from comp
    else {return 0;}
}
//getting grades
void GetOutput(char *file, char* input){
    int status;
    int fdin = open(input, O_RDONLY); //opening input
    if (fdin == -1) {printf("error with open arg[2]"); exit(-1);} //handler for open failure
    int fdout = open("tempOut.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IWOTH | S_IROTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR); //creating temp file
    if (fdout == -1){printf("error with tempOut.txt");exit(-1);} //handler for open failure
    pid_t childId = fork(); //forking for exec
    if (childId == 0){ //child
        dup2(fdin, 0);
        dup2(fdout, 1);
        execl(file, NULL);
    }
    else {wait(&status);} //father
    close(fdin);
    close(fdout);
}

//getting grades
int GetGrade(char* file, int compilationStatus, char* input, char* correctOut){
    int grade = -1; //init for failed compilation
    if (compilationStatus == 0){return grade;} //checking status
    else{
        GetOutput(file, input); //getting stdout of .out
        grade = CompFiles(correctOut); //update grade
        return grade;
    }
}
//function for compiling files
int CompileFile(char* in,int nameLength){
    int status;
    char out[MAX_NAME];
    strcpy(out, in); //out file name
    strcpy(in+nameLength,".c\0"); //adding extensions
    strcpy(out+nameLength,".out\0");
    int fd = open("gccLog.txt", O_CREAT | O_WRONLY , S_IWOTH | S_IROTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
    if (fd == -1){printf("error creating logs for gcc"); exit(-1);}
    pid_t childId = fork(); //forking before exec
    if (childId == 0){//child
        dup2(fd, 1);//io redirection
        dup2(fd, 2);
        execlp("gcc","gcc",in,"-o",out, NULL); //get correct file
    }
    else {wait(&status);close(fd);}//father
    int fdNewFile = open(out, O_RDONLY); //checking if .out was created
    if(fdNewFile == -1){return 0;} //file doesn't exist - compile failed
    else{
        close(fdNewFile);
        strcpy(in,out); //update name
        return 1;
    }
}
int main(int argc, char** argv) {
    int fdUsers;
    char cFile[MAX_NAME];
    char printedName[MAX_NAME];
    char buffer;
    int nameLength = 0;
    if (argc != 4){ //terminating if the argc is incorrect
        printf("Argument count is not correct. terminating...");
        return 0;
    }
    fdUsers = open(argv[1],O_RDWR); //opening user list
    if (fdUsers == -1){return 0;} //handler for open failure
    while ((read(fdUsers,&buffer, sizeof(buffer))) != 0){ //looping for all users
        if ((int) buffer == 10){ //space is found
            PrintGrade(cFile,nameLength, (GetGrade(cFile, CompileFile(cFile,nameLength), argv[2], argv[3])));
            nameLength = -1; //init length
        }
        cFile[nameLength] = buffer; //next letter
        nameLength++; //bigger name
    }
}
