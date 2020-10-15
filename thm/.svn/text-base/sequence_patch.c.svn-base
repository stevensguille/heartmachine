#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "sequence_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "utility.h"
#include "thm_node.h"
#include "light_effects.h"

#define  SEQUENCE_COLOR_IDX        0

static uint8_t 	sequence_loopIncr = 1;
static uint8_t  sequence_counter;

thm_patch_t sequence_patch = {
    .init     = &sequence_init,
    .enter    = &sequence_enter,
    .update   = &sequence_update,
    .exit     = &sequence_exit,
    .state    = THM_PATCH_STATE_UNKNOWN /*,
    .progress = 0*/
};

static uint8_t lightSequence[8][4] = {
		{0,4,8,12}, {1,5,9,13}, {2,6,10,14}, {3,7,11,15}, {0,1,2,3}, {4,5,6,7}, {8,9,10,11}, {12,13,14,15}
		//	{5,6,9,10}, {0,1,2,3,4,7,8,11,12,13,14,15}, {}
};

static uint8_t lightSequenceLength = 8;
static uint8_t lightSequenceCount = 4;
static uint8_t sequence_timer = 0;

/*
 * PORTED from the Java class
 */
/**
 * Common initialization function. Set the pointers to the correct place.
 */
void sequence_init() {

    sequence_patch.data[ SEQUENCE_COLOR_IDX ] = 300;

	sequence_counter = 0;
	sequence_loopIncr = 1;

	sequence_patch.data[0] = 10;	// speed
	sequence_patch.data[1] = 0;		// color
	sequence_patch.data[2] = 255;	// brightness
	sequence_patch.data[3] = 0;		// loopmode = forwards

/*
	printf_P( PSTR("some tests\n\r"));
	rgb_t  rgb;
	rgb_t *myrgb = &rgb;

	// get color from param into colorTable
	getColorFromTable(5, &myrgb);
	printf_P( PSTR("getColorFromTable %d %d %d\n\r"), myrgb->red, myrgb->green, myrgb->blue);

	getColorFromTable(0, &myrgb);
	printf_P( PSTR("getColorFromTable %d %d %d\n\r"), myrgb->red, myrgb->green, myrgb->blue);

	//
	// set the brightness
	setColorBrightness(&myrgb, 96);
	printf_P( PSTR("setColorBrightness %d %d %d\n\r"), myrgb->red, myrgb->green, myrgb->blue);
*/
}

/**
 *  Function to be called each time this patch begins.
 */
void sequence_enter() {
    printf_P( PSTR("Entering Sequence Patch\n\r") );

    mstimer_setLength( &patch_timer, 30 );
    mstimer_reset( &patch_timer );

    tlc_setAll( 0 );
}

/**
 *  Function to call
 */
void sequence_update( int16_t* deltas ) {

	uint8_t row = 0;
	uint8_t col = 0;
	uint8_t i = 0;
	uint8_t found = 0;

	// slow it down
	if (sequence_timer != sequence_patch.data[0]) {
		sequence_timer++;
		return;
	}

	sequence_timer = 0;

//    if ( mstimer_completed( &patch_timer ) ) {
//        mstimer_reset( &patch_timer );

 		for ( row = 0; row < THM_LIGHTING_NUM_ROW; row++) {
			for ( col = 0; col < THM_LIGHTING_NUM_COL; col++) {
				//
				// is this grid element in the sequence? if so, light it up
				int gridIndex = (row * THM_LIGHTING_NUM_ROW) + col;
				found = 0;
				// 4 = lightSequence[sequence_counter].length
				for ( i = 0; i < lightSequenceCount; i++) {
					if (lightSequence[sequence_counter][i] == gridIndex) {
						found = 1;
					}
				}

				rgb_t  rgb;
				rgb_t *myrgb = &rgb;
				//
				// set the color of this led if found
				if (found == 1) {
					//
					// get color from param into colorTable
					getColorFromTable((int) sequence_patch.data[1], &myrgb);
					//
					// set the brightness
					setColorBrightness(&myrgb, (int) sequence_patch.data[2]);
					//
					setLightRC(row, col, &rgb);
				} else {
					rgb_t black = {  0, 0,  0 };
					setLightRC(row, col, &black);
				}
				//System.out.println(row + " " + col + " " + found + " " + counter);
			}
		}
		//
		// if we are loop forward go up, else go down
		if (sequence_patch.data[3] == 0) {
			sequence_loopIncr = 1; // looping forward
		} else if (sequence_patch.data[3] == 1) {
			sequence_loopIncr = -1; // looping backward
		} else { // loop bidirectionaly...
			if (sequence_counter >= lightSequenceLength-1 && sequence_loopIncr == 1) {
				sequence_loopIncr = -1;
			} else if (sequence_counter <= 0 && sequence_loopIncr == -1) {
				sequence_loopIncr = 1;
			}
		}
		//printf_P( PSTR("sequence_patch.data[2] = %d, counter = %d, loopincr = %d\n"), sequence_patch.data[2], sequence_counter, sequence_loopIncr);
		//
		sequence_counter += sequence_loopIncr;
		if (sequence_counter >= lightSequenceLength) sequence_counter = 0;
		if (sequence_counter < 0) sequence_counter = lightSequenceLength-1;

//    }
    THM_TLC_CS_SELECT();
    tlc_update();
    THM_TLC_CS_UNSELECT();
}

//extern void sequence_message( void
void sequence_exit( void ) {
    printf_P( PSTR("EXIT: Good bye\n\r") );
}

