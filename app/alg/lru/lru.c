#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lru.h"


struct lru* create_lru(int capacity)
{
    struct lru* lru = malloc(sizeof(struct lru));
    if (!lru) {
        printf("[error] malloc lru failed!\n");
        return NULL;
    }
    lru->capacity = capacity;
    lru->head = NULL;
    return lru;
}

int get(struct lru* lru, int key)
{
    return 0;
}

int put(struct lru* lru, int key, int val)
{
    if (!lru) {
        return 0;
    }

#if 0    
    struct double_list* head = NULL;
    lru->buckets[key].key = key;
    head = lru->buckets[key].head;
    return add_node(&head, val);
#else
    return 1;
#endif    
}


