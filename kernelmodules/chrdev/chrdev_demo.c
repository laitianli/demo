#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/sched.h>

#define CHRDEV_DEMO_NAME "chrdev-demo"
#define CHRDEV_DEMO_COUNT 2
#define CHRDEV_DEMO_BUF_SIZE (8 << 10)
static DECLARE_WAIT_QUEUE_HEAD(g_wait_queue);
struct chrdev_data
{
	char	buf[CHRDEV_DEMO_BUF_SIZE];
	int		cur_pos;
	int		count;
};
static struct chardev_data* g_pdata;
static dev_t g_dev;
static struct cdev g_chrdev;

static int chrdev_demo_open(struct inode* i, struct file* fp);
static ssize_t chrdev_demo_read(struct file* fp, char __user *buf, size_t len, loff_t* pos);
static ssize_t chrdev_demo_write(struct file* fp, const char __user *data, size_t len, loff_t* pos);
static int chrdev_demo_release(struct inode* i, struct file* fp);
static struct file_operations g_fops = {
	.owner = THIS_MODULE,
	.open = chrdev_demo_open,
	.read = chrdev_demo_read,
	.write = chrdev_demo_write,
	.release = chrdev_demo_release,
};
static int chrdev_demo_open(struct inode* i, struct file* fp)
{
	printk("[Info]==[%s:%d]=\n", __func__, __LINE__);
	fp->private_data = g_pdata;
	return 0;
}
static ssize_t chrdev_demo_read(struct file* fp, char __user *buf, size_t len, loff_t* pos)
{
	struct chrdev_data *pdata = fp->private_data;

	printk("[Note]=[%s]=len=%d\n", __func__, len);
	wait_event_interruptible(g_wait_queue, pdata->cur_pos > 0);
	if(len > pdata->count)
	{
		len = pdata->count;
		pdata->count = 0;
	}
	pdata->cur_pos -= len;
	if(pdata->cur_pos<=0)
		pdata->cur_pos = 0;
	if (copy_to_user(buf, pdata->buf + pdata->cur_pos, len))
	{
		printk("[Error]=[%s]=copy_to_user failed.\n", __func__);
		return -EFAULT;
	}

	return len;

}
static ssize_t chrdev_demo_write(struct file* fp, const char __user *data, size_t len, loff_t* pos)
{
	struct chrdev_data* pdata = fp->private_data;

	if(pdata->cur_pos + len > CHRDEV_DEMO_BUF_SIZE)
		len = CHRDEV_DEMO_BUF_SIZE - pdata->cur_pos;

	if (copy_from_user(pdata->buf + pdata->cur_pos, data, len))
	{
		printk("[Error]=[%s]=copy_from_user failed.\n", __func__);
		return -EFAULT;
	}
	pdata->cur_pos += len;

	pdata->count += len;
	wake_up(&g_wait_queue);
	return len;
}
static int chrdev_demo_release(struct inode* i, struct file* fp)
{
	printk("[Info]==[%s:%d]=\n", __func__, __LINE__);
	return 0;
}

static int init_chrdev_data(void)
{
	g_pdata = kmalloc(sizeof(struct chrdev_data), GFP_KERNEL);
	if(!g_pdata)
	{
		printk("[error]=kmalloc failed.\n");
		return -ENOMEM; 
	}
	memset(g_pdata, 0 ,sizeof(struct chrdev_data));
}

static int __init chrdev_demo_init(void)
{
	int retval = 0;

	init_chrdev_data();
	retval = alloc_chrdev_region(&g_dev, 0, CHRDEV_DEMO_COUNT, CHRDEV_DEMO_NAME);

	printk("major=%d, minor=%d\n", MAJOR(g_dev), MINOR(g_dev));
	cdev_init(&g_chrdev, &g_fops);
	cdev_add(&g_chrdev, g_dev, CHRDEV_DEMO_COUNT);
    return 0;
}

static void __exit chrdev_demo_exit(void)
{
	cdev_del(&g_chrdev);
	unregister_chrdev_region(g_dev, CHRDEV_DEMO_COUNT);
}

module_init(chrdev_demo_init);
module_exit(chrdev_demo_exit);
MODULE_LICENSE("GPL");

