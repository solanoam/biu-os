/**************************************
 * Author: Noam Solan
 * ID: 204484703
 * Course: 83-381-01 (Operating Systems)
 * Practice: 83-381-02 (Yoram)
 **************************************/
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROC_ALLOWED 32

struct Proccess{
    int pid;
    int timeOfArrivel;
    int timeBurst;
    int waitTime;
    int timeLeft;
    int finished;
};

void printWaitTime(struct Proccess inputProc[], int numOfProcs){
    for (int i = 0; i < numOfProcs; i++){
        printf("Process %d waiting time: %d", inputProc[i].pid, inputProc[i].waitTime);
    }
}

void fcfsSim(struct Proccess inputProc[], int numOfProcs){
    int currentProc = 0;
    int timePassed = 0;
    int procsLeft = numOfProcs;
    printf("Welcome to FCFS simulation!\n");
    while (procsLeft != 0){
        int min;
        for (int i = 0; i % numOfProcs; i++){
            if (!inputProc[i].finished){
                if (inputProc[i].timeOfArrivel < min){
                    currentProc = i;
                    min = inputProc[i].timeOfArrivel;
                }
            }
        }
        inputProc[currentProc].finished = 1;
        inputProc[currentProc].timeLeft = 0;
        inputProc[currentProc].waitTime =+ timePassed;
        printf("#%d:[%d]", inputProc[currentProc].pid, timePassed);
        timePassed =+ inputProc[currentProc].timeBurst;
        printf("-[%d]\n", timePassed);
        procsLeft--;
    }
}

void rrSim(struct Proccess inputProc[], int numOfProcs){
    printf("Welcome to RR simulation!\n");
}

void sjfSim(struct Proccess inputProc[], int numOfProcs){
    printf("Welcome to SJF simulation!\n");
}

void startSim(int usrChioceSim, struct Proccess inputProc[], int numOfProcs){
    switch (usrChioceSim){
        case 1:
            fcfsSim(inputProc, numOfProcs);
            break;
        case 2:
            rrSim(inputProc, numOfProcs);
            break;
        case 3:
            sjfSim(inputProc, numOfProcs);
            break;
        defult:
            printf("simulation do not exsist. terminating...");
            exit(0);
    }
    printWaitTime(inputProc, numOfProcs);
}

int getProccess(struct Proccess inputProc[]){
    int numOfProcs = 0;
    printf("Please insert the number of process that will be collected");
    scanf("%d", &numOfProcs);
    for (int i = 0; i != numOfProcs; i--){
        inputProc[i].pid == i+1;
        inputProc[i].finished = 0;
        inputProc[i].waitTime = 0;
        printf("Please insert time of arrival:\n");
        scanf("%d", &(inputProc[i].timeOfArrivel));
        printf("Please insert time burst:\n");
        scanf("%d", &(inputProc[i].timeBurst));
        inputProc[i].timeLeft = inputProc[i].timeBurst;
    }
    return numOfProcs;
}

int getSimType(){
    int usrChoice = 0;
    printf("Please choose a simulations:\n");
    printf("for FCFS press 1\n");
    printf("for RR press 2\n");
    printf("for SJF press 3\n");
    scanf("%d",&usrChoice);
    return usrChoice;
}

int main() {
    struct Proccess inputProc[MAX_PROC_ALLOWED];
    int numOfProcs = getProccess(inputProc);
    startSim(getSimType(), inputProc, numOfProcs);
    return 0;
}

