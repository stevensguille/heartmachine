/*
 * solenoid_patch.c
 *
 * Parameters:
 * 0 - number of cycles between heartbeat thumps. Defaults to 300, 
 * which is roughly once every ten seconds. Smaller numbers = faster.
 *
 * 1 - set to something non-zero to turn on the ambient heartbeat, whose
 * pulse is controlled by parameter 0.
 *
 * Activation scheme:
 * 0 - OFF
 * 1 - Solenoid 1, 10s (main)
 * 2 - Solenoid 2, 10s (side)
 * 3 - Both, 10s
 * 4 - Single heartbeat, sol2-sol1 (sol1 burst slightly bigger)
 * 5 - Reverse heartbeat, sol1-sol2 (sol2 burst slightly bigger)
 * 6 - "WE WILL ROCK YOU"
 */

#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "solenoid_patch.h"
#include "mstimer.h"
#include "utility.h"
#include "thm_node.h"

/*
 * Solenoid patch states, for its own internal use.
 */
#define SOLENOID_PATCH_IDLE 0
#define SOLENOID_PATCH_FIRE 1
#define SOLENOID_PATCH_RUNNING 2

/*
 * This is arcane, but compact and flexy. The top bit is which solenoid,
 * 0 or 1. The second-from-top bit is which state, 0 (off) or 1 (on).
 * The rest of the bits are the time. So you can craft an array of
 * uint16_t's that will represent a sequence of ons and offs.
 *
 * The patch terminates when it finds a value of 0. At this point it
 * turns off both solenoids, for safety's sake. The last non-0 command
 * should involve turning off the last running solenoid anyway.
 */
#define EVENT_SOLENOID 0x8000
#define EVENT_SOLENOID1 0x0000
#define EVENT_SOLENOID2 EVENT_SOLENOID
#define EVENT_ON 0x4000
#define EVENT_OFF 0x0000
#define EVENT_TIME 0x3fff

// Ambient heartbeat sequence
uint16_t heartbeat_sequence[] PROGMEM = {
   EVENT_SOLENOID2 | EVENT_ON,   // Turn solenoid 2 on at time 0
   EVENT_SOLENOID2 | EVENT_OFF | 4, // Turn the solenoid off at time 4
   EVENT_SOLENOID1 | EVENT_ON  | 9, // Solenoid 1 on at time 9
   EVENT_SOLENOID1 | EVENT_OFF | 17, // Solenoid off at time 17
   0
};

// reverse heartbeat sequence
uint16_t reverse_heartbeat_sequence[] PROGMEM = {
   EVENT_SOLENOID1 | EVENT_ON,   // Turn solenoid 2 on at time 0
   EVENT_SOLENOID1 | EVENT_OFF | 4, // Turn the solenoid off at time 4
   EVENT_SOLENOID2 | EVENT_ON  | 9, // Solenoid 1 on at time 9
   EVENT_SOLENOID2 | EVENT_OFF | 17, // Solenoid off at time 17
   0
};

// "Turn on both solenoids" sequence
uint16_t fire_both_sequence[] PROGMEM = {
   EVENT_SOLENOID1 | EVENT_ON,
   EVENT_SOLENOID2 | EVENT_ON,
   EVENT_SOLENOID1 | EVENT_OFF | MAX_SOLENOID_DURATION,
   EVENT_SOLENOID2 | EVENT_OFF | MAX_SOLENOID_DURATION,
   0
};

// "Turn on solenoid 1" sequence
uint16_t fire_one_sequence[] PROGMEM = {
   EVENT_SOLENOID1 | EVENT_ON,
   EVENT_SOLENOID1 | EVENT_OFF | MAX_SOLENOID_DURATION,
   0
};

// "Turn on solenoid 2" sequence
uint16_t fire_two_sequence[] PROGMEM = {
   EVENT_SOLENOID2 | EVENT_ON,
   EVENT_SOLENOID2 | EVENT_OFF | MAX_SOLENOID_DURATION,
   0
};

// "We Will Rock You" sequence
uint16_t rock_you_sequence[] PROGMEM = {
   EVENT_SOLENOID1 | EVENT_ON,
   EVENT_SOLENOID1 | EVENT_OFF | 7,
   EVENT_SOLENOID1 | EVENT_ON  | 10,
   EVENT_SOLENOID1 | EVENT_OFF | 17,
   EVENT_SOLENOID2 | EVENT_ON  | 20,
   EVENT_SOLENOID2 | EVENT_OFF | 32,
   0
};

