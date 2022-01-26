#ifndef __LRU_H__
#define __LRU_H__
#include "double_list.h"

struct lru {
    int capacity;
//struct hash* hash;
    struct double_list* head;
};

struct lru* create_lru(int capacity);

int get(struct lru* lru, int key);

int put(struct lru* lru, int key, int val);

#endif


