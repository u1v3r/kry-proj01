#include "kasiski.h"

static int mallock_ngrams_count = 0;						/* pocitadlo aktualneho stavu ulozenych ngramov */
static int last_ngram_index = NGRAM_STOP;
static int ngram_count[NGRAM_STOP + 1];
static double friedman_result;
static int *all_pos;
static int all_pos_max = ALL_POS_COUNT;

void *kasiski_test(void *c_text_p, double friedman){

	input_text_t *c_text_s = c_text_p;
	int text_len = strlen(c_text_s->text);
	friedman_result = friedman;

#ifdef TIMES
	struct timeval start_time, end_time;
	long mtime, seconds, useconds;

	gettimeofday(&start_time, NULL);

#endif
	/* najdi vsetky ngramy v texte */
	find_ngrams(c_text_s->text,text_len);
#ifdef TIMES

	gettimeofday(&end_time, NULL);

	seconds  = end_time.tv_sec  - start_time.tv_sec;
	useconds = end_time.tv_usec - start_time.tv_usec;

	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	printf("find_ngrams(): %ld milliseconds\n", mtime);

#endif

#ifdef TIMES
	kasiski_thread_result_t *result;

	gettimeofday(&start_time, NULL);

	result = process_ngrams();

	gettimeofday(&end_time, NULL);

	seconds  = end_time.tv_sec  - start_time.tv_sec;
	useconds = end_time.tv_usec - start_time.tv_usec;

	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	printf("process_ngrams(): %ld milliseconds\n", mtime);

	return result;
#else
	/* spracuj vsetky najdene ngramy */
	return process_ngrams();
#endif
}

/* Vyhlada vsetky ngramy v texte */
void find_ngrams(const char *c_text,unsigned int c_len){

	unsigned int sub_len,i,all_pos_counter = 0, pos_counter = 0;
	char *subst = (char *)malloc((NGRAM_STOP + 1) * sizeof(char));
	kasiski_node_t *ngram_node;
	char *start_pos;
	int pos;
	all_pos = (int *)calloc(all_pos_max,sizeof(int));

	/* pocitaj dlzku substringu sub_len */
	for(sub_len = NGRAM_START; sub_len <= NGRAM_STOP; sub_len++){
		ngram_count[sub_len] = 0;
		all_pos_counter = pos_counter;
		pos_counter = 0;

		for(i = 0; i <= (sub_len == NGRAM_START ? c_len - sub_len : all_pos_counter - 1); i++){

			/* ostatne ngramy hladaj len na zaklade najdenych pozicii predchadzajucich ngramov */
			if(sub_len != NGRAM_START){
				substring(subst,c_text,all_pos[i],sub_len);
			}else{/* pri vyhladavani najkratsieho ngramu prechadzaj cely text */
				substring(subst,c_text,i,sub_len);
			}

			/* vklada a kontroluje ngram, aby sa rovnaky nevyhladaval znova */
			if(t_search(subst,hash_char,compare_char,hash_table_all_ngrams,SIZE_HASH_TABLE_ALL_NGRAMS) != NULL){
				continue;
			}
			t_insert(subst,subst,hash_char,hash_table_all_ngrams,sizeof(char)*sub_len+1,sizeof(char)*sub_len+1,SIZE_HASH_TABLE_ALL_NGRAMS);

			/* najskor zisti ci ma zmysel vobec zacinat hladat */
			if((start_pos = strstr(c_text,subst)) != NULL){
				pos = start_pos - c_text;
				ngram_node = create_default_ngram_node(subst);
				add_position(ngram_node,pos,sub_len,&pos_counter);
				substring_search(subst,start_pos,sub_len,ngram_node,c_text,&pos_counter);

				/* ak sa nachdza v texte n-krat, tak vypocitaj gcd a odstran bordel */
				if(ngram_node->count >= NGRAM_MIN_COUNT){

					ngram_node->len = sub_len;
					ngram_node->gcd = gcds_calc(ngram_node->distances,ngram_node->count - 1); /* "-1" - count udava pocet vykytov, nie vzdialenosti */
					/*printf("%d - %s\n",i,subst);*/

					/* odstranienie nadbytocnych 1,2,3 */
					if(((ngram_node->gcd == 1 || ngram_node->gcd == 2 || ngram_node->gcd == 3) &&
							(mallock_ngrams_count % 2 == 0 || ngram_count[sub_len] % 2 == 0))){
						free_ngram_node(ngram_node);
						continue;
					}

					t_insert(subst,ngram_node,hash_char,hash_table_ngrams,sizeof(char)*sub_len+1,sizeof(kasiski_node_t),SIZE_HASH_TABLE_NGRAMS);
					ngram_count[sub_len]++;								/* aktualizuj citac pre ngramy zvlast pre kazdu dlzku */
					mallock_ngrams_count++;								/* cekovy pocet ngramov */
				}

				free_ngram_node(ngram_node);
			}
		}


		/* nenasiel sa ziadny ngram danej dlzky */
		if(ngram_count[sub_len] == 0){
#if DEBUG == 1
			printf("je to 0 na %d koncim\n",sub_len);
#endif
			last_ngram_index = sub_len - 1;
			break;
		} else {
#if DEBUG == 1
			printf("\n\npre %d-gramy som nasiel: %d\n\n",sub_len,ngram_count[sub_len]);
#endif
		}

	}
	free(subst);
	free(all_pos);
}

