/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sd_card_mode.c
 *    Description : SD/MMC diver
 *
 *    History :
 *    1. Date        : June 3, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    1. Date        : October 30, 2008
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *       Adatp fot Micro-Blox LPC2478 board
 *
 *    $Revision: 4280 $
 **************************************************************************/
#define SD_CARD_MODE_GLOBAL
#include "sd_card_mode.h"

#define _CSD_GET_TRAN_SPEED_EXP()      (_MmcSdCsd[ 0]&0x07)
#define _CSD_GET_TRAN_SPEED_MANT()    ((_MmcSdCsd[ 0]&0xF8)>>3 )
#define _CSD_GET_NSAC()                (_MmcSdCsd[ 1]          )
#define _CSD_GET_TAAC_EXP()            (_MmcSdCsd[ 2]&0x7)
#define _CSD_GET_TAAC_MANT()          ((_MmcSdCsd[ 2]&0xF8)>>3 )
#define _CSD_GET_R2W_FACTOR()         ((_MmcSdCsd[15]&0x1C)>>2 )
#define _CSD_GET_READ_BL_LEN()         (_MmcSdCsd[ 6]&0x0F)
#define _CSD_GET_C_SIZE()            (((_MmcSdCsd[ 5]&0x03)<<10) + (_MmcSdCsd[4]<<2) + ((_MmcSdCsd[11]&0xc0)>>6))
#define _CSD_GET_C_SIZE_MULT()       (((_MmcSdCsd[10]&0x03)<<1 ) +((_MmcSdCsd[9]&0x80)>>7))
#define _CSD_GET_PERM_WRITE_PROTECT() ((_MmcSdCsd[13]&0x20)>>5 )
#define _CSD_GET_TMP_WRITE_PROTECT()  ((_MmcSdCsd[13]&0x10)>>4 )

#define _CSD_2_0_GET_C_SIZE()        (((_MmcSdCsd[4]&0x0F)<<16) + (_MmcSdCsd[11]<<8) + _MmcSdCsd[10])

#define _MCI_BUS_DLY() {\
                        for(volatile Int32U i = 10; i;--i);\
                      }\

const Int32U _SdTransfExp[] =
{
     10000UL,
    100000UL,
   1000000UL,
  10000000UL,
         0UL,
         0UL,
         0UL,
         0UL,
};

const Int32U _SdAccessTime [] =
{
        1UL,
       10UL,
      100UL,
     1000UL,
    10000UL,
   100000UL,
  1000000UL,
 10000000UL,
};

const Int32U _SdCsdMant[] =
{
  0UL,10UL,12UL,13UL,15UL,
  20UL,25UL,
  30UL,35UL,
  40UL,45UL,
  50UL,55UL,
  60UL,
  70UL,
  80UL,
};

const SdCommads_t _SdCmd[CMD_END] =
{
  // CMD0
  {0x00,SdNoArg     ,SdNoResp },
  // CMD1
  {0x01,SdOcr       ,SdR3 },
  // CMD2
  {0x02,SdNoArg     ,SdR2 },
  // CMD3
  {0x03,SdRelAddr   ,SdR1 },
  // CMD7
  {0x07,SdRelAddr   ,SdR1 },
  // CMD8
  {0x08,SdDataAdd   ,SdR7 },
  // CMD9
  {0x09,SdRelAddr   ,SdR2 },
  // CMD10
  {0x0A,SdRelAddr   ,SdR2 },
  // CMD12
  {0x0C,SdNoArg     ,SdR1b},
  // CMD13
  {0x0D,SdRelAddr   ,SdR1 },
  // CMD16
  {0x10,SdBlockLen  ,SdR1 },
  // CMD17
  {0x11,SdDataAdd   ,SdR1 },
  // CMD18
  {0x12,SdDataAdd   ,SdR1 },
  // CMD24
  {0x18,SdDataAdd   ,SdR1 },
  // CMD25
  {0x19,SdDataAdd   ,SdR1 },
  // CMD27
  {0x1B,SdOcr       ,SdR1 },
  // CMD28
  {0x1C,SdDataAdd   ,SdR1b},
  // CMD29
  {0x1D,SdDataAdd   ,SdR1b},
  // CMD30
  {0x1E,SdDataAdd   ,SdR1 },
  // CMD32
  {0x20,SdDataAdd   ,SdR1 },
  // CMD33
  {0x21,SdDataAdd   ,SdR1 },
  // CMD34
  {0x22,SdDataAdd   ,SdR1 },
  // CMD35
  {0x23,SdDataAdd   ,SdR1 },
  // CMD36
  {0x24,SdDataAdd   ,SdR1 },
  // CMD37
  {0x25,SdDataAdd   ,SdR1 },
  // CMD38
  {0x26,SdNoArg     ,SdR1b},
  // CMD42
  {0x2A,SdNoArg     ,SdR1b},
  // CMD55
  {0x37,SdRelAddr   ,SdR1 },
  // CMD56
  {0x38,SdNoArg     ,SdR1 },
  // ACMD46
  {0x06,SdDataAdd   ,SdR1 },
  // ACMD41
  {0x29,SdDataAdd   ,SdR3 },
};

