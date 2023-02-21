#include "ftpfun.h"

/*********groble argument*********/
char username[20]={0};/*login name*/
int type = -1;/*date translate mode 1:port,2:pasv*/
char port_ip[16] = {0};/*client ip*/
int port_port = 0;		/*client port*/
int portfd = -1;/*date tranfer socket by active*/
int pasvfd = -1;/*date tranfer socket by positive*/
char old_path[50]={0};/*rename,save the old name*/
off_t size = 0;/*send the size of file*/

HASH* hash = NULL;
HASH* hash_pid = NULL;
PIDIP* pidip = NULL;
INFO* info = NULL;/*share memary*/

int semid = 0;
int usrtype = 0;
int sum = 0;
int sameip = 0;
int upspeed = 0;
int downspeed = 0;
/*********************************/
int get_ltn_socket(int port,char* ip)
{
    int ret;
    int opt = 1;
    int ltn_sock;
    struct sockaddr_in ltn_addr;
    ltn_sock = socket(AF_INET,SOCK_STREAM,0);
    if (ltn_sock == -1)
        ERR_EXIT("Create socket");
    ltn_addr.sin_family = AF_INET;
    ltn_addr.sin_port = htons(port);
    if (ip == NULL)
        ltn_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        ltn_addr.sin_addr.s_addr = inet_addr(ip);

    ret = setsockopt(ltn_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (ret == -1)
        ERR_EXIT("setsockopt");
    ret = bind(ltn_sock,(struct sockaddr*)&ltn_addr,sizeof(ltn_addr));
    if (ret == -1)
        ERR_EXIT("Bind");

    ret = listen(ltn_sock,5);
    if (ret == -1)
        ERR_EXIT("Listen");
    return ltn_sock;
}
/*
    tag = 0:timeout,sec :the second of timeout
*/
int accept_timeout(int fd,struct sockaddr_in* addr,int tag,int sec)
{
    int ret;
    int sock;
    int len;
    struct timeval tm;
	memset(&tm,0,sizeof(tm));
    tm.tv_sec = sec;
    tm.tv_usec = 0;

    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(fd,&rset);
    while(1)
    {
        ret = select(fd + 1,&rset,NULL,NULL,&tm);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        if (tag == 0 && ret == 0)
            return -1;
        len = sizeof(addr);
	    sock = accept(fd,(struct sockaddr*)&addr,&len);
      	if (sock == -1)
		{
			if (errno == EINTR || errno ==  EBADF)
				continue;
        	else
			   	ERR_EXIT("Accept");
		}
        return sock;
    }
}

int readline(int sockfd,void* buf,size_t size)
{
    int ret = 0;
    int n = 0;
	int k = 0;
    int sum = 0;
    char* tmpbuf = (char*)malloc(size);
	memset(tmpbuf,0,size);
    char *p = tmpbuf;
    while (1)
    {
        ret = recv(sockfd,tmpbuf + sum,size,MSG_PEEK);
        if(ret == -1)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        if ((p = strstr(tmpbuf,"\r\n")) != NULL)
        {
            p[2]  = '\0';
            n = strlen(tmpbuf);
            k = read_loop(sockfd,buf,n);
			if (k <= 0)
				return 0;
			free(tmpbuf);
            return n;
        }
        sum += ret;
    }
}
int read_loop(int fd,void* buf,int size)
{
    int ret = 0;
    int num = 0;

    while (1)
    {
        ret = read(fd,buf+num,size);
        if (ret == -1)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }

        if (ret == 0)
            return num;

        num += ret;
        size -= ret;
        if (size == 0)
            return num;
    }

}
int write_loop(int fd,void* buf,int size)
{
    int ret = 0;
    int num = 0;

    while (1)
    {
        ret = write(fd,buf+num,size);
        if (ret == -1)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }

        if (ret == 0)
            return num;

        num += ret;
        size -= ret;

        if (size == 0)
            return num;
    }
}

