#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#define __delay_cycles(n)     __builtin_avr_delay_cycles(n)

/*  now include touch api.h with the localization defined above */

#include "defines.h"

#include "touch_qt_config.h"
#include "touch_api.h"

#include "qtouch.h"
#include "uart.h"
#include "mstimer.h"

/* Output can be observed in the watch window using this pointer */
qt_touch_lib_measure_data_t *pqt_measure_data = &qt_measure_data;

// Setup the USART access via streams.
FILE uart_str = FDEV_SETUP_STREAM(USART_transmit, USART_receive, _FDEV_SETUP_RW);

void update_qtouch( void );

int main( void )
{
    mstimer_t         measurement_timer;
    /*status flags to indicate the re-burst for library*/

    uint8_t           channel_num;
    uint16_t          channel_signal;
    uint16_t          channel_reference;
    unsigned int      channel_status;
    uint16_t*         channel_deltas;
    int16_t           avg_deltas[QT_NUM_KEYS];

    uint8_t           measurement_count;

    qt_lib_siginfo_t  lib_info;

    // UART initialization
    USART_init( 57600 );
    sei();

    /* initialise host app, pins, watchdog, etc */
    qt_init();

    /* configure timer ISR to fire regularly */
    mstimer_initTimer();

    // Fire every millisecond
    mstimer_init( &measurement_timer, qt_measurement_period_msec );

    /*  Address to pass address of user functions   */
    /*  This function is called after the library has made capacitive measurements,
    *   but before it has processed them. The user can use this hook to apply filter
    *   functions to the measured signal values.(Possibly to fix sensor layout faults)    */
    qt_filter_callback = 0;

        // Assign stdout and stdin to the USART stream.
    stdout = stdin = &uart_str;

    /* enable interrupts */

    qt_get_library_sig( &lib_info );

    printf( "QTouch Test:\r" );
    printf( "Library Version: %d.%d.%d\r", (lib_info.library_version >> 8), \
            (lib_info.library_version >> 4) & 0xf, \
            (lib_info.library_version) & 0xf );

    printf( "Ticks per ms: %u\r", TICKS_PER_MS );
    printf( "Measurement Period ms: %d\r", qt_measurement_period_msec );
    printf( "OCR Value: %u\r", (uint16_t)TICKS_PER_MS * qt_measurement_period_msec );

    printf( "Defines:\r" );
    printf( "QT_NUM_CHANNELS: %d\r", QT_NUM_CHANNELS );
    printf( "QT_DELAY_CYCLES: %d\r", QT_DELAY_CYCLES );

    // Main Loop
    for( ; ; )
    {
        if ( mstimer_completed( &measurement_timer ) ) {
            measurement_count++;
            qt_update();
            mstimer_reset( &measurement_timer );

            channel_deltas = qt_getLastMeasurement();

            for ( channel_num = 0; channel_num < QT_NUM_KEYS; channel_num++ ) {
                channel_signal = pqt_measure_data->channel_signals[channel_num];
                channel_reference = pqt_measure_data->channel_references[channel_num];
                channel_status = GET_SENSOR_STATE(channel_num); 

                printf("%d: (d,m,r,s) (%d,%d,%d,%d)\r", 
                        channel_num,      
                        channel_deltas[channel_num],     
                        channel_signal,  
                        channel_reference,
                        channel_status);
            }

            if( !(measurement_count % QT_HISTORY_LENGTH) ) {
                qt_getAverageMeasurement( avg_deltas, QT_HISTORY_LENGTH );
                printf("AVG: ");
                for ( channel_num = 0; channel_num < QT_NUM_KEYS; channel_num++ ) {
                    printf("%d: %d,", channel_num, avg_deltas[channel_num]);
                }
                printf("\r");

                measurement_count = 0;
            }

        }
    }
}


