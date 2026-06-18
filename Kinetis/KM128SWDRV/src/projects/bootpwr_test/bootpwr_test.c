/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * bootpwr_test.c
 ******************************************************************************/
#include "drivers.h"

/* RESETB PIN definition                                                      */
#define PIN_RESETB  PIN1                                              /* PTE1 */

void main (void)
{
  SIM_EnableModule (PORTE);
  PORT_Init (PORTE , PORT_MODULE_ALT1_MODE, PIN_RESETB);
  GPIO_Init (FGPIOE, GPIO_OUT_LOGIC1_MODE , PIN_RESETB);

  GPIO_Clr(FGPIOE, PIN_RESETB);
  arch_delay (0x20);
  GPIO_Set(FGPIOE, PIN_RESETB);

  /* main programme loop                                                      */
  while(1);
}
