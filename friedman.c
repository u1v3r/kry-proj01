/*
 * friedman.c
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#include "friedman.h"

static unsigned int letters[26] = {0};

void *friedman_test(void *ptr){

	double *friedman = (double *)malloc(sizeof(double));
	double k0 = 0.0, kp = 0.065, kr = 0.0385;

	char *c_text_s = ptr;
	int c,lf=0,i = 0;
	int len = strlen(c_text_s);
	*friedman = 0.0;

	if(len < 1) return friedman;

	while((c = c_text_s[i++]) != 0){
		letters[c - 'a']++;
	}
	int sum = 0;
	for (i = 0; i < 26; ++i) {
		printf("%c - %d\n",(i + 'a'),letters[i]);
		sum += letters[i];
	}

	printf("sum:%d\n",sum);

	for (i = 0; i < 26; ++i) {
		lf += (letters[i]*(letters[i] - 1));
	}

	printf("len: %d\n",len);
	printf("ko(%f)\n",lf);
	k0 = (double)lf/(double)(len*(len - 1));
	printf("(kp(%f) - kr(%f))/(k0(%f) - kr(%f))\n",kp,kr,k0,kr);
	*friedman = (kp - kr)/(k0 - kr);


	return friedman;
}
