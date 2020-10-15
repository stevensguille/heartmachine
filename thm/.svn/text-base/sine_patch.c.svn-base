/*
 * sine_patch.c - Sine Patch, aka Raver Bait
 *
 * Parameters:
 * 0 - speed, number of calls to skip before refreshing the frame
 *       - defaults to 3, hardcoded to 1 when activated
 * 1 - colour, hue in degrees around colour wheel.
 *       - defaults to 0, hardcoded to 120 when activated
 * 2 - colour change speed, in degrees per frame pixel update.
 *       - defaults to 9, hardcoded to 0 when touched
 * 3 - phase step in degrees for brightness.
 *       - defaults to 13, hardcoded to 33 when activated. Faster
 *         than 33 is sort of silly.
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "sine_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "utility.h"
#include "thm_node.h"
#include "qtouch.h"
#include "light_effects.h"
#include "patch_id.h"

thm_patch_t sine_patch = {
    .init     = &sine_init,
    .enter    = &sine_enter,
    .update   = &sine_update,
    .exit     = &sine_exit,
    .id       = PATCH_ID_SINE
};

uint8_t sine_timer = 0;
int8_t wave_index;
uint16_t phase;
rgb_t wave[4];

/*
 * PORTED from the Java class
 */
/**
 * Common initialization function. Set the pointers to the correct place.
 */
void sine_init() {   
    return;
}

/**
 *  Function to be called each time this patch begins.
 */
void sine_enter() {

	uint8_t i;

//    printf_P( PSTR("Entering Sine Patch\n\r") );

    mstimer_setLength( &patch_timer, 30 );
    mstimer_reset( &patch_timer );

	patch_data[ SINE_PATCH_SPEED_IDX ]       = 3;	// speed
	patch_data[ SINE_PATCH_COLOR_IDX ]       = 0;		// color
	patch_data[ SINE_PATCH_SHIFT_SPEED_IDX ] = 9;	// color shift speed in degrees around the colour wheel
	patch_data[ SINE_PATCH_PHASE_IDX ]       = 13;	// phase step size in degrees

	tlc_setAll( 0 );

//    printf_P( PSTR("p = %d\n\r"), p[0]);

	phase = 0; // phase in degrees
   wave_index = 0; // starting index in wave
   
   for(i = 0; i < 4; i++) {
      wave[i].red = 0;
      wave[i].green = 0;
      wave[i].blue = 0;
   }
}

/**
 *  Function to call
 */
void sine_update( int16_t* deltas ) {
   
   uint8_t i;
   static uint8_t j = 0, triggered = 0;
   static uint16_t oldparams[4];
   
   mstimer_reset( &patch_timer );

   /*
    * This is the part that responds to touching the center
    * capacitor.
    */
   if(triggered == 0 && abs(deltas[0]) > 20) {
      triggered = 1;
      for(i = 0; i < 4; i++) {
         oldparams[i] = patch_data[i];
      }                       // when activated:
      patch_data[ SINE_PATCH_SPEED_IDX ] = 1;   // - speed up
      patch_data[ SINE_PATCH_COLOR_IDX ] = 120; // - be red
      patch_data[ SINE_PATCH_SHIFT_SPEED_IDX ] = 0;   // - don't change colour
      patch_data[ SINE_PATCH_PHASE_IDX ] = 33;  // - cycle more quickly
   } else if (triggered == 1 && abs(deltas[0]) < 20) {
      triggered = 0;
      patch_data[ SINE_PATCH_SPEED_IDX ] = oldparams[ SINE_PATCH_SPEED_IDX ];

      // not restoring the old colour for smoothness
      patch_data[ SINE_PATCH_SHIFT_SPEED_IDX ] = oldparams[ SINE_PATCH_SHIFT_SPEED_IDX ];
      patch_data[ SINE_PATCH_PHASE_IDX ] = oldparams[ SINE_PATCH_PHASE_IDX ];
   }
   
	// slow it down
	if (sine_timer <= patch_data[ SINE_PATCH_SPEED_IDX ]) {
		sine_timer++;
		return;
	}

   THM_TLC_CS_SELECT();
   tlc_update();
   THM_TLC_CS_UNSELECT();
   
    sine_timer = 0;
	
   wave_index--;
   if(wave_index < 0)
      wave_index += 4;

   patch_data[ SINE_PATCH_COLOR_IDX ] = 
       (patch_data[ SINE_PATCH_COLOR_IDX ]
     + patch_data[ SINE_PATCH_SHIFT_SPEED_IDX ]) % 360;

   phase = (phase + patch_data[ SINE_PATCH_PHASE_IDX ]) % 360; 

   rgb_t *current = &(wave[wave_index]);
	HSVtoRGB(patch_data[ SINE_PATCH_COLOR_IDX ]/360.0, 1.0, sin(phase*3.1415659/360.0)*(.3+.2*triggered), &current);
	
   for(i = 0; i < 4; i++) {
      setLightRC(i, j, &wave[(i+wave_index)%4]);
	}
   j = (j+1)%4;
}

//extern void sine_message( void
void sine_exit( void ) {
	tlc_setAll( 0 );

   THM_TLC_CS_SELECT();
   tlc_update();
   THM_TLC_CS_UNSELECT();
}

