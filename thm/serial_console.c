/**
 *  A simple serial command console to trigger actions or events that would
 *  normally be received by the CAN bus.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "serial_console.h"
#include "uart.h"
#include "utility.h"

static const char cmd_ack_str[]  PROGMEM = "ACK\n\r";
static const char cmd_nack_str[] PROGMEM = "NACK\n\r";

static uint8_t serialBufferPos = 0;
uint8_t (*serialDispatch)(void) = NULL;
static uint8_t serialCmdRetval;

/**
 *  Process an incoming byte. Any incoming data
 */
uint8_t serialProcessByte( uint16_t recv_byte ) {
    serialCmdRetval = CMD_WAITING_TERMINATOR;

    if ( recv_byte == USART_NO_DATA )
        return serialCmdRetval;

#ifdef CMD_ECHO
    USART_transmit( recv_byte, stdout );
#endif

    // Received input
    if ( recv_byte == CMD_TERMINATOR ) {
        serialCommandBuffer[serialBufferPos] = '\0';
        serialCommandBuffer[serialBufferPos+1] = '\0';

#ifdef CMD_DEBUG
        printf_P( PSTR("cmd buffer: _%s_\n\r"), serialCommandBuffer);
#endif

        // Process Command
        if ( serialDispatch )
            serialCmdRetval = serialDispatch( );

        // Reset Buffer
        serialBufferPos = 0;

    } else {
        serialCommandBuffer[ serialBufferPos++ ] = recv_byte;
    }

    return serialCmdRetval;
}

 void sendACK( void ) {
    printf_P( cmd_ack_str );
}

 void sendNACK( void ) {
    printf_P( cmd_nack_str );
}

