#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "double_list.h"
/* 无head 双向循环链表 */
struct double_list* get_node(void)
{
    struct double_list * node = malloc(sizeof(struct double_list));
    if (!node) {
        return NULL;
    }
    node->val = 0;
    node->next = node->prev = node;
    return node;
}

int add_node(struct double_list** head, int val)
{
    struct double_list* node = NULL;
    struct double_list* p = NULL;
    if (!head) {
        return 0;
    }

    if (!*head) {
        node = get_node();
        if (!node) {
            return 0;
        }
        node->val = val;
        *head = node;
        return 1;
    }
    node = get_node();
    if (!node) {
        return 0;
    }
    node->val = val;
    /* insert at list tail */
    p = (*head)->prev;
    node->next = p->next;
    p->next->prev = node;
    p->next = node;
    node->prev = p;
    return 1;
}

int remove_node(struct double_list** head, struct double_list* node)
{
    if (!head || !*head || !node) {
        return 0;
    }
    if (*head == node) {
        node->prev = *head;
        node->next = *head;
        return 1;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return 1;
}

int remove_node_by_val(struct double_list** head, int val)
{
    struct double_list* p = NULL;
    unsigned int find_flag = 0;
    p = *head;
    do {
        if (val == p->val) {
            find_flag = 1;
            break;
        }
        p = p->next;
    }  while (p != *head);

    if (find_flag) {
        remove_node(head, p);
        free(p);
        return 1;
    }
    return 0;
}

void destroy_list(struct double_list** head)
{
    if (!head || !*head) {
        return ;
    }
    struct double_list* p = NULL;
    do { /* del head */
        p = (*head);
        if (p->next == p) {/* only one node*/
            free(p);
            *head = NULL;
            return ;
        }
        *head = p->next;
        (*head)->prev = p->prev;
        p->prev->next = *head;
        free(p);
    } while (*head);
    return ;
}

void dump_list(struct double_list* head)
{
   struct double_list* p = head;
   if (!head) {
        return ;
   }
   do {
        printf(" %d", p->val);
        p = p->next;
   } while (head != p);
}

