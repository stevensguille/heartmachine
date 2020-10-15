/**
 *  Functions for intializing and operating The Heart Machine specific hardware.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#include <avr/eeprom.h> 
#include <avr/io.h>

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

#ifdef SERIAL_CONSOLE
#include "serial_console.h"
#include "serial_command.h"
#endif

FILE uart_str = FDEV_SETUP_STREAM(USART_transmit, USART_receive, _FDEV_SETUP_RW);


/**
 *  Initialize all THM hardware and peripherals.
 */
void thm_init() {
    USART_init( 57600 );    

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

    // res = can_init( CAN_125KBPS );
    can_init( CAN_16_125KBPS, &node_id );

    // Enable the timer for a one millisecond tick
    mstimer_initTimer();

    // uncomment to Enable CAN Debug mode
    // can_debug1();

    // Redirect stdout and stdin streams through the UART
    stdout = stdin = &uart_str;

    qt_init();

#ifdef SERIAL_CONSOLE
    serialDispatch = &thm_commandDispatch;
    thm_qtouch_report = THM_QTOUCH_REPORT_ON;
#endif

    // Enable Interrupts
    sei();

    // Set MCP2515 to use GPIO for LED control
    mcp2515_setRegister( MCP_BFPCTRL, MCP_RXB_B0BFE | MCP_RXB_B1BFE );

    thm_setCANFilter( &node_id );
}


/**
 *  Very Unoptimized setting of an RGB value
 */
void thm_setLightingRGB( uint8_t led_num , rgb_t* val ) { 
    tlc_set(led_num+THM_LIGHTING_RED_BANK,   val->red);
    tlc_set(led_num+THM_LIGHTING_GREEN_BANK, val->green);
    tlc_set(led_num+THM_LIGHTING_BLUE_BANK,  val->blue);
}


/**
 *  Very Unoptimized setting of an RGB value to all RGBs
 */
void thm_setLightingAll( rgb_t* val ) {
    int led_num;

    for ( led_num=0; led_num < THM_LIGHTING_NUM_LEDS; led_num++ ) {
        thm_setLightingRGB( led_num, val );
    }
}


/**
 *  Read the node ID from the EEPROM and store in the local state.
 */
void thm_readNodeID() {
    node_id.id     = eeprom_read_byte((uint8_t*)NODE_ID_EEPROM_ADDR);
    node_id.artery = eeprom_read_byte((uint8_t*)ARTERY_ID_EEPROM_ADDR);
}


/**
 *  Set the Node ID and write it to eeprom
 */
void thm_setNodeID( uint8_t id_num, uint8_t artery_num) { 
    eeprom_write_byte( NODE_ID_EEPROM_ADDR, id_num );
    eeprom_write_byte( ARTERY_ID_EEPROM_ADDR, artery_num );

    // Now update our local id
    thm_readNodeID();

    // Update the CAN filter
    thm_setCANFilter( &node_id );
}


/**
 *  Convert a node_id type to a uint32_t can_id.
 */
uint32_t thm_convertToCanID( node_id_t* node_id ) {
    uint32_t can_id;

    can_id =  node_id->id;
    can_id |= ((node_id->artery & 0x7) << 8);

    return can_id;
}

/**
 *  Set the CAN filter using the node id.
 *
 *  We have an 11-bit identifier.i
 *  <8:11> Artery ID
 *  <0:7>  Node ID
 *
 *  For now, our mask will not match for the Artery ID
 */
uint8_t thm_setCANFilter( node_id_t* node_id ) {
    uint32_t can_id;
    uint8_t retval;
    
    if ( node_id->id == THM_NODE_UNKNOWN_ID
      || node_id->artery == THM_ARTERY_UNKNOWN_ID) {
        // We have yet to set the Node ID in eeprom
        retval = THM_SET_CAN_ID_FAIL;
    }

    can_id = thm_convertToCanID( node_id );
    // set the filter value
    // filter mask should only be for lowest 8 bits, 0xff
    retval = THM_SET_CAN_ID_SUCCESS;

    return retval;
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

