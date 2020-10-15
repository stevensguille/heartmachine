/*
 * sparkle_patch.c - The Sparkle Pony
 *
 * Parameters:
 * 0 - how many calls between adding blips (the bright core that starts a sparkle)
 *       - defaults to 25
 *       - hardcoded to 3 when touched
 * 1 - how many calls between sparkle frames
 *       - defaults to 3
 *       - hardcoded to 1 when touched
 * 2 - what number to divide a LED's own brightness by. Larger means sparkles will
 *     die more quickly. 1 means they will never die.
 *       - defaults to 12
 *       - hardcoded to 7 when touched
 * 3 - what number to divide a LED's neighbours by. Since there are 4, if it's
 *     larger than 4, the led will probably go white. Making this larger will make
 *     sparkles fade faster when they spread.
 *       - defaults to 9
 *       - hardcoded to 5 when touched
 *
 * Other interesting combinations: (25,3,4,12) gives smooth looking bursts instead of
 * sparkly ones.
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

#include "patches.h"
#include "patch_id.h"
#include "sparkle_patch.h"
#include "mstimer.h"
#include "Tlc5940.h"
#include "utility.h"
#include "thm_node.h"
#include "qtouch.h"
#include "light_effects.h"

thm_patch_t sparkle_patch = {
    .init     = &sparkle_init,
    .enter    = &sparkle_enter,
    .update   = &sparkle_update,
    .exit     = &sparkle_exit,
    .id = PATCH_ID_VECTOR,
};

rgb_t sparkle_buffer[4][4];
uint16_t sparkle_timer;
uint16_t blip_timer;
/**
 * Common initialization function. Set the pointers to the correct place.
 */
void sparkle_init() {   
}

/**
 *  Function to be called each time this patch begins.
 */
void sparkle_enter() {
   mstimer_setLength( &patch_timer, 30 );
   mstimer_reset( &patch_timer );

//	tlc_setAll( 0 );

	patch_data[0] = 25;	// blip speed
   patch_data[1] = 3; // sparkle speed
   patch_data[2] = 12; // center fade
   patch_data[3] = 9; // neighbour fade
   sparkle_timer = 0;
   blip_timer = 0;
}

/**
 *  Function to call
 */
void sparkle_update( int16_t* deltas ) {
   
   uint8_t to_activate;
   int8_t i, j;
   static uint8_t triggered = 0;
   static uint16_t old_params[4];
   float h, v;
   
   rgb_t blip, *blip_p, updating, *updating_p, examining, *examining_p;

   blip_p = &blip;
   updating_p = &updating;
   examining_p = &examining;

   /*
    * This is the part that responds to touching the center
    * capacitor.
    */
   if(triggered == 0 && abs(deltas[0]) > 20) {
      old_params[0] = patch_data[0];
      old_params[1] = patch_data[1];
      old_params[2] = patch_data[2];
      old_params[3] = patch_data[3];
      
      patch_data[0] = 3;
      patch_data[1] = 1;
      patch_data[2] = 7;
      patch_data[3] = 5;
      
      triggered = 1;
   } else if (triggered == 1 && abs(deltas[0]) < 20) {
      triggered = 0;
      patch_data[0] = old_params[0];
      patch_data[1] = old_params[1];
      patch_data[2] = old_params[2];
      patch_data[3] = old_params[3];
   }
   
	// slow it down
	if (sparkle_timer < patch_data[1] && blip_timer < patch_data[0]) {
      blip_timer++;
		sparkle_timer++;
		return;
	}
	
	if (sparkle_timer >= patch_data[1]) { 
      // get a copy of the current LED state, I hope...
      for(i = 0; i < 4; i++) {
         for(j = 0; j < 4; j++) {
            updating_p = &(sparkle_buffer[i][j]);
            getLightRC(i,j,updating_p);
         }
      }

      updating_p = &updating;
      
      for(i = 0; i < 4; i++) {
         for(j = 0; j < 4; j++) {
            updating.red = sparkle_buffer[i][j].red / patch_data[2];
            updating.green = sparkle_buffer[i][j].red / patch_data[2];
            updating.blue = sparkle_buffer[i][j].red / patch_data[2];
        
            if(i > 0) {
            examining_p = &sparkle_buffer[(i+3)%4][j];
            updating.red += examining_p->red / patch_data[3]; 
            updating.green += examining_p->green / patch_data[3]; 
            updating.blue += examining_p->blue / patch_data[3]; 
            }
            
            if(i < 4) {
            examining_p = &sparkle_buffer[(i+1)%4][j];
//            getLightRC((i+1)%4,j,&examining_p);
            updating.red += examining_p->red / patch_data[3]; 
            updating.green += examining_p->green / patch_data[3]; 
            updating.blue += examining_p->blue / patch_data[3]; 
            }
            
            if(j > 0) {
            examining_p = &sparkle_buffer[i][(j+3)%4];
//            getLightRC(i,(j+3)%4,&examining_p);
            updating.red += examining_p->red / patch_data[3]; 
            updating.green += examining_p->green / patch_data[3]; 
            updating.blue += examining_p->blue / patch_data[3]; 
            }
            
            if(j < 4) {
            examining_p = &sparkle_buffer[i][(j+1)%4];
//            getLightRC(i,(j+1)%4,&examining_p);
            updating.red += examining_p->red / patch_data[3]; 
            updating.green += examining_p->green / patch_data[3]; 
            updating.blue += examining_p->blue / patch_data[3]; 
            }
            
            setLightRC(i,j,updating_p);
//            
               }
            }
//         }
//      }
//      
//      printf_P(PSTR("buffer value: %d %d %d\n"), buffer[1][1].red, buffer[1][1].green, buffer[1][1].blue);
//      
//      for(i = 0; i < 4; i++) {
//         for(j = 0; j < 4; j++) {
//            setLightRC(i,j,&sparkle_buffer[4*i+j]);
//         }
//      }
      
      sparkle_timer = 0;
   }

   if(blip_timer >= patch_data[0]) {
      to_activate = rand() & 0x0f;
      
      h = (triggered)?0.33:((float)rand())/RAND_MAX;
      v = (triggered)?1.0:((float)rand())/(RAND_MAX*2)+.5;

      HSVtoRGB(h, 1.0, v, &blip_p);
      setLightLED(to_activate, &blip);
      blip_timer = 0;
   }

	THM_TLC_CS_SELECT();
   tlc_update();
   THM_TLC_CS_UNSELECT();

}

//extern void sine_message( void
void sparkle_exit( void ) {
	tlc_setAll( 0 );

   THM_TLC_CS_SELECT();
   tlc_update();
   THM_TLC_CS_UNSELECT();
}

