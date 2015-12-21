#include <unistd.h>
#include <stdio.h>
#include <sys/vfs.h>
#define PATH_NAME "/sys/kernel/debug"
int main()
{
	struct statfs buf = {0};
	int ret = statfs(PATH_NAME,&buf);
	if(ret != 0)
	{
		printf("statfs error\n");
		return -1;
	}
	printf("buf->f_type:0x%x\n",buf.f_type);
	return 0;
}
