/**
 *  A simple serial command console to trigger actions or events that would
 *  normally be received by the CAN bus.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#ifndef _SERIAL_CONSOLE_H_
#define _SERIAL_CONSOLE_H_

// Uncomment to enable verbose output
// #define CMD_VERBOSE

#define CMD_TERMINATOR          '\r'
#define CMD_FORMATCHAR          'd'

#define CMD_BUF_LEN             32

#define CMD_SUCCESS                 0
#define CMD_PARSE_ERROR             1
#define CMD_INVALID_ARGUMENT        2
#define CMD_INVALID_COMMAND         3
#define CMD_ARGUMENT_OUT_OF_RANGE   4
#define CMD_ARGUMENT_PARSE_ERROR    5

#define CMD_WAITING_TERMINATOR  0xff

#define CMD_ACK_STRING          "ACK\n\r"
#define CMD_NACK_STRING         "NACK\n\r"

// Uncomment to enable echo
// #define CMD_ECHO

// Uncomment to enable debug output
// #define CMD_DEBUG 1

// Command buffer
char    serialCommandBuffer[CMD_BUF_LEN];

extern uint8_t (*serialDispatch)(void);

// By default, read numerical values as decimal integers
uint8_t serialProcessByte( uint16_t recv_byte );

void    sendACK( void );
void    sendNACK( void );

#endif
