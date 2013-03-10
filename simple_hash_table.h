/*
 * simple_hash_table.h
 *
 *  Created on: 6.3.2013
 *      Author: Radovan Dvorsky
 */

#ifndef SIMPLE_HASH_TABLE_H_
#define SIMPLE_HASH_TABLE_H_

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node{
	struct _node *next;
	void *key;
   	void *value;
} hash_table_node_t;

void
t_insert(void *key, void *value, unsigned int (*hash_fnc)(void *f_value),hash_table_node_t *hash_table[], size_t key_s, size_t value_s);

hash_table_node_t
*t_search(void *key, unsigned int (*hash_fnc)(void *f_value), int (*comp)(void *a, void *b),hash_table_node_t *hash_table[]);

#endif /* SIMPLE_HASH_TABLE_H_ */
