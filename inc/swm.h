/*
 * swm.h
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

#ifndef SWM_H_
#define SWM_H_

//
// make sure that C++ compiler uses C-linkage for
// subroutines in this code module
//
#ifdef __cplusplus
extern "C" {
#endif

//
// creates SWM configuration for LEDs, RESET and debug
//
void swm_config_LED();
void swm_config_i2c();

#ifdef __cplusplus
}
#endif

#endif /* SWM_H_ */
