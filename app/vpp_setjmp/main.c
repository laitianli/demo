#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include "longjmp.h"
 
#define always_inline static inline __attribute__ ((__always_inline__))
 
#define RETURN_SETJMP_MAGIC 0x0930    
#define RETURN_LONGJMP_MAGIC 0x0931
 
#define RESUME_SETJMP_MAGIC 0x1112
#define RESUME_LONGJMP_MAGIC 0x1113
 
typedef double f64;
 
typedef uword (*worker_func_t)(uword func_arg);
 
struct process_context {
	f64 interval;
	clib_longjmp_t return_longjmp;
 	/* Where to longjmp to resume node after suspend. */
	clib_longjmp_t resume_longjmp;
	int process_index;
	void *stack;
	int stack_size;
	worker_func_t function;
	f64 wait_left;
	f64 wakeup_time;
	const char *name;
};
 
uword oam_process(uword arg);
 
uword dhcp_process(uword arg);
 
struct function_register {
	worker_func_t function;
	const char *name;
};
 
struct function_register worker_funcs[] = {
	{oam_process, "oam_process"},
	{dhcp_process, "dhcp_process"},
};
 
#define WORKER_NUM  (sizeof(worker_funcs) / sizeof(struct function_register))
 
struct process_context workers[WORKER_NUM];
 
always_inline int vlib_process_suspend_time_is_zero(f64 dt);
 
always_inline int vlib_process_suspend_time_is_zero(f64 dt)
{
	return dt < 10e-6;
}
 
int init_workers(void)
{
	int i;
 
	memset(workers, 0, sizeof(workers));
 
	for (i = 0; i < WORKER_NUM; ++i) {
		workers[i].interval = (i + 1);
		workers[i].process_index = i;
		workers[i].stack_size = (1 << 16);
		workers[i].stack = malloc(sizeof(char) * workers[i].stack_size);
	    if (workers[i].stack == NULL) {
			printf("alloc stack for process failed!\n");
			return -1;
		}
		memset(workers[i].stack, 0, sizeof(char) * workers[i].stack_size);
		workers[i].function = worker_funcs[i].function;
		workers[i].name = worker_funcs[i].name;
	}
 
	return 0;
}
 
always_inline f64 vlib_time_now(void)
{
    struct timespec ts;
 
    clock_gettime(CLOCK_MONOTONIC, &ts);
 
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}
 
always_inline f64 get_now(void)
{
	struct timeval t;
 
    gettimeofday(&t, NULL);
 
   	return (f64)t.tv_sec + (f64)t.tv_usec / 1000000.0;
}
 
always_inline f64 vlib_process_wait_for_event_or_clock(
        struct process_context *worker, f64 dt);
 
always_inline f64 vlib_process_wait_for_event_or_clock(
        struct process_context *worker, f64 dt)
{
    f64 wakeup_time;
	int r;
 
	if (vlib_process_suspend_time_is_zero(dt))
		return dt; 
 
	wakeup_time = get_now() + dt;
	r = clib_setjmp(&worker->resume_longjmp, RESUME_SETJMP_MAGIC);
	if (r == RESUME_SETJMP_MAGIC) {
		worker->wakeup_time = wakeup_time;
		worker->wait_left = dt;
		clib_longjmp(&worker->return_longjmp, RETURN_LONGJMP_MAGIC);
	}
	assert(worker->wakeup_time == wakeup_time);
 
	return wakeup_time - get_now();
}
 
int ts_to_str(uint64_t ts, char *buf, int max_len)
{
    time_t t = (time_t)ts;
    const char *format = "%Y-%m-%d %H:%M:%S";
    struct tm lt;
    int ret = 0;
 
    (void) localtime_r(&t, &lt);
    if ((ret = strftime(buf, max_len, format, &lt)) == 0) {
        return sprintf(buf, "unknown");
    }
 
    return ret;
}
 
uword oam_process(uword arg)
{
	f64 now, dt;
	int cnt = 0;
	char time_str[32];
	struct process_context *worker;
 
	worker = (struct process_context *)arg;
	dt = worker->interval;
	while (1) {
		dt = vlib_process_wait_for_event_or_clock(worker, dt);
		worker->wait_left = dt;
		if (!vlib_process_suspend_time_is_zero(dt))
			continue;
 
		now = get_now();
		ts_to_str(now, time_str, sizeof(time_str));
		printf("[%s][%s] worker %d: begin to recv packet ...\n", __func__,
				time_str, worker->process_index);
 
		/* recv one packet */
 
		/* handle this packet */
		
		now = get_now();
		ts_to_str(now, time_str, sizeof(time_str));
		printf("[%s][%s] worker %d: handle %d-th packet done!\n", __func__,
				time_str, worker->process_index, cnt);
 
		dt = worker->interval;
		cnt++;
	}
 
	return 0;
}
 
uword dhcp_process(uword arg)
{
	f64 now, dt;
	int cnt = 0;
	char time_str[32];
	struct process_context *worker;
 
	worker = (struct process_context *)arg;
	dt = worker->interval;
	while (1) {
		dt = vlib_process_wait_for_event_or_clock(worker, dt);
		worker->wait_left = dt;
		if (!vlib_process_suspend_time_is_zero(dt))
			continue;
 
		now = get_now();
		ts_to_str(now, time_str, sizeof(time_str));
		printf("[%s][%s] worker %d: begin to recv packet ...\n", __func__,
				time_str, worker->process_index);
 
		/* recv one packet */
 
		/* handle this packet */
		
		now = get_now();
		ts_to_str(now, time_str, sizeof(time_str));
		printf("[%s][%s] worker %d: handle %d-th packet done!\n", __func__,
				time_str, worker->process_index, cnt);
 
		dt = worker->interval;
		cnt++;
	}
 
	return 0;
}
 
int resume_process(struct process_context *worker)
{
	int r;
 
	r = clib_setjmp(&worker->return_longjmp, RETURN_SETJMP_MAGIC);
	if (r == RETURN_SETJMP_MAGIC) {
		clib_longjmp(&worker->resume_longjmp, RESUME_LONGJMP_MAGIC);
	}
 
	return 0;
}
 
int start_process(struct process_context *worker)
{
	int r;
 
	r = clib_setjmp(&worker->return_longjmp, RETURN_SETJMP_MAGIC); 
	if (r == RETURN_SETJMP_MAGIC) {
		r = clib_calljmp(worker->function, (uword)worker,
				worker->stack + worker->stack_size);
	}
 
	return r;
}
 
int main(void)
{
	int i;
	
	if (init_workers() < 0) {
		printf("init workers failed!\n");
		return -1;
	}
 
	for (i = 0; i < WORKER_NUM; ++i) {
		start_process(&workers[i]);
	}
 
	while (1) {
		for (i = 0; i < WORKER_NUM; ++i) {
			resume_process(&workers[i]);
		}
        /* do something */
	}
 
	return 0;
}