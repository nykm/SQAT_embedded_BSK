/*
 * display.cpp
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */

#include "i2c.h"

#include "hardware.h"

#include "ht16k33.h"

#include "display.h"

int disp_off()
{
	const int addr = HW_I2C_ADDR_HT16K33;
	char data[10];
	int rc=0;

	data[0] = HT16K33_CMD_OSCILLATOR_OFF;
	data[1]=0;

	rc = i2c_write( addr,data,1 );
	return rc;
}

static char disp_msg_data[10]={
		0,0,
		1,0,
		2,0,
		3,0,
		4,0,
};

static int disp_last_message = 0;

//
// set all display data to either "all on" or "all off"
//
static void disp_set_all(int alloff)
{
	char value = 0;
	switch( alloff ){
	case DISP_SHOW_NONE:
		value = SEGMENTS_NONE;
		break;
	case DISP_SHOW_ALL:
		value = SEGMENTS_ALL;
		break;
	default:
		return;
	}
	disp_msg_data[1] = value;
	disp_msg_data[3] = value;
	disp_msg_data[5] = value;
	disp_msg_data[7] = value;
	disp_msg_data[9] = value;
}

//
// reset display data to all off/on
// clear display message value
//
void disp_reset(int alloff)
{
	disp_set_all( alloff );
	disp_last_message = 0;
}

//
// power on display, set the clocking and initial data
// - data is all off/on
//
int disp_on(int alloff)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	char disp_cmd_data[10];
	int rc=0;

	disp_cmd_data[0] = HT16K33_CMD_OSCILLATOR_ON;
	disp_cmd_data[1]=0;

	rc = i2c_write( addr,disp_cmd_data,1 );
	if ( 0 <= rc ){
		disp_cmd_data[0] = HT16K33_BLINK_CMD | 0x01;
		rc = i2c_write( addr,disp_cmd_data,1 );
	} else {
		return rc;
	}
	if ( 0 <= rc ){
		disp_cmd_data[0] = HT16K33_CMD_BRIGHTNESS | 0x08;
		rc = i2c_write( addr,disp_cmd_data,1 );
	} else {
		return rc;
	}
	disp_set_all( alloff );
	rc = i2c_write( addr,disp_msg_data,10 );

	return rc;
}

static void rotate_message_left()
{
	char left = disp_msg_data[1];
	disp_msg_data[1] = disp_msg_data[3];
	disp_msg_data[3] = disp_msg_data[7];
	disp_msg_data[7] = disp_msg_data[9];
	disp_msg_data[9] = left;
}

//
// display message of:
//   - DISP_MSG_DOWN
//   - DISP_MSG_SAME
//   - DISP_MSG_UP
// if message is same as previous then rotate left
//
int disp_show_message(display_message_t message)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	if ( message<=DISP_MSG_FIRST || message>=DISP_MSG_LAST ){
		return DISP_ERR_ILLEGAL;
	}
	if ( message != disp_last_message ){
		if ( message==DISP_MSG_UP ){
			disp_msg_data[1]=SEGMENTS_NONE;
			disp_msg_data[3]=SEGMENTS_NONE;
			disp_msg_data[7]=SEGMENTS_u;
			disp_msg_data[9]=SEGMENTS_P;
		} else if ( message==DISP_MSG_DOWN ){
			disp_msg_data[1]=SEGMENTS_d;
			disp_msg_data[3]=SEGMENTS_o;
			disp_msg_data[7]=SEGMENTS_w;
			disp_msg_data[9]=SEGMENTS_n;
		} else if ( message==DISP_MSG_SAME ){
			disp_msg_data[1]=SEGMENTS_S;
			disp_msg_data[3]=SEGMENTS_A;
			disp_msg_data[7]=SEGMENTS_m;
			disp_msg_data[9]=SEGMENTS_E;
		}
	} else {
		rotate_message_left( );
	}
	disp_last_message = message;
	return i2c_write( addr, disp_msg_data,10 );
}

