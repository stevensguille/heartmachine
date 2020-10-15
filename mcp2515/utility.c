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

