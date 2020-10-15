/*
 *  The CAN command set to trigger actions or events sent from the THM Host.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */

#ifndef _CAN_COMMAND_H_
#define _CAN_COMMAND_H_

#include <inttypes.h>
#include "can.h"

// Uncomment to allow for debugging of CAN Messages over serial
#define CANCMD_SERIAL_DEBUG         

#define CANCMD_MAX_ARGS                  3

#define CANCMD_SUCCESS                   0
#define CANCMD_INVALID_ARGUMENT          1
#define CANCMD_INVALID_COMMAND           2
#define CANCMD_ARGUMENT_OUT_OF_RANGE     3

// Indices into the CAN payload 
#define CANCMD_TYPE_IDX                  0
#define CANCMD_DATA_START_IDX            2

#define CANCMD_SET_CURRENT_PATCH         0
#define CANCMD_GET_CURRENT_PATCH         1
#define CANCMD_SET_PATCH_DATA            2
#define CANCMD_GET_PATCH_DATA            3

#define CANCMD_ACK                       66
#define CANCMD_NACK                      67

#define CANCMD_REPLY_DLC                 8

#define CANCMD_UNKNOWN                   255

uint8_t cancmd_receiveMessage( void );
uint8_t cancmd_processMessage( CanMessage* rx_msg );

#endif
