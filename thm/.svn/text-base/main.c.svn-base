/**
 * A Unit test for the TLC5940 Lighting board for use with the Heart Machine
 * Node board.
 *
 * The program starts in 'twinkle' mode. A serial console is present to set a
 * number of parameters at run time. Connect via serial terminal 57600 8N1.
 *
 * Help is available via 'h<enter>'
 *
 * alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "revision.h"

#include "can.h"
#include "can_command.h"

#include "patches.h"
#include "patch_id.h"
#include "uart.h"
#include "serial_console.h"
#include "mstimer.h"
#include "utility.h"

#include "qtouch.h"
#include "touch_qt_config.h"
#include "touch_api.h"
#include "Tlc5940.h"

#include "thm_node.h"

#include "light_effects.h"

// All our patches are here
#include "patch_collection.h"

#ifdef SERIAL_CONSOLE
#include "serial_console.h"
#endif

#define TOUCHED_VAL             20 // Why is this sent out?
#define TOUCHED_SENSOR_NUM      0  // So far sensor 2 is used ? WHY

/**
 *  Main Function and Entry point into HEART MACHINE OBLIVION.
 */
int main(void)
{
    int16_t* channel_deltas;
    uint8_t  channel_num;
    uint8_t  node_molested;
    char     patch_id;

    // These timers are used to schedule events to be fired at regular intervals
    mstimer_t qtouch_timer;
    mstimer_t qtouch_report_timer;
    mstimer_t qtouch_send_timer;

    // Setup all the hardware and modules
    thm_init();
    
    node_molested = 0; // Not yet Molested

    // qtouch timer for scheduling measurements
    mstimer_init( &qtouch_timer, qt_measurement_period_msec );

    mstimer_init( &patch_timer, 30 );   // 33 times/second -> patches should actually set this
    mstimer_init( &cpatch_timer, 30 );   // 33 times/second -> patches should actually set this
    mstimer_init( &sol1_timer, 0 );     // Every Control Patch should set this
    mstimer_init( &sol2_timer, 0 );	    // Every Control Patch should set this
    mstimer_init( &qtouch_report_timer, 250 );  // 1/4 second
    mstimer_init( &qtouch_send_timer, 1000 );	// once a second

    // Enable Interrupts! Woohoo
    sei();

#ifdef SERIAL_CONSOLE
    printf_P( PSTR( "The Heart Machine - Node Firmware - r%d\n\r" ), REVISION_NUM );

    if ( node_id.id == THM_NODE_UNKNOWN_ID
         || node_id.artery == THM_ARTERY_UNKNOWN_ID) {
        printf_P( PSTR( "** Node and Artery ID have __NOT__ been set yet **\n\r") );
    } else {
        printf_P( PSTR( "Node ID: %u Artery ID: %u\n\r" ),
            node_id.id, node_id.artery );
    }

    printf_P( PSTR( "Completed TLC5940 Initialization.\n\r" ) );
    printf_P( PSTR( "Completed MCP2515 Initialization.\n\r" ) );
    printf_P( PSTR( "See Documentation on Serial Console Usage.\n\r\n\r" ) );
    printf_P( PSTR( "Starting Patch Registration process ... \n\r") );
#endif

    registerPatches();

#ifdef SERIAL_CONSOLE
    printf_P( PSTR( "Registered %d patches: \n\r" ),
        num_patches );

    printf_P( PSTR( "Initializing All Patches\n\r") );
#endif

    initializeAllPatches();

    patch_id = eeprom_read_byte( DEFAULT_PATCH_EEPROM_ADDR );
    if ( patch_id == DEFAULT_PATCH_UNSET )
        patch_id = DEFAULT_PATCH_ID;
    setCurrentPatch( patch_id );

    patch_id = eeprom_read_byte( DEFAULT_CPATCH_EEPROM_ADDR );
    if ( patch_id == DEFAULT_PATCH_UNSET )
        patch_id = DEFAULT_CPATCH_ID;
    setCurrentCPatch( patch_id ) ;

    qt_update();
    channel_deltas = qt_getLastMeasurement();

    for( ; ; ) {
        // Update QTouch Measurements
        if ( mstimer_completed( &qtouch_timer ) ) {
            mstimer_reset( &qtouch_timer );
            qt_update();
            
            // We've been touched! Agghhh
            // We'll probably want to count the time that we've been
            // continuously touched, but lets not for now.
            if ( QT_SENSOR_TOUCHED( TOUCHED_SENSOR_NUM ) ) {
                node_molested = 1; // it's ok. Definitely a good touch
                // printf("Molested!\n\r");
            }

            channel_deltas = qt_getLastMeasurement();
            // or .. average values stored in channel_deltas
            // qt_getAverageMeasurement( channel_deltas );
        }

        // Update the current patch and pass in qtouch delta values
        if ( mstimer_completed( &patch_timer ) ) {
            mstimer_reset( &patch_timer );
            if ( current_patch != NULL )
                current_patch->update( channel_deltas );
        }

        if ( mstimer_completed( &cpatch_timer ) ) {
            mstimer_reset( &cpatch_timer );
            if ( current_cpatch != NULL )
                current_cpatch->update( channel_deltas );
        }

// Only if serial console is enabled. Process Serial Byte
#ifdef SERIAL_CONSOLE
        serialProcessByte( USART_receive( stdin ) );

        // This block is display qtouch values. Use 'q' disable
        if ( thm_qtouch_report == THM_QTOUCH_REPORT_ON ) {
            if ( mstimer_completed( &qtouch_report_timer ) ) {
                mstimer_reset( &qtouch_report_timer );

                for ( channel_num = 0; channel_num < QT_NUM_KEYS; channel_num++ ) {
                    printf_P( PSTR("%u: %5d "), channel_num, channel_deltas[channel_num]);
                }
                printf_P( PSTR("\n\r"));
            }
        }
#endif

	    // If at any time during the last interval,
        //  there's been significant input, send a message to the server
		if ( mstimer_completed( &qtouch_send_timer ) ) {
			mstimer_reset( &qtouch_send_timer );

            if ( node_molested ) {
				cancmd_sendTouchMessage(&node_id, TOUCHED_VAL);
                node_molested = 0; // Deny that we've been molested
            }
		}

        // Check for CAN message and process if available.
        if ( ( can_checkReceive() == CAN_MSGAVAIL ) ) {
            cancmd_receiveMessage();
        }

    }
}


