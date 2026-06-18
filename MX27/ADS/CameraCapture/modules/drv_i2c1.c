/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_i2c1.c
 *    Description : iMX27 I2C driver
 *
 *    History :
 *    1. Date        : 11 May 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_i2c1.h"

static I2C_Ctrl_t I2C1_Ctrl;

/*************************************************************************
 * Function Name: I2C1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C interrupts handler
 *
 *************************************************************************/
void I2C1_Handler (void)
{
volatile Int8U Dummy;
  // Clear interrupt flag
  I2SR1_bit.IIF = 0;
  // Arbitration lost check
  if(!I2SR1_bit.IAL)
  {
    switch (I2C1_Ctrl.State)
    {
    case I2cSlaveAdd:
      if(I2SR1_bit.RXAK)
      {
        I2C1_Ctrl.Status = I2cNak;
        break;
      }
      I2DR1_bit.DATA = I2C1_Ctrl.Add;
      if(I2C1_Ctrl.SlaveAdd & 1)
      {
        // Receive
        I2C1_Ctrl.State = I2cRepeatedStart;
      }
      else
      {
        // Transmit
        ++I2C1_Ctrl.State;
      }
      break;
    case I2cSend:
      if(I2C1_Ctrl.DataSize--)
      {
        I2DR1_bit.DATA = *I2C1_Ctrl.pData++;
      }
      else
      {
        // Transmit complete
        I2C1_Ctrl.Status = I2cComplete;
      }
      break;
    case I2cRepeatedStart:
      I2CR1_bit.RSTA = 1;
      I2DR1_bit.DATA = I2C1_Ctrl.SlaveAdd;
      ++I2C1_Ctrl.State;
      break;
    case I2cRepeatedSlaveAdd:
      if(I2SR1_bit.RXAK)
      {
        I2C1_Ctrl.Status = I2cNak;
        break;
      }
      if(I2C1_Ctrl.DataSize == 1)
      {
        // Receive without akn
        I2CR1_bit.TXAK = 1;
      }
      else
      {
        // Receive with akn
        I2CR1_bit.TXAK = 0;
      }
      // Receive mode
      I2CR1_bit.MTX = 0;
      // Dummy read
      Dummy = I2DR1_bit.DATA;
      ++I2C1_Ctrl.State;
      break;
    case I2cReceive:
      if(--I2C1_Ctrl.DataSize == 0)
      {
        // Receive complete
        I2C1_Ctrl.Status = I2cComplete;
        // The stop condition must be generate before
        // reading of the last received byte
        I2CR1_bit.MSTA  = 0;
      }
      else if (I2C1_Ctrl.DataSize == 1)
      {
        // Receive without akn
        I2CR1_bit.TXAK = 1;
      }
      *I2C1_Ctrl.pData++ = I2DR1_bit.DATA;
    }
  }
  else
  {
    // Arbitration is lost
    I2SR1_bit.IAL = 0;
    if(I2C1_Ctrl.State)
    {
      I2C1_Ctrl.Status = I2cArbLost;
    }
  }
  if(I2C1_Ctrl.Status != I2cBusy)
  {
    // Stop condition
    I2CR1_bit.MSTA  = 0;
    I2C1_Ctrl.State = I2cIdle;
  }
}

/*************************************************************************
 * Function Name: I2C1_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C module
 *
 *************************************************************************/
void I2C1_Init (void)
{
  // Init variables
  I2C1_Ctrl.State  = I2cIdle;
  // Enable a clock
  PCCR0_bit.I2C1_EN = 1;
  // Disable I2C module (reset module)
  I2CR1_bit.IEN = 0;
  // Enable I2C module
  I2CR1_bit.IEN = 1;

  if(I2SR1_bit.IBB)
  {
    I2CR1_bit.MSTA = 0;
  }
  // The pins assign to the I2C1 module
  PTD_GIUS_bit.PIN17 = PTD_GIUS_bit.PIN18 = 0;
  // I2C speed init ~100kHz/IPCLK 66.5MHz
  IFDR1_bit.IC = 0x38;
  I2SR1_bit.IIF=0;
  I2CR1_bit.IIEN = 1;
  // Init AITC
  AITC_SetupIntr(I2C1_Handler,FALSE,INT_I2C1,0);
  AITC_EnableIntSource(INT_I2C1);
}

/*************************************************************************
 * Function Name: I2C1_Transfer
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             pInt8U pData, Int32U DataSize
 *
 * Return: Boolean
 *          TRUE - OK
 *          FALSE - Fault
 *
 * Description: Read/Write data from/to the I2C1
 *
 *************************************************************************/
Boolean I2C1_Transfer(Int8U SlaveAdd, Int8U Add, pInt8U pData, Int32U DataSize)
{
  // Parameters check
  if((DataSize == 0) && (pData == NULL))
  {
    return I2cParametersError;
  }
  // I2C Transfer init
  I2C1_Ctrl.DataSize = DataSize;
  I2C1_Ctrl.Status   = I2cBusy;
  I2C1_Ctrl.State    = I2cSlaveAdd;
  I2C1_Ctrl.SlaveAdd = SlaveAdd;
  I2C1_Ctrl.pData    = pData;
  I2C1_Ctrl.Add      = Add;
  // Wait bus Idle state
  while(I2SR1_bit.IBB);
  // Transmit mode
  I2CR1_bit.MTX  = 1;
  // Send Start condition
  I2CR1_bit.MSTA = 1;
  // Wait until Busy set
  while(!I2SR1_bit.IBB);
  // Load Slave address and direction
  I2SR1_bit.IIF=0;
  I2DR1_bit.DATA = I2C1_Ctrl.SlaveAdd & 0xFE;
  I2CR1_bit.IIEN = 1;
  // Wait until complete the data transfer
  while(I2C1_Ctrl.Status == I2cBusy);
  if(I2C1_Ctrl.Status == I2cArbLost)
  {
    I2C1_Init();
  }
  I2CR1_bit.IIEN = 0;
  // I2C status
  return I2C1_Ctrl.Status;
}
