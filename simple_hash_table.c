/*
 ============================================================================
 Name        : simple_hash_table.c
 Author      : Radovan Dvorsky
 Description : Simple hash table implementation
 ============================================================================
 */

#include "simple_hash_table.h"

void t_insert(void *key, void *value, unsigned int (*hash_fnc)(void *f_value),hash_table_node_t *hash_table[]){
	unsigned int hash = (*hash_fnc)(key);

	if(hash_table[hash] == NULL){
		hash_table[hash] = (hash_table_node_t *) malloc(sizeof(hash_table_node_t));
		hash_table[hash]->key = (void *)malloc(sizeof(int));
		memcpy(hash_table[hash]->key,key,sizeof(int));
		hash_table[hash]->value = (void *)malloc(sizeof(int));
		memcpy(hash_table[hash]->value,value,sizeof(int));
		hash_table[hash]->next = NULL;
	}else{
		hash_table_node_t *node = hash_table[hash];
		while(node->next != NULL){
			node = node->next;
		}

		node->next = (hash_table_node_t *) malloc(sizeof(hash_table_node_t));
		node->next->key = (void *)malloc(sizeof(int));
		memcpy(node->next->key,key,sizeof(int));
		node->next->value = (void *)malloc(sizeof(int));
		memcpy(node->next->value,value,sizeof(int));
		node->next->next = NULL;
	}

}

hash_table_node_t *t_search(void *key, unsigned int (*hash_fnc)(void *f_value), int (*comp)(void *a, void *b),hash_table_node_t *hash_table[]){
	unsigned int hash = (*hash_fnc)(key);
	hash_table_node_t *node = hash_table[hash];

	while(node != NULL){
		if((*comp)(node->key,key) == 1){
			return node;
		}
		node = node->next;
	}


	return NULL;
}
