/*****************************************************************************
 *   ssp.c:  SSP C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "irq.h"
#include "ssp.h"
#include "dma.h"

#include <intrinsics.h>

/* statistics of all the interrupts */
volatile DWORD interruptRxStat = 0;
volatile DWORD interruptOverRunStat = 0;
volatile DWORD interruptRxTimeoutStat = 0;

/*****************************************************************************
** Function name:   SSP1Handler
**
** Descriptions:    SSP port is used for SPI communication.
**            SSP1(SSP) interrupt handler
**            The algorithm is, if RXFIFO is at least half full,
**            start receive until it's empty; if TXFIFO is at least
**            half empty, start transmit until it's full.
**            This will maximize the use of both FIFOs and performance.
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
__irq __nested __arm void SSP1Handler (void)
{
DWORD regValue;

  __enable_interrupt();   /* handles nested interrupt */

  regValue = SSP1MIS;
  if ( regValue & SSPMIS_RORMIS ) /* Receive overrun interrupt */
  {
    interruptOverRunStat++;
    SSP1ICR = SSPICR_RORIC; /* clear interrupt */
  }
  if ( regValue & SSPMIS_RTMIS )  /* Receive timeout interrupt */
  {
    interruptRxTimeoutStat++;
    SSP1ICR = SSPICR_RTIC;  /* clear interrupt */
  }

  /* please be aware that, in main and ISR, CurrentRxIndex and CurrentTxIndex
  are shared as global variables. It may create some race condition that main
  and ISR manipulate these variables at the same time. SSPSR_BSY checking (polling)
  in both main and ISR could prevent this kind of race condition */
  if ( regValue & SSPMIS_RXMIS )  /* Rx at least half full */
  {
    interruptRxStat++;  /* receive until it's empty */
  }

  VICADDRESS = 0;           /* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:   SSP1Init
**
** Descriptions:    SSP1 port initialization routine
**
** parameters:      None
** Returned value:    true or false, if the interrupt handler
**            can't be installed correctly, return false.
**
*****************************************************************************/
DWORD SSP1Init( void )
{
BYTE i;
volatile BYTE Dummy;

  /* enable clock to SSP1 for security reason. By default, it's enabled already */
  PCONP |= (1 << 10);

  /* Configure PIN connect block */
  /* port 0 bits 7, 8, 9, 6 are SSP port SCK0, MISO0, MOSI0, and SSEL0 */
  /* set SSEL to GPIO pin that you will have the totoal freedom to set/reset
  the SPI chip-select pin */
  /* When DMA is enabled, enable USE_CS, or be careful with SSP1_SEL pin,
  clear SSP1_SEL before DMA starts, and set SSP1_SEL after finishing. */
  PINSEL0 &= ~0x000FF000;
#if USE_CS
  PINSEL0 |= 0x000AA000;
#else
  PINSEL0 |= 0x000A8000;
  IO0DIR = SSP1_SEL;  /* SSEL is output */
  IO0SET = SSP1_SEL;  /* set SSEL to high */
#endif

  /* SSP disable!!!! */
  SSP1CR1_bit.SSE = 0;

  /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  SSP1CR0 = 0x0707;

  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  SSP1CPSR = 0x2;

  for ( i = 0; i < FIFOSIZE; i++ )
  {
    Dummy = SSP1DR;   /* clear the RxFIFO */
  }

  if ( install_irq( SPI0_INT, (void *)SSP1Handler, HIGHEST_PRIORITY ) == FALSE )
  {
    return (FALSE);
  }

  /* Device select as master, SSP Enabled, loopback operational mode */
  SSP1CR1 = SSPCR1_LBM | SSPCR1_SSE;
  /* Set SSPINMS registers to enable interrupts */
  /* enable all error related interrupts */
  SSP1IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;
  return( TRUE );
}

/*****************************************************************************
** Function name:   SSP1Send
**
** Descriptions:    Send a block of data to the SSP1 port, the
**            first parameter is the buffer pointer, the 2nd
**            parameter is the block length.
**
** parameters:      buffer pointer, and the block length
** Returned value:    None
**
*****************************************************************************/
void SSP1Send( BYTE *buf, DWORD Length )
{
    DWORD i;
    for ( i = 0; i < Length; i++ )
    {
    /* as long as TNF bit is set (TxFIFO is not full), I can always transmit */
    while ( !(SSP1SR & SSPSR_TNF) );
    SSP1DR = *buf;
    buf++;
    /* Wait until the Busy bit is cleared */
    while ( !(SSP1SR & SSPSR_BSY) );
    }
    return;
}

/*****************************************************************************
** Function name:   SSP1Receive
** Descriptions:    the module will receive a block of data from
**            the SSP1, the 2nd parameter is the block
**            length.
** parameters:      buffer pointer, and block length
** Returned value:    None
**
*****************************************************************************/
void SSP1Receive( BYTE *buf, DWORD Length )
{
DWORD i;

  for ( i = 0; i < Length; i++ )
  {
    /* As long as Receive FIFO is not empty, I can always receive. */
    /* since it's a loopback test, clock is shared for both TX and RX,
    no need to write dummy byte to get clock to get the data */
    /* if it's a peer-to-peer communication, SSPDR needs to be written
    before a read can take place. */
    //    SSP1DR = 0xFF;
    while ( !(SSP1SR & SSPSR_RNE) );
    *buf = SSP1DR;
    buf++;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
