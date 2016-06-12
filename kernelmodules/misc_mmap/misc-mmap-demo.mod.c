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
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa915568d, "module_layout" },
	{ 0x874545ac, "alloc_pages_current" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xfa2e111f, "slab_buffer_size" },
	{ 0xd691cba2, "malloc_sizes" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0xc8168a4, "remove_proc_entry" },
	{ 0x343a1a8, "__list_add" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0x811e2e14, "misc_register" },
	{ 0xde0bdcff, "memset" },
	{ 0xf85ccdae, "kmem_cache_alloc_notrace" },
	{ 0xc16fe12d, "__memcpy" },
	{ 0xea147363, "printk" },
	{ 0x85f8a266, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0xbf3175aa, "create_proc_entry" },
	{ 0x642e54ac, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0x7ad231b0, "remap_pfn_range" },
	{ 0x5ae70d35, "put_page" },
	{ 0xc4192b04, "misc_deregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "40B0E0AF2383312FE8ED9B1");
