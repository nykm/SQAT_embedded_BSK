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

int i2c_read(int address,
		char* reg,int reglen,
		char* buffer,int length)
{
	i2c_data_t i2c_data;
	i2c_data.address=address;
	i2c_data.reg=reg;
	i2c_data.reglen =reglen;
	i2c_data.buffer = buffer;
	i2c_data.length = length;
	return i2c_read_p( &i2c_data );
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
	i2c_data_t i2c_data;
	i2c_data.address = address;
	i2c_data.reg = 0;
	i2c_data.reglen = 0;
	i2c_data.buffer = (char*)buffer;
	i2c_data.length = length;
	return i2c_write_p( &i2c_data );
}
