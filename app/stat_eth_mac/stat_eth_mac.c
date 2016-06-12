#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAC_COUNT  6
struct mac_info{
	char mac_name[16];
	char mac[20];
	char ip[30];
};

char* get_mac_string(char* mac)
{
	char tmp[20] = {0};
	strcpy(tmp, mac);
	char* p = tmp; 
	char* qq = NULL;
static	char resultbuf[30] = {0};
	int len = 0;
	memset(resultbuf, 0, sizeof(resultbuf));
	while(p && (qq = strstr(p, ":")))
	{
		*qq = '\0';
		len += sprintf(resultbuf + len, "%s", p);
		p = qq + 1;
	}
	sprintf(resultbuf + len, "%s", p);
	return resultbuf;
}
#define STR_HWaddr "HWaddr"
#define STR_inet_addr "inet addr:"
int get_mac_info(char *eth, struct mac_info* info)
{
	if(!eth || !info)
		return -1;
	int ret = 0;
	char cmdbuf[40] = {0};
	char resultbuf[256] = {0};
	sprintf(cmdbuf, "ifconfig %s", eth);
	FILE* pf = popen(cmdbuf, "r");
	if(!pf)
	{
		printf("[error] popen fail, cmd:%s, errorcode=%d\n", cmdbuf,errno);
		return -1;
	}
#if 0
#else
	char *p = NULL;
	while(fgets(resultbuf, sizeof(resultbuf) - 1, pf) != NULL )
	{
		if(!strncmp(resultbuf, "eth", 3))
		{
			strcpy(info->mac_name, eth);
			p = strstr(resultbuf, STR_HWaddr);
			if(p)
				sscanf(p, STR_HWaddr" %s", info->mac);
		}
		else if((p = strstr(resultbuf, STR_inet_addr)))
		{
			char *qq = strstr(p, "  Bcast");
			*qq = '\0';
			sscanf(p, STR_inet_addr"%s", info->ip);
		//	printf("ip=%s\n", info->ip);
		}
	}
	ret = 0;
#endif
ERROR:
	pclose(pf);
	return ret;
}

int main(int argc, char** argv)
{
	int i = 0; 
	char tmpbuf[40] = {0};
	char resultbuf[256] = {0};
	struct mac_info info;
	int mac_count = (argv[1]) ? atoi(argv[1]):MAC_COUNT;
	for (i = 0; i < mac_count; i++)
	{
		memset(&info, 0, sizeof(struct mac_info));
		memset(resultbuf, 0, sizeof(resultbuf));
		sprintf(tmpbuf, "eth%d", i);
		get_mac_info(tmpbuf, &info);
		if(strlen(info.mac_name) > 3)
			fprintf(stdout, "%s||%s||%s||%s\n", info.mac_name, info.ip, info.mac, get_mac_string(info.mac));
	}
	return 0;
}
