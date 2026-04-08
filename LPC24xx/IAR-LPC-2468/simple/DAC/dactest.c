/*****************************************************************************
 *   dactest.c:  main C entry file for NXP LPC23xx/LPC24xx Family Microprocessors
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
 *
 * The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 
 *
 ******************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "target.h"
#include "dac.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
DWORD i = 0;
long Add = 1;
  TargetResetInit();

  /* Initialize DAC  */
  DACInit();

  while ( 1 )
  {
    DACR = (i << 6) | DAC_BIAS;
    i += Add;
    if ( i == 1023  || i == 0)
    {
      if(Add == 1)
      {
        Add = -1;
      }
      else
      {
        Add = 1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
