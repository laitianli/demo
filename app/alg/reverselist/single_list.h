#ifndef _SINGLE_LIST_H_
#define _SINGLE_LIST_H_

struct single_list {
    int val;
    struct single_list * next;
};


int add_node(struct single_list** head, int val);

int del_node(struct single_list** head, struct single_list* node);

void destroy_single_list(struct single_list** head);
void dump_single_list(struct single_list* head);
#endif


