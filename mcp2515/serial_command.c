/**
 *  A simple serial command console to trigger actions or events that would
 *  normally be received by the CAN bus.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "serial_console.h"
#include "serial_command.h"
#include "patches.h"
#include "Tlc5940.h"
#include "uart.h"
#include "utility.h"
#include "thm_node.h"

#ifdef SERIAL_CONSOLE

static int16_t args[ CMD_MAX_ARGS ];

/**
 *  Process a command string and perform the relevant function. All relevant commands
 *  should be performed within.
 *
 *  returns 0 on success, otherwise an error code is returned. See above for error
 *  code definitions.
 */
uint8_t thm_commandDispatch( ) {
    int num_parsed;
    uint8_t retval;
    char* arg_str;
    char* pchar;
    rgb_t out;

    // general purpose
    // int i,j;

    retval = CMD_SUCCESS;

    pchar = strtok( serialCommandBuffer, " " );

    if ( pchar == NULL || strlen(pchar) != 1 ) {
        // Could not tokenize the command string
        //
#ifdef CMD_DEBUG
        printf_P( PSTR("Invalid command character: '%s'\n\r"), \
                pchar == NULL ? "" : pchar );
#endif

        retval = CMD_INVALID_COMMAND;
    } else {

        // If formatted correctly, the arguments (if present) start here.
        arg_str = serialCommandBuffer + sizeof(char)*2;

#ifdef CMD_DEBUG
        printf_P( PSTR( "ARG STRING: _%s_\n\r" ), arg_str);
#endif

        switch ( pchar[0] ) {
            case 'l': // set all colors
                num_parsed = getArgs(arg_str, 'u', 4, args);
                if ( num_parsed != 4 ) {
                    retval = CMD_PARSE_ERROR;
                } else if( args[0] >= TLC_MAXVAL
                        || args[1] >= TLC_MAXVAL
                        || args[2] >= TLC_MAXVAL ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting LED %u to: %u %u %u\n\r" ),
                            args[0], args[1], args[2], args[3]);
#endif
                    out.red   = args[1];
                    out.green = args[2];
                    out.blue  = args[3];
                    thm_setLightingRGB( args[0], &out );
                    tlc_update();
                }
                break;

            case 'c': // set all colors
                num_parsed = getArgs(arg_str, 'u', 3, args);
                if ( num_parsed != 3 ) {
                    retval = CMD_PARSE_ERROR;
                } else if( args[0] >= TLC_MAXVAL
                        || args[1] >= TLC_MAXVAL
                        || args[2] >= TLC_MAXVAL ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting all color to: %u %u %u\n\r" ),
                            args[0], args[1], args[2]);
#endif
                    out.red   = args[0];
                    out.green = args[1];
                    out.blue  = args[2];
                    thm_setLightingAll( &out );
                    tlc_update();
                }
                break;

            case 'p':
                // Set THM Patch by number.
                // @params: patch_number
                num_parsed = getArgs(arg_str, 'c', 1, args);
                args[0] = (char)args[0];

                if( num_parsed != 1 ) {
                    retval = CMD_PARSE_ERROR;
                } else if( isValidPatchNum( args[0] ) != THM_PATCH_VALID_NUM ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
#ifdef CMD_DEBUG
                    printf( PSTR( "Setting current patch to index %u\n\r" ), args[0] );
#endif
                    setCurrentPatch( args[0] );
                }
                break;

            case 'd':
                // Set Patch Data.
                // @params: data_index data_value
                num_parsed = getArgs(arg_str, 'd', 2, args);
                if ( num_parsed != 2 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( (args[0] < 0) || (args[0] >= THM_PATCH_NUM_DATA ) ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                    printf( PSTR( "Setting current patch to index %u\n\r" ), args[0] );
#endif
                    setCurrentPatchData( args[0], args[1] );
                }
                break;

            case 'i':
                // Set the node ID
                // @params: node_id
                num_parsed = getArgs( arg_str, 'd', 1, args );
                if ( num_parsed != 1 ) {
                    retval = CMD_PARSE_ERROR; 
                } else {
#ifdef CMD_DEBUG
                    printf( PSTR( "Setting Node ID to %u\n\r" ), args[0] );
#endif
                    thm_setNodeID( args[0], THM_DEFAULT_ARTERY_NUM );
                }
                break;

            case 'o':
                // Get the Current Node ID
                // @params: none
                num_parsed = getArgs( arg_str, 'd', 1, args );

                if ( num_parsed != 0 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    printf("%d %d\n\r", node_id.id, node_id.artery);
                }
                break;

            case 'g':
                // Return the value of a data parameter of the current patch
                // @params: data_index
                num_parsed = getArgs( arg_str, 'd', 1, args);
                if ( num_parsed != 1 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( (args[0] < 0) || (args[0] >= THM_PATCH_NUM_DATA ) ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
                    printf("%d\n\r", current_patch->data[args[0]]);
                }
                break;

            case 'x':
                // Get the current patch 'progress'
                // @params: none
                num_parsed = getArgs( arg_str, 'd', 1, args);
                if ( num_parsed != 0 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    printf("%d\n\r", current_patch->progress);
                }
                break;

            case 'q':
                // Toggle the qtouch reporting
                num_parsed = getArgs( arg_str, 'd', 1, args);
                if ( num_parsed != 0 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    thm_qtouch_report = ~thm_qtouch_report;
#ifdef CMD_DEBUG
                    printf("Qtouch Report: %u\n\r", thm_qtouch_report);
#endif
                }
                break;

            default:

#ifdef CMD_DEBUG
                printf_P( PSTR("Unknown command character: '%c'\n\r"), pchar[0] );
#endif
                retval = CMD_INVALID_COMMAND;
        }
    }

    // clear the command
    serialCommandBuffer[0] = '\0';

    if ( retval == CMD_SUCCESS ) {
        sendACK(); 
    } else {
        sendNACK();
    }

    return retval;
}

#endif
