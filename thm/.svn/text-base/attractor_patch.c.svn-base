#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "attractor_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "thm_node.h"

#define  ATT_THRESHOLD_IDX      0

thm_cpatch_t attractor_patch = { 
    .init     = &attractor_init,
    .enter    = &attractor_enter,
    .update   = &attractor_update,
    .activate = &attractor_activate,
    .exit     = &attractor_exit,
    .state    = THM_PATCH_STATE_UNKNOWN /*,
    .progress = 0 */
};

uint16_t attractor_count;
/**
 * Common initialization function. Set the pointers to the correct place.
 */
void attractor_init() {
    attractor_patch.data[ ATT_THRESHOLD_IDX ] = 20;
    // Mode: 0 - off, 1 - one-shot pulse, 2 - continuous pulse
    attractor_patch.data[1] = 0;
}

/**
 *  Function to be called each time this patch begins.
 */
void attractor_enter() {
    printf_P( PSTR("Entering Attractor Patch\n\r") );
    attractor_patch.data[ ATT_THRESHOLD_IDX ] = 20;
    attractor_patch.data[2] = 15; // length of 2 pulses
    
}

/**
 *  Function to call 
 */
void attractor_update( int16_t* deltas ) {
   double phase;
   if(attractor_patch.data[1]) {
      phase = (360.0*attractor_count)/attractor_patch.data[2];

      sol1_pwm_val = (uint16_t)(SOL_PWM_COUNT_MAX * fabs(sin(phase/360*2*3.1415659)));
      printf_P( PSTR("p: %d\n"), sol1_pwm_val, attractor_count);
      
      attractor_count++;
      // If we're at the end of the two-pulse cycle, see if
      // we have to turn the pulse off.
      if(attractor_count > attractor_patch.data[2] && attractor_patch.data[1] == 2) {
         attractor_count = 0;
      } else if (attractor_count > attractor_patch.data[2] && attractor_patch.data[1] == 1) {
         printf_P( PSTR("Done!\n"));
         attractor_patch.data[1] = 0;
         sol1_pwm_val = 0;
      }
   }
}

void attractor_activate( uint16_t activate ) {
   attractor_count = 0;
   attractor_patch.data[1] = activate;
}

//extern void attractor_message( void 
void attractor_exit( void ) {
    printf_P( PSTR("EXIT: Good bye\n\r") );
}

