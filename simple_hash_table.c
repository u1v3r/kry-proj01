/*
 ============================================================================
 Name        : simple_hash_table.c
 Author      : Radovan Dvorsky
 Description : Simple hash table implementation
 ============================================================================
 */

#include "simple_hash_table.h"

void t_insert(void *key, void *value, unsigned int (*hash_fnc)(void *f_value, unsigned int size),hash_table_node_t *hash_table[], size_t key_s, size_t value_s, unsigned int hash_table_size){
	unsigned int hash = (*hash_fnc)(key, hash_table_size);

	if(hash_table[hash] == NULL){
		hash_table[hash] = (hash_table_node_t *) malloc(sizeof(hash_table_node_t));
		hash_table[hash]->key = (void *)malloc(key_s);
		memcpy(hash_table[hash]->key,key,key_s);
		hash_table[hash]->value = (void *)malloc(value_s);
		memcpy(hash_table[hash]->value,value,value_s);
		hash_table[hash]->next = NULL;
	}else{
		hash_table_node_t *node = hash_table[hash];

		while(node->next != NULL){
			node = node->next;
		}

		node->next = (hash_table_node_t *) malloc(sizeof(hash_table_node_t));
		node->next->key = (void *)malloc(key_s);
		memcpy(node->next->key,key,key_s);
		node->next->value = (void *)malloc(value_s);
		memcpy(node->next->value,value,value_s);
		node->next->next = NULL;
	}

}

hash_table_node_t *t_search(void *key, unsigned int (*hash_fnc)(void *f_value, unsigned int size), int (*comp)(void *a, void *b),hash_table_node_t *hash_table[], unsigned int hash_table_size){
	unsigned int hash = (*hash_fnc)(key,hash_table_size);
	hash_table_node_t *node = hash_table[hash];

	while(node != NULL){
		if((*comp)(node->key,key) == 1){
			return node;
		}
		node = node->next;
	}

	return NULL;
}


