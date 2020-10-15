/**
 *  Pin and CPU Definitions for The Heart Machine.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <avr/io.h>

#define F_CPU           16000000
#define BAUD_RATE       57600

// The Heart Machine specific signals
#include "thm_defines.h"

#ifndef PB0
#define PB0     PORTB0
#define PB1     PORTB1
#define PB2     PORTB2
#define PB3     PORTB3
#define PB4     PORTB4
#define PB5     PORTB5
#define PB6     PORTB6
#define PB7     PORTB7
#endif
#ifndef PC0
#define PC0     PORTC0
#define PC1     PORTC1
#define PC2     PORTC2
#define PC3     PORTC3
#define PC4     PORTC4
#define PC5     PORTC5
#define PC6     PORTC6
#define PC7     PORTC7
#endif
#ifndef PD0
#define PD0     PORTD0
#define PD1     PORTD1
#define PD2     PORTD2
#define PD3     PORTD3
#define PD4     PORTD4
#define PD5     PORTD5
#define PD6     PORTD6
#define PD7     PORTD7
#endif
#ifndef PH0
#define PH0     PORTH0
#define PH1     PORTH1
#define PH2     PORTH2
#define PH3     PORTH3
#define PH4     PORTH4
#define PH5     PORTH5
#define PH6     PORTH6
#define PH7     PORTH7
#endif

#endif
