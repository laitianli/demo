#ifndef _CP_H_
#define _CP_H_
#include <linux/ioctl.h>
enum cp_task_state {
	CP_TASK_INIT,
	CP_TASK_RUNNING,
	CP_TASK_SUCCESS,
	CP_TASK_STOP,
	CP_TASK_ERROR,
	CP_TASK_OVER,
};

struct cp_info_user {
	enum cp_task_state	state;
	unsigned long long	task_time;
	unsigned long long	totle_size;
	unsigned long long	cp_size;
	unsigned long long	remain_size;
	char				src_file[256];
	char				dst_file[256];
};
struct cp_info_user_buf{
	unsigned long __user buf;
	unsigned int  len;
};
#define CP_MAGIC 'C'
#define SET_USERSPACE_BUF _IOW(CP_MAGIC,1,struct cp_info_user_buf)

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/spinlock.h>

#define PROC_NAME "cp-info"
struct cp_state {
	spinlock_t			lock;
	enum cp_task_state	state;
};

struct cp_info {
	struct cp_info_user 		user_info;
	struct cp_info_user_buf     user_buf;
};


#endif
#endif

