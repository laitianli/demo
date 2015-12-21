#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include "cp.h"


extern void begin_cp_task(void);
extern void end_cp_task(void);

static enum cp_task_state g_old_state = CP_TASK_INIT;
struct cp_info g_info = {0};
static int cp_info_open(struct inode* inode,struct file* fp);
static int cp_info_close(struct inode* inode,struct file* fp);
static ssize_t cp_info_read(struct file* fp,char __user* buf,size_t len,loff_t *pos);
static ssize_t cp_info_write(struct file* fp,const char __user* buf,size_t len,loff_t *pos);
static unsigned int cp_info_poll(struct file* fp,struct poll_table_struct* p);
static long cp_info_ioctl(struct file* fp,unsigned int cmd,unsigned long arg);

static struct proc_dir_entry *gp_proc_entry = NULL;

static DEFINE_SPINLOCK(g_state_lock);
static void set_state(enum cp_task_state state)
{
	unsigned long flags;
	spin_lock_irqsave(&g_state_lock,flags);
	g_info.user_info.state = state;
	spin_unlock_irqrestore(&g_state_lock,flags);
}

static enum cp_task_state get_state()
{
	enum cp_task_state state;
	unsigned long flags;
	spin_lock_irqsave(&g_state_lock,flags);
	state = g_info.user_info.state;
	spin_unlock_irqrestore(&g_state_lock,flags);
	return state;
}

static struct file_operations fops = {
	.owner  = THIS_MODULE,
	.open	=cp_info_open,
	.release=cp_info_close,
	.read	=cp_info_read,
	.write	=cp_info_write,
	.poll	=cp_info_poll,
	.unlocked_ioctl = cp_info_ioctl,
};

static int cp_info_open(struct inode* inode,struct file* fp)
{
	Log("open,g_info=%p",&g_info);
	fp->private_data = &g_info;
	return 0;
}
static int cp_info_close(struct inode* inode,struct file* fp)
{
	Log("close");
	fp->private_data = NULL;
	return 0;
}
/*读取cp任务发生变化时的状态信息*/
static ssize_t cp_info_read(struct file* fp,char __user* buf,size_t len,loff_t *pos)
{
	ssize_t ret = 0;	
	struct cp_info *pinfo = fp->private_data;
	Log("read,pinfo=%p",pinfo);
	if(copy_to_user(buf,pinfo,sizeof(struct cp_info)))
	{
		Log("[Error]=copy_to_user failed.");
		ret = -EFAULT;
	}
	else
		ret = sizeof(struct cp_info);
	return ret;
}
/*通知开启cp任务*/
static ssize_t cp_info_write(struct file* fp,const char __user* buf,size_t len,loff_t *pos)
{
	struct cp_info *pinfo = fp->private_data;
	char *tmpbuf = kmalloc(len,GFP_KERNEL);
	int ret = 0;
	Log("write, pinfo=%p",pinfo);
	if(!tmpbuf)
	{
		Log("[Error]=malloc memry failed.");
		ret = -ENOMEM;
	}
	if(copy_from_user(tmpbuf,buf,len))
	{
		Log("[Error]=copy_from_user failed.");
		kfree(tmpbuf);
		ret = -EFAULT;
		goto ERROR;
	}
	Log("len=%d,action:%s",len,tmpbuf);
	if(!strncmp(tmpbuf,"start",strlen("start")))
	{/*开启读任务*/
		set_state(CP_TASK_RUNNING);
	}
	else if(!strncmp(tmpbuf,"stop",strlen("stop")))
	{ /*停止读任务*/
		set_state(CP_TASK_STOP);
		
	}
	else if(!strncmp(tmpbuf,"restart",strlen("restart")))
	{
		
	}
	kfree(tmpbuf);
	return len;
ERROR:
	return ret;
}
static unsigned int cp_info_poll(struct file* fp,struct poll_table_struct* p)
{
	struct cp_info *pinfo = fp->private_data;
	unsigned int mask = 0;
	enum cp_task_state new_state = get_state();
	if (g_old_state != new_state)/*可读取*/
	{
		g_old_state = new_state;
		Log("new_state=%d",new_state);
		mask |= POLLIN | POLLRDNORM;
	}
	if (g_old_state == new_state) /*可写入*/
		mask |= POLLOUT | POLLWRNORM;
	return mask;
}

static long cp_info_ioctl(struct file* fp,unsigned int cmd,unsigned long arg)
{
	int ret = 0;
	struct cp_info *pinfo = fp->private_data;
	switch(cmd)
	{
	case SET_USERSPACE_BUF:
		if(copy_from_user(&pinfo->user_buf,arg,sizeof(struct cp_info_user_buf)))
		{
			ret = -EFAULT;
			Log("[Error]=copy_from_user failed.");
		} 
		break;
	default:
		Log("[Error]=cmd=%u,arg=%u",cmd,arg);
		ret = -EINVAL;
		break;
	}
	return ret;
}


int create_cp_info_proc(void)
{
	int ret = -1;
	//spin_lock_init(&g_info.st.lock);
	gp_proc_entry = create_proc_entry(PROC_NAME,0755,NULL);
	if(gp_proc_entry)
	{
		gp_proc_entry->proc_fops = &fops;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18)		
		gp_proc_entry->owner	 = THIS_MODULE;
#endif		
		ret = 0;
	}
	Log("ret=%d",ret);
	return ret;
}

void remove_cp_info_proc(void)
{
	remove_proc_entry(PROC_NAME,NULL);
}
