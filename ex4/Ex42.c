/**************************************
 * Author: Noam Solan
 * ID: 204484703
 * Course: 83-381-01 (Operating Systems)
 * Practice: 83-381-02 (Yoram)
 **************************************/
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#define HORI 0
#define VERT 1
#define RIGHT_BUTTON 'd'
#define LEFT_BUTTON 'a'
#define DOWN_BUTTON 's'
#define SWAP_BUTTON 'w'

//globals
int posX = 9;
int posY = 0;
int blockState = HORI;

//
void fallingBlock(int sig);

// validation for new x position
int validatorX(int currPosX, int currBlockState){
    if (currPosX == 16 && currBlockState == HORI){return posX;}
    if (currPosX == 1 && currBlockState == HORI){return posX;}
    if (currPosX == 17 && currBlockState == VERT){return posX;}
    if (currPosX == 0 && currBlockState == VERT){return posX;}
    return currPosX;
}
// validation for new y position
int validatorY(int currPosY, int currentBlockState){
    if (currPosY == 20 && currentBlockState == HORI){return 0;}
    if (currPosY == 19 && currentBlockState == VERT){return 0;}
    return currPosY;
}
// validation for new block state
int validatorFlip(int currPosX, int currPosY, int currentBlockState){
    if (currPosY == 17 && currentBlockState == VERT){return blockState;}
    if (currPosX >= 16 && currentBlockState == HORI){return blockState;}
    if (currPosX == 1 && currentBlockState == HORI){return blockState;}
    return currentBlockState;
}
//handling new position and state for the block
void newPosHandler(int currPosX, int currPosY, int currBlockState){
    posX = validatorX(currPosX, currBlockState);
    posY = validatorY(currPosY, currBlockState);
    blockState = validatorFlip(currPosX, currPosY, currBlockState);
}
//print the floor for the board
void printFloor(){
    printf("********************\n");
}
//print the block if the block is vertical
void printLineBlockVert(int posX){
    printf("*");
    int i;
    for (i = 0; i < 18; i++) {
        if (i == posX){ //only when reachig the block draw it
            printf("-");
        }
        else{
            printf(" ");
        }
    }
    printf("*\n");
}
//print regular line without block
void printLine(){
    printf("*                  *\n");
}
//print the block if the block is vertical
void printLineBlockHori(int posX){
    printf("*");
    int i;
    for (i = 0; i < 18; i++) {
        if (i == posX -1){ //only when reachig the block draw it
            printf("---");
            i += 2; //we have printed 3 chars
        }
        else{
            printf(" ");
        }
    }
    printf("*\n");
}
//handle printing the entire board
void printBoard(int currPosX, int currPosY, int currBlockState){
    int i;
    system("clear"); //clear the screen
    for (i = 0; i < 20; i++){
        switch (currBlockState){
            case HORI: //call horizotal function if block is horizontal
                if (currPosY != i){
                    printLine();
                }
                else { //line where the block is at
                    printLineBlockHori(currPosX);
                }
                break;
            case VERT: //call vertical function if block is vertical
                if (i == currPosY -1 || i == currPosY || i == currPosY + 1){ //line where the block is at
                    printLineBlockVert(currPosX);
                }
                else {
                    printLine();
                }
                break;    
        }
    }
    printFloor();
}
//reest the alarm once it's goes off
void resetAlarm(){
    signal(SIGALRM,fallingBlock); 
    alarm(1);
}

void keyHandler(int sig){
    char key;
    read(0,&key,sizeof(char)); //getting char in pipe
    switch (key){ //for each key, evaluate next step. handler will set the a new value if needed
        case LEFT_BUTTON:
            newPosHandler(posX - 1, posY, blockState);
            break;
        case DOWN_BUTTON:
            newPosHandler(posX, posY + 1, blockState);
            break;
        case RIGHT_BUTTON:
            newPosHandler(posX + 1, posY, blockState);
            break;
        case SWAP_BUTTON:
            switch (blockState){
                case HORI:
                    newPosHandler(posX, posY, VERT);
                    break;
                case VERT:
                    newPosHandler(posX,posY,HORI);
                    break;
            }
            break;
        default:
            break;
    }
    signal(SIGUSR2,keyHandler); //reseting listening to the signal
}
//moving the block down if the alarm went off
void fallingBlock(int sig){ 
    newPosHandler(posX, posY + 1, blockState);
    resetAlarm();
}

int main(){
    printBoard(posX, posY, blockState); //print board
    signal(SIGUSR2,keyHandler); //listen to signal from father
    resetAlarm(); //reset the alarm
    while(true) {
        printBoard(posX, posY, blockState); //print board
        pause(); //wait to signal - only function that run now are handlers and printboard each signal
    }
}
