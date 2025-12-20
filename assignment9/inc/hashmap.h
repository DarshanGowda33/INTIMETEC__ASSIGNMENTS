#ifndef HASHMAP_H
#define HASHMAP_H

#include "process.h"

#define HASHMAP_SIZE 100

typedef struct HashNode
{
    int pid;
    PCB* pcb;
    struct HashNode* next;
} HashNode;

typedef struct
{
    HashNode* buckets[HASHMAP_SIZE];
} HashMap;

HashMap* createHashmap();
void hashmapPut(HashMap* map, int pid, PCB* pcb);
PCB* hashmapGet(HashMap* map, int pid);
void hashmapRemove(HashMap* map, int pid);
void freeHashmap(HashMap* map);

#endif
