#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int aa = 6;
	int bb = 15;
	int result = 0;
	 __asm__("cmp %1, %2;"
			 "jge greater;"
			 "movl %1, %0;"
			 "jmp end;"
			 "greater: ;"
			 "movl %2, %0;"
			 "end:"
			 : "=r"(result)
			 : "r"(aa), "r"(bb)
			 );

	 printf("cmp command result is %d\n", result);
	return 0;
}
