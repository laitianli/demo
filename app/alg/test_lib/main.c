#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "double_list.h"

void test_double_list(int size)
{
    struct double_list* g_head = NULL;
    int i = 0;
    int count = size;
    printf("[note] array size: %d\n", count);
    for (i = 0; i < count; i++) {
        add_node(&g_head, i);
    }
    printf("double_list: \n");
    dump_list(g_head);
    printf("\n");
#if 1
    for(i = count - 1; i > 0; i--) {
        remove_node_by_val(&g_head, i);
        printf("after remvoe val: %d, double_list: \n", i);
        dump_list(g_head);
        printf("\n");
    }
#else
    int val = 3;
    remove_node_by_val(&g_head, val);
    printf("after remvoe val: %d, double_list: \n", val);
    dump_list(g_head);
    printf("\n");
#endif
    destroy_list(&g_head);
}

int main(int argv, char** argc)
{
    if (argv < 2) {
        printf("[Error] please input array size!\n");
        printf("> \teg: %s 10\n", argc[0]);
        return -1;
    }

    test_double_list(atoi(argc[1]));
    return 0;
}
