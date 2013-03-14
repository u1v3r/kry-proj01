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

	k0 = index_of_coincidence(c_text_s,len,0);
	*friedman = (kp - kr)/(k0 - kr);

	return friedman;
}

int ic_passwd_len(char *string, double len, double friedman, kasiski_thread_result_t *m_numbers){

	char **ics = (char **)malloc(TEXT_TO_COLUMN_MAX * sizeof(char *));
	unsigned long k, l = 0, m = 0;
	/*double *row_lens = NULL;*/
	double diff_sum = 0.0, ic_min = 100, diff_avg, kasiski = 0.0,kas_diff;
	unsigned int result_m = 1, kasiski_key = *((int*)m_numbers->values[0].key);

	/*friedman = floor(friedman);*/


	for (l = 0; l < m_numbers->len; ++l) {

		m = *((int *)m_numbers->values[l].key);

		text_to_columns(m,string,len,ics);

		diff_sum = 0;
		for (k = 0; k < m; ++k) {
			/*printf("%d:%d == %f\n",k,strlen(ics[k]),row_lens[k]);*/
			diff_sum += fabs(index_of_coincidence(ics[k],strlen(ics[k]),INDEX_OF_COINCIDENCE));
#if DEBUG == 3
			printf("ic: %f\n",fabs(index_of_coincidence(ics[k],strlen(ics[k]),0)));
			/*printf("sum: %f\n",diff_sum);*/
#endif
			free(ics[k]);
		}


		diff_avg = diff_sum/m;

#if DEBUG == 3
		printf("\n%lu - %f < %f\n\n",m,diff_avg,ic_min);
#endif
		if(diff_avg < ic_min){
			ic_min = diff_avg;
			result_m = m;
		}

		if(kasiski_key == m){
			kasiski = diff_avg;
		}
	}

	kas_diff = fabs(kasiski-ic_min);

#if DEBUG == 3
	printf("diff: %f\n",kas_diff);
#endif

	if(kas_diff <= 0.000020){
		result_m = kasiski_key;
	}

	/*printf("result: %lu\n",result_m);*/

	if(ics != NULL) free(ics);

	return result_m;
}


/** vracia index konincidencie, pricom plati cim blizsie 0, tym lepsie */
double index_of_coincidence(char *x, double len, double offset){

	double lf = 0.0;
	double freqs[LETTERS];
	unsigned long i = 0;
	memset(freqs,0,sizeof(freqs));

	letter_freqs(x,freqs);

	for (i = 0; i < LETTERS; ++i) {
		lf += (freqs[i]*(freqs[i] - 1));
	}

	return (lf/(len*(len - 1))) - offset;
}
