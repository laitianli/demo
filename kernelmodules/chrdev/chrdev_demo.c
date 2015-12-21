#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>

static int __init chrdev_demo_init(void)
{
    return 0;
}

static void __exit chrdev_demo_exit(void)
{
}

module_init(chrdev_demo_init);
module_exit(chrdev_demo_exit);
MODULE_LICENSE("GPL");

