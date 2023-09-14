
#include <linux/bpf.h>
#include <linux/pkt_cls.h>
//#include <uapi/linux/bpf.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
/*
 * Comments from Linux Kernel:
 * Helper macro to place programs, maps, license in
 * different sections in elf_bpf file. Section names
 * are interpreted by elf_bpf loader.
 * End of comments

 * You can either use the helper header file below
 * so that you don't need to define it yourself:
 * #include <bpf/bpf_helpers.h> 
 */
#define SEC(NAME) __attribute__((section(NAME), used))
#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))
#define __my_htons ___constant_swab16

typedef unsigned long long u64;
typedef long long s64;
typedef __u32 u32;
typedef __s32 s32;

typedef __u16 u16;
typedef __s16 s16;

typedef __u8  u8;
typedef __s8  s8;

struct vlan_hdr {
	__be16 h_vlan_TCI;
	__be16 h_vlan_encapsulated_proto;
};
typedef enum {enINGRESS = 1, enEGRESS} enDIRECT;
static int (*bpf_trace_printk)(const char *fmt, int fmt_size, ...) =
	(void *) BPF_FUNC_trace_printk;

 #define bpf_printk(fmt, ...)                            \
({                                                      \
        char ____fmt[] = fmt;                           \
        bpf_trace_printk(____fmt, sizeof(____fmt),      \
                         ##__VA_ARGS__);                \
})

 struct sk_buff;
 unsigned long long load_byte(void *skb,
				  unsigned long long off) asm("llvm.bpf.load.byte");
 unsigned long long load_half(void *skb,
				  unsigned long long off) asm("llvm.bpf.load.half");
 unsigned long long load_word(void *skb,
				  unsigned long long off) asm("llvm.bpf.load.word");

#ifndef offsetof
typedef unsigned int	 size_t;

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline)
#endif

/* IP flags. */
#define IP_CE		0x8000		/* Flag: "Congestion"		*/
#define IP_DF		0x4000		/* Flag: "Don't Fragment"	*/
#define IP_MF		0x2000		/* Flag: "More Fragments"	*/
#define IP_OFFSET	0x1FFF		/* "Fragment Offset" part	*/

#define GTPU_HEAD_LEN 8

#define LOCAL_PORT 2156
#define PROXY_PORT 2152

#define GTPU_InvalidTeId 49152


__always_inline static int ip_is_fragment(const struct iphdr *iph)
{
	return (iph->frag_off & __my_htons(IP_MF | IP_OFFSET)) != 0;
}


__always_inline static int bpf_cp_parse_ipv4(u8 *data, u16* nh_off, u8 *data_end, u8* should_recheck)
{
	struct iphdr *iph = (struct iphdr *)(data + *nh_off);

	if ((u8*)(iph + 1) > data_end)
		return 0;
	*nh_off += iph->ihl << 2;
	*should_recheck = ip_is_fragment(iph);
	return iph->protocol;
}

__always_inline static int bpf_cp_parse_ipv6(u8 *data, u16* nh_off, u8 *data_end)
{
	struct ipv6hdr *ip6h = (struct ipv6hdr *)(data + *nh_off);

	if ((u8*)(ip6h + 1) > data_end)
		return 0;
	*nh_off += sizeof(struct ipv6hdr);
	return ip6h->nexthdr;
}

__always_inline static unsigned int get_gtpu_id(u8 *data, u16 nh_off, u8 *data_end)
{
	if ((u8*)(data + nh_off) > data_end)
		return 0;
	unsigned int id = 0;
	u8* hgtpu = data + nh_off;
	if ((u8*)hgtpu + GTPU_HEAD_LEN <= data_end) 
		id = (hgtpu[4] << 24)|(hgtpu[5] << 16) | (hgtpu[6] << 8) | hgtpu[7];
	return id;
}