DiskCtrlBlk_t _SdDskCtrlBlk;
Int32U _SdLastError,_Tnac,_Twr;
Boolean _bSdChanged;
#pragma data_alignment = 4
Int8U _MmcSdCsd[16];

static Int32U _CardRCA;
static Boolean _bSdPermWriteProtect;
static Boolean _bHC;

/*************************************************************************
 * Function Name: _SdPresent
 * Parameters: none
 * Return: Boolean - true cart present
 *                 - false cart no present
 *
 * Description: SD/MMC present check
 *
 *************************************************************************/
static
Boolean _SdPresent (void)
{
#if defined(MICRO_BLOX_LPC2478)
  return(TRUE);
#else
  return(!(MMC_CP_FIO & MMC_CP_MASK));
#endif
}

/*************************************************************************
 * Function Name: _SdWriteProtect
 * Parameters: none
 * Return: Boolean - true cart is protected
 *                 - false cart no protected
 *
 * Description: SD/MMC write protect status
 *
 *************************************************************************/
static
Boolean _SdWriteProtect (void)
{
#if defined(IAR_LPC_2478_STK)
  return(0 != (MMC_WP_FIO & MMC_WP_MASK));
#else
  return(FALSE);
#endif
}

/*************************************************************************
 * Function Name: _SdSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SD/MMC ckl frequency
 *
 *************************************************************************/
static
Int32U _SdSetClockFreq (Int32U Frequency)
{
Int32U Pclk = SYS_GetFpclk(MCI_PCLK_OFFSET);
Int32U Div;
  Frequency <<= 1;

  for(Div = 4; Div < 256; ++Div)
  {
    if((Frequency * Div) > Pclk)
    {
      break;
    }
  }
  // Set divider, clk enable and power save
  MCICLOCK_bit.CLKDIV = Div - 1;
  _MCI_BUS_DLY();
  MCICLOCK_bit.ENABLE = 1;
  _MCI_BUS_DLY();
  MCICLOCK_bit.PWRSAVE = 1;
  _MCI_BUS_DLY();
  // Return real frequency
  return(Pclk/(Div<<1));
}

/*************************************************************************
 * Function Name: _SdInit
 * Parameters: none
 * Return: none
 *
 * Description: Init MCI module and Cart Present and Write Protect pins
 *
 *************************************************************************/
static
void _SdInit (void)
{
  PCLKSEL1_bit.PCLK_MCI = 1;  // set divider to /1
  PCONP_bit.PCSDC = 1;        // Enable MCI clk

#if defined(IAR_LPC_2478_STK)
  // Connect MCI signals to P1.2-P1.3,P1.05-P1.07 and P1.11,P1.12
  PINSEL2_bit.P1_2  = 2;
  PINSEL2_bit.P1_3  = 2;
  PINSEL2_bit.P1_5  = 2;
  PINSEL2_bit.P1_6  = 2;
  PINSEL2_bit.P1_7  = 2;
  PINSEL2_bit.P1_11 = 2;
  PINSEL2_bit.P1_12 = 2;

  // Set CP to input and disable pull resistors
  MMC_CP_FDIR &= ~MMC_CP_MASK;
  MMC_CP_MODE = 2;  // pin has neither pull-up nor pull-down

  // Set WP to input and disable pull resistors
  MMC_WP_FDIR &= ~MMC_WP_MASK;
  MMC_WP_MODE = 2;  // pin has neither pull-up nor pull-down
#elif defined(MICRO_BLOX_LPC2478)
  // Connect MCI signals to P0.19-P0.20,P0.21-P0.22 and P1.7,P1.11,P1.12
  PINSEL1_bit.P0_19 = 2;
  PINSEL1_bit.P0_20 = 2;
  PINSEL1_bit.P0_22 = 2;
  PINSEL2_bit.P1_7  = 2;
  PINSEL2_bit.P1_11 = 2;
  PINSEL2_bit.P1_12 = 2;

  SD_PWR_CTRL_SEL = 0;
  SD_PWR_CTRL_MODE = 2;
  SD_PWR_CTRL_FDIR = SD_PWR_CTRL_MASK;
  SD_PWR_CTRL_FSET = SD_PWR_CTRL_MASK;

#else
  // Connect MCI signals to P1.2-P1.3,P1.05-P1.07 and P1.11,P1.12
  PINSEL2_bit.P1_2  = 2;
  PINSEL2_bit.P1_3  = 2;
  PINSEL2_bit.P1_5  = 2;
  PINSEL2_bit.P1_6  = 2;
  PINSEL2_bit.P1_7  = 2;
  PINSEL2_bit.P1_11 = 2;
  PINSEL2_bit.P1_12 = 2;

  // Set CP to input and disable pull resistors
  MMC_CP_FDIR &= ~MMC_CP_MASK;
  MMC_CP_MODE = 2;  // pin has neither pull-up nor pull-down
#endif

  _SdPowerDown();

  // enable DMA , little endian
  PCONP_bit.PCGPDMA = 1;
  DMACCONFIGURATION = 1;
  DMACSYNC = 0;           // DMA sync enable
  DMACINTERRCLR = 3;
  DMACINTTCCLEAR = 3;
  DMACC0CONFIGURATION = DMACC1CONFIGURATION = 0;

}

