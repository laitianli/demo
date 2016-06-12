#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/list.h>
#define MISC_eqm_NAME  "eqm-encryption-demo"

static wait_queue_head_t eqm_qh[2]; /*[0]: 读取; [1]: 写入*/
static atomic_t be_eqm_read;
static atomic_t be_eqm_write;

static int eqm_open(struct inode* inode, struct file* file);
static int eqm_release(struct inode* inode, struct file* file);
static ssize_t eqm_read(struct file* pf, char __user* buf, size_t len, loff_t* off);
static ssize_t eqm_write(struct file *pf, const char __user* buf, size_t len, loff_t *off);
static unsigned int eqm_poll(struct file* pf, struct poll_table_struct* table);
static int eqm_mmap(struct file* pf, struct vm_area_struct* vma);

static struct file_operations eqm_fops = {
	.owner = THIS_MODULE,
	.open = eqm_open, 
	.release = eqm_release,
	.read = eqm_read,
	.write = eqm_write, 
	.poll = eqm_poll,
	.mmap =  eqm_mmap,
 };

static struct miscdevice eqm_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_eqm_NAME, 
	.fops = &eqm_fops,
};



#define DEBUG_CODE 2
#ifdef DEBUG_CODE
struct page_data{
	struct page** ppage;
	unsigned int w_current_page;
	unsigned int w_page_offset;
	unsigned int r_current_page;
	unsigned int r_page_offset;
	unsigned int page_sum;
	int (*alloc)(struct page_data* pdata, int count);
	int (*free)(struct page_data* pdata);
	int (*set_data)(struct page_data* pdata, const unsigned char* buf, size_t size);
	int (*get_data)(struct page_data* pdata, unsigned char* buf, size_t size);
};
static int alloc_page_data(struct page_data* pdata, int count)
{
	int i = 0;
	if (!pdata || pdata->ppage)
		return -EINVAL;
	pdata->ppage = kmalloc(sizeof(struct page*) * count, GFP_KERNEL);
	for (i = 0; i < count && pdata->ppage; i++) {
		
		pdata->ppage[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		get_page(pdata->ppage[i]);
		
	}
	pdata->page_sum = count;
	pdata->w_current_page = 0;
	pdata->w_page_offset = 0;
	pdata->r_current_page = 0;
	pdata->r_page_offset = 0;
	
	return pdata->page_sum;
	
}
static int free_page_data(struct page_data* pdata)
{
	int i = 0;
	if(!pdata || !pdata->ppage)
		return -EINVAL;
	for (i = 0; i < pdata->page_sum; i++)
		put_page(pdata->ppage[i]);
	kfree(pdata->ppage);
	pdata->ppage = NULL;
	pdata->page_sum = 0;
	pdata->w_current_page = 0;
	pdata->w_page_offset = 0;
	pdata->r_current_page = 0;
	pdata->r_page_offset = 0;
	return 0;
}		

int set_page_data(struct page_data* pdata, const unsigned char* buf, size_t size)
{
	int i = 0;
	char *page_buf = NULL;
	for(i = 0; i < pdata->page_sum; i++)
	{
		page_buf = page_address(pdata->ppage[i]);
		memset(page_buf, 'a'+i, PAGE_SIZE);
	}
	return 0;
}

int get_page_data(struct page_data* pdata, unsigned char* buf, size_t size)
{
	char *page_buf = NULL;
	if(!pdata || pdata->ppage == NULL)
		return -1;
	
	size = size & ~(PAGE_SIZE -1);
	if (size > 0 && size % PAGE_SIZE == 0) {
		int i = 0;
		for(i = 0; i < size / PAGE_SIZE; i++) {
			page_buf = page_address(pdata->ppage[i]);
			memcpy(buf+i*PAGE_SIZE, page_buf, PAGE_SIZE);
		}
	}

	
	return size;	
}

struct page_data gpd ={
		.alloc = alloc_page_data,
		.free = free_page_data,
		.set_data = set_page_data,
		.get_data = get_page_data,
	}; 
#endif /*DEBUG_CODE*/

static LIST_HEAD(g_list_encryption_data);
struct encryption_data {
	unsigned char* 		buf;
	unsigned int 		len;
	struct list_head 	list;
}; 
void wake_to_read(void)
{
	atomic_set(&be_eqm_read, 1);
	wake_up(&eqm_qh[READ]);
}


void add_encryption_data(unsigned char* buf, unsigned int len)
{
	struct encryption_data *pdata = kzalloc(sizeof(struct encryption_data), GFP_KERNEL);
	if(!pdata) {
		printk(KERN_ERR"[error] alloc memry failed.\n");
		return ;
	}
	pdata->buf = buf;
	pdata->len = len;
	INIT_LIST_HEAD(&pdata->list);
	list_add(&pdata->list, &g_list_encryption_data);
}

static int eqm_open(struct inode* inode, struct file* file)
{
	 printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
	gpd.alloc(&gpd, 10);
	return 0;
}
static int eqm_release(struct inode* inode, struct file* file)
{
	 printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
	gpd.free(&gpd);
	return 0;
}
static ssize_t eqm_read(struct file* pf, char __user* buf, size_t len, loff_t* off)
{
#if 1
	char *tmpbuf = NULL;
	len &= ~(PAGE_SIZE - 1);
	 printk("[:%s%s:%d], len=%d\n",__FILE__,__func__,__LINE__, len);
	tmpbuf = kmalloc(len, GFP_KERNEL);
	gpd.get_data(&gpd, tmpbuf, len);
	if(copy_to_user(buf, tmpbuf, len))
		return -EINVAL;
	kfree(tmpbuf);
#else
	wait_event_interruptible(&eqm_qh[READ], atomic_read(&be_eqm_read)== 1);
	/*  read data */

	atomic_set(&be_eqm_read, 1);
#endif
	return len;
}
static ssize_t eqm_write(struct file *pf, const char __user* buf, size_t len, loff_t *off)
{
#if 1
	len &= ~(PAGE_SIZE - 1);
	  printk("[:%s%s:%d], len=%d\n",__FILE__,__func__,__LINE__, len);
	gpd.set_data(&gpd, buf, len);
#else
	
	wait_event_interruptible(&eqm_qh[WRITE], atomic_read(&be_eqm_write)== 1);

	atomic_set(&be_eqm_write, 1);
#endif	
	return len;
}
static unsigned int eqm_poll(struct file* pf, struct poll_table_struct* table)
{
	unsigned int mask = 0;
	poll_wait(pf, &eqm_qh[READ], table);	/* read */
	//poll_wait(pf, &eqm_qh[WRITE], table);	/* write */
	if(atomic_read(&be_eqm_read) == 1)/* 表示设备可以读取 */
		mask |= POLLIN | POLLRDNORM;
/**表示设备可以写入 
	if(atomic_read(&be_eqm_write) == 1) 
		mask |= POLLOUT | POLLWRNORM;
*/
	return mask;
}
void eqm_vm_open(struct vm_area_struct * area)
{
	  printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
}

void eqm_vm_close(struct vm_area_struct * area)
{
	  printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
}
static struct vm_operations_struct eqm_vm_ops = {
	.open = eqm_vm_open,
	.close = eqm_vm_close,
};
static int eqm_mmap(struct file* pf, struct vm_area_struct* vma)
{
	unsigned long size = vma->vm_end - vma->vm_start;
	int i = 0;
	size &= ~(PAGE_SIZE-1);
	printk("[:%s%s:%d], size=%d, vm_ops=%p\n",__FILE__,__func__,__LINE__, size, vma->vm_ops);
	vma->vm_ops = &eqm_vm_ops;	
	for(i = 0; i < size / PAGE_SIZE; i++) {
		if (remap_pfn_range(vma, vma->vm_start + PAGE_SIZE*i,
				page_to_pfn(gpd.ppage[i]),
				PAGE_SIZE, vma->vm_page_prot)) {
			return -EAGAIN;
		}
	}
	return 0;
}

#ifdef DEBUG_CODE
#include <linux/proc_fs.h>	
#define DEBUG_POLL_PROC_NAME "debug_poll"
struct proc_dir_entry *g_debug_poll = NULL;

int debug_poll_write_proc(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
	wake_to_read();
	return count;
}

#endif

static int __init eqm_module_init(void)
{
	int i = 0;
    printk("[:%s%s:%d],PAGE_SIZE=0x%x\n",__FILE__,__func__,__LINE__, PAGE_SIZE);

	for(i = 0; i < sizeof(eqm_qh); i++)
		init_waitqueue_head(&eqm_qh[i]);

	atomic_set(&be_eqm_read, 0);
	atomic_set(&be_eqm_write, 0);

#ifdef DEBUG_CODE
	g_debug_poll = create_proc_entry(DEBUG_POLL_PROC_NAME, 0777, NULL);
	if(g_debug_poll)
	{
		g_debug_poll->write_proc = debug_poll_write_proc;
	}
#endif
    return  misc_register(&eqm_dev);
}

static void __exit eqm_module_exit(void)
{
    printk("[:%s%s:%d]\n",__FILE__,__func__,__LINE__);
#ifdef DEBUG_CODE
	remove_proc_entry(DEBUG_POLL_PROC_NAME,NULL);
#endif
	misc_deregister(&eqm_dev);
}

module_init(eqm_module_init);
module_exit(eqm_module_exit);
MODULE_LICENSE("GPL");

