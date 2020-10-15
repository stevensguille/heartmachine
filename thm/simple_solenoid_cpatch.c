/*
 * simple_solenoid_patch.c
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
#include "simple_solenoid_cpatch.h"
#include "mstimer.h"
#include "utility.h"
#include "thm_node.h"
#include "patch_id.h"

#define S_SOL1_MAX_IDX 0
#define S_SOL2_MAX_IDX 1
#define S_SOL_SEL_IDX  2

/*
 * param[0] = length of burn 1
 * param[1] = length of burn 2
 * param[2] = mode: 0 = burn1, 1 = burn2, 2 = both
 */

thm_cpatch_t simple_solenoid_patch = {
    .init       = &simple_solenoid_init,
    .enter      = &simple_solenoid_enter,
    .update     = &simple_solenoid_update,
    .activate   = &simple_solenoid_activate,
    .deactivate = &simple_solenoid_deactivate,
    .exit       = &simple_solenoid_exit,
    .id         = CPATCH_ID_SIMPLE_SOLENOID
};

#define DEF_MAX_DURATION 300 // ms
/**
 * Common initialization function. Set the pointers to the correct place.
 */

#define S_SOL_IDLE   0
#define S_SOL_FIRING 1

uint8_t s_sol_state = S_SOL_IDLE;

void simple_solenoid_init() {
}

/**
 *  Function to be called each time this patch begins.
 */
void simple_solenoid_enter() {
	printf_P( PSTR("Entering Simple Solenoid Patch\n\r") );

    mstimer_setLength( &cpatch_timer, 30 );
    mstimer_reset( &cpatch_timer );
	cpatch_data[S_SOL1_MAX_IDX] = DEF_MAX_DURATION; // steps to wait between pulses
	cpatch_data[S_SOL2_MAX_IDX] = DEF_MAX_DURATION; // steps to wait between pulses
	cpatch_data[S_SOL_SEL_IDX]  = THM_SOL_BOTH; // both by default

    DISABLE_PWM_SOL1();
    DISABLE_PWM_SOL2();

    THM_CLEAR_SOL1();
    THM_CLEAR_SOL2();
}

/**
 *  Function to call
 */
void simple_solenoid_update( int16_t* deltas ) {
    mstimer_reset( &cpatch_timer );

    if ( mstimer_completed( &sol1_timer ) ) {
        THM_CLEAR_SOL1();
    }

    if ( mstimer_completed( &sol2_timer ) ) {
        THM_CLEAR_SOL2();
    }

    if (mstimer_completed( &sol1_timer ) &&
        mstimer_completed( &sol2_timer ) ) {
        s_sol_state = S_SOL_IDLE;
        mstimer_setLength( &cpatch_timer, 30 );
        mstimer_reset( &cpatch_timer );
    }

    return;
}



/**
 *  Activate the patch's behaviour ?
 */
void simple_solenoid_activate( int16_t* params ) {
	mstimer_setLength( &sol1_timer, cpatch_data[S_SOL1_MAX_IDX]);
	mstimer_setLength( &sol2_timer, cpatch_data[S_SOL2_MAX_IDX]);
    mstimer_reset( &sol2_timer );

	switch ( cpatch_data[S_SOL_SEL_IDX] ) {
		case THM_SOL1:
			THM_SET_SOL1();
            mstimer_reset( &sol1_timer );
            break;
		case THM_SOL2:
            THM_SET_SOL2();
            mstimer_reset( &sol2_timer );
			break;

		case THM_SOL_BOTH:
			THM_SET_SOL1();
            THM_SET_SOL2();
            mstimer_reset( &sol1_timer );
            mstimer_reset( &sol2_timer );
            break;
    }
    
    mstimer_setLength( &cpatch_timer, 5 );
    mstimer_reset( &cpatch_timer );
    s_sol_state = S_SOL_FIRING;

    return;
}

void simple_solenoid_deactivate() {
    printf_P(PSTR("solenoid deactivate %d\n\r"), simple_solenoid_patch.data[2]);
    s_sol_state = S_SOL_IDLE;
}

//extern void solenoid_message( void
void simple_solenoid_exit( void ) {
    DISABLE_PWM_SOL1();
    DISABLE_PWM_SOL2();
}

