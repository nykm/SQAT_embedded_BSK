/*
 * random.h
 *
 *  Created on: Oct 10, 2016
 *      Author: timppa
 */

#ifndef RANDOM_H_
#define RANDOM_H_

void random_init(unsigned int s1,
		unsigned int s2,
		unsigned int s3,
		unsigned int s4);

unsigned int random_get(void);

#endif /* RANDOM_H_ */
