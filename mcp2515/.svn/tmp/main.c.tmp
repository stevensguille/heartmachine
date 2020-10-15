/**
 *  node_emu - Unit test for the mcp2515 CAN Controller as connected to
 *  The Heart Machine node board.
 *
 *  This program will Display any received CAN Messages on the serial port
 *  and reply using the command set described in can_command.{c,h}
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "thm_defines.h"

#include "can.h"
#include "can_command.h"
#include "delay.h"
#include "mcp2515.h"
#include "mcp2515_defs.h"
#include "mstimer.h"
#include "thm_node.h"


/**
 * Small Main program to test CAN Message Reception and reply
 */
int main(void)
{
    thm_init();

	sei();
	
	printf_P( PSTR("The Heart Machine: Node CAN Interface Test\n\r") );
	printf_P( PSTR("using mcp2515 library by Martin Thomas\n\r\n\r") );
    
    if ( node_id.id == THM_NODE_UNKNOWN_ID
         || node_id.artery == THM_ARTERY_UNKNOWN_ID) {
        printf_P( PSTR( "** Node and Artery ID have __NOT__ been set yet **\n\r") );
    } else {
        printf_P( PSTR( "Node ID: %u Artery ID: %u\n\r" ),
            node_id.id, node_id.artery );
    }

	printf_P( PSTR("This program will output all received messages and data on\n\r") );
	printf_P( PSTR("the serial console\n\r") );

<<<<<<< .mine
    printf( "Initializing SPI " );
	spi_init();		// init SPI-Interface (as "Master")
    printf("- OK\r");
	
	printf( "Initializing CAN Interface " );
	res = can_init( CAN_16_250KBPS );

	if (res == CAN_OK)
        printf( "- OK\r" );
    else
        printf( "- FAIL\r" );
	
	msg_avail_flag = 0;

    // run in debug mode (This is set to run in loopback)
    can_debug1();

    srand(512);

    // main loop	
=======
>>>>>>> .r269
	for (;;) {
        // This block executes in a tight loop as fast as the CPU can handle
        if ( ( can_checkReceive() == CAN_MSGAVAIL ) ) {
            printf_P( PSTR("CAN Message Available.\n\r"));
            cancmd_receiveMessage();
        }
    }
	
	return 0;
}

