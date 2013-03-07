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
	struct kasiski_node *next;	/* dalsi uzol */
	char *ngram;				/* text ngramu */
	int *positions;				/* pozicie ngramu */
	unsigned int count;			/* pocitadlo ngramov v texte */
	int *distances;				/* vzdialenosti medzi ngram */
	float weight;				/* vaha pouzivana pri vypocte gcd*/
	/*int dist_sum;*/
	int gcd;
} kasisky_node_t;



void *kasiski_test(void *c_text_s);
void find_ngrams(const char *c_text,unsigned int c_len);
void calc_factors();
void add_ngram(kasisky_node_t *ngram, int ngram_size);
kasisky_node_t *create_default_ngram_node(char *ngram, int position);
void add_position(kasisky_node_t *ngram, int position, int ngram_len);
kasisky_node_t *create_empty_ngram_node(void);
kasisky_node_t *search_ngram(kasisky_node_t *node, char *ngram);
void free_ngram_node(kasisky_node_t* ngram_node);
void actualize_gcd_counter(kasisky_node_t* ngram_node);
int hash_table_max();

#endif /* KASISKY_H_ */
