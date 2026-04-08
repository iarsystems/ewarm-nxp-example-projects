/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_i2c.c
 *    Description : The iMX21 I2C driver
 *
 *    History :
 *    1. Date        : 14 May 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_i2c.h"

static I2C_Ctrl_t I2C_Ctrl;

/*************************************************************************
 * Function Name: I2C_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C interrupts handler
 *
 *************************************************************************/
void I2C_Handler (void)
{
volatile Int8U Dummy;
  // Clear interrupt flag
  I2SR_bit.IIF = 0;
  // Arbitration lost check
  if(!I2SR_bit.IAL)
  {
    switch (I2C_Ctrl.State)
    {
    case I2cSlaveAdd:
      if(I2SR_bit.RXAK)
      {
        I2C_Ctrl.Status = I2cNak;
        break;
      }
      I2DR_bit.DATA = I2C_Ctrl.Add;
      if(I2C_Ctrl.SlaveAdd & 1)
      {
        // Receive
        I2C_Ctrl.State = I2cRepeatedStart;
      }
      else
      {
        // Transmit
        ++I2C_Ctrl.State;
      }
      break;
    case I2cSend:
      if(I2C_Ctrl.DataSize--)
      {
        I2DR_bit.DATA = *I2C_Ctrl.pData++;
      }
      else
      {
        // Transmit complete
        I2C_Ctrl.Status = I2cComplete;
      }
      break;
    case I2cRepeatedStart:
      I2CR_bit.RSTA = 1;
      I2DR_bit.DATA = I2C_Ctrl.SlaveAdd;
      ++I2C_Ctrl.State;
      break;
    case I2cRepeatedSlaveAdd:
      if(I2SR_bit.RXAK)
      {
        I2C_Ctrl.Status = I2cNak;
        break;
      }
      if(I2C_Ctrl.DataSize == 1)
      {
        // Receive without akn
        I2CR_bit.TXAK = 1;
      }
      else
      {
        // Receive with akn
        I2CR_bit.TXAK = 0;
      }
      // Receive mode
      I2CR_bit.MTX = 0;
      // Dummy read
      Dummy = I2DR_bit.DATA;
      ++I2C_Ctrl.State;
      break;
    case I2cReceive:
      if(--I2C_Ctrl.DataSize == 0)
      {
        // Receive complete
        I2C_Ctrl.Status = I2cComplete;
        // The stop condition must be generate before
        // reading of the last received byte
        I2CR_bit.MSTA  = 0;
      }
      else if (I2C_Ctrl.DataSize == 1)
      {
        // Receive without akn
        I2CR_bit.TXAK = 1;
      }
      *I2C_Ctrl.pData++ = I2DR_bit.DATA;
    }
  }
  else
  {
    // Arbitration is lost
    I2SR_bit.IAL = 0;
    if(I2C_Ctrl.State)
    {
      I2C_Ctrl.Status = I2cArbLost;
    }
  }
  if(I2C_Ctrl.Status != I2cBusy)
  {
    // Stop condition
    I2CR_bit.MSTA  = 0;
    I2C_Ctrl.State = I2cIdle;
  }
}

/*************************************************************************
 * Function Name: I2C_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C module
 *
 *************************************************************************/
void I2C_Init (void)
{
  // Init variables
  I2C_Ctrl.State  = I2cIdle;
  // Enable a clock
  PCCR0_bit.I2C_EN = 1;
  // Disable I2C module (reset module)
  I2CR_bit.IEN = 0;
  // Enable I2C module
  I2CR_bit.IEN = 1;

  if(I2SR_bit.IBB)
  {
    I2CR_bit.MSTA = 0;
  }
  // The pins assign to the I2C module
  PTD_GIUS_bit.PIN17 = PTD_GIUS_bit.PIN18 = 0;
  // I2C speed init ~100kHz/IPCLK 66.5MHz
  IFDR_bit.IC = 0x38;
  I2SR_bit.IIF=0;
  I2CR_bit.IIEN = 1;
  // Init AITC
  AITC_SetupIntr(I2C_Handler,FALSE,INT_I2C,0);
  AITC_EnableIntSource(INT_I2C);
}

/*************************************************************************
 * Function Name: I2C_Transfer
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             pInt8U pData, Int32U DataSize
 *
 * Return: Boolean
 *          TRUE - OK
 *          FALSE - Fault
 *
 * Description: Init I2C module
 *
 *************************************************************************/
Boolean I2C_Transfer(Int8U SlaveAdd, Int8U Add, pInt8U pData, Int32U DataSize)
{
  // Parameters check
  if((DataSize == 0) && (pData == NULL))
  {
    return I2cParametersError;
  }
  // I2C Transfer init
  I2C_Ctrl.DataSize = DataSize;
  I2C_Ctrl.Status   = I2cBusy;
  I2C_Ctrl.State    = I2cSlaveAdd;
  I2C_Ctrl.SlaveAdd = SlaveAdd;
  I2C_Ctrl.pData    = pData;
  I2C_Ctrl.Add      = Add;
  // Wait bus Idle state
  while(I2SR_bit.IBB);
  // Transmit mode
  I2CR_bit.MTX  = 1;
  // Send Start condition
  I2CR_bit.MSTA = 1;
  // Wait until Busy set
  while(!I2SR_bit.IBB);
  // Load Slave address and direction
  I2SR_bit.IIF=0;
  I2DR_bit.DATA = I2C_Ctrl.SlaveAdd & 0xFE;
  I2CR_bit.IIEN = 1;
  // Wait until complete the data transfer
  while(I2C_Ctrl.Status == I2cBusy);
  if(I2C_Ctrl.Status == I2cArbLost)
  {
    I2C_Init();
  }
  I2CR_bit.IIEN = 0;
  // I2C status
  return I2C_Ctrl.Status;
}
