
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "single_list.h"
#include "reverselist.h"

struct single_list *g_head = NULL;

int main(int argv, char** argc)
{
    int i = 0;
    int count = 40;
    for (i = 0; i < count; i++) {
        add_node(&g_head, i);
    }

    dump_single_list(g_head);

    reverse_list(&g_head);
    dump_single_list(g_head);
    g_head = reverse_list_recursive(&g_head);

    dump_single_list(g_head);
    destroy_single_list(&g_head);
    return 0;
}
