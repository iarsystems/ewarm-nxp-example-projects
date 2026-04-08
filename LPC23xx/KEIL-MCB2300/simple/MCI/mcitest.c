/*****************************************************************************
 *   mcitest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *  If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
 * this will disable trace port too.
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "mci.h"
#include "dma.h"
#include "target.h"
#include "board.h"

extern volatile DWORD CardType;

/* Below buffer allocation could be simplier, if the buffer used for DMA and non-DMA
are shared. Future clear up will be needed. For now, leave as it's. */
#if MCI_DMA_ENABLED
BYTE *src_addr;
BYTE *dest_addr;
#else
volatile BYTE WriteBlock[BLOCK_LENGTH], ReadBlock[BLOCK_LENGTH];
volatile DWORD TXBlockCounter, RXBlockCounter;
#endif

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
DWORD i, j;

  TargetResetInit();

  /* Init Card present and Write protect switches */
  SCS_bit.GPIOM = 1;          /* enable fast io for GPIO0,1 */
  MMC_CP_DIR &= ~MMC_CP_MASK; /* Card present port - input */
  MMC_WP_DIR &= ~MMC_WP_MASK; /* Write protect port - input */

  /* Fill block data pattern in write buffer and clear everything
  in read buffer. */
#if MCI_DMA_ENABLED
  /* on DMA channel 0, source is memory, destination is MCI FIFO. */
  /* On DMA channel 1, source is MCI FIFO, destination is memory. */
  PCONP |= (1 << 29); /* Enable GPDMA clock */

  src_addr = (BYTE *)DMA_SRC;
  dest_addr = (BYTE *)DMA_DST;
  for ( i = 0; i < DMA_SIZE; i++ )
  {
    *src_addr = i;
    *dest_addr = 0;
    src_addr++;
    dest_addr++;
  }
#else
  for ( i = 0; i < BLOCK_LENGTH; i++ )
  {
    WriteBlock[i] = i;
    ReadBlock[i] = 0;
  }
#endif

  /* For the SD card I tested, the minimum required block length is 512 */
  /* For MMC, the restriction is loose, due to the variety of SD and MMC
  card support, ideally, the driver should read CSD register to find the
  right speed and block length for the card, and set them accordingly.
  In this driver example, it will support both MMC and SD cards, and it
  does read the information by send SEND_CSD to poll the card status,
  however, to simplify the example, it doesn't configure them accordingly
  based on the CSD register value. This is not intended to support all
  the SD and MMC cards. */
  /* loop until card present */
  while(MMC_CP_FIO & MMC_CP_MASK);

  if ( MCI_Init() != TRUE )
  {
    while( 1 );     /* fatal error */
  }

  if ( (CardType = MCI_CardInit()) == CARD_UNKNOWN )
  {
    while ( 1 );    /* fatal error */
  }

  if ( MCI_Check_CID() != TRUE )
  {
    while ( 1 );    /* fatal error */
  }

  if ( MCI_Set_Address() != TRUE )
  {
    while ( 1 );    /* fatal error */
  }

  if ( MCI_Send_CSD() != TRUE )
  {
    while ( 1 );    /* fatal error */
  }

  if ( MCI_Select_Card() != TRUE )
  {
    while ( 1 );    /* fatal error */
  }

  if ( CardType == SD_CARD )
  {
    MCICLOCK_bit.WIDEBUS = 1;   /* Use wide bus for SD */
    for ( i = 0; i < 0x20; i++ );
    if ( MCI_Send_ACMD_Bus_Width( BUS_WIDTH_4BITS ) == FALSE )
    {
      while ( 1 );  /* fatal error */
    }
  }

  if ( MCI_Set_BlockLen( BLOCK_LENGTH ) != TRUE )
  {
    while ( 1 );    /* fatal error */
  }

  if(MMC_WP_FIO & MMC_WP_MASK)
  {
    while(1); /* fatal error card is protected */
  }

  for ( i = 0; i < BLOCK_NUM; i++ )
  {
#if !MCI_DMA_ENABLED
    TXBlockCounter = 0;
    RXBlockCounter = 0;
#endif
    if ( MCI_Write_Block( i ) != TRUE )
    {
      while ( 1 );    /* Fatal error */
    }
    /* When TX_ACTIVE is not set, it indicates TX is done from
    Interrupt_Write_Block, now, Interrupt_Read_Block can start. */
    /* Note TXEnable() is called in the Interrupt_Write_Block(). */
    MCI_TXDisable();

    if ( MCI_Read_Block( i ) != TRUE )
    {
      while ( 1 );    /* Fatal error */
    }
    /* When RX_ACTIVE is not set, it indicates RX is done from
    Interrupt_Read_Block, now, validation of RX and TX buffer can start. */
    /* Note RXEnable() is called in the Interrupt_Read_Block(). */
    MCI_RXDisable();

#if MCI_DMA_ENABLED
    src_addr = (BYTE *)DMA_SRC;
    dest_addr = (BYTE *)DMA_DST;
    for ( j = 0; j < BLOCK_LENGTH; j++ )
    {
      if ( *(src_addr+j) != *(dest_addr+j) )
      {
        while ( 1 );  /* Data comparison failure, fatal error */
      }
    }
    /* clear read buffer for next comparison */
    for ( j = 0; j < BLOCK_LENGTH; j++ )
    {
      *(dest_addr+j) = 0;
    }
#else
    for ( j = 0; j < BLOCK_LENGTH; j++ )
    {
      if ( WriteBlock[j] != ReadBlock[j] )
      {
        while ( 1 );  /* Data comparison failure, fatal error */
      }
    }
    /* clear read buffer for next comparison */
    for ( j = 0; j < BLOCK_LENGTH; j++ )
    {
      ReadBlock[j] = 0;
    }
#endif
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
