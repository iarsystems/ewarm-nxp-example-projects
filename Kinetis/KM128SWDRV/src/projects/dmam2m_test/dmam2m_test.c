/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * dmam2m_test.c
 ******************************************************************************/
#include "drivers.h"

static uint8 volatile srcbuff[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
static uint8 volatile dstbuff[20];

void main (void)
{
  SIM_EnableModule (DMAMUX0); /* Enable DMAMUX0                               */

  /* Initialize memory to memory simple continuous transfer on DMA channel 0  */
  DMA_Init (DMA0,DMA_CH_SWTRG_M2M_CN_CONFIG(srcbuff,dstbuff,sizeof(srcbuff)));
  while (!DMA_IsDone(DMA0));  /* Wait until transfer is completed             */
  DMA_Ack(DMA0);              /* Acknowledge completed transfer               */

  while(1);
}
