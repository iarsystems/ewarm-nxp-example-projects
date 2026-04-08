/*****************************************************************************
 *   mci.c:  SD/MMC module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <nxp/iolpc2468.h>
#include <intrinsics.h>
#include "type.h"
#include "irq.h"
#include "timer.h"
#include "mci.h"
#include "dma.h"

volatile DWORD CmdCRCErrCount = 0;
volatile DWORD CmdTimeoutErrCount = 0;
volatile DWORD CmdRespEndCount = 0;
volatile DWORD CmdSentCount = 0;
volatile DWORD CmdActiveCount = 0;

volatile DWORD DataCRCErrCount = 0;
volatile DWORD DataTimeoutErrCount = 0;
volatile DWORD DataTxUnderrunErrCount = 0;
volatile DWORD DataRxOverrunErrCount = 0;
volatile DWORD DataStartbitErrCount = 0;

volatile DWORD DataEndCount = 0;
volatile DWORD DataBlockEndCount = 0;
volatile DWORD DataTxActiveCount = 0;
volatile DWORD DataRxActiveCount = 0;

volatile DWORD DataFIFOCount = 0;

volatile DWORD CardRCA;
volatile DWORD CardType;

#if MCI_DMA_ENABLED
extern DWORD *src_addr;
extern DWORD *dest_addr;
#else
extern volatile BYTE WriteBlock[BLOCK_LENGTH], ReadBlock[BLOCK_LENGTH];
extern volatile DWORD TXBlockCounter, RXBlockCounter;
#endif

/******************************************************************************
** Function name:   MCI_Interrupt related
**
** Descriptions:    MCI interrupt handler and related APIs
**
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void MCI_TXEnable( void )
{
  MCIMASK0 |= DATA_TX_INT_MASK; /* Enable TX interrupts only */
  MCIMASK1 = MCIMASK0;
}

void MCI_TXDisable( void )
{
  MCIMASK0 &= ~DATA_TX_INT_MASK;  /* Disable TX interrupts only */
  MCIMASK1 = MCIMASK0;
}

/*****************************************************************/
void MCI_RXEnable( void )
{
  MCIMASK0 |= DATA_RX_INT_MASK; /* Enable RX interrupts only */
  MCIMASK1 = MCIMASK0;
}

void MCI_RXDisable( void )
{
  MCIMASK0 &= ~DATA_RX_INT_MASK;  /* Disable RX interrupts only */
  MCIMASK1 = MCIMASK0;
}

