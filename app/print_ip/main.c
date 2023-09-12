#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void print_ip(unsigned int ip4)
{
    printf("ip: 0x%x (%d.%d.%d.%d)\n", ip4, ip4 & 0xFF, (ip4 & 0xFF00) >> 8, (ip4 & 0xFF0000) >> 16, (ip4 & 0xFF000000) >> 24);
}

int main(int argv, char** argc)
{
    if (argv >= 2 && argc[1]) {
        unsigned int ip = strtol(argc[1], NULL, 16);
        print_ip(ip);
    }
    return 0;
}
