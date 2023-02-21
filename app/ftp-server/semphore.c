#include "semphore.h"

int sem_create(key_t key)
{
	union semun sem;
	int semid;
	sem.val = 1;
	semid = semget(key,1,IPC_CREAT | 0666);
	if (semid == -1)
		ERR_EXIT("create semphore");
	semctl(semid,0,SETVAL,sem);
	return semid;
}

int sem_open(key_t key)
{
	int id;
	id = semget(key,0,0);
	if (id == -1)
		ERR_EXIT("open semphore");
	return id;
}

int sem_p(int semid)
{
	int ret = 0;
	struct sembuf sem = {0,-1,/*SEM_UNDO*/0};
	sem.sem_num = 0;
	ret = semop(semid,&sem,1);
	if (ret == -1)
		ERR_EXIT("semphore P operation");
	return ret;
}

int sem_v(int semid)
{
	int ret = 0;
	struct sembuf sem = {0,1,/*SEM_UNDO*/0};
	sem.sem_num = 0;
	ret = semop(semid,&sem,1);
	if (ret == -1)
		ERR_EXIT("semphore V operation");
	return ret;
	
}

int sem_setval(int semid,int val)
{
	union semun sem;
	sem.val = val;
	int ret = semctl(semid,0,SETVAL,sem);
	if (ret == -1)
		ERR_EXIT("setvalue");
	return ret;
}
	
void sem_del(int semid)
{
	semctl(semid,0,IPC_RMID,0);
}

int sem_getval(int semid)
{
	int ret = semctl(semid,0,GETVAL,0);
	if (ret == -1)
		ERR_EXIT("semctl");
	return ret;
}
