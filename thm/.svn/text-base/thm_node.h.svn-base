/**
 *  Functions for intializing and operating The Heart Machine specific hardware.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#ifndef _TLC_NODE_H_
#define _TLC_NODE_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <inttypes.h>
#include <mstimer.h>
#include "patch_id.h"

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
#define THM_TOGGLE_SOL1()       SOL1_PORT ^= _BV(SOL1_BIT)
#define THM_TOGGLE_SOL2()       SOL2_PORT ^= _BV(SOL2_BIT)
#define THM_ONLY_SOL1()         THM_SET_SOL1(); THM_CLEAR_SOL2()
#define THM_ONLY_SOL2()         THM_SET_SOL2(); THM_CLEAR_SOL1()
#define THM_SET_BOTH_SOL()      SOL1_PORT |= _BV(SOL1_BIT) | _BV(SOL2_BIT) 
#define THM_CLEAR_BOTH_SOL()    SOL1_PORT &= ~( _BV(SOL1_BIT) | _BV(SOL2_BIT) )

#define THM_SOL_NONE            0
#define THM_SOL1                1
#define THM_SOL2                2
#define THM_SOL_BOTH            3

#define NODE_ID_EEPROM_ADDR         ((uint8_t*)4)
#define ARTERY_ID_EEPROM_ADDR       ((uint8_t*)5)
#define TOUCH_THRESHOLD_EEPROM_ADDR ((uint8_t*)6)
#define DEFAULT_PATCH_EEPROM_ADDR   ((uint8_t*)7)
#define DEFAULT_CPATCH_EEPROM_ADDR  ((uint8_t*)8)

// These values will be read from eeprom if the IDs have not been
// previously set.
#define THM_NODE_UNKNOWN_ID             255
#define THM_ARTERY_UNKNOWN_ID           255
#define TOUCH_THRESHOLD_UNCALIBRATED    255
#define DEFAULT_PATCH_UNSET             255
#define THM_DEFAULT_ARTERY_NUM          0

#define DEFAULT_PATCH_ID                PATCH_ID_VECTOR
#define DEFAULT_CPATCH_ID               CPATCH_ID_HEARTBEAT

#define THM_SET_CAN_ID_SUCCESS          0
#define THM_SET_CAN_ID_FAIL             1

#define THM_LIGHTING_NUM_LEDS       16
#define THM_LIGHTING_RED_BANK       16
#define THM_LIGHTING_BLUE_BANK      0
#define THM_LIGHTING_GREEN_BANK     32
#define THM_LIGHTING_NUM_ROW        4
#define THM_LIGHTING_NUM_COL        4

// These are used to initialize the mcp2515.
// Lowest three bits in the RXM High Byte
//#define THM_MCP2515_ARTERY_ID_MASK_H        0xfc
#define THM_MCP2515_ARTERY_ID_MASK_H        0xff

// Highest three bits in the RXM Low Byte 
//#define THM_MCP2515_ARTERY_ID_MASK_L        0x00
#define THM_MCP2515_ARTERY_ID_MASK_L        0xe0

// We ensure that all artery bytes are 0
//#define THM_MCP2515_NODE_ID_MASK_H          0xff
#define THM_MCP2515_NODE_ID_MASK_H          0xff

// Highest three bits in the RXM Low Byte
//#define THM_MCP2515_NODE_ID_MASK_L          0xe0
#define THM_MCP2515_NODE_ID_MASK_L          0xe0

// Uncomment to enable the serial console
#define SERIAL_CONSOLE

#define THM_QTOUCH_REPORT_ON                0
#define THM_QTOUCH_REPORT_OFF               1

#define THM_QTOUCH_DEFAULT_THRESHOLD        30

#define SOL_PWM_COUNT_MAX 32            // used by mstimer.c to perform Soft PWM

extern const uint8_t PROGMEM led_map[THM_LIGHTING_NUM_ROW][THM_LIGHTING_NUM_COL];

typedef struct _node_id_t {
    uint8_t id;
    uint8_t artery;
} node_id_t;

node_id_t node_id;

#ifdef SERIAL_CONSOLE
uint8_t thm_qtouch_report;
#endif

uint8_t thm_solenoid_pwm_active;
#define IS_PWM_ENABLED_SOL1()   (thm_solenoid_pwm_active & _BV( 0 ))
#define IS_PWM_ENABLED_SOL2()   (thm_solenoid_pwm_active & _BV( 1 ))
#define ENABLE_PWM_SOL1()       (thm_solenoid_pwm_active |= _BV( 0 ))
#define ENABLE_PWM_SOL2()       (thm_solenoid_pwm_active |= _BV( 1 ))
#define DISABLE_PWM_SOL1()      (thm_solenoid_pwm_active &= ~_BV( 0 ))
#define DISABLE_PWM_SOL2()      (thm_solenoid_pwm_active &= ~_BV( 1 ))

void thm_init( void );

uint8_t thm_getLEDNum( uint8_t row_index, uint8_t col_index );
void thm_readNodeID( void );
void thm_setNodeID( uint8_t id_num, uint8_t artery_num );
void thm_setTouchThreshold( uint8_t threshold );
void thm_setDefaultPatch( char patch_id );
void thm_setDefaultCPatch( char cpatch_id );

void thm_setRXIndicator( void );
void thm_clearRXIndicator( void );
void thm_setTXIndicator( void );
void thm_clearTXIndicator( void );

void thm_pwm_update( void );

uint8_t sol1_pwm_val;
uint8_t sol2_pwm_val;

#endif

