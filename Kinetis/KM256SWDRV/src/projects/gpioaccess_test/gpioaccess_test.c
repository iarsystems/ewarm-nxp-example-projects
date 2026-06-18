/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * gpioaccess_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* Set bus masters attribute to be controlled internally by the core        */
  MCM_SetMasterAttr (MCM_CM0_MASTER|MCM_DMA_MASTER,
                     MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC,
                     TRUE);

  /* User nonsec=Read/Write, User secure=Read/Write, Priv. secure=Read/Write. */
  PORT_Init (PORTA, PORT_MODULE_ALT1_MODE, PIN0);
  GPIO_Init (GPIOA, GPIO_OUT_LOGIC1_MODE, PIN0);
  GPIO_SetPortAttr (GPIOA, GPIO_PORT_RW_RW_RW, TRUE);

  /* User nonsec=None, User secure=None, Priv. secure=Read/Write.             */
  PORT_Init (PORTB, PORT_MODULE_ALT1_MODE, PIN0);
  GPIO_Init (GPIOB, GPIO_OUT_LOGIC1_MODE, PIN0);
  GPIO_SetPortAttr (GPIOB, GPIO_PORT_NO_NO_RW, TRUE);

  /* Toggling pins in privileged mode                                         */
  GPIO_Tgl (FGPIOA, PIN0);  /* passes - access via CPU's IOPORT               */
  GPIO_Tgl (GPIOA, PIN0);   /* passes - access via Peripheral bridge          */
  GPIO_Tgl (FGPIOB, PIN0);  /* passes - access via CPU's IOPORT               */
  GPIO_Tgl (GPIOB, PIN0);   /* passes - access via Peripheral bridge          */

  /* Toggling in user mode                                                    */
  UserMode();
  GPIO_Tgl (FGPIOA, PIN0);  /* passes - access via CPU's IOPORT               */
  GPIO_Tgl (GPIOA, PIN0);   /* passes - access via Peripheral bridge          */
  GPIO_Tgl (FGPIOB, PIN0);  /* RAZ/WI error - access via CPU's IOPORT         */
  GPIO_Tgl (GPIOB, PIN0);   /* hard fault error - access via Peripheral bridge*/

  while(1);
}
