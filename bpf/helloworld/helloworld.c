#include <linux/bpf.h>
#include <linux/pkt_cls.h>

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


SEC("xdp")
int xdp_drop_the_world(struct xdp_md *ctx) {
    // drop everything
  // 意思是无论什么网络数据包，都drop丢弃掉
    return XDP_DROP;
}


SEC("tc")
int tc_drop_the_world() {struct __sk_buff *skb
    // drop everything
  // 意思是无论什么网络数据包，都drop丢弃掉
    return TC_ACT_SHOT;
}

char _license[] SEC("license") = "GPL";