/******************************************************************************
** Function name:   MCI_CheckStatus
**
** Descriptions:    MCI Check status before and after the block read and
**            write. Right after the block read and write, this routine
**            is important that, even the FIFO is empty, complete
**            block has been sent, but, data is still being written
**            to the card, this routine is to ensure that the data
**            has been written based on the state of the card, not
**            by the length being set.
**
** parameters:      None
** Returned value:    TRUE or FALSE
**
******************************************************************************/
DWORD MCI_CheckStatus( void )
{
  DWORD respValue;
  while ( 1 )
  {
    if ( (respValue = MCI_Send_Status()) == INVALID_RESPONSE )
    {
      break;
    }
    else
    {
      /* The only valid state is TRANS per MMC and SD state diagram.
      RCV state may be seen, but, I have found that it happens
      only when TX_ACTIVE or RX_ACTIVE occurs before the WRITE_BLOCK and
      READ_BLOCK cmds are being sent, which is not a valid sequence. */
      if ( (respValue & (0x0F << 8)) == 0x0900 )
      {
        return ( TRUE );
      }
    }
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_CmdProcess
**
** Descriptions:    Called by MCI interrupt handler
**            To simplify the process, for card initialization, the
**            CMD interrupts are disable.
**
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void MCI_CmdProcess( void )
{
  DWORD MCIStatus;

  MCIStatus = MCISTATUS;
  if ( MCIStatus &  MCI_CMD_CRC_FAIL )
  {
    CmdCRCErrCount++;
    MCICLEAR = MCIStatus | MCI_CMD_CRC_FAIL;
  }
  if ( MCIStatus &  MCI_CMD_TIMEOUT )
  {
    CmdTimeoutErrCount++;
    MCICLEAR = MCIStatus | MCI_CMD_TIMEOUT;
  }
  /* Cmd Resp End or Cmd Sent */
  if ( MCIStatus &  MCI_CMD_RESP_END )
  {
    CmdRespEndCount++;
    MCICLEAR = MCIStatus | MCI_CMD_RESP_END;
  }
  if ( MCIStatus &  MCI_CMD_SENT )
  {
    CmdSentCount++;
    MCICLEAR = MCIStatus | MCI_CMD_SENT;
  }
  if ( MCIStatus &  MCI_CMD_ACTIVE )
  {
    CmdActiveCount++;
    MCICLEAR = MCIStatus | MCI_CMD_ACTIVE;
  }
  return;
}

/******************************************************************************
** Function name:   MCI_DataErrorProcess
**
** Descriptions:    Called by MCI interrupt handler
**            Process data error.
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void MCI_DataErrorProcess( void )
{
  DWORD MCIStatus;

  MCIStatus = MCISTATUS;
    if ( MCIStatus &  MCI_DATA_CRC_FAIL )
  {
    DataCRCErrCount++;
    MCICLEAR = MCIStatus | MCI_DATA_CRC_FAIL;
  }
  if ( MCIStatus &  MCI_DATA_TIMEOUT )
  {
    DataTimeoutErrCount++;
    MCICLEAR = MCIStatus | MCI_DATA_TIMEOUT;
  }

  /* Underrun or overrun */
  if ( MCIStatus &  MCI_TX_UNDERRUN )
  {
    DataTxUnderrunErrCount++;
    MCICLEAR = MCIStatus | MCI_TX_UNDERRUN;
  }
  if ( MCIStatus &  MCI_RX_OVERRUN )
  {
    DataRxOverrunErrCount++;
    MCICLEAR = MCIStatus | MCI_RX_OVERRUN;
  }
  /* Start bit error on data signal */
  if ( MCIStatus &  MCI_START_BIT_ERR )
  {
    DataStartbitErrCount++;
    MCICLEAR = MCIStatus | MCI_START_BIT_ERR;
  }
  return;
}

/******************************************************************************
** Function name:   MCI_DataInterruptProcess
**
** Descriptions:    Called by MCI interrupt handler
**            This is the key module processing the block write
**            read to from the card. The FIFO interrupts are not
**            handled, the routine mainly use TX_ACTIVE and RX_ACTIVE
**            interrupts to handle a block write and read.
**            There is an ugly polling right after the block write
**            and read to handle the situation that, the block has
**            been sent, but the prograamming to the card is still
**            in progress. This needs to be done when TX_ACTIVE or
**            RX_ACTIVE is still set. In the real application, during
**            the card programming, a semiphone like signaling will be
**            needed.
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void MCI_DataInterruptProcess( void )
{
  DWORD MCIStatus;
#if !MCI_DMA_ENABLED
  DWORD i, blockSize, DataWord;
#endif

  MCIStatus = MCISTATUS;
  if ( MCIStatus &  MCI_DATA_END )    /* Data end, and Data block end  */
  {
    DataEndCount++;
    MCICLEAR = MCIStatus | MCI_DATA_END;
  }
  if ( MCIStatus &  MCI_DATA_BLK_END )
  {
    DataBlockEndCount++;
    MCICLEAR = MCIStatus | MCI_DATA_BLK_END;
  }

  /* Tx active, and Rx active  */
  if ( MCISTATUS & MCI_TX_ACTIVE )
  {
    DataTxActiveCount++;
#if MCI_DMA_ENABLED
    while ( !(DMACRAWINTTCSTATUS & 0x01) );
    DMACINTTCCLEAR = 0x01;
#else
    while ( TXBlockCounter < BLOCK_LENGTH )
    {
      blockSize = 0;
      if ( MCISTATUS & MCI_TX_FIFO_EMPTY )
      {
        blockSize = FIFO_SIZE * 4;
      }
      else if ( MCISTATUS & MCI_TX_HALF_EMPTY )
      {
        blockSize = (FIFO_SIZE/2) * 4;
      }
      else if ( !(MCISTATUS & MCI_TX_FIFO_FULL) )
      {
        blockSize = 1 * 4;
      }

      for ( i = 0; i < blockSize; i += 4 )
      {
        DataWord = (DWORD)(WriteBlock[TXBlockCounter]) | (DWORD)(WriteBlock[TXBlockCounter+1] << 8)
          | (DWORD)(WriteBlock[TXBlockCounter+2] << 16) | (DWORD)(WriteBlock[TXBlockCounter+3] << 24);
        MCIFIFO0 = DataWord;
        TXBlockCounter += 4;
      }
    }
#endif
    /* Data has been written, but has not been programmed into the card yet. */
    /* Even it's a dead while loop inside the ISR, but, during the
    flash writing, there is not much else you should do. If it's not in the
    TRAN state, per MMC/SD state diagram, send STOP to bail out. */

    // wait transfer complete
    while(MCISTATUS_bit.TXACTIVE);
    MCIDATACTRL = 0;

    if ( MCI_CheckStatus() != TRUE )
    {
      MCI_Send_Stop();
    }
  }

  if ( MCISTATUS & MCI_RX_ACTIVE )
  {
    DataRxActiveCount++;
#if MCI_DMA_ENABLED
    while ( !(DMACRAWINTTCSTATUS & 0x02) );
    DMACINTTCCLEAR = 0x02;
#else
    while ( RXBlockCounter < BLOCK_LENGTH )
    {
      if ( !(MCISTATUS & MCI_RX_FIFO_FULL) )
      {
        blockSize = FIFO_SIZE * 4;
      }
      else if ( MCISTATUS & MCI_RX_HALF_FULL )
      {
        blockSize = (FIFO_SIZE/2) * 4;
      }
      else if ( MCISTATUS & MCI_RX_FIFO_EMPTY )
      {
        blockSize = 1 * 4;
      }
      else
      {
        blockSize = 0;
      }

      for ( i = 0; i < blockSize; i += 4 )
      {
        while ( !(MCISTATUS & MCI_RX_DATA_AVAIL) );
        DataWord = MCIFIFO0;
        ReadBlock[RXBlockCounter] = DataWord & 0xFF;
        ReadBlock[RXBlockCounter+1] = (DataWord >> 8) & 0xFF;
        ReadBlock[RXBlockCounter+2] = (DataWord >> 16) & 0xFF;
        ReadBlock[RXBlockCounter+3] = (DataWord >> 24) & 0xFF;
        RXBlockCounter += 4;
      }
    }
#endif

    // wait receive complete
    while(MCISTATUS_bit.RXACTIVE);

    MCIDATACTRL = 0;
    if ( MCI_CheckStatus() != TRUE )
    {
      MCI_Send_Stop();
    }
  }
  return;
}

