/**
 *  A collection of utility functions for various purposed.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/io.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

/**
 *  A helper function to parse arguments from a string using sscanf.
 *
 *  'fmt' specifies the specifier used in sscanf. Results are 
 *  stored in 'ret'
 *
 *  returns number of arguments successfully parsed.
 */
uint8_t getArgs(char* arg_str, char fmt, uint8_t num_args, int16_t ret[]) {
    int i;
    char fmt_str[3];
    char* pchr;

    // build the format string for sscanf.
    sprintf(fmt_str, "%%%c", fmt);

    pchr = strtok( arg_str, " " );

    for( i=0; i<num_args; i++ ) {
        // no more arguments to parse.
        if ( pchr == NULL ) {
            break;
        }
        
        // error parsing
        if ( sscanf( pchr, fmt_str, &ret[i] ) != 1 ) break;

        pchr = strtok( NULL, " " );
    }

    return i;
} 

void tween_set_value( linear_tween_t* tween, int val ) {
    tween->cur_val      = val;
    tween->target_val   = val;
    tween->num_steps    = 0;
    tween->step_count   = 0;
}

void tween_set_target( linear_tween_t* tween, uint8_t num_steps, int16_t target ) {
    tween->target_val = target;
    tween->step_val   = (target - tween->cur_val) / num_steps;
    tween->step_count = 0;
    tween->num_steps  = num_steps;
}

void tween_init( linear_tween_t* tween, int16_t value ) {
    tween_set_value( tween, value );
}

int16_t tween_get_next( linear_tween_t* tween ) {
    if ( tween->step_count >= tween->num_steps ) {
        tween->cur_val = tween->target_val;
    } else {
        tween->cur_val += tween->step_val;
        tween->step_count++;
    }

    return tween->cur_val;
}

int8_t lsb_set( uint8_t val ) {
    int i = 0;

    while( i <= 7 ) {
        if ( val >> i & 0x01 )
            return i;
        i++;
    }

    return -1;
}   

