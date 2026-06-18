/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * gpio_test.c
 ******************************************************************************/
#include "drivers.h"

/* LEDs definition                                                            */
#define LED1    PIN3        /* PTJ3 */
#define LED2    PIN4        /* PTJ4 */
#define BTN1    PIN1        /* PTD1 */

/* Callback function declaration                                              */
static void port_callback(PORT_CALLBACK_SRC src, uint8 pin);

void main (void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LED1);
  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LED2);
  PORT_Init (PORTD, PORT_MODULE_BUTTON_IRQ_MODE, BTN1);
  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC0_MODE, LED1);
  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC1_MODE, LED2);
  GPIO_Init (GPIOD, GPIO_INP_MODE, BTN1);

  PORT_InstallCallback (PRI_LVL3,port_callback);
  EnableInterrupts();

  while(1);
}

/* Callback function definition                                               */
static void port_callback(PORT_CALLBACK_SRC src, uint8 pin)
{
  if (src == PORTD_CALLBACK)
  {
    if (pin == BTN1)
    {
      GPIO_Tgl (GPIOJ, LED1);  /* standard access via peripheral bridge       */
      GPIO_Tgl (FGPIOJ, LED2); /* single cycle direct access via IOPORT       */
    }
  }
}
