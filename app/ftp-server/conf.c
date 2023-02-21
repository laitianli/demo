#include "conf.h"
#include "ftpfun.h"
int getval(char* string)
{
	int port = 0;
	char buf[100] = {0};
	char tmp[100] = {0};
	char *p = buf;
	int n = strlen(string);
	FILE* pf = fopen("ftp.conf","r");
	if (pf == NULL)
		ERR_EXIT("open file");
	while(!feof(pf))
	{
		memset(buf,0,100);
		memset(tmp,0,100);
		fgets(buf,99,pf);
		if (!isalpha(buf[0]))
			continue;
		if (strncmp(buf,string,n)==0)
		{
			if ((p = strchr(buf,'<')) != NULL)
				strcpy(tmp,&p[1]);
			if ((p = strchr(tmp,'>'))!= NULL)
				p[0]='\0';
			port = atoi(tmp);
			break;
		}	
	}
	fclose(pf);
	return port;
}
