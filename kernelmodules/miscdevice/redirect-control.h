#ifndef _REDIRECT_CONTROL_H_
#define _REDIRECT_CONTROL_H_
#include <linux/list.h>
/* redirect infomation */
#define REDIRECT_SET_INFO 			('U'<<24 | 'R'<<16 | 'L' <<8 | 'A')
/* user */
#define REDIRECT_SET_USER  			(REDIRECT_SET_INFO + 1)
#define REDIRECT_GET_USER  			(REDIRECT_SET_USER + 1)

struct redirect_user {
	char 				mac_ag[128];
	struct list_head 	list;
};

struct redirect_info {
	unsigned long 	red_timeout;		/* redirect timeout */
	unsigned char   red_tag;			/* redirect tag */
	char		 	red_network[10];	/* redirect network */
	char 			red_url[4096];  	/* redirect url */	
};

void redirect_add_user(const char* user);

unsigned long get_timeout(void);
unsigned char get_tag(void);
char* get_network_name(void);
char* get_url(void);
#endif

