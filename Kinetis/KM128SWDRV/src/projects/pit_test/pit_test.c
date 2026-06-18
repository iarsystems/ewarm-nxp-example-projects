/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pit_test.c
 ******************************************************************************/
#include "drivers.h"

/* PIT callback declarations                                                  */
void pit_callback (PIT_CALLBACK_TYPE type);

void main(void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  PORT_Init (PORTE, PORT_MODULE_ALT1_MODE, PIN5);
  GPIO_Init (GPIOE, GPIO_OUT_LOGIC1_MODE , PIN5);
  PIT_InstallCallback (PRI_LVL0,pit_callback);
  PIT_Init (PIT0, CH0, PIT_CH_TMR_EN_CONFIG, 1000000);

  EnableInterrupts();

  while(1);
}

/* PIT callback definition                                                    */
void pit_callback (PIT_CALLBACK_TYPE type)
{
  if (type == PIT0CH0_CALLBACK) { GPIO_Tgl (GPIOE, PIN5); }
}
/***************************************************************************
 * END OF MODULE
 ****************************************************************************/
