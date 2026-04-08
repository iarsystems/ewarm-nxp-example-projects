/****************************************************************************
 *   $Id: lpc13Axx_adc.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx ADC example
 *
 *   Description:
 *     This file contains ADC code example which include ADC 
 *     initialization, ADC interrupt handler, and APIs for ADC
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
#include "lpc13Axx.h"			/* LPC13Axx Peripheral Registers */
#include "lpc13Axx_adc.h"

#ifndef _BV
#define _BV(_x_) (1UL << (_x_))
#endif

volatile uint32_t ADCValue[2][ADC_NUM];
volatile uint32_t ADCTHCMPFlag[2][ADC_NUM];
volatile uint32_t ADCSEQIntDone[2] = {0,0};
volatile uint32_t OverRunCounter[2] = {0,0};
volatile uint32_t channel_flag[2] = {0,0}; 

volatile ADC_Config ADCConfig;
ADC_Config *ADCSetupPtr = (ADC_Config *)&ADCConfig;

/******************************************************************************
** Function name:		ADC_SEQHandler
**
** Descriptions:		ADC SEQx interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_SEQHandler( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr ) 
{
  uint32_t regVal;
  volatile uint32_t dummy;
  int i;
  
  regVal = ADCx->FLAGS;		/* Read ADC will clear the interrupt */
  if ( regVal & (0xFFF<<12) )	/* check OVERRUN error first */
  {
		if ( ADCx == LPC_ADC0 )
      OverRunCounter[0]++;
		if ( ADCx == LPC_ADC1 )
      OverRunCounter[1]++;
  	for ( i = 0; i < ADC_NUM; i++ )
  	{
  	  regVal = (regVal & 0x00FFF000) >> 12;
  	  /* if overrun, just read ADDR to clear */
  	  /* regVal variable has been reused. */
      if ( regVal & _BV(i) )
  	  {
        dummy = ADCx->DAT[i];
  	  }
  	}
		if ( ADCSetupPtr->seq_sel == SEQ_B )
			ADCx->SEQB_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now, turn off BURST bit. */
		else
			ADCx->SEQA_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now, turn off BURST bit. */
  	return;	
  }
    
  for ( i = 0; i < ADC_NUM; i++ )
  {
    if ( regVal & _BV(i) )
    {
      if ( (ADCx == LPC_ADC0) && ADCSetupPtr->tenbit_mode )
      {
        ADCValue[0][i] = ( LPC_ADC0->DAT[i] >> RESULT_OFFSET ) & 0x3FF;
        channel_flag[0] |= (0x1 << (LPC_ADC0->DAT[i] >> CHN_OFFSET));
        if ( ADCSetupPtr->burst_mode && ((channel_flag[0] & 0xFFF) == 0xFFF))
        {
          /* All the bits in have been set, it indicates all the ADC channels have been converted. */
          if ( ADCSetupPtr->seq_sel == SEQ_B )
            ADCx->SEQB_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now */
          else
            ADCx->SEQA_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now */
          channel_flag[0] = 0; 
          return;
        }
      }
      else if ( (ADCx == LPC_ADC1) && !ADCSetupPtr->tenbit_mode )
      {
        ADCValue[1][i] = ( LPC_ADC0->DAT[i] >> RESULT_OFFSET ) & 0xFFF;
        channel_flag[1] |= (0x1 << (LPC_ADC1->DAT[i] >> CHN_OFFSET));
        if ( ADCSetupPtr->burst_mode && ((channel_flag[1] & 0xFFF) == 0xFFF))
        {
          /* All the bits in have been set, it indicates all the ADC channels have been converted. */
          if ( ADCSetupPtr->seq_sel == SEQ_B )
            ADCx->SEQB_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now */
          else
            ADCx->SEQA_CTRL &= ~(ADC_BURST|ADC_START);	/* stop ADC now */
          channel_flag[1] = 0; 
          return;
        }
      }
    }
  }

  if ( ADCSetupPtr->seq_sel == SEQ_B )
    ADCx->SEQB_CTRL &= ~ADC_START;	/* stop ADC now */
  else
    ADCx->SEQA_CTRL &= ~ADC_START;	/* stop ADC now */
  return;
}

