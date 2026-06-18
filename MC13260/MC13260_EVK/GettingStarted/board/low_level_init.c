/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : low_level_init.c
 *    Description : low level init module 
 *
 *    History :
 *    1. Date        : July 22, 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include "board.h"

#pragma segment=".intvec"
/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Low level init code
 *
 *************************************************************************/
__arm void low_level_init (void)
{    
  // ASIC
  ASIC_INTCNTL_bit.FIDIS = 1;  // Disable all fast interrupts
  ASIC_INTCNTL_bit.NIDIS = 1;  // Disable all normal interrupts
  ASIC_INTENABLEH = 0;
  ASIC_INTENABLEL = 0; // all interrupts are disable
  ASIC_INTTYPEH = 0;
  ASIC_INTTYPEL = 0;   // all interrupt lines are routed to the normal interrupt
  ASIC_NIMASK = 0x10;  // Do not disable any normal interrupts
}
