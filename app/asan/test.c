#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv) 
{

    int * value = malloc(sizeof(int));
    if (NULL == value) {
        printf("malloc failed\n");
        exit(-1);
    }
    free(value);
    return *value;  // use after free. BOOM!!!!
}