//
// mapping of number to its segment data:
//   element index  |  segment data
//   0              |  63 (segments for zero)
//   1              |   6 (segments for one)
const char digit_segments[10]={
		SEGMENTS_0,
		SEGMENTS_1,
		SEGMENTS_2,
		SEGMENTS_3,
		SEGMENTS_4,
		SEGMENTS_5,
		SEGMENTS_6,
		SEGMENTS_7,
		SEGMENTS_8,
		SEGMENTS_9,
};

#if 1
//
// return the Nth rightmost digit from value
//   value | n | result
//   ------+---+-------
//   417   | 0 | 7
//   417   | 1 | 1
//   417   | 2 | 4
//   417   | 3 | 0
//
int disp_digit_of(int value,unsigned int n)
{
	while( n && value ){
		n--;
		value /= 10;
	}
	return value % 10;
}

int disp_show_decimal(int value)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	if ( value > 9999 ){
		return DISP_ERR_OVERFLOW;
	} else if ( value < 0 ){
		return DISP_ERR_UNDERFLOW;
	}
	disp_msg_data[1] = digit_segments[ disp_digit_of( value, 3 ) ];
	disp_msg_data[3] = digit_segments[ disp_digit_of( value, 2 ) ];
	disp_msg_data[5] = SEGMENTS_NONE;
	disp_msg_data[7] = digit_segments[ disp_digit_of( value, 1 ) ];
	disp_msg_data[9] = digit_segments[ disp_digit_of( value, 0 ) ];
	if ( disp_msg_data[1]==0x3F ){
		disp_msg_data[1] = 0x00;
		if ( disp_msg_data[3]==0x3F ){
			disp_msg_data[3]=0x00;
			if ( disp_msg_data[7]==0x3F ){
				disp_msg_data[7] = 0x00;
			}
		}
	}
	return i2c_write( addr,disp_msg_data,10 );
}

#else
int disp_show_decimal(int value)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	int rc=0;
	int i;
	int sign = 0;

	if ( value < 0 ){
		sign = -1;
		value = -value;
	}

	for( i=0; i<4; i++){
		int num = value % 10;
		int idx = 3-i;
		if ( idx>1 ){
			idx++;
		}
		disp_msg_data[idx*2+1] = digit_segments[num];
		value = value / 10;
	}
	// leading zeroes are shut off, except last
	if ( disp_msg_data[1]==SEGMENTS_0 ){
		disp_msg_data[1] = SEGMENTS_NONE;
		if ( disp_msg_data[3]==SEGMENTS_0 ){
			disp_msg_data[3]=SEGMENTS_NONE;
			if ( disp_msg_data[7]==SEGMENTS_0 ){
				disp_msg_data[7] = SEGMENTS_NONE;
			}
		}
	}
	if ( sign ){
		disp_msg_data[1]=SEGMENTS_MINUS;
	}
	rc = i2c_write( addr,disp_msg_data,10 );
	return rc;
}
#endif

const char digit_hex_segments[16]={
		SEGMENTS_0,
		SEGMENTS_1,
		SEGMENTS_2,
		SEGMENTS_3,
		SEGMENTS_4,
		SEGMENTS_5,
		SEGMENTS_6,
		SEGMENTS_7,
		SEGMENTS_8,
		SEGMENTS_9,
		SEGMENTS_A,
		SEGMENTS_B,
		SEGMENTS_C,
		SEGMENTS_D,
		SEGMENTS_E,
		SEGMENTS_F,
};

int disp_show_hex(int value)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	int rc=0;
	int i;

	for( i=0; i<4; i++){
		int num = value & 0x0F;
		int idx = 3-i;
		if ( idx>1 ){
			idx++;
		}
		disp_msg_data[idx*2+1] = digit_hex_segments[num];
		value = value >> 4;
	}
	if ( disp_msg_data[1]==SEGMENTS_0 ){
		disp_msg_data[1] = SEGMENTS_NONE;
		if ( disp_msg_data[3]==SEGMENTS_0 ){
			disp_msg_data[3]=SEGMENTS_NONE;
			if ( disp_msg_data[7]==SEGMENTS_0 ){
				disp_msg_data[7] = SEGMENTS_NONE;
			}
		}
	}
	rc = i2c_write( addr,disp_msg_data,10 );
	return rc;
}