/******************************************************************************
** Function name:   MCI_FIFOProcess
**
** Descriptions:    Called by MCI interrupt handler
**            FIFO interrupts are disabled.
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void MCI_FIFOProcess( void )
{
volatile DWORD MCIStatus;

  MCIStatus = MCISTATUS;
  DataFIFOCount++;
  return;
}

/******************************************************************************
** Function name:   MCI_Handler
**
** Descriptions:    MCI interrupt handler
**            The handler to handle the block data write and read
**            not for the commands.
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
__irq __arm  void MCI_Handler (void)
{

  /* handle MCI_STATUS interrupt */
  if ( MCISTATUS & CMD_INT_MASK )
  {
    MCI_CmdProcess();
  }
  else if ( MCISTATUS & DATA_ERR_INT_MASK )
  {
    MCI_DataErrorProcess();
  }
  else if ( MCISTATUS & DATA_INT_MASK )
  {
    MCI_DataInterruptProcess();
  }
    else if ( MCISTATUS & FIFO_INT_MASK )
  {
    MCI_FIFOProcess();
  }
  VICADDRESS = 0;   /* Acknowledge Interrupt */
}

/******************************************************************************
** Function name:   Set_MCIClock
**
** Descriptions:    Set MCI clock rate, during initialization phase < 400K
**            during data phase < 20Mhz.
**
** parameters:      Clock rate to be set
** Returned value:    None
**
******************************************************************************/
void Set_MCIClock( DWORD ClockRate )
{
DWORD i, ClkValue = 0;

  if ( ClockRate == SLOW_RATE )
  {
    ClkValue |= MCLKDIV_SLOW; /* slow clock */
  }
  else if ( ClockRate == NORMAL_RATE )
  {
    ClkValue |= MCLKDIV_NORMAL; /* normal clock */
  }
  MCICLOCK |= (1 << 8) | (1 << 9) | ClkValue;
  for ( i = 0; i < 0x10; i++ ); /* delay 3MCLK + 2PCLK before next write */
  return;
}

/******************************************************************************
** Function name:   MCI_Init
**
** Descriptions:    Set MCI clock and power registers, setup VIC for
**            data interrupt.
**
** parameters:      None
** Returned value:    true or fase, if VIC table is full, return false
**
******************************************************************************/
DWORD MCI_Init( void )
{
DWORD i;
  MCICLOCK = 0;

  if ( MCIPOWER & 0x02 )
  {
    MCIPOWER = 0x00;
  }
  for ( i = 0; i < 0x1000; i++ );

  PCONP |= ( 1 << 28 );     /* Enable clock to the MCI block */

  /* Disable all interrupts for now */
  MCIMASK0 = 0;
  MCIMASK1 = MCIMASK0;

  /*connect MCI signals to P1.2-P1.3,P1.05-P1.07 and P1.11,P1.12*/
  PINSEL2 = 0x0280A8A0;

  /*set up clocking default mode, clear any registers as needed */
  MCICOMMAND = 0;
  MCIDATACTRL = 0;
  MCICLEAR = 0x7FF;   /* clear all pending interrupts */

  MCIPOWER = 0x02;    /* power up */
  while ( !(MCIPOWER & 0x02) );
  for ( i = 0; i < 0x100; i++ );

  /* During identification phase, the clock should be less than
  400Khz. Once we pass this phase, the normal clock can be set up
  to 25Mhz on SD card and 20Mhz on MMC card. */
  Set_MCIClock( SLOW_RATE );
  MCIPOWER |= 0x01;   /* bit 1 is set already, from power up to power on */

  for ( i = 0; i < 0x2000; i++ );
  if ( install_irq( MCI_INT, (void *)MCI_Handler, HIGHEST_PRIORITY ) == FALSE )
  {
    return ( FALSE );
  }

  /* During the initialization phase, to simplify the process, the CMD related
  interrupts are disabled. The DATA related interrupts are enabled when
  the FIFOs are used and just before WRITE_BLOCK READ_BLOCK cmds are issues, and
  disabled after the data block has been written and read. Please also note,
  before WRITE_BLOCK only TX related data interrupts are enabled, and before
  READ_BLOCK only RX related data interrupts are enabled. */
  return (TRUE);
}