/* vyhladane ngramy zoradi podla cetnosti */
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

	sizes = (int *)calloc(NGRAM_STOP+2, sizeof(int));

	/* z hashmapy vytvor zoznam */
	kasisky_hashmap_to_list(hash_table_ngrams,SIZE_HASH_TABLE_NGRAMS,res_list,sizes);

	/* zorad vsetky ngramy pre kazdu dlzku zvlast */
	for (i = NGRAM_START; i <= last_ngram_index; ++i) {
		qsort(res_list[i],sizes[i],sizeof(kasiski_node_t),q_compare_ngram);
		for (j = 0; j < NGRAMS_GCDS_COUNT; ++j) {
			node_gcd = (res_list[i][j]).gcd;

			if(j == sizes[i] || node_gcd < 1){
				break;
			}

			/* odstrani vsetky 1 na zaklade friedmana */
			if((node_gcd == 1 && friedman_result > 1.5)) continue;
/*
			node_count = ((res_list[i][j]).count);
			printf("%d",node_count);
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

	/* vysledne gcd pre vsetky ngramy nasledne zorad samostatne v jednom zozname */
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

kasiski_node_t *create_default_ngram_node(){

	/* alokacia pamate pre strukturu */
	kasiski_node_t *new_node = (kasiski_node_t *) malloc(sizeof(kasiski_node_t));

	if(new_node == NULL){
		perror("node mallock failed\n");
		exit(1);
	}

	/* alokacia pamate pre pozicie a vzdialenosti v ngrame */
	new_node->positions = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));
	new_node->distances = (int *) malloc(NGRAMS_DEFAULT_POSITIONS * sizeof(int));
	new_node->count = 0;
	new_node->_ngram_max_pos = NGRAMS_DEFAULT_POSITIONS;

	return new_node;
}

void add_position(kasiski_node_t *ngram, int position, int ngram_len, unsigned int *all_pos_counter){

	/* kontrola pamate */
	if(ngram->count >= ngram->_ngram_max_pos - 1){
		ngram->_ngram_max_pos *= 2;
		ngram->positions = (int *) realloc(ngram->positions,ngram->_ngram_max_pos * sizeof(int));
		ngram->distances = (int *) realloc(ngram->distances,ngram->_ngram_max_pos * sizeof(int));

		if(ngram->positions == NULL || ngram->distances == NULL){
			perror("ngram->position | distances allocation\n");
			exit(1);
		}
	}

	/* vlozenie novej pozicie */
	ngram->count++;
	ngram->positions[ngram->count - 1] = position;

	/* vypocitanie vzdialenosti medzi poziciami pre mensie ako 2 nema zmysel */
	if(ngram->count < 2) return;

	/* ak je treba tak zvacsi pole */
	if(*all_pos_counter >= all_pos_max - 1){
		all_pos_max *= 2;
		all_pos = (int *)realloc(all_pos,all_pos_max * sizeof(int));
	}

	/* ulozi poziciu vyskytu daneho ngramu, ktora sa pouzije pri ngramov vacsej dlzky */
	all_pos[(*all_pos_counter)++] = position;

	/* distance[n - 1] = pos[n] - pos[0] */
	ngram->distances[ngram->count - 2] = ngram->positions[ngram->count - 1] - ngram->positions[0];
}


void free_ngram_node(kasiski_node_t* ngram_node) {
	free(ngram_node->distances);
	free(ngram_node->positions);
	free(ngram_node);
}

/* Vyhlada vsetky pozicie retazca v texte */
void substring_search(const char *search,const char *text, int subl_len, kasiski_node_t *ngram, const char *orig_text,unsigned int *all_pos_counter){

	int pos;
	char *ptr;

	while((ptr = strstr(text + 1,search)) != NULL){
		pos = ptr - orig_text;
		add_position(ngram,pos,subl_len,all_pos_counter);
		text = ptr;
	}
}

void kasisky_hashmap_to_list(hash_table_node_t *hash_table[], int hash_table_size, kasiski_node_t *res[NGRAM_STOP], int *sizes){

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
