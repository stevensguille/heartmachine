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

uint8_t can_init(uint8_t speedset)
{
	uint8_t res;
	
	res = mcp2515_init(speedset);

    mstimer_init( &timeout_timer, CANSENDTIMEOUT );
	
	if (res == MCP2515_OK) return CAN_OK;
	else return CAN_FAILINIT;
}

void can_initMessageStruct(CanMessage* msg)
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
		printf_P( PSTR("Transmit timeout\n\r"));
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

uint8_t can_testTransmit(const uint8_t ext, const uint8_t data2)
{
	CanMessage msg;
	
	can_initMessageStruct(&msg);
	msg.identifier = CANDEFAULTIDENT;
	msg.extended_identifier = ext;
	msg.dlc = 3;
	msg.dta[0] = 'M';
	msg.dta[1] = 'T';
	msg.dta[2] = data2;
	
	return can_sendMessage(&msg);
}

void can_debug1(void)
{
	uint8_t i;
	// CanMessage msg;
	
	printf_P( PSTR("CAN-DEBUG 1\n\r"));
	i = mcp2515_readRegister(MCP_CANCTRL);
	printf_P( PSTR("CANCTRL = 0x%02x\n\r"), i);
	i = mcp2515_readRegister(MCP_CANSTAT);
	printf_P( PSTR("CANSTAT = 0x%02x\n\r"), i);
	i = mcp2515_readRegister(MCP_CNF1);
	printf_P( PSTR("CNF1    = 0x%02x\n\r"), i);
	i = mcp2515_readRegister(MCP_CNF2);
    printf_P( PSTR("CNF2    = 0x%02x\n\r"), i);
	i = mcp2515_readRegister(MCP_CNF3);
	printf_P( PSTR("CNF3    = 0x%02x\n\r"), i);
	
#if CANUSELOOP == 1
	printf_P( PSTR("Setting Loopback-Mode - "));
	if ( mcp2515_setCANCTRL_Mode(MODE_LOOPBACK) == MCP2515_OK) {
		printf_P( PSTR("OK\n\r");
	}
	else {
		printf_P( PSTR("failed\n\r"));
	}
#else
    printf_P( PSTR("Setting Normal-Mode - "));
	if ( mcp2515_setCANCTRL_Mode(MODE_NORMAL) == MCP2515_OK) {
		printf_P( PSTR("OK\n\r"));
	}
	else {
		printf_P( PSTR("failed\n\r"));
	}
#endif 
	
	i = mcp2515_readRegister(MCP_CANCTRL);
	//printf_P( PSTR("CANCTRL  = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_RXB0CTRL);
	//printf_P( PSTR("RXB0CTRL = 0x%02x\n\r", i);
	i = mcp2515_readRegister(MCP_RXB1CTRL);
	//printf_P( PSTR("RXB1CTRL = 0x%02x\n\r", i);
}

void can_dumpMessage(CanMessage *msg)
{
	uint8_t i;

	printf_P( PSTR("\nMessage-Dump :\n\r"));
	// identifier type 0=standard 1=extended:
	if ( (msg->extended_identifier) == CAN_EXTID ) {
		printf_P( PSTR("Extended"));
	}
	else {
		printf_P( PSTR("Standard"));
	}
	printf_P( PSTR("-Identifier :\n\r"));
	printf_P( PSTR("Byte 0 = 0x%02x\n\r"), (uint8_t) ( (msg->identifier) ));
    printf_P( PSTR("Byte 1 = 0x%02x\n\r"), (uint8_t) ( (msg->identifier) >> 8 ));
	if ( (msg->extended_identifier) == CAN_EXTID ) {
		printf_P( PSTR("Byte 2 = 0x%02x\n\r"), (uint8_t) ( (msg->identifier) >> 16 ));
		printf_P( PSTR("Byte 3 = 0x%02x\n\r"), (uint8_t) ( (msg->identifier) >> 24 ));
	}
	
	// data length:
	printf_P( PSTR("Data :\n\r"));
	printf_P( PSTR( "dlc = 0x%02x"), msg->dlc );
	for (i=0;i<msg->dlc;i++) {
		printf_P( PSTR( "dta[%d] = 0x%02x", i, msg->dta[i] );
	}
	
	printf_P( PSTR("Meta:\n\r"));
	// used for receive only:
	// Received Remote Transfer Bit 
	//  (0=no... 1=remote transfer request received)
    printf_P( PSTR( "rtr = %d\n\r"), msg->rtr );

	// Acceptence Filter that enabled the reception
	printf_P( PSTR( "filhit = 0x02%x\n\r"), msg->filhit );
}

void can_dumpStatus(void)
{
	mcp2515_dumpExtendedStatus();
}

#endif /* CANDEBUG */
