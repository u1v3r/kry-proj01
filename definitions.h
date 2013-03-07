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
#define NGRAM_MIN_COUNT 3				/* od akoho poctu ngramov sa zohladnuju a ukladaju */
#define SIZE_HASH_TABLE_GCDS 199		/* */
#define SIZE_HASH_TABLE_NGRAMS 10000	/* */
#define INPUT_TEXT_MAX_LENGTH 5000 		/* dlzka textu ktory sa zo vstupu pouzije */

typedef struct _input_text{
	char *text;							/* skrateny text na hodnotu INPUT_TEXT_MAX_LENGHT*/
	char *orig_text;					/* originalny text s odstranenymi zbytocnnymi znakmi*/
	/*unsigned int len;*/					/* dlzka originalneho textu */
} input_text_t;

#endif /* DEFINITIONS_H_ */
