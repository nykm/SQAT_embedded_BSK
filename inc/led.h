/*
 * led.h
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

#ifndef LED_H_
#define LED_H_

#define LED_RED    7
#define LED_BLUE  16
#define LED_GREEN 17

//
// make sure that C++ compiler uses C-linkage for
// subroutines in this code module
//
#ifdef __cplusplus
extern "C" {
#endif

//
// initialize the GPIO for LEDs
//
void led_init();
//
// set the selected LED on
//
void led_on( int idLED );
//
// set the selected LED off
//
void led_off( int idLED );

#ifdef __cplusplus
}
#endif

#endif /* LED_H_ */
