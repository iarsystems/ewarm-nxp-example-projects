/*****************************************************************************
 *   adc.c:  ADC module file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.08.15  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "adc.h"

volatile DWORD ADC1Value[ITERATION][ADC_NUM];
volatile DWORD ADC1ValueAverage[ADC_NUM];
volatile DWORD ADC1IntDone = 0;
volatile DWORD ADC1Iteration = 0;

/******************************************************************************
** Function name:		ADC1Handler
**
** Descriptions:		ADC1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC1_Handler (void)
{
  DWORD regVal;
  
  regVal = ADC1_INT_STATUS;
  if ( regVal & (0x01<<0) )	/* check EOS error first */
  {
	ADC1_Stop();
	if ( ADC1Iteration >= ITERATION )
	{
	  ADC1Iteration = 0;
	}
	ADC1Value[ADC1Iteration][0] = ADC1_ACD0 & 0x3FF;
	ADC1Value[ADC1Iteration][1] = ADC1_ACD1 & 0x3FF;
	ADC1Value[ADC1Iteration][2] = ADC1_ACD2 & 0x3FF;
	ADC1Value[ADC1Iteration][3] = ADC1_ACD3 & 0x3FF;
	ADC1Value[ADC1Iteration][4] = ADC1_ACD4 & 0x3FF;
	ADC1Value[ADC1Iteration][5] = ADC1_ACD5 & 0x3FF;
	ADC1Value[ADC1Iteration][6] = ADC1_ACD6 & 0x3FF;
	ADC1Value[ADC1Iteration][7] = ADC1_ACD7 & 0x3FF;
	ADC1Iteration++;
	ADC1IntDone = 1;
	ADC1_INT_CLR_STATUS |= (0x01<<0);
  }
    
  if ( regVal & (0x01<<1) )	/* ADC2 Compare */
  {
	ADC1_INT_CLR_STATUS |= (0x01<<1);
  }
  return;
}

/*****************************************************************************
** Function name:		ADC1_Init
**
** Descriptions:		initialize ADC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ADC1_Init( void )
{
  DWORD regVal;

  ADC1_Stop();
  
  /* all the related pins are set to ADC inputs, AD1.0~7 */
  /* ADC1 channel 0~7 */
  SFSP0_8 = (0x00<<2)|(0x01<<0);	/* Analog input, func. 1. */
  SFSP0_9 = (0x00<<2)|(0x01<<0);	
  SFSP0_10 = (0x00<<2)|(0x01<<0);	
  SFSP0_11 = (0x00<<2)|(0x01<<0);	
  SFSP0_12 = (0x00<<2)|(0x01<<0);	
  SFSP0_13 = (0x00<<2)|(0x01<<0);	
  SFSP0_14 = (0x00<<2)|(0x01<<0);	
  SFSP0_15 = (0x00<<2)|(0x01<<0);

  ADC1_ACC0 = 0x0A;			/* AD1 channel 0~7 are 10-bit resolution */
  ADC1_ACC1 = 0x0A;
  ADC1_ACC2 = 0x0A;
  ADC1_ACC3 = 0x0A;
  ADC1_ACC4 = 0x0A;
  ADC1_ACC5 = 0x0A;
  ADC1_ACC6 = 0x0A;
  ADC1_ACC7 = 0x0A;
  
  regVal = ADC1_CONFIG;
  regVal &= ~(0x01<<0);			/* single scan. */
//  regVal |= (0x01<<0);			/* continuous scan. */
  /* Positive and negative edge of MSCSS timer(START3). */  
  ADC1_CONFIG = regVal|(0x1<<14)|(0x1<<15);

  install_irq( 17, (void *)ADC1_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( 17, ACTIVE_HIGH, HIGHEST_PRIORITY );

  /* Enable both Comp/match and EOS interrupts on ADC1 */ 
  ADC1_INT_SET_ENABLE = 0x03;
  return;
}

/*****************************************************************************
** Function name:		ADC1_Start
**
** Descriptions:		Start ADC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ADC1_Start( void )
{
  if ( ADC1_STATUS & 0x01 )
  {
	/* Stop ADC first if conversion in progress */
	ADC1_Stop();
  }	
  ADC1_CTRL = ((0x01<<0)|(0x01<<2));	/* Set both start and update bits. */
  return;
}

/*****************************************************************************
** Function name:		ADC1_Stop
**
** Descriptions:		Stop ADC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ADC1_Stop( void )
{
  /* if in continuous mode and conversion is progress,
  wait until it finishes. */
  while ( (ADC1_STATUS & 0x01)&&(ADC1_CONFIG & 0x01) );		
  ADC1_CTRL = (0x01<<1);
  return;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
