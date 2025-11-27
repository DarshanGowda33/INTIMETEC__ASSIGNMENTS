#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lruCache.h"

int main()
{
    LruCache *cache = NULL;
    char command[20];

    while (1)
    {
        scanf("%s", command);

        if (strcmp(command, "createCache") == 0)
        {
            int capacity;
            scanf("%d", &capacity);
            cache = createCache(capacity);
        
        }
        else if (strcmp(command, "put") == 0)
        {
            int key;
            char value[VALUE_SIZE];
            scanf("%d %s", &key, value);
            put(cache, key, value);

        }
        else if (strcmp(command, "get") == 0)
        {
            int key;
            scanf("%d", &key);
            printf("%s\n", get(cache, key));

        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
    }

    if (cache)
    {
        freeCache(cache);
    }
    return 0;
}
