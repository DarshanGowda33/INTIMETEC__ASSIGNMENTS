#include <stdlib.h>
#include "hashmap.h"

static int hashFunction(int pid)
{
    return pid % HASHMAP_SIZE;
}

HashMap* createHashmap()
{
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    for (int index = 0; index < HASHMAP_SIZE; index++)
    {
        map->buckets[index] = NULL;
    }
    return map;
}

void hashmapPut(HashMap* map, int pid, PCB* pcb)
{
    int idx = hashFunction(pid);
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));

    node->pid = pid;
    node->pcb = pcb;
    node->next = map->buckets[idx];
    map->buckets[idx] = node;
}

PCB* hashmapGet(HashMap* map, int pid)
{
    int idx = hashFunction(pid);
    HashNode* curr = map->buckets[idx];

    while (curr)
    {
        if (curr->pid == pid)
        {
            return curr->pcb;
        }
        curr = curr->next;
    }
    return NULL;
}

void hashmapRemove(HashMap* map, int pid)
{
    int idx = hashFunction(pid);
    HashNode* curr = map->buckets[idx];
    HashNode* prev = NULL;

    while (curr)
    {
        if (curr->pid == pid)
        {
            if (!prev)
            {
                map->buckets[idx] = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void freeHashmap(HashMap* map)
{
    for (int index = 0; index < HASHMAP_SIZE; index++)
    {
        HashNode* curr = map->buckets[index];
        while (curr)
        {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(map);
}
