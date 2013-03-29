/*
 * kry.h
 *
 *  Created on: 1.3.2013
 *      Author: rdy
 */

#ifndef KRY_H_
#define KRY_H_

#include "kasiski.h"
#include "friedman.h"
#include "decrypt.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef TIMES
#include "sys/time.h"
#endif

char *strip_stdin(char *output_text);

#endif /* KRY_H_ */
