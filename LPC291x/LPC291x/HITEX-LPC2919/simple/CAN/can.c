/*****************************************************************************
 *  can.c:  CAN module API file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.13  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC29xx.h"					/* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "can.h"

// Receive Queue: one queue for each CAN port
extern CAN_MSG MsgBuf_RX0, MsgBuf_RX1;
extern volatile DWORD CAN0RxDone, CAN1RxDone;

DWORD CAN0RxCount = 0, CAN1RxCount = 0;
DWORD CAN0TxCount = 0, CAN1TxCount = 0;
DWORD CAN0ErrCount = 0, CAN1ErrCount = 0;

/******************************************************************************
** Function name:		CAN_ISR_Rx0
**
** Descriptions:		CAN Rx0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void CAN_ISR_Rx0( void )
{
  DWORD * pDest;

  // initialize destination pointer
  pDest = (DWORD *)&MsgBuf_RX0;
  *pDest = CAN0RFS;  // Frame

  pDest++;
  *pDest = CAN0RID; // ID		//change by gongjun

  pDest++;
  *pDest = CAN0RDA; // Data A

  pDest++;
  *pDest = CAN0RDB; // Data B
	
  CAN0RxDone = TRUE;
  CAN0CMR = 0x04; // release receive buffer
  return;
}

/******************************************************************************
** Function name:		CAN_ISR_Rx1
**
** Descriptions:		CAN Rx1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void CAN_ISR_Rx1( void )
{
  DWORD *pDest;

  /* initialize destination pointer */
  pDest = (DWORD *)&MsgBuf_RX1;
  *pDest = CAN1RFS;  /* Frame */

  pDest++;
  *pDest = CAN1RID; /* ID */

  pDest++;
  *pDest = CAN1RDA; /* Data A */

  pDest++;
  *pDest = CAN1RDB; /* Data B */

  CAN1RxDone = TRUE;
  CAN1CMR = 0x04; /* release receive buffer	*/
  return;
}

/*****************************************************************************
** Function name:		CAN_Handler
**
** Descriptions:		CAN interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void CAN_Handler(void) 
{
  DWORD CANRxStatus, CANTxStatus;
  		
  CANRxStatus = CAN_RX_SR;
  if ( CANRxStatus & (0x1 << 8) )	/* RBS0 receive buffer status */
  {
	CAN0RxCount++;
	CAN_ISR_Rx0();
  }
  if ( CANRxStatus & (0x1 << 9) )	/* RBS1 receive buffer status */
  {
	CAN1RxCount++;
	CAN_ISR_Rx1();
  }

  CANTxStatus = CAN_TX_SR;
  if ( CANTxStatus & (0x1 << 0) )	/* TS0 TX status */
  {
	CAN0TxCount++;
  }
  if ( CANTxStatus & (0x1 << 1) )	/* TS1 TX status */
  {
	CAN1TxCount++;
  }

  if ( CAN0GSR & (0x1 << 6) )	/* Error status on CAN 0 */
  {
	/* The error count includes both TX and RX */
	CAN0ErrCount = (CAN0GSR >> 16);
  }
  if ( CAN1GSR & (0x1 << 6) )	/* Error status on CAN 1 */
  {
	/* The error count includes both TX and RX */
	CAN1ErrCount = (CAN1GSR >> 16);
  }
  return;
}

