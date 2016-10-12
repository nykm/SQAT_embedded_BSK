/*
 * i2c.c
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */


#include "chip.h"

#include "i2c.h"

int i2c_reset()
{
	if ( LPC_SYSCON ){
		LPC_SYSCON -> SYSAHBCLKCTRL |=  (1 << 5);
		LPC_SYSCON -> PRESETCTRL &= ~(1 << 6);		// I2C reset
		LPC_SYSCON -> PRESETCTRL |= (1 << 6);
		// I2C clock division
		uint32_t* pclk = (uint32_t*)0x40050014;		// page 222
		*pclk = 15;	// sysclk divided by 120

		return 0;
	}
	return -1;
}

static int i2c_pending_status()
{
	return LPC_I2C->STAT & I2C_STAT_MSTPENDING;
}

int i2c_read_p( i2c_data_t* data )
{
	LPC_I2C->CFG = I2C_CFG_MSTEN;
	// wait until pending
	while( 0==i2c_pending_status() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_READ_IDLE;
	}

	if ( data->reglen && data->reg ){
		int regbyte = 0;
		for( regbyte=0; regbyte<data->reglen; regbyte++){
			LPC_I2C->MSTDAT = (data->address << 1) | 0;
			LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
			// wait until pending
			while( 0==i2c_pending_status() );
			if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
				return I2C_ERR_WRITE_TX;
			}
			LPC_I2C->MSTDAT = data->reg[regbyte];
			LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
		}
		// wait until pending
		while( 0==i2c_pending_status() );
		LPC_I2C->MSTDAT = (data->address << 1) | 1;
		LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	} else {
		LPC_I2C->MSTDAT = (data->address << 1) | 1;
		LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	}
	// wait until pending
	while( 0==i2c_pending_status() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){
		return I2C_ERR_READ_RX;
	}
	int count = 0;
	while ( count<data->length ){
		data->buffer[count] = LPC_I2C->MSTDAT;
		count++;
		if ( count<data->length ){
			LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
			// wait until pending
			while( 0==i2c_pending_status() );
			if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){
				return I2C_ERR_READ_RX;
			}
		} else {
			LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
			// wait until pending
			while( 0==i2c_pending_status() );
			if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
				return I2C_ERR_READ_STOP;
			}
		}
	}
	return count;
}

#define SIMULATED_DATA 25

static int index_simulation = 0;

static const char simulated[SIMULATED_DATA]={
		9,0,		// round #1
		9,1,
		10,			// round #3
		2,0,
		3,3,		// round #5
		12,			// incorrect 12 is too high
		2,0,		// round #6
		5,1,
		3,0,		// round #8
		3,8,		// incorrect 3+8 == 11
		4,5,
		6,3,		// round #10
		0,12,		// incorrect 12 is too high
		10			// round #1 for game #2
};

void reset_simulation()
{
	index_simulation = 0;
}

static int get_simulated_data()
{
	int out = simulated[ index_simulation++ ];
	if ( index_simulation==SIMULATED_DATA ){
		index_simulation = 0;
	}
	return out;
}

int i2c_read(int address,
		char* reg,int reglen,
		char* buffer,int length)
{
	if ( address==0x70 ){
		i2c_data_t i2c_data;
		i2c_data.address=address;
		i2c_data.reg=reg;
		i2c_data.reglen =reglen;
		i2c_data.buffer = buffer;
		i2c_data.length = length;
		return i2c_read_p( &i2c_data );
	} else if ( address==0x90 ){ // BSK input
		// should always read ONE character at a time
		if ( buffer && length==1 ){
			int i=0;
			for(i=0; i<length; i++){
				char d = get_simulated_data();
				buffer[i]=d;
			}
			return length;
		}
	}
	return I2C_ERR_READ_RX;
}

static int i2c_write_p(i2c_data_t* data)
{
	LPC_I2C->CFG = I2C_CFG_MSTEN;
	// wait until pending
	while( 0==i2c_pending_status() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_IDLE;
	}

	LPC_I2C->MSTDAT = (data->address << 1) | 0;
	LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	// wait until pending
	while( 0==i2c_pending_status() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
		return I2C_ERR_WRITE_TX;
	}
	int count=0;
	while ( count<data->length ){
		LPC_I2C->MSTDAT = data->buffer[count];
		count++;
		LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
		// wait until pending
		while( 0==i2c_pending_status() );
		if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
			return I2C_ERR_WRITE_TX;
		}
	}
	LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
	// wait until pending
	while( 0==i2c_pending_status() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_STOP;
	}
	return 1;
}

int i2c_write(int address,const char* buffer,int length)
{
	if ( address==0x70 ){
		i2c_data_t i2c_data;
		i2c_data.address = address;
		i2c_data.reg = 0;
		i2c_data.reglen = 0;
		i2c_data.buffer = (char*)buffer;
		i2c_data.length = length;
		return i2c_write_p( &i2c_data );
	} else {
		return -1;
	}
}
