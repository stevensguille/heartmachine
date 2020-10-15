#include <stdio.h>
#include <stdlib.h>
#include "can.h"

/**
 * Prints out all 
 */
void dump_testMessage( const CanMessage* msg ) {
    uint8_t i;
    printf("id: ");
    if ( msg->extended_identifier == CAN_EXTID )
        printf("%02x %02x ",
            (uint8_t)(msg->identifier >> 24),
            (uint8_t)(msg->identifier >> 16));

    printf("%02x %02x ext_id: %s dlc: %u ",
        (uint8_t)(msg->identifier >> 8),
        (uint8_t)(msg->identifier),
        (msg->extended_identifier == CAN_EXTID ? "ext" : "std"),
        msg->dlc);

    printf("%u ", (uint16_t)(msg->dta[0] << 8) | (uint16_t)(msg->dta[1]));

    for( i=2; i < msg->dlc; i++ ) {
        printf("%c ", msg->dta[i]);
    }

    printf("\n\r");
}
