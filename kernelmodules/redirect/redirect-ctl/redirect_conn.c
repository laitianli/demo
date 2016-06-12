#include <linux/init.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <net/tcp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/interrupt.h>
#include <linux/in.h>
#include <linux/net.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>		/* for proc_net_* */
#include <linux/seq_file.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <linux/slab.h>
#include "redirect_core.h"

/*
 *  Client hash table: for input and output packets lookups of REDIRECT
 */
static struct list_head *redirect_clt_tab;

/*  SLAB cache for REDIRECT clients */
static kmem_cache_t *redirect_clt_cachep __read_mostly;

/*  counter for current REDIRECT clients */
static atomic_t redirect_clt_count = ATOMIC_INIT(0);

/* random value for REDIRECT client hash */
static unsigned int redirect_clt_rnd;

/*  counter for  REDIRECT times */
static atomic_t redirect_times = ATOMIC_INIT(24*6);

/*
 *  Fine locking granularity for big clients hash table
 */
#define CT_LOCKARRAY_BITS  2
#define CT_LOCKARRAY_SIZE  (1<<CT_LOCKARRAY_BITS)///4
#define CT_LOCKARRAY_MASK  (CT_LOCKARRAY_SIZE-1)///3

struct redirect_aligned_lock
{
	rwlock_t	l;
} __attribute__((__aligned__(SMP_CACHE_BYTES)));

/* lock array for clt table */
//连接表锁数组
static struct redirect_aligned_lock
__redirect_clttbl_lock_array[CT_LOCKARRAY_SIZE] __cacheline_aligned;

