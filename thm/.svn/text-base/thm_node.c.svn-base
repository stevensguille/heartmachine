/**
 *  Functions for intializing and operating The Heart Machine specific hardware.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#include <avr/eeprom.h> 
#include <avr/io.h>
#include <avr/wdt.h>

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "thm_node.h"
#include "uart.h"
#include "Tlc5940.h"
#include "mcp2515.h"
#include "mcp2515_defs.h"
#include "can.h"
#include "mstimer.h"
#include "qtouch.h"
#include "light_effects.h"
#include "utility.h"
#include "can_command.h"
#include "patch_collection.h"

#ifdef SERIAL_CONSOLE
#include "serial_console.h"
#include "serial_command.h"
#endif

FILE uart_str = FDEV_SETUP_STREAM(USART_transmit, USART_receive, _FDEV_SETUP_RW);

const uint8_t led_map[THM_LIGHTING_NUM_ROW][THM_LIGHTING_NUM_COL] PROGMEM = {
    { 0, 1, 2, 3 },
    { 4, 5, 6, 7 },
    { 8, 9, 10, 11},
    { 12, 13, 14, 15}
};


/**
 *  Initialize all THM hardware and peripherals.
 */
void thm_init() {
    uint8_t can_filter_l, can_filter_h;
    uint8_t touch_threshold;
    uint16_t stdid;
    int i;
    rgb_t c;
    
    USART_init( 57600 );    
    stdout = stdin = &uart_str;
    sei();
    
    // Read our current NODE ID from EEPROM
    thm_readNodeID();

    // maybe get a better seed from ADC values
    srand(0);

    // Set Chip Select and solenoid Pins as output
    CSTLC_DDR |= _BV( CSTLC_BIT );
    CSCAN_DDR |= _BV( CSCAN_BIT );
    SOL1_DDR  |= _BV( SOL1_BIT );
    SOL2_DDR  |= _BV( SOL2_BIT );

    THM_TLC_CS_SELECT();
    
    // Initialize TLC5940 Module with zero values on all channels.
    // This also initilizes the SPI bus
    tlc_init(0);

    THM_TLC_CS_UNSELECT();

    printf_P( PSTR("Initializing mcp2515: \n\r ") );
    // res = can_init( CAN_125KBPS );
    if ( can_init( CAN_16_125KBPS ) == MCP2515_OK )
        printf_P( PSTR("mcp2515 - OK\n\r"));
    else
        printf_P( PSTR("mcp2515 - FAIL\n\r"));
    //can_init( CAN_16_125KBPS, node_id.artery, node_id.id );

    mcp2515_setRegister( MCP_RXM0SIDH, THM_MCP2515_ARTERY_ID_MASK_H );
    mcp2515_setRegister( MCP_RXM0SIDL, THM_MCP2515_ARTERY_ID_MASK_L );
    mcp2515_setRegister( MCP_RXM1SIDH, THM_MCP2515_NODE_ID_MASK_H );
    mcp2515_setRegister( MCP_RXM1SIDL, THM_MCP2515_NODE_ID_MASK_L );

    // Set the Filter Mask for Node on Buffer 1
    // Artery is the High 6 bytes and node ID is low 5 bytes
    // of the 11-bit identifier.

    can_filter_h = ( ( 1 << (node_id.artery - 1) ) << 2);
    can_filter_l = 0;

    mcp2515_setRegister( MCP_RXF0SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF0SIDH, can_filter_h );
    mcp2515_setRegister( MCP_RXF1SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF1SIDH, can_filter_h );

    // Now for the Filter for Node ID
    can_filter_h = (( node_id.id >> 3 ) & 0x07 );
    can_filter_l = (( node_id.id & 0x07 ) << 5 );

    mcp2515_setRegister( MCP_RXF2SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF2SIDH, can_filter_h );
    mcp2515_setRegister( MCP_RXF3SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF3SIDH, can_filter_h );
    mcp2515_setRegister( MCP_RXF4SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF4SIDH, can_filter_h );
    mcp2515_setRegister( MCP_RXF5SIDL, can_filter_l );
    mcp2515_setRegister( MCP_RXF5SIDH, can_filter_h );

    can_filter_l = mcp2515_readRegister( MCP_RXF0SIDL );
    can_filter_h = mcp2515_readRegister( MCP_RXF0SIDH );

    stdid = (can_filter_h << 3) | (( can_filter_l >> 5 ) & 0x03);

    printf_P( PSTR("Putting mcp2515 in NORMAL mode - "));

    if ( mcp2515_setCANCTRL_Mode( MODE_NORMAL ) == MCP2515_OK ) {
        printf_P( PSTR("OK\n\r") );
    }

    // Enable the timer for a one millisecond tick
    mstimer_initTimer();

    // Set duty cycle to 0 for both solenoids
    DISABLE_PWM_SOL1();
    DISABLE_PWM_SOL2();
    THM_CLEAR_SOL1();
    THM_CLEAR_SOL2();

    sol1_pwm_val = 0;
    sol2_pwm_val = 0;
    
    // uncomment to Enable CAN Debug mode
#ifdef CAN_DEBUG
#if CAN_DEBUG  == 1 
    can_debug1();
#endif
#endif

    // Read in the calibrated touch threshold, use default
    // if not set
    touch_threshold = eeprom_read_byte(TOUCH_THRESHOLD_EEPROM_ADDR);

    if ( touch_threshold == TOUCH_THRESHOLD_UNCALIBRATED )
        touch_threshold = THM_QTOUCH_DEFAULT_THRESHOLD;

    printf_P( PSTR("Initializing QTouch with touch threshold: %u\n\r"),
            touch_threshold);
    qt_init( touch_threshold );

#ifdef SERIAL_CONSOLE
    serialDispatch = &thm_commandDispatch;
    thm_qtouch_report = THM_QTOUCH_REPORT_OFF;
#endif

    // Enable Interrupts

    // Set MCP2515 to use GPIO for LED control
    mcp2515_setRegister( MCP_BFPCTRL, MCP_RXB_B0BFE | MCP_RXB_B1BFE );

    // We'll try blinking network lights as verification of 
    // CKOUT Signal

    for ( i=0; i<4; i++ ) {
        thm_setTXIndicator();
        mstimer_delayms( 100u );
        thm_setRXIndicator();
        mstimer_delayms( 100u );
        thm_clearRXIndicator();
        thm_clearTXIndicator();
        mstimer_delayms( 100u );
    }

    // Now set both lights and send Hello Message!
    thm_setTXIndicator();
    thm_setRXIndicator();

    printf_P( PSTR("Broadcasting Presence on CAN Bus - ") );

    if ( cancmd_sayHello( &node_id ) == CAN_OK ) {
        printf_P( PSTR("OK\n\r") );
        c.green = 1000;
        setLightAll( &c );
        THM_TLC_CS_SELECT();
        tlc_update();
        THM_TLC_CS_UNSELECT();
        // Flash once.
        thm_clearTXIndicator();
        thm_clearRXIndicator();
        mstimer_delayms( 100u );
        thm_setTXIndicator();
        thm_setRXIndicator();
        mstimer_delayms( 250u );

    } else { // our hello message was not received ... 
        // flash four times quickly.
        printf_P( PSTR("FAIL\n\r") );
        c.red = 4000;
        setLightAll( &c );
        THM_TLC_CS_SELECT();
        tlc_update();
        THM_TLC_CS_UNSELECT();
        for ( i=0; i<4; i++ ) {
            thm_clearTXIndicator();
            thm_clearRXIndicator();
            mstimer_delayms( 100u );
            thm_setTXIndicator();
            thm_setRXIndicator();
            mstimer_delayms( 300u );
        }
    }
    
    tlc_clear();

    THM_TLC_CS_SELECT();
    tlc_update();
    THM_TLC_CS_UNSELECT();

    thm_clearTXIndicator();
    thm_clearRXIndicator();
}

