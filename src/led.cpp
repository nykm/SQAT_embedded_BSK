/*
 * led.c
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

//#include <LPC8xx.h>
#include "chip.h"
#include "led.h"

#define LED_BLUE_LOCATION 17

//
// initialize the GPIO for LEDs
// - enable clocking for GPIO block
// - reset GPIO peripheral block
// - configure port direction for all 3 LEDs
// - set all 3 LEDs off
//
void led_init()
{
	// LPC_SYSCON is at address: 0x40000000 + 0x48000 = 0x40048000
	LPC_SYSCTL_T* pLPC_SYSCON = (LPC_SYSCTL_T*)LPC_SYSCON;
	// enable clocking for GPIO
	pLPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	// peripheral reset control: assert the GPIO reset
	pLPC_SYSCON->PRESETCTRL &= ~(1<<10);
	// peripheral reset control: clear the GPIO reset
	pLPC_SYSCON->PRESETCTRL |= (1<<10);

	// LPC_GPIO_PORT is at address: 0xA0000000
	LPC_GPIO_T* pLPC_GPIO_PORT = (LPC_GPIO_T*)LPC_GPIO_PORT;
	// configure port directions
	pLPC_GPIO_PORT->DIR[0] |= (1<<LED_RED);
	pLPC_GPIO_PORT->DIR[0] |= (1<<LED_BLUE);
	pLPC_GPIO_PORT->DIR[0] |= (1<<LED_GREEN);
	// configure all LEDs OFF
	pLPC_GPIO_PORT->SET[0] |= (1<<LED_RED);
	pLPC_GPIO_PORT->SET[0] |= (1<<LED_BLUE);
	pLPC_GPIO_PORT->SET[0] |= (1<<LED_GREEN);
}

//
// set the selected LED on
//
void led_on( int idLED )
{
	LPC_GPIO_T* pLPC_GPIO_PORT = (LPC_GPIO_T*)LPC_GPIO_PORT;
	switch( idLED ){
	case LED_RED:
	case LED_BLUE:
	case LED_GREEN:
		// set the indexed LED on
		pLPC_GPIO_PORT->CLR[0] = (1 << idLED);
		break;
	}
}

//
// set the selected LED on
//
void led_off( int idLED )
{
	LPC_GPIO_T* pLPC_GPIO_PORT = (LPC_GPIO_T*)LPC_GPIO_PORT;
	switch( idLED ){
	case LED_RED:
	case LED_BLUE:
	case LED_GREEN:
		// set the indexed LED off
		pLPC_GPIO_PORT->SET[0] = (1 << idLED);
		break;
	}
}
