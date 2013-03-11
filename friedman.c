/*
 * friedman.c
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#include "friedman.h"

void *friedman_test(void *ptr){

	char *c_text_s = ptr;
	/*unsigned long c, i = 0, j = 0;*/
	double *friedman = (double *)malloc(sizeof(double));
	double k0 = 0.0, kp = 0.065, kr = 0.0385;
	double len = strlen(c_text_s);

	*friedman = 0.0;

	if(len < 1) return friedman;

	/*
	while((c = c_text_s[i]) != 0){
		letters[c - 'a']++;

		if(i % COLUMNS == 0 && i != 0){
			j++;
		}

		if(row_len == j){
			break;
		}

		if(j == 0){
			ics[i % COLUMNS] = (char *)calloc(row_len+2,sizeof(char));
		}

		ics[i % COLUMNS][j] = c;


		i++;
	}

	for (i = 0; i < 26; ++i) {
		lf += (letters[i]*(letters[i] - 1));
	}


	printf("skupiny:\n");
	for (i = 0; i < COLUMNS; ++i) {
		printf("%d - %s\n",i,ics[i]);
	}

	k0 = lf/(len*(len - 1));
*/
#if DEBUG == 1

	long sum = 0;

	for (i = 0; i < LETTERS; ++i) {
		printf("%c - %d\n",(i + 'a'),letters[i]);
		sum += letters[i];
	}

	printf("sum:%d\n",sum);
	printf("len: %d\n",len);
	printf("ko(%f)\n",lf);
	printf("(kp(%f) - kr(%f))/(k0(%f) - kr(%f))\n",kp,kr,k0,kr);
#endif

	k0 = index_of_coincidence(c_text_s,len);
	*friedman = (kp - kr)/(k0 - kr);

	/*test_len(c_text_s,len,(floor(*friedman)));*/

	return friedman;
}

void test_len(char *string, double len, hash_table_node_t *m_numbers){

	char **ics;
	char c;
	unsigned long i = 0 , j = 0, k, m, result_m;
	double row_len,diff_sum = 0.0, ic_min = 1;
	long column_max = 500;


	ics = (char **)malloc(column_max * sizeof(char *));

	for (m = 0; m < 10; m++) {

		row_len = len/m;
		j = 0;
		i = 0;

		while((c = string[i]) != 0){

			if(i % m == 0 && i != 0){
				j++;
			}

			if(row_len == j){
				break;
			}

			if(j == 0){
				ics[i % m] = (char *)calloc(row_len+2,sizeof(char));
			}

			ics[i % m][j] = c;


			i++;
		}



		diff_sum = 0;
		for (k = 0; k < m; ++k) {
			diff_sum += fabs(index_of_coincidence(ics[k],strlen(ics[k])) - 0.0667);
			free(ics[k]);
		}

		/*printf("%d - %f < %f\n",m,diff_sum,ic_min);*/

		if(diff_sum < ic_min){
			ic_min = diff_sum;
			result_m = m;
		}

	}

	printf("result: %lu\n",result_m);

	free(ics);
}

double index_of_coincidence(char *x, long len){

	double lf = 0.0;
	double freqs[LETTERS] = {0.0};
	unsigned long i = 0;

	letter_freqs(x,freqs);

	for (i = 0; i < LETTERS; ++i) {
		lf += (freqs[i]*(freqs[i] - 1));
	}



	return lf/(len*(len - 1));
}

void letter_freqs(char *input, double freqs[]){

	char c;
	unsigned long i = 0;

	while((c = input[i++]) != 0){
		freqs[c - 'a']++;
	}
}
