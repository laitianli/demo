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
	{ 0x9717ec2a, "struct_module" },
	{ 0x96dbb3d5, "cdev_del" },
	{ 0x297a281c, "cdev_init" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x89892632, "malloc_sizes" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xde0bdcff, "memset" },
	{ 0xdd132261, "printk" },
	{ 0xbe499d81, "copy_to_user" },
	{ 0xf9c8737d, "cdev_add" },
	{ 0x4c503ced, "kmem_cache_alloc" },
	{ 0x1000e51, "schedule" },
	{ 0xdb09708f, "__wake_up" },
	{ 0x65f4af5f, "prepare_to_wait" },
	{ 0x271f4d2a, "finish_wait" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4B1EFC83C29C47BBD723B1C");