int get_conn_sock(int port,char* ip)
{
	int ret = 0;
    int con_sock = 0;
	int opt = 1;
	uid_t uid = 0;
    struct sockaddr_in addr;
    con_sock = socket(AF_INET,SOCK_STREAM,0);
    if (con_sock == -1)
        ERR_EXIT("Create socket");
	memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(ip == NULL)
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        addr.sin_addr.s_addr = inet_addr(ip);
    ret = setsockopt(con_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (ret == -1)
        ERR_EXIT("setsockopt");
	uid = geteuid();
	seteuid(0);
	ret = bind(con_sock,(struct sockaddr*)&addr,sizeof(addr));
	if (ret == -1)
		ERR_EXIT("Local bind.");
	seteuid(uid);
    return con_sock;
}
int connect_client(int port,char* ip)
{
	int ret = 0;
	struct sockaddr_in con_addr;
	memset(&con_addr,0,sizeof(con_addr));
    con_addr.sin_family = AF_INET;
    con_addr.sin_port = htons(port);
    con_addr.sin_addr.s_addr = inet_addr(ip);
	ret = connect(portfd,(struct sockaddr*)&con_addr,sizeof(con_addr));
    if (ret == -1)
       ERR_EXIT("Connect");
	return portfd;
}
/*
accept from client
*/
int accept_from_client(int fd)
{
	struct sockaddr_in ltnaddr;
	memset(&ltnaddr,0,sizeof(ltnaddr));
	return accept_timeout(fd,&ltnaddr,0,TIMEOUT);
}

/*get command and argcment from a string*/
char* get_command(const char* string,char* cmd,char* arg)
{
	char tmp[200] = {0};
	char* p = tmp;
    strcpy(tmp,string);
	if((p=strchr(tmp,'\r')) != NULL)
		*p = 0;
	p = strchr(tmp,' ');
	if (p==NULL)
	{
		strcpy(cmd,tmp);
		arg = NULL;
	}
	else
	{
		*p=0;
		strcpy(cmd,tmp);
		strcpy(arg,p+1);
	}
  	return cmd;
}

/*print directory*/
int print_dir(int fd,char* dir)
{
	DIR* dp = NULL;
	struct dirent* entry = NULL;
	char buf[100]="\0";
	if((dp = opendir(dir)) == NULL)
		ERR_EXIT("Opendir");
	chdir(dir);
	while((entry = readdir(dp)) != NULL)
	{
		memset(buf,0,100);
		strcpy(buf,entry->d_name);
		if (!(strcmp(buf,".") == 0 || strcmp(buf,"..") == 0))
		{
			strcat(buf,"\r\n");
			write(fd,buf,strlen(buf));
		}
	}
	chdir("..");
	closedir(dp);
	return 0;

}
/*print directory detail*/
int print_dir_detail(int fd)
{
    DIR *dir=NULL;
	int res;
	int ftime = 0;
	struct dirent * drnt=NULL;
	struct stat st;
	struct timeval tv;
	char link[30]={0};
	char buf[200]={0};
	char str[200]={0};
	char string[200] = {0};
	char deffrm[15]= {0};
	memset(&st,0,sizeof(st));
	memset(&tv,0,sizeof(tv));
	gettimeofday(&tv,NULL);
	getcwd(buf,200);
	dir=opendir(buf);
	if (dir == NULL)
	{
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
		return 0;
	}
	while((drnt=readdir(dir))!=NULL)
	{
		if (strncmp(drnt->d_name,".",1) == 0 ||strcmp(drnt->d_name,".")==0 || strcmp(drnt->d_name,"..")==0)
			continue;
		memset(string,0,200);
		memset(str,0,200);
		memset(deffrm,0,15);
		strcpy(deffrm," %b %e %H:%M");
		sprintf(str,"%s/%s",buf,drnt->d_name);		
		lstat(str,&st);
		res = (st.st_mode) & S_IFMT;
		switch(res)
		{
			case S_IFSOCK:
				strcat(string,"s");
				break;
			case S_IFLNK:
			{
				memset(link,0,30);
				readlink(str,link,30);
				strcat(string,"l");
			}
				break;
			case S_IFREG:
				strcat(string,"-");
				break;
			case S_IFBLK:
				strcat(string,"b");
				break;
			case S_IFDIR:
				strcat(string,"d");
				break;
			case S_IFCHR:
				strcat(string,"c");
				break;
			case S_IFIFO:
				strcat(string,"p");
			break;
		}
		if ((st.st_mode)&S_IRUSR)
			strcat(string,"r");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IWUSR)
			strcat(string,"w");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IXUSR)
			strcat(string,"x");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IRGRP)
			strcat(string,"r");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IWGRP)
			strcat(string,"w");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IXGRP)
			strcat(string,"x");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IROTH)
			strcat(string,"r");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IWOTH)
			strcat(string,"w");
		else
			strcat(string,"-");
		if ((st.st_mode)&S_IXOTH )
			strcat(string,"x");
		else
			strcat(string,"-");
		sprintf(strlen(string) + string,"%4d",st.st_nlink);
		sprintf(strlen(string) + string,"%8d",st.st_uid);
		sprintf(strlen(string) + string,"%8d",st.st_gid);
		sprintf(strlen(string) + string,"%11d",(int)st.st_size);
		ftime = st.st_mtime;
		if (ftime - tv.tv_sec > 182*24*3600 || ftime > tv.tv_sec)
		{
			memset(deffrm,0,15);
			strcpy(deffrm," %b %e %Y");
		}	
		strftime(strlen(string)+string,strlen(string),deffrm,localtime(&(st.st_mtime)));
		if (S_ISLNK(st.st_mode))
			sprintf(strlen(string) + string,"  %s->%s\r\n",drnt->d_name,link);
		else 
			sprintf(strlen(string) + string,"  %s\r\n",drnt->d_name);
		write_loop(fd,string,strlen(string));	
	}
	closedir(dir);
	return 0;
}
/*************************************************/
/*user function*/
int fun_user(int fd,const char* cmd)
{
	char buf[] = "331 Please specify the password.\r\n" ;
	memset(username,0,sizeof(username));
	strcpy(username,cmd);
	write_loop(fd,buf,strlen(buf));
	return 0;
}

