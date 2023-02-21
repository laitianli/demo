#include "myftp.h"
#include "ftpfun.h"

COMMAND cmd_ary[] = 
{
	{"USER",fun_user},
	{"PASS",fun_pass},
	{"CDUP",fun_cdup},
	/***************/
	{"XCWD",fun_xcwd},
	{"CWD", fun_xcwd},
	/***************/
	{"REIN",NULL},
	{"QUIT",fun_quit},
	/*********************/
	{"PORT",fun_port},
	{"PASV",fun_pasv},
	{"TYPE",fun_type},
	{"SYST",fun_syst},
	/*********************/
	{"RETR",fun_retr},
	{"STOR",fun_stor},
	{"APPE",fun_appe},
	{"REST",fun_rest},
	/****************/
	{"RNFR",fun_rnfr},
	{"RNTO",fun_rnto},
	/****************/
	{"ABOR",fun_abor},
	/****************/
	{"DELE",fun_dele},
	{"XRMD",fun_xrmd},
	{"RMD", fun_xrmd},
	/****************/
	{"XMKD",fun_xmkd},
	{"MKD" ,fun_xmkd},
	/****************/
	{"XPWD",fun_xpwd},
	{"PWD" ,fun_xpwd},
	/****************/
	{"LIST",fun_list},
	{"NLST",fun_nlst},
	{"SYST",fun_syst},
	{"STAT",fun_stat},
	{"SIZE",fun_size},
	{"HELP",fun_help},
	{"SITE",fun_site},
	{"NOOP",fun_noop}
};
int main(int argv,char** argc)
{ /***************************/
	int i=0;
	int len = 0;
	int tag = 0;
	int ret = 0;
	int sv[2] = {0};
	int size = 0;
	int count = 0;
	int online = 0;
	pid_t pid = 0;
	char buf[60]= {0};
	char str[1024] = {0};
	char cmd[10] = {0};
	char arg[100] = {0};
	char con_ip[16] = {0};
	int ltn_sock,con_sock;
	struct sockaddr_in addr;
	struct passwd *pwd = NULL;

	struct sigaction act;
    struct sigaction act_pid;
    union sigval val;
    union sigval val_pid;

	STR_ARRAY send;
	STR_ARRAY rec;
	memset(&send,0,sizeof(STR_ARRAY));
	memset(&rec,0,sizeof(STR_ARRAY));
	sameip = getval("sameip");
	upspeed = getval("upspeed");
	downspeed = getval("downspeed");
	online = getval("online");
	signal(SIGINT,del_handle);
	signal(SIGKILL,SIG_DFL);
	signal(SIGQUIT,del_handle);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);

	set_process(0,0);

	ltn_sock = get_ltn_socket(getval("port"),NULL);
	hash = init_hash(online,hash_fun);
	hash_pid = init_hash(online,hash_fun_pid);
	/************************************/
	semid = sem_create(ftok(".",2));
	sem_setval(semid,1);
	/************************************/
	info = get_memary(create_shm(".",1,sizeof(INFO)));
	init_shm(info);
	pidip = get_memary_pidip(create_shm(".",2,sizeof(PIDIP)));
	init_shm_pidip(pidip);
	/************************************/
	/********1.sigaction(pid,ip)***/
    memset(&act,0,sizeof(act));
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&(act.sa_mask));
    act.sa_sigaction = pidip_handle;
    sigaction(SIGRTMIN,&act,NULL);

    /******2.sigaction(pid)******/

    memset(&act_pid,0,sizeof(act_pid));
    act_pid.sa_flags = SA_SIGINFO;
    sigemptyset(&(act_pid.sa_mask));
    act_pid.sa_sigaction = pid_handle;
    sigaction(SIGRTMIN+1,&act_pid,NULL);
    /****************************/
	while (1)
	{
		len = sizeof(addr);
		con_sock = accept(ltn_sock,(struct sockaddr*)&addr,&len);
		if (con_sock == -1)
		{
			if (errno == EFAULT || errno == EINTR || errno ==  EBADF)
				continue;
			else
				ERR_EXIT("Time out");
		}
		if (get_concount(info,semid) > online - 1)
		{
			sprintf(buf,"421 Too many session.The maxinum number of session:[%d].\r\n",online);
			write_loop(con_sock,buf,strlen(buf));
			shutdown(con_sock,SHUT_RDWR);
			continue;
		}

		len = sizeof(addr);
		memset(&addr,0,sizeof(addr));
		ret = getpeername(con_sock,(struct sockaddr*)&addr,&len);
		if (ret == -1)
			ERR_EXIT("Getsockname");
		memset(con_ip,0,16);
		strcpy(con_ip,inet_ntoa(addr.sin_addr));
		if(NULL != (int*)hash_getvalue(hash,con_ip,compare))
			count = *((int*)hash_getvalue(hash,con_ip,compare));
		if (count > sameip - 1)
		{
			sprintf(buf,"421 This number of  ip can only connect:[%d].\r\n",sameip);
			write_loop(con_sock,buf,strlen(buf));
			shutdown(con_sock,SHUT_RDWR);
			continue;
		}
		write_loop(con_sock,"220 (LaiTianLi FTP 1.0)\r\n",strlen("220 (LaiTianLi FTP 1.0)\r\n"));
		switch (fork())
		{
		case -1:
			ERR_EXIT("Create a new proccess");
		case 0:/*father*/
			ret = socketpair(AF_UNIX,SOCK_STREAM,0,sv);
			if (ret == -1)
				ERR_EXIT("create socketpair");
			close(ltn_sock);
			pid = fork();
			switch (pid)
			{
			case -1:
				ERR_EXIT("Create a new process");
			case 0:
				close(sv[0]);
//				signal(SIGPIPE,pipe_handle);
				/*****change process user and group****/
				ret = read(sv[1],username,sizeof(username));
				if (ret <= 0)
				{
					close(sv[1]);
					kill(getppid(),SIGKILL);
					exit(0);
				}
				if (strcmp(username,"anonymous") == 0)
				{
					usrtype = 1;
					pwd = getpwnam("ftp");
					setegid(pwd -> pw_gid);
					seteuid(pwd -> pw_uid);
					chdir(pwd -> pw_dir);
				}
				else
				{
					pwd = getpwnam(username);
					setegid(pwd -> pw_gid);
					seteuid(pwd -> pw_uid);
					chdir(pwd -> pw_dir);
				}
				/*************end**********************/
				size = sizeof(cmd_ary)/sizeof(COMMAND);
				while (1)
				{
					i = 0;
					memset(&rec,0,sizeof(rec));
					ret = read(sv[1],&rec,sizeof(rec));/*receive socketpair*/
					if (ret <= 0)
					{
						close(sv[1]);
						kill(getppid(),SIGKILL);
						exit(0);
					}
					if (strcmp(rec.cmd,"REST") == 0)
                    {
                        size = atoi(rec.arg);
                        continue;
                    }
					if (strcmp(rec.cmd,"QUIT") == 0)
						kill(getpid(),SIGINT);
					while (i < size)
					{
						if (strcmp(rec.cmd,cmd_ary[i].cmd) == 0)
						{
							ret = cmd_ary[i].action(con_sock,rec.arg);
							break;
						}
						else
							i++;
					}
				}
				break;
			default:/*father*/
				close(sv[1]);
				/*set a signl*/
				signal(SIGINT,SIG_DFL);
//				signal(SIGPIPE,SIG_DFL);
				signal(SIGCHLD,handle);	
				size = sizeof(cmd_ary)/sizeof(COMMAND);
				len = sizeof(addr);
				memset(&addr,0,sizeof(addr));
				ret = getpeername(con_sock,(struct sockaddr*)&addr,&len);
				if (ret == -1)
					ERR_EXIT("Getsockname");
				init_shm_pidip(pidip);
                pidip -> pid = getpid();
                strcpy(pidip -> ip,inet_ntoa(addr.sin_addr));
                memset(&val,0,sizeof(val));
                val.sival_int = getpid();
                sigqueue(getppid(),SIGRTMIN,val);
				while(1)
				{
					i = 0;
					tag = 0;
					memset(str,0,sizeof(str));
					ret = read(con_sock,str,1024);
					if (ret <= 0)
					{
    					memset(&val_pid,0,sizeof(val_pid));
					    val_pid.sival_int = getpid();
					    sigqueue(getppid(),SIGRTMIN + 1,val_pid);	
						close(con_sock);
						exit(0);
					}
					if (ret <= 4)/*deal with the interupt*/
					{	
						sleep(1);
						continue;
					}
					if (strlen(str) != 0)
					{
						memset(cmd,0,sizeof(cmd));
						memset(arg,0,sizeof(arg));
						get_command(str,cmd,arg);
					}
					while (i < size)
					{
						if (strcmp(cmd,cmd_ary[i].cmd) == 0)
						{
							if (cmd_ary[i].action == NULL)
								write_loop(con_sock,"502 Command not implemented.\r\n",strlen("502 Command not implemented.\r\n"));
							else if (!check_command(cmd))/*port pasv stor retr*/
							{
							/**********tranfer the command to data process**/
								memset(&send,0,sizeof(send));
								strcpy(send.cmd,cmd);
								if (arg != NULL)
									strcpy(send.arg,arg);
								else
									memset(send.arg,0,sizeof(send.arg));
								write(sv[0],&send,sizeof(send));/*send data command*/
								tag = 1;
								break;
							}
							else
							{
						 		ret = cmd_ary[i].action(con_sock,arg);
								if (ret == -1)
								{
    					            memset(&val_pid,0,sizeof(val_pid));
					                val_pid.sival_int = getpid();
					                sigqueue(getppid(),SIGRTMIN + 1,val_pid);	
									write_loop(con_sock,"530 Login incorrect.\r\n",strlen("530 Login incorrect.\r\n"));
									kill(getpid(),SIGINT);

								}
								if (strcmp("PASS",cmd) == 0)
									write(sv[0],username,strlen(username));
							    else if (strcmp("REST",cmd) == 0)
                                {
                                    memset(&send,0,sizeof(send));
                                    strcpy(send.cmd,cmd);
                                    sprintf(send.arg,"%d",size);
                                    write(sv[0],&send,sizeof(send));
                                }
								else if (strcmp("QUIT",cmd) == 0)
								{
									memset(&send,0,sizeof(send));
									strcpy(send.cmd,cmd); memset(send.arg,0,sizeof(send.arg));
									write(sv[0],&send,sizeof(send));/*send quit*/
								/******************************************/
    					            memset(&val_pid,0,sizeof(val_pid));
					                val_pid.sival_int = getpid();
					                sigqueue(getppid(),SIGRTMIN + 1,val_pid);	
									wait(0);
									kill(getpid(),SIGINT);
								/*****************************************/
								}/*quit*/
							}
					 		tag = 1;
							break;				
						}		
						else
							i++;
					}
					if (tag == 0)
						write_loop(con_sock,"500 Unknown command.\r\n",strlen("500 Unknown command.\r\n"));
				}
				close(con_sock);
				break;
			}/*switch:second fork*/
			close(con_sock);
			break;
		}/*switch:first fork*/
	}/*while*/
	return 0;
}