/******************************************************************************
** Function name:		ADC0_SEQA_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC0_SEQA_IRQHandler( void ) 
{
	if ( LPC_ADC0->FLAGS & SEQA_INT )
  {
    ADCSetupPtr->seq_sel = SEQ_A;
		ADC_SEQHandler( LPC_ADC0, ADCSetupPtr );
		ADCSEQIntDone[0] = 1;
  }
}

/******************************************************************************
** Function name:		ADC1_SEQA_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC1_SEQA_IRQHandler( void ) 
{
	if ( LPC_ADC1->FLAGS & SEQA_INT )
  {
    ADCSetupPtr->seq_sel = SEQ_A;
		ADC_SEQHandler( LPC_ADC1, ADCSetupPtr );
    ADCSEQIntDone[1] = 1;
  }
}

/******************************************************************************
** Function name:		ADC0_SEQA_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC0_SEQB_IRQHandler(void) 
{
	if ( LPC_ADC0->FLAGS & SEQB_INT )
  {
    ADCSetupPtr->seq_sel = SEQ_B;
		ADC_SEQHandler( LPC_ADC0, ADCSetupPtr );
  }
}

/******************************************************************************
** Function name:		ADC1_SEQA_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC1_SEQB_IRQHandler(void) 
{
	if ( LPC_ADC1->FLAGS & SEQB_INT )
  {
    ADCSetupPtr->seq_sel = SEQ_B;
		ADC_SEQHandler( LPC_ADC1, ADCSetupPtr );
  }
}

/******************************************************************************
** Function name:		ADC_OVR_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_OVR_IRQHandler( LPC_ADC_TypeDef *ADCx )
{
  uint32_t i, regVal=regVal;
	
	/* A dummy read to the data register wiil clear the overrun interrupt flag. */
	for ( i = 0; i < ADC_NUM; i++ )
  {
    if ( (ADCx == LPC_ADC0) && (LPC_ADC0->FLAGS & FLAGS_OVERRUN(i)) ) 
    {
      regVal = *(volatile unsigned long *)(LPC_ADC0_BASE 
  			  + ADC_OFFSET + ADC_INDEX * i);
    }
    else if ( (ADCx == LPC_ADC1) && (LPC_ADC1->FLAGS & FLAGS_OVERRUN(i)) ) 
    {
			regVal = *(volatile unsigned long *)(LPC_ADC1_BASE 
          + ADC_OFFSET + ADC_INDEX * i);			
    }
  }
  return;	
}

/******************************************************************************
** Function name:		ADC_OVR_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC0_OVR_IRQHandler( void )
{
  if ( LPC_ADC0->FLAGS & OVERRUN_INT )
  {
    ADC_OVR_IRQHandler( LPC_ADC0 );
  }
}

/******************************************************************************
** Function name:		ADC_OVR_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC1_OVR_IRQHandler( void )
{
  if ( LPC_ADC1->FLAGS & OVERRUN_INT )
  {
    ADC_OVR_IRQHandler( LPC_ADC1 );
  }
}

/******************************************************************************
** Function name:		ADC_THCMP_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_THCMP_IRQHandler( LPC_ADC_TypeDef *ADCx )
{
  uint32_t i;
	
	/* A dummy read to the data register wiil clear the overrun interrupt flag. */
	for ( i = 0; i < ADC_NUM; i++ )
  {
    if ( (ADCx == LPC_ADC0) && (LPC_ADC0->FLAGS & FLAGS_THCMP(i)) ) 
    {
			ADCTHCMPFlag[0][i]++;
      LPC_ADC0->FLAGS |= FLAGS_THCMP(i);
    }
    else if ( (ADCx == LPC_ADC1) && (LPC_ADC1->FLAGS & FLAGS_THCMP(i)) )
    {
			ADCTHCMPFlag[1][i]++;
      LPC_ADC1->FLAGS |= FLAGS_THCMP(i);		
    }
  }
  return;	
}

