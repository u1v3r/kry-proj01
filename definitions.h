/*
 * definitions.h
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define _POSIX_C_SOURCE 200809L
#define DEBUG 0

#define NGRAMS_DEFAULT_SIZE 1000 		/* alokacia - pocet vsetkych ngramov v subore */
#define NGRAMS_DEFAULT_POSITIONS 200 	/* alokacia - pocet alokovanej pamate pre pole pozicii ngramu v texte */
#define NGRAM_START 3 			   		/* pociatocna dlzka hladanych n-gramov */
#define NGRAM_STOP 5
#define NGRAM_MIN_COUNT 2				/* od akoho poctu ngramov sa zohladnuju a ukladaju */
#define SIZE_HASH_TABLE_GCDS 199		/* velkost hash table pre ukladanie vsetkych gcd */
#define SIZE_HASH_TABLE_NGRAMS 10007	/* velkost hash table pre ukladanie ngramov */
#define SIZE_HASH_TABLE_GCD_COUNT 59
#define INPUT_TEXT_MAX_LENGTH 8000 	/* dlzka textu ktory sa zo vstupu pouzije na kasiskyho test */
#define NGRAM_MIN_WEIGHT 3.4			/* koeficiant weight pri ktorom sa ngram zohladnuje do konecneho vysledku */
#define NGRAM_COUNT_CONST 0.2
#define NGRAM_LEN_CONST 0.4
#define LETTERS 26
#define HASHMAP_LIST_MAX_SIZE 500
#define NGRAMS_GCDS_COUNT 20			/* pocet ngramov, ktore sa zohladnuju pri vypocte do kasiskyho testu */


typedef struct _input_text{
	char *text;							/* skrateny text na hodnotu INPUT_TEXT_MAX_LENGHT */
	char *orig_text;					/* originalny text s odstranenymi zbytocnnymi znakmi*/
} input_text_t;

#endif /* DEFINITIONS_H_ */
