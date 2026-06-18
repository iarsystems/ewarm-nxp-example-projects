/****************************************************************************
 *   $Id: lpc13Axx_dac.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx DAC example
 *
 *   Description:
 *     This file contains DAC code example which include DAC 
 *     initialization, DAC interrupt handler, and APIs for DAC
 *     reading.
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
#include "LPC13Axx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "lpc13Axx_dac.h"

volatile uint32_t DAC_Value = 0;
volatile uint32_t shutoff_flag = 0;

/******************************************************************************
** Function name:		DAC_IRQHandler
**
** Descriptions:		DAC trigger interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void DAC_IRQHandler(void)
{
	if ( LPC_DAC->CTRL & CTRL_SHUTOFF_FLAG )
	{
		LPC_DAC->VAL = 0;
		shutoff_flag++;
		return;
	}
	
  /* Any write to VAL register will clear the trigger bit */
	if ( LPC_DAC->CTRL & CTRL_INT )
	{
    LPC_DAC->VAL = (DAC_Value << 4) & 0x0000FFFF;
    DAC_Value++;
    if ( DAC_Value > MAX_DAC_VALUE )
    {
      DAC_Value=0;
    }
	}
  return;
}

/*****************************************************************************
** Function name:		DACInit
**
** Descriptions:		initialize DAC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void DACInit( uint32_t dac_timer_val )
{
#if !TIMER_TRIG_ENABLED
	uint32_t regVal;
#endif
	
  /* DAC should be powered up first. */
  LPC_SYSCON->PDRUNCFG &= ~PDEN_DAC0;

  /* Enable AHB clock to the DAC. */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_DAC0;

  /* Peripheral reset control to DAC, a "1" bring DAC out of reset. */
  LPC_SYSCON->PRESETCTRL0 |= EN0_DAC0;
  LPC_SYSCON->PRESETCTRL0 &= ~EN0_DAC0;

  /* p0.12 as configured as DAC output */ 	
	LPC_SWM->PINENABLE0 &= ~(0x1<<24);

  DAC_Value = 0;
#if TIMER_TRIG_ENABLED
	LPC_DAC->CNTVAL = ( SystemCoreClock / dac_timer_val - 1 );
  LPC_DAC->CTRL = CTRL_TIMER_ENA | CTRL_DBLBUF_ENA;	
#else
#if DAC_SHUTOFF_ENABLED
  LPC_DAC->CTRL = CTRL_SHUTOFF_ENA;
	/* p0.6 as the DAC SHUTOFF Input */
	regVal = LPC_SWM->PINASSIGN12 & ~( 0xFF<<8 );
	LPC_SWM->PINASSIGN12 = regVal | ( 0x6<<8 );		
#endif

  LPC_DAC->CTRL |= (CTRL_TRIGSRC(1) | CTRL_POLARITY);
		/* p0.7 as the DAC external trigger Input */
	regVal = LPC_SWM->PINASSIGN11 & ~( 0xFFUL<<24 );
	LPC_SWM->PINASSIGN11 = regVal | ( 0x7<<24 );		
#endif
  NVIC_EnableIRQ(DAC_IRQn);
  return;
}

/*****************************************************************************
** Function name:		DACDMAInit
**
** Descriptions:		initialize DAC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void DACDMAInit( uint32_t dac_timer_val )
{
  /* DAC should be powered up first. */
  LPC_SYSCON->PDRUNCFG &= ~PDEN_DAC0;

  /* Enable AHB clock to the DAC. */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_DAC0;

  /* Peripheral reset control to DAC, a "1" bring DAC out of reset. */
  LPC_SYSCON->PRESETCTRL0 |= EN0_DAC0;

  /* p0.12 as configured as DAC output */ 	
	LPC_SWM->PINENABLE0 &= ~(0x1<<24);

  DAC_Value = 0;
	LPC_DAC->CNTVAL = ( SystemCoreClock / dac_timer_val - 1 );
  LPC_DAC->CTRL = CTRL_TIMER_ENA | CTRL_DBLBUF_ENA;
  return;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
