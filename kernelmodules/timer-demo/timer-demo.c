#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
static struct timer_list g_timerlist;
int g_index = 0;

static void timer_demo_handle(unsigned long data)
{
    LogPath();
	if(g_index ++ < 10)
		 mod_timer(&g_timerlist,jiffies + 2 * HZ);
}

static int __init timer_demo_init(void)
{
    LogPath();

	Log("HZ=%d",HZ);
    init_timer(&g_timerlist);
    g_timerlist.function = timer_demo_handle;
    g_timerlist.data = &g_timerlist;
    mod_timer(&g_timerlist,jiffies + 2 * HZ);
    return 0;
}

static void __exit timer_demo_exit(void)
{
    LogPath();
    del_timer(&g_timerlist);
}

module_init(timer_demo_init);
module_exit(timer_demo_exit);

MODULE_LICENSE("GPL");


