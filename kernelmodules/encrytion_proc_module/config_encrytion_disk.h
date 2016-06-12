#ifndef _CONFIG_ENCRYTION_DISK_H
#define _CONFIG_ENCRYTION_DISK_H
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/rwsem.h>
#include <linux/slab.h>

extern struct list_head encrytion_disk_list;
extern struct rw_semaphore encrytion_disk_sem;

extern int is_encrytion_dist(char *name);


#endif
