/**
 *  This module contains variables and functions it utilize the QTouch
 *  library in a more unified manner than that provided by the QTouch API.
 *
 *  Only keys are considered.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#ifndef _QTOUCH_H_
#define _QTOUCH_H_

#include <inttypes.h>
#include "touch_api.h"

// If QT_NUM_KEYS > 4, you'll need to link in another lib
#define QT_NUM_KEYS         1
#define QT_HISTORY_LENGTH   8

// These are provided by the QTouch Library
extern int16_t qt_get_sensor_delta( uint8_t sensor);
extern qt_touch_lib_config_data_t qt_config_data;
extern qt_touch_lib_measure_data_t qt_measure_data;

#define QT_SENSOR_TOUCHED(SENSOR_NUMBER) \
    qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] \
    & (1 << (SENSOR_NUMBER % 8))

int16_t  qt_delta_values[QT_HISTORY_LENGTH][QT_NUM_KEYS];
uint8_t  qt_history_index;
uint16_t qt_measurement_period_msec;

uint16_t qt_last_status;

void qt_init( threshold_t touch_threshold );
void qt_set_default_parameters( void );
void qt_config_keys( threshold_t touch_threshold );
void qt_update( void );

int16_t* qt_getLastMeasurement( void );
void qt_getAverageMeasurement( int16_t result[] );

#endif

