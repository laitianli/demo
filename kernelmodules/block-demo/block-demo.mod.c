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
	{ 0xa0f3c5e1, "struct_module" },
	{ 0x8b282f70, "blk_init_queue" },
	{ 0x712a8e53, "alloc_disk" },
	{ 0xdd132261, "printk" },
	{ 0x613eda8b, "del_gendisk" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0xeac1c4af, "unregister_blkdev" },
	{ 0xd93783bd, "elv_next_request" },
	{ 0xb04657dd, "add_disk" },
	{ 0x74375e53, "end_request" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7E61ACB745DE0CDA4D746EB");
