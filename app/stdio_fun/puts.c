#include <unistd.h>
#include <stdio.h>
#if 0
#define MY_STR	"aaaaaaaaaaa"
void my_puts()
{
	puts(MY_STR);
}

void my_print(int a, int b)
{
	printf("a=%d, b=%d\n", a, b);
}
int my_return(int aa, int bb)
{
	int len = printf("aa=%d, bb=%d\n",aa,bb);
	return len;
}
#endif

int fun_add(int a,int b)
{
	return a + b;
}
int main(void)
{
//	int len = 0;
	int result = 0;
	//int sum = 0;
	//my_puts();
//	my_print(3, 5);
//	len = my_return(22, 696);
	result = fun_add(3,5);
	//sum += len;
	//printf("sum =%d\n",sum);
	return 0;
}
