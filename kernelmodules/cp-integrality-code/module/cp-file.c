#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
/*
 *1.创建proc文件，操作函数集file_operations
 *2.创建拷贝线程，实现两个设备之前的拷贝
 * 
 */
extern int create_cp_info_proc(void);
extern void remove_cp_info_proc(void);

extern void begin_cp_task(void);
extern void stop_cp_task(void);

static int __init cp_file_init(void)
{
	int ret = 0;
	ret = create_cp_info_proc();

	//begin_cp_task();
	return ret;
}

static void __exit cp_file_exit(void)
{
	remove_cp_info_proc();
	//stop_cp_task();
}

module_init(cp_file_init);
module_exit(cp_file_exit);
MODULE_LICENSE("GPL");
