/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * osc32k_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route RTC 32KHz clock source to PTF7 for monitoring                      */
  SIM_SelClkout (CLKOUT_SRC5);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* check control macros                                                     */
  OSC32K_Disable();
  OSC32K_Enable ();

  /* adjust load capacitors                                                   */
  OSC32K_SetLoadCaps (OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC4P);
  OSC32K_SetLoadCaps (OSC32K_SC4P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC8P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC8P|OSC32K_SC4P);
  OSC32K_SetLoadCaps (OSC32K_SC8P|OSC32K_SC4P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC16P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC4P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC4P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC8P|OSC32K_SC2P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC8P|OSC32K_SC4P);
  OSC32K_SetLoadCaps (OSC32K_SC16P|OSC32K_SC8P|OSC32K_SC4P|OSC32K_SC2P);

  while(1);
}
