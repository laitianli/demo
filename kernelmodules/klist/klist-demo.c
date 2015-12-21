#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/klist.h>
#include <linux/kref.h>
#include <linux/slab.h>

void people_get(struct klist_node* node);
void people_put(struct klist_node* node);
DEFINE_KLIST(g_all_people_klist,people_get,people_put);;
struct people
{
    struct klist_node   node;
    int                 age;
    char                name[20];
    struct kref         kref;
};

static void release_people(struct kref* ref)
{
    struct people* p = container_of(ref,struct people,kref);
    printk("[Note]=[%s]=release memry.\n",__func__);
    kfree(p);
}

void people_get(struct klist_node* node)
{
    struct people* p = container_of(node,struct people,node);
    kref_get(&p->kref);
}
void people_put(struct klist_node* node)
{
    struct people* p = container_of(node,struct people,node);
    kref_put(&p->kref,release_people);
}
struct people*  alloc_people(void)
{
    struct people* p = kmalloc(sizeof(struct people),GFP_KERNEL);
    if(p)
    {
        memset(p,0,sizeof(struct people));
        kref_init(&p->kref);
    }
    return p;
}
#define PEOPLE_NUM 10
void build_people_list(void)
{
    int i = 0;
    struct people* p = NULL;
    for(i = 0; i < PEOPLE_NUM; i++)
    {
        p = alloc_people();
        p->age = 25 + i;
        sprintf(p->name,"%d-name",i);
        klist_add_tail(&p->node,&g_all_people_klist);
    }
}

void traverse_people_list(void)
{
    struct klist_iter iter;
    klist_iter_init(&g_all_people_klist,&iter);

    struct klist_node* next = NULL;
    struct people* p = NULL;
    while((next = klist_next(&iter)) != NULL)
    {
        p = container_of(next,struct people,node);
        printk("name:%s,age:%d\n",p->name,p->age);
    }
    klist_iter_exit(&iter);
}

void remove_people_list(void)
{
     int i = 0;
     struct klist_node* node = NULL;
     for(i = 0; i < PEOPLE_NUM; i++)
     {
         node = container_of(g_all_people_klist.k_list.next,struct klist_node,n_node);
         /*必须对people:kref减1，因为在klist_next会调用people_get*/
         kref_put(&(container_of(node,struct people,node)->kref),release_people);
		 /*释放people接口的最终函数*/
         klist_remove(node);
     }
 
}

static int __init klist_demo_init(void)
{
    build_people_list();
    
    traverse_people_list();
    
    return 0;
}

static void __exit klist_demo_exit(void)
{
    remove_people_list();
}

module_init(klist_demo_init);
module_exit(klist_demo_exit);
MODULE_LICENSE("GPL");
