#ifndef _HLIST_H_
#define _HLIST_H_
#include <linux/list.h>
#include <linux/spinlock.h>
struct hash_node {
	unsigned long key;
	void* data;
	struct hlist_node node;
};

struct hlist_head_ex {
	struct hlist_head *hhead;
	unsigned char 	   bit;
	spinlock_t 		   hlist_lock;
};
typedef void (*hlist_data_fn)(struct hash_node* data);
struct hlist_head_ex* init_hash_list(unsigned char hash_slot_bit); 
void release_hash_list(struct hlist_head_ex* h, hlist_data_fn fn);
void add_hash_list(struct hlist_head_ex* h, struct hash_node* data);
void del_hash_list(struct hlist_head_ex* h, struct hash_node* data);
struct hash_node* find_hash_list(struct hlist_head_ex* h, unsigned long key);

void traverse_hash_list(struct hlist_head_ex* h, hlist_data_fn fn);
#endif

