#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdio.h>  
int main()
{
	DIR *dir;
	struct dirent *ptr;
	dir=opendir("/root");
	while((ptr=readdir(dir))!=NULL)
		printf("d_name:%s\n",ptr->d_name);
	closedir(dir);
	return 0;
}

