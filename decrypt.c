/*
 * decrypt.c
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#include "decrypt.h"

static double english_freq[] = {
		0.08167,/* a */
		0.01492,/* b */
		0.02782,/* c */
		0.04253,/* d */
		0.12702,/* e */
		0.02228,/* f */
		0.02015,/* g */
		0.06094,/* h */
		0.06966,/* i */
		0.00153,/* j */
		0.00772,/* k */
		0.04025,/* l */
		0.02406,/* m */
		0.06749,/* n */
		0.07507,/* o */
		0.01929,/* p */
		0.00095,/* q */
		0.05987,/* r */
		0.06327,/* s */
		0.09056,/* t */
		0.02758,/* u */
		0.00978,/* v */
		0.02360,/* w */
		0.00150,/* x */
		0.01974,/* y */
		0.00074,/* z */
};

int *crack_paswd(char *text, int passwd_len, double text_len){

	char **ics = (char **)malloc(TEXT_TO_COLUMN_MAX * sizeof(char *));
	int i, j, g;
	double sum = 0;
	double l_freqs[LETTERS];
	double column_len;
	int *result = (int *)malloc(passwd_len * sizeof(int));
	double max;
	int best_shift = 0;

	ics = text_to_columns(passwd_len,text,text_len,ics);

	for (j = 0; j < passwd_len; ++j) {
		memset(l_freqs,0,sizeof(l_freqs));
		letter_freqs(ics[j],l_freqs);
		column_len = strlen(ics[j]);
		max = 0;
		for(g = 0; g < LETTERS; g++){
			sum = 0;
			for (i = 0; i < LETTERS; ++i) {
				sum += (english_freq[i] * l_freqs[(i + g) % LETTERS]) / column_len;
			}

			if(sum > max){
				max = sum;
				best_shift = g;
			}

		}
		result[j] = best_shift;
		free(ics[j]);
	}


	free(ics);

	return result;
}
