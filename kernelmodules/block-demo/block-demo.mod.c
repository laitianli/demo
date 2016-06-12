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
	{ 0x894bb20f, "blk_init_queue" },
	{ 0xbfdeb12, "alloc_disk" },
	{ 0xea147363, "printk" },
	{ 0xbbefd84d, "del_gendisk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x4d82231c, "blk_fetch_request" },
	{ 0x922656a2, "__blk_end_request_all" },
	{ 0x6067a146, "memcpy" },
	{ 0x66d37821, "add_disk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "6438A0C9B7EF828CB6EEC5D");
