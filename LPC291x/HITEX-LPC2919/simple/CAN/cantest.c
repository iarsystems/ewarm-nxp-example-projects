/*****************************************************************************
 *   cantest.c:  CAN test module file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"			/* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "can.h"
#include <intrinsics.h> /* for interrupts iar */

CAN_MSG MsgBuf_TX0, MsgBuf_TX1; // TX and RX Buffers for CAN message
CAN_MSG MsgBuf_RX0, MsgBuf_RX1; // TX and RX Buffers for CAN message

volatile DWORD CAN0RxDone, CAN1RxDone;

/*****************************************************************************
** Function name:		main
**
** Descriptions:		main routine for CAN module test
**
** parameters:			None
** Returned value:		int
** 
*****************************************************************************/
int main( void )
{
  /* Please note, this PCLK is set in the target.h file. */  
  CAN_Init( BITRATE1000K80MHZ );
	
  __enable_interrupt(); // iar

  /* This test program is connect CAN0 and CAN1 ports, send one message 
  from CAN0(TX) and verify received message on CAN1(RX) if it's a match, 
  both CAN TX and RX are working. 
	
  For more details on acceptance filter program, see Philips
  appnote AN10438 and the zip file associated with this appnote. */

#if !ACCEPTANCE_FILTER_ENABLED
  // Initialize MsgBuf
  MsgBuf_TX0.Frame = 0x80080000; // 29-bit, no RTR, DLC is 8 bytes
  MsgBuf_TX0.MsgID = 0x00012345; // CAN ID
  MsgBuf_TX0.DataA = 0x3C3C3C3C;
  MsgBuf_TX0.DataB = 0xC3C3C3C3;

  MsgBuf_RX1.Frame = 0x0;
  MsgBuf_RX1.MsgID = 0x0;
  MsgBuf_RX1.DataA = 0x0;
  MsgBuf_RX1.DataB = 0x0;
  CAN_SetACCF( ACCF_BYPASS );

  /* Test bypass */
  while ( 1 )
  {
	// Transmit initial message on CAN 0
	while ( !(CAN0GSR & (1 << 3)) );
	if ( CAN0_SendMessage( &MsgBuf_TX0 ) == FALSE )
	{
	  continue;
	}
 	if ( CAN1RxDone == TRUE )
	{
	  CAN1RxDone = FALSE;
	  if ( MsgBuf_RX1.Frame & (1 << 10) )	/* by pass mode */
	  {
		MsgBuf_RX1.Frame &= ~(1 << 10 );
	  }
	  if ( ( MsgBuf_TX0.Frame != MsgBuf_RX1.Frame ) ||
			( MsgBuf_TX0.MsgID != MsgBuf_RX1.MsgID ) ||
			( MsgBuf_TX0.DataA != MsgBuf_RX1.DataA ) ||
			( MsgBuf_TX0.DataB != MsgBuf_RX1.DataB ) )
	  {
		while ( 1 );
	  }
	  // Everything is correct, reset buffer
	  MsgBuf_RX1.Frame = 0x0;
	  MsgBuf_RX1.MsgID = 0x0;
	  MsgBuf_RX1.DataA = 0x0;
	  MsgBuf_RX1.DataB = 0x0;
	} // Message on CAN 1 received
  }
#else
  /* Test Acceptance Filter */
  /* Even though the filter RAM is set for all type of identifiers,
  the test module tests explicit standard identifier only */
  MsgBuf_TX0.Frame = 0x00080000; // 11-bit, no RTR, DLC is 8 bytes
  MsgBuf_TX0.MsgID = EXP_STD_ID; // Explicit Standard ID
  MsgBuf_TX0.DataA = 0x55AA55AA;
  MsgBuf_TX0.DataB = 0xAA55AA55;

  MsgBuf_RX1.Frame = 0x0;
  MsgBuf_RX1.MsgID = 0x0;
  MsgBuf_RX1.DataA = 0x0;
  MsgBuf_RX1.DataB = 0x0;
  CAN_SetACCF( ACCF_ON );

  while ( 1 )
  {
	// Transmit initial message on CAN 0
	while ( !(CAN0GSR & (1 << 3)) );
	if ( CAN0_SendMessage( &MsgBuf_TX0 ) == FALSE )
	{
	  continue;
	}

	/* please note: FULLCAN identifier will NOT be received as it's not set 
	in the acceptance filter. */
 	if ( CAN1RxDone == TRUE )
	{
	  CAN1RxDone = FALSE;
	  /* The frame field is not checked, as ID index varies based on the
	  entries set in the filter RAM. */
	  if ( ( MsgBuf_TX0.MsgID != MsgBuf_RX1.MsgID ) ||
			( MsgBuf_TX0.DataA != MsgBuf_RX1.DataA ) ||
			( MsgBuf_TX0.DataB != MsgBuf_RX1.DataB ) )
	  {
		while ( 1 );
	  }
	  // Everything is correct, reset buffer
	  MsgBuf_RX1.Frame = 0x0;
	  MsgBuf_RX1.MsgID = 0x0;
	  MsgBuf_RX1.DataA = 0x0;
	  MsgBuf_RX1.DataB = 0x0;
	} // Message on CAN 1 received
  }
#endif
  return ( 0 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
