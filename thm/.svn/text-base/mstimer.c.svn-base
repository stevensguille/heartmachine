/**
 *  The mstimer module provides a simple to use timer class to measure elapsed
 *  time in milliseconds. Multiple instances can measure any number of time
 *  intervals. A millisecond timebase is required using one of the timers 
 *  on board the AVR
 *
 *  A global counter of milliseconds is available in gMilliSecTick. It is
 *  necessary to call updateTick() at a regular interval that is a multiple
 *  of 1ms for the timers to operate.
 *  
 *  note: gMilliSecTick will over flow at 2^16-1 milliseconds. The timers
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "mstimer.h"
#include "thm_node.h"

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

/**
 *  Loops until the timer is completed
 */
void mstimer_waitUntilComplete( mstimer_t* timer ) {
    while ( !mstimer_completed( timer ) );
}

/**
 *  Delay a specified amount of milliseconds
 */
void mstimer_delayms( uint16_t delay_ms) {
    mstimer_t delay_timer;
    mstimer_init( &delay_timer, delay_ms );
    mstimer_waitUntilComplete( &delay_timer );
}

/**
 *  Initialize the hardware timer to generate the 1 millisecond tick.
 */
void mstimer_initTimer() {
    MSTIMER_OCR = MSTIMER_OCR_VAL;

    // Enable the Interrupt for Compare Match
    MSTIMER_TIMSK |= MSTIMER_OCIE;

    // Set Timer Pre_scaler  ( system clock / 1024 )
    MSTIMER_TCCRB |= CS_PRESCALE;
    // ( 1 << CS12 ) | ( 1 << CS10 );

    // Set Timer Mode and start CTC (count up to Compare, then reset) 
    MSTIMER_TCCRB |= MSTIMER_WGM;
}


static uint8_t pwm_count = 0;
static uint8_t ms_count = 0;

/**
 *  Interrupt handler for timer compare match.
 *
 *  Used to update the globe millisecond tick value.
 */
ISR(MSTIMER_INT_VECT) {
    if ( pwm_count > sol1_pwm_val && IS_PWM_ENABLED_SOL1() ) {
        THM_CLEAR_SOL1();
    }

    if ( pwm_count > sol2_pwm_val && IS_PWM_ENABLED_SOL2() ) {
        THM_CLEAR_SOL2();
    }

    if ( pwm_count++ >= SOL_PWM_COUNT_MAX ) {
        pwm_count = 0;
        if ( sol1_pwm_val > 0 && IS_PWM_ENABLED_SOL1() )
            THM_SET_SOL1();

        if ( sol2_pwm_val > 0 && IS_PWM_ENABLED_SOL2() )
            THM_SET_SOL2();
    }
    
    if ( ms_count++ >= INTS_PER_MS ) {
        ms_count = 0;
        gMilliSecTick++;
    }
}

