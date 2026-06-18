/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_vs1002.c
 *    Description : VS1002 driver
 *
 *    History :
 *    1. Date        : August 10, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "drv_vs1002.h"

#define VS1002_VER(var)       ((var)?var:VS1002_Pass)
#define VS1002_SHORT(var)     ((var)?VS1002_Fault:VS1002_Pass)

#define VS1002_RET(var) return((VS1002_STATUS_VERBOSE?VS1002_VER(var):VS1002_SHORT(var)))

Boolean PlayFile = 0;     // Play in progress
volatile pInt8U pVS1002_Data; // Stream transfer buffer
volatile Int32S VS1002_Size;  // Stream transfer remaining bytes
Boolean AddInc = 0;       // disable/enable address increment

/*************************************************************************
 * Function Name: VS1002_DReq
 * Parameters: none
 * Return: Boolean
 *
 * Description: Return state of the DREQ line
 *
 *************************************************************************/
static inline
Boolean VS1002_DReq (void)
{
  return(0 != (VS1002_DREQ_FIO & VS1002_DREQ_MASK));
}

/*************************************************************************
 * Function Name: VS1002_ChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: VS1002 chip select control
 * Select = true  - Chip is enable - command
 * Select = false - Chip is disable - data
 *
 *************************************************************************/
static inline
void VS1002_ChipSelect (Boolean Select)
{
  if(Select)
  {
    VS1002_CS_AU_FCLR = VS1002_CS_AU_MASK;
  }
  else
  {
    VS1002_CS_AU_FSET = VS1002_CS_AU_MASK;
  }
}

/*************************************************************************
 * Function Name: VS1002_SetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
static inline
Int32U VS1002_SetClockFreq (Int32U Frequency)
{
Int32U Div;
Int32U SysFreq = SYS_GetFpclk(SSP0_PCLK_OFFSET);
  for(Div = 2; Div < 254; Div+=2)
  {
    if(Frequency * Div > SysFreq)
    {
      break;
    }
  }
  SSP0CPSR_bit.CPSDVSR = Div;
  // Return real frequency
  return(SYS_GetFpclk(SSP0_PCLK_OFFSET)/Div);
}

/*************************************************************************
 * Function Name: VS1002_TranserByte
 * Parameters: Int8U ch
 * Return: Int16U
 *
 * Description: Read 8 bits from SPI
 *
 *************************************************************************/
static
Int16U VS1002_TranserByte (Int8U ch)
{
  while(!SSP0SR_bit.TNF);
  SSP0DR = ch;
  while(!SSP0SR_bit.RNE);
  return((Int8U)SSP0DR);
}

/*************************************************************************
 * Function Name: VS1002_ModuleInit
 * Parameters: none
 * Return: VS1002_Status_t
 *          VS1002_Pass, VS1002_Fault or VS1002_WrongRev, VS1002_NotComm
 *
 * Description: Initialize VS1002 module
 *
 *************************************************************************/
