
#include "ftpfun.h"

#ifndef _SEMPHORE_
#define _SEMPHORE_

union semun {
	int val;                  
	struct semid_ds *buf;     
	unsigned short *array;    
    struct seminfo *__buf;  
};

int sem_create(key_t key);

int sem_open(key_t key);

int sem_p(int semid);

int sem_v(int semid);

int sem_setval(int semid,int val);
	
int sem_getval(int semid);

void sem_del(int semid);

int sem_getval(int semid);
#endif
