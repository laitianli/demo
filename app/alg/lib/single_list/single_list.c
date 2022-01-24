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

int add_node(struct single_list** head, int val)
{
    struct single_list* p = NULL;
    struct single_list* node  = NULL;
    if (!head) {
        return 0;
    }
    if (!*head) {
        node = get_node();
        if (!node) {
            return 0;
        }
        node->val = val;
        node->next = NULL;
        *head = node;
        return 1;
    }
    p = *head;
    while (p->next) {
        p = p->next;
    }
    node = get_node();
    if (!node) {
        return 0;
    }
    node->val = val;
    node->next = NULL;
    p->next = node;
    return 1;
}

int del_node(struct single_list** head, struct single_list* node)
{
    struct single_list* p = NULL;

    if (!head || !*head || !node) {
        return 0;
    }
    if (*head == node) {
        p = *head;
        *head = (*head)->next;
        free(p);
        return 1;
    }
    p = *head;
    while (p->next != node) {
        p = p->next;
    }

    p->next = p->next->next;
    free(node);
    return 1;
}

void destroy_single_list(struct single_list** head)
{
    if (!head) {
        return ;
    }
    struct single_list* tmp = *head;
    struct single_list* p = *head;
    while (p) {
        tmp = p;
        p = p->next;
        free(tmp);
    }
    *head = NULL;
}

void dump_single_list(struct single_list* head)
{
    struct single_list* p = head;
    printf("list: ");
    while (p) {
        printf(" %d", p->val);
        p = p->next;
    }
    printf("\n");
}
