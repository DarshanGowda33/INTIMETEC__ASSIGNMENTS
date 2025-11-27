#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#define VALUE_SIZE 50

typedef struct Node
{
    int key;
    char value[VALUE_SIZE];
    struct Node *prev, *next;
} Node;

typedef struct
{
    int capacity;
    int size;
    Node *head, *tail;
} LruCache;

LruCache* createCache(int capacity);
void put(LruCache *cache, int key, const char *value);
char* get(LruCache *cache, int key);
void freeCache(LruCache *cache);

#endif