/*************************************************************************
 * Function Name: _SdPowerDown
 * Parameters: none
 * Return: none
 *
 * Description: Set power down state
 *
 *************************************************************************/
static
void _SdPowerDown (void)
{
  // Init power state
  SCS_bit.MCIPWR = 1;
  MCIPOWER    = 0;

#if defined(MICRO_BLOX_LPC2478)
  SD_PWR_CTRL_FSET = SD_PWR_CTRL_MASK;
#endif // defined(MICRO_BLOX_LPC2478)

  SD_CMD_MODE = 2;

  _MCI_BUS_DLY();
  MCIMASK0    = 0;        // Disable all interrupts for now
  MCIMASK1    = MCIMASK0;
  MCICOMMAND  = 0;
  _MCI_BUS_DLY();
  MCIDATACTRL = 0;
  _MCI_BUS_DLY();
  MCICLEAR    = 0x7FF;    // clear all pending interrupts
  MCICLOCK_bit.WIDEBUS = 0;
  _MCI_BUS_DLY();

  // Clock Freq. Identification Mode < 400kHz
  _SdSetClockFreq(IdentificationModeClock);
}

/*************************************************************************
 * Function Name: _SdSendCmd
 * Parameters: SdCmdInd_t ComdInd,Int32U Arg
 *
 * Return: SdState_t
 *
 * Description: SD/MMC commands implement
 *
 *************************************************************************/
static
SdState_t _SdSendCmd(SdCmdInd_t ComdInd,pInt32U pArg)
{
Int32U Status;
union {
  Int32U Data;
  struct
  {
    Int32U CMDINDEX   : 6;
    Int32U RESPONSE   : 1;
    Int32U LONGRSP    : 1;
    Int32U INTERRUPT  : 1;
    Int32U PENDING    : 1;
    Int32U ENABLE     : 1;
    Int32U            :21;
  };
} Command;


  // the command engine must be disabled when we modify the argument or the
  // peripheral resends
  while (MCISTATUS_bit.CMDACTIVE )
  {
    // Command in progress.
    MCICOMMAND = 0;
    _MCI_BUS_DLY();
  }
  Command.Data = 0;

  // Send command code
  Command.CMDINDEX = _SdCmd[ComdInd].TxData;
  if(pArg != NULL)
  {
    switch(_SdCmd[ComdInd].Resp)
    {
    case SdR2:
      Command.LONGRSP = 1;
    case SdR1:
    case SdR1b:
    case SdR3:
    case SdR7:
      Command.RESPONSE = 1;
    }
  }
  Command.ENABLE = 1;

  // Send command's arguments
  if(_SdCmd[ComdInd].Arg != SdNoArg)
  {
    MCIARGUMENT = *pArg;
  }
  else
  {
    MCIARGUMENT = 0;
  }

  // Send command
  MCICOMMAND = Command.Data;
  _MCI_BUS_DLY();

  // Wait command respond
  while((Status = MCISTATUS & 0x000000C5) == 0);

  MCICLEAR = Status;
  // Command timeout
  if(Status & (1UL << 2))
  {
    MCICOMMAND = 0;
    _MCI_BUS_DLY();
    return(SdNoResponse);
  }

  // Command CRC fault
  if(Status & (1UL << 0))
  {
    switch(MCICOMMAND_bit.CMDINDEX)
    {
    // Ignore CRC Error
    case  1: // CMD1
    case 41: // ACMD42
    case 12: // CMD12
      MCICOMMAND = 0;
      _MCI_BUS_DLY();
      MCIARGUMENT = 0xFFFFFFFF;
      break;
    default:
      MCICOMMAND = 0;
      _MCI_BUS_DLY();
      return(SdCardError);
    }
  }

  if(pArg != NULL)
  {
    switch (_SdCmd[ComdInd].Resp)
    {
    case SdNoResp:
      break;
    case SdR3:
    case SdR7:
      *pArg = MCIRESPONSE0;
      break;
    case SdR2:
      *pArg++ = MCIRESPONSE0;
      *pArg++ = MCIRESPONSE1;
      *pArg++ = MCIRESPONSE2;
      *pArg++ = MCIRESPONSE3;
      break;
    default:
      if(_SdCmd[ComdInd].TxData != MCIRESPCMD_bit.RESPCMD)
      {
        return(SdCardError);
      }
      *pArg = MCIRESPONSE0;
    }
  }
  MCICOMMAND = 0;
  _MCI_BUS_DLY();
  return(SdOk);
}

