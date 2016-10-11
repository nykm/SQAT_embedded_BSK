/*
 * bsk.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: timppa
 */

#include "hardware.h"
#include "i2c.h"
#include "delay.h"
#include "display.h"

#include "bsk.h"

/************************************************************************
 *
 *   IMPLEMENTATION BELOW
 *
 ************************************************************************/

//
// get_throw reads the BSK pin counting device and returns
// the number of knocked pins via the "pFrame" structure
//
// return value for the subroutine is:
//   ERR_PARAM_NULL if pFrame is zero/NULL
//   ERR_BAD_THROW_INDEX if index is not 1 or 2
//   1 for reading the first throw
//   2 for reading the second throw
//   ERR_READ_FAILED if i2c_read returns other than 1
//
int bsk_get_throw(bsk_frame_t* pFrame,int index)
{
	if ( 0==pFrame ){
		return ERR_PARAM_NULL;
	}
	//
	// reminder about pointers:
	//
	// pFrame->first_throw  gives the value of "first_throw"
	//
	// pFrame->first_throw = 2; set the value of "first_throw"
	//
	/**
	 * TODO: Oscar, REMOVE THESE
	 */
	if ( index==1 || index==2 ){
		char data;
		int rc = i2c_read( HW_BSK_PIN_COUNTER, 0,0, &data, 1);
		if ( rc==1 ){
			if ( index==1 )
				pFrame->first_throw = (int)data;
			else if (index==2 )
				pFrame->second_throw = (int)data;
			return index;
		}
		return ERR_READ_FAILED;
	}
	/**
	 * TODO: Oscar, UNTIL HERE
	 */
	return ERR_BAD_THROW;
}

//
// calculate the sum of points of "frames" first frames
//
int bsk_calculate(bsk_game_t* pGame,int frames)
{
	if ( 0==pGame ){
		return ERR_PARAM_NULL;
	}
	int sum=0;
	/**
	 * TODO: Oscar, REMOVE THESE
	 */
	if ( frames<0 || frames>BSK_FRAMES_IN_GAME ){
		return ERR_BAD_FRAME;
	}
	int i = 0;
	for(i=0; i<frames; i++){
		sum+=pGame->frames[i].first_throw;
		sum+=pGame->frames[i].second_throw;
	}
	/**
	 * TODO: Oscar, UNTIL HERE
	 */
	return sum;
}

//
// return "0" (zero) of whole frame is valid
//
// return -1 if pFrame (pointer) is invalid (NULL)
//
// return 1 if frame content is bad (first or second throw or sum)
//
int bsk_valid_frame(bsk_frame_t* pFrame)
{
	if ( 0==pFrame ){
		return -1;
	}
	/**
	 * TODO: Oscar, REMOVE THESE
	 */
	if ( pFrame->first_throw<0 || pFrame->first_throw>10 ){
		return 1;
	}
	if ( pFrame->second_throw<0 || pFrame->second_throw>10 ){
		return 1;
	}
	int sum = pFrame->first_throw + pFrame->second_throw;
	if ( sum > 10 ){
		return 1;
	}
	return 0;
	/**
	 * TODO: Oscar, UNTIL HERE
	 */
}

//
// play the full game
// - display points initially
// - get score for throw and spend its delay
// - get score for 2nd throw and spend its delay
// - and so on
//
int play_game()
{
	int sum=0;
	bsk_game_t bsk_game;
	int f=0;

	//
	// show initial score (zero)
	//
	disp_show_decimal( sum );
	/**
	 * TODO: Oscar, REMOVE THESE
	 */
	while( 1 ){
		int rc1 = bsk_get_throw( &bsk_game.frames[f], 1 );
		delay_1s();
		int rc2 = bsk_get_throw( &bsk_game.frames[f], 2 );
		delay_1s();
		if ( rc1==1 && rc2==2 ){
			if ( 0==bsk_valid_frame(&bsk_game.frames[f]) ){
				f++;
				sum = bsk_calculate( &bsk_game, f );
				disp_show_decimal( sum );
			}
		}
		if ( f==BSK_FRAMES_IN_GAME )break;
	}
	/**
	 * TODO: Oscar, UNTIL HERE
	 */
	return sum;
}
