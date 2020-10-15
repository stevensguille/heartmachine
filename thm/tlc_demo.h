#ifndef _TLC_DEMO_H_
#define _TLC_DEMO_H_

#include <inttypes.h>

#define TLC_VALID_MODE    0
#define TLC_INVALID_MODE  1

#define TLC_MODE_PULSATE 'p'
#define TLC_MODE_STATIC  's'
#define TLC_MODE_RANDOM  'r'
#define TLC_MODE_WALK    'w'
#define TLC_MODE_TWINKLE 't'

// The number of state variables for each mode
#define TLC_MODE_NUM_DATA 8

// used to store any variables required for a mode

#define P_STEP     0
#define P_BOT      1
#define P_TOP      2
#define P_DIR      3
#define P_VAL      4

#define T_THRESH   0
#define T_VAL      1

/**
 *  Update the TLC5940 depending on current mode.
 */
extern void    tlc_updateMode( void );
extern void    tlc_setMode( char mode );
extern uint8_t tlc_getMode( void );
extern uint8_t tlc_isValidMode( char mode );
extern void    tlc_setModeData( uint8_t index, int16_t val );

#endif
