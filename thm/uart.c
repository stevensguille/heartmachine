#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "uart.h"
#include "defines.h"

static volatile char rxd;

#define USART_BAUD_SELECT(baudRate, xtalCpu) ((xtalCpu + baudRate * 8L) / (baudRate * 16L) - 1)

/* size of RX/TX buffers */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1)
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1)

#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif

/*
 *  module global variables
 */
static volatile unsigned char USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_TxHead;
static volatile unsigned char USART_TxTail;
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;
static volatile unsigned char USART_LastRxError;

void USART_init( uint16_t baudrate ) {
    USART_TxHead = 0;
    USART_TxTail = 0;
    USART_RxHead = 0;
    USART_RxTail = 0;

    uint16_t ubrr = USART_BAUD_SELECT( baudrate, F_CPU ); 

    //Set baud rate 
    UBRR0 = ubrr;
    //UBRR0H = (unsigned char) ( baudrate >> 8 );
    //UBRR0L = (unsigned char) baudrate;

    // Enable receiver and transmitter and Receive Interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format: 8data, 1stop bit
    UCSR0C = (3 << UCSZ00);
}

int USART_transmit( unsigned char data, FILE *stream ) {

    unsigned char tmphead;

    tmphead = ( USART_TxHead + 1 ) & USART_TX_BUFFER_MASK;

    // wait for transmit opportunity
    while ( tmphead == USART_TxTail ) {
        // we'll buffer overflow if we try to add new data to the buffer
        // while it's full, so we block here.
        //
        // It is possible that a program may get stuck here if the USART
        // has not been initialized or if the interrupts have been disabled.
        ;
    }

    // put data into buffer and set the head pointer
    USART_TxBuf[tmphead] = data;
    USART_TxHead = tmphead;

    // enable URE interrupt for TX interrupt
    UCSR0B |= ( 1 << UDRIE0 );

    return 0;
}

int USART_receive( FILE* stream ) {
    unsigned char tmptail;
    unsigned char data;

    if ( USART_RxHead == USART_RxTail ) {
        return USART_NO_DATA;
    }

    if (UCSR0A & _BV(FE0))
        return _FDEV_EOF;

    if (UCSR0A & _BV(DOR0))
        return _FDEV_ERR;

    tmptail = ( USART_RxTail + 1) & USART_RX_BUFFER_MASK;
    USART_RxTail = tmptail;

    data = USART_RxBuf[tmptail];

    return data;
}

// Buffered Interrupt Read
ISR(USART_RX_vect) {
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;

    // read status and data
    usr  = UCSR0A;
    data = UDR0;

    // TODO: Add error status 
    tmphead = ( USART_RxHead + 1 ) & USART_RX_BUFFER_MASK;

    if ( tmphead == USART_RxTail ) {
        lastRxError = USART_BUFFER_OVERFLOW >> 8;
    } else {
        // new index for buffer
        USART_RxHead = tmphead;
        // put received data in the buffer
        USART_RxBuf[tmphead] = data;
    }

    USART_LastRxError = lastRxError;
} 

// buffered Transmit flush
ISR(USART_UDRE_vect) {
    unsigned char tmptail;

    // We still have data to send
    if ( USART_TxHead != USART_TxTail ) {
        tmptail = ( USART_TxTail + 1 ) & USART_TX_BUFFER_MASK;
        USART_TxTail = tmptail;

        // take next byte from bffer and send
        UDR0 = USART_TxBuf[tmptail];
    } else {
        // we have nothing left to send
        // disable the send buffer empty interrupt
        UCSR0B &= ~_BV(UDRIE0);
    }
}
