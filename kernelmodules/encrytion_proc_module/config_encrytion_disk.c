/*
 * Scatterlist Cryptographic API.
 *
 * Procfs information.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 */
#include <linux/init.h>
#include <linux/list.h>
#include <linux/rwsem.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include "config_encrytion_disk.h"

#define ENCRYTION_MAX_NAME 64

LIST_HEAD(encrytion_disk_list);
DECLARE_RWSEM(encrytion_disk_sem);

struct encrytion_disk {
	struct list_head list;
	char name[ENCRYTION_MAX_NAME];
};

static void *c_start(struct seq_file *m, loff_t *pos)
{
	struct list_head *v;
	loff_t n = *pos;

	down_read(&encrytion_disk_sem);
	list_for_each(v, &encrytion_disk_list)
		if (!n--)
			return list_entry(v, struct encrytion_disk, list);
	return NULL;
}

static void *c_next(struct seq_file *m, void *p, loff_t *pos)
{
	struct list_head *v = p;
	
	(*pos)++;
	v = v->next;
	return (v == &encrytion_disk_list) ?
		NULL : list_entry(v, struct encrytion_disk, list);
}

static void c_stop(struct seq_file *m, void *p)
{
	up_read(&encrytion_disk_sem);
}

static int c_show(struct seq_file *m, void *p)
{
	struct encrytion_disk *disk = (struct encrytion_disk *)p;
	
	seq_printf(m, "name         : %s", disk->name);
	//seq_printf(m, "is_encrytion_dist         : %d\n", is_encrytion_dist(disk->name));
	//seq_printf(m, "is_encrytion_dist         : %d\n", is_encrytion_dist("aaaa"));
	return 0;
}

static struct seq_operations encrytion_disk_seq_ops = {
	.start		= c_start,
	.next		= c_next,
	.stop		= c_stop,
	.show		= c_show
};

static int encrytion_disk_info_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &encrytion_disk_seq_ops);
}

static ssize_t encrytion_disk_info_write(struct file *file, const char __user *buf,
				    size_t size, loff_t *_pos)
{
	void *kbuf;
	char *name;
	long ret;

	/* start by dragging the command into memory */
	if (size <= 1 || size >= PAGE_SIZE || size > ENCRYTION_MAX_NAME-1)
		return -EINVAL;

	kbuf = kmalloc(sizeof(struct encrytion_disk), GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	ret = -EFAULT;
	name = (char *)(((struct encrytion_disk *)kbuf)->name);
	if ( copy_from_user((void *)name, buf, size) != 0)
		goto done;
	name[size-1] = 0;
	/*add to list*/
	down_write(&encrytion_disk_sem);
	list_add_tail(&(((struct encrytion_disk *)kbuf)->list), &encrytion_disk_list);
	up_write(&encrytion_disk_sem);
	ret = size;
	return (ssize_t)ret;
done:
	kfree(kbuf);
	return (ssize_t)ret;
}

 int is_encrytion_dist(char *name)
{
	struct list_head *v;
	struct encrytion_disk *disk;

	down_read(&encrytion_disk_sem);
	list_for_each(v, &encrytion_disk_list)
	{
		disk = list_entry(v, struct encrytion_disk, list);
		printk("<0>, disk->name:%s, name: %s\n", disk->name, name);
		if(!strcmp(disk->name, name))
		{
			up_read(&encrytion_disk_sem);
			return 1;
		}
	}
	up_read(&encrytion_disk_sem);
	return 0;
}
        
static struct file_operations proc_encrytion_disk_ops = {
	.open		= encrytion_disk_info_open,
	.read		= seq_read,
	.write	= encrytion_disk_info_write,
	.llseek		= seq_lseek,
	.release	= seq_release
};

static int __init encrytion_disk_init_proc(void)
{
	struct proc_dir_entry *proc;
	int ret;
	ret = 0;
	proc = create_proc_entry("test_encrytion_disk_name", 0, NULL);
	if (proc)
	{
		proc->proc_fops = &proc_encrytion_disk_ops;
		ret =1;
	}
	INIT_LIST_HEAD(&encrytion_disk_list);
	return ret;
}

static void __exit encrytion_disk_destroy_proc(void)
{
	//struct list_head *v;
	struct encrytion_disk *disk, *tmp_disk;
	int ret = is_encrytion_dist("sdb");
	printk("<0>, ret=%d\n", ret);
	down_write(&encrytion_disk_sem);
	list_for_each_entry_safe(disk, tmp_disk, &encrytion_disk_list, list)
	{
		//disk = list_entry(v, struct encrytion_disk, list);
		//if(disk != NULL)
		//{
			list_del(&disk->list);
			kfree(disk);
		//}
	}
	up_write(&encrytion_disk_sem);
	remove_proc_entry("encrytion_disk_name", NULL);
	
}


module_init(encrytion_disk_init_proc);
module_exit(encrytion_disk_destroy_proc);

MODULE_AUTHOR("xiaosan.li <lixiaoshan_18899@163.com>");
MODULE_DESCRIPTION("encrytion disk for kernel 2.6.32");
MODULE_LICENSE("GPL");
