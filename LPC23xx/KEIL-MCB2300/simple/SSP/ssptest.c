/*****************************************************************************
 *   ssptest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
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
#include "LPC230x.H"                        /* LPC23xx/24xx definitions */
#include "type.h"
#include "irq.h"
#include "ssp.h"
#include "target.h"
#include "dma.h"

BYTE *src_addr;
BYTE *dest_addr;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  DWORD i;

  TargetResetInit();

  SSP0Init();	/* initialize SSP0(SSP) port, share pins with SPI0
  on port0(p0.15-18). */

  /* DMA_SRC and DMA_DST are RAM area shared by both DMA and non DMA operation */
  src_addr = (BYTE *)DMA_SRC;
  dest_addr = (BYTE *)DMA_DST;
  for ( i = 0; i < BUFSIZE; i++ )
  {
    *src_addr = i;
    *dest_addr = 0;
    src_addr++;
    dest_addr++;
  }

#if DMA_ENABLED
#if !USE_CS
  IOCLR0 = SSP0_SEL;	/* SSEL is output */
#endif
  /* on DMA channel 0, Source is memory, destination is SSPO TX,
  on DMA channel 1, source is SSP0 RX, Destination is memory */
  /* Enable channel and IE bit */

  DMA_Init( 0, M2P );
  GPDMA_CH0_CFG |= 0x18001 | (0x00 << 6) | (0x05 << 11);
  DMA_Init( 1, P2M );
  GPDMA_CH1_CFG |= 0x08001 | (0x01 << 1) | (0x06 << 11);

  /* Enable DMA TX and RX on SSP0 */
  SSP0DMACR = 0x03;
#if !USE_CS
  IOSET0 = SSP0_SEL;	/* SSEL is output */
#endif
#else
#if !USE_CS
  IOCLR0 = SSP0_SEL;	/* SSEL is output */
#endif
  src_addr = (BYTE *)DMA_SRC;
  dest_addr = (BYTE *)DMA_DST;
  i = 0;
  while ( i <= BUFSIZE )
  {
    /* to check the RXIM and TXIM interrupt, I send a block data at one time
    based on the FIFOSIZE(8). */
    SSP0Send( (BYTE *)src_addr, FIFOSIZE );
    /* If RX interrupt is enabled, below receive routine can be
    also handled inside the ISR. */
    SSP0Receive( (BYTE *)dest_addr, FIFOSIZE );
    src_addr += FIFOSIZE;
    dest_addr += FIFOSIZE;
    i += FIFOSIZE;
  }
#if !USE_CS
  IOSET0 = SSP0_SEL;	/* SSEL is output */
#endif
#endif

  /* verifying write and read data buffer. */
  src_addr = (BYTE *)DMA_SRC;
  dest_addr = (BYTE *)DMA_DST;
  for ( i = 0; i < BUFSIZE; i++ )
  {
    if ( *(src_addr+i) != *(dest_addr+i) )
    {
      while( 1 );			/* Verification failed */
    }
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
