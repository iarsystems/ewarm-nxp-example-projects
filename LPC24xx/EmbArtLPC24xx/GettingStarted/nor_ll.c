/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : mor_ll.c
 *    Description : Embedded Artists LPC24xx NOR low level init
 *
 *    (c) Copyright IAR Systems 2011
 *
 *
 *    History :
 *    1. Date        : October 25, 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "board.h"

#pragma segment=".iflash"

/*************************************************************************
 * Function Name: low_level_init
 * Parameters: void
 * Return: void
 *
 * Description: init EMC and NOR flash interface
 * note: system max clock is 48MHz
 *
 *************************************************************************/
void low_level_init(void) @ ".iflash"
{
	// EMS init
  PCONP |= 1<<11;
  EMCCONTROL         = 1;
  EMCSTATICCNFG0     = 0x81;
  EMCSTATICWAITWEN0  = 0;   // <= 0ns
  EMCSTATICWAITOEN0  = 0;   // <= 0ns
  EMCSTATICWAITRD0   = 4;   // <= 90ns
  EMCSTATICWAITWR0   = 2;   // <= 40ns
  EMCSTATICWAITTURN0 = 2;   // <= 30ns

  // Assign GPIO to SMC A1-A23, EO, WE, CS0, D0-15
  PINSEL6 = 0x55555555;
  PINSEL8 = 0x55555554;
  PINSEL9 = 0x10055555;
}

