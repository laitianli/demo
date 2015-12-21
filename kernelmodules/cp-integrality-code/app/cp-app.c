#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <poll.h>
#include "../module/cp.h"
#define Log(fmt,arg...) printf("[APP]=[%s:%d]="fmt"\n",__func__,__LINE__,##arg)
#define FILE_NAME "/proc/cp-info"
#define USER_BUF_SZ	4096
int open_cp_info(void)
{
	int fd = open(FILE_NAME,O_RDONLY);
	if(fd <= 0)
	{
		Log("[Error]=can not open %s file.\n",FILE_NAME);
		return ;
	}
	Log("fd=%d\n",fd);
}

void close_cp_info(int fd)
{
	close(fd);
}

int ioctl_cp_info(int fd)
{
	struct cp_info_user_buf user_buf = {0};
	void* buf = malloc(USER_BUF_SZ);
	if(!buf)
	{
		Log("[Error]=call malloc failed.");
		return -1;
	}	
	user_buf.buf = buf;
	user_buf.len = USER_BUF_SZ;
	int ret = ioctl(fd,SET_USERSPACE_BUF,&user_buf);
	if(ret < 0)
	{
		Log("[Error]=call SET_USERSPACE_BUF cmd failed.");
		return -1;
	}
	return 0;
}

void read_cp_info(int fd)
{
	int ret = 0;
	struct pollfd fds[1] = {0};
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	while(1)
	{
		ret = poll(fds,1,1000);
		if(ret == -1)
		{
			Log("[Error]=[%s:%d]=poll failed.\n",__func__,__LINE__);
			return ;
		}
		if(ret == 0)
		{
			Log("timeout\n");
			continue;
		}
		else if (ret == 1)
		{
			Log("begin to read .\n");
		}
	}
}
int main(int argc,char** argv)
{
	int fd = open_cp_info();
	if(fd <= 2)
		return -1;
	int ret = ioctl_cp_info(fd);
	if(ret < 0)
		return -1;
	read_cp_info(fd);
	close_cp_info(fd);
	return 0;
}
