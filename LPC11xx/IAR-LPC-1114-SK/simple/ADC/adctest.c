/*****************************************************************************
 *   adctest.c:  main C entry file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "adc.h"
#if ADC_DEBUG
#include "uart.h"
#endif

extern volatile uint32_t ADCValue[ADC_NUM];
extern volatile uint32_t ADCIntDone;

#if ADC_DEBUG
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

uint8_t ConvertDigital ( uint8_t digital )
{
  uint8_t ascii_char;

  if ( (digital >= 0) && (digital <= 9) )
  {
	ascii_char = digital + 0x30;	/* 0~9 */
  }
  else
  {
	ascii_char = digital - 0x0A;
	ascii_char += 0x41;				/* A~F */
  }
  return ( ascii_char );
}
#endif

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i;
  
  SystemInit();

#if ADC_DEBUG  
  /* NVIC is installed inside UARTInit file. */
  UARTInit(115200);
#endif

  /* Initialize ADC  */
  ADCInit( ADC_CLK );

#if BURST_MODE				/* Interrupt driven only */
  ADCBurstRead();
  while ( !ADCIntDone );
  ADCIntDone = 0;
#else
  for ( i = 0; i < ADC_NUM; i++ )
  {
#if ADC_INTERRUPT_FLAG				/* Interrupt driven */
	ADCRead( i );
	while ( !ADCIntDone );
	ADCIntDone = 0;
#else
	ADCValue[i] = ADCRead( i );	/* Polling */
#endif
  }
#endif

#if ADC_DEBUG
  for ( i = 0; i < ADC_NUM; i++ )
  {
//	LPC_UART->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	UARTBuffer[0] = ConvertDigital( (uint8_t)(ADCValue[i]>>28));
	UARTBuffer[1] = ConvertDigital( (uint8_t)((ADCValue[i]>>24)&0xF));
	UARTBuffer[2] = ConvertDigital( (uint8_t)((ADCValue[i]>>20)&0xF));
	UARTBuffer[3] = ConvertDigital( (uint8_t)((ADCValue[i]>>16)&0xF));
	UARTBuffer[4] = ConvertDigital( (uint8_t)((ADCValue[i]>>12)&0xF));
	UARTBuffer[5] = ConvertDigital( (uint8_t)((ADCValue[i]>>8)&0xF));
	UARTBuffer[6] = ConvertDigital( (uint8_t)((ADCValue[i]>>4)&0xF));
	UARTBuffer[7] = ConvertDigital( (uint8_t)(ADCValue[i]&0xF));
	UARTBuffer[8] = '\r';
	UARTBuffer[9] = '\n';
	UARTSend( (uint8_t *)UARTBuffer, 10 );
//	LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
  }
#endif
  return ( 0 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

