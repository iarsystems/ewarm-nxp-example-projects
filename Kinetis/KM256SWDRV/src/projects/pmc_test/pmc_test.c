/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pmc_test.c
 ******************************************************************************/
#include "drivers.h"

/* LEDs definition                                                            */
#define LVW_WARN_LED    PIN3        /* PTJ3 */
#define LVD_WARN_LED    PIN4        /* PTJ4 */

/* callback function declaration                                              */
static void pmc_callback (PMC_CALLBACK_TYPE type);

void main (void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  PMC_Init (PMC_MODULE_LVDRE_OFF_LVDINT_ON_LVWINT_ON_CONFIG(PMC_LVDH,PMC_LVW4),
            PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG, PRI_LVL3, pmc_callback);
  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LVW_WARN_LED);
  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LVD_WARN_LED);
  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC1_MODE, LVW_WARN_LED);
  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC1_MODE, LVD_WARN_LED);
  EnableInterrupts();

  while(1);
}

/* callback function definition                                               */
static void pmc_callback (PMC_CALLBACK_TYPE type)
{
  if (type == LVW_CALLBACK)
  {
    GPIO_Tgl (GPIOJ, LVW_WARN_LED); /* toggles when voltage below 3.00 V      */
    arch_delay (400000l);
  }
  if (type == LVD_CALLBACK)         /* toggles when voltage below 2.56 V      */
  {
    GPIO_Tgl (GPIOJ, LVD_WARN_LED);
    arch_delay (400000l);
  }
}