static inline
VS1002_Status_t VS1002_ModuleInit (void)
{
Int32U i;

  // Configure chip select pins
  VS1002_CS_AU_SEL   = 0;
  VS1002_CS_AU_MODE  = 2;                 // disable both pulls
  VS1002_CS_AU_FDIR |= VS1002_CS_AU_MASK; // pin is output
  VS1002_CS_AU_FSET  = VS1002_CS_AU_MASK; // high level

  // Configure data request pin
  VS1002_DREQ_SEL    = 0;
  VS1002_DREQ_MODE   = 2;                 // disable both pulls
  VS1002_DREQ_FDIR  &= ~VS1002_DREQ_MASK; // pin is input

  // Configure SSP0 CLK, MOSI and MISO pins
  VS1002_MISO_SEL    = 3;
  VS1002_MISO_MODE   = 2;
  VS1002_MOSI_SEL    = 3;
  VS1002_MOSI_MODE   = 2;
  VS1002_SCLK_SEL    = 3;
  VS1002_SCLK_MODE   = 2;

  // SSP0 init
  PCLKSEL1_bit.PCLK_SSP0 = 0; // cclk / 4
  PCONP_bit.PCSSP0 = 1;
  SSP0CR1  = 0;
  SSP0CR0  = 0x7;
  SSP0IMSC = 0;
  SSP0DMACR = 0;  // disable DMA

  // Enable SPI
  SSP0CR1_bit.SSE = 1;
  // Clock Freq <= 2MHz
  VS1002_SetClockFreq(VS1002_INIT_CLK_FREQ);

  // clear input fifo
  while(SSP0SR_bit.RNE)
  {
    volatile Int32U dummy = SSP0DR;
  }

  // Deselect chip
  VS1002_ChipSelect(0);

  // Deinitialization VS1002 module (VS1002)
  // Wait 100ms
  DelayResolution100us(1000);
  VS1002_SendCmd(VS1002_CmdReset,&i);

  // Get the chip ID
  VS1002_SendCmd(VS1002_CmdGetRevision,&i);
  if (i != VS1002_VS1002_REV)
  {
    VS1002_RET(VS1002_WrongRev);
  }
  // Init Clk
  i = 0x9800; // 12.288MHz * 2
  VS1002_SendCmd(VS1002_CmdSetClkReg,&i);
  // Clock Freq <= 4MHz
  VS1002_SetClockFreq(VS1002_CLK_FREQ);

  // Init Mode
  // VS1002 native SPI modes, Share SPI chip select, Streaming mode
  i = 0x0C40|
     (VS1002_PLUS_V_ENA?1UL<<7:0); // +V mode
  VS1002_SendCmd(VS1002_CmdSetModeReg,&i);

  PlayFile = 0;

  VS1002_RET(VS1002_Pass);
}

/*************************************************************************
 * Function Name: VS1002_Transmit
 * Parameters: pInt8U pData , Int32U Size,
 *             Boolean StreamMode, Boolean SrcAddInc
 * Return: none
 *
 * Description: Transmit data
 *
 *************************************************************************/
static
void VS1002_Transmit(pInt8U pData , Int32U Size, Boolean StreamMode, Boolean SrcAddInc)
{
Int32S iSize = Size;
  VS1002_ChipSelect(0);

  while(iSize > 0)
  {
    if(StreamMode)
      while(!VS1002_DReq());
    for(int i = 8; i; --i)
    {
      VS1002_TranserByte(*pData);
      if(!(--iSize))
      {
        return;
      }
      if(SrcAddInc)
      {
        ++pData;
      }
    }
  }
}

/*************************************************************************
 * Function Name: VS1002_SendCmd
 * Parameters: VS1002_Cmd_t Cmd, pInt32U pData
 * Return: none
 *
 * Description: Send command to the VS1002 module
 *
 *************************************************************************/
