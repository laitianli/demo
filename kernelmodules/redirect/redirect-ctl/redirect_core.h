#include <linux/if_ether.h>
/*
 *     REDIRECT client entry hash table
 */
#define REDIRECT_CLT_TAB_BITS   13
#define REDIRECT_CLT_TAB_SIZE     (1 << REDIRECT_CLT_TAB_BITS)  /*8192*/
#define REDIRECT_CLT_TAB_MASK     (REDIRECT_CLT_TAB_SIZE - 1)

/*
 *      REDIRECT client Flags
 */
#define REDIRECT_CLT_F_HASHED	0x0001		/* hashed entry */

/*
 *	REDIRECT structure allocated for each dynamically scheduled client
 */
struct redirect_client {
	struct list_head        c_list;         /* hashed list heads */

	/* Mac */
	unsigned char		mac[ETH_ALEN];

	/* User-Agent */
	unsigned char		user_agent[128];
	int ag_len;

	/* Protocol, addresses and port numbers */
	__u32                   caddr;          /* client address */
	__u32                   daddr;          /* destination address */
	__u16                   cport;
	__u16                   dport;
	__u16                   protocol;       /* Which protocol (TCP/UDP) */

	/* counter and timer */
	atomic_t		refcnt;		/* reference count */
	struct timer_list	timer;		/* Expiration timer */
	volatile unsigned long	timeout;	/* timeout */

	/* Flags and state transition */
	volatile __u16          flags;          /* status flags */
};

struct redirect_user
{
	char mac[6];
	char user_agent[128];
};
/*count:user1&user2&user3*/
extern int get_redirect_user_count(void);
extern void get_redirect_user(struct redirect_user *ru, int *len);

extern struct redirect_client *redirect_clt_new(unsigned char *mac, char *ag, int len);
extern struct redirect_client *redirect_clt_get(unsigned char *mac, char *ag, int len);
extern void redirect_clt_put(struct redirect_client *rc);
extern int redirect_clt_init(void);
extern void redirect_clt_exit(void);
extern int get_redirect_times();
extern void set_redirect_times(int times);
extern int get_redirect_tag();
extern void set_redirect_tag(int tag);
extern int setup_redirect_url( const char *url );
extern char *get_redirect_url(void);


struct gbuffer{
    u8  *buf;
    u32 len;
};

typedef struct gbuffer gbuffer;
typedef struct gbuffer gbuffer_t;


static inline void _gbuffer_free(gbuffer *p)

{

    if ( NULL == p )
        return;

    if ( likely( p->buf != NULL ) ){
        kfree( p->buf );
        p->buf = NULL;
    }

    kfree( p );
}



static inline gbuffer_t* __gbuffer_alloc(void)

{
    gbuffer_t *p = NULL;
    p = kzalloc( sizeof(*p), GFP_KERNEL );
    if ( unlikely( NULL == p ) ){
        return NULL;
    }

    p->buf = NULL;
    p->len = 0;

    return p;
}


