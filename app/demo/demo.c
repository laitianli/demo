#include <stdio.h>
#define MAX_ERRNO 4095
#define ECHILD  10
int main(int argv,char** argc)
{
    int sector_size = 512;
    int capacity = (16 * 1024 * 1024);

    char buf[8] = {0};

    buf[0] = (capacity >> 24) & 0xFF;
    buf[1] = (capacity >> 16) & 0xFF;
    buf[2] = (capacity >> 8) & 0xFF;
    buf[3] = capacity & 0xFF;

    int tmp_capacity = (buf[0] << 24) |     \
                       (buf[1] << 16) |     \
                       (buf[2] << 8) |      \
                       buf[3];

//    printf("tmp_capacity:%d",tmp_capacity);
//
    int size = 8 ,i = 0,result = 0;
    for(i = size; i < size*1280; i *= 2)
    {
        result = (i & ~(i-1)) - 1;
        printf("i=%d,result=%d\n",i,result);
    }
                        
    unsigned long devtag = 0x9000;
    unsigned long bus = (devtag >> 16) & 0xFF;
    unsigned long dev = (devtag >> 11 ) & 0xF;
    unsigned long fun = (devtag >> 8) & 0xF;
    printf("devtag=%d,bus=%d,dev=%d,fun=%d\n",devtag,bus,dev,fun);

    printf("\n-MAX_ERRNO=0x%x\n",(unsigned long)-MAX_ERRNO);
    printf("-ECHILD=0x%x\n",(unsigned long)-ECHILD);

	int aa[] = {1,2,3,4,5};
	int **x = (int**)&aa;
	printf("aa=%p,aa+1=%p\n",aa,*(x+1));
    return 0;
}
