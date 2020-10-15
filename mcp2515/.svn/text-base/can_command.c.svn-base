/**
 *  The CAN command set to trigger actions or events sent from the THM Host.
 *
 *  Each CAN Command follows the following format, using a CAN Message with
 *  a standard 11-bit Identifier.
 *
 *  ID <8:10> Artery ID
 *     <0:7>  Node ID
 *
 *  Data Length DLC = 8
 *
 *  DTA[0]    Command Type
 *  DTA[1]    Unused
 *  DTA[2:3]  Payload Byte 0
 *  DTA[4:5]  Payload Byte 1
 *  DTA[6:7]  Payload Byte 2
 *
 *  Each Payload byte is unpacked as an int16_t although it can be cast to
 *  whatever type you wish.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#include "can_command.h"
#include "can.h"
#include "patches.h"
#include "thm_node.h"

static CanMessage cancmd_ReplyMsg;

void cancmd_init( void );
static void cancmd_dumpRawMessage( CanMessage* rx_msg );
static void cancmd_dumpTHMMessage( CanMessage* rx_msg );
static int16_t* cancmd_extractArgs( uint8_t* can_data );
static void cancmd_setData( uint8_t* can_data, uint8_t index, int16_t value );

/**
 *  Initialize the static ACK and NACK can messages.
 *
 *  Hmm. Should the ID be for the host then? No, because
 *  then the host wouldn't know who it came from. Keep the
 *  node_id from the node sender. Host can tell who sent
 *  from there.
 *
 */
void cancmd_init() {
    can_initMessageStruct( &cancmd_ReplyMsg, &node_id );

    cancmd_ReplyMsg.extended_identifier     = CAN_STDID;
    cancmd_ReplyMsg.identifier              = node_id.id;
    cancmd_ReplyMsg.dlc                     = CANCMD_REPLY_DLC;
    cancmd_ReplyMsg.dta[ CANCMD_TYPE_IDX ]  = CANCMD_ACK;
}


#if ( defined(CANCMD_SERIAL_DEBUG) && defined(SERIAL_CONSOLE))
static void cancmd_dumpRawMessage( CanMessage* rx_msg ) {
    int i;

    printf( "id: %04x dlc: %u dta: ",
        (unsigned int)rx_msg->identifier, rx_msg->dlc );

    for ( i=0; i < rx_msg->dlc; i++ ) {
        printf( "%02x ", rx_msg->dta[i]);  
    }
    printf("\n\r");
}

static void cancmd_dumpTHMMessage( CanMessage* rx_msg ) {
    int16_t* data;
    int i;

    printf( "ArteryID: %u NodeID: %u Type: %x Data: ",
        (uint8_t)((rx_msg->identifier >> 8) & 0x07),
        (uint8_t)(rx_msg->identifier & 0xff),
        rx_msg->dta[ CANCMD_TYPE_IDX ] );

    data = cancmd_extractArgs( rx_msg->dta );

    for ( i=0; i < CANCMD_MAX_ARGS; i++ ) {
        printf( "%d ", data[i]);  
    }
    printf("\n\r");
}
#endif

/**
 *  Utility function to extract the data contained int 
 *
 *  We assume each CAN message has a dlc of at least 1 where the first
 *  index is the command 'type' and subsequent bytes are 16-bit int's packed
 *  as pairs of two bytes, little-endian
 *
 *  data_index < 3
 */
static int16_t* cancmd_extractArgs( uint8_t* can_data ) {
    return (int16_t*)&(can_data[ CANCMD_DATA_START_IDX ]);
}


/**
 *  Utility Function to set the data for an outgoing message.
 */
static void cancmd_setData( uint8_t* can_data, uint8_t index, int16_t value ) {
    can_data[ CANCMD_DATA_START_IDX+(index*2)   ] = (value & 0xff);
    can_data[ CANCMD_DATA_START_IDX+(index*2)+1 ] = ( ( value >> 8 ) & 0xff );
}

/**
 *  Check if a message is available on the CAN bus for this node and process
 *  it if present. 
 */
