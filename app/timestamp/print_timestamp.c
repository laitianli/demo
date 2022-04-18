#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
//tcpdump timestamp
int main(int argv, char** argc)
{
    struct timeval tv;
    struct timezone tz;
    char timestr[32] = {0};
    struct tm *tm;
    gettimeofday(&tv, &tz);
    tm = localtime(&tv.tv_sec);
    strftime(timestr, sizeof(timestr), "%H:%M:%S", tm);
    printf("ts: %s.%06u\n", timestr, tv.tv_usec);

    return 0;
}
