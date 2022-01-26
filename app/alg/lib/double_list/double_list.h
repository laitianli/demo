#ifndef _DOUBLE_LIST_H_
#define _DOUBLE_LIST_H_

struct double_list {
    int val;
    struct double_list *prev;
    struct double_list *next;
};


int add_node(struct double_list** head, int val);
int remove_node(struct double_list** head, struct double_list* node);
int remove_node_by_val(struct double_list** head, int val);

void destroy_list(struct double_list** head);

void dump_list(struct double_list* head);
#endif