/******************************************************************************
** Function name:   MCI_SendCmd
**
** Descriptions:    The routine is used to send a CMD to the card
**
** parameters:      CmdIndex, Argument, ExpectResp Flag, AllowTimeout flag
** Returned value:    None
**
******************************************************************************/
void MCI_SendCmd( DWORD CmdIndex, DWORD Argument, DWORD ExpectResp, DWORD AllowTimeout )
{
DWORD i, CmdData = 0;
DWORD CmdStatus;

  /* the command engine must be disabled when we modify the argument
     or the peripheral resends */
  while ( (CmdStatus = MCISTATUS) & MCI_CMD_ACTIVE )  /* Command in progress. */
  {
    MCICOMMAND = 0;
    MCICLEAR = CmdStatus | MCI_CMD_ACTIVE;
  }

  for ( i = 0; i < 0x100; i++ );

    /*set the command details, the CmdIndex should 0 through 0x3F only */
  CmdData |= (CmdIndex & 0x3F); /* bit 0 through 5 only */
  if ( ExpectResp == EXPECT_NO_RESP )     /* no response */
  {
    CmdData &= ~((1 << 6) | (1 << 7));    /* Clear long response bit as well */
  }
  else if ( ExpectResp == EXPECT_SHORT_RESP ) /* expect short response */
  {
    CmdData |= (1 << 6);
  }
  else if ( ExpectResp == EXPECT_LONG_RESP )  /* expect long response */
  {
    CmdData |= (1 << 6) | (1 << 7);
  }

  if ( AllowTimeout )     /* allow timeout or not */
  {
    CmdData |= (1 << 8);
  }
  else
  {
    CmdData &= ~(1 << 8);
  }

  /*send the command*/
  CmdData |= (1 << 10);   /* This bit needs to be set last. */
  MCIARGUMENT = Argument; /* Set the argument first, finally command */
  MCICOMMAND = CmdData;
  return;

}

/******************************************************************************
** Function name:   MCI_GetCmdResp
**
** Descriptions:    Get response from the card. This module is always used
**            in pair with MCI_SendCmd()
**
** parameters:      Expected cmd data, expect response flag, pointer to the
**            response
**            Expected cmd data should be the same as that in SendCmd()
**            expect response flag could be EXPECT_NO_RESP
**                            EXPECT_SHORT_RESP
**                            EXPECT_LONG_RESP
**            if GetCmdResp() is 0, check the pointer to the response
**            field to get the response value, if GetCmdResp() returns
**            non-zero, no need to check the response field, just resend
**            command or bailout.
** Returned value:    Response status, 0 is valid response.
**
******************************************************************************/
DWORD MCI_GetCmdResp( DWORD ExpectCmdData, DWORD ExpectResp, DWORD *CmdResp )
{
DWORD CmdRespStatus = 0;
DWORD LastCmdIndex;

  if ( ExpectResp == EXPECT_NO_RESP )
  {
    return ( 0 );
  }

  while ( 1 )
  {
    CmdRespStatus = MCISTATUS;
    if ( CmdRespStatus & (MCI_CMD_TIMEOUT) )
    {
      MCICLEAR = CmdRespStatus | MCI_CMD_TIMEOUT;
      MCICOMMAND = 0;
      MCIARGUMENT = 0xFFFFFFFF;
      return ( CmdRespStatus );
    }
    if (  CmdRespStatus & MCI_CMD_CRC_FAIL )
    {
      MCICLEAR = CmdRespStatus | MCI_CMD_CRC_FAIL;
      LastCmdIndex = MCICOMMAND & 0x003F;
      if ( (LastCmdIndex == SEND_OP_COND) || (LastCmdIndex == SEND_APP_OP_COND)
          || (LastCmdIndex == STOP_TRANSMISSION) )
      {
        MCICOMMAND = 0;
        MCIARGUMENT = 0xFFFFFFFF;
        break;      /* ignore CRC error if it's a resp for SEND_OP_COND
                or STOP_TRANSMISSION. */
      }
      else
      {
        return ( CmdRespStatus );
      }
    }
    else if ( CmdRespStatus & MCI_CMD_RESP_END )
    {
      MCICLEAR = CmdRespStatus | MCI_CMD_RESP_END;
      break;  /* cmd response is received, expecting response */
    }
  }

  if ( (MCIRESPCMD & 0x3F) != ExpectCmdData )
  {
    /* If the response is not R1, in the response field, the Expected Cmd data
    won't be the same as the CMD data in SendCmd(). Below four cmds have
    R2 or R3 response. We don't need to check if MCI_RESP_CMD is the same
    as the Expected or not. */
    if ( (ExpectCmdData != SEND_OP_COND) && (ExpectCmdData != SEND_APP_OP_COND)
        && (ExpectCmdData != ALL_SEND_CID) && (ExpectCmdData != SEND_CSD) )
    {
      CmdRespStatus = INVALID_RESPONSE; /* Reuse error status */
      return ( INVALID_RESPONSE );
    }
  }

  if ( ExpectResp == EXPECT_SHORT_RESP )
  {
    *CmdResp = MCIRESPONSE0;
  }
  else if ( ExpectResp == EXPECT_LONG_RESP )
  {
    *CmdResp = MCIRESPONSE0;
    *(CmdResp+1) = MCIRESPONSE1;
    *(CmdResp+2) = MCIRESPONSE2;
    *(CmdResp+3) = MCIRESPONSE3;
  }
  return ( 0 ); /* Read MCI_RESP0 register assuming it's not long response. */
}

