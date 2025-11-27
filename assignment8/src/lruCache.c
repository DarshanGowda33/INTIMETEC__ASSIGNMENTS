#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lruCache.h"

static void moveToFront(LruCache *cache, Node *node)
{
    if (cache->head == node)
    {
        return;
    }
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    if (node->next)
    {
        node->next->prev = node->prev;
    }
    if (cache->tail == node)
    {
        cache->tail = node->prev;
    }
    node->prev = NULL;
    node->next = cache->head;
    cache->head->prev = node;
    cache->head = node;
}

LruCache* createCache(int capacity)
{
    LruCache *cache = (LruCache*)malloc(sizeof(LruCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;
    return cache;
}

char* get(LruCache *cache, int key)
{
    Node *temp = cache->head;
    while (temp)
    {
        if (temp->key == key)
        {
            moveToFront(cache, temp);
            return temp->value;
        }
        temp = temp->next;
    }
    return "NULL";
}

void put(LruCache *cache, int key, const char *value)
{
    Node *temp = cache->head;

    while (temp)
    {
        if (temp->key == key)
        {
            strcpy(temp->value, value);
            moveToFront(cache, temp);
            return;
        }
        temp = temp->next;
    }

    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = NULL;
    newNode->next = cache->head;

    if (cache->head)
    {
        cache->head->prev = newNode;
    }
    cache->head = newNode;

    if (!cache->tail)
    {
        cache->tail = newNode;
    }
    cache->size++;

    if (cache->size > cache->capacity)
    {
        Node *delete = cache->tail;
        cache->tail = delete->prev;
        cache->tail->next = NULL;
        free(delete);
        cache->size--;
    }
}

void freeCache(LruCache *cache)
{
    Node *temp = cache->head;
    while (temp)
    {
        Node *next = temp->next;
        free(temp);
        temp = next;
    }
    free(cache);
}
