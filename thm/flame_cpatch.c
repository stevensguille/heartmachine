/*
 * flame_cpatch.c
 */

#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "flame_cpatch.h"
#include "mstimer.h"
#include "utility.h"
#include "thm_node.h"
#include "patch_id.h"

#define FLAME_NONE 0
#define FLAME1     1
#define FLAME2     2
#define FLAME_BOTH 3

#define SENTINEL   255

#define FLAME_TEMPO_IDX     0
#define FLAME_FRAME_IDX     1
#define FLAME_COUNT_IDX     2

// We will rock you?
static uint8_t flame_switch[][2] = {
    { 5, FLAME1 },
    { 20, FLAME_NONE },
    { 5, FLAME2 },
    { 20, FLAME_NONE },
    { 5, FLAME_BOTH },
    { 45, FLAME_NONE },
    { 0, SENTINEL }
};

thm_cpatch_t flame_patch = {
    .init       = &flame_init,
    .enter      = &flame_enter,
    .update     = &flame_update,
    .activate   = &flame_activate,
    .deactivate = &flame_deactivate,
    .exit       = &flame_exit,
    .id         = CPATCH_ID_FLAME
};

/**
 * Common initialization function. Set the pointers to the correct place.
 */
void flame_init() {
    return;
}

/**
 *  Function to be called each time this patch begins.
 */
void flame_enter() {
	printf_P( PSTR("Entering Flame Patch\n\r") );
    
 	cpatch_data[ FLAME_TEMPO_IDX ] = 10; // steps to wait between pulses
    mstimer_setLength( &cpatch_timer, cpatch_data[ FLAME_TEMPO_IDX ] );

    cpatch_data[ FLAME_FRAME_IDX ] = 0;
    cpatch_data[ FLAME_COUNT_IDX ] = 0;

    DISABLE_PWM_SOL1();
    DISABLE_PWM_SOL2();
    THM_CLEAR_BOTH_SOL();


    mstimer_reset( &cpatch_timer );
}

/**
 *  Function to call
 */
void flame_update( int16_t* deltas ) {
    mstimer_setLength( &cpatch_timer, cpatch_data[ FLAME_TEMPO_IDX ] );
    mstimer_reset( &cpatch_timer );

    cpatch_data[ FLAME_COUNT_IDX ]++;

    if ( cpatch_data[ FLAME_COUNT_IDX ]++
            >= flame_switch[cpatch_data[FLAME_FRAME_IDX]][0] ) {
        cpatch_data[ FLAME_COUNT_IDX ] = 0;

        //cpatch_data[ FLAME_FRAME_IDX ] = (cpatch_data[ FLAME_FRAME_IDX ] + 1) & 0x07;
        cpatch_data[ FLAME_FRAME_IDX ] = (cpatch_data[ FLAME_FRAME_IDX ] + 1);
        if ( flame_switch[cpatch_data[ FLAME_FRAME_IDX ]][1] == SENTINEL )
            cpatch_data[ FLAME_FRAME_IDX ] = 0;

        switch( flame_switch[ cpatch_data[FLAME_FRAME_IDX ] ][1] ) {
            case FLAME_BOTH:
                THM_SET_BOTH_SOL();
                break;
            case FLAME1:
                THM_SET_SOL1();
                break;
            case FLAME2:
                THM_SET_SOL2();
                break;
            case FLAME_NONE:
                THM_CLEAR_BOTH_SOL();
                break;
            default:
                break;
        }
    }
}

/**
 *  Activate the patch's behaviour ?
 */
void flame_activate( int16_t* params ) {
    return;
}

void flame_deactivate( ) {
    return;
}

//extern void solenoid_message( void
void flame_exit( void ) {
    THM_CLEAR_SOL1();
    THM_CLEAR_SOL2();
}

