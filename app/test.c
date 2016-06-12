#define PAGE_SHIFT 12
#define VSYSCALL_END (-2UL << 20) /* 0xffe00000 */
#define VSYSCALL_START (-10UL << 20) /* 0xff600000 */
#define VSYSCALL_FIRST_CALL (((VSYSCALL_END-VSYSCALL_START) >> PAGE_SHIFT) - 1) /*0x7FF*/
#include <stdio.h>


void print_buf(char* tag, unsigned char* buf, int len)
{
	int ii = 0;
	printf("\n------------------%s-----------------len=%d\n",tag, len);
	for(ii = 0; ii < len; ii++)
	{
		printf("aaaaaaaaaa\n");		//printf("%02X %s", buf[ii], (((ii+1)%16==0) ? "\n":" "));
	}
}

int main(int argv,char** argc)
{
#if 1
     char a = 0xb6;
     short b = 0xb600;
    int c = 0xb6000000;

    printf("a = %d\n",a);
    printf("(int)a = 0x%x\n",a);
    if ( a == 0xb6)  printf("a\n");   
    if ( b == 0xb600) printf("b\n");  
    if ( c == 0xb6000000) printf("c\n");  
#else
     char a = 0x6;
     short b = 0x600;
    int c = 0x6000000;
    if ( a == 0x6)  printf("a\n");   
    if ( b == 0x600) printf("b\n");  
    if ( c == 0x6000000) printf("c\n");  
#endif
	printf("VSYSCALL_END(-2UL << 20):0x%x\n", VSYSCALL_END);
	printf("VSYSCALL_START(-10UL << 20): 0x%x\n", VSYSCALL_START);
	printf("VSYSCALL_FIRST_CALL: 0x%x\n", VSYSCALL_FIRST_CALL);

	unsigned char aa = 0xFF, bb = 0;

	printf("0xFF+1=%02X\n", bb = aa += 1);
	printf("bb=%02X\n", bb -= (unsigned char)1);

	unsigned char buffer[12] = {1};
	buffer[0] = 1;
	buffer[1] = 2;
	buffer[2] = 3;
	memset(&buffer[3], 1, sizeof(buffer)-3);
	print_buf("aaaaabefore", buffer, sizeof(buffer));

	float fa = 10245.6;
	float ftime=100;

	printf("(float/float) %f/%f=%5.2f\n", fa, ftime, fa / ftime);
return 0;
}
