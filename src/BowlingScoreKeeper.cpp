/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: add include for "swm.h"
#include "swm.h"
// TODO: add include for "led.h"
#include "led.h"
// TODO: add include for "delay.h"
#include "delay.h"
// TODO: add include for "i2c.h"
#include "i2c.h"
// TODO: add include for "display.h"
#include "display.h"
// TODO: add include for "random.h"
#include "random.h"
// TODO: add include for "romapi.h"
#include "romapi.h"

#define SWM_MODE_I2C 1
#define SWM_MODE_LED 2

#define SWM_MODE SWM_MODE_I2C


#if (SWM_MODE==SWM_MODE_I2C)
#define I2C_DEMO_MODE_MSG     1
#define I2C_DEMO_MODE_DECIMAL 2
#define I2C_DEMO_MODE_HEX     3

#define I2C_DEMO_MODE 2
#endif

int main(void)
{
#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#endif

	// TODO: add call to swm_config_LED();
#if (SWM_MODE==SWM_MODE_I2C)
    swm_config_i2c();
    i2c_reset();
    unsigned int romid[4];
    if ( 0==romapi_read_id( romid,4 ) ){
    	random_init(romid[0],romid[1],romid[2],romid[3]);
    } else {
    	random_init(12345,12345,12345,12345);
    }
#elif (SWM_MODE==SWM_MODE_LED)
	swm_config_LED();
	// TODO: add call to led_init();
	led_init();
#endif

#if (SWM_MODE==SWM_MODE_I2C)
	disp_on( DISP_SHOW_NONE );
	disp_reset( DISP_SHOW_NONE );
	volatile static int i = 0;
	while( 1 ){
#if (I2C_DEMO_MODE==I2C_DEMO_MODE_HEX)
		disp_show_hex( random_get() );
		i++;
		delay(100000);
#endif
#if (I2C_DEMO_MODE==I2C_DEMO_MODE_DECIMAL)
		disp_show_decimal( random_get() % 11 );
		i++;
		delay(1000000);
#endif
#if (I2C_DEMO_MODE==I2C_DEMO_MODE_MSG)
		if ( i < 5 ){
			disp_show_message( DISP_MSG_DOWN );
			delay_1s();
			i++;
		} else if ( i<10 ){
			disp_show_message( DISP_MSG_SAME );
			delay_1s();
			i++;
		} else if ( i<15 ){
			disp_show_message( DISP_MSG_UP );
			delay_1s();
			i++;
		} else if ( i==15 ){
			i=0;
		}
#endif
	}
#elif (SWM_MODE==SWM_MODE_LED)
    volatile static int i = 0 ;
    while(1) {
    	// TODO: add code to LED_on( blue )
    	led_on( LED_GREEN );
    	// TODO: add code to delay 1 second
    	delay_1s();
    	// TODO: add code to LED_off( blue )
    	led_off( LED_GREEN );
    	// TODO: add code to delay 1 second
    	delay_1s();

    	// TODO: add code to LED_on( red )
    	led_on( LED_RED );
    	// TODO: add code to delay 1 second
    	delay_1s();
    	// TODO: add code to LED_off( red )
    	led_off( LED_RED );
    	// TODO: add code to delay 1 second
    	delay_1s();
        i++ ;
    }
#endif
    return 0 ;
}
