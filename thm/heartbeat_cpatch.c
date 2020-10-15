/*
 * heartbeat_patch.c
 *
 *  Created on: Aug 4, 2010
 *      Author: max
 */

#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "heartbeat_cpatch.h"
#include "mstimer.h"
#include "utility.h"
#include "thm_node.h"
#include "patch_id.h"

#define HB_TEMPO_IDX    0
#define HB_TOP_IDX      1
#define HB_BOTTOM_IDX   2
#define HB_RISE_IDX     3
#define HB_FALL_IDX     4
#define HB_DELAY1_IDX   5
#define HB_DELAY2_IDX   6
#define HB_SOL_SEL_IDX  7

#define HB_BUMP1        0
#define HB_DELAY1       1
#define HB_BUMP2        2
#define HB_DELAY2       3

uint8_t hb_state;
mstimer_t hb_delay_timer;
static linear_tween_t a_tween;

thm_cpatch_t heartbeat_patch = {
    .init       = &heartbeat_init,
    .enter      = &heartbeat_enter,
    .update     = &heartbeat_update,
    .activate   = &heartbeat_activate,
    .deactivate = &heartbeat_deactivate,
    .exit       = &heartbeat_exit,
    .id         = CPATCH_ID_HEARTBEAT
};

/**
 * Common initialization function. Set the pointers to the correct place.
 */
void heartbeat_init() {
    tween_init( &a_tween, 0 );
    mstimer_init( &hb_delay_timer, 0 );
}

/**
 *  Function to be called each time this patch begins.
 */
void heartbeat_enter() {
	printf_P( PSTR("Entering Heartbeat Patch\n\r") );

    mstimer_setLength( &cpatch_timer, 30 );
    mstimer_reset( &cpatch_timer );

    // Delays between transitions
    cpatch_data[HB_TEMPO_IDX]   = 30;
    cpatch_data[HB_TOP_IDX]     = SOL_PWM_COUNT_MAX;
    cpatch_data[HB_BOTTOM_IDX]  = 0;
    cpatch_data[HB_RISE_IDX]    = 8;   // steps
    cpatch_data[HB_FALL_IDX]    = 16;
    cpatch_data[HB_DELAY1_IDX]   = 100; // ms
    cpatch_data[HB_DELAY2_IDX]   = 500; // ms
    cpatch_data[HB_SOL_SEL_IDX] = THM_SOL_BOTH; // ms

    ENABLE_PWM_SOL1();
    ENABLE_PWM_SOL2();

    hb_state = HB_BUMP1;
}


/**
 *  Function to call
 */
void heartbeat_update( int16_t* deltas ) {
    int16_t val;

    mstimer_setLength( &cpatch_timer, cpatch_data[HB_TEMPO_IDX] );
    mstimer_reset( &cpatch_timer );

    switch(hb_state) {
        case HB_BUMP1:
        case HB_BUMP2:
            if ( a_tween.step_count == a_tween.num_steps ) {
                // We're done 
                if ( a_tween.target_val <= cpatch_data[HB_BOTTOM_IDX] ) {
                    // We've completed a falling transition. Done the Bump
                    mstimer_setLength( &hb_delay_timer, 
                        (hb_state == HB_BUMP1) ? cpatch_data[HB_DELAY1_IDX] : cpatch_data[HB_DELAY2_IDX]);
                    mstimer_reset( &hb_delay_timer );
                    hb_state = (hb_state == HB_BUMP1) ? HB_DELAY1 : HB_DELAY2;
                } else {
                    // We've completed a rising transition start the falling
                    // transition
                    tween_set_target( &a_tween, cpatch_data[HB_FALL_IDX],
                           cpatch_data[HB_BOTTOM_IDX] );
                }
            }

            val = tween_get_next(&a_tween);

            switch(cpatch_data[HB_SOL_SEL_IDX]) {
                case THM_SOL_BOTH: // ms
                    sol1_pwm_val = val;
                    sol2_pwm_val = val;
                    break;
                
                case THM_SOL1:
                    sol1_pwm_val = val;
                    sol2_pwm_val = 0;
                    break;

                case THM_SOL2:
                    sol1_pwm_val = 0;
                    sol2_pwm_val = val;
                    break;

                default:
                    sol1_pwm_val = 0;
                    sol2_pwm_val = 0;
            }

            break;

        case HB_DELAY1:
        case HB_DELAY2:
            if ( mstimer_completed(&hb_delay_timer) ) {
                hb_state = (hb_state == HB_DELAY1) ? HB_BUMP2 : HB_BUMP1;
                // Start the rise transition
                tween_set_target( &a_tween, cpatch_data[HB_RISE_IDX], cpatch_data[HB_TOP_IDX] );
            }
            break;
    }

    return;
}

/**
 *  Activate the patch's behaviour ?
 */
void heartbeat_activate( int16_t*  params ) {
    // Reset the timers and counters.
    mstimer_reset( &cpatch_timer );
    mstimer_reset( &hb_delay_timer );
    sol1_pwm_val = 0;
    sol2_pwm_val = 0;
}

void heartbeat_deactivate( ) {
    return;
}

//extern void solenoid_message( void
void heartbeat_exit( void ) {
    DISABLE_PWM_SOL1();
    DISABLE_PWM_SOL2();
    THM_CLEAR_SOL1();
    THM_CLEAR_SOL2();
}

