/**
 *  The mstimer module provides a simple to use timer class to measure elapsed
 *  time in milliseconds. Multiple instances can measure any number of time
 *  intervals.
 *
 *  A global counter of milliseconds is available in gMilliSecTick. It is
 *  necessary to call updateTick() at a regular interval that is a multiple
 *  of 1ms for the timers to operate.
 * 
 *  note: gMilliSecTick will over flow at 2^16-1 milliseconds. The timers
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#ifndef _MSTIMER_H_
#define _MSTIMER_H_

#include <inttypes.h>

#define TICKS_PER_MS     (uint16_t)(F_CPU / 1000)
#define CS_PRESCALER     (uint16_t)64

// atmega328 Timer Clock Source Prescalar
#define CS_PRESCALE_1    _BV(CS10)
#define CS_PRESCALE_8    _BV(CS11)
#define CS_PRESCALE_64   _BV(CS11) | _BV(CS10)
#define CS_PRESCALE_256  _BV(CS12)
#define CS_PRESCALE_1024 _BC(CS12) | _BC(CS10)

// MS_PER_UPDATE is determined by the frequency at which updateTick() is called.
// It should not be less than 1.
#ifndef MS_PER_UPDATE
#define MS_PER_UPDATE 1
#endif

volatile uint16_t gMilliSecTick;

typedef struct _mstimer_t {
    uint16_t start;
    uint16_t length;
} mstimer_t;

void resetTick( void );
inline void updateTick( void ); 

void mstimer_init(mstimer_t* timer, uint16_t length);
void mstimer_setLength(mstimer_t* timer, uint16_t new_length);
void mstimer_reset(mstimer_t* timer);

uint16_t mstimer_elapsed(mstimer_t* timer);
uint8_t mstimer_completed(mstimer_t* timer);

#endif
