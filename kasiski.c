/*
 * kasiski.c
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#include "kasiski.h"

int mallock_ngrams_count = 0;						/* pocitadlo aktualneho stavu ulozenych ngramov */
int ngrams_max_size = NGRAMS_DEFAULT_SIZE;			/* inicializacia maximalnej hodnoty ngramov v tabulke */
int ngram_pos_max_size = NGRAMS_DEFAULT_POSITIONS;
kasisky_node_t* last_ngram;
int last_ngram_size;
int last_ngram_index = NGRAM_STOP;
int result_gcd = 0;
int ngram_count[NGRAM_STOP + 1];
int ngram_pos_counter = 0;

/*int *gcds;*/

static kasisky_node_t *kasiski_ngrams_table[NGRAM_STOP + 1];
static hash_table_node_t *hash_table_gcds[SIZE_HASH_TABLE_GCDS];
static hash_table_node_t *hash_table_ngrams[SIZE_HASH_TABLE_NGRAMS];

void *kasiski_test(void *c_text_p){

	int *kasisky = (int *)malloc(sizeof(int));
	input_text_t *c_text_s = c_text_p;

	find_ngrams(c_text_s->text,strlen(c_text_s->text));
	*kasisky = hash_table_max();

#if DEBUG == 1
	calc_factors();
#endif

	return kasisky;

}

void calc_factors(){
#if DEBUG == 1
	printf("last_ngram_index: %d\n",last_ngram_index);
#endif

	int i, j, ngram_avg_pos = 0;
	/* int gcd_avg,gcd_sum = 0, counter = 0, gcd_weight = 0; */
	float max_w = 0;
	kasisky_node_t *node, *max_node = NULL;

	ngram_avg_pos = ngram_pos_counter/mallock_ngrams_count;


	printf("\n\n");
	printf("%d/%d == %d\n",ngram_pos_counter,mallock_ngrams_count,ngram_avg_pos);


	for (i = last_ngram_index; i >= NGRAM_START; --i) {
		node = kasiski_ngrams_table[i];

		while(node != NULL){

			/* ak je pocet ngramov mensi ako priemer, tak preskoc */
			if(node->count < ngram_avg_pos) {
				node = node->next;
				continue;
			}

			/* mensie nema moc vyznam
			if(node->gcd < 2) {
				node = node->next;
				continue;
			}


			gcd_weight = (float)(gcd_avg > node->gcd ? (float)gcd_avg/(float)node->gcd : (float)node->gcd/(float)gcd_avg);
			printf("%d / %d == %f\n",gcd_avg,node->gcd,gcd_weight);

			cim blizsie k priemeru tým vyznamnejsie
			printf("delim: 1/%d - %d == %f\n",gcd_avg,node->gcd,1/(float)(abs(gcd_avg - node->gcd)));
			node->weight *= 1/(float)(abs(gcd_avg - node->gcd));

			if(gcd_weight > 2){
				node = node->next;
				continue;
			}

			 odstran ngram s velkou odchylkou od priemeru
			if(gcd_avg > gcd*1.3 || gcd_avg < gcd/1.3){
				node = node->next;
				continue;
			}
*/

			printf("\nngram: %s\n",node->ngram);
			for (j = 0; j < node->count; ++j) {
				printf("pos: %d,",node->positions[j]);
			}
			printf("\n");
			for (j = 0; j < node->count - 1; ++j) {
				printf("dis: %d,",node->distances[j]);
			}
			printf("\n");


			/* vypocitat priemerny pocet vyskytov a zohladnovat len vyssie */
			if(node->weight > max_w){
				max_w = node->weight;
				max_node = node;
			}


			printf("wieght: %f\n",node->weight);

			printf("gcd: %d\n",node->gcd);
			printf("\n\n");


			node = node->next;
		}
	}

	printf("vitaz: (%d)\n",hash_table_max());
	printf("max_weight:%s(%d) - %f\n",max_node->ngram,max_node->gcd,max_w);

}

void free_ngram_node(kasisky_node_t* ngram_node) {
	free(ngram_node->ngram);
	free(ngram_node->distances);
	free(ngram_node->positions);
	free(ngram_node);
}

void actualize_gcd_counter(kasisky_node_t* ngram_node) {

	hash_table_node_t *h_node;

	if ((h_node = t_search(&ngram_node->gcd, hash_int, compare_int,hash_table_gcds)) == NULL ) {
		/* pre kazdy novy zaznam v hash table zacni pocitat od 1 */
		int gcd_counter = 1;
		t_insert(&ngram_node->gcd, &gcd_counter, hash_int,hash_table_gcds);
	} else {
		/* inak inkrementuj citac vyskytov */
		(*((int*) h_node->value))++;
	}
}

