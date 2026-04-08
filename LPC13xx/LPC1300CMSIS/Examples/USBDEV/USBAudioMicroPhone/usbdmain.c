/*****************************************************************************
 *   pwmtest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC13xx.h"                        /* LPC23xx/24xx definitions */
#include "type.h"
#include "lpc13xx_timer.h"
#include "lpc13xx_gpio.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"

BYTE  Mute;                                 /* Mute State */
DWORD Volume;                               /* Volume Level */

#if USB_DMA
  #if KEIL_IDE
  DWORD InfoBuf[P_C] __at  DMA_BUF_ADR;       /* Packet Info Buffer */
  short DataBuf[B_S] __at (DMA_BUF_ADR+4*P_C);/* Data Buffer */
  #else
  DWORD *InfoBuf = (DWORD *)(DMA_BUF_ADR);
  short *DataBuf = (short *)(DMA_BUF_ADR + 4*P_C);
  #endif
#else
short DataBuf[B_S];                         /* Data Buffer */
#endif

WORD  DataOut;                              /* Data Out Index */
WORD  DataIn;                               /* Data In Index */

BYTE  DataRun;                              /* Data Stream Run State */

WORD  PotVal;                               /* Potenciometer Value */

DWORD VUM;                                  /* VU Meter */

DWORD Tick;                                 /* Time Tick */


TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;


/*****************************************************************************
**   Main Function  main()
******************************************************************************/

int main (void) {

  SystemInit();

 /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  LPC_IOCON->PIO0_1 &= ~0x07;
  LPC_IOCON->PIO0_1 |= 0x01;		/* CLK OUT */

  /* Enable AHB clock to the USB block. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<14);
  LPC_IOCON->PIO0_3      &= ~0x1F;
  LPC_IOCON->PIO0_3      |= 0x01;		/* Secondary function VBUS */
  LPC_IOCON->PIO0_6   &= ~0x07;
  LPC_IOCON->PIO0_6   |= 0x01;		/* Secondary function SoftConn */

//  /* P0.1 is push-button input, P2.0~3 are LED output. */
//  GPIO2->DIR |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
//  GPIO0->DIR &= ~(1<<1);


// Initialize timer 0, prescale count time of 1ms
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will not reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//do no thing for external output
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	// Set Match value, count value is time (timer * 1000uS =timer mS )
	TIM_MatchConfigStruct.MatchValue   = 1000;


  /* 16-bit timer 0. */
  TIM_Init(LPC_TMR16B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
  TIM_ConfigMatch(LPC_TMR16B0,&TIM_MatchConfigStruct);
//  NVIC_EnableIRQ(TIMER_16_0_IRQn);
  TIM_Cmd(LPC_TMR16B0,ENABLE);





  /* Set port 3_0 to output */
  GPIO_SetDir( 3, 0, 1 );
  GPIO_SetValue( 3, 0);
  GPIO_ClearValue( 0, 3);
  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1)                                /* Loop forever */
  {
	  // Wait for 1000 millisecond
	  		while ( !(TIM_GetIntStatus(LPC_TMR16B0,0)));

	  		{
	  		TIM_ClearIntPending(LPC_TMR16B0,0);
	  		GPIO_SetValue(PORT3, 0);
	  		}

	  		// Wait for 1000 millisecond
	  		while ( !(TIM_GetIntStatus(LPC_TMR16B0,0)));
	  		{
	  		TIM_ClearIntPending(LPC_TMR16B0,0);
	  		GPIO_ClearValue(PORT3, 0);
	  		}

  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
