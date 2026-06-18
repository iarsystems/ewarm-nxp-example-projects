/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : buttons.c
 *    Description :
 *
 *    History :
 *    1. Date: June 15th, 2006
 *       Author: Todor Atanasov
 *       Description: Created
 *
 *    $Revision: 28 $
 **************************************************************************/

#include <NXP/iolpc2129.h>
#include "buttons.h"

// init buttons port
void InitButtons(void)
{
  // Assign pins to the Ext Interrup logic
  PINSEL0_bit.P0_15 = 2;        // EINT2  (Button B1)
  PINSEL0_bit.P0_9 = 3;         // EINT3  (Button B2)

  // Clear interrupt flag
  EXTINT_bit.EINT2 = 1;
  EXTINT_bit.EINT3 = 1;

  // Set Wakeup DISABLED
  EXTWAKE_bit.EXTWAKE2 = 0;
  EXTWAKE_bit.EXTWAKE3 = 0;

  IO0DIR_bit.P0_15 = 0;         //set port0.15 as input
  IO0DIR_bit.P0_9 = 0;          //set port0.9 as input
}

