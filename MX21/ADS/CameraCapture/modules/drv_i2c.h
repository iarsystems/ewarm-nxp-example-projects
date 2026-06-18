/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_i2c.h
 *    Description : The I2C driver include file
 *
 *    History :
 *    1. Date        : 14 May, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __DRV_I2C_H
#define __DRV_I2C_H

typedef enum
{
  I2cComplete = 0, I2cBusy,  I2cNak, I2cArbLost, I2cParametersError
} I2cStatus_t;

typedef enum
{
  I2cIdle = 0, I2cSlaveAdd,  I2cSend,
  I2cRepeatedStart, I2cRepeatedSlaveAdd, I2cReceive
} I2cState_t;

// I2c control structure
typedef struct
{
  volatile Int32U  Status;
  Int32U  State;
  Int32U  DataSize;
  pInt8U  pData;
  Int8U   Add;
  Int8U   SlaveAdd;
} I2C_Ctrl_t;

/*************************************************************************
 * Function Name: I2C_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C module
 *
 *************************************************************************/
void I2C_Init (void);

/*************************************************************************
 * Function Name: I2C_Transfer
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             pInt8U pData, Int32U DataSize
 *
 * Return: Boolean
 *          TRUE - OK
 *          FALSE - Fault
 *
 * Description: Read/Write data from/to the I2C
 *
 *************************************************************************/
Boolean I2C_Transfer(Int8U SlaveAdd, Int8U Add, pInt8U pData, Int32U DataSize);

#endif // __DRV_I2C_H
