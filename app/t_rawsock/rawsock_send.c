#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>

/*-----------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------*/

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#define TAP_SRC_IP_ADDR "10.88.120.23" 
//#define TAP_DST_IP_ADDR "10.88.120.205" 
#define TAP_DST_IP_ADDR "127.0.0.1" 
#define ETHER_HDR_LEN1  14
#define IP_HDR_LEN      20
#define UDP_HDR_LEN     8
#define SRS_TOTAL_BUFFER_SIZE (SRS_INFO_SIZE*SRS_INFO_COUNT_MAX + 200)//200 for sizeof(MacSrsInd_t)+sizeof(MacSrsNumerology_t)
#define MAX_ULSCH_STORED     100
#define ETH_ALEN 6
uint8_t* phySrsBufferPtr = NULL;
uint8_t ulschIndBuffer[10000] = {0};
uint8_t numOfUlschStored = 0;
int enableDelayUlsch = 0;

/*-----------------------------------------------------------------------------
// Type Declarations
//---------------------------------------------------------------------------*/
struct ethhdr1
{
   uint8_t  ether_dhost[ETH_ALEN];
   uint8_t  ether_shost[ETH_ALEN];
   uint16_t ether_type;
} __attribute__((packed));

struct iphdr1
{
   uint8_t  ver_hl;     /* version and header length default 0x45*/
   uint8_t  tos;        /* type of service */
   uint16_t len;        /* total length */
   uint16_t id;         /* identification */
   uint16_t offset;     /* fragment offset */
   uint8_t  ttl;        /* time to live */
   uint8_t  protocol;   /* protocol */
   uint16_t checksum;   /* checksum */
   uint32_t src;        /* source IP address */
   uint32_t dst;        /* destination IP address */
} __attribute__ ((packed));


/*UDP header*/
struct udphdr1
{
   uint16_t   source_port;      /* source port */
   uint16_t   dest_port;        /* dest port */
   uint16_t   len;              /* length of packet */
   uint16_t   checksum;         /* checsum */
} __attribute__ ((packed));

typedef enum
{
    MAC_PHY_CRC_INDICATION,
    MAC_PHY_RACH_INDICATION,
    MAC_PHY_RX_ULSCH_INDICATION,
    MAC_PHY_RX_SR_INDICATION,
    MAC_PHY_HARQ_INDICATION,
    MAC_PHY_RX_CSI_INDICATION,
    TOTAL_MAC_PHY_INDICATION
} MacPhyIndMsg_e;

/*
//|-----------------------------------------------------------------------------
//|Global Variables
//|-----------------------------------------------------------------------------
*/
uint32_t dst_ipaddr = -1;
int isFirstSlotIndRecvd = 0;
uint16_t recvSlotIndCount = 0;
uint8_t UL_SNR_0 = 128;

/*
//|-----------------------------------------------------------------------------
//| Local variables
//|-----------------------------------------------------------------------------
*/
static uint8_t tapBuffer[65535] = {0};
static uint32_t local_ipaddr = -1;
static int raw_sock = -1;
static uint8_t phyIndBuffer[TOTAL_MAC_PHY_INDICATION][10000] = {0};
/* UL redundancy Version */
static uint8_t ulRv[4] = {0, 2, 3, 1};

static uint16_t delayRound = 5;
static uint32_t maxDelayUs;
static uint32_t delayUsPerRound;

/*
//|-----------------------------------------------------------------------------
//| Function definitions
//|-----------------------------------------------------------------------------
*/

/*
 * udp fapi function start
 */
static unsigned short int csum_fold(unsigned int sum)
{
    while ( sum >> 16 ) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}

static unsigned short ip_fast_csum(uint8_t *iph, unsigned int ihl)
{
   unsigned int *word = (unsigned int *) iph;
   unsigned int *stop = word + ihl;
   unsigned int csum;
   int carry;

   csum = word[0];
   csum += word[1];
   carry = (csum < word[1]);
   csum += carry;

   csum += word[2];
   carry = (csum < word[2]);
   csum += carry;

   csum += word[3];
   carry = (csum < word[3]);
   csum += carry;

   word += 4;
   do {
           csum += *word;
           carry = (csum < *word);
           csum += carry;
           word++;
   } while (word != stop);

   return csum_fold(csum);
}

static uint8_t *build_ip_header(uint8_t *ptr, uint32_t src_ip, uint32_t dst_ip, uint16_t len)
{
    struct iphdr1* iphdr = (struct iphdr1*)ptr;
    iphdr->ver_hl = 0x45;
    iphdr->tos = 0;
    iphdr->len = htons(len);
    iphdr->id = 0;
    iphdr->offset = 0x0000;
    iphdr->ttl = 0xff;
    iphdr->protocol = 0x11;
    iphdr->checksum = 0x0000;
    iphdr->src = htonl(src_ip);
    iphdr->dst = htonl(dst_ip);

    iphdr->checksum = ip_fast_csum(ptr, 5);

    return (uint8_t*)(iphdr + 1);
}

