/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : drv_i2c1.h
 *    Description : I2C1 driver include file
 *
 *    History :
 *    1. Date        : 16 October, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

#ifndef __DRV_I2C1_H
#define __DRV_I2C1_H

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
  volatile I2cStatus_t Status;
  Int32U      State;
  Int32U      DataSize;
  pInt8U      pData;
  Int8U       Add;
  Int8U       SlaveAdd;
} I2C_Ctrl_t;

#define I2C_READ_DATA   1

/*************************************************************************
 * Function Name: I2C1_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C1 module
 *
 *************************************************************************/
void I2C1_Init (void);

/*************************************************************************
 * Function Name: I2C1_Transfer
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             pInt8U pData, Int32U DataSize
 *
 * Return: I2cStatus_t
 *
 * Description: Read/Write data from/to the I2C
 *
 *************************************************************************/
I2cStatus_t I2C1_Transfer(Int8U SlaveAdd, Int8U Add, pInt8U pData, Int32U DataSize);

#endif // __DRV_I2C1_H