//读写时用连接的HASH值和锁数量取模，就得到相应锁
static inline void clt_read_lock(int key)
{
	read_lock(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_read_unlock(int key)
{
	read_unlock(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_write_lock(int key)
{
	write_lock(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_write_unlock(int key)
{
	write_unlock(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_read_lock_bh(int key)
{
	read_lock_bh(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_read_unlock_bh(int key)
{
	read_unlock_bh(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_write_lock_bh(int key)
{
	write_lock_bh(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

static inline void clt_write_unlock_bh(int key)
{
	write_unlock_bh(&__redirect_clttbl_lock_array[key&CT_LOCKARRAY_MASK].l);
}

int get_redirect_user_count()
{
	int count = atomic_read(&redirect_clt_count);
	return count;
}

void get_redirect_user(struct redirect_user *ru, int *len)
{
	int idx, i=0;
	struct redirect_client *rc;

  	//循环所有连接HASH表
	for (idx=0; idx<REDIRECT_CLT_TAB_SIZE; idx++) {
		/*
		 *  Lock is actually needed in this loop.
		 */
		clt_read_lock_bh(idx);
		//循环链表
		list_for_each_entry(rc, &redirect_clt_tab[idx], c_list) {
			//获取用户信息
			memcpy(ru[i].mac, rc->mac, sizeof(rc->mac));
			memcpy(ru[i].user_agent, rc->user_agent, rc->ag_len);
			i++;
			if (i == *len){
				clt_read_unlock_bh(idx);
				goto out;
			}
				
		}
		clt_read_unlock_bh(idx);
	}
out:
		*len = i;
}

int get_redirect_times()
{
	int tag = atomic_read(&redirect_times);
	return tag;
}

void set_redirect_times(int times)
{
	if (times<=0 || times > 24*60)
		times = 24*60;
	atomic_set(&redirect_times, times);
}


/*
 *	Returns hash value for REDIRECT client entry
 */
 static __inline__ int redirect_clt_hashkey(const unsigned char *mac)
{
	return jhash(mac, ETH_ALEN, redirect_clt_rnd)&REDIRECT_CLT_TAB_MASK;
}

/*
将客户端结构添加到连接HASH表
 */
static inline int redirect_clt_hash(struct redirect_client *rc)
{
	int hash;
	int ret;

	/* Hash by mac */
	hash = redirect_clt_hashkey(rc->mac);

	clt_write_lock(hash);

	if (!(rc->flags & REDIRECT_CLT_F_HASHED)) {
		list_add(&rc->c_list, &redirect_clt_tab[hash]);
		//设置HASH标志
		rc->flags |= REDIRECT_CLT_F_HASHED;
		//再次增加引用计数
		atomic_inc(&rc->refcnt);
		ret = 1;
	} else {
		printk("redirect_clt_hash(): request for already hashed, "
			  "called from %p\n", __builtin_return_address(0));
		ret = 0;
	}

	clt_write_unlock(hash);

	return ret;
}

/*
从客户端HASH表中断开
 */
static inline int redirect_clt_unhash(struct redirect_client *rc)
{
	int hash;
	int ret;

	hash = redirect_clt_hashkey(rc->mac);

	clt_write_lock(hash);

	if (rc->flags & REDIRECT_CLT_F_HASHED) {
		//从链表中删除
		list_del(&rc->c_list);
		rc->flags &= ~REDIRECT_CLT_F_HASHED;
		atomic_dec(&rc->refcnt);//减少连接引用计数
		ret = 1;
	} else
		ret = 0;

	clt_write_unlock(hash);

	return ret;
}

struct redirect_client *redirect_clt_get(unsigned char *mac, char *ag, int len)
{
	int hash;
	struct redirect_client *rc;

	hash = redirect_clt_hashkey(mac);

	clt_read_lock(hash);

	list_for_each_entry(rc, &redirect_clt_tab[hash], c_list) {
		len = len < rc->ag_len?len:rc->ag_len;
		if (0==memcmp(mac, rc->mac, ETH_ALEN)&&
			0==memcmp(ag, rc->user_agent, len)) {
			/* HIT */
			atomic_inc(&rc->refcnt);
			clt_read_unlock(hash);
			return rc;
		}
	}

	clt_read_unlock(hash);

	return NULL;
}

/*
 *      Put back the client
 */
void redirect_clt_put(struct redirect_client *rc)
{
	atomic_dec(&rc->refcnt);
}

//连接超时函数
static void redirect_clt_expire(unsigned long data)
{
	struct redirect_client *rc = (struct redirect_client *)data;

	/*
	 *	unhash it if it is hashed in the conn table
	 */
	 //将连接结构中连接HASH表中断开
	if (!redirect_clt_unhash(rc))
		goto expire_later;

	/*
	 *	refcnt==1 implies I'm the only one referrer
	引用为1表示可以删除连接了
	 	*/
	if (likely(atomic_read(&rc->refcnt) == 0)) {
		/* delete the timer if it is activated by other users */
		//删除时钟，如果是定时器到期的话，
		//定时器是已经删除了的
		if (timer_pending(&rc->timer))
			del_timer(&rc->timer);

		//减少客户端总数
		atomic_dec(&redirect_clt_count);

		//释放连接的cache内存
		kmem_cache_free(redirect_clt_cachep, rc);
		printk("die... \n");
		return;
	}

	/* hash it back to the table */
	//还不能删除，重新把结构挂回连接HASH表
	redirect_clt_hash(rc);

  expire_later:
	//修改定时
	mod_timer(&rc->timer, jiffies+10*HZ);
}

//客户端定时器立即到期
static void redirect_clt_expire_now(struct redirect_client *rc)
{
	if (del_timer(&rc->timer))
		mod_timer(&rc->timer, jiffies);
}

/*
 *	Create a new client entry and hash it into the redirect_clt_tab
客户的建立
 */
struct redirect_client *redirect_clt_new(unsigned char *mac, char *ag, int len)
{
	struct redirect_client *rc;

	/*申请conn的内存*/
	rc = kmem_cache_alloc(redirect_clt_cachep, GFP_ATOMIC);
	if (rc == NULL) {
		printk("redirect_clt_new: no memory available.\n");
		return NULL;
	}

	memset(rc, 0, sizeof(*rc));
	INIT_LIST_HEAD(&rc->c_list);
	init_timer(&rc->timer);
	rc->timer.data     = (unsigned long)rc;
	rc->timer.function = redirect_clt_expire;//连接超时函数
	rc->timeout = 24*60*60*HZ/atomic_read(&redirect_times);
	/*连接基本参数赋值*/
	memcpy(rc->mac, mac, ETH_ALEN);
	rc->ag_len = (len<sizeof(rc->user_agent)?len:(sizeof(rc->user_agent)-1));
	memcpy(rc->user_agent, ag, rc->ag_len);
	/*
	 * Set the entry is referenced by the current thread before hashing
	 * it in the table, so that other thread run ip_vs_random_dropentry
	 * but cannot drop this entry.
	 */
	atomic_set(&rc->refcnt, 0);//引用初始值为0
	atomic_inc(&redirect_clt_count);//增加计数

	/* Hash it in the ip_vs_conn_tab finally */
	//将该连接节点加入到REDIRECT连接表中
	redirect_clt_hash(rc);

	//启动定时器
	mod_timer(&rc->timer, jiffies + rc->timeout);
	return rc;
}

/*
释放所有客户，在删除REDIRECT模块时调用，方法是
让所有客户定时器到期而自动调用定时到期函数
 */
static void redirect_clt_flush(void)
{
	int idx;
	struct redirect_client *rc;

  flush_again:
  	//循环所有连接HASH表
	for (idx=0; idx<REDIRECT_CLT_TAB_SIZE; idx++) {
		/*
		 *  Lock is actually needed in this loop.
		 */
		clt_write_lock_bh(idx);
		//循环链表
		list_for_each_entry(rc, &redirect_clt_tab[idx], c_list) {
			//定时器立即到期
			redirect_clt_expire_now(rc);
		}
		clt_write_unlock_bh(idx);
	}

	if (atomic_read(&redirect_clt_count) != 0) {
		//如果客户数不为0，重新调度进程，重新释放
		schedule();
		goto flush_again;
	}
}


int redirect_clt_init(void)
{
	int idx;

	 /*约8192个客户端*/
	redirect_clt_tab = vmalloc(REDIRECT_CLT_TAB_SIZE*sizeof(struct list_head));
	if (!redirect_clt_tab)
		return -ENOMEM;

	/* Allocate ip_vs_conn slab cache */
	/*创建专用对象缓冲区*/
	redirect_clt_cachep = kmem_cache_create("redirect_client",
					      sizeof(struct redirect_client), 0,
					      SLAB_HWCACHE_ALIGN, NULL, NULL);
	if (!redirect_clt_cachep) {
		vfree(redirect_clt_tab);
		return -ENOMEM;
	}

	/*每个连接需要120字节*/
	printk("Each Client entry needs %Zd bytes at least\n",
		  sizeof(struct redirect_client));

	for (idx = 0; idx < REDIRECT_CLT_TAB_SIZE; idx++) {
		INIT_LIST_HEAD(&redirect_clt_tab[idx]);
	}

	for (idx = 0; idx < CT_LOCKARRAY_SIZE; idx++)  {
		rwlock_init(&__redirect_clttbl_lock_array[idx].l);
	}

	/* calculate the random value for connection hash */
	get_random_bytes(&redirect_clt_rnd, sizeof(redirect_clt_rnd));

	return 0;
}


void redirect_clt_exit(void)
{
	/* flush all the client entries first */
	redirect_clt_flush();

	/* Release the empty cache */
	kmem_cache_destroy(redirect_clt_cachep);
	vfree(redirect_clt_tab);
}


