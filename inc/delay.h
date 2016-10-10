/*
 * delay.h
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

#ifndef DELAY_H_
#define DELAY_H_

//
// make sure that C++ compiler uses C-linkage for
// subroutines in this code module
//
#ifdef __cplusplus
extern "C" {
#endif

void delay(volatile int d);
//
// this causes delay of approx 1s (when clocking is the default)
//
void delay_1s();

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H_ */
