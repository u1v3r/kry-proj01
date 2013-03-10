/*
 * friedman.h
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#ifndef FRIEDMAN_H_
#define FRIEDMAN_H_

#include "definitions.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *friedman_test(void *ptr);
void test_len(char *string, double len, double friedman);
double index_of_coincidence(char *x, long row_len);
void letter_freqs(char *input, double freqs[]);
#endif /* FRIEDMAN_H_ */
