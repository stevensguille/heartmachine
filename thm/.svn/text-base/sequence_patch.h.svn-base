/*
 * sequence_patch.h
 *
 *  Created on: Aug 2, 2010
 *      Author: max
 */

#ifndef _SEQUENCE_PATCH_H_
#define _SEQUENCE_PATCH_H_

#include "patches.h"

void sequence_init( void );
void sequence_enter( void );
void sequence_update( int16_t* deltas );
void sequence_exit( void );

extern thm_patch_t sequence_patch;

// called this because it's not obvious to me what deane is doing here
typedef struct _deane_t {
	uint8_t		value[4];
} deane_t;

#endif
