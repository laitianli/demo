#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "single_list.h"

struct single_list* get_node(void)
{
    struct single_list * node = malloc(sizeof(struct single_list));
    if (!node) {
        return NULL;
    }
    node->val = 0;
    node->next = NULL;
    return node;
}

void add_node(struct single_list* head, int val)
{
    struct single_list* p = NULL;
    struct single_list* node  = NULL;
    if (!head) {
        return NULL;
    }
    p = head;
    while (p->next) {
        p = p->next;
    }
    node = get_node();
    if (!node) {
        return NULL;
    }
    node->val = val;
    node->next = NULL;
    p->next = node;    
}

void del_node(struct single_listi** head, struct single_list* node)
{
    struct single_list* p = NULL;

    if (!head || *head || !node) {
        return 0;
    }
    if (*head == node) {
        p = *head;
        *head = *head->next;
        free(p);
        return 1;
    }
    p = head;
    while (p->next != node) {
        p = p->next;
    }

    p->next = p->next->next;
    return 1;
}

void distroy_single_list(struct single_list* head)
{

}

