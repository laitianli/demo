#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
void hex_print(const unsigned char* buf, int len)
{
    int i = 0, j = 0;
    const unsigned char* ch = buf;
    unsigned int s = 0;
    unsigned int nshorts = len / sizeof(unsigned short);
    if (!buf || len <= 0) {
        printf("[error] argument error!\n");
        return ;
    }
    while (nshorts != 0) {
        if ((i++ % 8) == 0) {
            printf("\n");
            printf("0x%04x: ", j);
            j += 16;
        }
        s = *((unsigned char*)ch);
        ch++;
        printf(" %02x%02x", s, *(unsigned char*)ch);
        ch++;
        nshorts--;
    }

    if (len & 1) {
        if ((i % 8) == 0)
            printf("%04x: ", j);

        printf(" %02x", *(unsigned char*)ch);
    }
    printf("\n");
}

int read_binary(const char* file_name, unsigned char* buf, int size)
{
    int fd = open(file_name, O_RDONLY);
    if (fd < 0) {
        printf("[error] open file:%s failed. errorcode=%d\n", file_name, errno);
        return -1;
    }
    int len = read(fd, buf, size);
    if(len <= 0) {
        printf("[error] read file:%s failed. errorcode=%d\n", file_name, errno);
        return -1;
    }
    close(fd);
    return len;
}

int main(int argv, char **argc)
{
    #define BUF_SIZE 1024
    if (argv < 3) {
        printf("[error] Please input buflen and file_name to hexdump!!!\n");
        printf("\t eg: ./t_main 134 <hexdump_file>\n");
        return -1;
    }
    int buflen = atoi(argc[1]);
    if (buflen <= 0) {
        printf("[error] Please input correct buflen to hexdump!!!\n");
        return -1;
    }
    const char* file_name = argc[2];
    if (!file_name || !strlen(file_name)) {
        printf("[error] please input file name to hexdump!\n");
        return -1;
    }
    unsigned char *buf = malloc(buflen);
    if (!buf) {
        printf("[error] malloc %d buf failed!\n", buflen);
        return -1;
    }
    int readlen = read_binary(file_name, buf, buflen);
    if (readlen < 0) {
        return -1;
    }
    hex_print(buf, readlen);

    free(buf);
    return 0;
}
