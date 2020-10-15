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
#include "light_effects.h"
#include "uart.h"
#include "utility.h"
#include "thm_node.h"
#include "patch_collection.h"

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
    int i;

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
            case '1': // set a row (row_num,r,g,b)
                num_parsed = getArgs(arg_str, 'u', 4, args);
                if ( num_parsed != 4 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( args[0] >= THM_LIGHTING_NUM_ROW ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "row %u = (%u,%u,%u)\n\r" ),
                                args[0], args[1], args[2], args[3] );
#endif      
                    out.red   = args[1];
                    out.green = args[2];
                    out.blue  = args[3];

                    setLightRow( args[0], &out );
                } 
                break;

            case '2': // set a col (col_num,r,g,b)
                num_parsed = getArgs(arg_str, 'u', 4, args);
                if ( num_parsed != 4 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( args[0] >= THM_LIGHTING_NUM_COL ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "row %u = (%u,%u,%u)\n\r" ),
                                args[0], args[1], args[2], args[3] );
#endif      
                    out.red   = args[1];
                    out.green = args[2];
                    out.blue  = args[3];

                    setLightCol( args[0], &out );
                } 
                break;

            case 'r': // set a rectangle (x,y,w,h,r,g,b)
                num_parsed = getArgs(arg_str, 'u', 7, args);
                if ( num_parsed != 7 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( args[0] >= THM_LIGHTING_NUM_ROW
                         || args[1] >= THM_LIGHTING_NUM_COL ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "rect (x,y,w,h) (%u,%u,%u,%u) = (%u,%u,%u)\n\r" ),
                                args[0], args[1], args[2], args[3],
                                args[4], args[5], args[6]);
#endif      
                    out.red   = args[4];
                    out.green = args[5];
                    out.blue  = args[6];

                    setLightRect( args[0], args[1], args[2], args[3],
                            &out );
                } 

                break;

            case 'l': // set LED to color
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
                    setLightLED( args[0], &out );
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
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting all color to: %u %u %u\n\r" ),
                            args[0], args[1], args[2]);
#endif
                    out.red   = args[0];
                    out.green = args[1];
                    out.blue  = args[2];
                    setLightAll( &out );
                    tlc_update();
                }
                break;

            case 'p':
                // Set THM Patch by ID.
                // @params: patch_id_char
                num_parsed = getArgs(arg_str, 'c', 1, args);
                args[0] = (char)args[0];

                if( num_parsed != 1 ) {
                    retval = CMD_PARSE_ERROR;
                } else if( isValidPatchID( args[0] ) != THM_PATCH_VALID_ID ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting current visual patch to ID %c\n\r" ),
                           (char)args[0] );
#endif
                    setCurrentPatch( (char)args[0] );
                }
                break;

            case 'b':
                // Set THM Control Patch by number.
                // @params: patch_id char
                num_parsed = getArgs(arg_str, 'c', 1, args);
                args[0] = (char)args[0];

#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting current control patch to ID %c\n\r" ),
                            (char)args[0] );
#endif

                if( num_parsed != 1 ) {
                    retval = CMD_PARSE_ERROR;
                } else if( isValidCPatchID( (char)args[0] ) != THM_PATCH_VALID_ID ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    setCurrentCPatch( (char)args[0] );
                }
                break;
            
            case '?':
                // What patch are we running?
                printf_P(PSTR("%c\n\r"), current_cpatch->id);
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
                    printf_P( PSTR( "Setting current patch parameter %u to %u\n\r" ), args[0], args[1] );
#endif
                    patch_data[ args[0] ] = args[1];
                }
                break;

            case 'n':
                // Set Control Patch Data.
                // @params: data_index data_value
                num_parsed = getArgs(arg_str, 'u', 2, args);
                if ( num_parsed != 2 ) {
                    retval = CMD_PARSE_ERROR;
                } else if ( (args[0] < 0) || (args[0] >= THM_PATCH_NUM_DATA ) ) {
                    retval = CMD_ARGUMENT_OUT_OF_RANGE;
                } else {
#ifdef CMD_DEBUG
                   printf_P( PSTR( "Setting current control patch parameter %u to %u\n\r" ), args[0], args[1] );
#endif
                    cpatch_data[ args[0] ] = args[1] ;
                }
                break;

            case 'N':
                // get cpatch data.
                // @params: data_index data_value
                for ( i=0; i<THM_PATCH_NUM_DATA; i++)
                    printf_P( PSTR( "%u: %d " ), i, cpatch_data[i]);
                printf_P( PSTR("\n\r") );
                break;
                
            case 'i':
                // Set the node ID
                // @params: node_id artery_id
                num_parsed = getArgs( arg_str, 'u', 2, args );
                if ( num_parsed != 2 ) {
                    retval = CMD_PARSE_ERROR; 
                } else {
#ifdef CMD_DEBUG
                    printf_P( PSTR( "Setting Node ID to %u Artery: %u\n\r" ), args[0], args[1] );
                    // Do a reset via watchdog
#endif
                    thm_setNodeID( args[0], args[1]  );
                    printf_P( PSTR( "Please Reset Now ...\n\r" ) );
                    for ( ;; );
                }
                break;

            case 'o':
                // Get the Current Node ID
                // @params: none
                num_parsed = getArgs( arg_str, 'd', 1, args );

                if ( num_parsed != 0 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    printf_P( PSTR("%d %d\n\r"), node_id.id, node_id.artery);
                }
                break;

            case 'g':
                // Return the value of a data parameter of the current patch
                // @params: data_index
                for ( i=0; i<THM_PATCH_NUM_DATA; i++)
                    printf_P( PSTR( "%u: %d " ), i, patch_data[i]);
                printf_P( PSTR("\n\r") );
                break;
            


            case 'x':
                // Activate current control patch.
                // @params: activate value
                num_parsed = getArgs( arg_str, 'd', 2, args);
                if ( num_parsed < 1 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    current_cpatch->activate(args);
                }
                break;

            case 'q':
                // Toggle the qtouch reporting
                num_parsed = getArgs( arg_str, 'd', 1, args);
                if ( num_parsed != 0 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    thm_qtouch_report = !thm_qtouch_report;
#ifdef CMD_DEBUG
                    printf_P( PSTR("Qtouch Report: %u\n\r"), thm_qtouch_report);
#endif
                }
                break;

            case 'Q':
                // Set the qtouch threshold value
                num_parsed = getArgs( arg_str, 'u', 1, args);
                if ( num_parsed != 1 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    thm_setTouchThreshold( (uint8_t)args[0] );
                }
                break;

            case '[':
                // set solenoid values
                num_parsed = getArgs( arg_str, 'u', 2, args);
                if ( num_parsed != 2 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    sol1_pwm_val = args[0];
                    sol2_pwm_val = args[1];
                }
                break;

            case '<':
                // Enable Solenoid PWM
                ENABLE_PWM_SOL1();
                ENABLE_PWM_SOL2();
                break;

            case '>':
                // disable Solenoid PWM
                DISABLE_PWM_SOL1();
                DISABLE_PWM_SOL2();
                break;

            case '{':
                // Set Default Patch
                num_parsed = getArgs( arg_str, 'c', 1, args);
                if ( num_parsed != 1 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    thm_setDefaultPatch( (char)args[0] );
                }
                break;

            case '}':
                // Set Default CPatch
                num_parsed = getArgs( arg_str, 'c', 1, args);
                if ( num_parsed != 1 ) {
                    retval = CMD_INVALID_ARGUMENT;
                } else {
                    thm_setDefaultCPatch( (char)args[0] );
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