/******************************************************************************
** Function name:		CAN_Init
**
** Descriptions:		Initialize CAN, install CAN interrupt handler
**
** parameters:			bitrate
** Returned value:		true or false, false if initialization failed.
** 
******************************************************************************/
DWORD CAN_Init( DWORD can_btr )
{
  CAN0RxDone = CAN1RxDone = FALSE;

  /* CAN0 TX and RX are enabled on pin select. */
  SFSP0_0 = (0x01<<2)|(0x02<<0);	/* Digital no PU and PD, func. 2. */
  SFSP0_1 = (0x01<<2)|(0x02<<0);

  /* CAN1 TX and RX are enabled on pin select. */
  SFSP0_24 = (0x01<<2)|(0x02<<0);	/* Digital no PU and PD, func. 2. */
  SFSP0_25 = (0x01<<2)|(0x02<<0);

  CAN0MOD = CAN1MOD = 1;	/* Reset CAN */
  CAN0IER = CAN1IER = 0;	/* Disable Receive Interrupt */
  /* in GSRs, Only bit 16 through 31 are R/W, the rest are RO. */
  CAN0GSR = CAN1GSR = 0;	/* Reset error counter when CANxMOD is in reset */

  CAN0BTR = CAN1BTR = can_btr;
  CAN0MOD = CAN1MOD = 0x0;	/* CAN in normal operation mode */

  /* Install CAN interrupt handler */
  install_irq( 36, (void *)CAN_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( 36, ACTIVE_HIGH, HIGHEST_PRIORITY );
  /* Enable receive, transmit 1~3 interrupt */
  CAN0IER = CAN1IER = (0x1<<0)|(0x1<<1)|(0x1<<9)|(0x1<<10);
  return( TRUE );
}

/******************************************************************************
** Function name:		CAN_SetACCF_Lookup
**
** Descriptions:		Initialize CAN, install CAN interrupt handler
**
** parameters:			bitrate
** Returned value:		true or false, false if initialization failed.
** 
******************************************************************************/
void CAN_SetACCF_Lookup( void )
{
  DWORD address = 0;
  DWORD i;
  DWORD ID_high, ID_low;

  /* Set explicit standard Frame */  
  CAN_SFF_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i += 2 )
  {
	ID_low = (i << 29) | (EXP_STD_ID << 16);
	ID_high = ((i+1) << 13) | (EXP_STD_ID << 0);
	*((volatile DWORD *)(CAN_AFR_BASE_ADDR + address)) = ID_low | ID_high;
	address += 4; 
  }
		
  /* Set group standard Frame */
  CAN_SFF_GRP_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i += 2 )
  {
	ID_low = (i << 29) | (GRP_STD_ID << 16);
	ID_high = ((i+1) << 13) | (GRP_STD_ID << 0);
	*((volatile DWORD *)(CAN_AFR_BASE_ADDR + address)) = ID_low | ID_high;
	address += 4; 
  }
 
  /* Set explicit extended Frame */ 
  CAN_EFF_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i++  )
  {
	ID_low = (i << 29) | (EXP_EXT_ID << 0);
	*((volatile DWORD *)(CAN_AFR_BASE_ADDR + address)) = ID_low;
	address += 4; 
  }

  /* Set group extended Frame */ 
  CAN_EFF_GRP_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i++  )
  {
	ID_low = (i << 29) | (GRP_EXT_ID << 0);
	*((volatile DWORD *)(CAN_AFR_BASE_ADDR + address)) = ID_low;
	address += 4; 
  }
   
  /* Set End of Table */
  CAN_EOT = address;
  return;
}

/******************************************************************************
** Function name:		CAN_SetACCF
**
** Descriptions:		Set acceptance filter and SRAM associated with	
**
** parameters:			ACMF mode
** Returned value:		None
**
** 
******************************************************************************/
void CAN_SetACCF( DWORD ACCFMode )
{
  switch ( ACCFMode )
  {
	case ACCF_OFF:
	  CAN_AFMR = ACCFMode;
	  CAN0MOD = CAN1MOD = 1;	/* Reset CAN */
	  CAN0IER = CAN1IER = 0;	/* Disable Receive Interrupt */
	  CAN0GSR = CAN1GSR = 0;	/* Reset error counter when CANxMOD is in reset */
	break;

	case ACCF_BYPASS:
	  CAN_AFMR = ACCFMode;
	break;

	case ACCF_ON:
	case ACCF_FULLCAN:
	  CAN_AFMR = ACCF_OFF;
	  CAN_SetACCF_Lookup();
	  CAN_AFMR = ACCFMode;
	break;

	default:
	break;
  }
  return;
}

/******************************************************************************
** Function name:		CAN0_SendMessage
**
** Descriptions:		Send message block to CAN0	
**
** parameters:			pointer to the CAN message
** Returned value:		true or false, if message buffer is available,
**						message can be sent successfully, return TRUE,
**						otherwise, return FALSE.
** 
******************************************************************************/
DWORD CAN0_SendMessage( CAN_MSG *pTxBuf )
{
  DWORD CANStatus;

  CANStatus = CAN0SR;
  if ( CANStatus & (0x1<<2) )		/* TBS1, transmit buffer status */
  {
	CAN0TFI1 = pTxBuf->Frame & 0xC00F0000;
	CAN0TID1 = pTxBuf->MsgID;
	CAN0TDA1 = pTxBuf->DataA;
	CAN0TDB1 = pTxBuf->DataB;
	CAN0CMR = 0x21;
	return ( TRUE );
  }
  else if ( CANStatus & (0x1<<10) )	/* TBS2, transmit buffer status */
  {
	CAN0TFI2 = pTxBuf->Frame & 0xC00F0000;
	CAN0TID2 = pTxBuf->MsgID;
	CAN0TDA2 = pTxBuf->DataA;
	CAN0TDB2 = pTxBuf->DataB;
	CAN0CMR = 0x41;
	return ( TRUE );
  }
  else if ( CANStatus & (0x1<<18) )	/* TBS3, transmit buffer status */
  {	
	CAN0TFI3 = pTxBuf->Frame & 0xC00F0000;
	CAN0TID3 = pTxBuf->MsgID;
	CAN0TDA3 = pTxBuf->DataA;
	CAN0TDB3 = pTxBuf->DataB;
	CAN0CMR = 0x81;
	return ( TRUE );
  }
  return ( FALSE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
