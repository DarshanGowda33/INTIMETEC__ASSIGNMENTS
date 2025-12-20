#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scheduler.h"

Scheduler* createScheduler()
{
    Scheduler* s = (Scheduler*)malloc(sizeof(Scheduler));
    s->pcbMap = createHashmap();
    s->readyQueue = createQueue();
    s->waitingQueue = createQueue();
    s->terminatedQueue = createQueue();
    s->runningProcess = NULL;
    s->systemClock = 0;
    s->killEvents = NULL;
    s->killCount = 0;
    return s;
}

void addProcess(Scheduler* scheduler, int pid, const char* name, int burst, int ioStart, int ioDur)
{
    PCB* pcb = createPcb(pid, name, burst, ioStart, ioDur);
    hashmapPut(scheduler->pcbMap, pid, pcb);
    enqueue(scheduler->readyQueue, pcb);
}

void addKillEvent(Scheduler* scheduler, int pid, int time)
{
    scheduler->killEvents = realloc(scheduler->killEvents, sizeof(KillEvent) * (scheduler->killCount + 1));
    scheduler->killEvents[scheduler->killCount++] = (KillEvent){pid, time};
}

static void checkKills(Scheduler* scheduler)
{
    for (int index = 0; index < scheduler->killCount; index++)
    {
        if (scheduler->killEvents[index].killTime == scheduler->systemClock)
        {
            PCB* pcb = hashmapGet(scheduler->pcbMap, scheduler->killEvents[index].pid);
            if (pcb && !pcb->isKilled)
            {
                pcb->isKilled = 1;
                pcb->completionTime = scheduler->systemClock;
                pcb->state = TERMINATED;

                if (scheduler->runningProcess == pcb)
                {
                    scheduler->runningProcess = NULL;
                }
                else
                {
                    removeByPid(scheduler->readyQueue, pcb->pid);
                    removeByPid(scheduler->waitingQueue, pcb->pid);
                }

                enqueue(scheduler->terminatedQueue, pcb);
            }
        }
    }
}

static void updateIo(Scheduler* scheduler)
{
    QueueNode* curr = scheduler->waitingQueue->front;
    while (curr)
    {
        curr->pcb->ioRemaining--;
        curr = curr->next;
    }

    curr = scheduler->waitingQueue->front;
    while (curr)
    {
        PCB* pcb = curr->pcb;
        curr = curr->next;

        if (pcb->ioRemaining == 0)
        {
            removeByPid(scheduler->waitingQueue, pcb->pid);
            pcb->state = READY;
            enqueue(scheduler->readyQueue, pcb);
        }
    }
}

void runScheduler(Scheduler* scheduler)
{
    while (!isEmpty(scheduler->readyQueue) || !isEmpty(scheduler->waitingQueue) || scheduler->runningProcess)
    {

        checkKills(scheduler);
        updateIo(scheduler);

        if (!scheduler->runningProcess && !isEmpty(scheduler->readyQueue))
        {
            scheduler->runningProcess = dequeue(scheduler->readyQueue);
            scheduler->runningProcess->state = RUNNING;
        }

        if (scheduler->runningProcess)
        {
            PCB* p = scheduler->runningProcess;
            p->executionTime++;

            if (p->hasIo && p->executionTime == p->ioStartTime)
            {
                p->ioPending = 1;
            }

            if (p->executionTime == p->burstTime)
            {
                p->completionTime = scheduler->systemClock + 1;
                p->turnaroundTime = p->completionTime;

                p->waitingTime = p->turnaroundTime - p->burstTime;

                p->state = TERMINATED;
                enqueue(scheduler->terminatedQueue, p);
                scheduler->runningProcess = NULL;
            }
            else if (p->ioPending)
            {
                p->ioPending = 0;
                p->ioRemaining = p->ioDuration;
                p->state = WAITING;
                enqueue(scheduler->waitingQueue, p);
                scheduler->runningProcess = NULL;
            }
        }

        sleep(1);
        scheduler->systemClock++;
    }
}

void printResults(Scheduler* scheduler)
{
    printf("\nPID   Name            CPU  IO   Status           Turnaround  Waiting\n");
    printf("---------------------------------------------------------------------\n");

    for (int index = 0; index < HASHMAP_SIZE; index++)
    {
        HashNode* node = scheduler->pcbMap->buckets[index];
        while (node)
        {
            PCB* p = node->pcb;

            if (p->state == TERMINATED)
            {
                if (p->isKilled)
                {
                    printf("%-5d %-15s %-4d %-4d KILLED at %-6d %-11s %-7s\n",
                           p->pid, p->name,
                           p->burstTime, p->ioDuration,
                           p->completionTime, "-", "-");
                }
                else
                {
                    printf("%-5d %-15s %-4d %-4d OK               %-11d %-7d\n",
                           p->pid, p->name,
                           p->burstTime, p->ioDuration,
                           p->turnaroundTime, p->waitingTime);
                }
            }
            node = node->next;
        }
    }
}

void freeScheduler(Scheduler* scheduler)
{
    freeQueue(scheduler->readyQueue);
    freeQueue(scheduler->waitingQueue);
    freeQueue(scheduler->terminatedQueue);
    freeHashmap(scheduler->pcbMap);
    free(scheduler->killEvents);
    free(scheduler);
}
