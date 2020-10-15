#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "wipe_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "utility.h"
#include "thm_node.h"

#define  WIPE_COLOR_IDX        0

static int16_t wipe_current_led;
static rgb_t   wipe_current_rgb;

thm_patch_t wipe_patch = {
    .init     = &wipe_init,
    .enter    = &wipe_enter,
    .update   = &wipe_update,
    .exit     = &wipe_exit,
    .state    = THM_PATCH_STATE_UNKNOWN,
    .progress = 0
};

/**
 * Common initialization function. Set the pointers to the correct place.
 */
void wipe_init() {
    wipe_patch.data[ WIPE_COLOR_IDX ] = 300;

    wipe_current_rgb.red   = TLC_MAXVAL;
    wipe_current_rgb.green = TLC_MAXVAL;
    wipe_current_rgb.blue  = TLC_MAXVAL;
}

/**
 *  Function to be called each time this patch begins.
 */
void wipe_enter() {
    printf_P( PSTR("Entering Wipe Patch\n\r") );
    wipe_current_led = 0;

    mstimer_setLength( &patch_timer, 30 );
    mstimer_reset( &patch_timer );

    tlc_setAll( 0 );
}

/**
 *  Function to call
 */
void wipe_update( int16_t* deltas ) {
   
    if ( mstimer_completed( &patch_timer ) ) { 
        mstimer_reset( &patch_timer );

        wipe_current_rgb.red   = deltas[0];
        wipe_current_rgb.green = deltas[1];
        wipe_current_rgb.blue  = deltas[2];

        thm_setLightingRGB( wipe_current_led, &wipe_current_rgb );

        wipe_current_led = (wipe_current_led + 1) % THM_LIGHTING_NUM_LEDS;

        THM_TLC_CS_SELECT();
        tlc_update();
        THM_TLC_CS_UNSELECT();
    }
}

//extern void wipe_message( void 
void wipe_exit( void ) {
    printf_P( PSTR("EXIT: Good bye\n\r") );
}

