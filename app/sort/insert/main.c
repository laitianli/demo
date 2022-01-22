#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "insert.h"

int main(int argv, char** argc)
{
    int array[] = {23, 200, -1, 1, 45, 56, 78, 100, 57};
    insert_sort(array, sizeof(array)/sizeof(int));
    dump_array(array, sizeof(array)/sizeof(int));
    return 0;
}