thm_cpatch_t solenoid_cpatch = {
    .init     = &solenoid_init,
    .enter    = &solenoid_enter,
    .update   = &solenoid_update,
    .activate = &solenoid_activate,
    .exit     = &solenoid_exit,
    .id       = CPATCH_ID_SOLENOID,
};

uint16_t solenoid_count = 0;
uint16_t *firing_sequence;
uint8_t solenoid_state = SOLENOID_PATCH_IDLE;
/**
 * Common initialization function. Set the pointers to the correct place.
 */
void solenoid_init() {

	cpatch_data[0] = 300; // steps to wait between pulses
   cpatch_data[1] = 0; // No heartbeat by default
}

/**
 *  Function to be called each time this patch begins.
 */
void solenoid_enter() {

	printf_P( PSTR("Entering Solenoid Patch\n\r") );

   mstimer_setLength( &cpatch_timer, 30 );
   mstimer_reset( &cpatch_timer );

	solenoid_count = 0;
   solenoid_state = SOLENOID_PATCH_IDLE;
   cpatch_data[0] = 300; // steps to wait between pulses
   cpatch_data[1] = 0; // No heartbeat by default
	
   THM_CLEAR_SOL1();
   THM_CLEAR_SOL2();
}

/**
 *  Function to call
 */
void solenoid_update( int16_t* deltas ) {
   static uint16_t sequence_step = 0;   
//   printf_P( PSTR("%d %d\n"),solenoid_state, cpatch_data[0]);
   
   switch(solenoid_state) {
      // If nothing else is happening, wait until we fire the next heartbeat.
      case SOLENOID_PATCH_IDLE:

      if(cpatch_data[1] == 0)
      {
         solenoid_count = cpatch_data[0];
         return;
      } else if(solenoid_count < cpatch_data[0]) {
         solenoid_count++;
         return;
      }
      
      solenoid_count = 0;
      firing_sequence = heartbeat_sequence;
      
      // Start firing a sequence.
      case SOLENOID_PATCH_FIRE:
      sequence_step = 0;
      solenoid_state = SOLENOID_PATCH_RUNNING;
      // Continue firing the current sequence.
      case SOLENOID_PATCH_RUNNING:
      while(pgm_read_word(firing_sequence) != 0 && (pgm_read_word(firing_sequence) & EVENT_TIME) == sequence_step) {
         if(pgm_read_word(firing_sequence) & EVENT_SOLENOID) {
            if(pgm_read_word(firing_sequence) & EVENT_ON) {
               THM_SET_SOL2();
            } else {
               THM_CLEAR_SOL2();
            }
         } else {
            if(pgm_read_word(firing_sequence) & EVENT_ON) {
               THM_SET_SOL1();
            } else {
               THM_CLEAR_SOL1();
            }
         }
         firing_sequence++;
      }
      if(pgm_read_word(firing_sequence) == 0) {
         // Turn off all the solenoids: if you activate one and then the other
         // without deactivating the first, it can be stuck on otherwise.
         THM_CLEAR_SOL1();
         THM_CLEAR_SOL2();
         
         solenoid_state = SOLENOID_PATCH_IDLE;
      }
      sequence_step++;
      break;
   }
}

/**
 *  Activate the patch's behaviour
 */
void solenoid_activate(uint16_t* activate) {
   printf_P( PSTR("Activate: %d\n"), activate);
   if(activate[0]) {
      switch(activate[0]) {
         case 1:
         firing_sequence = fire_one_sequence;
         break;
         case 2:
         firing_sequence = fire_two_sequence;
         break;
         case 3:
         firing_sequence = fire_both_sequence;
         break;
         case 4:
         firing_sequence = heartbeat_sequence;
         break;
         case 5:
         firing_sequence = reverse_heartbeat_sequence;
         break;
         case 6:
         firing_sequence = rock_you_sequence;
         break;
      }  
         
      solenoid_state = SOLENOID_PATCH_FIRE;
   } else {
      THM_CLEAR_SOL1();
      THM_CLEAR_SOL2();
      solenoid_state = SOLENOID_PATCH_IDLE;
   }
}

//extern void solenoid_message( void
void solenoid_exit( void ) {
    THM_CLEAR_SOL1();
    THM_CLEAR_SOL2();
}

