/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * dmap2m_test.c
 ******************************************************************************/
#include "drivers.h"

/* static data definitions                                                    */
static int32 volatile samples[3000];

/* DMA callback prototype                                                     */
static void dma0_callback (DMA_CH_CALLBACK_TYPE type);

void main (void)
{
  /* Initialize SMC and SIM modules                                           */
  SIM_Init (SIM_MODULE_LP_CONFIG);
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_OFF_CONFIG);

  /* Clock mode 2:1:1, 2.00 MHz (chip is trimmed)                             */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init (FLL_MODULE_BLPI_2MHZ_CONFIG);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_DI);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */

  PMC_SetBGEN ();                                 /* enable VREF in VLPR mode */
  SMC_SetMode (VLPR);                             /* transition to VLPR mode  */

  /* Install DMA0 callback and initialize DMA0 channel                        */
  DMA_InstallCallback (DMA0, PRI_LVL3, dma0_callback);
  DMA_Init (DMA0,DMA_CH_HWTRG_P2M_CS_CONFIG(DMA_REQ_AFE_CH0,DMA_SIZE32BIT,
                                          &AFE_CH0_RR,samples,sizeof(samples)));

  /* Initialize AFE_CH0                                                       */
  AFE_ChInit (CH0,AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024),0,PRI_LVL0,NULL);
  AFE_Init   (AFE_MODULE_LPM_RJFORMAT_CONFIG(AFE_IRC_CLK, AFE_DIV4, 2000000));
  AFE_EnableDMA (CH0);

  /* Enable interrupts                                                        */
  EnableInterrupts();

  /* Trigger AFE conversions                                                  */
  AFE_SwTrigger (CH0);

  /* Main loop                                                                */
  while (1)
  {
    /* Transition to VLPS mode                                                */
    SMC_SetMode (VLPS);       /* Power consumption: 1.01mA@VLPS; 1.04mA@STOP  */

    /* Wait for 3 seconds to enable current measurement                       */
    arch_delay  (3000000);    /* Power consumption: 1.22mA@VLPR; 2.29mA@RUN   */

    /* Re-initialize DMA0 for peripheral to memory transfer                   */
    DMA_Init (DMA0,DMA_CH_HWTRG_P2M_CS_CONFIG(DMA_REQ_AFE_CH0,DMA_SIZE32BIT,
                                          &AFE_CH0_RR,samples,sizeof(samples)));
  }
}

/* DMA callback function definition                                           */
static void dma0_callback (DMA_CH_CALLBACK_TYPE type)
{
/* wakeup by IRQ */
}
