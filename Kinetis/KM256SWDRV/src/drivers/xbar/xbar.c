/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      xbar.c
 * @version   1.0.3.0
 * @date      Jan-05-2015
 * @brief     Inter-Peripheral Crossbar Switch (XBAR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "xbar.h"

#if !defined(MCU_MKM34Z7)
  #error "The xbar.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static XBAR_CALLBACK pCallbackXBAR;

/******************************************************************************
 * public function definitions                                                *
 ******************************************************************************/
void XBAR_Init (tXBAR out0, tXBAR out1, tXBAR out2, tXBAR out3, uint16 ip,
                XBAR_CALLBACK pCallback)
{
  if (pCallback) { pCallbackXBAR = pCallback; NVIC_SetIsr(INT_XBAR,ip); }
  XBAR_CTRL0 = (out0.CTRL | (out1.CTRL << 8));
  XBAR_CTRL1 = (out2.CTRL | (out3.CTRL << 8));
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
weak void xbar_isr (void)
{
  if(XBAR_CTRL0 & XBAR_CTRL0_STS0_MASK)
  {
    ioclrh (XBAR_CTRL0,XBAR_CTRL0_STS1_MASK); /* clear STS0 but not STS1 flag */
    pCallbackXBAR(XBAR_DMA0_CALLBACK);
  }
  if(XBAR_CTRL0 & XBAR_CTRL0_STS1_MASK)
  {
    ioclrh (XBAR_CTRL0,XBAR_CTRL0_STS0_MASK); /* clear STS1 but not STS0 flag */
    pCallbackXBAR(XBAR_DMA1_CALLBACK);
  }
  if(XBAR_CTRL1 & XBAR_CTRL1_STS2_MASK)
  {
    ioclrh (XBAR_CTRL1,XBAR_CTRL1_STS3_MASK); /* clear STS2 but not STS3 flag */
    pCallbackXBAR(XBAR_DMA2_CALLBACK);
  }
  if(XBAR_CTRL1 & XBAR_CTRL1_STS3_MASK)
  {
    ioclrh (XBAR_CTRL1,XBAR_CTRL1_STS2_MASK); /* clear STS3 but not STS2 flag */
    pCallbackXBAR(XBAR_DMA3_CALLBACK);
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
