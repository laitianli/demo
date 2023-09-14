#include <arpa/inet.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
//#include <linux/if_vlan.h>
#include <linux/ip.h>
struct vlan_hdr {
	__be16	h_vlan_TCI;
	__be16	h_vlan_encapsulated_proto;
};
#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))
#define __my_htons ___constant_swab16
static inline int parse_ipv4(void *data, __u32 nh_off, void *data_end, __be32 *src, __be32 *dest)
{
    struct iphdr *iph = (struct iphdr *)((char* )data + nh_off);

    *src = iph->saddr;
    *dest = iph->daddr;
    return iph->protocol;
}

SEC("prog")
int xdp_count_prog(struct xdp_md *ctx)
{
	void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    struct ethhdr *eth = data;
    __be32 dest_ip = 0, src_ip = 0;
    __u16 h_proto;
    __u32 nh_off;
    int ipproto;

    nh_off = sizeof(*eth);

    /* parse vlan */
    h_proto = eth->h_proto;
    if (h_proto == __my_htons(ETH_P_8021Q) ||
        h_proto == __my_htons(ETH_P_8021AD)) {
        struct vlan_hdr *vhdr;

        vhdr = (struct vlan_hdr *)((char*)data + nh_off);
        nh_off += sizeof(struct vlan_hdr);
        h_proto = vhdr->h_vlan_encapsulated_proto;
    }
    if (h_proto != __my_htons(ETH_P_IP))
        goto pass;

    ipproto = parse_ipv4(data, nh_off, data_end, &src_ip, &dest_ip);
#if 1
    if (dest_ip == __my_htons(0))
        return XDP_DROP;
#endif	
pass:
    return XDP_PASS;

}

char _license[] SEC("license") = "GPL";