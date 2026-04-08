/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * llwu_test.c
 ******************************************************************************/
#include "drivers.h"

/* Button definitions                                                         */
#define BTN_VLPS_WKUP PIN0  /* PTD0; SW1  on TWR-KM34Z50M */
#define LED           PIN1  /* PTD1; LED3 on TWR-KM34Z50M */

/* Callback function declaration                                              */
static void port_callback (PORT_CALLBACK_SRC src, uint8 pin);

/******************************************************************************/
void main (void)
{
  PMC_Init (PMC_MODULE_LVDRE_OFF_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG);

  IRTC_Init(IRTC_MODULE_COMP_OFF_CONFIG(LFSR_CLK_2HZ, 4),
            IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_2HZ,4),
            IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_2HZ,4),
            IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_2HZ,4),
            0);

  SIM_EnableModule(PORTD);
  PORT_Init (PORTD, PORT_MODULE_BUTTON_IRQ_MODE, BTN_VLPS_WKUP);
  GPIO_Init (GPIOD, GPIO_INP_MODE, BTN_VLPS_WKUP);

  PORT_Init (PORTD, PORT_MODULE_LED_MODE, LED);
  GPIO_Init (GPIOD, GPIO_OUT_LOGIC1_MODE, LED);

  if (PMC_GetACKISO()) { PMC_SetACKISO (); }  /* release I/O states           */

  /* toggle LED to notify reset condition                                     */
  GPIO_Clr (GPIOD, LED);
  arch_delay (5000000l);
  GPIO_Set (GPIOD, LED);
  arch_delay (5000000l);

  /* install PORTx interrupt                                                  */
  PORT_InstallCallback (PRI_LVL3, port_callback);

  LLWU_Init(LLWU_MODULE_NEGEDGE_ONLY_DETECT_CONFIG(LLWU_PTD0), PRI_LVL0, NULL);

  EnableInterrupts();

  /* main programme loop                                                      */
  while (1)
  {
//    SMC_SetMode(STOP);                /* IDD=311.1uA, IBAT=1.9uA            */
//    SMC_SetMode(VLPS_PSTOP1);         /* IDD=  5.0uA, IBAT=1.9uA            */
//    SMC_SetMode(VLPS_PSTOP2);         /* IDD=  5.0uA, IBAT=1.9uA            */
    SMC_SetMode(VLPS);                /* IDD=  4.9uA, IBAT=1.9uA            */
//    SMC_SetMode(VLLS3);               /* IDD=  1.9uA, IBAT=1.9uA            */
//    SMC_SetMode(VLLS2);               /* IDD=  1.8uA, IBAT=1.9uA            */
//    SMC_SetMode(VLLS1);               /* IDD=  1.3uA, IBAT=1.9uA            */
//    SMC_SetMode(VLLS0_POR_ON );       /* IDD=  0.7uA, IBAT=1.9uA            */
//    SMC_SetMode(VLLS0_POR_OFF);       /* IDD=  0.4uA, IBAT=1.9uA            */
  }
}

/* callback functions                                                         */
void port_callback (PORT_CALLBACK_SRC src, uint8 pin)
{
  if (src == PORTD_CALLBACK)
  {
    if (pin == BTN_VLPS_WKUP)
    {
      GPIO_Clr (GPIOD, LED);
      arch_delay (1000000l);
      GPIO_Set (GPIOD, LED);
    }
  }
}