/*************************************************************************
 * Function Name: _SdInitMedia
 * Parameters: none
 *
 * Return: SdState_t
 *
 * Description: SD/MMC detect and initialize
 *
 *************************************************************************/
static
SdState_t _SdInitMedia (void)
{
Int32U i,res;
volatile Int32U Dly;
Int8U MmcSdCid[16];

  _Tnac = 1;
  if(!_SdPresent())
  {
    if(MCIPOWER_bit.CTRL)
    {
      // Set to power down state
      _SdPowerDown();
    }
    return(SdNoPresent);
  }

  _SdPowerDown();
  SdDly_1ms(100);

#if defined(MICRO_BLOX_LPC2478)
  SD_PWR_CTRL_FCLR = SD_PWR_CTRL_MASK;
#endif // defined(MICRO_BLOX_LPC2478)
  MCIPOWER_bit.CTRL = 0x02;    // power up
  _MCI_BUS_DLY();
  while ((MCIPOWER_bit.CTRL != 0x02));
  SdDly_1ms(10);
  MCIPOWER_bit.CTRL = 0x01;   // power on
  _MCI_BUS_DLY();
  SdDly_1ms(100);
  MCICLOCK_bit.PWRSAVE = 0;
  SdDly_1ms(1);
  MCICLOCK_bit.PWRSAVE = 1;
  _MCI_BUS_DLY();

  // CMD0 (Go to IDLE)
  SD_CMD_MODE = 0;
  MCIPOWER_bit.OPENDRAIN = 1;
  _MCI_BUS_DLY();

  if(_SdSendCmd(CMD0,NULL) != SdOk)
  {
    return(SdNoResponse);
  }
  // Determinate Card type SD or MMC
  _SdDskCtrlBlk.DiskType = DiskSD_Spec2_0;
  res = Cmd8Reg;
  if(SdOk == _SdSendCmd(CMD8,&res))
  {
    _bHC = TRUE;
  }
  else
  {
    _SdDskCtrlBlk.DiskType = DiskSD_Spec1_x;
  }

  for(i=100;i;--i)
  {
    res = 0;
    if((_SdSendCmd(CMD55,&res) == SdOk) &&
       (res & 0x100))
    {
      res = _bHC?OcrReg | OcrReg_HC:OcrReg;
      if((_SdSendCmd(ACMD41,&res) == SdOk) &&
         (res & 0x80000000))
      {
        // SD card is find
        _bHC = 0 != (res & OcrReg_HC);
        break;
      }
    }
    else
    {
      _SdDskCtrlBlk.DiskType = DiskMMC;
      _MCI_BUS_DLY();
      // CMD1 for MMC Init sequence
      // will be complete within 500ms
      res = OcrReg;
      if(_SdSendCmd(CMD1,&res) ==  SdOk && (res & 0x80000000))
      {
        // MMC card is find
        _bHC = FALSE;
        break;
      }
    }

    SdDly_1ms(10);
  }


  if(i == 0)
  {
    return(SdNoResponse);
  }

  // Read CID
  if(_SdSendCmd(CMD2,(pInt32U)MmcSdCid) != SdOk)
  {
    return(SdNoResponse);
  }

  // Set address
  _CardRCA = (_SdDskCtrlBlk.DiskType == DiskMMC)?0x00010000:0x00000000;
  if(_SdSendCmd(CMD3,&_CardRCA) != SdOk)
  {
    return(SdNoResponse);
  }
  if(DiskMMC != _SdDskCtrlBlk.DiskType)
  {
    _CardRCA &= 0xFFFF0000;
  }
  else
  {
    _CardRCA  = 0x00010000;
  }

  // Set Pushpull CMD output type
  MCIPOWER_bit.OPENDRAIN = 0;
  SD_CMD_MODE = 2;
  _MCI_BUS_DLY();

  // Read CSD
  _MmcSdCsd[0] = 0;
  _MmcSdCsd[1] = 0;
  _MmcSdCsd[2] = _CardRCA >> 16;
  _MmcSdCsd[3] = _CardRCA >> 24;
  if(_SdSendCmd(CMD9,(pInt32U)_MmcSdCsd) != SdOk)
  {
    return(SdNoResponse);
  }

  // Implement CSD data
  _SdCsdImplemet();

  // Enter in TRAN state
  res = _CardRCA;
  if(_SdSendCmd(CMD7,&res) != SdOk)
  {
    return(SdNoResponse);
  }
  res = _CardRCA;
  if(SdOk != _SdSendCmd(CMD13,&res))
  {
    return(SdNoResponse);
  }
  else if(!(res & READY_FOR_DATA) ||
          ((res & CURRENT_STATE) != CARD_TRAN))
  {
    return(SdCardError);
  }

  // Set Block size
  res = _SdDskCtrlBlk.BlockSize;
  if(_SdSendCmd(CMD16,&res))
  {
    return(SdNoResponse);
  }

  if(DiskMMC != _SdDskCtrlBlk.DiskType)
  {
    // Use wide bus for SD
    MCICLOCK_bit.WIDEBUS = 1;
    _MCI_BUS_DLY();
    res = _CardRCA;
    if((_SdSendCmd(CMD55,&res) != SdOk) ||
       !(res & 0x100))
    {
      return(SdCardError);
    }
    // Set bus width 4bits
    res = 2;
    if(_SdSendCmd(ACMD6,&res) != SdOk)
    {
      return(SdCardError);
    }
  }
  return(SdOk);
}

