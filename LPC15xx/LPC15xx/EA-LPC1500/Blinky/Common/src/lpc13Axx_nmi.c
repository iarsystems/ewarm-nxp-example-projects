/****************************************************************************
 *   $Id: lpc13Axx_nmi.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx NMI interrupt example
 *
 *   Description:
 *     This file contains NMI interrupt handler code example.
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
#include "LPC13Axx.h"
#include "lpc13Axx_mrt.h"
#include "lpc13Axx_uart.h"
#include "lpc13Axx_spi.h"
#include "lpc13Axx_i2c.h"
#include "lpc13Axx_sct.h"
#include "lpc13Axx_bod.h"
#include "lpc13Axx_wdt.h"
#include "lpc13Axx_gpio.h"
#include "lpc13Axx_rtc.h"
#include "lpc13Axx_comp.h"
#include "lpc13Axx_nmi.h"

#if NMI_ENABLED
volatile uint32_t NMI_Counter[MAX_NMI_NUM];

/*****************************************************************************
** Function name:		NMI_Handler
**
** Descriptions:		NMI interrupt handler
** parameters:		None			 
** 						
** Returned value:	None
** 
*****************************************************************************/
void NMI_Handler( void )
{
  uint32_t regVal;

  regVal = LPC_SYSCON->NMISRC;
  regVal &=	~0x80000000;
  if ( regVal < MAX_NMI_NUM )
  {
		if ( regVal == WDT_IRQn )
		{
			WDT_IRQHandler();
		}
		if ( regVal == BOD_IRQn )
		{
			BOD_IRQHandler();
		}
		if ( regVal == FLASH_IRQn )
		{
			FLASH_IRQHandler();
		}
		if ( regVal == EE_IRQn )
		{
			EE_IRQHandler();
		}
		if ( regVal == DMA_IRQn )
		{
			DMA_IRQHandler();
		}
		if ( regVal == GINT0_IRQn )
		{
      GINT0_IRQHandler();
    }
		if ( regVal == GINT1_IRQn )
		{
      GINT1_IRQHandler();
    }
		if ( (regVal >= PIN_INT0_IRQn) && (regVal <= PIN_INT7_IRQn) )
		{
			PIN_INT_Handler ( regVal - PIN_INT0_IRQn );
		}
		if ( regVal == OSTIMER_IRQn )
		{
			OSTIMER_IRQHandler();
		}
		if ( regVal == SCT0_IRQn )
		{
			SCT0_IRQHandler();
		}
		if ( regVal == SCT1_IRQn )
		{
			SCT1_IRQHandler();
		}
		if ( regVal == SCT2_IRQn )
		{
			SCT2_IRQHandler();
		}
		if ( regVal == SCT3_IRQn )
		{
			SCT3_IRQHandler();
		}
		if ( regVal == MRT_IRQn )
		{
			MRT_IRQHandler();
		}
		if ( regVal == UART0_IRQn )
		{
			UART0_IRQHandler();
		}
		if ( regVal == UART1_IRQn )
		{
			UART1_IRQHandler();
		}
		if ( regVal == UART2_IRQn )
		{
			UART2_IRQHandler();
		}
		if ( regVal == I2C_IRQn )
		{
			I2C_IRQHandler();
		}
		if ( regVal == SPI0_IRQn )
		{
			SPI0_IRQHandler();
		}
		if ( regVal == SPI1_IRQn )
		{
			SPI_IRQHandler();
		}
		if ( regVal == CAN_IRQn )
		{
			CAN_IRQHandler();
		}

		if ( regVal == USB_IRQn )
		{
			USB_IRQHandler();
		}
		if ( regVal == USB_FIQn )
		{
			USB_FIQHandler();
		}
		if ( regVal == USBWakeup_IRQn )
		{
			USBWakeup_IRQHandler();
		}
		if ( regVal == ADC0_SEQA_IRQn )
		{
			ADC0_SEQA_IRQHandler();
		}
		if ( regVal == ADC0_SEQB_IRQn )
		{
			ADC0_SEQB_IRQHandler();
		}
		if ( regVal == ADC0_THCMP_IRQn )
		{
			ADC0_THCMP_IRQHandler();
		}
		if ( regVal == ADC0_OVR_IRQn )
		{
			ADC0_OVR_IRQHandler();
		}
		if ( regVal == ADC1_SEQA_IRQn )
		{
			ADC1_SEQA_IRQHandler();
		}
		if ( regVal == ADC1_SEQB_IRQn )
		{
			ADC1_SEQB_IRQHandler();
		}
		if ( regVal == ADC1_THCMP_IRQn )
		{
			ADC1_THCMP_IRQHandler();
		}
		if ( regVal == ADC1_OVR_IRQn )
		{
			ADC1_OVR_IRQHandler();
		}
		if ( regVal == DAC_IRQn )
		{
			DAC_IRQHandler();
		}
		if ( regVal == COMP0_IRQn )
		{
			COMP0_IRQHandler();
		}
		if ( regVal == COMP1_IRQn )
		{
			COMP1_IRQHandler();
		}
		if ( regVal == COMP2_IRQn )
		{
			COMP2_IRQHandler();
		}
		if ( regVal == COMP3_IRQn )
		{
			COMP3_IRQHandler();
		}
		if ( regVal == QEI_IRQn )
		{
			QEI_IRQHandler();
		}
		if ( regVal == RTCALARM_IRQn )
		{
			RTC_ALARM_IRQHandler();
		}
		if ( regVal == RTCWAKE_IRQn )
		{
			RTC_WAKE_IRQHandler();
		}
		if ( regVal == EZH_IRQn )
		{
			EZH_IRQHandler();
		}
		if ( regVal == EZH_MBOX_IRQn )
		{
			EZH_MBOX_IRQHandler();
		}
		if ( regVal == PVT_AMBER_IRQn )
		{
			PVT_AMBER_IRQHandler();
		}
		if ( regVal == PVT_RED_IRQn )
		{
			PVT_RED_IRQHandler();
		}
		NMI_Counter[regVal]++; 
  }
  return;
}
								
/*****************************************************************************
** Function name:		NMI_Init
**
** Descriptions:		NMI initialization
** parameters:			NMI number			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void NMI_Init( uint32_t NMI_num )
{
  uint32_t i;

  for ( i = 0; i < MAX_NMI_NUM; i++ )
  {
    NMI_Counter[i] = 0x0;
  }
  LPC_SYSCON->NMISRC = 0x80000000|NMI_num;
  return;
}

#endif
