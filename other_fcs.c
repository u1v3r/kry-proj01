/*
 * other_fcs.c
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#include "other_fcs.h"
#include "stdio.h"

unsigned int gcd_slow(unsigned int a, unsigned int b) {

	if(a==0) return b;

	return gcd_slow(b % a, a);
}

/*
 * http://comeoncodeon.wordpress.com/2009/02/26/binary-gcd-algorithm/
 */
int gcd_fast(int u,int v) {

	int k = 0, t = 0, i;

	while (!(u & 1) && !(v & 1)) {
		k++;
		u >>= 1;
		v >>= 1;
	}

	if (u&1){
		t = u;
	}
	else{
		t = -v;
	}

	do {
		while (!(t & 1))
			t >>= 1;
		if (t > 0)
			u = t;
		else
			v = -t;

		t = u - v;

	} while (t);

	for (i=0;i<k;i++){
		u<<=1;
	}

	return u;
}

void substring(char *dest,const char *string, int start, int len) {


	/*memset(dest,0,NGRAM_STOP);*/
	memcpy(dest,string + start,len);
	dest[len] = '\0';

}

unsigned int hash_int(void *value, unsigned int hash_table_size){
	return *((int*)value) % hash_table_size;
}

unsigned int hash_char(void *value, unsigned int hash_table_size){


	unsigned hash_value = 0;
    int i = 0;
    char c;

    while((c = ((char*)value)[i++]) != 0){
    	hash_value = c ^ 11 * hash_value;
    }

    return (hash_value % hash_table_size);


/*	return ((*(size_t*)value)>> 2) % hash_table_size;*/
}

int compare_int(void *a, void *b){

	if(*((int *)a) == *((int *)b)){
		return 1;
	}

	return 0;
}

int compare_char(void *a, void *b){

	if(strcmp((char *)a,(char *)b) == 0) return 1;

	return 0;
}

int gcds_calc(int *ngram, int dist_count){
	int i, tmp;

	/* ngram ma len dva vyskyty */
	if(dist_count == 1 ) {
		return ngram[0];
	}else if(dist_count < 1){
		/* ngramy sa neopakuju */
		return 1;
	}

	tmp = gcd_fast(ngram[dist_count-1],ngram[dist_count-2]);

	for (i = dist_count-3; i >= 0; --i) {
		if(ngram[i] < 0){
			return -1;
		}
		tmp = gcd_fast(tmp,ngram[i]);
	}


	return tmp;
}

char **text_to_columns(int column_length, char *string, double text_len, char **ics){

	long column_max = TEXT_TO_COLUMN_MAX;
	/*double *row_lens = (double *)malloc(column_max * sizeof(double));*/
	char c;
	double row_len;
	unsigned long i = 0 , j = 0;

	/* pri velkych m treba vytovirt miesto :D */
	if(column_length >= column_max - 1 ){
		column_max = column_length + 1;
		ics = (char **)realloc(ics,column_max * sizeof(char *));
		/*row_lens = (double *)realloc(row_lens,column_max * sizeof(double));*/
	}

	row_len = text_len/column_length;
	j = 0;
	i = 0;

	while((c = string[i]) != 0){

		if(i % column_length == 0 && i != 0){
			j++;
		}

		if(row_len == j){
			break;
		}

		if(j == 0){
			ics[i % column_length] = (char *)calloc(row_len+2,sizeof(char));
		}

		ics[i % column_length][j] = c;
		i++;
	}

	return ics;

}

void letter_freqs(char *input, double freqs[]){

	char c;
	unsigned long i = 0;

	while((c = input[i++]) != 0){
		freqs[c - 'a']++;
	}
}
