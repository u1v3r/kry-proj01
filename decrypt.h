/*
 * decrypt.h
 *
 *  Created on: 7.3.2013
 *      Author: rdy
 */

#ifndef DECRYPT_H_
#define DECRYPT_H_

#include "definitions.h"
#include "other_fcs.h"

int *crack_paswd(char *text, int passwd_len, double text_len);

#endif /* DECRYPT_H_ */