__always_inline static void bpf_cp_change_udp_port_ingress(u8 *data, u16 nh_off, u8 *data_end)
{
	if ((u8*)(data + nh_off) > data_end)
		return ;

	struct udphdr* uh = (struct udphdr*)(data + nh_off);
	if ((u8*)uh + sizeof(struct udphdr) <= data_end) {
		if (__my_htons(PROXY_PORT) == uh->dest && 
			get_gtpu_id(data, nh_off + sizeof(struct udphdr), data_end) < GTPU_InvalidTeId) {
			uh->dest = __my_htons(LOCAL_PORT);
			uh->check = 0;
		}
	}
}
__always_inline static void bpf_cp_change_udp_port_egress(u8 *data, u16 nh_off, u8 *data_end, u8 should_recheck)
{
	if ((u8*)(data + nh_off) > data_end)
		return ;

	struct udphdr* uh = (struct udphdr*)(data + nh_off);
	
	if ((u8*)uh + sizeof(struct udphdr) <= data_end) {		
		if (uh->source == __my_htons(LOCAL_PORT)&& 
			get_gtpu_id(data, nh_off + sizeof(struct udphdr), data_end) < GTPU_InvalidTeId) {
			uh->source = __my_htons(PROXY_PORT);
			if (should_recheck) {
				u32 check = (u32)uh->check;
				if (LOCAL_PORT > PROXY_PORT)
					check += (u32)((LOCAL_PORT - PROXY_PORT) << 8);
				else
					check += (u32)((PROXY_PORT - LOCAL_PORT) << 8);
				uh->check = (__u16)(check + (check >= 0xFFFF));
			}
		}
	}	
}

__always_inline static int do_change_udp_port(u8 *data, u8 *data_end, enDIRECT dir) 
{
	struct ethhdr *eth = (struct ethhdr *)data;
	int rc = TC_ACT_OK;
	u16 h_proto;
	u16 nh_off;
	u32 ipproto;
	u8 should_recheck = 0;
	nh_off = sizeof(*eth);
	if (data + nh_off > data_end)
		return rc;

	h_proto = eth->h_proto;

	if (h_proto == __my_htons(ETH_P_8021Q) || h_proto == __my_htons(ETH_P_8021AD)) {
		struct vlan_hdr *vhdr;

		vhdr = (struct vlan_hdr *)(data + nh_off);
		nh_off += sizeof(struct vlan_hdr);
		if (data + nh_off > data_end)
			return rc;
		h_proto = vhdr->h_vlan_encapsulated_proto;
	}
	if (h_proto == __my_htons(ETH_P_8021Q) || h_proto == __my_htons(ETH_P_8021AD)) {
		struct vlan_hdr *vhdr;

		vhdr = (struct vlan_hdr *)(data + nh_off);
		nh_off += sizeof(struct vlan_hdr);
		if (data + nh_off > data_end)
			return rc;
		h_proto = vhdr->h_vlan_encapsulated_proto;
	}
	if (h_proto == __my_htons(ETH_P_IP)) {
		ipproto = bpf_cp_parse_ipv4(data, &nh_off, data_end, &should_recheck);
	}
	else if (h_proto == __my_htons(ETH_P_IPV6)) {
		ipproto = bpf_cp_parse_ipv6(data, &nh_off, data_end);
	}
	else
		ipproto = 0;
	if (data + nh_off > data_end)
		return rc;
	if (ipproto == IPPROTO_UDP) {


		if (dir == enINGRESS) {
			bpf_cp_change_udp_port_ingress(data, nh_off, data_end);
		}
		else if (dir == enEGRESS) {
			bpf_cp_change_udp_port_egress(data, nh_off, data_end, should_recheck);
		}
		rc = TC_ACT_OK;
	}
	return rc;
}

SEC("tc_ingress")
int tc_ingress_func(struct __sk_buff *skb) 
{
    return do_change_udp_port((u8 *)(long)skb->data, (u8 *)(long)skb->data_end, enINGRESS);
}

SEC("tc_egress")
int tc_egress_func(struct __sk_buff *skb)
{
    return do_change_udp_port((u8 *)(long)skb->data, (u8 *)(long)skb->data_end, enEGRESS);
}

SEC("xdp_ingress")
int xdp_ingress_func(struct xdp_md *ctx)
{
	void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
	bpf_printk("call xdp_ingress_func\n");
	do_change_udp_port(data, data_end, enINGRESS);
	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
