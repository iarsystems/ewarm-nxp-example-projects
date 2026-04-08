/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_CS43L22.c
*    Description : i.MX25 CS43L22 driver (beep only)
 *
 *    History :
 *    1. Date        : 14 January 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_CS43L22.h"
#include "drv_i2c1.h"

/*************************************************************************
 * Function Name: CS43L22_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: CS43L22 initialization
 *
 *************************************************************************/
Boolean CS43L22_Init (void)
{
Boolean status;
Int8U data;

  // Pin assigment
  // Reset
  IOMUXC_SW_MUX_CTL_PAD_VSTBY_ACK_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_VSTBY_ACK_bit.SION     = 1;
  GPIO3_DR_bit.DR18          = 0;
  GPIO3_GDIR_bit.GDIR18      = 1;
  // MCLK
  IOMUXC_SW_MUX_CTL_PAD_CLKO = ALT0_MUX_MODE;
  // AUMUX4 TXD
  IOMUXC_SW_MUX_CTL_PAD_EB0  = ALT4_MUX_MODE;
  // AUMUX4 TXC
  IOMUXC_SW_MUX_CTL_PAD_OE   = ALT4_MUX_MODE;
  // AUMUX4 TXFS
  IOMUXC_SW_MUX_CTL_PAD_RW   = ALT4_MUX_MODE;

  MCR_bit.CLKO_EN  = 0;

  // Some delay
  for(volatile Int32U i = 5000; i; i--);
  // Release reset
  GPIO3_DR_bit.DR18 = 1;
  // Some delay
  for(volatile Int32U i = 200; i; i--);

  // The default state of the “Power Ctl. 1” register (0x02) is 0x01.
  // Load the desired register settings while keeping the "Power Ctl 1" register set to 0x01.
  data = 0x01;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PWR_CTRL1_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR | I2C_READ_DATA,CS43L22_ID_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  if(0xE0 != (0xF8 & data))
  {
    return(FALSE);
  }
  // Load the required initialization settings listed
  data = 0x99;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,0,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  data = 0x80;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,0x47,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR | I2C_READ_DATA,0x32,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  data |= 1U<<7;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,0x32,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  data &=~(1U<<7);
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,0x32,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  data = 0x00;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,0,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x0A;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PWR_CTRL2_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  // Apply MCLK at the appropriate frequency
  data = 0x23;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_CLK_CTRL_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  // Enable MCLK 24MHz
  MCR_bit.CLKO_SEL = 1;
  MCR_bit.CLKO_DIV = 0;
  MCR_bit.CLKO_EN  = 1;

  // AUMUX4 init
  PTCR4_bit.TCSEL   = 4-1;
  PTCR4_bit.TCLKDIR = 0;
  PTCR4_bit.TFSEL   = 4-1;
  PTCR4_bit.TFSDIR  = 0;
  PDCR4 = 0;

  // Set the “Power Ctl 1” register (0x02) to 0x9E
  data = 0x9E;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PWR_CTRL1_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x00;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_INTF_CTRL2_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x00;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PLAYBACK_CTRL2_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x0;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_MISC_CTRL_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x80;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PCMA_VOL_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x80;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_PCMB_VOL_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = 0x80;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_INTF_CTRL1_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }


  return(TRUE);
}

/*************************************************************************
 * Function Name: CS43L22_Vol
 * Parameters: Int32U Vol
 *
 * Return: Boolean
 *
 * Description: Set Beep volume
 *
 *************************************************************************/
Boolean CS43L22_Vol (Int32U Vol)
{
Boolean status;
Int8U data;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR | I2C_READ_DATA,
                         CS43L22_BEEP_VOL_OFF_REG,
                         &data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data &= ~0x3F;
  data |= Vol & 0x3F;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_BEEP_VOL_OFF_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: CS43L22_BeepCfg
 * Parameters: Int32U Freq, Int32U On, Int32U off
 *
 * Return: Boolean
 *
 * Description: Configure Beep
 *
 *************************************************************************/
Boolean CS43L22_BeepCfg (Int32U Freq, Int32U On, Int32U Off)
{
Boolean status;
Int8U data;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR | I2C_READ_DATA,
                         CS43L22_BEEP_VOL_OFF_REG,
                         &data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data &= ~0xE0;
  data |= (Off<<5) & 0xE0;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_BEEP_VOL_OFF_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data = On & 0xF;
  data |= (Freq<<4) & 0xF0;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_BEEP_FREQ_ON_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: CS43L22_Beep
 * Parameters: BeepType_t Type
 *
 * Return: Boolean
 *
 * Description: Generate beep
 *
 *************************************************************************/
Boolean CS43L22_Beep (BeepType_t Type)
{
Boolean status;
Int8U data;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR | I2C_READ_DATA,
                         CS43L22_BEEP_TONE_REG,
                         &data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data &= ~0xC0;
  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_BEEP_TONE_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }

  data |= (Type<<6) & 0xC0;
  data |= 1<<5;

  status = I2C1_Transfer(CS43L22_SLAVE_ADDR,CS43L22_BEEP_TONE_REG,&data,1);
  if (I2cComplete != status)
  {
    return(FALSE);
  }
  return(TRUE);
}
