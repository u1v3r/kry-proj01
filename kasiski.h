/*
 * kasiski.h
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#ifndef KASISKY_H_
#define KASISKI_H_

#include "definitions.h"
#include "other_fcs.h"
#include "simple_hash_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct kasiski_node{
	struct kasiski_node *next;	/* ukazatel na dalsi uzol */
	char *ngram;				/* text ngramu */
	int *positions;				/* pozicie ngramu */
	unsigned int _ngram_max_pos;
	int count;					/* pocitadlo ngramov v texte */
	unsigned int len;			/* dlzka ngramu */
	int *distances;				/* vzdialenosti medzi ngram */
	/*float weight;*/			/* vaha pouzivana pri vypocte gcd*/
	/*int dist_sum;*/
	int gcd;
} kasiski_node_t;

void *kasiski_test(void *c_text_s);
void find_ngrams(const char *c_text,unsigned int c_len);
void add_position(kasiski_node_t *ngram, int position, int ngram_len);
kasiski_node_t *create_default_ngram_node(char *ngram);
void free_ngram_node(kasiski_node_t* ngram_node);
void actualize_gcd_counter(kasiski_node_t* ngram_node);
void search_rep(const char *search, const char *text, int sub_len,int text_len, kasiski_node_t *ngram);
void kasisky_hashmap_to_list(hash_table_node_t *hash_table[], int hash_table_size, kasiski_node_t *res[NGRAM_STOP], int *sizes);
hash_table_node_t *gcdcount_hashmap_to_list(hash_table_node_t *hash_table[],int size);
hash_table_node_t *process_ngrams();
int q_compare_ngram(const void * a, const void * b);
int q_compare_gcd(const void * a, const void * b);
int q_compare_int(const void * a, const void * b);

#endif /* KASISKY_H_ */
