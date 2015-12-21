#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int dividend = 20;
	int dividor = 5;
	int result = 0;

	__asm__("divb %2;"
			"movl %%eax, %0;"
			:"=m"(result) 
			:"a"(dividend),"g"(dividor));

	printf("divb result is %d\n",result);
	return 0;
}