/*************************************************************************
 * Function Name: _SdCsdImplemet
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement data from CSD
 *
 *************************************************************************/
static
void _SdCsdImplemet (void)
{
Int32U Freq;
Int64U Tmp;
  // Calculate SPI max clock
  Freq = _SdTransfExp[_CSD_GET_TRAN_SPEED_EXP()] * _SdCsdMant[_CSD_GET_TRAN_SPEED_MANT()];
  Freq = _SdSetClockFreq(Freq);
  if(DiskMMC == _SdDskCtrlBlk.DiskType)
  {
    // Calculate Time outs for MMC cards
    Tmp = _SdAccessTime[_CSD_GET_TAAC_EXP()] * _SdCsdMant[_CSD_GET_TAAC_MANT()];
    Tmp /= 10000; // us
    // Freq [Hz], Tmp[1 us], *10
    Tmp = (Freq*Tmp)/100000LL;
    // NSAC*100*10
    Tmp += 1000*_CSD_GET_NSAC();
    // Max time out
    _Tnac = Tmp;
    _Twr  = Tmp * (1<<_CSD_GET_R2W_FACTOR());
  }
  else
  {
    // Calculate Time outs for SD cards
    // Freq [Hz], RD_TIME_OUT[ms]
    Tmp = Freq/1000;
    _Tnac = Tmp * RD_TIME_OUT;
    _Twr  = Tmp * WR_TIME_OUT;
  }
  // Calculate Block size and Block Number
  _SdDskCtrlBlk.BlockSize = 1<<_CSD_GET_READ_BL_LEN();
  if(_bHC)
  {
    _SdDskCtrlBlk.BlockNumb = (_CSD_2_0_GET_C_SIZE()+1) * 1024;
  }
  else
  {
    _SdDskCtrlBlk.BlockNumb = (_CSD_GET_C_SIZE()+1)*(4<<_CSD_GET_C_SIZE_MULT());
  }
  if(_SdDskCtrlBlk.BlockSize != SD_DEF_BLK_SIZE)
  {
    // because Windows support only 512 bytes block
    _SdDskCtrlBlk.BlockNumb *= (_SdDskCtrlBlk.BlockSize>>9);
    _SdDskCtrlBlk.BlockSize  = SD_DEF_BLK_SIZE;
  }
  // Set Write Protect
  _bSdPermWriteProtect = _CSD_GET_PERM_WRITE_PROTECT() |\
                          _CSD_GET_TMP_WRITE_PROTECT();
  _SdDskCtrlBlk.WriteProtect = _SdWriteProtect() |\
                              _bSdPermWriteProtect;
}

