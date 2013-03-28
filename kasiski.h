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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void *kasiski_test(void *c_text_s, double friedman);
void find_ngrams(const char *c_text,unsigned int c_len);
void add_position(kasiski_node_t *ngram, int position, int ngram_len);
kasiski_node_t *create_default_ngram_node(char *ngram);
void free_ngram_node(kasiski_node_t* ngram_node);
/*void actualize_gcd_counter(kasiski_node_t* ngram_node);*/
void search_rep(const char *search, const char *text, int sub_len,int text_len, kasiski_node_t *ngram);
void prep(const char *x, int m, int bad_char_table[]);
void quick_text_search(const char *search,const char *text, int subl_len, int text_len, kasiski_node_t *ngram);
void kasisky_hashmap_to_list(hash_table_node_t *hash_table[], int hash_table_size, kasiski_node_t *res[NGRAM_STOP], int *sizes);
hash_table_node_t *gcdcount_hashmap_to_list(hash_table_node_t *hash_table[],int size);
kasiski_thread_result_t *process_ngrams();
int q_compare_ngram(const void * a, const void * b);
int q_compare_gcd(const void * a, const void * b);
int q_compare_int(const void * a, const void * b);

#endif /* KASISKY_H_ */
