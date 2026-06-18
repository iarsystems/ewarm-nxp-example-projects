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
#include <nxp/iolpc2468.h>
#include "type.h"
#include "irq.h"
#include "mci.h"
#include "dma.h"

#if MCI_DMA_ENABLED
/******************************************************************************
** Function name:   DMA_Init
**
** Descriptions:
**
** parameters:
** Returned value:
**
******************************************************************************/
DWORD DMA_Init( DWORD ChannelNum, DWORD DMAMode )
{
  /* USB RAM is used for test.
  Please note, Ethernet has its own SRAM, but GPDMA can't access
  that. GPDMA can access USB SRAM and IRAM. Ethernet DMA controller can
  access both IRAM and Ethernet SRAM. */
  if ( ChannelNum == 0 )
  {
    DMACINTERRCLR = 0x01;
    DMACINTTCCLEAR = 0x01;
    if ( DMAMode == M2M )
    {
      /* Ch0 is set for M2M tranfer from AHB1 to AHB2 SRAM */
      DMACC0SRCADDR = DMA_SRC;
      DMACC0DESTADDR = DMA_DST;
      /* Terminal Count Int enable */
      DMACC0CONTROL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;
    }
    else if ( DMAMode == M2P )
    {
      /* Ch0 set for M2P transfer from mempry to MCI FIFO. */
      DMACC0SRCADDR = DMA_SRC;
      DMACC0DESTADDR = DMA_MCIFIFO;
      /* The burst size is set to 4, the size is 32 bit. */
      /* Terminal Count Int enable */
      DMACC0CONTROL = (DMA_SIZE & 0x0FFF) | (0x02 << 12) | (0x02 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 26) | 0x80000000;
    }
    else if ( DMAMode == P2M )
    {
      /* Ch0 set for P2M transfer from MCI FIFO to memory. */
      DMACC0SRCADDR = DMA_MCIFIFO;
      DMACC0DESTADDR = DMA_DST;
      /* The burst size is set to 4, the size is 32 bit. */
      /* Terminal Count Int enable */
      DMACC0CONTROL = (DMA_SIZE & 0x0FFF) | (0x02 << 12) | (0x02 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 27) | 0x80000000;
    }
    else
    {
      return ( FALSE );
    }
  }
  else if ( ChannelNum == 1 )
  {
    DMACINTERRCLR = 0x02;
    DMACINTTCCLEAR = 0x02;
    if ( DMAMode == M2M )
    {
      /* Ch1 is set for M2M tranfer */
      DMACC1SRCADDR = DMA_SRC;
      DMACC1DESTADDR = DMA_DST;
      /* Terminal Count Int enable */
      DMACC1CONTROL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;
    }
    else if ( DMAMode == M2P )
    {
      /* Ch1 set for M2P transfer from mempry to MCI FIFO. */
      DMACC1SRCADDR = DMA_SRC;
      DMACC1DESTADDR = DMA_MCIFIFO;
      /* The burst size is set to 8, the size is 32 bit. */
      /* Terminal Count Int enable */
      DMACC1CONTROL = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 26) | 0x80000000;
    }
    else if ( DMAMode == P2M )
    {
      /* Ch1 set for P2M transfer from MCI_FIFO to memory. */
      DMACC1SRCADDR = DMA_MCIFIFO;
      DMACC1DESTADDR = DMA_DST;
      /* The burst size is set to 8, the size is 32 bit. */
      /* Terminal Count Int enable */
      DMACC1CONTROL = (DMA_SIZE & 0x0FFF) | (0x02 << 12) | (0x02 << 15)
        | (0x02 << 18) | (0x02 << 21) | (1 << 27) | 0x80000000;
    }
    else
    {
      return ( FALSE );
    }
  }
  else
  {
    return ( FALSE );
  }

  DMACCONFIGURATION = 0x01;  /* Enable DMA channels, little endian */
  while ( !(DMACCONFIGURATION & 0x01) );
    return (TRUE);
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