uint8_t cancmd_receiveMessage( ) {
    CanMessage rx_msg;
    uint8_t read_result;
    uint8_t retval;

    uint8_t command_type;

    read_result = can_readMessage( &rx_msg );
    command_type = rx_msg.dta[ CANCMD_TYPE_IDX ];

    switch ( read_result ) {
        case CAN_OK:
            // We got a command so do something you lazy bum.
            thm_setRXIndicator();
            retval = cancmd_processMessage( &rx_msg );
            thm_clearRXIndicator();
            break;

        case CAN_NOMSG: // No message is actually in receive buffer.
        default: // Unknown result from read Message. Do nothing
            retval = 0;
            break;
    }

    return read_result;
}


/**
 *  Process a CAN message from the host and perform the associated action.
 *
 *  We assume each CAN message has a dlc of at least 1 where the first
 *  index is the command 'type' and subsequent bytes are 16-bit int's packed
 *  as pairs of two bytes little-endian.
 *
 *  dta[1] = LSB
 *  dta[2] = MSB
 *
 */ 
uint8_t cancmd_processMessage( CanMessage* rx_msg ) {
    uint8_t retval;
    uint8_t command_type;
    int16_t* args; // we are assuming that the data is little-endian packed.

    retval = CANCMD_SUCCESS;

    // Extract the Command Type and the 
    command_type = rx_msg->dta[ CANCMD_TYPE_IDX ];
    args = cancmd_extractArgs( rx_msg->dta );

#if ( defined(CANCMD_SERIAL_DEBUG) && defined(SERIAL_CONSOLE))
    printf("CAN Message Received:\n\r");
    cancmd_dumpRawMessage( rx_msg );
    cancmd_dumpTHMMessage( rx_msg );
#endif

    // Do we need to clear these ?
    // cancmd_setData( cancmd_ACKMsg.dta, 0 );
    // cancmd_setData( cancmd_NACKMsg.dta, 0 );

    switch ( command_type ) {
        case CANCMD_SET_CURRENT_PATCH:
            // args[0] patch_index
            if ( !isValidPatchNum( args[0] ) ) {
                retval = CANCMD_ARGUMENT_OUT_OF_RANGE;
            } else {
                setCurrentPatch( args[0] );
            }
            break;

        case CANCMD_GET_CURRENT_PATCH:
            // Returns A Message with Current Patch index
            cancmd_setData( cancmd_ReplyMsg.dta, 0, current_patch_index ); 
            thm_setTXIndicator();
            can_sendMessage( &cancmd_ReplyMsg );
            thm_clearTXIndicator();
            break;

        case CANCMD_SET_PATCH_DATA:
            // args[0] Data Index 
            // args[1] Data Value

            if ( (args[0] < 0) || (args[0] >= THM_PATCH_NUM_DATA ) ) {
                retval = CANCMD_ARGUMENT_OUT_OF_RANGE;
            } else {
                setCurrentPatchData( args[0], args[1] );
            }
            break;

        case CANCMD_GET_PATCH_DATA:
            // args[0] Data Index

            if ( (args[0] < 0) || (args[0] >= THM_PATCH_NUM_DATA ) ) {
                retval = CANCMD_ARGUMENT_OUT_OF_RANGE;
            } else {
                cancmd_setData( cancmd_ReplyMsg.dta, 0, current_patch->data[ args[0] ] );
                thm_setTXIndicator();
                can_sendMessage( &cancmd_ReplyMsg );
                thm_clearTXIndicator();
            }
            break;

        default:
            // unknown command
            retval = CANCMD_UNKNOWN;
    }

#if ( defined(CANCMD_SERIAL_DEBUG) && defined(SERIAL_CONSOLE))
    if ( retval == CANCMD_SUCCESS ) {
        printf_P( PSTR( "Command %02x Success!\n\r"), command_type );
    } else {
        printf_P( PSTR( "Command %02x Failed!\n\r"), command_type );
        if ( retval == CANCMD_ARGUMENT_OUT_OF_RANGE ) {
            printf_P( PSTR( "One of argument %d %d %d is out of range\n\r" ),
                        args[0], args[1], args[2] );
        } else if ( retval == CANCMD_UNKNOWN ) {
            printf_P( PSTR( "Command Type Unknown\n\r" ) );
        } 
    }
#endif

    // TO BE FILLED IN WHEN COMMAND SPECIFICATION IS FLESHED OUT.
    // set_patchDATA( DATA_INDEX, DATA_VAL )
    // get_patchDATA( DATA_INDEX )
    // get_patchPROGRESS( )

    return retval;
}

