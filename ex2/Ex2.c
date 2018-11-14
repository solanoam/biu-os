#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAX_NAME 32

int CompFiles(int input){
    if ((OpenFile("comp.out")) == -1) {
        execlp("gcc", "gcc", "comp.c", "-o", "comp.out");
    }

}

void GetOutput(char *file, char* input){
    execlp("%s","%s","<","%s"," >", "temp.out", file, file, input);
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
        case 1:
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
        grade =
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
    char out[MAX_NAME];
    strcpy(out, in);
    strcpy(in+nameLength,".c\0");
    strcpy(out+nameLength,".out\0");
//    printf("%s",in);
//    printf("\n");
//    printf("%s",out);
//    printf("\n");
    execlp("gcc","gcc","%s","-o","%s",in,out);
    int fdNewFile = open(out, O_RDONLY) == -1;
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
//    fdin2 = OpenFile(argv[2]);
//    if (fdin2 == -1){close(fdin1); return 0;}
//    fdin3 = OpenFile(argv[3]);
//    if (fdin3 == -1){close(fdin1); close(fdin2); return 0;}
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
