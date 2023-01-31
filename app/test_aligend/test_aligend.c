#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ODP_CACHE_LINE_SIZE 64
#define ODP_ALIGNED(x) __attribute__((__aligned__(x)))
#define ODP_ALIGNED_CACHE   ODP_ALIGNED(ODP_CACHE_LINE_SIZE)

#define OFP_NUM_IFNET_IP_ADDRS 8
#define	OFP_ETHER_ADDR_LEN		6
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long long   uint64_t;

struct ofp_ifnet_ipaddr {
	uint32_t ip_addr; /* network byte order */
	uint32_t bcast_addr; /* network byte order */
	uint8_t  masklen;
};

struct ip6_addr {
	uint8_t addr[16];
};
typedef void* odp_pktio_t;
/** @internal */
typedef struct odp_pktin_queue_t {
	odp_pktio_t pktio; /**< @internal pktio handle */
	int index;         /**< @internal pktio queue index */
} odp_pktin_queue_t;

/** @internal */
typedef struct odp_pktout_queue_t {
	odp_pktio_t pktio; /**< @internal pktio handle */
	int index;         /**< @internal pktio queue index */
} odp_pktout_queue_t;
//ODP_ALIGNED_CACHE 
struct ofp_ifnet {
	struct ofp_ifnet_ipaddr	ip_addr_info[OFP_NUM_IFNET_IP_ADDRS];
	uint32_t ip_addr_mtx;
	uint16_t	port;
	uint16_t	vlan;
	uint16_t	vrf;
	uint8_t		if_state;
	uint8_t		if_type;
	uint32_t	if_flags;

	uint8_t		mac[OFP_ETHER_ADDR_LEN];
	uint16_t	if_mtu;

	uint32_t	ip_p2p; /* network byte order */
	uint32_t	ip_local; /* network byte order */
	uint16_t	physport;
	uint16_t	physvlan;
	uint32_t	ip_remote; /* network byte order */

	uint8_t		link_local[16];
	uint8_t		ip6_addr[16];
	uint8_t		ip6_prefix;
	void		*vlan_structs;
	char		if_name[32];
	void* 		pktio;
	uint32_t        chksum_offload_flags;
	unsigned	out_queue_num;
	uint8_t		out_queue_type;
	odp_pktout_queue_t out_queue_pktout[64];
	void* out_queue_queue[64];
	void*	loopq_def;
	void*	pkt_pool;
	int		linux_index;
	int		fd;
	void*	spq_def;

}ODP_ALIGNED(1);

static inline void write_ip6_addr(uint8_t *ip6_addr)
{
	*((uint64_t *)ip6_addr) = 0x99888;
	*((uint64_t *)(ip6_addr + 8)) = 0xabcd;
}
static inline void read_ip6_addr(uint8_t *ip6_addr, uint8_t *dst)
{
	*((uint64_t*)dst) = *((uint64_t*)ip6_addr);
	*((uint64_t*)(dst + 8)) = *((uint64_t*)(ip6_addr + 8));
}
static inline int compare_ip6_addr(uint8_t* addr1, uint8_t* addr2)
{
	return (((*(uint64_t *)addr1 ==  *(uint64_t *)addr2) &&
		(*(uint64_t *)(addr1 + 8) ==  *(uint64_t *)(addr2 + 8)))
	? 1 : 0);
}

void test_func()
{
	uint8_t addr1[16];
	uint8_t addr2[16];
	struct ofp_ifnet if1, if2;
	memset(&if1, 0, sizeof(struct ofp_ifnet));
	memset(&if2, 0, sizeof(struct ofp_ifnet));
	write_ip6_addr(if1.ip6_addr);	
	write_ip6_addr(if2.ip6_addr);	
	read_ip6_addr(if1.ip6_addr, addr1);
	read_ip6_addr(if2.ip6_addr, addr2);
	compare_ip6_addr(if1.ip6_addr, if2.ip6_addr);

	srandom(time(NULL));
	struct ofp_ifnet *pif1, *pif2;
    char *buf1, *buf2;
	int s1, s2;
	s1 = sizeof(struct ofp_ifnet) + (random() % 10321) + 1;
	buf1 = malloc(s1);
	if (!buf1) {
		printf("[error] malloc memory failed! [%s:%d]\n", __func__, __LINE__);
		return ;
	}
	s2 = sizeof(struct ofp_ifnet)+ (random() % 4971) + 1;
	buf2 = malloc(s2);
	if (!buf2) {
		free(buf1);
		printf("[error] malloc memory failed! [%s:%d]\n", __func__, __LINE__);
		return ;
	}

	memset(buf1, 0, s1);
	memset(buf2, 0, s2);
    pif1 = (struct ofp_ifnet*)(buf1 + 1);
    pif2 = (struct ofp_ifnet*)(buf2 + 1);
	write_ip6_addr(pif1->ip6_addr);	
	write_ip6_addr(pif2->ip6_addr);	
	read_ip6_addr(pif1->ip6_addr, addr1);
	read_ip6_addr(pif2->ip6_addr, addr2);
	compare_ip6_addr(pif1->ip6_addr, pif2->ip6_addr);

	free(buf1);
	free(buf2);

}


int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("[Error] please intput numerical value!!\n");
		printf("\t eg: %s 1000000\n", argv[0]);
		exit(-1);
	}

	printf("sizoef(uint8_t): %8lu\n", sizeof(uint8_t));
	printf("sizoef(uint16_t): %8lu\n", sizeof(uint16_t));
	printf("sizoef(uint32_t): %8lu\n", sizeof(uint32_t));
	printf("sizoef(uint64_t): %8lu\n", sizeof(uint64_t));
	int i = 0;
	do {
		test_func();
		usleep(80);
		if ((i + 1) % 16 == 0)
			printf("%10d\r", i + 1);
	}while(++i);
	return 0;
}
