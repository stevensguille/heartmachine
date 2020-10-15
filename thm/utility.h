#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <avr/io.h>
#include <inttypes.h>

#define  MAX(a, b)                    ( ( a > b ) ? a : b )
#define  MIN(a, b)                    ( ( a > b ) ? b : a )

// Clamp a between max and min
#define  CLAMP(a, min_val, max_val)   MIN( MAX( a, min_val ) , max_val)

// returns -1 for a < 0, 1 for a > 0, 0 otherwise 
#define  SIGN(a) ( (a > 0) ? 1 : ( (a < 0) ? -1 : 0 ) )  

uint8_t getArgs(char* arg_str, char fmt, uint8_t num_args, int16_t ret[]);

typedef struct _linear_tween_t {
    int16_t cur_val;
    int16_t step_val;
    int16_t target_val;
    uint8_t num_steps;
    uint8_t step_count;
} linear_tween_t;

int16_t tween_get_next( linear_tween_t* tween );
void tween_init( linear_tween_t* tween, int16_t value );
void tween_set_value( linear_tween_t* tween, int val );
void tween_set_target( linear_tween_t* tween, uint8_t num_steps, int16_t target );
int8_t lsb_set( uint8_t val );

#endif

