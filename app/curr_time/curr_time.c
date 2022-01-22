#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

char *get_curr_timer(void)
{
    static char str_time[20] = {0};
    struct timeval tv = {0};
    struct tm *t = {0};
    gettimeofday(&tv, NULL);
    char* str = ctime(&tv.tv_sec);
    printf("str:%s\n", str);

    t = gmtime(&tv.tv_sec);
    printf("%d-%d-%d %d:%d:%d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    time_t tt = time(NULL);
    t = gmtime(&tt);
    printf("%d-%d-%d %d:%d:%d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return NULL;
}

int main(int argv, char** argc)
{
    get_curr_timer();
    return 0;
}
