/*
 * swm.c
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

//#include <LPC8xx.h>
#include "chip.h"

#include "swm.h"

//
// creates SWM configuration for LEDs
// - the GPIOs are enabled by default
// - so we ensure that RESET and debugging work
//
// For SWM configuration reading/writing the clocking of SWM
// must be enabled. After the read/write the clocking can be
// disabled, saves power...
//
void swm_config_LED()
{
	//
	// LPC_SYSCON is at address: 0x40000000 + 0x48000 = 0x40048000
	LPC_SYSCTL_T* pLPC_SYSCON = (LPC_SYSCTL_T*)LPC_SYSCON;
	// enable system clock for SWM block: toggle bit 7 to one
	pLPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
	//
	// LPC_SWM is at address: 0x40000000 + 0x0C000 = 0x4000C000
	LPC_SWM_T* pLPC_SWM = (LPC_SWM_T*)LPC_SWM;
	// RESET only
//	pLPC_SWM->PINENABLE0 = 0xffffffbfUL;
	// RESET, SWCLK, SWDIO
	pLPC_SWM->PINENABLE0 = 0xffffffb3UL;
	// disable system clock of SWM block: toggle bit 7 to zero
	pLPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<7);
}

void swm_config_i2c()
{
	// LPC_SYSCON is at address: 0x40000000 + 0x48000 = 0x40048000
	LPC_SYSCTL_T* pLPC_SYSCON = (LPC_SYSCTL_T*)LPC_SYSCON;
	// enable system clock for SWM block: toggle bit 7 to one
	pLPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
	LPC_SWM_T* pLPC_SWM = (LPC_SWM_T*)LPC_SWM;
	// setup SWM for GPIO, I2C (pins 10,11) and debugging stuff (SWCLK, SWDIO, RESET)
	pLPC_SWM->PINASSIGN[0] = 0xffff0004UL;
	pLPC_SWM->PINASSIGN[7] = 0x0affffffUL;
	pLPC_SWM->PINASSIGN[8] = 0xffffff0bUL;
	pLPC_SWM->PINENABLE0 = 0xffffffb3UL;
	// deactivate the clock of SWM (not needed any more)
	pLPC_SYSCON->SYSAHBCLKCTRL &= (~(1<<7));
}