/*************************************************************************
 * Function Name: _SdRead
 * Parameters: pInt8U pData, Int32U Add, Int32U Length
 *
 * Return: SdState_t
 *
 * Description: Read from SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdRead(pInt8U pData, Int32U Add, Int32U Length)
{
Int32U Data;
SdState_t Status = SdOk;
DataCtrl_t DataCtrl = {0};

  if(Length == 0)
  {
    return(Status);
  }

  // Set read time out
  MCIDATATIMER = _Tnac;
  // Clear all related flags
  MCICLEAR = 0x0000072A;

  // DMA init
  // set source address
  DMACC0SRCADDR = (Int32U)&MCIFIFO0;
  // set destination address
  DMACC0LLI = 0;                  // no linked list
  DMACC0CONTROL_bit.SWIDTH = 2;   // source width 32 bits
  DMACC0CONTROL_bit.DWIDTH = 0;   // destination width 8 bits
  DMACC0CONTROL_bit.SBSIZE = 2;   // 8 words
  DMACC0CONTROL_bit.DBSIZE = 2;   // 8 words
  DMACC0CONTROL_bit.DI = 1;       // destination address increment
  DMACC0CONTROL_bit.SI = 0;       // source address no increment
  DMACC0CONTROL_bit.I  = 1;       // enable terminal count interrupt

  while(Length)
  {
    // Set data size for one block
    MCIDATALENGTH = _SdDskCtrlBlk.BlockSize;
    Data = Add;
    Status = _SdSendCmd(CMD17,&Data);
    if(SdOk != Status)
    {
      return(Status);
    }

    if(!(Data & READY_FOR_DATA) ||
       ((Data & CURRENT_STATE) != CARD_TRAN))
    {
      _SdSendCmd(CMD12,NULL);
      return(SdCardError);
    }

    DMACC0DESTADDR = (Int32U)pData;
    // Peripheral to memory. Peripheral control
    DMACC0CONFIGURATION_bit.SRCPERIPHERAL = DMA_MMCSD;
    DMACC0CONFIGURATION_bit.DESTPERIPHERAL = 0;
    DMACC0CONFIGURATION_bit.FLOWCNTRL = 6;
    DMACC0CONFIGURATION_bit.L = 1;
    DMACC0CONFIGURATION_bit.E = 1;

    // block length
    DataCtrl.BLOCKSIZE = 9;
    // Data direction
    DataCtrl.DIRECTION = 1;
    // Block transfer
    DataCtrl.MODE = 0;
    // DMA disable
    DataCtrl.DMAENABLE = 1;
    // Enable data path
    DataCtrl.ENABLE = 1;
    MCIDATACTRL = DataCtrl.Data;
    _MCI_BUS_DLY();

    // wait receive complete
    while(MCISTATUS_bit.RXACTIVE);

    DMACC0CONFIGURATION_bit.E = 0;
    if(DMACRAWINTERRORSTATUS_bit.RAWINTERRORSTATUS0)
    {
      // clear error flag and terminate transfer
      DMACINTERRCLR_bit.INTERRCLR0 = 1;
      return(SdDmaError);
    }

    DMACINTTCCLEAR_bit.INTTCCLEAR0 = 1;

    MCIDATACTRL = 0;
    _MCI_BUS_DLY();

    // data timeout
    if(MCISTATUS_bit.DATATIMEOUT)
    {
      MCICLEAR = 1UL << 3;
      return(SdNoResponse);
    }
    // Start bit not detected on all data signals in wide bus mode.
    if(MCISTATUS_bit.STARTBITERR)
    {
      MCICLEAR = 1UL << 9;
      return(SdCardError);
    }

    // CRC error
    if(MCISTATUS_bit.DATACRCFAIL)
    {
      MCICLEAR = 1UL << 1;
      return(SdCardError);
    }
    // rx overrun
    if(MCISTATUS_bit.RXOVERRUN)
    {
      MCICLEAR = 1UL << 5;
      return(SdCardError);
    }

    if(_bHC)
    {
      ++Add;
    }
    else
    {
      Add += _SdDskCtrlBlk.BlockSize;
    }
    --Length;
    pData += _SdDskCtrlBlk.BlockSize;

    // Clear data end and data block end
    MCICLEAR = (1UL << 8) | (1UL << 10);
    // data end
    if(!Length)
    {
      return(Status);
    }
    // loop read next block
  }
  return(Status);
}

/*************************************************************************
 * Function Name: _SdWrite
 * Parameters: const Int8U * pData, Int32U Add, Int32U Length
 *
 * Return: SdState_t
 *
 * Description: Write to SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdWrite(const Int8U * pData, Int32U Add, Int32U Length)
{
Int32U Data;
SdState_t Status = SdOk;
DataCtrl_t DataCtrl = {0};

  if(!Length)
  {
    return(Status);
  }

  // Set write time out
  MCIDATATIMER = _Twr;
  // Clear all related flags
  MCICLEAR = 0x0000072A;

  // DMA init
  // set source address
  DMACC0DESTADDR = (Int32U)&MCIFIFO0;
  DMACC0LLI = 0;                  // no linked list
  DMACC0CONTROL_bit.SWIDTH = 0;   // source width 8 bits
  DMACC0CONTROL_bit.DWIDTH = 2;   // destination width 32 bits
  DMACC0CONTROL_bit.SBSIZE = 2;   // 8 words
  DMACC0CONTROL_bit.DBSIZE = 2;   // 8 words
  DMACC0CONTROL_bit.DI = 0;       // destination address increment
  DMACC0CONTROL_bit.SI = 1;       // source address increment
  DMACC0CONTROL_bit.I  = 1;       // enable terminal count interrupt

  while(Length)
  {
    // Set data size for one block
    MCIDATALENGTH = _SdDskCtrlBlk.BlockSize;

    Data = Add;
    Status = _SdSendCmd(CMD24,&Data);
    if(SdOk != Status)
    {
      return(Status);
    }

    if(!(Data & READY_FOR_DATA) ||
       ((Data & CURRENT_STATE) != CARD_TRAN))
    {
      _SdSendCmd(CMD12,NULL);
      return(SdCardError);
    }

    // set destination address
    DMACC0SRCADDR = (Int32U)pData;
    // Peripheral to memory. Peripheral control
    DMACC0CONFIGURATION_bit.SRCPERIPHERAL = 0;
    DMACC0CONFIGURATION_bit.DESTPERIPHERAL = DMA_MMCSD;
    DMACC0CONFIGURATION_bit.FLOWCNTRL = 5;
    DMACC0CONFIGURATION_bit.L = 1;
    DMACC0CONFIGURATION_bit.E = 1;

    // block length
    DataCtrl.BLOCKSIZE = 9;
    // Data direction
    DataCtrl.DIRECTION = 0;
    // Block transfer
    DataCtrl.MODE = 0;
    // DMA disable
    DataCtrl.DMAENABLE = 1;
    // Enable data path
    DataCtrl.ENABLE = 1;
    MCIDATACTRL = DataCtrl.Data;
    _MCI_BUS_DLY();

    // wait transfer complete
    while(MCISTATUS_bit.TXACTIVE);

    DMACC0CONFIGURATION_bit.E = 0;
    if(DMACRAWINTERRORSTATUS_bit.RAWINTERRORSTATUS0)
    {
      // clear error flag and terminate transfer
      DMACINTERRCLR_bit.INTERRCLR0 = 1;
      return(SdDmaError);
    }

    DMACINTTCCLEAR_bit.INTTCCLEAR0 = 1;

    MCIDATACTRL = 0;
    _MCI_BUS_DLY();

    // data timeout
    if(MCISTATUS_bit.DATATIMEOUT)
    {
      MCICLEAR = 1UL << 3;
      return(SdNoResponse);
    }
    // Start bit not detected on all data signals in wide bus mode.
    if(MCISTATUS_bit.STARTBITERR)
    {
      MCICLEAR = 1UL << 9;
      return(SdCardError);
    }
    // CRC error
    if(MCISTATUS_bit.DATACRCFAIL)
    {
      MCICLEAR = 1UL << 1;
      return(SdCardError);
    }
    // tx underrun
    if(MCISTATUS_bit.TXUNDERRUN)
    {
      MCICLEAR = 1UL << 4;
      return(SdCardError);
    }

    if(_bHC)
    {
      ++Add;
    }
    else
    {
      Add += _SdDskCtrlBlk.BlockSize;
    }
    --Length;
    pData += _SdDskCtrlBlk.BlockSize;

    // Clear data end and data block end
    MCICLEAR = (1UL << 8) | (1UL << 10);
    // data end
    if(!Length)
    {
      return(Status);
    }
    // loop write next block
  }
  return(Status);
}

/*************************************************************************
 * Function Name: MmcVerify
 * Parameters: const Int8U * pData, Int32U Add, Int32U Length
 *
 * Return: SdState_t
 *
 * Description: Verify data in SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdVerify(const Int8U * pData, Int32U Add, Int32U Length)
{
#pragma segment="USB_DMA_RAM"
#pragma location="USB_DMA_RAM"
static __no_init Int8U TempBuffer[2048];  // maximum block length is 2048
SdState_t Status = SdOk;

  while(Length)
  {
    Status = _SdRead(TempBuffer,Add,_SdDskCtrlBlk.BlockSize);
    if(Status != SdOk)
    {
      break;
    }
    for(Int32U i = 0; i < _SdDskCtrlBlk.BlockSize; ++i)
    {
      if(TempBuffer[i] != *pData++)
      {
        return(SdMiscompare);
      }
    }
    if(_bHC)
    {
      --Length;
      ++Add;
    }
    else
    {
      Length -= _SdDskCtrlBlk.BlockSize;
      Add += _SdDskCtrlBlk.BlockSize;
    }
    pData += _SdDskCtrlBlk.BlockSize;
  }
  return(Status);
}

/*************************************************************************
 * Function Name: SdGetLastError
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Return Last error
 *
 *************************************************************************/
