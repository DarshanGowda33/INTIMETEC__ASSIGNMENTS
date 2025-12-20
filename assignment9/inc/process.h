#ifndef PROCESS_H
#define PROCESS_H

typedef enum
{
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} ProcessState;

typedef struct PCB
{
    int pid;
    char name[50];

    int burstTime;
    int ioStartTime;
    int ioDuration;

    int executionTime;
    int ioRemaining;

    int waitingTime;
    int turnaroundTime;
    int completionTime;
    int arrivalTime;

    ProcessState state;

    int isKilled;
    int hasIo;

    int ioPending;
} PCB;

PCB* createPcb(int pid, const char* name, int burstTime, int ioStartTime, int ioDuration);
void freePcb(PCB* pcb);

#endif
