/*
 * other_fcs.h
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#ifndef OTHER_FCS_H_
#define OTHER_FCS_H_

#include "definitions.h"

#include <string.h>

unsigned
int gcd_slow(unsigned int a, unsigned int b);

int
gcd_fast(int u,int v);

void
substring(char *dest,const char *string, int start, int len);

int
gcds_calc(int *ngram, int count);

unsigned int
hash_int(void *value);

int
compare_int(void *a, void *b);

unsigned int
hash_char(void *value);

int
compare_char(void *a, void *b);

#endif /* OTHER_FCS_H_ */