void VS1002_SendCmd (VS1002_Cmd_t Cmd, pInt32U pData)
{
Int32U Tmp;
pVS1002_Stream_t pVS1002_Stream;

  switch (Cmd)
  {
  case VS1002_CmdPowerUp:
    *pData = VS1002_ModuleInit();
    break;
  case VS1002_CmdPowerDown:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_ReadCmd);
    VS1002_TranserByte(VS1002_STATUS);
    Tmp = (VS1002_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  VS1002_TranserByte(0)       & 0x00FF;
    VS1002_ChipSelect(0);
    // Set SS APDOWN2 bit (3)
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_STATUS);
    VS1002_TranserByte(Tmp >> 8);
    VS1002_TranserByte(Tmp | (1UL<<3));
    VS1002_ChipSelect(0);
    // Wait 10ms
    DelayResolution100us(100);
    break;
  case VS1002_CmdGetRevision:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_ReadCmd);
    VS1002_TranserByte(VS1002_STATUS);
    VS1002_TranserByte(0);
    Tmp = VS1002_TranserByte(0);
    *pData = (Tmp>>4) & 0x7;
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdSetClkReg:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_CLOCKF);
    VS1002_TranserByte(*pData>>8);
    VS1002_TranserByte(*pData   );
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdSetModeReg:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_MODE);
    VS1002_TranserByte(*pData>>8);
    VS1002_TranserByte(*pData   );
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdSetVol:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_VOL);
    VS1002_TranserByte(*pData>>8);
    VS1002_TranserByte(*pData   );
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdTstSin:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_ReadCmd);
    VS1002_TranserByte(VS1002_MODE);
    Tmp = (VS1002_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  VS1002_TranserByte(0)       & 0x00FF;
    VS1002_ChipSelect(0);
    // Set test bit
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_MODE);
    VS1002_TranserByte(Tmp >> 8);
    VS1002_TranserByte(Tmp | (1UL << 5));
    VS1002_ChipSelect(0);
    if(*pData)
    {
      // Send test command sequence
      VS1002_TranserByte(0x53);
      VS1002_TranserByte(0xEF);
      VS1002_TranserByte(0x6E);
      VS1002_TranserByte(*pData);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
    }
    else
    {
      // Send exit form the test command sequence
      VS1002_TranserByte(0x45);
      VS1002_TranserByte(0x78);
      VS1002_TranserByte(0x69);
      VS1002_TranserByte(0x74);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
      VS1002_TranserByte(0x00);
    }
    DelayResolution100us(1000);
    // Clear test bit
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_MODE);
    VS1002_TranserByte(Tmp>>8);
    VS1002_TranserByte(Tmp   );
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdPlay:
    // Wait until data transfer is active
    pVS1002_Stream = (pVS1002_Stream_t)pData;
    // for synchronization
    VS1002_ChipSelect(1);
    PlayFile = 1;
    VS1002_Transmit(pVS1002_Stream->pStream,pVS1002_Stream->Size,TRUE,TRUE);
    break;
  case VS1002_CmdPlayStop:
    VS1002_ChipSelect(1);
    // Init Transfer of 2048 zero (clear on-chip buffer)
    Tmp = 0;
    VS1002_Transmit((pInt8U)&Tmp,2048,FALSE,FALSE);
    PlayFile = 0;
    VS1002_SendCmd(VS1002_CmdReset,&Tmp);
    break;
  case VS1002_CmdWideStereo:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_ReadCmd);
    VS1002_TranserByte(VS1002_MODE);
    Tmp = (VS1002_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  VS1002_TranserByte(0)       & 0x00FF;
    VS1002_ChipSelect(0);
    if(*pData)
    {
      Tmp |= 1;
    }
    else
    {
      Tmp &= ~1;
    }
    // Set test bit
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_MODE);
    VS1002_TranserByte(Tmp >> 8);
    VS1002_TranserByte(Tmp | (1UL << 5));
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdLoudness:
    Tmp = *pData?0x89:0;
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_BASS);
    VS1002_TranserByte(Tmp>>8);
    VS1002_TranserByte(Tmp   );
    VS1002_ChipSelect(0);
    break;
  case VS1002_CmdReset:
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_ReadCmd);
    VS1002_TranserByte(VS1002_MODE);
    Tmp = (VS1002_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  VS1002_TranserByte(0)       & 0x00FF;
    VS1002_ChipSelect(0);
    Tmp |= (1UL << 2);
    // Set test bit
    VS1002_ChipSelect(1);
    VS1002_TranserByte(VS1002_WriteCmd);
    VS1002_TranserByte(VS1002_MODE);
    VS1002_TranserByte(Tmp >> 8);
    VS1002_TranserByte(Tmp);
    VS1002_ChipSelect(0);
    // wait > 2us
    for(volatile Int32U i = 20; i; i--);
    // wait until reset out
    while(FALSE == VS1002_DReq());
    break;
  }
}
