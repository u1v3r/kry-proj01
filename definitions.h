/*
 * definitions.h
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define _POSIX_C_SOURCE 200809L

#include "simple_hash_table.h"

#define DEBUG 0
/*#define PROFILE*/
/*#define TIMES*/

#define INDEX_OF_COINCIDENCE 0.067
#define NGRAMS_DEFAULT_SIZE 1000 		/* alokacia - pocet vsetkych ngramov v subore */
#define NGRAMS_DEFAULT_POSITIONS 200 	/* alokacia - pocet alokovanej pamate pre pole pozicii ngramu v texte */
#define NGRAM_START 3 			   		/* pociatocna dlzka hladanych n-gramov */
#define NGRAM_STOP 5
#define NGRAM_MIN_COUNT 2				/* od akoho poctu ngramov sa zohladnuju a ukladaju */
#define SIZE_HASH_TABLE_GCDS 199		/* velkost hash table pre ukladanie vsetkych gcd */
#define SIZE_HASH_TABLE_NGRAMS 110017	/* velkost hash table pre ukladanie ngramov */
#define SIZE_HASH_TABLE_ALL_NGRAMS 10007/* velkost hash table pre ukladanie vsetkych nramov (aj neopakujucich) */
#define SIZE_HASH_TABLE_GCD_COUNT 5351
#define INPUT_TEXT_MAX_LENGTH 15000 	/* dlzka textu ktory sa zo vstupu pouzije na kasiskyho test */
#define TEXT_TO_COLUMN_MAX 1000			/* maximalny pocet buniek pri alokovani */
#define LETTERS 26
#define HASHMAP_LIST_MAX_SIZE 500
#define NGRAMS_GCDS_COUNT 30			/* pocet ngramov, ktore sa zohladnuju pri vypocte do kasiskyho testu */
#define KASISKY_IC_DIFF	0.000040		/* rozdiel medzi hodnotami (kasiski - IC), pri ktorom sa uprednostni kasisky pred IC */


typedef struct _input_text{
	char *text;							/* skrateny text na hodnotu INPUT_TEXT_MAX_LENGHT */
	char *orig_text;					/* originalny text s odstranenymi zbytocnnymi znakmi*/
} input_text_t;

typedef struct _thread_result{
	unsigned int len;					/* pocet vratenych hodnot */
	struct _hash_table_node *values;	/* pole hodnot */
} kasiski_thread_result_t;

typedef struct _kasiski_node{
	struct _kasiski_node *next;			/* ukazatel na dalsi uzol */
	int *positions;						/* pozicie ngramu */
	unsigned int _ngram_max_pos;
	int count;							/* pocitadlo ngramov v texte */
	unsigned int len;					/* dlzka ngramu */
	int *distances;						/* vzdialenosti medzi ngram */
	int gcd;
} kasiski_node_t;

typedef struct _test{
	const char *string;
	unsigned int start;
	unsigned int len;
} test_t;

#endif /* DEFINITIONS_H_ */
