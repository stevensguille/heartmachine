/*
 * sine_patch.h
 *
 *  Created on: Aug 2, 2010
 *      Author: max
 */

#ifndef SINE_PATCH_H_
#define SINE_PATCH_H_

#define SINE_PATCH_SPEED_IDX          0 
#define SINE_PATCH_COLOR_IDX          1
#define SINE_PATCH_SHIFT_SPEED_IDX    2 
#define SINE_PATCH_PHASE_IDX          3 

#include "patches.h"

void sine_init( void );
void sine_enter( void );
void sine_update( int16_t* deltas );
void sine_exit( void );

extern thm_patch_t sine_patch;


#endif /* SINE_PATCH_H_ */
