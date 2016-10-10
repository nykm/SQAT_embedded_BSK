/*
 * romapi.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: timppa
 */

#include "romapi.h"

static unsigned int command_param[5];
static unsigned int command_result[5];

//
// type definition for subroutine entry
//
typedef int (iap_call_t)(unsigned int[],unsigned int[]);

int romapi_read_id(unsigned int* romid,int length)
{
	iap_call_t* entry = (iap_call_t*)IAP_LOCATION;
	int rc=0;

	if ( 0==romid || length!=4 ){
		return -1;
	}
	for (rc=0; rc<5; rc++){
		command_param[rc]=0;
		command_result[rc]=0;
	}
	command_param[0] = ROM_IAP_GET_ID;

	rc = (entry)(command_param,command_result);
	if ( rc!=0 ){
		return -1;
	}
	if ( command_result[0] == 0 ){
		int i;
		for(i=0; i<4; i++){
			romid[i]=command_result[1+i];
		}
		return 0;
	} else {
		return command_result[0];
	}
#if 0
	for( rc=0; rc<5; rc++){
		int r = command_result[rc];
		if ( r ){
			r++;
		}
		r = command_param[rc];
		if ( r ){
			r++;
		}
	}
	return 0;
#endif
}
