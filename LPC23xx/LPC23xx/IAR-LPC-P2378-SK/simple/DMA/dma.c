/*****************************************************************************
 *   dma.c:  DMA module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "dma.h"

#include <intrinsics.h>

volatile DWORD DMATCCount = 0;
volatile DWORD DMAErrCount = 0;

/******************************************************************************
** Function name:   DMAHandler
**
** Descriptions:    DMA interrupt handler
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
__irq __nested __arm void DMAHandler (void)
{
DWORD regVal;

  __enable_interrupt(); /* handles nested interrupt */

  regVal = DMACINTTCSTATUS;
  if ( regVal )
  {
    DMATCCount++;
    DMACINTTCCLEAR |= regVal;
  }

  regVal = DMACINTERRSTAT;
  if ( regVal )
  {
    DMAErrCount++;
    DMACINTERRCLR |= regVal;
  }

  VICADDRESS = 0;  /* Acknowledge Interrupt */
}

/******************************************************************************
** Function name:   DMA_Init
**
** Descriptions:
**
** parameters:
** Returned value:
**
******************************************************************************/
DWORD DMA_Init( DWORD DMAMode )
{
  /* USB RAM is used for test.
  Please note, Ethernet has its own SRAM, but GPDMA can't access
  that. GPDMA can access USB SRAM and IRAM. Ethernet DMA controller can
  access both IRAM and Ethernet SRAM. */
  PCONP |= (1 << 29); /* Enable GPDMA clock */

  /* clear all interrupts on channel 0 */
  DMACINTTCCLEAR = 0x01;
  DMACINTERRCLR = 0x01;

  if ( DMAMode == M2M )
  {
    /* Ch0 is used for M2M test, for M2P and P2M, go to peripheral directories
    where both DAM and non DMA examples are provided. */
    DMACC0SRCADDR = DMA_SRC;
    DMACC0DESTADDR = DMA_DST;
    /* Terminal Count Int enable */
    DMACC0CONTROL = ((DMA_SIZE/4) & 0x0FFF) | (0x04 << 12) | (0x04 << 15)
    | (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;
  }
  else
  {
    return ( FALSE );
  }

  DMACCONFIGURATION = 0x01;  /* Enable DMA channels, little endian */
  while ( !(DMACCONFIGURATION & 0x01) );

  if ( install_irq( GPDMA_INT, (void *)DMAHandler, HIGHEST_PRIORITY ) == FALSE )
  {
    return ( FALSE );
  }
  return (TRUE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
