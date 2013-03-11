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
    const char *string = (char*)value;
    /*int len = strlen(string);*/

    while((c = string[i++]) != 0){
    	hash_value = c ^ 11 * hash_value;
    }

    /*
    for(i = 0; i < len; i++){
        hash_value = string[i] ^ 11 * hash_value;
    }
    */

    return (hash_value % hash_table_size);
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

int gcds_calc(int *ngram, int len){
	int i, tmp;

	if(len < 2) return -1;

	tmp = gcd_fast(ngram[len-1],ngram[len-2]);

	for (i = len-3; i >= 0; --i) {
		if(ngram[i] < 0){
			return -1;
		}
		tmp = gcd_fast(tmp,ngram[i]);
	}


	return tmp;
}
