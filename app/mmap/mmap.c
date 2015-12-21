#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_SIZE 1024
#define FILE_NAME "/tmp/mmap.txt"
#define STRING "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n"
static int file_size = 0;
int create_tmp_file(void)
{
	int fd = open(FILE_NAME, O_CREAT | O_RDWR);
	if (fd < 0)
	{
		printf("create file: %s failed.\n", FILE_NAME);
		return -1;
	}

	size_t sz = write(fd,STRING,strlen(STRING));
	file_size = sz;
	printf("fd = %d, write size = %d\n",fd ,sz);

	close(fd);
	return 0;
}

int mmap_file()
{
	int fd = open(FILE_NAME,O_RDWR);
	if (fd < 0)
	{
		printf("open file: %s failed.\n", FILE_NAME);
		return -1;
	}	
//	char* addr = mmap(0, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	char* addr = mmap(0, file_size, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd, 0);
	memset(addr, 'b', 5);
	printf("file addr = %s, strlen=%d\n", addr, strlen(addr));

 //   int ret = msync(addr,file_size,MS_SYNC | MS_INVALIDATE);
//	printf("ret=%d\n",ret);
	munmap(addr, file_size);
	return 0;
}

int main(void)
{
	int ret = 0;
	ret = create_tmp_file();
	 mmap_file();
	
}
