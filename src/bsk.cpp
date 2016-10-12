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
	if (index < 1 || index > 2) {
		return ERR_BAD_THROW_INDEX;
	}

	if ( 0==pFrame ){
		return ERR_PARAM_NULL;
	}

	char data = 0;

	if (i2c_read(0x90, (char*) 0, 0, &data, 1) != 1) {
		return ERR_READ_FAILED;
	}

	if (index == 1) {
		pFrame->first_throw = (int) data;
	} else {
		pFrame->second_throw = (int) data;
	}

	return 0;
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

	for (int i = 0; i < frames; i++) {
		sum += pGame->frames[i].first_throw;
		sum += pGame->frames[i].second_throw;
	}

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

	if (pFrame->first_throw > 10 || pFrame->second_throw > 10) {
		return 1;
	}

	if (pFrame->first_throw + pFrame->second_throw > 10) {
		return 1;
	}

	return 0;
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
	// use these variables if you wish; they are not compulsory
	int sum=0;
	bsk_game_t bsk_game;

	// set initial scores
	for (unsigned int i = 0; i < 10; i++) {
		bsk_game.frames[i].first_throw = 0;
		bsk_game.frames[i].second_throw = 0;
	}

	// show initial score (zero)
	disp_show_decimal(sum);

	// go through 10 frames and update throws
	for (unsigned int f = 0; f < 10; f++) {
		bsk_frame_t* frame = &bsk_game.frames[f];
		int throw_index = 1;
		// go through 2 throws: 1 & 2
		while (throw_index <= 2) {
			// get next valid frame
			do {
				// get throw value until valid value is gotten
				while (bsk_get_throw(frame, throw_index) != 0);
			} while (bsk_valid_frame(frame) != 0);

			// no need for second throw so continue to next frame
			if (frame->first_throw == 10) {
				break;
			}

			++throw_index;
		}

		sum = bsk_calculate(&bsk_game, f + 1);
		disp_show_decimal(sum);
		delay_1s();
	}

	return 0;
}
