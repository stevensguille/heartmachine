/**
 *  Base struct and functions for creating and manipulating node patches.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "patches.h"


uint16_t getCurrentPatchData( uint8_t data_index ) {
    return current_patch->data[data_index];
}

void setCurrentPatchData( uint8_t data_index, uint16_t val ) {
    current_patch->data[data_index] = val;
}

/**
 *  Read a data value in patch.
 *
 *  Note: It is up to the patch implementation to determine
 *  what the data represents. 
 */
uint16_t getPatchData( thm_patch_t* patch, uint8_t data_index ) {
    return patch->data[data_index];
}

/**
 *  Set a data value in patch.
 *
 *  Note: It is up to the patch implementation to determine
 *  what the data represents. 
 */
void setPatchData( thm_patch_t* patch, uint8_t data_index, uint16_t val ) {
    patch->data[data_index] = val;
}


