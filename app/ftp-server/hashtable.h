#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _HASH_
#define _HASH_
/******hash structure*********/
typedef struct _hash_node_
{
	void* key;
	void* value;
	struct _hash_node_ *prev;
	struct _hash_node_ *next;
}HASH_NODE;

typedef struct _hashtable_
{
	int total;
	HASH_NODE** array;
	unsigned int (*hash_function)(int total,void* key);
}HASH;

/*******hash function*********/
/*init the hash table*/
HASH* init_hash(int total,unsigned int (*hash_function)(int total,void* key));

/*search by key*/
HASH_NODE* hash_search(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key));

/*insert a node into hashtable*/
HASH_NODE * hash_insert(HASH* p_hash,void* key,void* value,int size_key,int size_value,int (*compare)(HASH_NODE* node,void* key));

/*get the value by key*/
void* hash_getvalue(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key));

/*delete a node by key*/
void hash_delete(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key));
/*
destroy the hashtable
*/
void hash_destroy(HASH** p_hash);

/*print the hashtable*/
void hash_print(HASH* p_hash);

/*hash function*/
unsigned int hash_fun(int total,void* key);

int compare(HASH_NODE* node,void* key);

unsigned int hash_fun_pid(int total,void* key);

int compare_pid(HASH_NODE* node,void* key);

#endif
