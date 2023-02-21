#ifndef _TCP_SOCK_H_
#define _TCP_SOCK_H_

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <dirent.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <crypt.h>
#include <pwd.h>
#include <shadow.h>
#include <grp.h>/*getgrgid*/
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "statser.h"
#include "semphore.h"
#include "hashtable.h"
#include "conf.h"
#define ERR_EXIT(m) (perror(m),exit(EXIT_FAILURE))

#define CTRL_PORT      6666
#define DATE_PORT      20
#define TIMEOUT        20
#define MAXINTERFACES  16
#define MAX_ONLINE     3
#define MAX_IP         10
/**************translate the message**************/
typedef struct _function_
{
	char cmd[20];
	int (*action)(int fd,const char* cmd);
}COMMAND;

/*************************************************/
extern char username[20];/*login name*/
extern int type ;/*date translate mode 1:port,2:pasv*/
extern int tratype;
extern char port_ip[16] ;/*client ip*/
extern int port_port;		/*client port*/
extern int portfd ;/*date tranfer socket by active*/
extern int pasvfd ;/*date tranfer socket by positive*/
extern char old_path[50];/*rename,save the old name*/

extern HASH* hash;
extern HASH* hash_pid;

extern PIDIP* pidip;
extern INFO* info;
extern int semid;
extern int usrtype;
extern int sameip;
extern int sum;
extern int upspeed;
extern int downspeed;
/*************************************************/
/*user function*/
int fun_user(int fd,const char* cmd);

/*user passwd*/
int fun_pass(int fd,const char* cmd);

/*Change dir*/
int fun_xcwd(int fd,const char* cmd);

/*type*/
int fun_type(int fd,const char* cmd);


int fun_cdup(int fd,const char* cmd);

/*QUIT*/
int fun_quit(int fd,const char* cmd);
/*********************/
/*positive mode*/
int fun_port(int fd,const char* cmd);

/*passive mode*/
int fun_pasv(int fd,const char* cmd);
/*********************/

/*RETR*/
int fun_retr(int fd,const char* cmd);



int fun_stor(int fd,const char* cmd);

int fun_appe(int fd,const char* cmd);

int fun_rest(int fd,const char* cmd);


int fun_rnfr(int fd,const char* cmd);

int fun_rnto(int fd,const char* cmd);

int fun_abor(int fd,const char* cmd);

int fun_dele(int fd,const char* cmd);

int fun_xrmd(int fd,const char* cmd);

int fun_xmkd(int fd,const char* cmd);


int fun_xpwd(int fd,const char* cmd);


int fun_list(int fd,const char* cmd);

int fun_nlst(int fd,const char* cmd);

int fun_syst(int fd,const char* cmd);

int fun_stat(int fd,const char* cmd);

int fun_size(int fd,const char* cmd);

int fun_help(int fd,const char* cmd);

int fun_size(int fd,const char* cmd);

int fun_site(int fd,const char* cmd);

int fun_noop(int fd,const char* cdm);

/*get listen socket*/
int get_ltn_socket(int port,char* ip);

/*
addept timeout if tag = 0,and sec argument set the time out
*/
int accept_timeout(int fd,struct sockaddr_in* addr,int tag,int sec);

/*
read a line from a socket discription
*/
int readline(int sockfd,void* buf,size_t size);

/*
read data from a socket discription into buf,that can read discontinous
*/
int read_loop(int fd,void* buf,int size);

/*
write data into a socket discription to buf,that can write discontinous
*/
int write_loop(int fd,void* buf,int size);
/*
get connet socket
*/
int get_conn_sock(int port,char* ip);
/*
connect to client
*/
int connect_client(int port,char*ip);
/*
accept from client
*/
int accept_from_client(int fd);
/*get command and argcment from a string*/
char* get_command(const char* string,char* cmd,char* arg);

/*print directory*/
int print_dir(int fd,char* dir);

/*print directory detail*/
int print_dir_detail(int fd);
/*change ip to a char*/
void change_ip_port(const char* string,char* ip,int* port);

/*get local ip*/
int get_local_ip(char* ipaddr);

void speedhandle(int sig);
/*****************************************************************************************/
#endif
