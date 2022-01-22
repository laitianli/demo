#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
void print_out(int *index) 
{
    int i = 0;
     for (i = 1; i <= 10; i++)
     {
        if (index[i] == 1) {
            printf("%d ",i);
        }
    }
    printf("\n");
}
void find_sum(int sum, int begin,int end ,int *index)
{
    //如果需要求和的值==0
    if (sum < begin || end < begin) {
        return;
    }
    //相等输出找到的结果
    if (sum == end) {
        index[end] = 1;
        print_out(index);
    }
    //放入end
    index[end] = 1;
    find_sum(sum - end, begin, end - 1, index);
    //不放入end
    index[end] = 0;
    find_sum(sum, begin, end - 1, index);
}


int main(int argv, char** argc)
{
    if (argv < 3) {
        printf("[Error] Please input argument, run app: %s 6 10\n", argc[0]);
        return -1;
    }
    int begin = 1;
    int end = atoi(argc[1]);
    int sum = atoi(argc[2]);
    printf("get sum: %d from [ 1,2,......,%d ]\n", sum, end);
    int *index = malloc(sizeof(int) * end);
    if(!index) {
        printf("[error] malloc failed!\n");
        return -1;
    }
    find_sum(sum, begin, end, index);
    return 0;
}

