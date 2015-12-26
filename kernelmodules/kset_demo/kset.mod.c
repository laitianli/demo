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
	{ 0x494729b2, "struct_module" },
	{ 0xfced3ef3, "kset_register" },
	{ 0x27215ccb, "kobject_set_name" },
	{ 0xdefd7f5a, "kobject_unregister" },
	{ 0x1d26aa98, "sprintf" },
	{ 0xde0bdcff, "memset" },
	{ 0xdd132261, "printk" },
	{ 0xd41fb180, "kobject_add" },
	{ 0xed253618, "sysfs_create_file" },
	{ 0x12204822, "kset_unregister" },
	{ 0x9e0088c7, "kobject_init" },
	{ 0x28110123, "sysfs_remove_file" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "939521B4CB8D8AED1E66C79");