/******************************************************************************
** Function name:   MCI_Go_Idle_State
**
** Descriptions:    CMD0, the very first command to be sent to initialize
**            either MMC or SD card.
**
** parameters:      None
** Returned value:    true or false, true if card has been initialized.
**
******************************************************************************/
DWORD MCI_Go_Idle_State( void )
{
DWORD retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    /* Send CMD0 command repeatedly until the response is back correctly */
    MCI_SendCmd( GO_IDLE_STATE, 0x00000000, EXPECT_NO_RESP, 0 );
    respStatus = MCI_GetCmdResp( GO_IDLE_STATE, EXPECT_NO_RESP, (DWORD *)respValue );
    if ( respStatus == 0 )
    {
      break;
    }
    retryCount--;
  }

  if ( respStatus != 0 )    /* timeout, give up */
  {
    return ( FALSE );
  }
  return( TRUE );
}

/******************************************************************************
** Function name:   MCI_Send_OP_Cond
**
** Descriptions:    CMD1 for MMC
**
** parameters:      None
** Returned value:    true or false, true if card has response back before
**            timeout, false is timeout on the command.
**
******************************************************************************/
DWORD MCI_Send_OP_Cond( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x200;     /* reset retry counter */
  while ( retryCount > 0 )
  {
    /* Send CMD1 command repeatedly until the response is back correctly */
    MCI_SendCmd( SEND_OP_COND, OCR_INDEX, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SEND_OP_COND, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    /* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
    if ( !(respStatus & MCI_CMD_TIMEOUT) && (respValue[0] & 0x80000000) )
    {
      return ( TRUE );  /* response is back and correct. */
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_ACMD
**
** Descriptions:    CMD55, before sending an ACMD, call this routine first
**
** parameters:      None
** Returned value:    true or false, true if card has responded before timeout.
**            false is timeout.
**
******************************************************************************/
DWORD MCI_Send_ACMD( void )
{
DWORD i, retryCount;
DWORD CmdArgument;
DWORD respStatus;
DWORD respValue[4];

  if ( CardType == SD_CARD )
  {
    CmdArgument = CardRCA;  /* Use the address from SET_RELATIVE_ADDR cmd */
  }
  else      /* if MMC or unknown card type, use 0x0. */
  {
    CmdArgument = 0x00000000;
  }

  retryCount = 20;
  while ( retryCount > 0 )
  {
    /* Send CMD55 command followed by an ACMD */
    MCI_SendCmd( APP_CMD, CmdArgument, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( APP_CMD, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    if ( !respStatus && (respValue[0] & CARD_STATUS_ACMD_ENABLE) )  /* Check if APP_CMD enabled */
    {
      return( TRUE );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_ACMD_OP_Cond
**
** Descriptions:    If Send_OP_Cond is timeout, it's not a MMC card, try
**            this combination to see if we can communicate with
**            a SD card.
**
** parameters:      None
** Returned value:    true or false, true if card has been initialized.
**
******************************************************************************/
DWORD MCI_Send_ACMD_OP_Cond( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  /* timeout on SEND_OP_COND command on MMC, now, try SEND_APP_OP_COND
  command to SD */
  retryCount = 0x200;     /* reset retry counter */
  while ( retryCount > 0 )
  {
    MCIPOWER &= ~(1 << 6 ); /* Clear Open Drain output control for SD */
    for ( i = 0; i < 0x3000; i++ );

    if ( MCI_Send_ACMD() == FALSE )
    {
      continue;
    }

    /* Send ACMD41 command repeatedly until the response is back correctly */
    MCI_SendCmd( SEND_APP_OP_COND, OCR_INDEX, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SEND_APP_OP_COND, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    if ( !(respStatus & MCI_CMD_TIMEOUT) && (respValue[0] & 0x80000000) )
    {
      return ( TRUE );  /* response is back and correct. */
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return( FALSE );
}

/******************************************************************************
** Function name:   MCI_CardInit
**
** Descriptions:    Try CMD1 first for MMC, if it's timeout, try CMD55
**            and CMD41 for SD, if both failed, initialization faliure,
**            bailout with unknown card type. Otherwise, return the
**            card type, either MMC or SD.
**
** parameters:      None
** Returned value:    Card type.
**
******************************************************************************/
DWORD MCI_CardInit( void )
{
DWORD i, CardType;

  if ( MCI_Go_Idle_State() == FALSE )
  {
    return( CARD_UNKNOWN );
  }

  MCIPOWER |= (1 << 6 );    /* Set Open Drain output control for MMC */
  for ( i = 0; i < 0x3000; i++ );

  /* Try CMD1 first for MMC, if it's timeout, try CMD55 and CMD41 for SD,
  if both failed, initialization faliure, bailout. */
  if ( MCI_Send_OP_Cond() == TRUE )
  {
    CardType = MMC_CARD;
    return ( CardType );  /* Found the card, it's a MMC */
  }
  else if ( MCI_Send_ACMD_OP_Cond() == TRUE )
  {
    CardType = SD_CARD;
    return ( CardType );  /* Found the card, it's a SD */
  }
  /* tried both MMC and SD card, give up */
  return ( CARD_UNKNOWN );
}

/******************************************************************************
** Function name:   MCI_Check_CID
**
** Descriptions:    Send CMD2, ALL_SEND_CID
**
** parameters:      None
** Returned value:    If not timeout, return true.
**
******************************************************************************/
DWORD MCI_Check_CID( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  /* This command is normally after CMD1(MMC) or ACMD41(SD). */
  retryCount = 0x20;      /* reset retry counter */
  while ( retryCount > 0 )
  {
    /* Send CMD2 command repeatedly until the response is back correctly */
    MCI_SendCmd( ALL_SEND_CID, 0, EXPECT_LONG_RESP, 0 );
    respStatus = MCI_GetCmdResp( ALL_SEND_CID, EXPECT_LONG_RESP, (DWORD *)&respValue[0] );
    /* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
    if ( !(respStatus & MCI_CMD_TIMEOUT) )
    {
      return ( TRUE );  /* response is back and correct. */
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Set_Address
**
** Descriptions:    Send CMD3, STE_RELATIVE_ADDR, should after CMD2
**
** parameters:      None
** Returned value:    TRUE if response is back before timeout.
**
******************************************************************************/
DWORD MCI_Set_Address( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];
DWORD CmdArgument;

  /* If it's a SD card, SET_RELATIVE_ADDR is to get the address
  from the card and use this value in RCA, if it's a MMC, set default
  RCA addr. 0x00010000. */
  if ( CardType == SD_CARD )
  {
    CmdArgument = 0;
  }
  else      /* If it's unknown or MMC_CARD, fix the RCA address */
  {
    CmdArgument = 0x00010000;
  }

  retryCount = 0x20;      /* reset retry counter */
  while ( retryCount > 0 )
  {
    /* Send CMD3 command repeatedly until the response is back correctly */
    MCI_SendCmd( SET_RELATIVE_ADDR, CmdArgument, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SET_RELATIVE_ADDR, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    /* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
    /* It should go to IDEN state and bit 8 should be 1 */
    if ( !(respStatus & MCI_CMD_TIMEOUT) && ((respValue[0] & (0x0F << 8)) == 0x0500) )
    {
      CardRCA = respValue[0] & 0xFFFF0000;  /* Save the RCA value from SD card */
      return ( TRUE );  /* response is back and correct. */
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_CSD
**
** Descriptions:    CMD9, SEND_CSD cmd, it should be sent only at
**            STBY state and after CMD3. See MMC and SD spec. state
**            diagram.
**
** parameters:      None
** Returned value:    Response value
**
******************************************************************************/
DWORD MCI_Send_CSD( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];
DWORD CmdArgument;

  if ( CardType == SD_CARD )
  {
    CmdArgument = CardRCA;
  }
  else      /* if MMC or unknown card type, use default RCA addr. */
  {
    CmdArgument = 0x00010000;
  }

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    /* Send SET_BLOCK_LEN command before read and write */
    MCICLEAR = (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
    MCI_SendCmd( SEND_CSD, CmdArgument, EXPECT_LONG_RESP, 0 );
    respStatus = MCI_GetCmdResp( SEND_CSD, EXPECT_LONG_RESP, (DWORD *)&respValue[0] );
    if ( !respStatus )
    {
      return ( TRUE );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Select_Card
**
** Descriptions:    CMD7, SELECT_CARD, should be after CMD9, the state
**            will be inter-changed between STBY and TRANS after
**            this cmd.
**
** parameters:      None
** Returned value:    return false if response times out.
**
******************************************************************************/
DWORD MCI_Select_Card( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];
DWORD CmdArgument;

  if ( CardType == SD_CARD )
  {
    CmdArgument = CardRCA;
  }
  else      /* if MMC or unknown card type, use default RCA addr. */
  {
    CmdArgument = 0x00010000;
  }

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    /* Send SELECT_CARD command before read and write */
    MCICLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
    MCI_SendCmd( SELECT_CARD, CmdArgument, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SELECT_CARD, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0700 ))
    {           /* Should be in STANDBY state now and ready */
      return ( TRUE );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_Status
**
** Descriptions:    CMD13, SEND_STATUS, the most important cmd to
**            debug the state machine of the card.
**
** parameters:      None
** Returned value:    Response value(card status), true if the ready bit
**            is set in the card status register, if timeout, return
**            INVALID_RESPONSE 0xFFFFFFFF.
**
******************************************************************************/
DWORD MCI_Send_Status( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];
DWORD CmdArgument;

  if ( CardType == SD_CARD )
  {
    CmdArgument = CardRCA;
  }
  else      /* if MMC or unknown card type, use default RCA addr. */
  {
    CmdArgument = 0x00010000;
  }

  /* Note that, since it's called after the block write and read, this timeout
  is important based on the clock you set for the data communication. */
  retryCount = 0x60;
  while ( retryCount > 0 )
  {
    /* Send SELECT_CARD command before read and write */
    MCICLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
    MCI_SendCmd( SEND_STATUS, CmdArgument, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SEND_STATUS, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    if ( !respStatus && (respValue[0] & (1 << 8)) )
    { /* The ready bit should be set, it should be in either TRAN or RCV
      state now */
      return ( respValue[0] );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( INVALID_RESPONSE );
}

/******************************************************************************
** Function name:   MCI_Set_BlockLen
**
** Descriptions:    CMD16, SET_BLOCKLEN, called after CMD7(SELECT_CARD)
**            called in the TRANS state.
**
** parameters:      The length of the data block to be written or read.
** Returned value:    true or false, return TRUE if ready bit is set, and it's
**            in TRANS state.
**
******************************************************************************/
DWORD MCI_Set_BlockLen( DWORD blockLength )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    /* Send SET_BLOCK_LEN command before read and write */
    MCICLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
    MCI_SendCmd( SET_BLOCK_LEN, blockLength, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SET_BLOCK_LEN, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    /* bit 9 through 12 should be in transfer state now. bit 8 is ready. */
    if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900))
    {
      return ( TRUE );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_ACMD_Bus_Width
**
** Descriptions:    ACMD6, SET_BUS_WIDTH, if it's SD card, we can
**            use the 4-bit bus instead of 1-bit. This cmd
**            can only be called during TRANS state.
**            Since it's a ACMD, CMD55 APP_CMD needs to be
**            sent out first.
**
** parameters:      Bus width value, 1-bit is 0, 4-bit is 10
** Returned value:    true or false, true if the card is still in the
**            TRANS state after the cmd.
**
******************************************************************************/
DWORD MCI_Send_ACMD_Bus_Width( DWORD buswidth )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;      /* reset retry counter */
  while ( retryCount > 0 )
  {
    if ( MCI_Send_ACMD() == FALSE )
    {
      continue;
    }

    /* Send ACMD6 command to set the bus width */
    MCI_SendCmd( SET_ACMD_BUS_WIDTH, buswidth, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( SET_ACMD_BUS_WIDTH, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
    {
      return ( TRUE );  /* response is back and correct. */
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_Stop
**
** Descriptions:    CMD12, STOP_TRANSMISSION. if that happens, the card is
**            maybe in a unknown state that need a warm reset.
**
** parameters:      None
** Returned value:    true or false, true if, at least, the card status
**            shows ready bit is set.
**
******************************************************************************/
DWORD MCI_Send_Stop( void )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    MCICLEAR = 0x7FF;
    MCI_SendCmd( STOP_TRANSMISSION, 0x00000000, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( STOP_TRANSMISSION, EXPECT_SHORT_RESP, (DWORD *)respValue );
    /* ready bit, bit 8, should be set in the card status register */
    if ( !respStatus && (respValue[0] & (1 << 8)) )
    {
      return( TRUE );
    }
    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );
}

/******************************************************************************
** Function name:   MCI_Send_Write_Block
**
** Descriptions:    CMD24, WRITE_BLOCK, send this cmd in the TRANS state
**            to write a block of data to the card.
**
** parameters:      block number
** Returned value:    Response value
**
******************************************************************************/
DWORD MCI_Send_Write_Block( DWORD blockNum )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    MCICLEAR = 0x7FF;
    MCI_SendCmd( WRITE_BLOCK, blockNum * BLOCK_LENGTH, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( WRITE_BLOCK, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    /* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
    if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
    {
      return( TRUE );     /* ready and in TRAN state */
    }

    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );       /* Fatal error */
}

/******************************************************************************
** Function name:   MCI_Send_Read_Block
**
** Descriptions:    CMD17, READ_SINGLE_BLOCK, send this cmd in the TRANS
**            state to read a block of data from the card.
**
** parameters:      block number
** Returned value:    Response value
**
******************************************************************************/
DWORD MCI_Send_Read_Block( DWORD blockNum )
{
DWORD i, retryCount;
DWORD respStatus;
DWORD respValue[4];

  retryCount = 0x20;
  while ( retryCount > 0 )
  {
    MCICLEAR = 0x7FF;
    MCI_SendCmd( READ_SINGLE_BLOCK, blockNum * BLOCK_LENGTH, EXPECT_SHORT_RESP, 0 );
    respStatus = MCI_GetCmdResp( READ_SINGLE_BLOCK, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
    /* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
    if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
    {
      return( TRUE );     /* ready and in TRAN state */
    }

    for ( i = 0; i < 0x20; i++ );
    retryCount--;
  }
  return ( FALSE );         /* Fatal error */
}

/******************************************************************************
** Function name:   MCI_Write_Block
**
** Descriptions:    Set MCI data control register, data length and data
**            timeout, send WRITE_BLOCK cmd, finally, enable
**            interrupt. On completion of WRITE_BLOCK cmd, TX_ACTIVE
**            interrupt will occurs, data can be written continuously
**            into the FIFO until the block data length is reached.
**
** parameters:      block number
** Returned value:    true or false, if cmd times out, return false and no
**            need to continue.
**
******************************************************************************/
DWORD MCI_Write_Block( DWORD blockNum )
{
DWORD i;
DWORD DataCtrl = 0;

  MCICLEAR = 0x7FF;
  MCIDATACTRL = 0;
  for ( i = 0; i < 0x10; i++ );

  /* Below status check is redundant, but ensure card is in TRANS state
  before writing and reading to from the card. */
  if ( MCI_CheckStatus() != TRUE )
  {
    MCI_Send_Stop();
    return( FALSE );
  }

  MCIDATATIMER = DATA_TIMER_VALUE;
  MCIDATALENGTH = BLOCK_LENGTH;
  if ( MCI_Send_Write_Block( blockNum ) == FALSE )
  {
    return ( FALSE );
  }

#if MCI_DMA_ENABLED
  DMA_Init( 0, M2P );
  DMACC0CONFIGURATION = 0x10001 | (0x00 << 1) | (0x04 << 6) | (0x05 << 11);
  /* Write, block transfer, DMA, and data length */
  DataCtrl |= ((1 << 0) | (1 << 3) | (DATA_BLOCK_LEN << 4));
#else
  /* Write, block transfer, and data length */
  DataCtrl |= ((1 << 0) | (DATA_BLOCK_LEN << 4));
#endif
  MCI_TXEnable();

  MCIDATACTRL = DataCtrl;
  for ( i = 0; i < 0x10; i++ );

  return ( TRUE );
}

/******************************************************************************
** Function name:   MCI_Read_Block
**
** Descriptions:    Set MCI data control register, data length and data
**            timeout, send READ_SINGLE_BLOCK cmd, finally, enable
**            interrupt. On completion of READ_SINGLE_BLOCK cmd,
**            RX_ACTIVE interrupt will occurs, data can be read
**            continuously into the FIFO until the block data
**            length is reached.
**
** parameters:      block number
** Returned value:    true or false, if cmd times out, return false and no
**            need to continue.
**
**
******************************************************************************/
DWORD MCI_Read_Block( DWORD blockNum )
{
DWORD i;
DWORD DataCtrl = 0;

  MCICLEAR = 0x7FF;
  MCIDATACTRL = 0;
  for ( i = 0; i < 0x10; i++ );

  /* Below status check is redundant, but ensure card is in TRANS state
  before writing and reading to from the card. */
  if ( MCI_CheckStatus() != TRUE )
  {
    MCI_Send_Stop();
    return( FALSE );
  }
  MCI_RXEnable();

  MCIDATATIMER = DATA_TIMER_VALUE;
  MCIDATALENGTH = BLOCK_LENGTH;

  if ( MCI_Send_Read_Block( blockNum ) == FALSE )
  {
    return ( FALSE );
  }
#if MCI_DMA_ENABLED
  DMA_Init( 1, P2M );
  DMACC1CONFIGURATION = 0x10001 | (0x04 << 1) | (0x00 << 6) | (0x06 << 11);
  /* Write, block transfer, DMA, and data length */
  DataCtrl |= ((1 << 0) | (1 << 1) | (1 << 3) | (DATA_BLOCK_LEN << 4));
#else
  /* Read, enable, block transfer, and data length */
  DataCtrl |= ((1 << 0) | (1 << 1) | (DATA_BLOCK_LEN << 4));
#endif
  MCIDATACTRL = DataCtrl;
  for ( i = 0; i < 0x10; i++ );
  return ( TRUE );
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
