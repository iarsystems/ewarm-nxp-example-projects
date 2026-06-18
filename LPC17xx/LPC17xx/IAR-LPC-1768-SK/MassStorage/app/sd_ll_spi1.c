
/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sd_ll_spi1.h
 *    Description : SD/MMC low level SPI1 driver
 *
 *    History :
 *    1. Date        : April 10, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#define _SSP_FIFO_SIZE 8

/*************************************************************************
 * Function Name: SdPowerOn
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOn (void)
{
  /*MMC/SD power on*/
  MMC_PWR_FCLR = MMC_PWR_MASK;
  SdDly_1ms(1);
}
/*************************************************************************
 * Function Name: SdPowerOff
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOff (void)
{
  /*MMC/SD power off*/
  MMC_PWR_FSET = MMC_PWR_MASK;
  SdDly_1ms(1);
}
/*************************************************************************
 * Function Name: SdChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: SD/MMC Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void SdChipSelect (Boolean Select)
{
  if (Select)
  {
    MMC_CS_FCLR = MMC_CS_MASK;
  }
  else
  {
    MMC_CS_FSET = MMC_CS_MASK;
    // Synchronization
    while(!SSP1SR_bit.TNF);
    SSP1DR = 0xFF;
    // Wait until tx fifo and tx shift bufer are empty
    while(SSP1SR_bit.BSY);
    while(!SSP1SR_bit.RNE);
    do
    {
      Select = SSP1DR;
    }
    while(SSP1SR_bit.RNE);
  }
}
/*************************************************************************
 * Function Name: SdPresent
 * Parameters: none
 * Return: Boolean - true cart present
 *                 - false cart no present
 *
 * Description: SD/MMC precent check
 *
 *************************************************************************/
Boolean SdPresent (void)
{
  return((MMC_CP_FIO & MMC_CP_MASK) == 0);
}

/*************************************************************************
 * Function Name: SdWriteProtect
 * Parameters: none
 * Return: Boolean - true cart is protected
 *                 - false cart no protected
 *
 * Description: SD/MMC Write protect check
 *
 *************************************************************************/
Boolean SdWriteProtect (void)
{
  return((MMC_WP_FIO & MMC_WP_MASK) != 0);
}

/*************************************************************************
 * Function Name: SdSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
Int32U SdSetClockFreq (Int32U Frequency)
{
Int32U Div;
Int32U SysFreq = SYS_GetFpclk(SSP1_PCLK_OFFSET);
  for(Div = 2; Div < 254; Div+=2)
  {
    if(Frequency * Div > SysFreq)
    {
      break;
    }
  }
  SSP1CPSR_bit.CPSDVSR = Div;
  // Return real frequency
  return(SysFreq/Div);
}

/*************************************************************************
 * Function Name: SdInit
 * Parameters: none
 * Return: none
 *
 * Description: Init SPI, Cart Present, Write Protect and Chip select pins
 *
 *************************************************************************/
void SdInit (void)
{
Int32U i;
volatile Int32U Dummy;

  /* Chip select */
  MMC_CS_FSET  = MMC_CS_MASK;
  MMC_CS_FDIR |= MMC_CS_MASK;
  MMC_CS_SEL = 0;
  /* Cart present */
  MMC_CP_FDIR &= ~MMC_CP_MASK;
  MMC_CP_SEL = 0;
  MMC_CP_MODE = 2;  // pin has neither pull-up nor pull-down
 /* Write protect */
  MMC_WP_FDIR &= ~MMC_WP_MASK;
  MMC_WP_SEL = 0;
  MMC_WP_MODE = 2;  // pin has neither pull-up nor pull-down
  /* MMC/SD PWR */
  MMC_PWR_FSET  = MMC_PWR_MASK;
  MMC_PWR_FDIR |= MMC_PWR_MASK;
  MMC_PWR_SEL = 0;

  // Spi init
  PCONP_bit.PCSSP1 = 1;			// SSP1 clock enable
  SSP1CR0  = 0x7;
  SSP1CR1  = 0;
  SSP1IMSC = 0;
  // Clock Freq. Identification Mode < 400kHz
  SdSetClockFreq(IdentificationModeClock);
  PINSEL0_bit.P0_7 = PINSEL0_bit.P0_8 = PINSEL0_bit.P0_9 = 2;
  // Enable SPI
  SSP1CR1_bit.SSE = 1;
  for ( i = 0; i < 8; i++ )
  {
    Dummy = SSP1DR; /* clear the RxFIFO */
  }
}

/*************************************************************************
 * Function Name: SdTranserByte
 * Parameters: Int8U ch
 * Return: Int8U
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
Int8U SdTranserByte (Int8U ch)
{
  while(!SSP1SR_bit.TNF);
  SSP1DR = ch;
  while( SSP1SR_bit.BSY);
  while(!SSP1SR_bit.RNE);
  return((Int8U)SSP1DR);
}

/*************************************************************************
 * Function Name: SdSendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
void SdSendBlock (pInt8U pData, Int32U Size)
{
Int32U OutCount = Size;
  while (OutCount)
  {
    while(SSP1SR_bit.TNF && OutCount)
    {
      SSP1DR = *pData++;
      --OutCount;
    }
  }
  // add some short delay
  while (SSP1SR_bit.BSY);
  while (SSP1SR_bit.RNE || !SSP1SR_bit.TFE)
  {
    volatile Int32U Dummy = SSP1DR;
  }
}

/*************************************************************************
 * Function Name: SdReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
void SdReceiveBlock (pInt8U pData, Int32U Size)
{
Int32U Delta = 0;
  while (Size || Delta)
  {
    while(SSP1SR_bit.TNF && (Delta < _SSP_FIFO_SIZE) && Size)
    {
      SSP1DR = 0xFF;
      --Size; ++Delta;
    }
    while (SSP1SR_bit.RNE)
    {
      *pData++ = SSP1DR;
      --Delta;
    }
  }
}

/*************************************************************************
 * Function Name: SdDly_1ms
 * Parameters: Int32U Delay
 * Return: none
 *
 * Description: Delay [msec]
 *
 *************************************************************************/
void SdDly_1ms (Int32U Delay)
{
volatile Int32U i;
  for(;Delay;--Delay)
  {
    for(i = SD_DLY_1MSEC;i;--i);
  }
}
