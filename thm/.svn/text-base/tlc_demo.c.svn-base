#include "tlc_demo.h"
#include "Tlc5940.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static char tlc_mode = TLC_MODE_TWINKLE;

// used to store any variables required for a mode
static int16_t modeData[TLC_MODE_NUM_DATA];

static void tlc_initMode( void ) {
    switch( tlc_mode ) {
        case TLC_MODE_PULSATE:
            modeData[P_STEP] = 200;
            modeData[P_BOT]  = 0;
            modeData[P_TOP]  = TLC_MAXVAL;
            modeData[P_DIR]  = 1;
            break;
        case TLC_MODE_TWINKLE:
            modeData[T_THRESH] = 200;
            modeData[T_VAL]    = TLC_MAXVAL;
            break;
            
        default:
            break;
    }
}


/**
 *  Check if 'mode' is valid
 */
uint8_t tlc_isValidMode( char mode ) {
    if ( mode   != TLC_MODE_PULSATE \
        && mode != TLC_MODE_STATIC \
        && mode != TLC_MODE_RANDOM \
        && mode != TLC_MODE_WALK \
        && mode != TLC_MODE_TWINKLE )
        return TLC_INVALID_MODE;
    else
        return TLC_VALID_MODE;
}

/**
 *  Update the TLC5940 depending on current mode.
 */
void tlc_updateMode( void ) { 
    uint8_t channel;
    int16_t step;
    int16_t val;

    switch( tlc_mode ) {
        case TLC_MODE_PULSATE:
            modeData[P_VAL] = modeData[P_VAL]
                            + (modeData[P_STEP] * modeData[P_DIR]);
            
            // clamp the resulting value.
            modeData[P_VAL] = ( modeData[P_VAL] < modeData[P_BOT] ?
                                modeData[P_BOT] : modeData[P_VAL] );
            modeData[P_VAL] = ( modeData[P_VAL] > modeData[P_TOP] ?
                                modeData[P_TOP] : modeData[P_VAL] );

            tlc_setAll(modeData[P_VAL]);

            // reverse the step value if at the edge
            if ( ( modeData[P_VAL] >= modeData[P_TOP] && modeData[P_DIR] == 1 )
            || ( modeData[P_VAL] <= modeData[P_BOT] && modeData[P_DIR] == -1 ) )
                modeData[P_DIR] *= -1;
            break;

        case TLC_MODE_STATIC:
            // Nothing to see here.
            break;

        case TLC_MODE_RANDOM:
            for( channel=0; channel<NUM_TLCS*16; channel++ ) {
                tlc_set( channel, (uint16_t)(rand()) % 700 );
            }
            break;

        case TLC_MODE_WALK:

            for( channel=0; channel<NUM_TLCS*16; channel++ ) {
                val = tlc_get(channel);
                step = ( (rand() % 2) ? -1 : 1 ) * ( abs(rand()) % 50 );

                tlc_set( channel, \
                   (int16_t)tlc_get(channel) \
                 + ((rand() % 2) == 0 ? -1 : 0) * (int16_t)(rand() % 50 ));
            }
            break;

        case TLC_MODE_TWINKLE:
            for( channel=0; channel<NUM_TLCS*16; channel++ ) {
                tlc_set( channel, (rand() % 256) > modeData[T_THRESH] ?
                    modeData[T_VAL] : 0 );
            }

        default:
            break;
    }

    tlc_update();
}

void tlc_setMode( char mode ) {
    if( tlc_isValidMode( mode ) == TLC_VALID_MODE ) {
        tlc_mode = mode;
        tlc_initMode();
    }
}

void tlc_setModeData( uint8_t index, int16_t val ) {
    modeData[index] = val;
}

