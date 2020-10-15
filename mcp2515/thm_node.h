/**
 *  Functions for intializing and operating The Heart Machine specific hardware.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#ifndef _TLC_NODE_H_
#define _TLC_NODE_H_

#include <avr/io.h>
#include <inttypes.h>

// TLC Pseudo Chip Select pin
#define CSTLC_PORT      PORTD
#define CSTLC_DDR       DDRD
#define CSTLC_BIT       PORTD7

// mcp2515 CAN Interface chip select
#define CSCAN_PORT      PORTD
#define CSCAN_DDR       DDRD
#define CSCAN_BIT       PORTD6

// For Solenoid Control
#define SOL1_PORT       PORTD
#define SOL1_DDR        DDRD
#define SOL1_BIT        PORTD4

#define SOL2_PORT       PORTD
#define SOL2_DDR        DDRD
#define SOL2_BIT        PORTD5

// Added for Heart Machine Node
#define THM_TLC_CS_UNSELECT()   CSTLC_PORT &= ~_BV(CSTLC_BIT)
#define THM_TLC_CS_SELECT()     CSTLC_PORT |= _BV(CSTLC_BIT)

#define THM_SET_SOL1()          SOL1_PORT |= _BV(SOL1_BIT)
#define THM_SET_SOL2()          SOL2_PORT |= _BV(SOL2_BIT)
#define THM_CLEAR_SOL1()        SOL1_PORT &= ~_BV(SOL1_BIT)
#define THM_CLEAR_SOL2()        SOL2_PORT &= ~_BV(SOL2_BIT)

#define NODE_ID_EEPROM_ADDR         ((uint8_t*)4)
#define ARTERY_ID_EEPROM_ADDR       ((uint8_t*)5)

// These values will be read from eeprom if the IDs have not been
// previously set.
#define THM_NODE_UNKNOWN_ID         255
#define THM_ARTERY_UNKNOWN_ID       255
#define THM_DEFAULT_ARTERY_NUM      0

#define THM_SET_CAN_ID_SUCCESS      0
#define THM_SET_CAN_ID_FAIL         1

#define THM_LIGHTING_NUM_LEDS       16
#define THM_LIGHTING_RED_BANK       16
#define THM_LIGHTING_BLUE_BANK      0
#define THM_LIGHTING_GREEN_BANK     32

// Uncomment to enable the serial console
#define SERIAL_CONSOLE

#define THM_QTOUCH_REPORT_ON        0
#define THM_QTOUCH_REPORT_OFF       1

typedef struct _rgb_t {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgb_t;

typedef struct _node_id_t {
    uint8_t id;
    uint8_t artery;
} node_id_t;

node_id_t node_id;

#ifdef SERIAL_CONSOLE
uint8_t thm_qtouch_report;
#endif

void thm_init( void );
void thm_setSolenoidDutyCycle( void );

void thm_setLightingRGB( uint8_t led_num , rgb_t* val );
void thm_setLightingAll( rgb_t* val );
void thm_setLightingColumn( uint8_t col_num, rgb_t* val );
void thm_setLightingRow( uint8_t row_num, rgb_t* val );

void thm_readNodeID( void );
void thm_setNodeID( uint8_t id_num, uint8_t artery_num );

uint8_t thm_setCANFilter( node_id_t* node_id );

void thm_setRXIndicator( void );
void thm_clearRXIndicator( void );
void thm_setTXIndicator( void );
void thm_clearTXIndicator( void );

#endif

