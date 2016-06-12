#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>      
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define REDIRECT_CONTROL_NAME "/dev/redirect_control"
#define REDIRECT_SET_INFO 			('U'<<24 | 'R'<<16 | 'L' <<8 | 'A')
#define REDIRECT_GET_USER_COUNT  			(REDIRECT_SET_INFO + 1)
#define REDIRECT_GET_USER_DATA  			(REDIRECT_GET_USER_COUNT + 1)

struct redirect_info {
	unsigned long 	red_timeout;		/* redirect timeout */
	unsigned char   red_tag;			/* redirect tag */
	char		 	red_network[10];	/* redirect network */
	char 			red_url[4096];  	/* redirect url */	
};


struct redirect_user
{
	char mac[6];
	char user_agent[128];
};


struct redirect_user_ex {
	unsigned long 			count;
	struct redirect_user* 	user;	
};

int main(int argv, char **argc)
{
	int ret = 0;
	int fd = 0;
	fd = open(REDIRECT_CONTROL_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("[ERROR] open \"%s\" failed , error code=%d\n", REDIRECT_CONTROL_NAME, errno);
		return -1;
	}
	struct redirect_info info = {
		.red_timeout=2,
		.red_tag = 1,
		.red_network="eth12",
		.red_url = "www.qq.com"	
	};
//	ret = ioctl(fd, REDIRECT_SET_INFO, &info);


	int count = 0;
	ioctl(fd, REDIRECT_GET_USER_COUNT, &count);
	printf("sum count=%d\n", count);
	int size = count * sizeof(struct redirect_user);
 	struct redirect_user *p = malloc(size);

	ioctl(fd, REDIRECT_GET_USER_DATA, p);

	//printf("result count=%d \n", p->count);
	int i = 0;
	
	for(i = 0; i < count; i++)
	{
		printf("mac:%02x:%02x:%02x:%02x:%02x:%02x\n", p[i].mac[0], p[i].mac[1], (unsigned char)p[i].mac[2], (unsigned char)p[i].mac[3],  p[i].mac[4], p[i].mac[5]);
		printf("agent:%s\n", p[i].user_agent);
	}
	
	
	close(fd);
	return 0;
}


