#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h> 
#include <asm/types.h> 
#include <linux/netlink.h>  
#include <linux/socket.h>  

#define DEMO_NETLINK_ID		29
#define MAX_PAYLOAD 1024  /* maximum payload size*/  

static int g_sock_fd = 0;
struct nlmsghdr *nlh = NULL;  
struct iovec *iov = NULL; 
struct msghdr *msg = NULL;
int main(int argv, char **argc)
{
	int ret = 0;
	int curr_pid = getpid();
	printf("[Info] curr_pid=%d\n", curr_pid);
	g_sock_fd = socket(PF_NETLINK, SOCK_RAW, DEMO_NETLINK_ID);
	if (g_sock_fd == -1)
	{
		printf("[error] create socket failed, error code = %d\n", errno);
		goto EXIT;
	}
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	if (!nlh)
	{
		printf("[Error] malloc memery failed.\n");
		goto EXIT;
	}
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

	struct sockaddr_nl src_addr={0}, dest_addr={0};  
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = curr_pid;
	src_addr.nl_groups = 0;
	ret = bind(g_sock_fd, (const struct sockaddr *)&src_addr, sizeof(struct sockaddr_nl));
	if (ret == -1)
	{
		printf("[error] bind socket failed, error code = %d\n", errno);
		goto EXIT;
	}

	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid	= 0;
	dest_addr.nl_groups = 0;
	
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = curr_pid;
	nlh->nlmsg_flags = 0;

	iov = (struct iovec*)malloc(sizeof(struct iovec));
	memset(iov, 0, sizeof(iov));
	iov->iov_base = &nlh;
	iov->iov_len = nlh->nlmsg_len;

	msg = (struct msghdr*)malloc(sizeof(struct msghdr));
	memset(msg, 0, sizeof(struct msghdr));
	msg->msg_name = (void*)&dest_addr;
	msg->msg_namelen = sizeof(struct sockaddr_nl);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;

	ret = sendmsg(g_sock_fd, msg, 0);
	if(ret <= 0)
	{
		printf("[error] sendmsg data failed, error code = %d\n", errno);
		goto EXIT;
	}
	printf("sendmsg data len = %d\n", ret);
EXIT:
	nlh ? free(nlh) : NULL;
	iov ? free(iov) : NULL;
	msg ? free(msg) : NULL;
	close(g_sock_fd);
	return 0;
}


