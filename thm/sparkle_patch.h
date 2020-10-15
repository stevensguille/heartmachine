/*
 * sparkle_patch.h
 *
 *  Created on: Aug 2, 2010
 *      Author: max
 */

#ifndef SPARKLE_PATCH_H_
#define SPARKLE_PATCH_H_

#include "patches.h"

void sparkle_init( void );
void sparkle_enter( void );
void sparkle_update( int16_t* deltas );
void sparkle_exit( void );

extern thm_patch_t sparkle_patch;


#endif /* SPARKLE_PATCH_H_ */
