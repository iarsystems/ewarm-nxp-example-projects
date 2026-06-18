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
 *    $Revision: 5880 $
 **************************************************************************/
#include <NXP/iomcimx31.h>
#include "drv_i2c1.h"
#include "drv_aitc.h"
#include "drv_atlas.h"

static volatile I2C_Ctrl_t I2C1_Ctrl;

int I2C1_Write(Int8U * Source,Int32U Slave,Int32U Size)
{
  //Wait Busy
  while(I2SR1_bit.IBB);
  //I2C Transmit mode
  I2CR1_bit.MTX = 1;
  //Strat bit
  I2CR1_bit.MSTA = 1;
  //Slave addr and R/W = 0
  I2DR1 = (Slave<<1);
  //Wait Busy 
  while(!(I2SR1_bit.IBB));  
  I2SR1_bit.IIF = 0;
  //
  while(1)
  {
    while(!(I2SR1_bit.IIF));
    //Acknowledge Not received
    /*if((I2SR1 & (1<<0))) 
    {
      //Stop bit
      I2CR1 &= ~(1UL<<5);
      return (-1);
    }*/
    
    if(!Size) break;
    
    I2SR1_bit.IIF = 0;

    Size--;
    I2DR1 = *Source++;  
  }
  //Stop bit
  I2CR1_bit.MSTA = 0;
  return 0;
}
//
int I2C1_Read(Int8U * Dest,int Slave, unsigned int Size)
{
  //Wait Busy 
  while(I2SR1_bit.IBB);
  //I2C Transmit mode
  I2CR1_bit.MTX = 1;
  //Strat bit
  I2CR1_bit.MSTA = 1;
  //Slave addr and R/W = 1
  I2DR1 = (Slave<<1) | 1 ;
  //Wait Busy 
  while(!(I2SR1_bit.IBB));  
  I2SR1_bit.IIF = 0;
  //
  while(!(I2SR1_bit.IIF));
  //Acknowledge Not received
 /* if((_I2SR1 & (1<<0)))
  {//
    //Stop bit
    _I2CR1 &= ~(1UL<<5);
    return (-1);
  }*/
  //Start Receining
  I2SR1_bit.IIF = 0;
  I2CR1_bit.MTX = 0;
  //Dummy Read
  *Dest = I2DR1;

  while(Size)
  {
    if(1 == Size) I2CR1_bit.TXAK = 1;
    else I2CR1_bit.TXAK = 0;
    //
    while(!(I2SR1_bit.IIF));
    
    if(1 == Size) I2CR1_bit.MSTA = 0; 
    
    I2SR1_bit.IIF = 0;
    *Dest++ = I2DR1;
    Size--;
  }
    
  return 0;
}

void I2C1_Init(void)
{
  // Enable I2C module
  // The pins assign to the I2C1 module
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_OUT_EN0 = 1;  /*I2C_SDA*/
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_IN_EN0 = 2;   /*I2C_SDA*/
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_OUT_EN1 = 1;  /*I2C_SCL*/
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_IN_EN1 = 2;   /*I2C_SCL*/
  // Enable a clock
  CCM_CGR0 |= 3<<26;
  // Disable I2C module (reset module)
  I2CR1_bit.IEN = 0;
  /*Set Frequency*/
  IFDR1_bit.IC = 0x31;  /*~350kHz (66,5MHz/192)*/
  
  I2CR1_bit.IEN = 1;

  I2SR1_bit.IAL = 0;
  
  if(I2SR1_bit.IBB)
  {
    I2CR1_bit.MSTA = 0;
  }
}
