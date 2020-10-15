#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "pulse_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "utility.h"
#include "thm_node.h"

#define  PULSE_STRIDE_IDX       0
#define  PULSE_BOTTOM_IDX       1
#define  PULSE_TOP_IDX          2

static int16_t pulse_current_val;
static int16_t pulse_current_dir;

thm_patch_t pulse_patch = {
    .init     = &pulse_init,
    .enter    = &pulse_enter,
    .update   = &pulse_update,
    .exit     = &pulse_exit,
    .state    = THM_PATCH_STATE_UNKNOWN,
    .progress = 0
};


/**
 * Common initialization function. Set the pointers to the correct place.
 */
void pulse_init() {
    pulse_patch.data[ PULSE_STRIDE_IDX ] = 20;
    pulse_patch.data[ PULSE_BOTTOM_IDX ] = 0;
    pulse_patch.data[ PULSE_TOP_IDX ]    = TLC_MAXVAL;

    pulse_current_val = pulse_patch.data[ PULSE_BOTTOM_IDX ];
    pulse_current_dir = 1;
}

/**
 *  Function to be called each time this patch begins.
 */
void pulse_enter() {
    printf_P( PSTR("Entering Pulse Patch\n\r") );
    pulse_current_val = pulse_patch.data[ PULSE_BOTTOM_IDX ];
    pulse_current_dir = 1;

    mstimer_setLength( &patch_timer, 30 );
}

/**
 *  Function to call 
 */
void pulse_update( int16_t* deltas ) {
   
    if ( mstimer_completed( &patch_timer ) ) { 
        mstimer_reset( &patch_timer );

        pulse_current_val += ( pulse_patch.data[ PULSE_STRIDE_IDX ] * pulse_current_dir );

        // clamp the resulting value
        pulse_current_val = CLAMP( pulse_current_val,
                pulse_patch.data[ PULSE_BOTTOM_IDX ],
                pulse_patch.data[ PULSE_TOP_IDX ]);

        pulse_patch.data[ PULSE_STRIDE_IDX ] = CLAMP( abs(deltas[0]), 200, 1000 );
        pulse_patch.data[ PULSE_TOP_IDX ]    = CLAMP( abs(deltas[1]), 300, TLC_MAXVAL );

        // printf_P( PSTR( "%d:%d\n\r" ) ,pulse_current_val, pulse_current_dir );

        tlc_setAll(pulse_current_val);

        if ( ( pulse_current_dir == 1  && 
              pulse_current_val >= pulse_patch.data[ PULSE_TOP_IDX ] )
          || ( pulse_current_dir == -1 && 
              pulse_current_val <= pulse_patch.data[ PULSE_BOTTOM_IDX ] ) )
            pulse_current_dir *= -1;

        THM_TLC_CS_SELECT();
        tlc_update();
        THM_TLC_CS_UNSELECT();
    }
}

//extern void pulse_message( void 
void pulse_exit( void ) {
    printf_P( PSTR("EXIT: Good bye\n\r") );
}

