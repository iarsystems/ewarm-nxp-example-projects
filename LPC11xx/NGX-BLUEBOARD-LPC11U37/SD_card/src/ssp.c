#include <LPC11Uxx.H>       /* LPC17xx definitions */

#include "type.h"
#include "ssp.h"
#include "utils.h"

#define PIO1_29_IOCON_ADDRESS (0x400040D4UL)
/*****************************************************************************
** Function name:   spi1_init
**
** Descriptions:    SPI1(SSP) port initialization routine
**
** parameters:      None
** Returned value:  true or false, if the interrupt handler
**                  can't be installed correctly, return false.
**
*****************************************************************************/
U32 spi1_init(void)
{
volatile U8 i, Dummy;

  LPC_SYSCON->PRESETCTRL |= (0x1<<0);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
  LPC_SYSCON->SSP0CLKDIV = 0x02;      /* Divided by 2 */
  LPC_IOCON->PIO0_8 &= ~0x07; /*  SSP I/O config */
  LPC_IOCON->PIO0_8 |= 0x01;    /* SSP MISO */
  LPC_IOCON->PIO0_9 &= ~0x07;
  LPC_IOCON->PIO0_9 |= 0x01;    /* SSP MOSI */
  LPC_IOCON->PIO1_29 = 0x01;/* P2.11 function 1 is SSP clock, need to combined */

  LPC_SSP0->CR0 = SSP_CONFIGURATION0;
  LPC_SSP0->CR1 = SSPCR1_SSE;
  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  LPC_SSP0->CPSR = 0x02;

  for ( i = 0; i < FIFOSIZE; i++ )
  {
    Dummy = LPC_SSP0->DR;   /* clear the RxFIFO */
  }
  return( TRUE );
}


/******************************************************************************
**                            End Of File
******************************************************************************/
