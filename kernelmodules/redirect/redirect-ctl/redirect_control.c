#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
//#if 0
#include "redirect_control.h"
//#else
#include "redirect_core.h"
//#endif
#define REDIRECT_MISCDEV_NAME "redirect_control"

struct redirect_info red_info;
static unsigned long user_count = 0;
static LIST_HEAD(redirect_user_slot);

static int redirect_open(struct inode* inode, struct file* file);
static int redirect_release(struct inode* inode, struct file* file);
static int redirect_ioctl(struct inode *inode, struct file *pf, unsigned int cmd, unsigned long arg);

static struct file_operations redirect_fops = {
	.owner = THIS_MODULE,
	.open = redirect_open,
	.release = redirect_release,
	.ioctl = redirect_ioctl,
};

static struct miscdevice  redirect_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = REDIRECT_MISCDEV_NAME,
	.fops = &redirect_fops,	
};
#if 0
void redirect_add_user(const char* user)
{
	struct redirect_user *ruser = NULL;
	list_for_each_entry(ruser, &redirect_user_slot, list) {
		if (!strcmp(ruser->mac_ag, user))
			return;
	}
	ruser = kzalloc(sizeof(struct redirect_user), GFP_KERNEL);
	BUG_ON(!ruser);
	INIT_LIST_HEAD(&ruser->list);
	strcpy(ruser->mac_ag, user);
	list_add_tail(&ruser->list, &redirect_user_slot);
}
#endif
unsigned long get_timeout(void)
{
	return red_info.red_timeout;
}
unsigned char get_tag(void)
{
	return red_info.red_tag;
}
char* get_network_name(void)
{
	return red_info.red_network;
}
char* get_url(void)
{
	return red_info.red_url;
}

static int redirect_open(struct inode* inode, struct file* file)
{
	return 0;
}

static int redirect_release(struct inode* inode, struct file* file)
{
	return 0;
}

static int redirect_ioctl(struct inode *inode, struct file *pf, unsigned int cmd, unsigned long arg)
{
	unsigned long __user *argp = (void __user *)arg;
	switch(cmd)
	{
		case REDIRECT_SET_INFO:
		{
			if(copy_from_user((char*)&red_info, (char*)argp, sizeof(struct redirect_info)))
				return -EINVAL;

			//get_user(redirect_tag, argp);
			printk(KERN_INFO "red_timeout=%ld\n", red_info.red_timeout);
			printk(KERN_INFO "red_tag=%d\n", red_info.red_tag);
			printk(KERN_INFO "red_nerwork=%s\n", red_info.red_network);
			printk(KERN_INFO "red_url=%s\n", red_info.red_url);
			set_redirect_times(red_info.red_timeout);
			set_redirect_tag(red_info.red_tag);
			setup_redirect_url(red_info.red_url);
			break;
		}
		case REDIRECT_GET_USER_COUNT:
		{
			user_count = get_redirect_user_count();
			printk(KERN_INFO "user_count=%d\n", user_count);
			put_user(user_count, argp);
			break;
		}
		case REDIRECT_GET_USER_DATA:
		{
			int i = 0;
			int count = user_count;
			printk(KERN_INFO "count=%d\n", count);

			int size = sizeof(struct redirect_user)* count;
			struct redirect_user * puser= kzalloc(size, GFP_KERNEL);
			get_redirect_user(puser, &count);
			

			printk(KERN_INFO "count=%d\n", count);
			for(i = 0; i < count; i++)
			{
				printk(KERN_INFO "agent=%s\n", puser[i].user_agent);
					
			}
			if(copy_to_user(argp, puser, count * sizeof(struct redirect_user)))
				return -EINVAL;
			kfree(puser);
			break;
		}
	}
	return 0;
}


int redirect_ctl_module_init(void)
{
	int ret = 0;
	ret = misc_register(&redirect_miscdev);
	if(ret)
		printk(KERN_ERR "Loader redirect control modules failed.\n");
	else
		printk(KERN_INFO "Loader redirect control modules success.\n");
		
	return ret;
}

void redirect_ctl_module_exit(void)
{
	misc_deregister(&redirect_miscdev);
}
#if 0
module_init(redirect_ctl_module_init);
module_exit(redirect_ctl_module_exit);
MODULE_LICENSE("GPL");
#endif

