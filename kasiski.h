#ifndef KASISKY_H_
#define KASISKI_H_

#include "definitions.h"
#include "other_fcs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef TIMES
#include "sys/time.h"
#endif

hash_table_node_t *hash_table_ngrams[SIZE_HASH_TABLE_NGRAMS];
hash_table_node_t *hash_table_gcd_count[SIZE_HASH_TABLE_GCD_COUNT];
hash_table_node_t *hash_table_all_ngrams[SIZE_HASH_TABLE_ALL_NGRAMS];

void *kasiski_test(void *c_text_s, double friedman);
void find_ngrams(const char *c_text,unsigned int c_len);
void add_position(kasiski_node_t *ngram, int position, int ngram_len, unsigned int *all_pos_counter);
kasiski_node_t *create_default_ngram_node();
void free_ngram_node(kasiski_node_t* ngram_node);
void substring_search(const char *search,const char *text, int subl_len, kasiski_node_t *ngram, const char *orig_text,unsigned int *all_pos_counter);
void kasisky_hashmap_to_list(hash_table_node_t *hash_table[], int hash_table_size, kasiski_node_t *res[NGRAM_STOP], int *sizes);
hash_table_node_t *gcdcount_hashmap_to_list(hash_table_node_t *hash_table[],int size);
kasiski_thread_result_t *process_ngrams();
int q_compare_ngram(const void * a, const void * b);
int q_compare_gcd(const void * a, const void * b);
int q_compare_int(const void * a, const void * b);

#endif /* KASISKY_H_ */
