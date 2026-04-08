/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : drv_MC34704.c
 *    Description : i.MX25 MC34704 driver
 *
 *    History :
 *    1. Date        : 16 October 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_MC34704.h"
#include "drv_i2c1.h"

/*************************************************************************
 * Function Name: MC34704_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: MC34704 initialization
 *
 *************************************************************************/
Boolean MC34704_Init (void)
{
Boolean status;
Int8U data;

  // Enable all groups regulators
  data = 0x0F;
  status = I2C1_Transfer(MC34704_SLAVE_ADDR,GENERAL2,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  return(TRUE);
}
