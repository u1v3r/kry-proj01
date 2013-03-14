/*
 * kasiski.c
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#include "kasiski.h"

int mallock_ngrams_count = 0;						/* pocitadlo aktualneho stavu ulozenych ngramov */
int ngrams_max_size = NGRAMS_DEFAULT_SIZE;			/* inicializacia maximalnej hodnoty ngramov v tabulke */
/*int ngram_pos_max_size = NGRAMS_DEFAULT_POSITIONS;*/
kasiski_node_t* last_ngram;
/*int last_ngram_size;*/
int last_ngram_index = NGRAM_STOP;
int ngram_count[NGRAM_STOP + 1];
double friedman_result;
/*int ngram_pos_counter = 0;
int tmp_pos_counter = 0;*/
/*kasiski_node_t gcds[NGRAMS_GCDS_COUNT * NGRAM_STOP];*/

/*static kasisky_node_t *kasiski_ngrams_table[NGRAM_STOP + 1];*/
static hash_table_node_t *hash_table_gcds[SIZE_HASH_TABLE_GCDS];
static hash_table_node_t *hash_table_ngrams[SIZE_HASH_TABLE_NGRAMS];
static hash_table_node_t *hash_table_gcd_count[SIZE_HASH_TABLE_GCD_COUNT];

void *kasiski_test(void *c_text_p){

	input_text_t *c_text_s = c_text_p;
	int text_len = strlen(c_text_s->text);

#ifndef THREADS
	friedman_result = floor(c_text_s->friedman_res);
#endif

	/* najdi vsetky ngramy v text */
	find_ngrams(c_text_s->text,text_len);
	/**kasisky = gcd_hash_table_max();*/

	return process_ngrams();
}

kasiski_thread_result_t *process_ngrams(){

	int i, j;
	int *sizes;
	kasiski_node_t **res_list;
	hash_table_node_t *gcd_node,*values;
	int node_gcd, /*node_count,*/ node_len = 0;
	kasiski_thread_result_t *result = (kasiski_thread_result_t *)malloc(sizeof(kasiski_thread_result_t));

	/* inicializacie dvojrozmerne pole pre ulozenie poctu ngramov */
	res_list = (kasiski_node_t **)malloc((NGRAM_STOP + 1) * sizeof(kasiski_node_t *));
	for (i = NGRAM_START; i <= last_ngram_index; ++i) {
		res_list[i] = (kasiski_node_t *)malloc(ngram_count[i] * sizeof(kasiski_node_t));
	}

	sizes = (int *)calloc(NGRAM_STOP+1, sizeof(int));

	kasisky_hashmap_to_list(hash_table_ngrams,SIZE_HASH_TABLE_NGRAMS,res_list,sizes);


	for (i = NGRAM_START; i <= last_ngram_index; ++i) {
		qsort(res_list[i],sizes[i],sizeof(kasiski_node_t),q_compare_ngram);
		for (j = 0; j < NGRAMS_GCDS_COUNT; ++j) {
			if(j == sizes[i] || (res_list[i][j]).gcd < 1){
				break;
			}
			node_gcd = (res_list[i][j]).gcd;
			/*node_count = ((res_list[i][j]).count);
					printf("%s - %d",(((kasisky_node_t)res_list[i][j]).ngram),node_count);
			printf(" - %d\n",node_gcd);
			 */
			if((gcd_node = t_search(&node_gcd,hash_int,compare_int,hash_table_gcd_count,SIZE_HASH_TABLE_GCD_COUNT)) != NULL){
				(*((int *)gcd_node->value))++;
			}else{
				int node_value = 1;
				t_insert(&node_gcd,&node_value,hash_int,hash_table_gcd_count,sizeof(int),sizeof(int),SIZE_HASH_TABLE_GCD_COUNT);
				node_len++;
			}
		}

		free(res_list[i]);
	}


	free(sizes);
	free(res_list);

	values = gcdcount_hashmap_to_list(hash_table_gcd_count,node_len);

	qsort(values,node_len,sizeof(hash_table_node_t),q_compare_gcd);

#if DEBUG == 3
	printf("------------------kasiski----------------\n");
	printf("i: value - key\n");
	for (i = 0; i < node_len; ++i) {
		printf("%d: %d - %d\n",i,*((int *)values[i].value),*((int *)values[i].key));
	}
	printf("-----------------------------------------\n");
#endif


	result->len = node_len;
	result->values = values;

	return result;
}


void free_ngram_node(kasiski_node_t* ngram_node) {
	free(ngram_node->ngram);
	free(ngram_node->distances);
	free(ngram_node->positions);
	free(ngram_node);
}

