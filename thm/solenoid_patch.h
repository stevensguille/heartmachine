/*
 * solenoid_patch.h
 *
 *  Created on: Aug 4, 2010
 *      Author: max
 */

#ifndef SOLENOID_PATCH_H_
#define SOLENOID_PATCH_H_

#include "patches.h"

void solenoid_init( void );
void solenoid_enter( void );
void solenoid_update( int16_t* deltas );
void solenoid_activate( uint16_t* activate );
void solenoid_deactivate( void );
void solenoid_exit( void );

extern thm_cpatch_t solenoid_cpatch;

#define MAX_SOLENOID_DURATION	300

#endif /* SOLENOID_PATCH_H_ */
