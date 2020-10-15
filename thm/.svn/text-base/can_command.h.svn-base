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
// #define CANCMD_SERIAL_DEBUG         

#define CANCMD_MAX_ARGS                  3

#define CANCMD_SUCCESS                   0
#define CANCMD_INVALID_ARGUMENT          1
#define CANCMD_INVALID_COMMAND           2
#define CANCMD_ARGUMENT_OUT_OF_RANGE     3

// Indices into the CAN payload 
#define CANCMD_TYPE_IDX                  0
#define CANCMD_DATA_START_IDX            2

#define CANCMD_RESET					0
#define CANCMD_SET_VISUAL_PATCH			1
#define CANCMD_GET_VISUAL_PATCH			2
#define CANCMD_SET_VISUAL_PARAM         3
#define CANCMD_GET_VISUAL_PARAM			4
#define CANCMD_SET_CONTROL_PATCH		5
#define CANCMD_GET_CONTROL_PATCH		6
#define CANCMD_SET_CONTROL_PARAM        7
#define CANCMD_GET_CONTROL_PARAM		8
#define CANCMD_UPDATE            		9
#define CANCMD_SYNC            			10
#define CANCMD_TEST1					11
#define CANCMD_TEST2					12
#define CANCMD_ACTIVATE_CONTROL			13
#define CANCMD_DEACTIVATE_CONTROL		14
#define CANCMD_SET_DEFAULT_PATCH        15
#define CANCMD_SET_DEFAULT_CPATCH       16
#define CANCMD_HELLO            		69 // Sent on Boot

#define CANCMD_ACK                       66
#define CANCMD_NACK                      67

#define CANCMD_REPLY_DLC                 8

#define CANCMD_UNKNOWN                   255

uint8_t cancmd_receiveMessage( void );
uint8_t cancmd_processMessage( CanMessage* rx_msg );
void cancmd_sendTouchMessage(node_id_t *node, uint16_t threshold);
uint8_t cancmd_sayHello( node_id_t* node );

#endif
