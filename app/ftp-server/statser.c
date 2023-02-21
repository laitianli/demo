#include "conf.h"
#include "statser.h"
#include <string.h>
#include <stdlib.h>
/*create share memary*/
int create_shm(char* path,int t,size_t size)
{
	key_t key = ftok(path,t);
	int shmid = shmget(key,size,IPC_CREAT | 0666);
	if (shmid == -1)
		ERR_EXIT("Create share memary");
	return shmid;
}

INFO* get_memary(int shmid)
{
	INFO* info = shmat(shmid,NULL,0);
		if (info == NULL)
			ERR_EXIT("get share memary");
	return info;
}
PIDIP* get_memary_pidip(int shmid)
{
	PIDIP* pidip = shmat(shmid,NULL,0);
	if(info == NULL)
		ERR_EXIT("get share memary");
	return pidip;
}
void init_shm_pidip(PIDIP* pidip)
{
	pidip -> pid = 0;
	memset(pidip -> ip,0,16);
}
void init_shm(INFO* p_info)
{
	p_info -> upsum = 0;
	p_info -> upsize = 0;
	p_info -> downsum = 0;
	p_info -> downsize = 0;
	p_info -> concount = 0;
}
void set_upsum(INFO* p_info,int semid,int sum)
{
	sem_p(semid);
	p_info -> upsum += sum;	
	sem_v(semid);
}

void set_downsum(INFO* p_info,int semid,int sum)
{
	sem_p(semid);
	p_info -> downsum += sum;
	sem_v(semid);
}

void set_upsize(INFO* p_info,int semid,off_t size)
{
	sem_p(semid);
	p_info -> upsize += size;
	sem_v(semid);
}

void set_downsize(INFO* p_info,int semid,off_t size)
{
	sem_p(semid);
	p_info -> downsize += size;
	sem_v(semid);
}

void set_concount(INFO* p_info,int semid,int count,int tag)
{
	sem_p(semid);
	if (tag == 1)
		p_info -> concount += count;
	else if (tag == -1)
		p_info -> concount -= count;
	sem_v(semid);
}
/*********************************/

int get_upsum(INFO* p_info,int semid)
{
	int ret = 0;
	sem_p(semid);
	ret = p_info -> upsum;
	sem_v(semid);
	return ret;
}

int get_downsum(INFO* p_info,int semid)
{
	int ret = 0;
	sem_p(semid);
	ret = p_info -> downsum;
	sem_v(semid);
	return ret;
}
off_t get_upsize(INFO* p_info,int semid)
{
	off_t ret = 0;
	sem_p(semid);
	ret = p_info -> upsize;
	sem_v(semid);
	return ret;
}

off_t get_downsize(INFO* p_info,int semid)
{
	off_t ret = 0;
	sem_p(semid);
	ret = p_info -> downsize;
	sem_v(semid);
	return ret;
}
int get_concount(INFO* p_info,int semid)
{	
	int ret = 0;
	sem_p(semid);
	ret = p_info -> concount;
	sem_v(semid);
	return ret;
}
void del_shm(int shmid)
{
	shmctl(shmid,IPC_RMID,0);
}
