#include <unistd.h>
#include <stdio.h>
#include <string.h>

static int  encryption_disk_setup(char *str)
{
	char* p = str, *q = NULL;
	char name[32] = {0};
	while(p && (q = strstr(p,","))) {
		*q = '\0';
		strcpy(name, p);
		printf("name=%s\n",name);
		p = q +1;
	}
	if(p)
		strcpy(name,p);
	printf("name=%s\n",name);
}
int main(void)
{
	char* buf = malloc(100);
	strcpy(buf,"sdb,sdb1,sdb2,sdb3,sdb4");
	encryption_disk_setup(buf);
	free(buf);
	return 0;
}
