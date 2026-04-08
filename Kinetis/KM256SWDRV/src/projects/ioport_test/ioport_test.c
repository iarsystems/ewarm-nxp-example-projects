/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * ioport_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* SYSCLK:BUSCLK:FLASHCLK = 1:1:1                                           */
  SIM_SetClkMode(SYSCLK_MODE0);
  SIM_EnableModule(PORTF);
  PORT_Init (PORTF, PORT_MODULE_LED_MODE, PIN1|PIN2);
  GPIO_Init (GPIOF, GPIO_OUT_LOGIC0_MODE, PIN1|PIN2);

  while(1)
  {
    /* PTF1 pin controlled via Peripheral Bridge (AIPS-Lite)                  */
    GPIO_Set (GPIOF , PIN1);
    GPIO_Clr (GPIOF , PIN1);

    /* PTF2 pin controlled via IOPORT                                         */
    GPIO_Set (FGPIOF, PIN2);
    GPIO_Clr (FGPIOF, PIN2);
  }
}
