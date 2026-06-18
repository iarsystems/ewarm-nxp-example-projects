//This is a LPC2124 or LPC2129 timer interupt demonstration code -LED toggling is done inside hardware.c
/*-------------------------------------------------------------------------------------------------------------------------------*/

#include <NXP/iolpc2129.h> //Needed by Macros only usually
#include "hardware.h"  //Supplies Hardware specific macros (e.g. S_BUT1)

#ifndef SRAM_VIA_JLINK
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;
#endif

/*-------------------------------------------------------------------------------------------------------------------------------*/
int main(void)
{
  CPUinit();
  __disable_interrupt();
  INTERRUPTSinit();
  __enable_interrupt();
  TIMER0initstart();

  A_LED1_OFF;
  A_LED2_OFF;
  while(1)
  {
    //Force LEDs on by pushing Buttons 1,2
    if (S_BUT1==DOWN) A_LED1_ON;
    if (S_BUT2==DOWN) A_LED2_ON;
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/

