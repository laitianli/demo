#include <stdio.h>
struct data
{
    int a    __attribute__( ( aligned ( 8 ) ) ) ;
    char ch  __attribute__( ( aligned ( 1 ) ) ) ;
    float s  __attribute__( ( aligned ( 4 ) ) ) ;    
} ; 
struct data e ;

float c[4] __attribute__((aligned(0x1000))) = {1.0, 2.0, 3.0, 4.0};
int main( )
{
    //example 1
    printf ( "&e.a = %u,&e.ch = %u,&e.s = %u\n", &e.a, &e.ch, &e.s ) ;
    printf ( "sizeof(struct data)=%d\n", sizeof ( e ) ) ;
    printf("sizeof(int)=%d,sizeof(char)=%d,sizeof(float)=%d\n",sizeof(int),sizeof(char),sizeof(float));
    struct data* p = &e;
    printf("[struct data] begin address:%p,[struct data] end address:%p\n",p,p+1);

    //example 2
    float a[4] __attribute__((aligned(0x1000))) = {1.0, 2.0, 3.0, 4.0};
    printf("a is temporary array:&a[0]=%p,&a[1]=%p,&a[2]=%p,&a[3]=%p\n", &a[0], &a[1], &a[2], &a[3]);

    //example 3
    static float b[4] __attribute__((aligned(0x1000))) = {1.0, 2.0, 3.0, 4.0};
    printf("b is static array: &b[0]=%p,&b[1]=%p,&b[2]=%p,&b[3]=%p\n",&b[0],&b[1],&b[2],&b[3]);

    //example 4
    printf("c is global array: &c[0]=%p,&c[1]=%p,&c[2]=%p,&c[3]=%p\n",&c[0],&c[1],&c[2],&c[3]);
    return 0 ;
}
