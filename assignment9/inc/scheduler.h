#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"
#include "hashmap.h"

typedef struct
{
    int pid;
    int killTime;
} KillEvent;

typedef struct
{
    HashMap* pcbMap;
    Queue* readyQueue;
    Queue* waitingQueue;
    Queue* terminatedQueue;

    PCB* runningProcess;

    int systemClock;

    KillEvent* killEvents;
    int killCount;
} Scheduler;

Scheduler* createScheduler();
void addProcess(Scheduler* scheduler, int pid, const char* name, int burstTime, int ioStartTime, int ioDuration);
void addKillEvent(Scheduler* scheduler, int pid, int killTime);
void runScheduler(Scheduler* scheduler);
void printResults(Scheduler* scheduler);
void freeScheduler(Scheduler* scheduler);

#endif
