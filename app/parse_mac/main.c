#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILTER_SIZE 8
struct ether_mac {
    unsigned char mac[6];
};


struct ether_mac  eths[MAX_FILTER_SIZE] = {0};

static int check_is_mac(const char* mac, unsigned char *a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e, unsigned char* f)
{
    int len = sscanf(mac, "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX", a, b, c, d, e, f);
    if(len != 6) {
        printf("[Error]| %s | (len:%d) is not mac address.\n", mac, len);
        return -1;
    }
    return 0;
}

static int parse_mac(const char* val, void* arg)
{
    char *str_mac = strdup(val);
    if (!str_mac) {
        printf("[Error] strdup error\n");
        return -1;
    }
    struct ether_mac* ether = (struct ether_mac*)arg;
    char* p = NULL;
    char* cur = str_mac;
    int ret = 0;
    int i = 0;
    while (cur && (p = strstr(cur, ","))) {
        *p = '\0';
        ret = check_is_mac(cur, &ether[i].mac[0], &ether[i].mac[1], &ether[i].mac[2], &ether[i].mac[3], &ether[i].mac[4], &ether[i].mac[5]);
        if(!ret) {
            i++;
        }
        cur = p + 1;
    }

    if (cur && *cur != '\0') {
        ret = check_is_mac(cur, &ether[i].mac[0], &ether[i].mac[1], &ether[i].mac[2], &ether[i].mac[3], &ether[i].mac[4], &ether[i].mac[5]);
        if(!ret) {
            i++;
        }
    } 

    free(str_mac);
    return i;
}

void dump_ether_mac(struct ether_mac* eth, int size)
{
    int i = 0;
    printf("mac: \n");
    for (i = 0; i < size; i ++) {
        printf("\t%02x:%02x:%02x:%02x:%02x:%02x\n", eth[i].mac[0], eth[i].mac[1], eth[i].mac[2], eth[i].mac[3], eth[i].mac[4], eth[i].mac[5]);    
    }
}

int main(int argv, char** argc)
{
    char* macs = "00:12:13:aa:cd:01,00:12:13:aa:cd:ba,23:24,00:12:13:aa:cd:de,00:12:13:aa:cd:02,";
    printf("macs: %s\n", macs);
    int ret = 0;
    memset(eths, 0, sizeof(eths));
    ret = parse_mac(macs, (void*)eths);
    if(ret > 0) {
        
    }
    dump_ether_mac(eths, ret); 
    return 0;
}
