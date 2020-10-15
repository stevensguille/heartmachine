/**
 * A Unit test for the TLC5940 Lighting board for use with the Heart Machine
 * Node board.
 *
 * The program starts in 'twinkle' mode. A serial console is present to set a
 * number of parameters at run time. Connect via serial terminal 57600 8N1.
 *
 * Help is available via h<enter>
 *
 * alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "thm_defines.h"

#include "uart.h"
#include "mstimer.h"

#include "Tlc5940.h"
#include "tlc_demo.h"

#define CMD_TERMINATOR   '\r'

#define CMD_BUF_LEN             32
#define CMD_SUCCESS             0
#define CMD_PARSE_ERROR         1
#define CMD_INVALID_ARGUMENT    2
#define CMD_INVALID_COMMAND     3

FILE uart_str = FDEV_SETUP_STREAM(USART_Transmit, USART_Receive, _FDEV_SETUP_RW);

uint8_t  tlc_cs_on = 1;

// By default, read values as decimal integers
char     fmt_char = 'd';

ISR(TIMER1_OVF_vect)
{
    tlc_disable_XLAT_pulses();
    tlc_clear_XLAT_interrupt();
    tlc_needXLAT = 0;
    if (tlc_onUpdateFinished) {
        sei();
        tlc_onUpdateFinished();
    }
}

ISR(TIMER0_COMPA_vect) {
    updateTick();
}

/**
 * Display Usage.
 */
void print_usage( void ) {
    printf_P( PSTR( "\r\
tlc5940_test: usage:\r\
Commands take the form: CMD [VAL1] .... [VALN]\r\
All commands are terminated with a '\\r' character where\r\
CMD is one of the following single characters:\r\
\r\
    m - Set mode of TLC behavior\r\
        s <mode>\r\
        Valid modes are:\r\
          p - pulsate (var: speed)\r\
          s - static\r\
          r - random\r\
          w - random walk (doesn't really work)\r\
          t - twinkle  (var: speed)\r\
\r\
    s - Set value of a specific TLC Channel\r\
        s <channel> <value>\r\
        Note: This will put the TLC in static mode.\r\
\r\
    d - Set value of a tlc_demo modeData\r\
        d <index> <value>\r\
        Each mode associates an index with a specific function\r\
\r\
    t - Enable/Disable the TLC5940 Pseudo Chip Select\r\
        t <value>\r\
        0: disables chip select, otherwise enabled\r\
\r\
    f - Set the number format used to parse numerical command arguments\r\
        f <d|o|u|x>\r\
        d: signed decimal\r\
        o: octal\r\
        u: unsigned decimal\r\
        x: hexidecimal\r\
\r\
    k - Set the output mask. All outputs will be filtered through\r\
        this mask to determine if the channel should be on or not.\r\
        k <hex_mask>\r\
\r\
    e - Set rx echo\r\
        e <0|1>\r\
        0: disables local echo\r\
        1: enables local echo\r\
\r\
    t - Toggle the tlc5940 pseudo chip-select.\r\
\r\r" ) );
}

/**
 *  Initialize peripheral modules and onboard hardware.
 */
void setup( void ) {
    // maybe get a better seed from ADC values
    srand(0);

    // Initialize TLC5940 Module with zero values on all channels.
    tlc_init(0);

    // Setup our pseudo TLC Chip Select
    TLC_INIT_CS();

    tlc_setMode( TLC_MODE_TWINKLE );

    // Enable the TLC Chip Select
    //TLC_SELECT();

    // Initialize the USART
    USART_Init( USART_BAUD_SELECT( BAUD_RATE, F_CPU ) );

    // Redirect stdout and stdin streams through the UART
    stdout = stdin = &uart_str; 

    // set up timer 0 for ms timing
    // CTC Mode. Compare and Clear
    // 16000000 / 1024 / 15 = 1041Hz -> ~1ms
    TCCR0A |= _BV(WGM01);

    // Compare A Value
    OCR0A = 15;

    // Compare Match A Interrupt Enable
    TIMSK0 |= _BV(OCIE0A);

    // Prescalar Select and Timer Enable
    // F_CPU / 1024 
    TCCR0B |= ( _BV(CS02) | _BV(CS00) );
}


/**
 *  A helper function to grab arguments from a tokenized string.
 *
 *  This assumes that 
 *
 *  returns number of arguments successfully parsed.
 */
uint8_t get_args(char* arg_str, char fmt, int num_args, int ret[]) {
    int i;
    char fmt_str[3];
    char* pchr;

    // build the format string for sscanf.
    sprintf(fmt_str, "%%%c", fmt);

    pchr = strtok( arg_str, " " );

    for( i=0; i<num_args; i++ ) {
        // no more arguments to parse.
        if ( pchr == NULL ) break;
        
        // error parsing
        if ( sscanf( pchr, fmt_str, &ret[i] ) != 1 ) break;

        pchr = strtok( NULL, " " );
    }

    return i;
} 


