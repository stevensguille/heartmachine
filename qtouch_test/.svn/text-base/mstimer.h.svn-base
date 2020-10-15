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
 *  alexander kon-i ho <koniho@gmail.com> 2010
 */
#ifndef _MSTIMER_H_
#define _MSTIMER_H_

#include <inttypes.h>

#ifndef MSTIMER_TIMER_NUM
    #define MSTIMER_TIMER_NUM   1
#endif

#ifndef MSTIMER_CS_PRESCALE
    #define CS_PRESCALER ((uint16_t)64)
#endif

#define TICKS_PER_MS     (uint16_t)(F_CPU / 1000)

#if MSTIMER_TIMER_NUM == 0
    #define MSTIMER_OCR         OCR0A
    #define MSTIMER_OCIE        _BV(OCIE1A)
    #define MSTIMER_TIMSK       TIMSK1
    #define MSTIMER_TCCRB       TCCR1B
    #define MSTIMER_WGM         _BV(WGM12)
    #define MSTIMER_INT_VECT    TIMER1_COMPA_vect

    #define CS_PRESCALE_1       _BV(CS00)
    #define CS_PRESCALE_8       _BV(CS01)
    #define CS_PRESCALE_64      _BV(CS01) | _BV(CS00)
    #define CS_PRESCALE_256     _BV(CS02)
    #define CS_PRESCALE_1024    _BV(CS02) | _BV(CS00)

#elif  MSTIMER_TIMER_NUM == 1
    #define MSTIMER_OCR         OCR1A
    #define MSTIMER_OCIE        _BV(OCIE1A)
    #define MSTIMER_TIMSK       TIMSK1
    #define MSTIMER_TCCRB       TCCR1B
    #define MSTIMER_WGM         _BV(WGM12)
    #define MSTIMER_INT_VECT    TIMER1_COMPA_vect

    #define CS_PRESCALE_1       _BV(CS10)
    #define CS_PRESCALE_8       _BV(CS11)
    #define CS_PRESCALE_64      _BV(CS11) | _BV(CS10)
    #define CS_PRESCALE_256     _BV(CS12)
    #define CS_PRESCALE_1024    _BV(CS12) | _BV(CS10)

#elif  MSTIMER_TIMER_NUM == 2
    #define MSTIMER_OCR         OCR2A
#endif


volatile uint16_t gMilliSecTick;

typedef struct _mstimer_t {
    uint16_t start;
    uint16_t length;
} mstimer_t;

void resetTick( void );

void mstimer_initTimer( void );

void mstimer_init(mstimer_t* timer, uint16_t length);
void mstimer_setLength(mstimer_t* timer, uint16_t new_length);
void mstimer_reset(mstimer_t* timer);

uint16_t mstimer_elapsed(mstimer_t* timer);
uint8_t  mstimer_completed(mstimer_t* timer);

#endif
