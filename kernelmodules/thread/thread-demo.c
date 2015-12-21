#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define THREAD_NAME "thread demo"
static struct task_struct *g_thread = NULL;
static struct timer_list g_timer;
static int sleep_condition  = 0;
static int is_thread_stop = 0;

static void timer_handle(unsigned long data)
{
	static int is_thread_run = 1;
	unsigned long timeout = 0;
	if (is_thread_run)
	{
		timeout = jiffies + 3 * HZ;
		is_thread_run = 0;
		//kthread_stop(data);
		sleep_condition = 1;
	}
	else
	{
		timeout = jiffies + HZ / 50;
		is_thread_run = 1;
		sleep_condition = 0;

		if (!is_thread_stop)
			wake_up_process(data);
	}

	static int count = 5;
	if (--count < 0)
		sleep_condition = 2;
	else
		mod_timer(&g_timer,timeout);
}

void create_timer(void)
{
	init_timer(&g_timer);
	g_timer.function = timer_handle;
	g_timer.data = g_thread;
	mod_timer(&g_timer,jiffies +  HZ / 50);
}

static int thread_handle(void* data)
{
	create_timer();
	while (!kthread_should_stop())
	{
		if (sleep_condition == 1)
		{
			Log("===< thread sleep >====");
			schedule();
            set_current_state(TASK_INTERRUPTIBLE);
			continue;
		}
		else if(sleep_condition == 2)
			break;

		Log("===< thread running >====");
	}

	is_thread_stop = 1;
	Log("===< thread stop >====");
	return 0;
}

static int __init kernel_thread_init(void)
{
	g_thread = kthread_run(thread_handle, g_thread,THREAD_NAME);
	if (IS_ERR(g_thread))
	{
		printk("[Error]=create thread error.\n");
		return -1;
	}
	wake_up_process(g_thread);
	return 0;
}

static void __exit kernel_thread_exit(void)
{

}

module_init(kernel_thread_init);
module_exit(kernel_thread_exit);
MODULE_LICENSE("GPL");