Int32U SdGetLastError(void)
{
  return(_SdLastError);
}

/*************************************************************************
 * Function Name: SdStatusUpdate
 * Parameters: none
 *
 * Return: none
 *
 * Description: Update status of SD/MMC card
 *
 *************************************************************************/
void SdStatusUpdate (void)
{
Int32U Data = _CardRCA;
  // Update WP state
  _SdDskCtrlBlk.WriteProtect = _SdWriteProtect() |\
                               _bSdPermWriteProtect;
  if(!_SdPresent())
  {
    _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
    _SdPowerDown();
    return;
  }

  if((_SdDskCtrlBlk.DiskStatus != DiskCommandPass))
  {
    switch (_SdInitMedia())
    {
    case SdOk:
      _SdDskCtrlBlk.DiskStatus = DiskCommandPass;
      _SdDskCtrlBlk.MediaChanged = TRUE;
      break;
    case SdCardError:
      _SdDskCtrlBlk.DiskStatus = DiskNotReady;
      break;
    default:
      _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
      break;
    }
  }
  else if(_SdSendCmd(CMD13,&Data) != SdOk)
  {
    _SdDskCtrlBlk.DiskStatus = DiskNotReady;
  }
  else if (!(Data & READY_FOR_DATA))
  {
    _SdDskCtrlBlk.DiskStatus = DiskNotReady;
  }
  else if ((Data & CURRENT_STATE) == CARD_TRAN)
  {
    _SdDskCtrlBlk.DiskStatus = DiskCommandPass;
  }
  else
  {
    _SdDskCtrlBlk.DiskStatus = DiskNotReady;
  }
}

