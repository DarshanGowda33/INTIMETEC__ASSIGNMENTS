#include <stdlib.h>
#include "queue.h"

Queue* createQueue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue* queue, PCB* pcb)
{
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->pcb = pcb;
    node->next = NULL;

    if (!queue->rear)
    {
        queue->front = queue->rear = node;
    }
    else
    {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
}

PCB* dequeue(Queue* queue)
{
    if (!queue->front)
    {
        return NULL;
    }
    QueueNode* temp = queue->front;
    PCB* pcb = temp->pcb;

    queue->front = temp->next;
    if (!queue->front)
    {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return pcb;
}

int isEmpty(Queue* queue)
{
    return queue->front == NULL;
}

PCB* removeByPid(Queue* queue, int pid)
{
    QueueNode* curr = queue->front;
    QueueNode* prev = NULL;

    while (curr)
    {
        if (curr->pcb->pid == pid)
        {
            PCB* pcb = curr->pcb;

            if (!prev) 
            {
                queue->front = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }

            if (curr == queue->rear)
            {
                queue->rear = prev;
            }
            free(curr);
            queue->size--;
            return pcb;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

void freeQueue(Queue* queue)
{
    while (!isEmpty(queue))
    {
        dequeue(queue);
    }
    free(queue);
}