void actualize_gcd_counter(kasiski_node_t* ngram_node) {

	hash_table_node_t *h_node;

	if(ngram_node->gcd <= 0) return;

	if ((h_node = t_search(&ngram_node->gcd, hash_int, compare_int,hash_table_gcds,SIZE_HASH_TABLE_GCDS)) == NULL ) {
		/* pre kazdy novy zaznam v hash table zacni pocitat od 1 */
		int gcd_counter = 1;
		t_insert(&ngram_node->gcd, &gcd_counter, hash_int,hash_table_gcds,sizeof(int),sizeof(int),SIZE_HASH_TABLE_GCDS);

	} else {
		/* inak inkrementuj citac vyskytov */
		(*((int*) h_node->value))++;
	}
}

void find_ngrams(const char *c_text,unsigned int c_len){

	unsigned int sub_len,i;
	char *subst = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	/*char *test_subst = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));*/
	kasiski_node_t *ngram_node;
	/*double weigth_sum = 0.0;
	double weight_avg = 3.1;
	*/
	/*gcds = (int *)malloc(ngrams_max_size * sizeof(int));*/

	/* pocitaj dlzku substringu */
	for(sub_len = NGRAM_STOP; sub_len >= NGRAM_START; sub_len--){
		ngram_count[sub_len] = 0;

		/* vytvaraj substring o danej dlzke */
		for(i = 0; i <= c_len - sub_len; i++){
			substring(subst,c_text,i,sub_len);
			if(t_search(subst,hash_char,compare_char,hash_table_ngrams,SIZE_HASH_TABLE_NGRAMS) != NULL){
				continue;
			}

			ngram_node = create_default_ngram_node(subst);
			search_rep(subst,c_text,sub_len,c_len,ngram_node);

			/* ak sa nachdza v texte n-krat, tak vytvor strukturu */
			if(ngram_node->count >= NGRAM_MIN_COUNT){

				/*weigth_sum += ngram_node->weight;*/
				ngram_node->len = sub_len;
				ngram_node->gcd = gcds_calc(ngram_node->distances,ngram_node->count - 1); /* "-1" - count udava pocet vykytov, nie vzdialenosti */


				/* odstranienie nadbytocnych 1,2,3 */
				if((ngram_node->gcd == 1 || ngram_node->gcd == 2 || ngram_node->gcd == 3) &&
						(mallock_ngrams_count % 2 == 0 || mallock_ngrams_count % 3 == 0 ||
								ngram_count[sub_len] % 2 == 0 || ngram_count[sub_len] % 3 == 0 ) ){

					free_ngram_node(ngram_node);
					/*tmp_pos_counter = 0;*/
					continue;
				}

				t_insert(ngram_node->ngram,ngram_node,hash_char,hash_table_ngrams,sizeof(char)*sub_len+1,sizeof(kasiski_node_t),SIZE_HASH_TABLE_NGRAMS);
				/*ngram_pos_counter += tmp_pos_counter;*/			/* aktualizuj citac celkoveho poctu pozicii (pouziva sa na priemer) */
				ngram_count[sub_len]++;								/* aktualizuj citac pre ngramy zvlast pre kazdu dlzku */
				mallock_ngrams_count++;								/* cekovy pocet ngramov */
				actualize_gcd_counter(ngram_node);					/* aktualizuj citac gcd */
			}else{
				/* ak je len jeden, tak uvolni pamat */
				free_ngram_node(ngram_node);
				/*tmp_pos_counter = 0;*/
			}
		}



		/* nenasiel sa ziadny ngram danej dlzky */
		if(ngram_count[sub_len] == 0){
#if DEBUG == 1
			printf("je to 0 na %d koncim\n",sub_len);
#endif
			last_ngram_index = sub_len - 1;
			/*weight_avg -= 1.0;*/
			break;
		}/*else{
			weight_avg = (weigth_sum/mallock_ngrams_count) - 0.01;
#if DEBUG == 1
			printf("priemer pre %d: %f\n",sub_len, weight_avg);
#endif
		}*/
		else {
#if DEBUG == 1
			printf("\n\npre %d-gramy som nasiel: %d\n\n",sub_len,ngram_count[sub_len]);
#endif
		}

	}

	free(subst);
	/*free(test_subst);*/
}

kasiski_node_t *create_default_ngram_node(char *ngram){

	/* alokacia pamate pre strukturu */
	kasiski_node_t *new_node = (kasiski_node_t *) malloc(sizeof(kasiski_node_t));

	if(new_node == NULL){
		perror("node mallock failed\n");
		exit(1);
	}

	/* alokacia pamate pre pozicie a vzdialenosti v ngrame */
	new_node->positions = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));
	new_node->distances = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));

	new_node->ngram = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	strcpy(new_node->ngram,ngram);

	/*new_node->dist_sum = 0;*/
	/*new_node->positions[0] = position;*/
	new_node->count = 0;
	new_node->_ngram_max_pos = NGRAMS_DEFAULT_POSITIONS;

	return new_node;
}

