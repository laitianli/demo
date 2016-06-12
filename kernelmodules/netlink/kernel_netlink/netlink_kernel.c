#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/sock.h>

#define DEMO_NETLINK_ID		29

static struct sock* g_sock;
static struct net g_net;
static u32 pid = 0;

void demo_netlink_recv(struct sk_buff *__skb)
{
	struct nlmsghdr *nlh;  
    struct sk_buff  *skb;  
	printk("[%s:%s:%d]=skb=%p\n",__FILE__,__func__,__LINE__, __skb);

   	skb = skb_get(__skb);  
    if (skb->len >= NLMSG_SPACE(0)) {  
       nlh = (struct nlmsghdr *)skb->data;  
        if( pid != 0 ) printk("Pid != 0 \n ");  
        pid = nlh->nlmsg_pid;  //获取应用程序的pid  
        printk(KERN_INFO "net_link: pid is %d, data %s:\n", pid, (char *)NLMSG_DATA(nlh));  
        kfree_skb(skb);   
    } 
}

int create_demo_netlink(void)
{
	g_sock = netlink_kernel_create(&g_net, DEMO_NETLINK_ID, 0,
				demo_netlink_recv, NULL, THIS_MODULE);
	if (!g_sock)
	{
		printk(KERN_ERR "[Error] netlink_kernel_create failed.\n");
		return -ENODEV;
	}
	return 0;
}

void remove_demo_netlink(void)
{
	netlink_kernel_release(g_sock);
}

static int __init netlink_kernel_module_init(void)
{
    printk("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	return create_demo_netlink();
    return  0;
}

static void __exit netlink_kernel_module_exit(void)
{
    printk("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	remove_demo_netlink();
}

module_init(netlink_kernel_module_init);
module_exit(netlink_kernel_module_exit);
MODULE_LICENSE("GPL");

