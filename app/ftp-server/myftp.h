#include "ftpfun.h"
#include "statser.h"
#include "semphore.h"
#include "hashtable.h"

#ifndef _MAIN_H_
#define _MAIN_H_

typedef struct _str_arry_
{
	char cmd[10];
	char arg[100];
}STR_ARRAY;

int check_command(char* cmd);

int set_value(STR_ARRAY ary);

void handle(int sig);

void pipe_handle(int sig);

void del_handle(int sig);

void pidip_handle(int sig, siginfo_t *info, void *buf);

void pid_handle(int sig, siginfo_t *info, void *buf);

pid_t set_process(int nochdir,int noclose);
#endif
