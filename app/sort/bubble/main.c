#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bubble.h"

int main(int argv, char** argc)
{
    int array[] = {23, 1, 45, 56, 78, 100, 57};
    bubble_sort(array, sizeof(array)/sizeof(int));
    dump_array(array, sizeof(array)/sizeof(int));
    return 0;
}