static uint8_t *build_udp_header(uint8_t *ptr, uint16_t src_port, uint16_t dst_port, uint16_t len)
{
    struct udphdr1 *udphdr = (struct udphdr1*)ptr;

    udphdr->source_port = htons(src_port);
    udphdr->dest_port = htons(dst_port);
    udphdr->len = htons(len);
    udphdr->checksum = 0x0000;

    return (uint8_t*)(udphdr + 1);
}

int open_raw_sock(const char* src_ip, const char* dst_ip)
{
    raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1)
        return -1;

    char on = 1;
    if (setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
    {
        raw_sock = -1;
        return -2;
    }

    local_ipaddr = ntohl((uint32_t)inet_addr(src_ip));
    dst_ipaddr = ntohl((uint32_t)inet_addr(dst_ip));

    return 0;
}

/*
 * open tap for packet received from PTR_channel
 */
int open_tap()
{
    char *env_src_addr = getenv("TAP_SRC_IP_ADDR");
    char *env_dst_addr = getenv("TAP_DST_IP_ADDR");

    const char* src_ip = (env_src_addr != NULL) ? env_src_addr : TAP_SRC_IP_ADDR;
    const char* dst_ip = (env_dst_addr != NULL) ? env_dst_addr : TAP_DST_IP_ADDR;

    if ((src_ip != NULL) && (dst_ip != NULL))
    {
        int ret = open_raw_sock(src_ip, dst_ip);

        if (ret == 0)
        {
            printf("open_raw_sock success(ret=%d), src_ip = %s, dst_ip = %s\n", ret, src_ip, dst_ip);
        }
        else
        {
            printf("open_raw_sock failed(ret=%d), src_ip = %s, dst_ip = %s\n", ret, src_ip, dst_ip);
        }
    }

    return 0;
}

/*
 * close the tap
 */
int close_tap()
{
    if (raw_sock != -1) {
        close(raw_sock);
        raw_sock = -1;
    }
    return 0;
}

static inline int getTapDesPort(void)
{
	return 99999;
}

void do_tap_fun_fragment(void* data, uint32_t len, void* headerData, uint32_t headerLen)
{
    uint32_t total_len = len + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN;
    uint32_t cell_index = 0; //AddressRegistryPtr->getL2ProcessorInstance() - 1;
    uint32_t src_ip = local_ipaddr + cell_index;//SRC_IP + cell_index;
    uint16_t src_port = 40001 + cell_index;
    uint32_t buf_offset = 0;
    uint32_t data_offset = 0;
    static uint32_t id = 666;
    const uint32_t fragmentSize = 1472; 
    uint16_t iphdroffset = 0;
    const uint16_t iphdroffsetSize = 185;
    const uint16_t iphdroffsetSize1 = 184;
    uint8_t fragmentNum = (len + headerLen) / fragmentSize;
    uint32_t LastfragmentLen = (len + headerLen) % fragmentSize;

    //frist fragment with ip header and udp header
    total_len = fragmentSize + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN;
    uint8_t* ptr = build_ip_header(tapBuffer+ETHER_HDR_LEN1, src_ip, dst_ipaddr, total_len - ETHER_HDR_LEN1);
    build_udp_header(ptr, src_port, getTapDesPort(), len + headerLen + UDP_HDR_LEN);
    if(headerLen != 0)
    {
        memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN, headerData, headerLen);
        memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN + headerLen, data, (fragmentSize -headerLen));
    }
    else
    {
    memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN, data, fragmentSize);
    }
    buf_offset = ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN + fragmentSize;
    data_offset += (fragmentSize - headerLen);

    //other fragment with ip header
    while(fragmentNum)
    {
        fragmentNum--;
        if(fragmentNum != 0)
        {
            total_len = fragmentSize + ETHER_HDR_LEN1 + IP_HDR_LEN;
        }
        else
        {
            total_len = LastfragmentLen + ETHER_HDR_LEN1 + IP_HDR_LEN;
        }
        ptr = build_ip_header(tapBuffer + ETHER_HDR_LEN1 + buf_offset, src_ip, dst_ipaddr, total_len - ETHER_HDR_LEN1);
        memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + buf_offset, 
                ((uint8_t*)data + data_offset), 
                (total_len - ETHER_HDR_LEN1 - IP_HDR_LEN));
        if(fragmentNum != 0)
        {
            buf_offset += fragmentSize + ETHER_HDR_LEN1 + IP_HDR_LEN;
            data_offset += fragmentSize;
        }
    }

    //send to wireshark
    if (raw_sock != -1) {
        fragmentNum = (len + headerLen) / fragmentSize;
        struct sockaddr_in addr;
        addr.sin_addr.s_addr = htonl(dst_ipaddr);
        addr.sin_family = AF_INET;
        addr.sin_port = ntohs(getTapDesPort());

        //frist fragment
        struct iphdr1* iphdr = (struct iphdr1*)(tapBuffer+ETHER_HDR_LEN1);
        iphdr->id = id;
        iphdr->offset = htons(0x2000);
        iphdr->checksum = ip_fast_csum((uint8_t*)iphdr, 5);
        sendto(raw_sock, (void*)iphdr, ntohs(iphdr->len), 0, (struct sockaddr *)&addr, sizeof(addr));
        iphdroffset += iphdroffsetSize;
        buf_offset = ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN + fragmentSize;

        //other fragment size == fragmentSize
        while(fragmentNum)
        {
            fragmentNum--;
            iphdr = (struct iphdr1*)(tapBuffer + ETHER_HDR_LEN1 + buf_offset);
            iphdr->id = id;
            iphdr->offset = htons(iphdroffset);
            if(fragmentNum != 0)
            {
                iphdr->offset |= htons(0x2000);
            }
            iphdr->checksum = ip_fast_csum((uint8_t*)iphdr, 5);
            sendto(raw_sock, (void*)iphdr, ntohs(iphdr->len), 0, (struct sockaddr *)&addr, sizeof(addr));
            iphdroffset += iphdroffsetSize1;
            buf_offset += fragmentSize + ETHER_HDR_LEN1 + IP_HDR_LEN;
        }
    }
}

