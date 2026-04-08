/*****************************************************************************
 *   dmatest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
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
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "target.h"
#include "dma.h"

extern volatile DWORD DMATCCount;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
DWORD i;
DWORD *src_addr, *dest_addr;

  TargetResetInit();

  src_addr = (DWORD *)DMA_SRC;
  dest_addr = (DWORD *)DMA_DST;
  for ( i = 0; i < DMA_SIZE/4; i++ )
  {
    *src_addr = i;
    *dest_addr = 0;
    src_addr++;
    dest_addr++;
  }

  DMA_Init( M2M );
  DMACC0CONFIGURATION |= 0x08001; /* Enable channel and IE bit */

  while ( !DMATCCount );    /* Wait until DMA is done */

  /* Verify result */
  src_addr = (DWORD *)DMA_SRC;
  dest_addr = (DWORD *)DMA_DST;
  for ( i = 0; i < DMA_SIZE/4; i++ )
  {
    if ( *src_addr != *dest_addr )
    {
      while ( 1 );  /* fatal error */
    }
    src_addr++;
    dest_addr++;
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