void add_position(kasiski_node_t *ngram, int position, int ngram_len){

	/* kontrola pamate */
	if(ngram->count >= ngram->_ngram_max_pos - 1){
		ngram->_ngram_max_pos *= 2;
		ngram->positions = (int *) realloc(ngram->positions,ngram->_ngram_max_pos * sizeof(int));
		ngram->distances = (int *) realloc(ngram->distances,ngram->_ngram_max_pos * sizeof(int));

		if(ngram->positions == NULL || ngram->distances == NULL){
			perror("ngram->position | distances allocation\n");
			exit(1);
		}
#if DEBUG == 2
		printf("Memory reallock: new ngram->positions size = %d\n",ngram->_ngram_max_pos);
#endif
	}

	/* vlozenie novej pozicie */
	ngram->count++;
	ngram->positions[ngram->count - 1] = position;


	/* vypocitanie vzdialenosti medzi poziciami */
	if(ngram->count < 2) return;

	/* distance[n - 1] = pos[n] - pos[0] */
	ngram->distances[ngram->count - 2] = ngram->positions[ngram->count - 1] - ngram->positions[0];

	/*ngram->dist_sum = ngram->dist_sum + (ngram->positions[ngram->count - 1] - ngram->positions[ngram->count - 2]);*/

/*
	ngram->weight = ngram->count * NGRAM_COUNT_CONST;
	ngram->weight += ngram_len * NGRAM_LEN_CONST;
*/


#if DEBUG == 2
	printf("\n-------------------------------------\n");
	int i;
	printf("New ngram (%s) position (%d)\n",ngram->ngram,ngram->positions[ngram->count - 1]);
	printf("All positions/distances:\n");
	for (i = 0; i < ngram->count; ++i) {
		printf("pos: %d,",ngram->positions[i]);

		if(i < 1) continue;

		printf("dis: %d",ngram->distances[i - 1]);
	}
	printf("\n-------------------------------------\n");
#endif
}


void search_rep(double search_start, const char *text, int sub_len,int text_len, kasiski_node_t *ngram){

	int i,j;

	/* prechadzaj cely text */
	for (i = 0; i < text_len; ++i) {
		/* postupne porovnavaj znaky */
		for (j = 0; j < sub_len; ++j) {
			if(text[i + j] != search_start[search_start + j]){
				break;
			}

			/* ak sa rovnaju vsetky znaky, tak pridaj */
			if(j == sub_len - 1){
				add_position(ngram,i+1,sub_len);
				/*tmp_pos_counter++;*/
			}
		}
	}

	/* prechadzaj cely text
	for (i = 0; i < text_len; ++i) {
		 postupne porovnavaj znaky
		for (j = 0; j < sub_len; ++j) {
			if(text[i + j] != search[j]){
				break;
			}

			 ak sa rovnaju vsetky znaky, tak pridaj
			if(j == sub_len - 1){
				add_position(ngram,i+1,sub_len);
			}
		}
	}

	*/
}

void kasisky_hashmap_to_list(hash_table_node_t *hash_table[], int hash_table_size, kasiski_node_t *res[NGRAM_STOP], int *sizes){

	/*unsigned int max_size = HASHMAP_LIST_MAX_SIZE;*/
	unsigned int i = 0, len = 0;
	hash_table_node_t *node;

	for (i = 0; i < hash_table_size; ++i) {
		node = hash_table[i];
		if(node != NULL){
			while(node != NULL){
				len = (*((kasiski_node_t *)node->value)).len;
				res[len][sizes[len]++] = *((kasiski_node_t *)node->value);
				node = node->next;
			}
		}
	}
}

hash_table_node_t *gcdcount_hashmap_to_list(hash_table_node_t *hash_table[],int size){

	unsigned int i = 0,j = 0;
	hash_table_node_t *node,*result;
	result = (hash_table_node_t *)malloc(sizeof(hash_table_node_t) * size);

	for (i = 0; i < SIZE_HASH_TABLE_GCD_COUNT; ++i) {
		node = hash_table[i];
		if(node != NULL){
			while(node != NULL){
				result[j++] = *node;
				node = node->next;
			}
		}
	}

	return result;
}

int q_compare_ngram(const void * a, const void * b){
	return (*((kasiski_node_t *)b)).count - (*((kasiski_node_t *)a)).count;
}

int q_compare_gcd(const void * a, const void * b){

	hash_table_node_t h_a = *((hash_table_node_t *)a);
	hash_table_node_t h_b = *((hash_table_node_t *)b);

	return (*((int*)h_b.value)) - (*((int*)h_a.value));
}

int q_compare_int(const void * a, const void * b){
	return ( *(int*)a - *(int*)b );
}
