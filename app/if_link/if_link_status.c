#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <errno.h>
#define BUFLEN  4096

int if_link_status(const char *if_name)
{
    int fd = 0;
    struct sockaddr_nl sa = {0};

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (fd < 0) {
        printf("[Error] socket error!, errorcode:%d\n", errno);
        return -1;
    }

    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTNLGRP_LINK; /* 查询网卡状态 */
    int ret = bind(fd, (struct sockaddr*)&sa, sizeof(sa));
    if (ret) {
        printf("[Error] bind error!, errorcode: %d\n", errno);
        close(fd);
        return -1;
    }

    char buf[BUFLEN] = {0};
    struct nlmsghdr* nh = NULL;
    struct ifinfomsg* ifinfo = NULL;
    struct rtattr* attr = NULL;
    int len = 0;
    while ((ret = read(fd, buf, sizeof(buf))) > 0) {
        for (nh = (struct nlmsghdr*)buf; NLMSG_OK(nh, ret); nh = NLMSG_NEXT(nh, ret))
        {
            if (nh->nlmsg_type == NLMSG_DONE)
                break;
            else if (nh->nlmsg_type == NLMSG_ERROR)
                return -1;
            else if (nh->nlmsg_type != RTM_NEWLINK)
                continue;
            /* 读取网卡名字 */
            ifinfo = NLMSG_DATA(nh);
            printf("[Note] if: %u link %s\n", ifinfo->ifi_index, 
                (ifinfo->ifi_flags & IFF_LOWER_UP)? "up": "down" );
            
            /* 读取网卡名称 */
            attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));
            len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));
            for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len)) {
                if (attr->rta_type == IFLA_IFNAME) {
                    printf("[Note] if_name: %s", (char*)RTA_DATA(attr));
                    break;
                }
            }
            printf("\n");

        }
    }
}

int main(int argc, char** argv)
{
    if_link_status(NULL);
    return 0;
}

