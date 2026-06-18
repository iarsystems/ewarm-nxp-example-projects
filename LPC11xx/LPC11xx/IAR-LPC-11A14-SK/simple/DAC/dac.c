/****************************************************************************
 *   $Id: dac.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx DAC example
 *
 *   Description:
 *     This file contains DAC code example which include DAC 
 *     initialization, DAC interrupt handler, and APIs for DAC
 *     reading.
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

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors’ 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.

****************************************************************************/

#include "LPC11Axx.h"     /* LPC11Axx Peripheral Registers */
#include "type.h"
#include "dac.h"
#if TIMER_TRIG_ENABLED
#include "timer16.h"
#include "timer32.h"
#endif

volatile uint32_t DAC_Value = 0;

/******************************************************************************
** Function name:   DAC_IRQHandler
**
** Descriptions:    DAC trigger interrupt handler
**
** parameters:      None
** Returned value:    None
** 
******************************************************************************/
void DAC_IRQHandler(void)
{
  uint32_t regVal;

  /* Any write to CR register will clear the trigger bit */
  regVal = LPC_DAC->CR & ~(MAX_DAC_VALUE << 6);
  LPC_DAC->CR = regVal | (DAC_Value << 6);
  DAC_Value++;
  if ( DAC_Value > MAX_DAC_VALUE )
  {
    DAC_Value=0;
  }
  return;
}

/*****************************************************************************
** Function name:   DACInit
**
** Descriptions:    initialize DAC channel
**
** parameters:      None
** Returned value:    None
** 
*****************************************************************************/
void DACInit( void )
{
  /* DAC should be powered up first. */
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<14);

  /* Enable AHB clock to the DAC. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<21);

  /* Peripheral reset control to DAC, a "1" bring DAC out of reset. */
  LPC_SYSCON->PRESETCTRL |= (0x1<<10);

  /* setup the related pin to DAC output */
  LPC_IOCON->PIO0_4  &= ~0x9F; /* Clear bit7, change to analog mode. */ 
  LPC_IOCON->PIO0_4  |= 0x02;  /* DOUT */

  DAC_Value = 0;
#if ATRG_ENABLED
  /* Enable Analog ADC or DAC trigger */
#if TIMER_TRIG_ENABLED
  /* In timer16/32.h, set timer configuration to MATCH_OUT */
#if 1
  init_timer16(1, SystemCoreClock/1000 - 1);
  enable_timer16(1);
  /* DAC trigger both edges on CT16B1_MAT1 */
  LPC_DAC->CR = (CT16B1_M0_TRIGGER<<17)|(0x3<<21)|DAC_BIAS;
#endif
#if 0
  init_timer16(1, SystemCoreClock/1000 - 1);
  enable_timer16(1);
  /* DAC trigger both edges on CT16B1_MAT1 */
  LPC_DAC->CR = (CT16B1_M1_TRIGGER<<17)|(0x3<<21)|DAC_BIAS;
#endif
#if 0
  init_timer32(1, SystemCoreClock/1000 - 1);
  enable_timer32(1);
  /* DAC trigger both edges on CT16B1_MAT1 */
  LPC_DAC->CR = (CT32B1_M0_TRIGGER<<17)|(0x3<<21)|DAC_BIAS;
#endif
#if 0
  init_timer32(1, SystemCoreClock/1000 - 1);
  enable_timer32(1);
  /* DAC trigger both edges on CT16B1_MAT1 */
  LPC_DAC->CR = (CT32B1_M1_TRIGGER<<17)|(0x3<<21)|DAC_BIAS;
#endif        
#else
#if 0
  /* DAC trigger on both edges on Comparator */
  LPC_DAC->CR = (CMP_TRIGGER<<17)|(0x3<<21)|DAC_BIAS; 
#endif        
#if 1
  LPC_IOCON->PIO0_16 &= ~0x07;
  LPC_IOCON->PIO0_16 |= 0x01;   /* ATRG0 */
  /* DAC trigger on both edges on ATRG0 */
  LPC_DAC->CR = (ATRG0_TRIGGER<<17)|(0x3<<21)|DAC_BIAS; 
#endif        
#if 0
  LPC_IOCON->PIO0_17 &= ~0x07;
  LPC_IOCON->PIO0_17 |= 0x01;   /* ATRG1 */  
  /* DAC trigger on both edges on ATRG0 */
  LPC_DAC->CR = (ATRG1_TRIGGER<<17)|(0x3<<21)|DAC_BIAS; 
#endif        
#endif
  NVIC_EnableIRQ(DAC_IRQn);
#endif   
  return;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
