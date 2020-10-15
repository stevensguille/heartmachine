#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "attractor_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"

#define  ATT_THRESHOLD_IDX      0

thm_patch_t attractor_patch = { 
    .init     = &attractor_init,
    .enter    = &attractor_enter,
    .update   = &attractor_update,
    .exit     = &attractor_exit,
    .state    = THM_PATCH_STATE_UNKNOWN,
    .progress = 0
};

/**
 * Common initialization function. Set the pointers to the correct place.
 */
void attractor_init() {
    attractor_patch.data[ ATT_THRESHOLD_IDX ] = 20;
}

/**
 *  Function to be called each time this patch begins.
 */
void attractor_enter() {
    printf_P( PSTR("Entering Attractor Patch\n\r") );
    attractor_patch.data[ ATT_THRESHOLD_IDX ] = 20;
}

/**
 *  Function to call 
 */
void attractor_update( int16_t* deltas ) {
    uint8_t channel;

    attractor_patch.data[ ATT_THRESHOLD_IDX ] = abs(deltas[0]);

    for ( channel=0; channel < NUM_TLCS*16; channel++ ) {
        tlc_set( channel, 
            (rand() % 256) > attractor_patch.data[ ATT_THRESHOLD_IDX ] ?
                TLC_MAXVAL : 0 );
    }

}

//extern void attractor_message( void 
void attractor_exit( void ) {
    printf_P( PSTR("EXIT: Good bye\n\r") );
}

