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
    int timeFinish;
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

void decQueue(int queue[], int queueLen){
    int i;
    for (i=1; i < queueLen; i++){
        queue[i-1] = queue[i];
    }
}

void rrSim(struct Proccess inputProc[], int numOfProcs, int frame){
    int queue[MAX_PROC_ALLOWED];
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
        oldProc = currentProc;
        int i;
        for (i = 0; i < numOfProcs; i++) {
            if (!(inputProc[i].finished)){
                if (inputProc[i].timeOfArrivel <= timePassed+currentJob &&
                    inputProc[currentProc].timeLeft > inputProc[i].timeLeft ||
                    inputProc[currentProc].finished){
                    currentProc = i;
                }
            }
        }
        if (oldProc != currentProc){
            if (inputProc[oldProc].timeFinish != 0){
                printf("#%d:[%d]-[%d]\n", inputProc[oldProc].pid, inputProc[oldProc].lastStartTime, inputProc[oldProc].timeFinish);
            }
            else {
                printf("#%d:[%d]-[%d]\n", inputProc[oldProc].pid, inputProc[oldProc].lastStartTime, timePassed + currentJob);
            }
            timePassed += currentJob;
            inputProc[currentProc].lastStartTime = timePassed;
            currentJob = 0;
        }
        inputProc[currentProc].timeLeft--;
        currentJob++;
        if (inputProc[currentProc].timeLeft == 0) {
            if (!(inputProc[currentProc].finished)) {
                timePassed += currentJob;
                currentJob = 0;
                procsLeft--;
                inputProc[currentProc].finished = 1;
                inputProc[currentProc].timeFinish = timePassed;
                inputProc[currentProc].waitTime = timePassed - inputProc[currentProc].timeOfArrivel - inputProc[currentProc].timeBurst;
            }
        }
    }
    printf("#%d:[%d]-[%d]\n", inputProc[oldProc].pid, inputProc[oldProc].lastStartTime, timePassed);
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
        inputProc[i].timeFinish = 0;
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

/**************
* Guy Salman
* ID: 308491091
* Course: 83-381-01 (Operating Systems)
* Practice: 83-381-02
**************/
#include <stdio.h>
#include <stdlib.h>
#define Max_Array 32
struct Process {
    int   pid;
    int   time_arrival;
    int   burst_time;
    int   wait_time;
    int   finished;
    int   lastTimeStart;
    int   finishTime;
} typedef Process;

void printWait(int num_of_processes, Process processes[]){
    int i;
    for (i = 0; i < num_of_processes; i++){
        printf("Process %d waiting time: %d\n", i+1, processes[i].wait_time);
    }
}

void FCFS(int num_of_processes, Process* processes)
{
    int time_passed = 0;
    int i;
    int j, k;
    for (k = 0; k<num_of_processes; k++)
    {
        processes[k].wait_time = 0;
    }
    for (i = 0; i<num_of_processes; i++)
    {
        if (processes[i].time_arrival>time_passed)
        {
            time_passed = processes[i].time_arrival;
        }
        processes[i].wait_time = time_passed - processes[i].time_arrival;
        printf("#%d:[%d]-[%d] \n", i + 1, time_passed, processes[i].burst_time + processes[i].time_arrival);
        time_passed = time_passed + processes[i].burst_time;
    }
    for (j = 0; j<num_of_processes; j++)
    {
        printf("Process %d waiting time: %d \n", j + 1, processes[j].wait_time);
    }

}

void RR(int time_split, int num_of_processes, Process* processes)
{
    int count, remain, i, time = 0;
    int remain_time[Max_Array];
    int flag = 0;
    int check = 0;
    int num_of_finish = 0;
    remain = num_of_processes;
    for (i = 0; i<num_of_processes; i++)
    {
        remain_time[i] = processes[i].burst_time;
        processes[i].finished = 0;
        processes[i].wait_time = 0;
    }
    if (num_of_processes == 1)
    {
        printf("#%d: [%d]-[%d]\n", num_of_processes, time, time + remain_time[0]);
        check = 1;
    }

    while (check != 1)
    {
        i = 0;
        for (i = 0; i<num_of_processes; i++)
        {
            if (processes[i].time_arrival <= time)
            {
                if (remain_time[i] <= time_split && remain_time[i]>0 && processes[i].finished == 0)
                {
                    processes[i].wait_time = time - processes[i].time_arrival;
                    printf("#%d: [%d]-[%d]\n", i + 1, time, time + remain_time[i]);
                    time = time + remain_time[i];
                    processes[i].finished = 1;
                    remain_time[i] = 0;
                    num_of_finish++;
                }
                else if (remain_time[i]>time_split && processes[i].finished == 0)
                {
                    remain_time[i] = remain_time[i] - time_split;
                    printf("#%d: [%d]-[%d]\n", i + 1, time, time + time_split);
                    time = time + time_split;
                }
            }
            else
            {
                //time++;
                continue;
            }

        }
        if (num_of_finish == num_of_processes)
        {
            check = 1; // return until everyone is finished;
        }
    }
}

void SJF(int num_of_processes, Process* processes)
{
    int i, j, time = 0;
    int remain_time[Max_Array];
    int check = 0;
    int currentJob = 0;
    int prevJob = 0;
    int num_of_finish = 0;

    for (i = 0; i<num_of_processes; i++)
    {
        remain_time[i] = processes[i].burst_time;
        processes[i].finished = 0;
        processes[i].wait_time = 0;
    }
    while (check != 1)
    {
        for (i = 0; i<num_of_processes; i++)
        {
            if (processes[i].time_arrival <= time && processes[i].finished == 0)
            {
                if (remain_time[i]<remain_time[currentJob])
                {
                    currentJob = i;
                    processes[currentJob].lastTimeStart = time;
                }
            }
        }

        remain_time[currentJob]--;
        if (currentJob != prevJob) // switching jobs
        {
            if (processes[prevJob].finishTime != 0){
                printf("#%d: [%d]-[%d]\n", prevJob + 1, processes[prevJob].lastTimeStart, processes[prevJob].finishTime);
            }
            else{
                printf("#%d: [%d]-[%d]\n", prevJob + 1, processes[prevJob].lastTimeStart, time);
            }
            prevJob = currentJob;
        }
        if (remain_time[currentJob] == 0)
        {
            processes[currentJob].finished = 1;
            processes[currentJob].finishTime = time+1;
            processes[currentJob].wait_time = time - processes[currentJob].time_arrival - processes[currentJob].burst_time + 1;
            remain_time[currentJob] = 999999999;
            num_of_finish++;

        }/**/
        if (num_of_finish == num_of_processes)
        {
            check = 1;
        }
        time++;
    }
    printf("#%d: [%d]-[%d]\n", prevJob + 1, processes[prevJob].lastTimeStart, time);
}