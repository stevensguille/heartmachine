/******************************************************************************
 * Copyright (C) 2005 Martin THOMAS, Kaiserslautern, Germany
 * <eversmith@heizung-thomas.de>
 * http://www.siwawi.arubi.uni-kl.de/avr_projects
 *****************************************************************************
 *
 * File    : timebase.c
 * Version : 0.9
 * 
 * Summary : AVR Timebase using Hardware-Timer 0
 *
 *****************************************************************************/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "mstimer.h"

inline void updateTick() { gMilliSecTick += MS_PER_UPDATE; }

/**
 *  We should reset the tick count periodically to prevent
 *  overflow performing comparisons with the tick count.
 *
 *  Note: Any mstimers should be reset after a resetTick()
 *  call before calling mstimer_elapsed() or mstimer_completed()
 */
void resetTick() { 
    uint8_t sreg;

    // Disable Interrupts while we clear the counter.
    sreg = SREG;
    cli();

    gMilliSecTick = 0;

    SREG = sreg;
}

/**
 * Initialize a timer object.
 *
 * length determines how long before mstimer_completed() returns
 * true.
 */
void mstimer_init(mstimer_t* timer, uint16_t length) {
    mstimer_setLength( timer, length );
    mstimer_reset( timer );
}

/**
 * Set the lenght of the timer, which determines how long before
 * mstimer_completed() returns true.
 */
void mstimer_setLength(mstimer_t* timer, uint16_t n_length) {
    timer->length = n_length;
}

/**
 *  Reset the internal counter for this timer.
 */
void mstimer_reset(mstimer_t* timer) {
    uint8_t sreg;

    // Disable Interrupts while we get the value of the counter.
    sreg = SREG;
    cli();

    timer->start = gMilliSecTick;

    SREG = sreg;
}

/**
 *  Returns the number of MS since timer was initialized or reset.
 */
uint16_t mstimer_elapsed( mstimer_t* timer ) {
    return gMilliSecTick - timer->start;
}

/**
 *  Returns true if more than timer->length milliseconds have passed
 *  since a call to mstimer_init() or mstimer_reset()
 */
uint8_t mstimer_completed(mstimer_t* timer ) {
    return ( mstimer_elapsed( timer ) >= timer->length );
}