/******************************************************************************
** Function name:		ADC_OVR_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC0_THCMP_IRQHandler( void )
{
  if ( LPC_ADC0->FLAGS & THCMP_INT )
  {
    ADC_THCMP_IRQHandler( LPC_ADC0 );
  }
}

/******************************************************************************
** Function name:		ADC_OVR_IRQHandler
**
** Descriptions:		ADC Overrun interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC1_THCMP_IRQHandler( void )
{
  if ( LPC_ADC1->FLAGS & THCMP_INT )
  {
    ADC_THCMP_IRQHandler( LPC_ADC1 );
  }
}

/*****************************************************************************
** Function name:		ADCCalibration
**
** Descriptions:		Calibration ADC
**
** parameters:			None
** Returned value:	None
** 
*****************************************************************************/
void ADCCalibration( void )
{
	uint32_t regVal0, regVal1;
	
  LPC_SYSCON->PDRUNCFG &= ~(PDEN_ADC0 | PDEN_ADC1);
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (EN0_ADC0 | EN0_ADC1);
	/* Save ADCx CTRL register value */
	regVal0 = LPC_ADC0->CTRL;
	regVal1 = LPC_ADC1->CTRL;
	LPC_SYSCON->PRESETCTRL0 = (EN0_ADC0 | EN0_ADC1);
  NVIC_DisableIRQ(ADC0_OVR_IRQn);
  NVIC_DisableIRQ(ADC0_THCMP_IRQn);
  NVIC_DisableIRQ(ADC0_SEQA_IRQn);
  NVIC_DisableIRQ(ADC0_SEQB_IRQn);
  NVIC_DisableIRQ(ADC1_OVR_IRQn);
  NVIC_DisableIRQ(ADC1_THCMP_IRQn);
  NVIC_DisableIRQ(ADC1_SEQA_IRQn);
  NVIC_DisableIRQ(ADC1_SEQB_IRQn);
	/* Calibrate both ADC0 and ADC1, ADC clock set to 500KHz  */
  LPC_ADC0->CTRL = ( SystemCoreClock / LPC_SYSCON->ADCASYNCCLKDIV / ADC_CALIB_CLK - 1 );
	LPC_ADC0->CTRL |= ADC_CAL_MODE;
  while ( LPC_ADC0->CTRL & ADC_CAL_MODE );
  LPC_ADC1->CTRL = ( SystemCoreClock / LPC_SYSCON->ADCASYNCCLKDIV / ADC_CALIB_CLK - 1 );
	LPC_ADC1->CTRL |= ADC_CAL_MODE;
  while ( LPC_ADC1->CTRL & ADC_CAL_MODE );
	/* Calibration is done, restore ADCx CTRL register */
	LPC_ADC0->CTRL = regVal0;
	LPC_ADC1->CTRL = regVal1;
  return;	
}

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			pointer to which ADC, pointer to ADC config table
** Returned value:	None
** 
*****************************************************************************/
void ADCInit( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr )
{
  uint32_t i;

  /* Disable Power down bit to the ADC block. */ 
  if ( ADCx == LPC_ADC0 ) {	
    LPC_SYSCON->PDRUNCFG &= ~PDEN_ADC0;
    /* Enable AHB clock to the ADC. */
    LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_ADC0;
    LPC_SYSCON->PRESETCTRL0 |= EN0_ADC0;
		/* p0.8, p0.7, p0.6, p0.5, p0.4, p0.3, p0.2, p0.1, p1.0, p0.31, p0.0, p0.30*/
    LPC_SWM->PINENABLE0 &= ~0x00000FFF;		
  }
	else if ( ADCx == LPC_ADC1 ) {
    LPC_SYSCON->PDRUNCFG &= ~PDEN_ADC1;
    /* Enable AHB clock to the ADC. */
    LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_ADC1;
    LPC_SYSCON->PRESETCTRL0 |= EN0_ADC1;
		/* p1.1, p0.9, p0.10, p0.11, p1.2, p1.3, p0.13, p0.14, p0.15, p0.16, p1.4, p1.5 */
    LPC_SWM->PINENABLE0 &= ~(0xFFFUL<<12);
    if (ADCSetupPtr->seq_sel == SEQ_B )
		{
			ADCx->SEQB_CTRL = ADCSetupPtr->channels | ADCSetupPtr->burst_mode | ADCSetupPtr->trigger | 
			ADCSetupPtr->sync_bypass | ADCSetupPtr->single_step | ADCSetupPtr->seq_ctrl_mode;
			if ( ADCSetupPtr->hwtrig_mode ) {
        ADCx->SEQB_CTRL |= TRIGGER(ADC0_PIN_TRIG0);
      }
			ADCx->SEQB_CTRL |= ADC_SEQ_ENA;		
		}
		else
		{
			ADCx->SEQA_CTRL = ADCSetupPtr->channels | ADCSetupPtr->burst_mode | ADCSetupPtr->trigger | 	
				ADCSetupPtr->sync_bypass | ADCSetupPtr->single_step | ADCSetupPtr->seq_ctrl_mode;
			if ( ADCSetupPtr->hwtrig_mode ) {
        ADCx->SEQA_CTRL |= TRIGGER(ADC0_PIN_TRIG0);
      }
			ADCx->SEQA_CTRL |= ADC_SEQ_ENA;			
		}
  }
  LPC_SYSCON->ADCASYNCCLKDIV = 1;
	
  for ( i = 0; i < ADC_NUM; i++ )
  {
	  ADCValue[0][i] = 0x0;
    ADCValue[1][i] = 0x0;
  }

	ADCx->INPUTSEL = ADCSetupPtr->input_sel;
	
  ADCx->CTRL = ( ( SystemCoreClock / LPC_SYSCON->ADCASYNCCLKDIV / ADCSetupPtr->adc_clock - 1 ) << 0 );
  if ( ADCSetupPtr->lpwr_mode )
		ADCx->CTRL |= ADC_LPWR_MODE;  		/* Low-power mode */
  if ( ADCSetupPtr->tenbit_mode )
		ADCx->CTRL |= ADC_10BIT_MODE;  		/* 10-bit mode */
  if ( ADCSetupPtr->async_mode )
		ADCx->CTRL |= ADC_ASYNC_MODE;  		/* Async mode */
	ADCx->CTRL |= ADC_ENA;							/* ADC operational */ 

	if ( ADCSetupPtr->thr_sel == 0 ) {
    ADCx->CHAN_THRSEL = 0x000;
    ADCx->THR0_LOW = THR0_LOW_VALUE;
    ADCx->THR0_HIGH = THR0_HIGH_VALUE;
  }
  else {
    ADCx->CHAN_THRSEL = 0xFFF;
    ADCx->THR1_LOW = THR1_LOW_VALUE;
    ADCx->THR1_HIGH = THR1_HIGH_VALUE;
  }
			
  /* If POLLING, no need to do the following */
  if ( ADCSetupPtr->int_mode ) {
    if ( ADCx == LPC_ADC0 ) {
      NVIC_EnableIRQ(ADC0_OVR_IRQn);
      NVIC_EnableIRQ(ADC0_THCMP_IRQn);
      if ( ADCSetupPtr->seq_sel == SEQ_B )
        NVIC_EnableIRQ(ADC0_SEQB_IRQn);
      else
        NVIC_EnableIRQ(ADC0_SEQA_IRQn);			
    }
    else if ( ADCx == LPC_ADC1 ) {
      NVIC_EnableIRQ(ADC1_OVR_IRQn);
      NVIC_EnableIRQ(ADC1_THCMP_IRQn);
      if ( ADCSetupPtr->seq_sel == SEQ_B )
        NVIC_EnableIRQ(ADC1_SEQB_IRQn);
      else
        NVIC_EnableIRQ(ADC1_SEQA_IRQn);		
    }
    ADCx->INTEN = SEQA_ENA|OVR_ENA|
		  ADCCMP_ENA0(1) | ADCCMP_ENA1(1) | ADCCMP_ENA2(1) | ADCCMP_ENA3(1) |
		  ADCCMP_ENA4(1) | ADCCMP_ENA5(1) | ADCCMP_ENA6(1) | ADCCMP_ENA7(1) |
		  ADCCMP_ENA8(1) | ADCCMP_ENA9(1) | ADCCMP_ENA10(1) | ADCCMP_ENA11(1);		/* Enable all interrupts */
  }
  return;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			pointer to which ADC, pointer to ADC config table, Channel number
** Returned value:	Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr, uint8_t channelNum )
{
  uint32_t regVal, ADC_Data;

	if ( ADCSetupPtr->seq_sel == SEQ_B )
  {
    ADCx->SEQB_CTRL &= 0xFFFFF000;
    if ( !ADCSetupPtr->hwtrig_mode )	/* If hardware trigger, no need to set START bit. */
      ADCx->SEQB_CTRL |= ADC_START | (1 << channelNum);
		else
      ADCx->SEQB_CTRL |= (1 << channelNum);
  }
	else
  {
    ADCx->SEQA_CTRL &= 0xFFFFF000;
    if ( !ADCSetupPtr->hwtrig_mode )	/* If hardware trigger, no need to set START bit. */
      ADCx->SEQA_CTRL |= ADC_START | (1 << channelNum);
		else
      ADCx->SEQA_CTRL |= (1 << channelNum);
  }
	/* switch channel,start A/D convert */
  if ( ADCSetupPtr->int_mode == 0 )
  {
		/* Polling mode */
    while ( 1 )			/* wait until end of A/D convert */
    {
      if ( ADCx == LPC_ADC0 ) {
			  regVal = *(volatile unsigned long *)(LPC_ADC0_BASE 
  			  + ADC_OFFSET + ADC_INDEX * channelNum);
				if ( regVal & ADC_OVERRUN ) {
          OverRunCounter[0]++;
        }
      }
      else if ( ADCx == LPC_ADC1 ) {
			  regVal = *(volatile unsigned long *)(LPC_ADC1_BASE 
          + ADC_OFFSET + ADC_INDEX * channelNum);
				if ( regVal & ADC_OVERRUN ) {
          OverRunCounter[1]++;
        }				
      }			
  	  /* read result of A/D conversion */
  	  if ( regVal & ADC_DATAVALID )
  	  {
  	    break;
  	  }
    }	

    if ( ADCSetupPtr->seq_sel == SEQ_B ) {	
      ADCx->SEQB_CTRL &= ~ADC_START;	/* stop ADC now */
    }
    else {
      ADCx->SEQA_CTRL &= ~ADC_START;	/* stop ADC now */
    }	
	  if ( ADCSetupPtr->tenbit_mode )
      ADC_Data = ( regVal >> RESULT_OFFSET ) & 0x3FF;
    else
      ADC_Data = ( regVal >> RESULT_OFFSET ) & 0xFFF;
    return ( ADC_Data );	/* return A/D conversion value */
  }
	else
  {
		/* if it's interrupt driven, the ADC reading is done inside the handler, 
		return the channel number instead. */
    return ( channelNum );	
  }
}