/**
 *  Process a command string and perform the relevant function. All relevant commands
 *  should be performed within.
 *
 *  returns 0 on success, otherwise an error code is returned. See above for error
 *  code definitions.
 */
int process_cmd(char* cmd_str) {
    int16_t args[16];
    int num_parsed;
    char* arg_str;

    uint8_t retval = CMD_SUCCESS;

    char* pchar;

    pchar = strtok( cmd_str, " " );

    if ( pchar == NULL || strlen(pchar) != 1 ) {
        printf_P( PSTR("Invalid command character: '%s'\r"), \
                pchar == NULL ? "" : pchar );
        return -1;
    }

    // If formatted correctly, the arguments (if present) start here.
    arg_str = cmd_str + sizeof(char)*2;

    switch ( pchar[0] ) {
        case 'h':
            print_usage();
            break;

        case 'm':
            // Set TLC5940 demo mode
            num_parsed = get_args(arg_str, 'c', 1, args);
            args[0] = (char)args[0];
            if( num_parsed != 1 )
                retval = CMD_PARSE_ERROR;
            else if( tlc_isValidMode( args[0] ) != TLC_VALID_MODE )
                retval = CMD_INVALID_ARGUMENT;
            else
                tlc_setMode( args[0] );
            break;

        case 'e':
            // Set Receive Echo
            num_parsed = get_args(arg_str, fmt_char, 1, args);
            if ( num_parsed != 1 )
                retval = CMD_PARSE_ERROR;
            else 
                USART_SetEcho( args[0] );
            break;

        case 's':
            // Set the value of an TLC5940 channel (or all)
            num_parsed = get_args(arg_str, fmt_char, 2, args);
            if ( num_parsed != 2 )
                retval = CMD_PARSE_ERROR;
            else if ( args[0] < -1 || args[0] >= NUM_TLCS*16
                    || args[1] < -1 || args[1] > TLC_MAXVAL ) 
                retval = CMD_INVALID_ARGUMENT;
            else {
                tlc_setMode( TLC_MODE_STATIC );
                if ( args[0] == -1 ) 
                    tlc_setAll(args[1]);
                else 
                    tlc_set(args[0], args[1]);
            }
            break;

        case 'd':
            // Set the value of modeData
            num_parsed = get_args(arg_str, fmt_char, 2, args);
            if ( num_parsed != 2 )
                retval = CMD_PARSE_ERROR;
            else
                tlc_setModeData( args[0], args[1] );

        case 't':
            // Set/Unset Pseudo TLC5940 chip select
            num_parsed = get_args(arg_str, fmt_char, 2, args);
            if ( num_parsed != 1 )
                retval = CMD_PARSE_ERROR;
            else
                tlc_cs_on = args[0];
            break;

        case 'f':
            // Set number format (used by sscanf)
            num_parsed = get_args(arg_str, 'c', 1, args);
            args[0] = (char)args[0];
            if ( num_parsed != 1 )
                retval = CMD_PARSE_ERROR;
            else if (  args[0] != 'd' \
                    && args[0] != 'o' \
                    && args[0] != 'u' \
                    && args[0] != 'x' )
                retval = CMD_INVALID_ARGUMENT;
            else
                fmt_char = args[0];
            break;

        default:
            printf_P( PSTR("Unknown command character: '%c'\r"), pchar[0] );

    }

    // clear the command
    cmd_str[0] = '\0';

    return retval;
}

int main(void)
{
    uint16_t recv_byte;
    char cmd_buf[CMD_BUF_LEN] = "";
    uint8_t cmd_buf_pos = 0;
    uint8_t cmd_retval;

    mstimer_t timer;

    setup();

    // Enable Interrupts
    sei();

    // Initialize MilliSecond timer to 30ms complete.
    mstimer_init( &timer, 30 );

    printf_P( PSTR( "Completed TLC5940 Initialization with %u chips.\r" ), NUM_TLCS);
    printf_P( PSTR( "Enter 'h' for usage information\r\r" ) );
    //print_usage();

    DDRD |= _BV(2);
    PORTD &= ~_BV(2);

    for(;;) {
        //printf("%u %u %u\r", gMilliSecTick, MS_PER_UPDATE, counter );
        recv_byte = USART_Receive( stdin );

        if ( recv_byte == CMD_TERMINATOR ) {
            cmd_buf[cmd_buf_pos] = '\0';
            cmd_retval = process_cmd( cmd_buf );
            printf("retval: %d\r", cmd_retval);
            cmd_buf_pos = 0;
        } else if ( recv_byte != USART_NO_DATA ) {
            cmd_buf[cmd_buf_pos++] = recv_byte;
        }

        if ( mstimer_completed( &timer ) ) {
            mstimer_reset( &timer ); 
            TLC_SELECT();
            tlc_updateMode();
            TLC_UNSELECT();
        }
  }
}

