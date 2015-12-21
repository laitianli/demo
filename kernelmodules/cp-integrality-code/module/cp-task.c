#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/err.h>
#include <linux/syscalls.h>
#include "cp.h"
extern struct cp_info g_info;
static char src_file[256] = "/proc/cmdline";
static char dst_file[256] = PROC_NAME;

static int do_open_file(const char* filename,int flags,int mode)
{

	// return sys_open(filename,flags,mode);
}

static int do_close_file(unsigned int fd)
{
	return sys_close(fd);
}

static int do_read_file(unsigned int fd, char __user *buf, size_t count)
{
	int ret = 0;
	return ret;
}

static int do_write_file(unsigned int fd, char __user *buf,size_t count)
{
	//return sys_write(fd,buf,count);
	return 0;
}

void begin_cp_task(void)
{
	int sfd = do_open_file(src_file,O_RDONLY,0444);
	Log("sfd=%d",sfd);
	if(sfd <= 0)
	{
		Log("[Error]=file:%s open failed.",src_file);
		return 0;
	}

	int size = do_read_file(sfd,g_info.user_buf.buf,g_info.user_buf.len);
	Log("read size=%d",size);
	do_close_file(sfd);

	int dfd=do_open_file(dst_file,O_RDWR,0777);
	Log("dfd=%d",dfd);
	if(dfd <= 0)
	{
		Log("[Error]=file:%s open failed.",dst_file);
		return 0;
	}
	size = do_write_file(dfd, g_info.user_buf.buf,size);
	Log("write size=%d",size);
	do_close_file(dfd);
}

void stop_cp_task(void)
{
}