/*****************************************************************************
** Function name:		ADCReadGlobal
**
** Descriptions:		Read ADC channel via global register
**                  Polling only, no burst.
**
** parameters:			pointer to which ADC, pointer to ADC config table, Channel number
** Returned value:	Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCReadGlobal( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr, uint8_t channelNum )
{
  uint32_t regVal, ADC_Data;

	if ( ADCSetupPtr->seq_sel == SEQ_B )
  {
    ADCx->SEQB_CTRL &= 0xFFFFF000;
    if ( !ADCSetupPtr->hwtrig_mode )	/* If hardware trigger, no need to set START bit. */
      ADCx->SEQB_CTRL |= ADC_START | (1 << channelNum);
		else
      ADCx->SEQB_CTRL |= (1 << channelNum);
    
		/* switch channel,start A/D convert */
    /* Polling mode */
    while ( 1 )			/* wait until end of A/D convert */
    {
      if ( ADCx == LPC_ADC0 ) {
			  regVal = ADCx->SEQB_GDAT;
        if ( regVal & ADC_OVERRUN )
          OverRunCounter[0]++;
      }
      else if ( ADCx == LPC_ADC1 ) {
        regVal = ADCx->SEQB_GDAT;
        if ( regVal & ADC_OVERRUN )
          OverRunCounter[1]++;
      }			
  	  /* read result of A/D conversion */
  	  if ( regVal & ADC_DATAVALID )
  	  {
  	    break;
  	  }
    }
    ADCx->SEQB_CTRL &= ~ADC_START;	/* stop ADC now */		
  }
	else
  {
    ADCx->SEQA_CTRL &= 0xFFFFF000;
    if ( !ADCSetupPtr->hwtrig_mode )	/* If hardware trigger, no need to set START bit. */
      ADCx->SEQA_CTRL |= ADC_START | (1 << channelNum);
		else
      ADCx->SEQA_CTRL |= (1 << channelNum);
    
		/* switch channel,start A/D convert */
    /* Polling mode */
    while ( 1 )			/* wait until end of A/D convert */
    {
      if ( ADCx == LPC_ADC0 ) {
			  regVal = ADCx->SEQA_GDAT;
        if ( regVal & ADC_OVERRUN )
          OverRunCounter[0]++;
      }
      else if ( ADCx == LPC_ADC1 ) {
        regVal = ADCx->SEQA_GDAT;
        if ( regVal & ADC_OVERRUN )
          OverRunCounter[1]++;
      }			
  	  /* read result of A/D conversion */
  	  if ( regVal & ADC_DATAVALID )
  	  {
  	    break;
  	  }
    }
    ADCx->SEQA_CTRL &= ~ADC_START;	/* stop ADC now */				
  }

	if ( ADCSetupPtr->tenbit_mode )
    ADC_Data = ( regVal >> RESULT_OFFSET ) & 0x3FF;
  else
    ADC_Data = ( regVal >> RESULT_OFFSET ) & 0xFFF;
  return ( ADC_Data );	/* return A/D conversion value */
}

/*****************************************************************************
** Function name:		ADC0BurstRead
**
** Descriptions:		Use burst mode to convert multiple channels once.
**                  Result will be updated inside ISR.
**
** parameters:			pointer to which ADC, pointer to ADC config table
** Returned value:	None
** 
*****************************************************************************/
void ADCBurstRead( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr )
{
	if ( ADCSetupPtr->seq_sel == SEQ_B )
  {
    if ( ADCx->SEQB_CTRL & ADC_START )
    {
      ADCx->SEQB_CTRL &= ~ADC_START;
    }
    /* Read all channels, 0 through 11. */
    ADCx->SEQB_CTRL |= 0xFFF;
    ADCx->SEQB_CTRL |= ADC_BURST;		/* Set burst mode and start A/D convert */
  }
	else
  {
    if ( ADCx->SEQA_CTRL & ADC_START )
    {
      ADCx->SEQA_CTRL &= ~ADC_START;
    }
    /* Read all channels, 0 through 11. */
    ADCx->SEQA_CTRL |= 0xFFF;
    ADCx->SEQA_CTRL |= ADC_BURST;		/* Set burst mode and start A/D convert */
  }
  return;						/* the ADC reading is done inside the handler, return 0. */
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
