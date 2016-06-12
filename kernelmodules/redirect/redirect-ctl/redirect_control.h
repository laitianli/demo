#ifndef _REDIRECT_CONTROL_H_
#define _REDIRECT_CONTROL_H_
#include <linux/list.h>
/* redirect infomation */
#define REDIRECT_SET_INFO 			('U'<<24 | 'R'<<16 | 'L' <<8 | 'A')
/* user */
#define REDIRECT_GET_USER_COUNT  			(REDIRECT_SET_INFO + 1)
#define REDIRECT_GET_USER_DATA  			(REDIRECT_GET_USER_COUNT + 1)

struct redirect_user_ex {
	unsigned long 			count;
	struct redirect_user* 	user;	
};

struct redirect_info {
	unsigned long 	red_timeout;		/* redirect timeout */
	unsigned char   red_tag;			/* redirect tag */
	char		 	red_network[10];	/* redirect network */
	char 			red_url[4096];  	/* redirect url */	
};

int redirect_ctl_module_init(void);
void redirect_ctl_module_exit(void);

#endif

