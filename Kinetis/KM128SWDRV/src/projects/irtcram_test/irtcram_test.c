/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * irtcram_test.c
 ******************************************************************************/
#include "drivers.h"

/* LPTMR callback function declaration                                        */
static void lptmr_callback (void);

/* static data definition                                                     */
static uint32 counter_rtc;
static uint32 counter_ram;

void main (void)
{
  /* enable peripheral clocks                                                 */
  SIM_EnableModule (LPTMR);
  SIM_EnableModule (UART1);
  SIM_EnableModule (PORTI);

  /* enable low voltage reset detection (1.6V), disable interrupts and bandgap*/
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_POLLMODE_CONFIG(9600,2e6));
  FMSTR_Init();

  /* initialize LPTMR in counter reset mode                                   */
  LPTMR_Init(LPTMR_MODULE_TMR_CNT_RST_ONTCF_MODE_CONFIG(0,LPTMR_LPOCLK),5);
  LPTMR_InstallCallback (PRI_LVL1, lptmr_callback);
  EnableInterrupts();     /* enable interrupts on global level                */
  LPTMR_Enable();         /* enable LPTMR                                     */

  while (1)
  {
    FMSTR_Poll ();
    /* read variable from RTC register file - takes 75 us @ 2 MHz core clock  */
    IRTC_RdRam ((uint8*)&counter_ram, sizeof(counter_ram));
  }
}

/* LPTMR interrupt - called every 10 ms                                       */
static void lptmr_callback (void)
{
  counter_rtc++;          /* increment variable                               */
  /* store variable in RTC register file - takes 143 us @ 2 MHz core clock    */
  IRTC_WrRam ((uint8*)&counter_rtc,sizeof(counter_rtc));
}
