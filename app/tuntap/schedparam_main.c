#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>


static void set_thread_schedparam(void)
{
    struct sched_param param;
    int prioIncrement = 1;
    pid_t pid = getpid();
    param.sched_priority = sched_get_priority_min(SCHED_FIFO) + prioIncrement;
    param.sched_priority = 1;//sched_get_priority_min(SCHED_FIFO) + prioIncrement;
    sched_setscheduler(pid, SCHED_FIFO, &param);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    //pthread_setschedparam(pthread_self(), SCHED_RR, &param);
}

void* new_task_func(void* arg)
{
	set_thread_schedparam();
	while (1) {
		;
//		sleep(1);
	}
	return NULL;
}

void create_new_task(void)
{
	int cpu = 10;
	cpu_set_t cpumask;
	pthread_attr_t attr;
	CPU_ZERO(&cpumask);
	CPU_SET(cpu, &cpumask);

	pthread_attr_init(&attr);
	pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpumask);
	pthread_t tid;
	int ret = pthread_create(&tid, &attr, new_task_func, NULL);
	if (ret) {
		printf("[error] pthread_create failed!\n");
	}
	printf("[Note] create new task success.\n");
	pthread_join(tid, NULL);
}

int main(int argc, char** argv)
{
	create_new_task();
	return 0;
}
