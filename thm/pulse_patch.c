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
#include "light_effects.h"
#include "patch_id.h"

#define  PULSE_TEMPO_IDX          0
#define  PULSE_STEPS_IDX          1
#define  PULSE_R_BOTTOM_IDX       2
#define  PULSE_G_BOTTOM_IDX       3
#define  PULSE_B_BOTTOM_IDX       4
#define  PULSE_R_TOP_IDX          5
#define  PULSE_G_TOP_IDX          6
#define  PULSE_B_TOP_IDX          7

static uint8_t  pulse_steps;
static int16_t  pulse_scale;

uint8_t* pulse_frame_pointer;

thm_patch_t pulse_patch = {
    .init     = &pulse_init,
    .enter    = &pulse_enter,
    .update   = &pulse_update,
    .exit     = &pulse_exit,
    .id       = PATCH_ID_PULSE
};

/*
 * Common initialization function. Set the pointers to the correct place.
 */
void pulse_init() {
}

/**
 *  Function to be called each time this patch begins.
 */
void pulse_enter() {
    rgb_t rgb;

    printf_P( PSTR("Entering Pulse Patch\n\r") );

    patch_data[ PULSE_TEMPO_IDX ]    = 30;
    patch_data[ PULSE_STEPS_IDX ]    = 32;
    patch_data[ PULSE_R_BOTTOM_IDX ] = 32;
    patch_data[ PULSE_G_BOTTOM_IDX ] = 500;
    patch_data[ PULSE_B_BOTTOM_IDX ] = 250;
    patch_data[ PULSE_R_TOP_IDX ]    = 512;
    patch_data[ PULSE_G_TOP_IDX ]    = 512;
    patch_data[ PULSE_B_TOP_IDX ]    = 1024;

    pulse_scale = 1;

    rgb.red = patch_data[ PULSE_R_BOTTOM_IDX ];
    rgb.green = patch_data[ PULSE_G_BOTTOM_IDX ];
    rgb.blue = patch_data[ PULSE_B_BOTTOM_IDX ];

    setLightAll( &rgb );

    // ~ 30 updates per second
    mstimer_setLength( &patch_timer, patch_data[ PULSE_TEMPO_IDX ] );

    pulse_steps = 64;

    tlc_setAll_buf( tlc_buf1, 0 );
    tlc_setAll_buf( tlc_buf2, 0 );

    pulse_frame_pointer = tlc_buf1;
    fadeTo( pulse_frame_pointer, pulse_steps );

    mstimer_reset( &patch_timer );
}


/**
 *  Function to call 
 */
void pulse_update( int16_t* deltas ) {
    rgb_t rgb;

    //patch_data[PULSE_TEMPO_IDX] = CLAMP( (50-deltas[0]), 3, 50);
    //patch_data[PULSE_TEMPO_IDX] = 10;
    mstimer_setLength( &patch_timer, patch_data[ PULSE_TEMPO_IDX ] );

    // Call the Fade function and if the fade is complete,
    // update the target frame
    if ( fadeTo( NULL, pulse_steps ) < 0 ) {
        // We've finished the current frame. Flip to the next.
        if ( pulse_frame_pointer == tlc_buf1 ) {
            // Update the target frame with qtouch values
            rgb.red   = patch_data[ PULSE_R_BOTTOM_IDX ];
            rgb.green = patch_data[ PULSE_G_BOTTOM_IDX ];
            rgb.blue  = patch_data[ PULSE_B_BOTTOM_IDX ];

            setLightAll_buf( tlc_buf2, &rgb );
            //printf_P( PSTR("%u\n\r"), tlc_get_buf( tlc_buf2, 0 ));
            pulse_frame_pointer = tlc_buf2;
            fadeTo( pulse_frame_pointer, pulse_steps );
        } else {
            rgb.red   = patch_data[ PULSE_R_TOP_IDX ];
            rgb.green = patch_data[ PULSE_G_TOP_IDX ];
            rgb.blue  = patch_data[ PULSE_B_TOP_IDX ];

            setLightAll_buf( tlc_buf1, &rgb );
            //printf_P( PSTR("%u\n\r"), tlc_get_buf( tlc_buf1, 0 ));
            pulse_frame_pointer = tlc_buf1;
            fadeTo( pulse_frame_pointer, pulse_steps );
        }

        // Now we'll set the number of steps from the qtouch deltas.
        //fadeTo( pulse_frame_pointer, pulse_steps );
    }

    THM_TLC_CS_SELECT();
    tlc_update();
    THM_TLC_CS_UNSELECT();
}

void pulse_exit( void ) {
	tlc_setAll( 0 );

   THM_TLC_CS_SELECT();
   tlc_update();
   THM_TLC_CS_UNSELECT();
}

