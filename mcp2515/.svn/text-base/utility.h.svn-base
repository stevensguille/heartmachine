#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <avr/io.h>
#include <inttypes.h>

#define  MAX(a, b)                    ( ( a > b ) ? a : b )
#define  MIN(a, b)                    ( ( a > b ) ? b : a )

// Clamp a between max and min
#define  CLAMP(a, min_val, max_val)   MIN( MAX( a, min_val ) , max_val)

uint8_t getArgs(char* arg_str, char fmt, uint8_t num_args, int16_t ret[]);

#endif

