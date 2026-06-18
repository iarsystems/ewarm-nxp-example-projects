/****************************************************************************
 *   $Id: lpc13Axx_bod.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx BOD(Brown-OUt Detect) example
 *
 *   Description:
 *     This file contains BOD code example which include BOD 
 *     initialization, BOD interrupt handler, and APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc13Axx.h"			/* lpc13Axx Peripheral Registers */
#include "lpc13Axx_gpio.h"
#include "lpc13Axx_nmi.h"
#include "lpc13Axx_bod.h"

volatile uint32_t bod_counter;

/*****************************************************************************
** Function name:		BOD_IRQHandler
**
** Descriptions:		BOD interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void BOD_IRQHandler(void)
{
  if ( LPC_SYSCON->SYSRESSTAT & BOD_RESET )
  {
		/* If the BOD RESET has occured, disable the BOD interrupt,
		or we will stuck here serving BOD interrupt instead of
		LED blinking showing BOD RESET. */ 
		NVIC_DisableIRQ(BOD_IRQn);
		return;
  }

  bod_counter++;

  /* Turn on BOD RST LED only, turn off the rest. */
  GPIOSetBitValue( 0, BOD_INTERRUPT_LED, 1 );
  GPIOSetBitValue( 0, BOD_RESET_LED, 0 );
  GPIOSetBitValue( 0, POR_RESET_LED, 1 );
  return;
}

/*****************************************************************************
** Function name:		BOD_Init
**
** Descriptions:		Initialize BOD control register
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void BOD_Init( void )
{
	LPC_SYSCON->PDRUNCFG &= ~PDEN_BOD;          /* Power-up BOD          */
	
  /* Set BOD control register. interrupt level assertion is 1.65V, 
  de-assertion is 1.80V. */
  LPC_SYSCON->BODCTRL = BOD_INT_LVL3|BOD_RST_LVL2;
//	LPC_SYSCON->BODCTRL = BOD_INT_LVL3|BOD_RST_LVL2|BOD_RST_ENABLE;
//	LPC_SYSCON->BODCTRL = (0x3<<2)|(0x1<<6);
  /* Enable the BOD Interrupt */
#if NMI_ENABLED
	NVIC_DisableIRQ( BOD_IRQn );
	NMI_Init( BOD_IRQn );
#else
  NVIC_EnableIRQ(BOD_IRQn);
#endif
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
