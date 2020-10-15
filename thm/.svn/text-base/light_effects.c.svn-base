#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "Tlc5940.h"
#include "utility.h"
#include "light_effects.h"

#define DATA_LEN (NUM_TLCS*16)

struct _rgb_t colorTable[] = {
		{MAX_COLOR,0,0}, {0,MAX_COLOR,0}, {0,0,MAX_COLOR},
		{MAX_COLOR,MAX_COLOR,0}, {MAX_COLOR,0,MAX_COLOR}, {MAX_COLOR,MAX_COLOR/2,0},
		{0,MAX_COLOR*.75,MAX_COLOR}, {MAX_COLOR,MAX_COLOR,255}
};

void setLightFromChannelData( uint16_t* frame_data ) {
   int chan_num; 

    for ( chan_num = 0; chan_num < DATA_LEN; chan_num++ ) {
            tlc_set( chan_num, frame_data[chan_num] );
    }
}

/**
 *  Animate between frames. Next frame when animateFrames is called
 *  'steps_per_frame' times.
 *
 *  Pass frame_data = NULL to continue animation
 */
int8_t animateFrames( uint16_t** frames_data, uint8_t num_frames,
        uint8_t steps_per_frame, uint8_t cycle_type ) {
    static uint8_t frame_index;
    static uint8_t frame_index_step;
    static uint8_t step_index;
    uint16_t** frames_data_p = NULL;

    // We start a new Animation here.
    if ( frames_data != NULL ) {
        frame_index      = 0;
        frame_index_step = 1;
        step_index       = 0;
        frames_data_p    = frames_data;
        
        // Set Initial Frame
        setLightFromChannelData( frames_data_p[ frame_index ] );
    } else if ( frame_index != -1 ) {
        // We've called this function 'steps_per_frame' times so step
        // to the next frame
        if ( ++step_index > steps_per_frame ) {
            step_index = 0;
            frame_index = frame_index + frame_index_step;

            // Decide what to do when we've finished iterating through the
            // frames. We check if frame_index < 0 for PINGPONG
            if ( frame_index > num_frames || frame_index < 0 ) {
                switch ( cycle_type ) { 
                    case CYCLE_TYPE_SINGLE:
                        frame_index = -1;
                        break;
                    case CYCLE_TYPE_LOOP:
                        frame_index = 0;
                        break;
                    case CYCLE_TYPE_PINGPONG:
                        frame_index_step = -1;
                        break;
                }
            }
            setLightFromChannelData( frames_data_p[ frame_index ] );
        }
    }

    return frame_index;
}

/**
 *  Fade Animate between a number of frames.
 *
 * 
 */
int8_t fadeFrames( uint16_t** frame_data, uint8_t num_frames,
        uint8_t steps_per_frame, uint8_t cycle_type) {
//    static uint8_t frame_index;
//    static uint8_t frame_index_step;
//    static int8_t  step_index;

    return 0;
}

/**
 *  A generator for a linear interpolation between the current value
 *  in tlc_GSData and the data in 'targ_data' ( which has length
 *  uint8_t[ 16*NUM_TLCS ].
 *
 *  Returns the current_step index, -1 when fade is complete.
 *
 *  Pass targ_date = NULL, to step through to the next frame.
 */
int8_t fadeTo( uint8_t* targ_data, uint8_t steps ) {
    int chan_num;
    int16_t delta;
    int16_t tmp;
    uint8_t retval;

    static int16_t step_val[DATA_LEN];
    static uint8_t step_count;

    // Start a new Fade.
    if ( targ_data != NULL ) {
        for ( chan_num = 0; chan_num < DATA_LEN; chan_num++ ) {
            tmp                  = tlc_get( chan_num );
            delta                = tlc_get_buf(targ_data, chan_num) - tmp;
            step_val[ chan_num ] = delta / steps;
            step_val[ chan_num ] = SIGN( step_val[ chan_num ] )
                * MAX( 1, abs( step_val[ chan_num ] ) );
            // printf_P( PSTR("step %d %u:%d\n\r"), delta, chan_num, step_val[ chan_num ]);

            // set the current values to what's in the buffer
            tlc_set( chan_num, tmp );
        }
        step_count = 0;
        retval = step_count;
    } else {
        if ( step_count >= steps ) {
            for ( chan_num = 0; chan_num < DATA_LEN; chan_num++ ) {
                tlc_set( chan_num, tlc_get(chan_num) );
            }
            retval = -1;
        } else {
            for ( chan_num = 0; chan_num < DATA_LEN; chan_num++ ) {
                tmp = tlc_get( chan_num ) + step_val[ chan_num ];
                //printf_P( PSTR("%u:%u\n\r"), chan_num, tmp);
                tlc_set( chan_num, CLAMP( tmp, 0, MAX_COLOR-1) );
            }
            retval = ++step_count;
        }
    }

    return retval;
}


