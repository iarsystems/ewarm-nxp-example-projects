/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : low_level_init.c
 *    Description : Low level init procedure
 *
 *    History :
 *    1. Date        : 08, September 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
  unsigned int reg;
  /*Disable WWDT*/
  reg = WDOG_STCTRLH;
  reg &= ~1UL;
  /*Unlock sequence*/
  WDOG_UNLOCK  = 0xC520;
  WDOG_UNLOCK  = 0xD928;
  WDOG_STCTRLH = reg;
  /* perform initialization*/
#if defined(K40) || defined(K20)
  /* DIV1-4 = /1*/
  SIM_CLKDIV1 = 0x00000000;
  MCG_C2 = (MCG_C2 & MCG_C2_RANGE_MASK) | MCG_C2_RANGE(1) | MCG_C2_EREFS_MASK;
#else
  /* DIV1-4 = /2*/
  SIM_CLKDIV1 = 0x11110000;
#endif

  MCG_C1 = (MCG_C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) | MCG_C1_CLKS(2);

  return 1;
}

