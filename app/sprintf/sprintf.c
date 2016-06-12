#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
char g_patten[128] = {0};
int main(void)
{
	g_patten[0] = 'A';
	int len = sprintf(g_patten,"-:%d:%s**",getpid(),"sprintf");
	printf("len=%d,g_patten=%s\n",len,g_patten);
	return 0;
}
