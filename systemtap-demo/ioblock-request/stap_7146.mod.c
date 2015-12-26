#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xfbff0f1, "struct_module" },
	{ 0x1d676acf, "boot_tvec_bases" },
	{ 0x89b301d4, "param_get_int" },
	{ 0x98bd6f46, "param_set_int" },
	{ 0x672144bd, "strlcpy" },
	{ 0x14b2d556, "register_kprobe" },
	{ 0x9f984513, "strrchr" },
	{ 0x167e7f9d, "__get_user_1" },
	{ 0x3ffa545e, "register_kretprobe" },
	{ 0xef04d90c, "kmem_cache_zalloc" },
	{ 0x89892632, "malloc_sizes" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0x6729d3df, "__get_user_4" },
	{ 0x59eeab4e, "destroy_workqueue" },
	{ 0x1fb3e629, "unregister_kprobe" },
	{ 0x496da090, "__next_cpu" },
	{ 0xdfa38fb3, "__first_cpu" },
	{ 0x760a0f4f, "yield" },
	{ 0xbd8e3191, "cpu_possible_map" },
	{ 0x6091797f, "synchronize_rcu" },
	{ 0x8cbdc82, "unregister_kretprobe" },
	{ 0xe914e41e, "strcpy" },
	{ 0xf3522c9c, "vscnprintf" },
	{ 0xcc07af75, "strnlen" },
	{ 0x6ecf8a12, "relay_buf_full" },
	{ 0xbe499d81, "copy_to_user" },
	{ 0x271f4d2a, "finish_wait" },
	{ 0x65f4af5f, "prepare_to_wait" },
	{ 0x1000e51, "schedule" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xf3b39202, "mod_timer" },
	{ 0x9be7216f, "dput" },
	{ 0xa464d8ff, "mutex_unlock" },
	{ 0xf831a1d1, "lookup_one_len" },
	{ 0xd8eb2ab5, "mutex_lock" },
	{ 0xec7bc0d, "__mod_timer" },
	{ 0x7d11c268, "jiffies" },
	{ 0xa5c89579, "init_timer" },
	{ 0x3bea9b9c, "get_fs_type" },
	{ 0x2b25c6da, "__create_workqueue" },
	{ 0xf98ff104, "relay_open" },
	{ 0x6067a146, "memcpy" },
	{ 0xb9ee4df7, "relay_file_operations" },
	{ 0xb3a307c6, "si_meminfo" },
	{ 0x4cbbd171, "__bitmap_weight" },
	{ 0xc2689d0d, "cpu_online_map" },
	{ 0xf689e4fb, "__alloc_percpu" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xedcf8e0e, "queue_delayed_work" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x5a5e7ea3, "simple_read_from_buffer" },
	{ 0x881793e6, "debugfs_create_file" },
	{ 0xd012c83a, "free_percpu" },
	{ 0x4e017548, "simple_empty" },
	{ 0x323014a3, "debugfs_create_dir" },
	{ 0x5ca357ff, "relay_close" },
	{ 0x4e6d5aae, "debugfs_remove" },
	{ 0xdd132261, "printk" },
	{ 0xf9a482f9, "msleep" },
	{ 0xdb09708f, "__wake_up" },
	{ 0x3f38f856, "relay_flush" },
	{ 0xc659d5a, "del_timer_sync" },
	{ 0x37a0cba, "kfree" },
	{ 0xdf15005, "_spin_unlock_irqrestore" },
	{ 0xdd5a37a7, "_spin_lock_irqsave" },
	{ 0x8f77584d, "mmput" },
	{ 0x42f8c19a, "up_read" },
	{ 0x6057a993, "mem_section" },
	{ 0xda4008e6, "cond_resched" },
	{ 0x46399357, "get_user_pages" },
	{ 0x8328c5fb, "put_page" },
	{ 0xc7c2ea9e, "set_page_dirty_lock" },
	{ 0x9a5fdbf3, "down_read" },
	{ 0x71116896, "get_task_mm" },
	{ 0xaf25400d, "snprintf" },
	{ 0x429328d9, "_spin_lock" },
	{ 0x8d3894f2, "_ctype" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "209F9F645A7FF45495EE938");
