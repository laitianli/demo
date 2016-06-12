#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include "hlist.h"
#define HLIST_PROC_NAME "hlist_demo"


#define HASH_BIT 5
struct hlist_head_ex *g_hlist_head = NULL;
struct proc_dir_entry *g_hlist_proc = NULL;

struct session_data {
	unsigned long session;
};

unsigned long get_hash_key(void)
{
	return jiffies;
}

void free_session_data(struct hash_node* data)
{
	if(data) {
		kfree(data->data);
		kfree(data);
	}
}

void init_session(void)
{
	int i = 0;
	struct hash_node* hdata = NULL;
	struct session_data *sdata = NULL;
	for(i = 0; i < 10; i++) {
		hdata = kzalloc(sizeof(struct hash_node), GFP_KERNEL);
		if(!hdata) {
			printk(KERN_ERR"kzalloc hash_node failed.\n");
			return ;
		}
		hdata->key = get_hash_key();
		sdata= kzalloc(sizeof(struct session_data), GFP_KERNEL);
		sdata->session = i+100;
		hdata->data = (void*)sdata;
		
		printk(KERN_INFO "[ key:%ld value: %ld]\n", hdata->key, sdata->session);
		
		add_hash_list(g_hlist_head, hdata);
	}
}
char tmpbuf[4096]  = {0};
int data_len = 0;
void get_hlist_data(struct hash_node* d)
{
	data_len += sprintf(tmpbuf + data_len, "%ld\t", 
		((struct session_data*)d->data)->session);
}

int read_hlist_proc(char *page, char **start, off_t off,
			  int count, int *eof, void *data)
{
	memset(tmpbuf, 0, sizeof(tmpbuf));
	data_len = 0;
	traverse_hash_list(g_hlist_head, get_hlist_data);
	data_len += sprintf(tmpbuf + data_len, "\n");
	strncpy(page, tmpbuf, data_len);
	data_len -= off;
	*start = page + off;
	return data_len;
}
int write_hlist_proc(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	unsigned long session = 0;
	struct hash_node* hdata = NULL;
	struct session_data *sdata = NULL;
	char tmpbuffer[20] = {0};
	if(copy_from_user(tmpbuffer, buffer, count-1))
		return -EINVAL;
	
	if (tmpbuffer[0] == '-' && 'd' == tmpbuffer[1]) {
		unsigned long key = 0;
		sscanf(&tmpbuffer[3], "%ld", &key);

		hdata = find_hash_list(g_hlist_head, key);
		if(hdata) {
			del_hash_list(g_hlist_head, hdata);
			kfree(hdata->data);
			kfree(hdata);
		}		
		goto END;
	}
	sscanf(tmpbuffer, "%ld", &session);
	hdata = kzalloc(sizeof(struct hash_node), GFP_KERNEL);
	if(!hdata) {
		printk(KERN_ERR"kzalloc hash_node failed.\n");
		return -1;
	}
	hdata->key = get_hash_key();
	printk(KERN_INFO "[ key:%ld value: %ld]\n", hdata->key, session);
	sdata= kzalloc(sizeof(struct session_data), GFP_KERNEL);
	sdata->session = session;
	hdata->data = (void*)sdata;

	add_hash_list(g_hlist_head, hdata);
END:	
	return count;
}

void init_hlist_proc(void)
{
	g_hlist_proc = create_proc_entry(HLIST_PROC_NAME, 0744, NULL);
	if(!g_hlist_proc) {
		printk(KERN_ERR"create proc file \"%s\" failed.\n", HLIST_PROC_NAME);
		return ;
	}
	g_hlist_proc->read_proc = read_hlist_proc;
	g_hlist_proc->write_proc = write_hlist_proc;
}

void release_hlist_proc(void)
{
	remove_proc_entry(HLIST_PROC_NAME, NULL);
}

static int __init hlist_demo_module_init(void)
{
	g_hlist_head = init_hash_list(HASH_BIT);
	init_session();
	init_hlist_proc();	
	return 0;
}

static void __exit hlist_demo_module_exit(void)
{
	release_hash_list(g_hlist_head, free_session_data);
	release_hlist_proc();
}
module_init(hlist_demo_module_init);
module_exit(hlist_demo_module_exit);
MODULE_LICENSE("GPL");


