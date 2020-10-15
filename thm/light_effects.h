/**
 *  A small set of functions to perform some lighting effects on
 *  the TLC5940
 *
 *  Each Tlc5940 has 16*12-bits = 24 bytes of data.
 *
 */
#ifndef _LIGHTING_EFFECTS_H_
#define _LIGHTING_EFFECTS_H_

#include <inttypes.h>
#include <thm_node.h>
#include "Tlc5940.h"

#define CYCLE_TYPE_LOOP         0
#define CYCLE_TYPE_PINGPONG     1
#define CYCLE_TYPE_SINGLE       2
#define COLOR_TABLE_LENGTH 		8
#define MAX_COLOR				4096		// we have 12-bit color

typedef struct _rgb_t {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgb_t;

typedef struct _hsv_t {
	double	h;
	double	s;
	double 	v;
} hsv_t;

uint8_t tlc_buf1[ NUM_TLCS * 24 ];
uint8_t tlc_buf2[ NUM_TLCS * 24 ];

int8_t animateFrames( uint16_t** frames_data, uint8_t num_frames,
        uint8_t steps_per_frame, uint8_t cycle_type );

int8_t fadeTo( uint8_t* targ_data, uint8_t steps );
int8_t fadeTo_P( uint8_t* targ_data, uint8_t steps );

void setLightCol( uint8_t col_num, rgb_t* val );
void setLightRow( uint8_t row_num, rgb_t* val );
void setLightRect( uint8_t x, uint8_t y, uint8_t w, uint8_t h, rgb_t* val );
//void setLightCol_buf( uint8_t* buf, uint8_t col_num, rgb_t* val );
//void setLightRow_buf( uint8_t* buf, uint8_t row_num, rgb_t* val );
void setLightRect_buf( uint8_t* buf, uint8_t x, uint8_t y,
        uint8_t w, uint8_t h, rgb_t* val );

void setLightLED( uint8_t led_num , rgb_t* val );
void setLightRC(uint8_t row, uint8_t col, rgb_t *c);
void setLightAll( rgb_t* val );

void glowAll( uint16_t step, rgb_t* max );
void dimAll( uint16_t step, rgb_t* min );

void setLightLED_buf( uint8_t* buf, uint8_t led_num , rgb_t* val );
void setLightRC_buf( uint8_t* buf, uint8_t row, uint8_t col, rgb_t *c);
void setLightAll_buf( uint8_t* buf, rgb_t* val );

void getLightLED(uint8_t led_num, rgb_t *c);
void getLightLED_buf( uint8_t* buf, uint8_t led_num, rgb_t *c);

void getLightRC(uint8_t row, uint8_t col, rgb_t *c);

hsv_t* RGBtoHSV (uint16_t R, uint16_t G, uint16_t B, hsv_t **HSV);
rgb_t *HSVtoRGB (float h, float s, float v, rgb_t **RGB);

int16_t max(int16_t a, int16_t b);
int16_t min(int16_t a, int16_t b);

rgb_t *setColorBrightness(rgb_t **RGB, int b);
rgb_t *getColorFromTable(int i, rgb_t **RGB);

extern struct _rgb_t colorTable[];

#endif
