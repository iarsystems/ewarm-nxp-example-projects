
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
#include "sd_ll_spi1.h"
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
    IO0CLR_bit.P0_20 = 1;
  }
  else
  {
    IO0SET_bit.P0_20 = 1;
    // Synchronization
    while(!SSPSR_bit.TNF);
    SSPDR = 0xFF;
    // Wait until tx fifo and tx shift bufer are empty
    while(SSPSR_bit.BSY);
    while(!SSPSR_bit.RNE);
    do
    {
      Select = SSPDR;
    }
    while(SSPSR_bit.RNE);
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
  return(TRUE);
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
  return(FALSE);
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
Int32U SysFreq = SYS_GetFpclk();
  for(Div = 2; Div < 254; Div+=2)
  {
    if(Frequency * Div > SysFreq)
    {
      break;
    }
  }
  SSPCPSR_bit.CPSDVSR = Div;
  // Return real frequency
  return(SYS_GetFpclk()/Div);
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

  // Chip select
  IO0SET_bit.P0_20  = 1;
  IO0DIR_bit.P0_20  = 1;
  // Cart present
  // Spi init
  PM_OpenPeripheral(PC_PCSPI1);
  SSPCR0  = 0x7;
  SSPCR1  = 0;
  SSPIMSC = 0;
  // Clock Freq. Identification Mode < 400kHz
  SdSetClockFreq(IdentificationModeClock);
  PINSEL1_bit.P0_17 = PINSEL1_bit.P0_18 = PINSEL1_bit.P0_19 = 2;
  PINSEL1_bit.P0_20 = 0;
  // Enable SPI
  SSPCR1_bit.SSE = 1;
  for ( i = 0; i < 8; i++ )
  {
    Dummy = SSPDR; /* clear the RxFIFO */
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
  while(!SSPSR_bit.TNF);
  SSPDR = ch;
  while( SSPSR_bit.BSY);
  while(!SSPSR_bit.RNE);
  return((Int8U)SSPDR);
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
    while(SSPSR_bit.TNF && OutCount)
    {
      SSPDR = *pData++;
      --OutCount;
    }
  }
  while( SSPSR_bit.BSY);
  while (SSPSR_bit.RNE || !SSPSR_bit.TFE)
  {
    volatile Int32U Dummy = SSPDR;
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
    while(SSPSR_bit.TNF && (Delta < _SSP_FIFO_SIZE) && Size)
    {
      SSPDR = 0xFF;
      --Size; ++Delta;
    }
    while (SSPSR_bit.RNE)
    {
      *pData++ = SSPDR;
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
