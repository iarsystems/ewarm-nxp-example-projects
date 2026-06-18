/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : init.c
 *    Description : Low-Level Init Functions
 *
 *    History :
 *    1. Date        : October 2014
 *       Author      : Stanimir Bonev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "NXP\iolpc54xxx.h"
#include "stdint.h"

#pragma section=".intvec"

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: 1 - continue with data initialization
 *         0 - skip data initialization and go to main
 *
 * Description: Low-level initialization.
 *              Initializes vector table register and tunes EMC.
 *              Configures EMC address lines A14-A22
 *
 *************************************************************************/
int __low_level_init(void)
{
  VTOR = (uint32_t)__section_begin(".intvec");
  return 1;
}