/*user passwd*/
int fun_pass(int fd,const char* cmd)
{
	int tag = 0;
	char pwd_enpt[100];
	char sat[20] = {0};
	char buf[100] = "530 Login incorrect.\r\n";
	struct spwd *sp = NULL;
	struct passwd *pwd = NULL;
	if (strcmp(username,"anonymous") == 0)
	{
		memset(buf,0,100);
		strcpy(buf,"230 Login successful.Have fun.\r\n");
		
		pwd = getpwnam("ftp");
		setegid(pwd -> pw_gid);
		seteuid(pwd -> pw_uid);
		chdir(pwd -> pw_dir);
		write_loop(fd,buf,strlen(buf));
		return 0;
	}
	sp = getspnam(username);
	if(sp == NULL)
		return -1;
	memset(sat,0,sizeof(sat));
	strncpy(sat,sp->sp_pwdp,12);
	strcpy(pwd_enpt,crypt(cmd,sat));
	if(strcmp(pwd_enpt,sp -> sp_pwdp) == 0)
	{
		memset(buf,0,100);
		strcpy(buf,"230 Login successful.Have fun.\r\n");
		pwd = getpwnam(username);
		setegid(pwd -> pw_gid);
		seteuid(pwd -> pw_uid);
		chdir(pwd -> pw_dir);
		tag = 1;
	}
	write_loop(fd,buf,strlen(buf));
	if (tag == 0)/*passwd not right*/
		return -1;
	return 0;
}			
/*Change dir*/
int fun_xcwd(int fd,const char* cmd)
{
	char buf[100] = "250 Directory successfully changed.\r\n";
	char tmp[200] = {0};
	char dir[200] = {0};
	int n = strlen("/");
	getcwd(dir,200);
	if (strncmp(dir,"/home",5) == 0)
		strcpy(tmp,cmd);
	else
	{
		if (cmd[0] != '/')
			strcpy(tmp,cmd);
		else
		{
			if (strncmp(cmd,"/",n)==0)
				sprintf(tmp,"/var/ftp%s",cmd);
			else
				sprintf(tmp,"%s%s",dir,cmd);
		}
	}
	if (0 == chdir(tmp))
		write_loop(fd,buf,strlen(buf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}		
/*type*/
int fun_type(int fd,const char* cmd)
{
	cmd = NULL;
	char buf[100] = "200 Switching to ASCII mode.\r\n";
	write_loop(fd,buf,strlen(buf));
	return 0;
}
/*QUIT*/
int fun_quit(int fd,const char* cmd)
{
	cmd = NULL;
	write_loop(fd,"221 quit successful.\r\n",strlen("221 quit successful.\r\n"));
	sleep(2);
	shutdown(fd,SHUT_RDWR);
	return 0;
}
/*********************/
/*positive mode*/
int fun_port(int fd,const char* cmd)
{
	char ip[16]="\0";
	char local_ip[16] = "\0";
	int port=0;
	char buf[120]="200 PORT command successful.Consider using PASV.\r\n";
	type = 1;/*port mode*/
	memset(port_ip,0,sizeof(port_ip));
	change_ip_port(cmd,ip,&port);
	strcpy(port_ip,ip);
	port_port = port;
	get_local_ip(local_ip);
    portfd = get_conn_sock(DATE_PORT,local_ip);/*create and get the date socket*/
	write_loop(fd,buf,strlen(buf));
	return portfd;
}
/*passive mode*/
int fun_pasv(int fd,const char* cmd)
{
	socklen_t len = 0;
	int ret;
    int ltn_sock;
    struct sockaddr_in ltn_addr,ltnaddr;
	char buf[60]="\0";
	char tmpip[16]="\0";
	char string[30] = "\0";
	char *p = tmpip;
	int p1,p2;
	cmd = NULL;
	type = 2;
/***********************************************/
    int opt = 1;
    ltn_sock = socket(AF_INET,SOCK_STREAM,0);
    if (ltn_sock == -1)
        ERR_EXIT("Create socket");
	len = sizeof(ltn_addr);
	getsockname(fd,(struct sockaddr*)&ltn_addr,&len);
		ltn_addr.sin_port = htons(0);

    	ret = setsockopt(ltn_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
 		if (ret == -1)
    	    ERR_EXIT("setsockopt");
		ret = bind(ltn_sock,(struct sockaddr*)&ltn_addr,sizeof(ltn_addr));
	    if (ret == -1)
    	    ERR_EXIT("Bind");
		ret = listen(ltn_sock,5);
		if (ret == -1)
   		 	ERR_EXIT("Listen");
		len = sizeof(ltnaddr);
		getsockname(ltn_sock,(struct sockaddr*)&ltnaddr,&len);
	strcpy(tmpip,inet_ntoa(ltnaddr.sin_addr));
	while((p = strchr(tmpip,'.')) != NULL)
	 	p[0] = ',';
	p1 = ntohs(ltnaddr.sin_port) / 256;
	p2 = ntohs(ltnaddr.sin_port) % 256;
	sprintf(string,"%s,%d,%d",tmpip,p1,p2);
	sprintf(buf,"227 Entering Passive Mode(%s).\r\n",string);
	write_loop(fd,buf,strlen(buf));
	pasvfd = ltn_sock;
	return 0;
}
int fun_cdup(int fd,const char* cmd)
{
	char buf[100] = "\0";
	char strbuf[60] = "250 Directory successful changed.\r\n";
	char dest[100] = "\0";
	char *p = buf;
	int len = 0;
	cmd = NULL;
	getcwd(buf,100);
	len = strlen(buf);
	p = &buf[len-1];
	if (*p == '/')
		write_loop(fd,"250 current directory is '/'\r\n",strlen("250 current directory is '/'\r\n"));
	while (*p != '/')
		p --;
	p[0] = '\0';
	strcpy(dest,buf);
	if (0 ==chdir(dest))
		write_loop(fd,strbuf,strlen(strbuf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}
/*********************/

/*
RETR:loaddown
*/
int fun_retr(int fd,const char* cmd)
{
	int acceptfd = 0;
	char buf[1024] = {0};
	char dir[100] = {0};
	char file[100] = {0};
	int n = 0;
	int ret = 0;
	struct itimerval val,oval;
	char bgnstr[70]= {0};
	char endstr[30]= "226 File send OK.\r\n";
	struct stat st;
	getcwd(dir,100);
	sprintf(file,"%s/%s",dir,cmd);
	FILE* pf = fopen(file,"r");
	if (pf == NULL)
	{
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
		return 0;
	}
	stat(cmd,&st);
	sprintf(bgnstr,"150 Open Binary mode data connect for %s (%d bytes).\r\n",cmd,(int)st.st_size);
	write_loop(fd,bgnstr,strlen(bgnstr));
	if (type == 1)
		acceptfd = connect_client(port_port,port_ip);
	else if (type == 2)
		acceptfd = accept_from_client(pasvfd);
	if (size > 0)
		fseek(pf,size,SEEK_SET);
	signal(SIGALRM,speedhandle);
	memset(&val,0,sizeof(val));
	memset(&oval,0,sizeof(oval));
	val.it_value.tv_sec = 1;
	val.it_value.tv_usec = 0;
	val.it_interval.tv_sec = 1;
	val.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&val,&oval);
	sum = 0;
	while (1)
	{
		if (feof(pf))
			break;
		if (sum <= downspeed * 1024)
		{
			if ((n = fread(buf,sizeof(char),1024,pf)) != 0)
			{
				ret = write_loop(acceptfd,buf,n);
				if (ret <= 0)
				{
					shutdown(acceptfd,SHUT_RDWR);
					return 0;
				}
			}
			else
				break;
			set_downsize(info,semid,n);
			sum += n;
		}
		else pause();
	}
	set_downsum(info,semid,1);
	fclose(pf);
	shutdown(acceptfd,SHUT_RDWR);
	write_loop(fd,endstr,strlen(endstr));
	return 0;
}
/*
upload
*/
int fun_stor(int fd,const char* cmd)
{
	int acceptfd = 0;
	char buf[1024] = {0};
	char dir[100] = {0};
	char file[100] = {0};
	struct itimerval val,oval;
	int n = 0;
	char bgnstr[30]= "150 Ok to send data.\r\n";
	char endstr[30]= "226 File receive OK.\r\n";
	getcwd(dir,100);
	sprintf(file,"%s/%s",dir,cmd);
	FILE* pf = fopen(file,"w+");
	if (pf == NULL)
	{
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
		return -1;
	}
	write_loop(fd,bgnstr,strlen(bgnstr));
	if (type == 1)
		acceptfd = connect_client(port_port,port_ip);
	else if (type == 2)
		acceptfd = accept_from_client(pasvfd);
	if (size > 0)
		fseek(pf,size,SEEK_SET);
	signal(SIGALRM,speedhandle);
	memset(&val,0,sizeof(val));
	memset(&oval,0,sizeof(oval));
	val.it_value.tv_sec = 1;
	val.it_value.tv_usec = 0;
	val.it_interval.tv_sec = 1;
	val.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&val,&oval);
	sum = 0;
	while (1)
	{
		if (sum <= upspeed * 1024)
		{
			n = read_loop(acceptfd,buf,1024);
			if (n <= 0 )
				break;
			fwrite(buf,sizeof(char),n,pf);
			set_upsize(info,semid,n);
			memset(buf,0,1024);
			sum += n;
		}
		else pause();
	}
	set_upsum(info,semid,1);
	shutdown(acceptfd,SHUT_RDWR);
	fclose(pf);
	write_loop(fd,endstr,strlen(endstr));
	return 0;
		
}
int fun_appe(int fd,const char* cmd)
{
	int acceptfd = 0;
	char buf[1024] = {0};
	char bgnstr[30]= "150 Ok to send data.\r\n";
	char endstr[30]= "226 File receive OK.\r\n";
	int n = 0;
	FILE* pf = fopen(cmd,"a");
	if (pf == NULL)
		return -1;
	write_loop(fd,bgnstr,strlen(bgnstr));
	if (type == 1)
		acceptfd = connect_client(port_port,port_ip);
	else if (type == 2)
		acceptfd = accept_from_client(pasvfd);
	if (size > 0)
		fseek(pf,size,SEEK_SET);
	while ((n = read_loop(acceptfd,buf,1024)) != 0)
	{
		fwrite(buf,sizeof(char),n,pf);
		memset(buf,0,1024);
	}
	write_loop(fd,endstr,strlen(endstr));
	shutdown(acceptfd,SHUT_RDWR);
	fclose(pf);
	return 0;
}
int fun_rest(int fd,const char* cmd)
{
	char buf[100] = "\0";
	size = atol(cmd);
	sprintf(buf,"350 Restart position accepted (%ld)\r\n",size);
	write_loop(fd,buf,strlen(buf));
	return 0;
}
int fun_rnfr(int fd,const char* cmd)
{
	char buf[60]="350 Ready for RNTO.\r\n";
	strcpy(old_path,cmd);
	write_loop(fd,buf,strlen(buf));
	return 0;
}
int fun_rnto(int fd,const char* cmd)
{
	char tmppath[200]={0};
	char tmpcmd[200] = {0};
	char dir[100]={0};
	char buf[40]="250 Rename successful.\r\n";
	int n = strlen("/home/study");
	getcwd(dir,100);
	if (strncmp(dir,"/home/study",n) != 0)
	{	
		sprintf(tmppath,"/var/ftp%s",old_path);
		sprintf(tmpcmd,"/var/ftp%s",cmd);
	}
	else
	{
		strcpy(tmppath,old_path);
		strcpy(tmpcmd,cmd);
	}
	if (0 == rename(tmppath,tmpcmd))
		write_loop(fd,buf,strlen(buf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}
int fun_abor(int fd,const char* cmd)
{
	cmd = NULL;
	char nobuf[50] = "225 No tranfer to Abort.\r\n";
	write_loop(fd,nobuf,strlen(nobuf));
	return 0;
}
int fun_dele(int fd,const char* cmd)
{
	char buf[35]="250 Delete operation successful.\r\n";
	char tmp[100]={0},file[120]={0};
	getcwd(tmp,100);
	sprintf(file,"%s/%s",tmp,cmd);
	if (0 == remove(file))
		write_loop(fd,buf,strlen(buf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}
int fun_xrmd(int fd,const char* cmd)
{
	char buf[70]="250 Remove directory operation successful.\r\n";
	char tmp[100] = {0};
	char dir[100] = {0};
	int n = strlen("/home");
	if (usrtype == 1)
	{
		return 0;
	}

	getcwd(dir,100);
	if (strncmp(dir,"/home",n) != 0)
	{
		strcpy(tmp,dir);
		strcat(tmp,cmd);
	}
	else 
		strcpy(tmp,cmd);
	if (0 == rmdir(tmp))
		write_loop(fd,buf,strlen(buf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}
int fun_xmkd(int fd,const char* cmd)
{
	char buf[100]={0};
	char dir[100]={0};
	char file[120]={0};	
	if (usrtype == 1)
	{
		getcwd(dir,100);
		sprintf(file,"%s/%s",dir,cmd);
	}
	else
		strcpy(file,cmd);
	sprintf(buf,"257 \"%s\" created successful.\r\n",cmd);
	if (0 == mkdir(file,0755))
		write_loop(fd,buf,strlen(buf));
	else
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
	return 0;
}
/**/
int fun_xpwd(int fd,const char* cmd)
{
	char buf[200] = "\0";
	char tmp[200] = "\0";
	int n = strlen("/var/ftp");
	cmd = NULL;
	getcwd(tmp,200);
	if (strcmp(tmp,"/var/ftp")==0)
		strcpy(buf,"257 \"/\"\r\n");
	else if (strncmp(tmp,"/var/ftp/",n+1)==0)
		sprintf(buf,"257 \"%s\"\r\n",&tmp[n]);
	else
		sprintf(buf,"257 \"%s\"\r\n",tmp);
	write_loop(fd,buf,strlen(buf));
	return 0;
}
int fun_list(int fd,const char* cmd)
{
	cmd = NULL;
	int ret = 0;
	int acceptfd=0;
	char buf[100]="150 Here come the directory listing.\r\n";
	char subuf[100]="226 Directory send OK.\r\n";
	if (type == 0)
	{
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
		return 0;
	}
	write_loop(fd,buf,strlen(buf));
	if (type == 1)/*active transfer*/
	{	
		portfd = connect_client(port_port,port_ip);
		ret = print_dir_detail(portfd);
		shutdown(portfd,SHUT_RDWR);
	}
	else if(type == 2)
	{
		acceptfd = accept_from_client(pasvfd);
		ret = print_dir_detail(acceptfd);
		shutdown(acceptfd,SHUT_RDWR);
	}
	type = 0;
	write_loop(fd,subuf,strlen(subuf));
	return 0;
}
int fun_nlst(int fd,const char* cmd)
{
	cmd = NULL;
	int ret = 0;
	int acceptfd=0;
	char buf[100]="150 Here come the directory listing.\r\n";
	char subuf[100]="226 Directory send OK.\r\n";
	if (type == 0)
	{
		write_loop(fd,"550 Permission denied.\r\n",strlen("550 Permission denied.\r\n"));
		return 0;
	}
	write_loop(fd,buf,strlen(buf));
	if (type == 1)/*active transfer*/
	{	
		portfd = connect_client(port_port,port_ip);
		ret = print_dir(portfd,".");
		shutdown(portfd,SHUT_RDWR);
	}
	else if(type == 2)
	{
		acceptfd = accept_from_client(pasvfd);
		ret = print_dir(acceptfd,".");
		shutdown(acceptfd,SHUT_RDWR);
	}
	type = 0;
	if (ret == 0)
		write_loop(fd,subuf,strlen(subuf));
	return 0;
}
int fun_syst(int fd,const char* cmd)
{
	cmd = NULL;
	write_loop(fd,"215 Unix type.\r\n",strlen("215 Unix type.\r\n"));
	return 0;
}
int fun_help(int fd,const char* cmd)
{
	cmd = NULL;
	char buf[500]=
	{
		"215 USER,PASS,CDUP,XCWD,CWD,QUIT,PORT,PASV,TYPE,SYST,RETR,STOR\n    APPE,ERST,RNFR,RNTO,ABOR,DELE,XRMD,RMD,XMKD,MKD,XPWD,PWD\n    LIST,NLST,SYST,SIZE,HELP,SITE,NOOP\r\n"
	};
	write_loop(fd,buf,strlen(buf));
	return 0;
}
int fun_stat(int fd,const char* cmd)
{
	cmd = NULL;
	char buf[200] = {0};
	int upsum = get_upsum(info,semid);
	off_t upsize = get_upsize(info,semid);
	int downsum = get_downsum(info,semid);
	off_t downsize = get_downsize(info,semid);
	int count = get_concount(info,semid);
	sprintf(buf,"211 [Online]:%d  [up]: %d, %ld Bytes   [down]: %d, %ld Bytes\r\n",count,upsum,upsize,downsum,downsize);

	write(fd,buf,strlen(buf));
	return 0;
}
int fun_size(int fd,const char* cmd)
{
	char buf[30]={0};
	struct stat st;
	memset(&st,0,sizeof(st));
	lstat(cmd,&st);
	sprintf(buf,"213 %s:%d\r\n",cmd,(int)st.st_size);
	write_loop(fd,buf,strlen(buf));
	
	return 0;
}
int fun_site(int fd,const char* cmd)
{
	mode_t mode;
	char dir[100] = {0};
	char buf[50] = "200 SITE CHMOD command ok.\r\n";
	char tmp[10] = {0};
	char file[100] = {0};
	char tmpbuf[100] = {0};
	getcwd(dir,100);
	strcpy(file,dir);
	strcat(file,"/");
	strcpy(tmpbuf,&cmd[6]);
	char *p  = strchr(tmpbuf,' ');
	if (p != NULL)
		*p = '\0';
	strcpy(tmp,tmpbuf);
	sscanf(tmp,"%o",(unsigned int*)&mode);
	strcat(file,&p[1]);
	if(0 != chmod(file,mode))
	{
		memset(buf,0,50);
		strcpy(buf,"550 SITE CHMOD command failed.\r\n");
	}
	write_loop(fd,buf,strlen(buf));
	return 0;
}
int fun_noop(int fd,const char* cmd)
{
	cmd = NULL;
	write_loop(fd,"200 Noop ok.\r\n",strlen("200 Noop ok.\r\n"));
	return 0;
}
	

/*change ip to a char*/
void change_ip_port(const char* string,char* ip,int* port)
 {
    int count = 0;
    int a1 = 0,a2 = 0;
    int len = strlen(string)+1;
    char* tmpstr = (char*)malloc(sizeof(char)*len);
    char tmpip[16] = "\0";
    strcpy(tmpstr,string);
    char* p = tmpstr;
    while((p = strchr(tmpstr,',')) != NULL)
	{
		if (count == 3)
		{
			*p = 0;
			break;
		}
		else
			*p = '.';
		count ++;
	}
	strcpy(tmpip,tmpstr);
	strcpy(tmpstr,&p[1]);/*3,4*/
	if((p = strchr(tmpstr,','))!=NULL)
		*p = 0;
	a1 = atoi(tmpstr);
	a2 = atoi(p+1);
    strcpy(ip,tmpip);
	*port = a1 * 256 + a2;
	free(tmpstr);
 }
/********************************************************************************************/
int get_local_ip(char* ipaddr)
{
	int MAXIFTERFACE=512;
	struct	ifreq buf[MAXIFTERFACE];
	struct	ifconf ifc;
	int fd,ifterface;

	if ( (fd=socket(AF_INET,SOCK_STREAM,0)) >= 0)
	{
		ifc.ifc_len=sizeof(buf);
		ifc.ifc_buf=(caddr_t)buf;

		if ( !ioctl(fd,SIOCGIFCONF,(char *)&ifc))
		{
			ifterface=ifc.ifc_len/sizeof(struct	ifreq);
			while ( ifterface-- > 0 )
			{
				if (!ioctl(fd,SIOCGIFADDR,(char *)&buf[ifterface]))
				{
					strcpy(ipaddr,inet_ntoa( ((struct sockaddr_in*)(&buf[ifterface].ifr_addr))->sin_addr));
					break;
				}

			}
		}
	}
	close(fd);
//	ip=inet_addr(ipaddr);
	return 0;
}

void speedhandle(int sig)
{
	if (sig == SIGALRM)
		sum = 0;
}


















