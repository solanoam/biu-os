/**************************************
 * Author: Noam Solan
 * ID: 204484703
 * Course: 83-381-01 (Operating Systems)
 * Practice: 83-381-02 (Yoram)
 **************************************/
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROC_ALLOWED 32
#define MAX_TIME_ARRIVE 4096

struct Proccess{
    int pid;
    int timeOfArrivel;
    int timeBurst;
    int waitTime;
    int timeLeft;
    int lastStartTime;
    int finished;
};

void printWaitTime(struct Proccess inputProc[], int numOfProcs){
    int i;
    for (i = 0; i < numOfProcs; i++){
        printf("Process %d waiting time: %d\n", inputProc[i].pid, inputProc[i].waitTime);
    }
}

void fcfsSim(struct Proccess inputProc[], int numOfProcs){
    int currentProc = 0;
    int timePassed = 0;
    int procsLeft = numOfProcs;
    while (procsLeft != 0){
        int min = MAX_TIME_ARRIVE;
        int i;
        for (i = 0; i < numOfProcs; i++){
            if (!(inputProc[i].finished)){
                if (inputProc[i].timeOfArrivel < min){
                    currentProc = i;
                    min = inputProc[i].timeOfArrivel;
                }
            }
        }
        inputProc[currentProc].finished = 1;
        inputProc[currentProc].timeLeft = 0;
        inputProc[currentProc].waitTime += timePassed-inputProc[currentProc].timeOfArrivel;
        printf("#%d:[%d]", inputProc[currentProc].pid, timePassed);
        timePassed += inputProc[currentProc].timeBurst;
        printf("-[%d]\n", timePassed);
        procsLeft--;
        min = MAX_TIME_ARRIVE;
    }
}
/*
void rrSim(struct Proccess inputProc[], int numOfProcs, int frame){
    int currentProc = 0;
    int oldProc = 0;
    int lastProc = 0;
    int timePassed = 0;
    int procsLeft = numOfProcs;
    int currentJob = 0;
    while (procsLeft) {
        int i;
        int count = 0;
        int chosen = 0;
        for (i = currentProc + 1; count < numOfProcs; i = i % numOfProcs) {
            if (i == numOfProcs) { i = 0; }
            if (!inputProc[i].finished && !(currentJob % frame) && !(chosen)) {
                if (inputProc[i].timeOfArrivel <= timePassed + currentJob) {
                    oldProc = currentProc;
                    currentProc = i;
                    chosen = 1;
                }
            }
            if (i == numOfProcs - 1 && !(currentJob % frame) && (oldProc != currentProc)) {
                if (currentJob != 0) {
                    printf("#%d:[%d]-[%d]\n", inputProc[oldProc].pid, timePassed, timePassed + currentJob);
                    timePassed += currentJob;
                    currentJob = 0;
                }
            }
            i++;
            count++;
        }
        if (!(currentJob % frame) && chosen) {
            inputProc[currentProc].timeLeft--;
            currentJob++;
        }
        else {
            timePassed++;
        }
        if (!inputProc[currentProc].timeLeft) {
            printf("#%d:[%d]-[%d]\n", inputProc[currentProc].pid, timePassed, timePassed + currentJob);
            timePassed += currentJob;
            currentJob = 0;
            procsLeft--;
            inputProc[currentProc].finished = 1;
            inputProc[currentProc].waitTime = timePassed - inputProc[currentProc].timeOfArrivel - inputProc[currentProc].timeBurst;
            int i;
            for (i = 0; i < numOfProcs; i++){
                if (!(inputProc[i].finished)) {currentProc = i;}
            }
        }
    }
}*/
void printSim(int queue[], int time, int len){
    printf("time of print is : %d\n", time);
    for (int i = 0; i < len; i++){
        printf("%d ", queue[i]+1);
    }
    printf("\n");
}

void decQueue(int queue[], int queueLen){
    for (int i=1; i < queueLen; i++){
        queue[i-1] = queue[i];
    }
}

