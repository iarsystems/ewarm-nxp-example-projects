/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : led.c
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
#include "led.h"

// init led port
void LedsInit(void) {
  IO0DIR_bit.P0_8 = 1;     //set port0.8 to output
  IO0DIR_bit.P0_10 = 1;    //set port0.10 to output
  IO0DIR_bit.P0_11 = 1;    //set port0.11 to output

  IO0SET_bit.P0_8 = 1;     //LED1 OFF
  IO0SET_bit.P0_10 = 1;    //LED2 OFF
  IO0SET_bit.P0_11 = 1;    //LED3 OFF
}
