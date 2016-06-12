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
	{ 0x9d39c447, "netlink_kernel_create" },
	{ 0xea147363, "printk" },
	{ 0x452b2242, "netlink_kernel_release" },
	{ 0xb4390f9a, "mcount" },
	{ 0x7064efe4, "kfree_skb" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A4C9CB740B9B7B5B85269B1");
