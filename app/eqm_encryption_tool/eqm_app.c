#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define PAGE_SIZE	(1UL << 12)
#define EQM_ENCRYPTION_DEVICE "/dev/eqm-encryption"
#define EQM_DECRYPTION_DEVICE "/dev/eqm-decryption"

pthread_t encryption_pid;
pthread_t decryption_pid;

/* 需要映射到用户空间的数据长度 */
#define MISC_EQM_GET_DATA_LENGTH	('E' << 16 | 'Q' << 8 | 'A')

/**ltl
 * 功能:加密算法接口
 * 参数:
 * 返回值:
 * 说明: 对所有数据+1
 */
static char* encryption(unsigned char *buf, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
 		buf[i] += 1;

	return buf;
}

/**ltl
 * 功能:解密算法接口
 * 参数:
 * 返回值:
 * 说明: 对所有数据-1
 */
static char* decryption(unsigned char *buf, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
		buf[i] -= 1;
	return buf;
}

void print_buf(unsigned char* buf, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
		printf("%02x ", buf[i]);
}

#define PBuf(STR, COUNT, buf, len) \
	do {\
		static int i = 0;\
		if(i++ < COUNT)\
		{	printf("%s:\n", STR); \
			if(len > 0)print_buf(buf, len);\
			}\
	}while(0)

void* encryption_thread(void* data)
{
	unsigned char* addr = NULL;
	int ret = 0;
	int mmap_size = 0 ;
	int fd = open(EQM_ENCRYPTION_DEVICE, O_RDWR);
	if (fd <= 0)
	{
		printf("[error] open file [%s] failed.\n", EQM_ENCRYPTION_DEVICE);
		return 0;
	}

	struct pollfd fds[1] = {fd, POLLIN, 0};

	do
	{
		ret = poll(fds, 1, 3000);
		if(ret < 0)
		{
			printf("[error]=poll error.\n");
			return 0;
		} else if(ret == 0) {
			printf("encryption thread poll timeout.\n");
			continue;
		}
		else if(ret > 0) {			
			mmap_size = 0;

			ret = ioctl(fd, MISC_EQM_GET_DATA_LENGTH, &mmap_size);
			//printf("mmap_size:%d\n", mmap_size);
			addr =(unsigned char*)mmap(0, mmap_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

			encryption(addr, mmap_size);

			fsync(fd);

			munmap(addr, mmap_size);

			continue;
		}
	}while(1);

	close(fd);
	return NULL;
}


void* decryption_thread(void* data)
{
	unsigned char* addr = NULL;
	int ret = 0;
	int mmap_size = 0 ;
	int fd = open(EQM_DECRYPTION_DEVICE, O_RDWR);
	if (fd <= 0)
	{
		printf("[error] open file [%s] failed.\n", EQM_DECRYPTION_DEVICE);
		return 0;
	}

	struct pollfd fds[1] = {fd, POLLIN, 0};

	do
	{
		ret = poll(fds, 1, 3000);
		if(ret < 0)
		{
			printf("[error]=poll error.\n");
			return 0;
		} else if(ret == 0) {
			printf("decryption thread poll timeout.\n");
			continue;
		}
		else if(ret > 0) {			
			mmap_size = 0;

			ret = ioctl(fd, MISC_EQM_GET_DATA_LENGTH, &mmap_size);
			//printf("mmap_size:%d\n", mmap_size);
			addr =(unsigned char*)mmap(0, mmap_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

			decryption(addr, mmap_size);

			fsync(fd);

			munmap(addr, mmap_size);

			continue;
		}
	}while(1);

	close(fd);
	return NULL;
}
			

int main(int argc, char** argv)
{
	int ret = 0;
	ret = pthread_create(&encryption_pid, NULL, encryption_thread, NULL);
	if(ret) {
		printf("[Error] pthread_create encryption failed.\n");
		return -1;
	}

	ret = pthread_create(&decryption_pid, NULL, decryption_thread, NULL);
	if(ret) {
		printf("[Error] pthread_create decryption failed.\n");
		return -1;
	}

	/* 查看event_sd.c */
	pthread_join(encryption_pid, NULL);
	pthread_join(decryption_pid, NULL);

	return 0;
}