void rrSim(struct Proccess inputProc[], int numOfProcs, int frame){
    int queue[MAX_PROC_ALLOWED];
    for (int p = 0; p < MAX_PROC_ALLOWED; p++){
        queue[p] = 545655;
    }
    int currentProc = 0;
    int timePassed = 0;
    int procsLeft = numOfProcs;
    struct Proccess oldProc = inputProc[0];
    int queueLen = 0;
    int timeFrameLeft = 0;
    while (procsLeft != 0) {
        timeFrameLeft = 0;
        int i;
        int z;
        int count = 0;
        for (i = currentProc; count < numOfProcs; count++) {
            if (!(inputProc[i].finished) && inputProc[i].timeOfArrivel <= timePassed && currentProc != i) {
                for (z = 0; z < queueLen; z++) {
                    if (i == queue[z]) {
                        break;
                    }
                }
                if (z == queueLen){
                    queue[queueLen] = i;
                    queueLen++;
                }
            }
            i++;
            i = i % numOfProcs;
        }
        if (!(inputProc[currentProc].finished) && inputProc[currentProc].timeOfArrivel <= timePassed){
            queue[queueLen] = i;
            queueLen++;
        }
        //printSim(queue, timePassed, queueLen);
        currentProc = queue[0];
        if (queueLen) {
            if (oldProc.pid != inputProc[currentProc].pid && !(inputProc[currentProc].finished) && (queueLen)) {
                if ((timePassed != 0)) {
                    printf("#%d:[%d]-[%d]\n", oldProc.pid, oldProc.lastStartTime, timePassed);
                }
                inputProc[currentProc].lastStartTime = timePassed;
                oldProc = inputProc[currentProc];
            }
            if (!(inputProc[currentProc].finished)) {
                inputProc[currentProc].timeLeft -= frame;
                if (inputProc[currentProc].timeLeft <= 0) {
                    inputProc[currentProc].finished = 1;
                    procsLeft--;
                    timeFrameLeft = inputProc[currentProc].timeLeft;
                    inputProc[currentProc].waitTime =
                            timePassed + frame + timeFrameLeft - inputProc[currentProc].timeOfArrivel -
                            inputProc[currentProc].timeBurst;
                }
            }
            timePassed += frame + timeFrameLeft;
            decQueue(queue, queueLen);
            queueLen--;

        }
        else {
            timePassed++;
        }
    }

    printf("#%d:[%d]-[%d]\n", oldProc.pid, oldProc.lastStartTime, timePassed);
}

void sjfSim(struct Proccess inputProc[], int numOfProcs){
    int currentProc = 0;
    int oldProc = 0;
    int timePassed = 0;
    int procsLeft = numOfProcs;
    int currentJob = 0;
    while (procsLeft) {
        int i;
        for (i = 0; i < numOfProcs; i++) {
            if (!inputProc[i].finished && !(currentJob % 1)){
                if (inputProc[i].timeOfArrivel <= timePassed+currentJob && inputProc[currentProc].timeLeft >= inputProc[i].timeLeft){
                    oldProc = currentProc;
                    currentProc = i;
                }
            }
            if (i == numOfProcs - 1 && !(currentJob % 1) && (oldProc != currentProc)){
                if (currentJob != 0) {
                    printf("#%d:[%d]-[%d]\n", inputProc[oldProc].pid, timePassed, timePassed + currentJob);
                    timePassed += currentJob;
                    currentJob = 0;
                }
            }
        }
        inputProc[currentProc].timeLeft--;
        currentJob++;
        if (!inputProc[currentProc].timeLeft) {
            printf("#%d:[%d]-[%d]\n", inputProc[currentProc].pid, timePassed, timePassed + currentJob);
            timePassed += currentJob;
            currentJob = 0;
            procsLeft--;
            inputProc[currentProc].finished = 1;
            inputProc[currentProc].waitTime = timePassed - inputProc[currentProc].timeOfArrivel - inputProc[currentProc].timeBurst;
            int i;
            for (i = 0; i < numOfProcs; i++){
                if (!(inputProc[i].finished)) {currentProc = i;}
            }
        }
    }
}

void startSim(int usrChioceSim, struct Proccess inputProc[], int numOfProcs){
    int frame;
    switch (usrChioceSim){
        case 1:
            fcfsSim(inputProc, numOfProcs);
            break;
        case 2:
            scanf("%d", &frame);
            rrSim(inputProc, numOfProcs, frame);
            break;
        case 3:
            sjfSim(inputProc, numOfProcs);
            break;
        default:
            exit(0);
    }
    printWaitTime(inputProc, numOfProcs);
}

int getProccess(struct Proccess* inputProc){
    int numOfProcs = 0;
    scanf("%d", &numOfProcs);
    int i;
    for (i = 0; i != numOfProcs; i++){
        inputProc[i].pid = i+1;
        inputProc[i].finished = 0;
        inputProc[i].waitTime = 0;
        inputProc[i].lastStartTime = 0;
        scanf("%d", &(inputProc[i].timeOfArrivel));
        scanf("%d", &(inputProc[i].timeBurst));
        inputProc[i].timeLeft = inputProc[i].timeBurst;
    }
    return numOfProcs;
}

int getSimType(){
    int usrChoice = 0;
    scanf("%d",&usrChoice);
    return usrChoice;
}

int main() {
    struct Proccess inputProc[MAX_PROC_ALLOWED];
    int numOfProcs = getProccess(inputProc);
    startSim(getSimType(), inputProc, numOfProcs);
    return 0;
}

