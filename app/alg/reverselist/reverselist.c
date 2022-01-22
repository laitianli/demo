#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "single_list.h"

int reverse_list(struct single_list** head)
{
    if (!head) {
        return -1;
    }

    struct single_list *prev = NULL;
    struct single_list *curr = NULL;
    struct single_list *next = NULL;

    prev = *head;
    curr = prev->next;
    prev->next = NULL;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    *head = prev;

    return 0;
}

int reverse_list_recursive(struct single_list* head)
{
    return 0;
}
