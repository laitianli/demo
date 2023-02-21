#include "ftpfun.h"
#include "hashtable.h"
#include "semphore.h"
#ifndef _CONSER_H_
#define _CONSER_H_
typedef struct _info_
{
	int upsum;
	int downsum;
	off_t upsize;
	off_t downsize;
	int concount;
	pid_t ctlpid;
	pid_t datapid;
}INFO;

typedef struct _pid_ip_
{
	int pid;
	char ip[16];
}PIDIP;

/*create share memary*/
int create_shm(char* path,int t,size_t size);

INFO* get_memary(int shmid);

PIDIP* get_memary_pidip(int shmid);

void init_shm_pidip(PIDIP* pidip);

void init_shm(INFO* p_info);

void set_upsum(INFO* p_info,int semid,int sum);

void set_downsum(INFO* p_info,int semid,int sum);

void set_upsize(INFO* p_info,int semid,off_t size);

void set_downsize(INFO* p_info,int semid,off_t size);

void set_concount(INFO* p_info,int semid,int count,int tag);
/*********************************/

int get_upsum(INFO* p_info,int semid);

int get_downsum(INFO* p_info,int semid);

off_t get_upsize(INFO* p_info,int semid);

off_t get_downsize(INFO* p_info,int semid);

int get_concount(INFO* p_info,int semid);

void del_shm(int shmid);

#endif
