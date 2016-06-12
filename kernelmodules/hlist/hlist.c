#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include "hlist.h"

#define HASH_KEY(key,bit) (hash_long(key, bit))


struct hlist_head_ex* init_hash_list(unsigned char hash_slot_bit)
{
	int i = 0;
	struct hlist_head_ex* h = kzalloc(sizeof(struct hlist_head_ex), GFP_KERNEL);
	BUG_ON(!h);
#define HASH_SLOT(slot_bit) (1 << slot_bit)
	h->hhead = kzalloc(sizeof(struct hlist_head) * HASH_SLOT(hash_slot_bit) , GFP_KERNEL);
	BUG_ON(!h->hhead);
	for (i = 0; i < HASH_SLOT(hash_slot_bit); i++)
		INIT_HLIST_HEAD(&h->hhead[i]);
	h->bit = hash_slot_bit;
	spin_lock_init(&h->hlist_lock);
	return h;
}

void release_hash_list(struct hlist_head_ex* h, hlist_data_fn fn)
{
	traverse_hash_list(h, fn);		
	kfree(h->hhead);
	kfree(h);
}

void traverse_hash_list(struct hlist_head_ex* h, hlist_data_fn fn)
{
	int i = 0;
	struct hlist_node *entry, *next;
	struct hash_node* data = NULL;
	struct hlist_head * hash_list;
	BUG_ON(!h);
	if(fn) {
		for(i = 0; i < HASH_SLOT(h->bit); i++) {
			spin_lock(&h->hlist_lock);
			hash_list = &h->hhead[i];
			hlist_for_each_entry_safe(data, entry, next, hash_list, node) {	
				fn(data);
			}
			spin_unlock(&h->hlist_lock);
		}
	}
}

void add_hash_list(struct hlist_head_ex* h, struct hash_node* data)
{
	if(!h->hhead|| !data)
		return;
	if(!hlist_unhashed(&data->node)) {
		printk(KERN_ERR" hash node has been in hash list.\n");
		return ;
	}
	spin_lock(&h->hlist_lock);
	hlist_add_head(&data->node, &h->hhead[HASH_KEY(data->key, h->bit)]);
	spin_unlock(&h->hlist_lock);
}

void del_hash_list(struct hlist_head_ex* h, struct hash_node* data)
{
	if(!data)
		return;
	if(hlist_unhashed(&data->node)) {
		printk(KERN_ERR"hash node isn`t in hash list.\n");
		return ;
	}
	spin_lock(&h->hlist_lock);
	hlist_del_init(&data->node);
	spin_unlock(&h->hlist_lock);
}

struct hash_node* find_hash_list(struct hlist_head_ex* h, unsigned long key)
{
	struct hlist_head *hash_list = &h->hhead[HASH_KEY(key, h->bit)];
	struct hlist_node *entry, *next;
	struct hash_node* data = NULL;
	spin_lock(&h->hlist_lock);
	hlist_for_each_entry_safe(data, entry, next, hash_list, node) {
		BUG_ON(hlist_unhashed(&data->node));
		if(data->key == key)
			goto END;
	}
END:
	spin_unlock(&h->hlist_lock);
	return data;
}


