#include <rpc/rpc.h>
#include "date.h"
#include <time.h>
long *
bin_date_1_svc(void *argp, struct svc_req *rqstp)
{
    static long timeval; /* 必须为静态变量或全局变量 */
    timeval = time((long *)0);
    return &timeval;
}

char **
str_date_1_svc(long *bintime, struct svc_req *rqstp)
{
    static char *ptr;    /* 必须为静态变量或全局变量 */
    ptr = ctime(bintime);
    printf("[Note][%s:%d] str_date_1_svc called. bintime=%ld\n",
           __FILE__, __LINE__, *bintime);
    printf("[Note][%s:%d] str_date_1_svc called. strtime=%s",
           __FILE__, __LINE__, ptr);
    return &ptr;
}