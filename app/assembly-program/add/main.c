#include <stdio.h>

int main(void)
{
	int data1 = 10;
	int result = 20;
#if 0
	__asm__("movl %1,%%eax\n\t"
			"addl %%eax,%0"
			:"=r"(result)
			:"r"(data1),"0"(result)
			:"%eax");
#else
	__asm__("addl %1,%0"
			:"=r"(result)
			:"r"(data1),"a"(result));
#endif
	printf("the result is %d \n",result);
	return 0;
}
