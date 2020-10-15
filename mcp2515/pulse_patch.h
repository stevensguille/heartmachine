#ifndef _PULSE_PATCH_H_
#define _PULSE_PATCH_H_

#include "patches.h"

void pulse_init( void );
void pulse_enter( void );
void pulse_update( int16_t* deltas );
void pulse_exit( void );

extern thm_patch_t pulse_patch;

#endif 
