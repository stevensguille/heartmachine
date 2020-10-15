/**
 *  This module contains variables and functions it utilize the QTouch
 *  library in a more unified manner than that provided by the QTouch API.
 *
 *  Only keys are considered.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "defines.h"
#include "touch_api.h"
#include "qtouch.h"
#include "utility.h"

// Used by qtouch lib for drift compensation.
static volatile uint16_t qt_current_time_ms_touch = 25u;

/**
 * Calls all required initialization functions for the QTouch library.
 */
void qt_init( threshold_t touch_threshold ) {
    // We're using a 25 millisecond measurment interval
    qt_measurement_period_msec = 25;

    qt_history_index = 0;

    qt_config_keys( touch_threshold );
    qt_init_sensing();
    // Parameter setting comes after initialization?
    qt_set_default_parameters();

    // Disable pull-ups
    MCUCR |= _BV(PUD); // (1u << PUD);
}

/**
 * Set all QTouch configuration parameters to their default.
 */
void qt_set_default_parameters( void ) {
    // This will be modified by the user to different valu
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
    qt_config_data.qt_pos_recal_delay = DEF_QT_POS_RECAL_DELAY;
}

/**
 *  Configure the QTOUCH 'keys' 
 */
void qt_config_keys( threshold_t touch_threshold )
{
    // Maybe we can read out the damn calibration values from eeprom and
    // be able to set them from CAN/SERIAL

    //  enable sensors 0..3: keys on channels 0..3 with 6.25% hysteresis
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, touch_threshold, HYST_6_25 ); // PORTC0/1

    // NOW WITH ONLY ONE KEY!

//    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 50u, HYST_6_25 ); // PORTC2/3
//    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 50u, HYST_6_25 ); // PORTC4/5

    // We are only using 3 keys for the heartmachine. Yea it's hacky.
    //qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );
}

/**
 *  This function is to be called every qt_measurement_period_msec milliseconds
 */
void qt_update() {
    uint8_t channel_num;
    int8_t i;
     
    qt_current_time_ms_touch += qt_measurement_period_msec;

    do {
        qt_last_status = qt_measure_sensors ( qt_current_time_ms_touch );
        i++;
    } while ( qt_last_status & QTLIB_BURST_AGAIN );

    //printf_P( PSTR("Bursted %d times\n\r"), i);

    qt_history_index = ( qt_history_index + 1 ) % QT_HISTORY_LENGTH;

    for ( channel_num=0; channel_num < QT_NUM_KEYS; channel_num++) {
        qt_delta_values[ qt_history_index ][ channel_num ] =
            qt_get_sensor_delta( channel_num );
    }
}


/**
 *  Returns the last measured qtouch delta values
 */
int16_t* qt_getLastMeasurement() {
    return qt_delta_values[qt_history_index];
}


/**
 *  Returns the average of the last num_to_avg qtouch delta values
 */
void qt_getAverageMeasurement( int16_t result[] ) {
    uint8_t i, j;

    // Clear the contents
    for ( i = 0; i < QT_NUM_KEYS; i++) result[i] = 0;

    for ( i = 0; i < QT_HISTORY_LENGTH; i++ ) {
        for ( j = 0; j < QT_NUM_KEYS; j++) {
            result[j] += qt_delta_values[ ( qt_history_index+i ) % QT_HISTORY_LENGTH ][j];
        }
    } 

    for ( i = 0; i < QT_NUM_KEYS; i++)
        result[i] = result[i] / QT_HISTORY_LENGTH;
}
