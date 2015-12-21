#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/version.h>
#include "kobject_demo.h"
/*
static struct attribute g_attr[] = {
	[0] = {"val_1",THIS_MODULE,0755},
	[1] = {"val_2",THIS_MODULE,0755},
};
*/
ssize_t demo_show_val_1(struct demo_obj* obj,struct demo_attribute* attr,char* buf)
{
	printk("[Note]=[%s:%s:%d]=\n",__FILE__,__func__,__LINE__);
	int len = sprintf(buf,"fun:%s\n",__func__);
	return len;
}
ssize_t demo_store_val_1(struct demo_obj* obj,struct demo_attribute* attr,const char* buf,size_t count)
{
	printk("[Note]=[%s:%s:%d]=count:%d,buf:%s\n",__FILE__,__func__,__LINE__,count,buf);
	return count;
}
ssize_t demo_show_val_2(struct demo_obj* obj,struct demo_attribute* attr,char* buf)
{
	printk("[Note]=[%s:%s:%d]=\n",__FILE__,__func__,__LINE__);
	int len = sprintf(buf,"fun:%s\n",__func__);
	return len;
}
ssize_t demo_store_val_2(struct demo_obj* obj,struct demo_attribute* attr,const char* buf,size_t count)
{
	printk("[Note]=[%s:%s:%d]=connt:%d,buf:%s\n",__FILE__,__func__,__LINE__,count,buf);
	return count;
}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18)
static struct demo_attribute g_demo_attr[] = {
	[0] = {.attr = {"val_1",THIS_MODULE,0755},.show = demo_show_val_1,.store = demo_store_val_1},
	[1] = {.attr = {"val_2",THIS_MODULE,0755},.show = demo_show_val_2,.store = demo_store_val_2},
};
#else
static struct demo_attribute g_demo_attr[] = {
    [0] = {.attr = {"val_1",0755},.show = demo_show_val_1,.store = demo_store_val_1},
    [1] = {.attr = {"val_2",0755},.show = demo_show_val_2,.store = demo_store_val_2},
};
#endif

void demo_kobject_release(struct kobject* obj)
{
	printk("[Note]=[%s:%s:%d]=\n",__FILE__,__func__,__LINE__);
}

ssize_t object_show(struct kobject* obj,struct attribute* attr,char* buf)
{
	int len = 0;
	struct demo_attribute* demo_attr = container_of(attr,struct demo_attribute,attr);
	struct demo_obj* dobj = container_of(obj,struct demo_obj,obj);
	len = demo_attr->show(dobj,demo_attr,buf);
	return len;
}

ssize_t object_store(struct kobject* obj,struct attribute* attr,const char* buf,size_t count)
{
	struct demo_attribute* demo_attr = container_of(attr,struct demo_attribute,attr);
	struct demo_obj* dobj = container_of(obj,struct demo_obj,obj);
	int len = demo_attr->store(dobj,demo_attr,buf,count);
	if(len != count)
		return -1;
	return count;
}

static struct sysfs_ops g_ops = {
	.show = object_show,
	.store = object_store,
};

static struct kobj_type g_type = {
	.release = demo_kobject_release,
	.sysfs_ops = &g_ops,
//	.default_attrs = &g_attr,
};


int demo_create_file(struct demo_obj* obj,const struct demo_attribute* attr)
{
	int err = 0;
	if(obj)
		err = sysfs_create_file(&obj->obj,&attr->attr);
	return err;
}

int demo_remove_file(struct demo_obj* obj,const struct demo_attribute* attr)
{
	int err = 0;
	if(obj)
		sysfs_remove_file(&obj->obj,&attr->attr);
	return err;

}

struct demo_obj g_kobj = {0};
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18) 
static int __init kobject_demo_init(void)
{
	kobject_init(&g_kobj.obj);
	int	ret = kobject_set_name(&g_kobj.obj,"%s","kobject_demo");
	if(ret)
	{
		printk("[OS][error]=[%s:%s:%d] error code:%d\n",__FILE__,__func__,__LINE__,ret);
		return -1;
	}

	g_kobj.obj.ktype = &g_type;

	ret = kobject_add(&g_kobj.obj);
	if(ret)
	{
		printk("[OS][error]=[%s:%s:%d] error code:%d\n",__FILE__,__func__,__LINE__,ret);
		return -1;
	}

	int i = 0;
	for (i = 0; i < sizeof(g_demo_attr)/sizeof(struct demo_attribute); i++)
	{
		ret = demo_create_file(&g_kobj.obj,&g_demo_attr[i]);
		if(ret)
		{
			printk("[OS][error]=[%s:%s:%d] error code:%d\n",__FILE__,__func__,__LINE__,ret);
			return -1;
		}

	}
    dump_stack();
	return 0;
}
static void __exit kobject_demo_exit(void)
{
	int i = 0;
	for (i = 0; i < sizeof(g_demo_attr)/sizeof(struct demo_attribute); i++)
	{
		demo_remove_file(&g_kobj.obj,&g_demo_attr[i]);
	}
	kobject_unregister(&g_kobj);
}
#else
static int __init kobject_demo_init(void)
{
    int ret = kobject_init_and_add(&g_kobj.obj,&g_type,NULL,"kobject_demo");
    if(ret != 0)
    {
        printk(KERN_DEBUG"[Error]=kobject_init_and_add failed.\n");
        return -1;
    }
	int i = 0;
	for (i = 0; i < sizeof(g_demo_attr)/sizeof(struct demo_attribute); i++)
	{
		ret = demo_create_file(&g_kobj.obj,&g_demo_attr[i]);
		if(ret)
		{
			printk("[OS][error]=[%s:%s:%d] error code:%d\n",__FILE__,__func__,__LINE__,ret);
			return -1;
		}

	}
    char *path = kobject_get_path(&g_kobj.obj,GFP_KERNEL);
    printk("path=%s\n",path);
    if(path)
        kfree(path);
//   ret =  kobject_rename(&g_kobj.obj,"kobj-rename");
//   printk("ret=%d\n",ret);

    return 0;
}
static void __exit kobject_demo_exit(void)
{
    int i = 0;
    for(i = 0; i < sizeof(g_demo_attr)/sizeof(struct demo_attribute); i++)
        demo_remove_file(&g_kobj.obj,&g_demo_attr);

    kobject_del(&g_kobj);
}
#endif

module_init(kobject_demo_init);
module_exit(kobject_demo_exit);

MODULE_LICENSE("GPL");