static void do_tap_fun_socket(void* data, uint32_t len,void* headerData, uint32_t headerLen)
{
    //const uint32_t fragmentSize = 1472;
    const uint32_t fragmentSize = 5000;
    if ((len + headerLen) > fragmentSize)
    {
		printf("[Note][%s:%d] len + headerLen: %d, fragmentSize: %d\n", __func__, __LINE__, len + headerLen, fragmentSize);
        if((len + headerLen) < 60000)
        {
            do_tap_fun_fragment(data, len, headerData, headerLen);
        }
        else
        {
            do_tap_fun_fragment(data, 60000, headerData, headerLen);
        }
        return;
    }

    uint32_t total_len = len + headerLen + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN;
    uint32_t cell_index =0;// AddressRegistryPtr->getL2ProcessorInstance() - 1;
    uint32_t src_ip = local_ipaddr + cell_index;//SRC_IP + cell_index;
    uint16_t src_port = 40001 + cell_index;

    uint8_t* ptr = build_ip_header(tapBuffer+ETHER_HDR_LEN1, src_ip, dst_ipaddr, total_len - ETHER_HDR_LEN1);
    build_udp_header(ptr, src_port, getTapDesPort(), total_len - ETHER_HDR_LEN1 - IP_HDR_LEN);
    if(headerLen != 0)
    {
        memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN, headerData, headerLen);
        memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN + headerLen, data, len);
    }
    else
    {
    memcpy(tapBuffer + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN, data, len);
    }
    if (raw_sock != -1) {
        struct iphdr1* iphdr = (struct iphdr1*)(tapBuffer+ETHER_HDR_LEN1);
        //iphdr->src = local_ipaddr;
        //iphdr->dst = dst_ipaddr;

        iphdr->checksum = ip_fast_csum((uint8_t*)iphdr, 5);

        struct sockaddr_in addr;
        addr.sin_addr.s_addr = htonl(dst_ipaddr);
        addr.sin_family = AF_INET;
        addr.sin_port = ntohs(getTapDesPort());
        int rt = sendto(raw_sock, (void*)iphdr, ntohs(iphdr->len), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (rt < 0)
        {
            printf("tap send failed. len:%u rt:%d errno_%u_%s \n", len,rt, errno, strerror(errno));
        }
    }
}

void do_tap_fun(void* data, uint32_t len,void* headerData, uint32_t headerLen)
{
	do_tap_fun_socket(data, len, headerData, headerLen);
}


int main(int argv, char** argc)
{
	if (argv < 3) {
		printf("please input datalen and count!!\n");
		printf("\t%s <datalen> <count>\n", argc[0]);
		printf("\teg: %s 2000 10\n", argc[0]);
		return -1;
	}
	int data_len = atoi(argc[1]);
	int count = atoi(argc[2]);
	int i = 0;
	unsigned char headerData[32] = {0};
	unsigned char data[6000] = {0};
	for (i = 0; i < data_len && i < sizeof(data); i++) {
		data[i] = i % 255;
	}
	
	open_tap();
	
	for (i = 0; i < count; i++) {
		do_tap_fun(data, data_len, headerData, sizeof(headerData));
		sleep(1);		
	}
	
	close_tap();
	return 0;
}
