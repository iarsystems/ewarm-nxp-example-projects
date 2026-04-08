/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lptmrisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* LPTMR callback function declaration                                        */
static void lptmr_callback (void);

/* static data definition                                                     */
static volatile uint16 counter;

void main (void)
{
  /* core, bus and flash clock = 1000 KHz                                     */
  SIM_SetClkMode (SYSCLK_MODE0);
  SIM_SetClkDiv  (SYSCLK_DIV2);

  /* enable clock to LPTMR on-chip peripheral                                 */
  SIM_EnableModule (LPTMR);

  /* enable low voltage reset detection (1.6V), disable interrupts and bandgap*/
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);

  /* enable transitions for VLPR and VLPS mode                                */
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_OFF_CONFIG);
  OSC32K_Disable ();      /* disable OSC32K oscillator                        */
  SMC_SetMode(VLPR);      /* enter VLPR mode to minimize current consumption  */

  /* initialize LPTMR in counter reset mode                                   */
  LPTMR_Init(LPTMR_MODULE_TMR_CNT_RST_ONTCF_MODE_CONFIG(0,LPTMR_LPOCLK),3999);
  LPTMR_InstallCallback (PRI_LVL1, lptmr_callback);
  EnableInterrupts();     /* enable interrupts on global level                */
  LPTMR_Enable();         /* enable LPTMR (8000ms interrupt)                  */

  while (1)
  {
    SMC_SetMode(VLPW);    /* keep staying in VLPW mode; IDD+IDDA+IBAT=130.0uA */
    SMC_SetMode(VLPS);    /* keep staying in VLPS mode; IDD+IDDA+IBAT=4.5uA   */
  }
}

/* LPTMR interrupt - called every 8000 ms                                     */
static void lptmr_callback (void)
{
    counter++;            /* increment variable; IDD+IDDA+IBAT=177uA          */
    arch_delay(4000000);  /* wait for 4000 ms                                 */
}
