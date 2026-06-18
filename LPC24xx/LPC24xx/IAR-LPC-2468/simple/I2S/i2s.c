/*****************************************************************************
 *   i2s.c:  I2S C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "irq.h"
#include "i2s.h"
#include "intrinsics.h"

/*****************************************************************************
** Function name:   I2SInit
**
** Descriptions:    Initialize I2S controller
**
** parameters:      None
** Returned value:    true or false, return false if the I2S
**            interrupt handler was not installed correctly
**
*****************************************************************************/
DWORD I2SInit( void )
{

  /*enable I2S in the PCONP register. I2S is disabled on reset*/
  PCONP |= (1 << 27);

  /*connect the I2S sigals to port pins
    P0.7 - TX_CLK, P0.8 - TX_WS, P0.9 - TX_SDA */

  PINSEL0_bit.P0_7 = 1;
  PINSEL0_bit.P0_8 = 1;
  PINSEL0_bit.P0_9 = 1;

  /* For all the test program assuming USB is used, the CCLK is set to
  48Mhz while all the PCLK are the same, 1/2 of CCLK = 16Mhz. It also
  applys to I2S. To generate 48khz sample rate for 16-bit stereo data
  requires a bit rate of 48,000 x 16 x 2 = 1.536Mhz, the TX and RX clock rate
  register should be 12000000 / 1536000 ~ 8.

  Please note, in order to generate accurate TX/RX clock rate for I2S,
  PCLK and CCLK needs to be carefully reconsidered. For this test
  program, the TX is looped back to RX without external I2S device,
  clock rate is not critical in this matter. */
  I2STXRATE = 8;

  /* Audio output is the master, audio input is the slave, */
  I2SDAO_bit.RESET      = 1;  // reset FIFIO
  I2SDAO_bit.WORS_WIDTH = 1;  // 16 bits
  I2SDAO_bit.MONO       = 0;  // Stereo
  I2SDAO_bit.WS_SEL     = 0;  // Master mode
  I2SDAO_bit.WS_SEL     = 0;  // Master mode
  I2SDAO_bit.WS_HALFPERIOD = 0x10;
  I2SDAO_bit.RESET      = 0;  // release reset
  I2SDAO_bit.MUTE       = 0;  // release mute
  I2SDAO_bit.STOP       = 0;  // start I2S
  return( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

