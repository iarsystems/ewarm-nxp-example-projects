/*****************************************************************************
 *   dactest.c:  main C entry file for NXP LPC23xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.08.12  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *  If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
 * this will disable trace port too.
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "target.h"
#include "dac.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
DWORD i = 0;

  TargetResetInit();

  /* Initialize DAC  */
  DACInit();

  while ( 1 )
  {
    DACR = (i << 6) | DAC_BIAS;
    i++;
    if ( i == 1024 )
    {
      i = 0;
    }
  }

  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
