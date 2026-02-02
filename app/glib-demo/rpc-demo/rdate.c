#include <stdio.h>
#include <rpc/rpc.h>
#include "date.h"

int main(int argc, char *argv[]) {
    CLIENT *cl;
    char *server;
    long *lresult;
    char **sresult;

    if (argc != 2) {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        exit(1);
    }
    server = argv[1];

    /* 创建客户端句柄 */
    cl = clnt_create(server, DATE_PROG, DATE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(server);
        exit(1);
    }

    /* 调用远程过程 */
    if ((lresult = bin_date_1(NULL, cl)) == NULL) {
        clnt_perror(cl, server);
        exit(1);
    }
    printf("Time on host %s = %ld\n", server, *lresult);

    if ((sresult = str_date_1(lresult, cl)) == NULL) {
        clnt_perror(cl, server);
        exit(1);
    }
    printf("Date is: %s", *sresult);

    clnt_destroy(cl); /* 销毁句柄，释放资源 */
    return 0;
}