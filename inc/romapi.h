/*
 * romapi.h
 *
 *  Created on: Oct 9, 2016
 *      Author: timppa
 */

#ifndef ROMAPI_H_
#define ROMAPI_H_

//
// entry point to IAP code in ROM
//
// as the address is odd, the MCU will change to Thumb code mode
//
#define IAP_LOCATION (0x1FFF1FF1)

//
// ROM API functions
//
#define ROM_IAP_GET_ID 58

//
// This provides the 4 integers of the unique chip id
//
int romapi_read_id(unsigned int* romid,int length);

#endif /* ROMAPI_H_ */