int check_command(char* cmd)
{
	char *string[]={"NLST","PORT","PASV","LIST","SITE","STOR","DELE","MKD","APPE","RETR","CDUP","CWD","PWD"};
	int i;
	for(i=0; i< 13; i++)
		if (0 == strcmp(string[i],cmd))
			return 0;
	return 1;	
}
void pidip_handle(int sig, siginfo_t *msg, void *buf)
{
	int ipp = 0;
	int count = 0;
	buf = NULL;
	set_concount(info,semid,1,1);
	if (NULL != (int*)hash_getvalue(hash,pidip ->ip,compare))
		ipp = *((int*)hash_getvalue(hash,pidip ->ip,compare));
	ipp += 1;
	hash_insert(hash,pidip->ip,(void*)&ipp,strlen(pidip->ip),sizeof(int),compare);/*ip:count,compare(char,int)*/
    hash_insert(hash_pid,&(pidip->pid),pidip->ip,sizeof(int),strlen(pidip ->ip),compare_pid);
    count = *((int*)hash_getvalue(hash,pidip ->ip,compare));
}
void pid_handle(int sig, siginfo_t *msg, void *buf)
{
    int count = 0;
    char ip[16] = {0};
	buf = NULL;
    strcpy(ip,(char*)hash_getvalue(hash_pid,&(msg->si_int),compare_pid));
    count = *((int*)hash_getvalue(hash,ip,compare));
    if (count == 0)
        hash_delete(hash,ip,compare);
    else
    {
        count--;
        hash_insert(hash,ip,(void*)&count,strlen(ip),sizeof(int),compare);
    }
	set_concount(info,semid,1,-1);
}

void pipe_handle(int sig)
{
	kill(getppid(),SIGKILL);
	sleep(1);
	kill(getpid(),SIGKILL);
}

void handle(int sig)
{
	wait(0);	
	exit(0);
}

void del_handle(int sig)
{
	hash_destroy(&hash);
	hash_destroy(&hash_pid);
	sem_del(sem_create(ftok(".",2)));
    del_shm(create_shm(".",1,sizeof(INFO)));
    del_shm(create_shm(".",2,sizeof(PIDIP)));
	exit(0);
}

pid_t set_process(int nochdir,int noclose)
{
	int ret;
	int i;
	ret = fork();
	if(ret == -1)
		ERR_EXIT("fork");
	if(ret > 0 )
		exit(0);
	setsid();
	ret = fork();
	if(ret == -1)
		ERR_EXIT("fork");
	if(ret > 0 )
		exit(0);
	if(noclose != 1)
	{
		for(i = 0; i <= 2; i++)
			close(i);
	}
	return 0;
}
