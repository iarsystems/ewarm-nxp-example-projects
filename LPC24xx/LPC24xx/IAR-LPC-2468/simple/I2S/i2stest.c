/*****************************************************************************
 *   i2stest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 
 *
 ******************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "i2s.h"
#include "target.h"

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
#define STEP  0x100
int i = 0;
long add = STEP;
  TargetResetInit();

  /* The test program is to connect CLK, WS, and SDA lines between the
  TX and HT82V731. DAO is the master output. It generate signal with triangle
  shape */


  if ( I2SInit() == FALSE ) /* initialize I2S */
  {
    while ( 1 );      /* Fatal error */
  }

  while(1)
  {
    while (I2SSTATE_bit.TX_LEVEL == TXFIFO_FULL);
    I2STXFIFO = i + ((0xFFFF - i) << 16);

    i += add;
    if((i >  (32767 - STEP)) || (i < (-32768 + STEP)))
    {
      if(add == STEP)
      {
        add = -STEP;
      }
      else
      {
        add = STEP;
      }
    }
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
