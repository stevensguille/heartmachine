/******************************************************************************
 * 
 * Controller Area Network (CAN) Demo-Application
 * Atmel AVR with Microchip MCP2515 
 * 
 * Copyright (C) 2005 Martin THOMAS, Kaiserslautern, Germany
 * <eversmith@heizung-thomas.de>
 * http://www.siwawi.arubi.uni-kl.de/avr_projects
 *
 *****************************************************************************
 *
 * File    : can.c
 * Version : 0.9
 * 
 * Summary : Interface between application and CAN-Hardware
 *           ("middle tier", "abstraction layer")
 *
 *****************************************************************************/

#include <string.h>
#include <stdio.h>

#include "can.h"
#include "mcp2515.h"
#include "mstimer.h"
#include "thm_node.h"

#if (CANDEBUG)
#include "uart.h"
#endif

/*extern*/ uint8_t gCANAutoProcess = CANAUTOPROCESS;

static mstimer_t timeout_timer;

uint8_t can_init(uint8_t speedset, node_id_t *can_id)
{
	uint8_t res;
	
	res = mcp2515_init(speedset, can_id->id );

    mstimer_init( &timeout_timer, CANSENDTIMEOUT );
	
	if (res == MCP2515_OK) return CAN_OK;
	else return CAN_FAILINIT;
}

void can_initMessageStruct(CanMessage* msg, node_id_t* can_id)
{
	memset(msg,0,sizeof(CanMessage));
}

uint8_t can_sendMessage(const CanMessage* msg)
{
	uint8_t res, txbuf_n;
	uint8_t timeout = 0;

	//time = timebase_actTime();
    mstimer_reset( &timeout_timer );
	
	do {
		res = mcp2515_getNextFreeTXBuf(&txbuf_n); // info = addr.
		if (mstimer_completed( &timeout_timer ) )
            timeout = 1;
	} while (res == MCP_ALLTXBUSY && !timeout);
	
	if (!timeout) {
		mcp2515_write_canMsg( txbuf_n, msg);
		mcp2515_start_transmit( txbuf_n );
		return CAN_OK;
	}
	else {

#if (CANDEBUG)
		printf("Transmit timeout\n\r");
#endif

		return CAN_FAILTX;
	}
}

uint8_t can_readMessage(CanMessage *msg)
{
	uint8_t stat, res;
	
	stat = mcp2515_readStatus();
	
	if ( stat & MCP_STAT_RX0IF ) {
		// Msg in Buffer 0
		mcp2515_read_canMsg( MCP_RXBUF_0, msg);
		mcp2515_modifyRegister(MCP_CANINTF, MCP_RX0IF, 0);
		res = CAN_OK;
	}
	else if ( stat & MCP_STAT_RX1IF ) {
		// Msg in Buffer 1
		mcp2515_read_canMsg( MCP_RXBUF_1, msg);
		mcp2515_modifyRegister(MCP_CANINTF, MCP_RX1IF, 0);
		res = CAN_OK;
	}
	else {
		res = CAN_NOMSG;
	}	
	
	return res;
}

/* returns either 
 #define CAN_MSGAVAIL   (3) - a message has been received
 #define CAN_NOMSG      (4) - no new message
*/
uint8_t can_checkReceive(void)
{
	uint8_t res;
	
	res = mcp2515_readStatus(); // RXnIF in Bit 1 and 0
	if ( res & MCP_STAT_RXIF_MASK ) {
		return CAN_MSGAVAIL;
	}
	else {
		return CAN_NOMSG;
	}
}

/* checks Controller-Error-State, returns CAN_OK or CAN_CTRLERROR 
   only errors (and not warnings) lead to "CTRLERROR" */
uint8_t can_checkError(void)
{
	uint8_t eflg = mcp2515_readRegister(MCP_EFLG);

	if ( eflg & MCP_EFLG_ERRORMASK ) {
		return CAN_CTRLERROR;
	}
	else {
		return CAN_OK;
	}
}

#if (CANDEBUG)

void can_debug1(void)
{
	uint8_t i;
	// CanMessage msg;
	
	printf("CAN-DEBUG 1\n\r");
	i = mcp2515_readRegister(MCP_CANCTRL);
	printf("CANCTRL = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_CANSTAT);
	printf("CANSTAT = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_CNF1);
	printf("CNF1    = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_CNF2);
    printf("CNF2    = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_CNF3);
	printf("CNF3    = 0x%02x\n\r", i);
	
#if CANUSELOOP == 1
	printf("Setting Loopback-Mode - ");
	if ( mcp2515_setCANCTRL_Mode(MODE_LOOPBACK) == MCP2515_OK) {
		printf("OK\n\r");
	}
	else {
		printf("failed\n\r");
	}
#else
    printf("Setting Normal-Mode - ");
	if ( mcp2515_setCANCTRL_Mode(MODE_NORMAL) == MCP2515_OK) {
		printf("OK\n\r");
	}
	else {
		printf("failed\n\r");
	}
#endif 
	
	i = mcp2515_readRegister(MCP_CANCTRL);
	//printf("CANCTRL  = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_RXB0CTRL);
	//printf("RXB0CTRL = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_RXB1CTRL);
	//printf("RXB1CTRL = 0x%02x\n\r", i);
}

void can_dumpMessage(CanMessage *msg)
{
	uint8_t i;

	printf("\nMessage-Dump :\n\r");
	// identifier type 0=standard 1=extended:
	if ( (msg->extended_identifier) == CAN_EXTID ) {
		printf("Extended");
	}
	else {
		printf("Standard");
	}
	printf("-Identifier :\n\r");
	printf("Byte 0 = 0x%02x\n\r", (uint8_t) ( (msg->identifier) ));
    printf("Byte 1 = 0x%02x\n\r", (uint8_t) ( (msg->identifier) >> 8 ));
	if ( (msg->extended_identifier) == CAN_EXTID ) {
		printf("Byte 2 = 0x%02x\n\r", (uint8_t) ( (msg->identifier) >> 16 ));
		printf("Byte 3 = 0x%02x\n\r", (uint8_t) ( (msg->identifier) >> 24 ));
	}
	
	// data length:
	printf("Data :\n\r");
	printf( "dlc = 0x%02x", msg->dlc );
	for (i=0;i<msg->dlc;i++) {
		printf( "dta[%d] = 0x%02x", i, msg->dta[i] );
	}
	
	printf("Meta:\n\r");
	// used for receive only:
	// Received Remote Transfer Bit 
	//  (0=no... 1=remote transfer request received)
    printf( "rtr = %d\n\r", msg->rtr );

	// Acceptence Filter that enabled the reception
	printf( "filhit = 0x02%x\n\r", msg->filhit );
}

void can_dumpStatus(void)
{
	mcp2515_dumpExtendedStatus();
}

#endif /* CANDEBUG */
