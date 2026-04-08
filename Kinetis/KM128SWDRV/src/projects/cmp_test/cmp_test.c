/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * cmp_test.c
 ******************************************************************************/
#include "drivers.h"

/* CMP callback declarations                                                  */
void cmp_callback (CMP_CALLBACK_SRC module, CMP_CALLBACK_TYPE type);

/* LEDs definition                                                            */
#define LED_G  PIN5        /* PTE5 */
#define LED_R  PIN1        /* PTF1 */

void main(void)
{
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

   /* clock mode 2:1:1, 48MHz                                                 */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* PORTs init                                                               */
  PORT_Init (PORTE, PORT_MODULE_LED_MODE, LED_G);
  PORT_Init (PORTF, PORT_MODULE_LED_MODE, LED_R);
  GPIO_Init (GPIOE, GPIO_OUT_LOGIC0_MODE, LED_G);
  GPIO_Init (GPIOF, GPIO_OUT_LOGIC0_MODE, LED_R);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);

  /* Comparator module initialized                                            */
  CMP_Init  (CMP0,
             CMP_MODULE_EN_FILTER7_SAMPLE_PER_255_DAC_EN_CONFIG_BOTH_EDGES(32),
             IN0,
             IN7_DAC);
  CMP_InstallCallback (PRI_LVL1, (CMP_CALLBACK)cmp_callback);

  /* enable interrupts on global level                                        */
  EnableInterrupts();

  /* main loop                                                                */
  while(1);
}

/* CMP callbacks definitions                                                  */
void cmp_callback (CMP_CALLBACK_SRC module, CMP_CALLBACK_TYPE type)
{
  if (module & CMP0_CALLBACK)
  {
    if(type & CMP_NEGEDGE_CALLBACK)
    {
      GPIO_Tgl (GPIOE, LED_G);
    }
    if(type & CMP_POSEDGE_CALLBACK)
    {
      GPIO_Tgl (GPIOF, LED_R);
    }
  }
}
