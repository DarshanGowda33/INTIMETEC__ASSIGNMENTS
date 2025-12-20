#include <stdlib.h>
#include <string.h>
#include "process.h"

PCB* createPcb(int pid, const char* name, int burstTime, int ioStartTime, int ioDuration)
{
    PCB* pcb = (PCB*)malloc(sizeof(PCB));

    pcb->pid = pid;
    strncpy(pcb->name, name, 49);
    pcb->name[49] = '\0';

    pcb->burstTime = burstTime;
    pcb->ioStartTime = ioStartTime;
    pcb->ioDuration = ioDuration;

    pcb->executionTime = 0;
    pcb->ioRemaining = 0;

    pcb->waitingTime = 0;
    pcb->turnaroundTime = 0;
    pcb->completionTime = 0;
    pcb->arrivalTime = 0;

    pcb->state = READY;

    pcb->isKilled = 0;
    pcb->hasIo = (ioStartTime > 0 && ioDuration > 0);
    pcb->ioPending = 0;

    return pcb;
}

void freePcb(PCB* pcb)
{
    if (pcb)
    {
        free(pcb);
    }
}
