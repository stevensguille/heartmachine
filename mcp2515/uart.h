#ifndef _USART_H_
#define _USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static volatile char rxd;

#define USART_RX_BUFFER_SIZE 32
#define USART_TX_BUFFER_SIZE 32

#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
  #error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
  #error TX buffer size is not a power of 2
#endif

// Macro helper for putting strings into program memory
#ifndef P
#define P(s) ({static const char c[] __attribute__ ((progmem)) = s;c;})
#endif

/* 
** high byte error return code of USART_getc()
*/
#define USART_FRAME_ERROR      0x0800 // Framing Error by UART
#define USART_OVERRUN_ERROR    0x0400 // Overrun condition by UART
#define USART_BUFFER_OVERFLOW  0x0200 // receive ringbuffer overflow
#define USART_NO_DATA          0x0100 // no receive data available

extern void USART_init( unsigned int baudrate );
extern int USART_transmit( const unsigned char data, FILE* stream );

#define USART_puts_P(__s)       USART_puts_p(P(__s))

extern int USART_receive( FILE* stream ); 

#endif
