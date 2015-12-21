#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

struct demo_operations 
{
	void (*first_fun)(void);
	void (*second_fun)(void);
	void (*third_fun)(void);

	const struct demo_operations* inherits;
};

void parent_first_fun(void)
{
	Log("parent_first_fun=0x%p",parent_first_fun);
}
void parent_second_fun(void)
{
	Log("parent_second_fun=0x%p",parent_second_fun);
}
void parent_third_fun(void)
{
	Log("parent_third_fun=0x%p",parent_third_fun);
}
void child_first_fun(void)
{
	Log("child_first_fun=0x%p",child_first_fun);
}
void child_second_fun(void)
{
	Log("child_second_fun=0x%p",child_second_fun);
}
void child_third_fun(void)
{
	Log("child_third_fun=0x%p",child_third_fun);
}

const struct demo_operations g_basic = {
	.first_fun	= parent_first_fun,
	.second_fun = parent_second_fun,
	.third_fun = parent_third_fun,
};

struct demo_operations g_child = {
	.inherits = &g_basic,

	.first_fun = child_first_fun,
	.second_fun = child_second_fun,
};

static int inherits_module_init(void)
{
    printk(KERN_DEBUG"Hello World!\n");
	g_child.first_fun();
	g_child.second_fun();
//	g_child.third_fun();
    return 0;
}

static void inherits_module_exit(void)
{
    printk(KERN_DEBUG"task01 is over!\n");
}

module_init(inherits_module_init);
module_exit(inherits_module_exit);

MODULE_LICENSE("GPL");
