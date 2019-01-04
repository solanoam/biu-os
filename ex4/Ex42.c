//
// Created by Noam Solan on 26/12/2018.
//
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#define HORI 0
#define VERT 1

//globals
int posX = 9;
int posY = 0;
int blockState = HORI;

void fallingBlock(int sig);


int validatorX(int currPosX, int currBlockState){
    if (currPosX == 17 && currBlockState == HORI){return posX;}
    if (currPosX == 1 && currBlockState == HORI){return posX;}
    if (currPosX == 19 && currBlockState == VERT){return posX;}
    if (currPosX == 1 && currBlockState == VERT){return posX;}
    return currPosX;
}

int validatorY(int currPosY, int currentBlockState){
    if (currPosY == 20 && currentBlockState == HORI){return 0;}
    if (currPosY == 19 && currentBlockState == VERT){return 0;}
    return currPosY;
}
int validatorFlip(int currPosX, int currPosY, int currentBlockState){
    if (currPosY == 19 && currentBlockState == HORI){return blockState;}
    if (currPosX == 19 && currentBlockState == VERT){return blockState;}
    if (currPosX == 1 && currentBlockState == VERT){return blockState;}
    return currentBlockState;
}

void newPosHandler(int currPosX, int currPosY, int currBlockState){
    posX = validatorX(currPosX, currBlockState);
    posY = validatorY(currPosY, currBlockState);
    blockState = validatorFlip(currPosX, currPosY, currBlockState);
}

void printFloor(){
    printf("********************\n");
}

void printLineBlockVert(int posX){
    printf("*");
    int i;
    for (i = 0; i < 18; i++) {
        if (i == posX){
            printf("|");
        }
        else{
            printf(" ");
        }
    }
    printf("*\n");
}

void printLine(){
    printf("*                  *\n");
}

void printLineBlockHori(int posX){
    printf("*");
    int i;
    for (i = 0; i < 18; i++) {
        if (i == posX -1){
            printf("---");
            i += 2;
        }
        else{
            printf(" ");
        }
    }
    printf("*\n");
}

void printBoard(int currPosX, int currPosY, int currBlockState){
    int i;
    system("clear");
    for (i = 0; i < 20; i++){
        if (currBlockState == HORI){
            if (currPosY != i){
                printLine();
            }
            else {
                printLineBlockHori(currPosX);
            }
        }
        else {
            if (i == currPosY -1 || i == currPosY || i == currPosY + 1){
                printLineBlockVert(currPosX);
            }
            else {
                printLine();
            }
        }
    }
    printFloor();
}

void resetAlarm(){
    signal(SIGALRM,fallingBlock);
    alarm(1);
}

void keyHandler(int sig){
    //TODO handling read error
    char key;
    read(0,&key,sizeof(char));
    switch (key){
        case 'a':
            newPosHandler(posX - 1, posY, blockState);
            break;
        case 's':
            newPosHandler(posX, posY + 1, blockState);
            break;
        case 'd':
            newPosHandler(posX + 1, posY, blockState);
            break;
        case 'w':
            if (blockState == HORI){newPosHandler(posX, posY, VERT);}
            if (blockState == VERT){newPosHandler(posX, posY, HORI);}
            break;
        default:
            break;
    }
    signal(SIGUSR2,keyHandler);
}

void fallingBlock(int sig){
    newPosHandler(posX, posY + 1, blockState);
    resetAlarm();
}

int main(){
    printBoard(posX, posY, blockState);
    signal(SIGUSR2,keyHandler);
    resetAlarm();
    while(true) {
        printBoard(posX, posY, blockState);
        pause();
    }
}
