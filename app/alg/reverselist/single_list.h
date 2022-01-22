#ifndef _SINGLE_LIST_H_
#define _SINGLE_LIST_H_

struct single_list {
    int val;
    struct single_list * next;
};


struct single_list* get_node(void);

void add_node(struct single_list* head, int val);

void del_node(struct single_list* head, struct single_list* node);

void distroy_single_list(struct single_list* head);

#endif


