/*****************************************************************************
 *   adc.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __ADC_H 
#define __ADC_H

#define ADC_NUM			8		/* for ADC1 on LPC29xx */
#define ITERATION		10

extern void ADC1_Handler( void );
extern void ADC1_Init( void );
extern void ADC1_Start( void );
extern void	ADC1_Stop( void );

#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
