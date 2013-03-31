#ifndef FRIEDMAN_H_
#define FRIEDMAN_H_

#include "definitions.h"
#include "other_fcs.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *friedman_test(void *ptr);
int ic_passwd_len(char *string, double len, double friedman, kasiski_thread_result_t *m_numbers);
double index_of_coincidence(char *x, double len, double offset);

#endif /* FRIEDMAN_H_ */