/*************************************************************************
 * Function Name: SdDiskInit
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Init MMC/SD disk
 *
 *************************************************************************/
void SdDiskInit (void)
{
  _SdDskCtrlBlk.BlockNumb =\
  _SdDskCtrlBlk.BlockSize =\
  _SdLastError           = 0;
  // Init SPI
  _SdInit();
  // Media Init
  switch (_SdInitMedia())
  {
  case SdOk:
    _SdDskCtrlBlk.DiskStatus = DiskCommandPass;
    _SdDskCtrlBlk.MediaChanged = TRUE;
    break;
  case SdCardError:
    _SdDskCtrlBlk.DiskStatus = DiskNotReady;
    break;
  default:
    _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
    break;
  }
}

/*************************************************************************
 * Function Name: SdGetDiskCtrlBkl
 * Parameters:  none
 *
 * Return: pDiskCtrlBlk_t
 *
 * Description: Return pointer to control block structure of the disk
 *
 *************************************************************************/
pDiskCtrlBlk_t SdGetDiskCtrlBkl (void)
{
  return(&_SdDskCtrlBlk);
}

/*************************************************************************
 * Function Name: SdDiskIO
 * Parameters: pInt8U pData,Int32U BlockStart,
 *             Int32U BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: MMC/SD disk I/O
 *
 *************************************************************************/
DiskStatusCode_t SdDiskIO (pInt8U pData,Int32U BlockStart,
                              Int32U BlockNum, DiskIoRequest_t IoRequest)
{
  if((pData == NULL) || (BlockStart+BlockNum > _SdDskCtrlBlk.BlockNumb))
  {
    return(DiskParametersError);
  }
  if (_SdDskCtrlBlk.DiskStatus != DiskCommandPass)
  {
    return(_SdDskCtrlBlk.DiskStatus);
  }
  switch (IoRequest)
  {
  case DiskWrite:
    if(_SdDskCtrlBlk.WriteProtect)
    {
      return(DiskParametersError);
    }
    switch (_SdWrite(pData,
                     _bHC?BlockStart:BlockStart*_SdDskCtrlBlk.BlockSize,
                     BlockNum))
    {
    case SdOk:
      break;
    case SdCardError:
      _SdDskCtrlBlk.DiskStatus = DiskNotReady;
      break;
    default:
      _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
      break;
    }
    break;
  case DiskRead:
    switch (_SdRead(pData,
                   _bHC?BlockStart:BlockStart*_SdDskCtrlBlk.BlockSize,
                   BlockNum))
    {
    case SdOk:
      break;
    case SdCardError:
      _SdDskCtrlBlk.DiskStatus = DiskNotReady;
      break;
    default:
      _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
      break;
    }
    break;
  case DiskVerify:
    switch (_SdVerify(pData,
                     _bHC?BlockStart:BlockStart*_SdDskCtrlBlk.BlockSize,
                     BlockNum))
    {
    case SdOk:
      break;
    case SdMiscompare:
      return(DiskMiscompareError);
    case SdCardError:
      _SdDskCtrlBlk.DiskStatus = DiskNotReady;
      break;
    default:
      _SdDskCtrlBlk.DiskStatus = DiskNotPresent;
      break;
    }
    break;
  default:
    return(DiskParametersError);
  }
  return(_SdDskCtrlBlk.DiskStatus);
}

