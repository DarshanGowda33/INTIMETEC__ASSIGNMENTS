#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

typedef struct QueueNode
{
    PCB* pcb;
    struct QueueNode* next;
} QueueNode;

typedef struct
{
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

Queue* createQueue();
void enqueue(Queue* queue, PCB* pcb);
PCB* dequeue(Queue* queue);
int isEmpty(Queue* queue);
PCB* removeByPid(Queue* queue, int pid);
void freeQueue(Queue* queue);

#endif
