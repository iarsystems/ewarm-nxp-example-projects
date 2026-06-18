/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_MMA7660FC.c
 *    Description : MMA7660FC driver
 *
 *    History :
 *    1. Date        : 3 February 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_MMA7660FC.h"
#include "drv_i2c1.h"

/*************************************************************************
 * Function Name: MMA7660FC_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: MMA7660FC initialization
 *
 *************************************************************************/
Boolean MMA7660FC_Init (void)
{
Boolean status;
Int8U data[3];

  // Set mode and sample rate
  data[0] = 0x10;   // interrupt after every measurement
  data[1] = 0x01;   // active mode
  data[2] = 0xE1;   // 120 sample per second
  status = I2C1_Transfer(MMA7660FC_SLAVE_ADDR,MMA7660FC_INTSU,data,3);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: MMA7660FC_GetAccData
 * Parameters: pAcc_Data_t pData
 *
 * Return: Boolean
 *
 * Description: Get Data form MAA7660FC initialization
 *
 *************************************************************************/
Boolean MMA7660FC_GetAccData (pAcc_Data_t pData)
{
Boolean status;
Int8U data[3];

  do
  {
    status = I2C1_Transfer(MMA7660FC_SLAVE_ADDR | I2C_READ_DATA,MMA7660FC_XOUT,data,3);
    if (I2cComplete != status)
    {
      return(FALSE);
    }
  }
  while(0x40 & (data[0] | data[1] | data[2]));

  for(Int32U i = 0; 3 > i; i++)
  {
    if(0x20 & data[i])
    {
      // negative number
      data[i] |= 0xC0;
    }
  }

  pData->X = data[0];
  pData->Y = data[1];
  pData->Z = data[2];
  return(TRUE);
}