void setLightLED( uint8_t led_num, rgb_t* val ) { 
    setLightLED_buf( tlc_GSData, led_num, val );
}

/**
 *  Set the LED to an RGB value.  
 */
void setLightLED_buf( uint8_t* buf, uint8_t led_num, rgb_t* val ) {
	if (led_num >= THM_LIGHTING_NUM_LEDS) {
		return;
	}

    tlc_set(led_num+THM_LIGHTING_RED_BANK,   val->red);
    tlc_set(led_num+THM_LIGHTING_GREEN_BANK, val->green);
    tlc_set(led_num+THM_LIGHTING_BLUE_BANK,  val->blue);
}

void setLightRC(uint8_t row, uint8_t col, rgb_t *RGB) {
	//printf_P( PSTR("%d %d: %d,%d,%d"), row, col, RGB->red, RGB->green, RGB->blue);
	setLightLED((row * THM_LIGHTING_NUM_ROW)+col, RGB);
}

void getLightLED( uint8_t led_num, rgb_t* val ) {
    getLightLED_buf( tlc_GSData, led_num, val );
}

/*
 * get the LED values back
 */
void getLightLED_buf( uint8_t* buf, uint8_t led_num, rgb_t  *val ) {
    val->red = tlc_get_buf( buf, led_num+THM_LIGHTING_RED_BANK);
    val->green = tlc_get_buf( buf, led_num+THM_LIGHTING_GREEN_BANK);
    val->blue = tlc_get_buf( buf, led_num+THM_LIGHTING_BLUE_BANK);
}

void getLightRC(uint8_t row, uint8_t col, rgb_t *RGB) {
	getLightLED((row * THM_LIGHTING_NUM_ROW)+col, RGB);
}

void setLightRect( uint8_t x, uint8_t y,
        uint8_t w, uint8_t h, rgb_t* val ) {
    setLightRect_buf( tlc_GSData, x, y, w, h, val );
}

/**
 *  Set a rectangle to an rgb color.
 */
void setLightRect_buf( uint8_t* buf, uint8_t x, uint8_t y,
        uint8_t w, uint8_t h, rgb_t* val ) {

    uint8_t row, col;

    if ( x >= THM_LIGHTING_NUM_ROW || y >= THM_LIGHTING_NUM_COL
            || w == 0 || h == 0 )
        return;

    for ( col=y; col < y+h; col++ ) {
        for ( row=x; row < x+w; row++ ) {
            setLightLED_buf( buf, thm_getLEDNum( row, col ), val );
        }
    }
}


/**
 *  Set a single row of LEDs to 'val'
 */
void setLightRow( uint8_t row_num, rgb_t* val ) {
    setLightRect( row_num, 0, 1, THM_LIGHTING_NUM_COL, val);
}


/**
 *  Set a single column of LEDs to 'val'
 */ 
void setLightCol( uint8_t col_num, rgb_t* val ) {
    setLightRect( 0, col_num, THM_LIGHTING_NUM_ROW, 1, val);
}   

void setLightAll( rgb_t* val ) {
    setLightAll_buf( tlc_GSData, val );
}

/** 
 *  Very Unoptimized setting of an RGB value to all RGBs
 */ 
void setLightAll_buf( uint8_t* buf, rgb_t* val ) {
    int led_num;
    
    for ( led_num=0; led_num < THM_LIGHTING_NUM_LEDS; led_num++ ) {
        setLightLED_buf( buf, led_num, val );
    }
}   


/**
 *
 */ 
void glowAll( uint16_t dim_step, rgb_t* max ) {
    int led_num;
    rgb_t c;

    for( led_num=0; led_num < THM_LIGHTING_NUM_LEDS; led_num++ ) {
        getLightLED( led_num, &c );
        c.red   = MIN( c.red+dim_step, max->red );
        c.green = MIN( c.green+dim_step, max->green );
        c.blue  = MIN( c.blue+dim_step, max->blue );
    }
 }

