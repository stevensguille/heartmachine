#ifndef _ATTRACTOR_PATCH_H_
#define _ATTRACTOR_PATCH_H_

#include "patches.h"

void attractor_init( void );
void attractor_enter( void );
void attractor_update( int16_t* deltas );
void attractor_activate( uint16_t activate );
void attractor_exit( void );

extern thm_cpatch_t attractor_patch;
#endif 