void find_ngrams(const char *c_text,unsigned int c_len){

	int sub_len,i,j,tmp_pos_counter = 0;
	char *subst = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	char *test_subst = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	kasisky_node_t *ngram_node;

	/*gcds = (int *)malloc(ngrams_max_size * sizeof(int));*/

	/* pocitaj dlzku substringu */
	for(sub_len = NGRAM_START; sub_len <= NGRAM_STOP; sub_len++){
		ngram_count[sub_len] = 0;

		/* vytvaraj substring o danej dlzke */
		for(i = 0; i <= c_len - sub_len; i++){
			substring(subst,c_text,i,sub_len);
			/*subst = substring2(c_text,i,sub_len);*/

			/* ak sa uz nachadza v prehladavanych tak nehladaj znovu */
			if(search_ngram(kasiski_ngrams_table[sub_len],subst) != NULL){
#if DEBUG == 2
				printf("\"%s\" sa nasiel v tabulke hladanych - preskakujem\n",subst);
#endif
				continue;
			}

			ngram_node = create_default_ngram_node(subst,i);

			/* zacni hladat dany substring v texte */
			for(j = 0; j <= c_len - sub_len; j++){
				substring(test_subst,c_text,j,sub_len);
				/*test_subst = substring2(c_text,j,sub_len);*/
				/*printf("po druhom\n");*/
				if(strcmp(subst,test_subst) == 0 && i != j){
					add_position(ngram_node,j,sub_len);
					tmp_pos_counter++;
				}
			}

			/* ak sa nachdza v texte n-krat, tak vytvor strukturu */
			if(ngram_node->count > NGRAM_MIN_COUNT){
#if DEBUG == 2
				printf("Pre %s som nasiel %d ngramov\n",ngram_node->ngram,ngram_node->ngram_count);
#endif
				/* "-1" - count udava pocet vykytov, nie vzdialenosti */
				ngram_node->gcd = gcds_calc(ngram_node->distances,ngram_node->count - 1);
				add_ngram(ngram_node,sub_len);						/* pridaj ngram do tabulky ngramov */
				ngram_pos_counter += tmp_pos_counter;				/* aktualizuj citac celkoveho poctu pozicii (pouziva sa na priemer) */
				ngram_count[sub_len]++;								/* aktualizuj citac pre ngramy zvlast pre kazdu dlzku */
				mallock_ngrams_count++;								/* cekovy pocet ngramov */
				actualize_gcd_counter(ngram_node);					/* aktualizuj citac gcd */
			}else{
				/* ak je len jeden, tak uvolni pamat */
				free_ngram_node(ngram_node);
				tmp_pos_counter = 0;
			}
		}



		/* nenasiel sa ziadny ngram danej dlzky */
		if(ngram_count[sub_len] == 0){
#if DEBUG == 1
			printf("je to 0 na %d koncim\n",sub_len);
#endif
			last_ngram_index = sub_len - 1;
			break;
		}else {
#if DEBUG == 1
			printf("\n\npre %d-gramy som nasiel: %d\n\n",sub_len,ngram_count[sub_len]);
#endif
		}

	}

	free(subst);
	free(test_subst);
}

void add_ngram(kasisky_node_t *ngram, int ngram_index){

#if DEBUG == 2
	printf("%s ukladam na %d\n",ngram->ngram,ngram_index);
#endif

	if(ngram_index != last_ngram_size){
		kasiski_ngrams_table[ngram_index] = ngram;
	}else{
		last_ngram->next = ngram;
	}

	last_ngram = ngram;
	last_ngram_size = ngram_index;
}

kasisky_node_t *create_default_ngram_node(char *ngram, int position){

	kasisky_node_t *new_node = create_empty_ngram_node();

	new_node->ngram = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	strcpy(new_node->ngram,ngram);

	/*new_node->dist_sum = 0;*/
	new_node->positions[0] = position;
	new_node->count = 1;

	return new_node;
}

void add_position(kasisky_node_t *ngram, int position, int ngram_len){

	/* kontrola pamate */
	if(ngram->count >= ngram_pos_max_size - 1){
		ngram_pos_max_size *= 2;
		ngram->positions = (int *) realloc(ngram->positions,ngram_pos_max_size * sizeof(int));
		ngram->distances = (int *) realloc(ngram->distances,ngram_pos_max_size * sizeof(int));

		if(ngram->positions == NULL || ngram->distances == NULL){
			perror("ngram->position | distances allocation\n");
			exit(1);
		}
#if DEBUG == 2
		printf("Memory reallock: new ngram->positions size = %d\n",ngram_pos_max_size);
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


	ngram->weight = ngram->count * 0.2f;
	ngram->weight += ngram_len * 0.1f;

#if DEBUG == 2
	int i;
	printf("New ngram (%s) position (%d)\n",ngram->ngram,ngram->positions[ngram->ngram_count - 1]);
	printf("All positions/distances:\n");
	for (i = 0; i < ngram->ngram_count; ++i) {
		printf("pos: %d,",ngram->positions[i]);

		if(i < 1) continue;

		printf("dis: %d",ngram->distances[i - 1]);
	}
	printf("\n-------------------------------------\n");
#endif
}

kasisky_node_t *search_ngram(kasisky_node_t *node, char *ngram){

	/* prerobit na hash table */

	if(node == NULL) return NULL;

	if(strcmp(node->ngram,ngram) == 0){
		return node;
	}else{
		if(node->next != NULL){
			return search_ngram(node->next,ngram);
		}
	}

	return NULL;
}

kasisky_node_t *create_empty_ngram_node(void){

	/* alokacia pamate pre strukturu */
	kasisky_node_t *node = (kasisky_node_t *) malloc(sizeof(kasisky_node_t));

	if(node == NULL){
		perror("node mallock failed\n");
		exit(1);
	}

	/* alokacia pamate pre pozicie a vzdialenosti v ngrame */
	node->positions = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));
	node->distances = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));

	return node;
}

int hash_table_max(){

	int i, key = 0, max = 0;
	hash_table_node_t *node;

	for (i = 0; i < SIZE_HASH_TABLE_GCDS; ++i) {
		node = hash_table_gcds[i];
		while(node != NULL){
			if(*((int*)node->value) > max){
				max = *((int*)node->value);
				key = *((int*)node->key);
			}

			node = node->next;
		}
	}

	return key;
}