/**
 *  Reduces the level of every light channel to a minimum color.
 */
void dimAll( uint16_t dim_step, rgb_t* min ) {
    int led_num;
    rgb_t c;

    for( led_num=0; led_num < THM_LIGHTING_NUM_LEDS; led_num++ ) {
        getLightLED( led_num, &c );
        c.red   = MAX( c.red-dim_step, min->red );
        c.green = MAX( c.green-dim_step, min->green );
        c.blue  = MAX( c.blue-dim_step, min->blue );
    }
}

/*
 * convert an RGB value to an HSV value
 */
// PORTED
hsv_t* RGBtoHSV (uint16_t R, uint16_t G, uint16_t B, hsv_t **HSV) {
	// R,G,B in [0,255]
	float H = 0, S = 0, V = 0;
	float cMax = MAX_COLOR;
	uint16_t cHi = MAX(R, MAX(G,B));	// highest color value
	uint16_t cLo = MIN(R, MIN(G,B));	// lowest color value
	uint16_t cRng = cHi - cLo;				    // color range

	// compute value V
	V = cHi / cMax;

	// compute saturation S
	if (cHi > 0)
		S = (float) cRng / cHi;

	// compute hue H
	if (cRng > 0) {	// hue is defined only for color pixels
		float rr = (float)(cHi - R) / cRng;
		float gg = (float)(cHi - G) / cRng;
		float bb = (float)(cHi - B) / cRng;
		float hh;
		if (R == cHi)                      // r is highest color value
			hh = bb - gg;
		else if (G == cHi)                 // g is highest color value
			hh = rr - bb + 2.0f;
		else                               // b is highest color value
			hh = gg - rr + 4.0f;
		if (hh < 0)
			hh= hh + 6;
		H = hh / 6;
	}


	(*HSV)->h = H; (*HSV)->s = S; (*HSV)->v = V;
	return *HSV;
}

// PORTED
rgb_t *HSVtoRGB (float h, float s, float v, rgb_t **RGB) {

	// h,s,v in [0,1]
	float rr = 0, gg = 0, bb = 0;
	float hh = fmod((6 * h), 6);
	uint16_t   c1 = (uint16_t) hh;
	float c2 = hh - c1;
	float x = (1 - s) * v;
	float y = (1 - (s * c2)) * v;
	float z = (1 - (s * (1 - c2))) * v;
	switch (c1) {
		case 0: rr=v; gg=z; bb=x; break;
		case 1: rr=y; gg=v; bb=x; break;
		case 2: rr=x; gg=v; bb=z; break;
		case 3: rr=x; gg=y; bb=v; break;
		case 4: rr=z; gg=x; bb=v; break;
		case 5: rr=v; gg=x; bb=y; break;
	}

	uint16_t N = MAX_COLOR;

	(*RGB)->red = MIN(round(rr*N),N-1);
	(*RGB)->green = MIN(round(gg*N),N-1);
	(*RGB)->blue = MIN(round(bb*N),N-1);
	return *RGB;
}

// PORTED
// given a colour, set its brightness from 0-255.
// DOES NOT create a new color.  It sets the color in place
rgb_t *setColorBrightness(rgb_t **RGB, int b) {

	// local copy for us to work on
	static hsv_t hsv;
	hsv_t *myhsv = &hsv;

	RGBtoHSV((*RGB)->red, (*RGB)->green, (*RGB)->blue, &myhsv);
	float bf = (float) b;
	hsv.v = bf / MAX_COLOR; // set the brightness
	HSVtoRGB(hsv.h, hsv.s, hsv.v, RGB);
	return *RGB;
}

// PORTED
// given a color index, get the color from the colorTable
rgb_t *getColorFromTable(int i, rgb_t **RGB) {

	// check for valid color index
	if (i >= COLOR_TABLE_LENGTH) {
		i = COLOR_TABLE_LENGTH-1; // use the last color
	}

	// if color index is 0, create a random color
	if (i == 0) {
		(*RGB)->red = rand() % MAX_COLOR;
		(*RGB)->green = rand() % MAX_COLOR;
		(*RGB)->blue = rand() % MAX_COLOR;
	} else {
		(*RGB)->red = colorTable[i-1].red;
		(*RGB)->green = colorTable[i-1].green;
		(*RGB)->blue = colorTable[i-1].blue;
	}
	return *RGB;
}


