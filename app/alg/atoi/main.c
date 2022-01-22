#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static inline int my_pow(int base, int index)
{
    int i = 0;
    int sum = 1;
    for (i = 1; i <= index; i++)
        sum *= base;
    return sum;
}
int my_atoi(const char* str)
{
    if (!str) 
        return -1;
    const char ch_0 = '0';
    const char ch_9 = '9';
    int sum = 0;
    int i = 0;
    const char* p = str;
    while (*p != '\0')
    {
        if (*p < ch_0 || *p > ch_9)
            break ;
        p++;
    }
    while (p != str) {
        p--;
        sum += ((int)*p - (int)ch_0) * my_pow(10, i);
        i ++;
    }
    return sum;
}

int main(int argv, char** argc)
{
    char *str = "1235678a89";
    int ret = my_atoi(str);
    printf("my_atoi=%d\n", ret);
    return 0;
}
