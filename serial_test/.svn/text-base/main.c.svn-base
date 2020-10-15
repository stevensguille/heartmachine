#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define USART_BAUD 57600
#define MYUBRR F_CPU/16/USART_BAUD-1

#define PD7 7
#define PD6 6

FILE uart_str = FDEV_SETUP_STREAM(USART_Transmit, USART_Receive, _FDEV_SETUP_RW);

int main(void) {
    unsigned int recv_byte;
    unsigned char a;
    int16_t blam;

    //    USART_init( MYUBRR );
    USART_init( USART_BAUD_SELECT( 57600, F_CPU ) );

    stdout = stdin = &uart_str;

    DDRB |= _BV(5);
    DDRD |= _BV(PD7) | _BV(PD6);

    // Enable Global Interrupts
    sei();
  
    for( a = 'a'; a <= 'z'; a++ ) {
        printf("%c", a);
    }
    printf("\n\r");

    for ( blam=-100; blam < 300; blam++ ) {
        printf("%d\n\r", blam);
    }

    for ( ; ; ) {

        recv_byte = USART_Receive( stdin );

        if ( recv_byte == USART_NO_DATA ) {
            continue;
        } else {
   
            // Do some stuff with it.
            switch ( (unsigned char) recv_byte ) {
                case '\r':
                    printf("\n");
                    break;
                case 'q':
                    PORTB &= ~_BV(5);
                    break;
                case 'w':
                    PORTB |= _BV(5);
                    break;
                case 'e':
                    PORTB ^= _BV(5);
                    break;
                case 't':
                    printf("\n\rTEEHEE!\n\r");
                    break;
                default:
                    break;

                // Echo the recv_byte
                printf("%c", recv_byte);
            }
        }
    }
}

