#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
/*init the hash table*/
HASH* init_hash(int total,unsigned int (*hash_function)(int total,void* key))
{
	int i = 0;
	HASH_NODE* node = NULL;
	HASH* hash = (HASH*)malloc(sizeof(HASH));
	if (hash == NULL)
		return NULL;
	memset(hash,0,sizeof(HASH));
	hash -> total = total;
	hash -> hash_function = hash_function;
	hash -> array = (HASH_NODE**)malloc(sizeof(HASH_NODE*) * total);
	if (hash -> array == NULL)
		return NULL;
	for (i = 0; i < total; i++)
	{
		node = (HASH_NODE*)malloc(sizeof(HASH_NODE));
		if (node == NULL)
			return NULL;
		memset(node,0,sizeof(HASH_NODE));
		(hash -> array)[i] = node;
	}
	return hash;
}

/*
search by key
if compare function return 0,node -> key equal key
*/
HASH_NODE * hash_search(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key))
{
	unsigned int index = p_hash -> hash_function(p_hash->total,key);	
	HASH_NODE* q = NULL;
	for (q = (p_hash->array)[index]; q -> next != NULL; q = q -> next)
	{
		if (compare(q,key) == 0)
			return q;
	}
	return NULL;	
}

/*
insert a node into hashtable
if compare function return 0,node -> key equal key
*/
HASH_NODE* hash_insert(HASH* p_hash,void* key,void* value,int size_key,int size_value,int (*compare)(HASH_NODE* node,void* key))
{
	HASH_NODE* node = NULL;
	unsigned int index = p_hash -> hash_function(p_hash->total,key);
	HASH_NODE* p = hash_search(p_hash,key,compare);
	void* v = (void*)malloc(size_value);
	if (v == NULL)
		return NULL;		
	memset(v,0,size_value);
	memcpy(v,value,size_value);
	
	void* k = (void*)malloc(size_key);
	if (k == NULL)
		return NULL;
	memset(k,0,size_key);
	memcpy(k,key,size_key);

	if (p != NULL)
	{
		free(p -> value);
		p -> value = v;	
		return p;
	}
	else
	{
		node = (HASH_NODE*)malloc(sizeof(HASH_NODE));/*calloc result the segument*/
		if (node == NULL)
			return NULL;
		memset(node,0,sizeof(HASH_NODE));
		node -> value = v;
		node -> key = k;
		node -> next = p_hash->array[index];
		(p_hash -> array)[index] -> prev = node;
		node -> prev = NULL;
		(p_hash -> array)[index] = node;
		return node;
	}
}

/*
get the value by key
if compare function return 0,node -> key equal key
*/
void* hash_getvalue(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key))
{
	if (p_hash == NULL)
		return NULL;
	HASH_NODE* p = hash_search(p_hash,key,compare);
	if (p == NULL)
		return NULL;
	else
		return (p -> value);
}

/*
delete a node by key
if compare function return 0,node -> key equal key
*/
void hash_delete(HASH* p_hash,void* key,int (*compare)(HASH_NODE* node,void* key))
{
	unsigned int i = p_hash -> hash_function(p_hash -> total,key);
	HASH_NODE* p = hash_search(p_hash,key,compare);
	if (p == NULL)
		return;
	if (p -> prev == NULL)/*first node*/
	{
		p -> next -> prev = (p_hash -> array)[i];
		(p_hash -> array)[i] = p -> next;
	}
	else
	{
		p -> prev -> next = p -> next;
		p -> next -> prev = p -> prev;
	}
	free(p -> value);
	free(p -> key);
	free(p);
}

/*
destroy the hashtable
*/
void hash_destroy(HASH** p_hash)
{
	int i;
	HASH_NODE* node = NULL,*p = NULL;
	(*p_hash) -> hash_function = NULL;
	for (i=0; i< (*p_hash) -> total; i++)
	{
		node = ((*p_hash) -> array)[i];
		p = node -> next;
		while (p != NULL)
		{
			free(node -> value);
			free(node -> key);
			free(node);
			node = p;
			p = p -> next;			
		}
		free(p);/*delete the last node that has not data*/
	}
	free((*p_hash) -> array);
	(*p_hash) -> array = NULL;
	(*p_hash) = NULL;
	return;
}
/*print the hashtable*/
void hash_print(HASH* p_hash)
{
	HASH_NODE* p = NULL;
	int i;
	for (i=0; i < p_hash -> total; i++)
	{
		p = (p_hash -> array)[i];
		while (p -> next != NULL)
		{
			printf("%d\t%d\n",*((int*)p->key),*((int*)p->value));
	//		printf("%s\t%s\n",(char*)p->key,(char*)p->value);
			p = p -> next;
		}
		 	
	}
}

/**************ip,count*****************************/
int compare_pid(HASH_NODE* node,void* key)
{
	if (*((int*)(node -> key)) == *((int*)(key)))
		return 0;
	return 1;
} 
/*hast function*/
unsigned int hash_fun(int total,void* key)
{	
	unsigned int index =0;
	char *p = (char*)key;
	while(*p)
	{
		index = *p + 31*index;
		p++;
	}
	return index % total;
}
/*************pid,ip*******************************/
/*hast function*/
unsigned int hash_fun_pid(int total,void* key)
{	
	int m = *((int*)key);
	int sum = 0;
	while(m != 0)
	{
		sum += m % 10;
		m = m / 10;
	}	
	return sum % total;
}
int compare(HASH_NODE* node,void* key)
{
	if (strcmp((char*)(node -> key),(char*)key) == 0)
	 	return 0;
	return 1;
}