/**
 *  Returns the corresponding LED number given a Row and Column.
 */
uint8_t thm_getLEDNum( uint8_t row_num, uint8_t col_num ) {
    return pgm_read_byte(&(led_map[row_num][col_num]));
}


/**
 *  Read the node ID from the EEPROM and store in the local state.
 */
void thm_readNodeID() {
    node_id.id      = eeprom_read_byte(NODE_ID_EEPROM_ADDR);
    node_id.artery  = eeprom_read_byte(ARTERY_ID_EEPROM_ADDR);
}


/**
 *  Set the Node ID and write it to eeprom
 */
void thm_setNodeID( uint8_t id_num, uint8_t artery_num) { 
    eeprom_write_byte( NODE_ID_EEPROM_ADDR, id_num );
    eeprom_write_byte( ARTERY_ID_EEPROM_ADDR, artery_num );

    // Now update our local id
    thm_readNodeID();
}

/**
 *  Save a new touch threshold value
 *  (we assume that threshold_t fits in one byte
 */
void thm_setTouchThreshold( threshold_t threshold ) { 
    eeprom_write_byte( TOUCH_THRESHOLD_EEPROM_ADDR, threshold );

    // NEED A RESET NOW
}

/**
 *  Set the Default Patch. No effect if Patch ID is invalid.
 */
void thm_setDefaultPatch( char patch_id ) { 
    if ( isValidPatchID( patch_id ) == PATCH_VALID_ID ) {
        eeprom_write_byte( DEFAULT_PATCH_EEPROM_ADDR, patch_id ); 
    }
}

/**
 *  Set the Default CPatch. No effect if Patch ID is invalid.
 */
void thm_setDefaultCPatch( char cpatch_id ) {
    if ( isValidCPatchID( cpatch_id ) == PATCH_VALID_ID ) {
        eeprom_write_byte( DEFAULT_CPATCH_EEPROM_ADDR, cpatch_id ); 
    }
}

/**
 *  Set the CAN RX Led Indicator
 */
void thm_setRXIndicator() {
    mcp2515_modifyRegister(MCP_BFPCTRL, MCP_RXB_B1BFS, 0xff);
}


/**
 *  Clear the CAN RX Led Indicator
 */
void thm_clearRXIndicator() {
    mcp2515_modifyRegister(MCP_BFPCTRL, MCP_RXB_B1BFS, 0);
}


/**
 *  Set the CAN TX Led Indicator
 */
void thm_setTXIndicator() {
    mcp2515_modifyRegister(MCP_BFPCTRL, MCP_RXB_B0BFS, 0xff);
}


/**
 *  Clear the CAN TX Led Indicator
 */
void thm_clearTXIndicator() {
    mcp2515_modifyRegister(MCP_BFPCTRL, MCP_RXB_B0BFS, 0);
}


