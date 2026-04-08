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
#include "lpc407x_8x.h"
#include "board.h"
#include "arm_comm.h"

#include "disk.h"
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
                        for(volatile uint32_t i = 10; i;--i);\
                      }\

const uint32_t _SdTransfExp[] =
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

const uint32_t _SdAccessTime [] =
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

const uint32_t _SdCsdMant[] =
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
uint32_t _SdLastError,_Tnac,_Twr;
Boolean _bSdChanged;
#pragma data_alignment = 4
uint8_t _MmcSdCsd[16];

static uint32_t _CardRCA;
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
  return(!(SD_CP_FIO & SD_CP_MASK));
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
  return(0 != (SD_WP_FIO & SD_WP_MASK));
}

/*************************************************************************
 * Function Name: _SdSetClockFreq
 * Parameters: uint32_t Frequency
 * Return: uint32_t
 *
 * Description: Set SD/MMC ckl frequency
 *
 *************************************************************************/
static
uint32_t _SdSetClockFreq (uint32_t Frequency)
{
uint32_t Pclk = PeripheralClock;
uint32_t Div;
uint32_t tmp;
  Frequency <<= 1;

  for(Div = 4; Div < 256; ++Div)
  {
    if((Frequency * Div) > Pclk)
    {
      break;
    }
  }
  /* Set divider, clk enable and power save */
  tmp = LPC_MCI->CLOCK & ~0xFF;
  LPC_MCI->CLOCK = tmp | ((Div - 1) & 0xFF);
  _MCI_BUS_DLY();
  /*clock enable*/
  LPC_MCI->CLOCK |= (1<<8);
  _MCI_BUS_DLY();
  /*Clock enabled when bus is active*/
  LPC_MCI->CLOCK |= (1<<9);
  _MCI_BUS_DLY();
  /* Return real frequency */
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
  
  LPC_SC->PCONP |= (1<<28);        /* Enable MCI clk */

  /* Connect MCI signals to P1.2-P1.3,P1.05-P1.07 and P1.11,P1.12 */
  SD_PWR_IOCON = 0xA2;
  SD_CMD_IOCON = 0x22;
  SD_CLK_IOCON = 0x22;
  SD_DAT_0_IOCON = 0xA2;
  SD_DAT_1_IOCON = 0xA2;
  SD_DAT_2_IOCON = 0x22;
  SD_DAT_3_IOCON = 0x22;
    
  /* Set CP to input and disable pull resistors */
  SD_CP_FDIR &= ~SD_CP_MASK;
  SD_CP_IOCON = 0x20; /* pin has neither pull-up nor pull-down */

  /* Set WP to input and disable pull resistors */
  SD_WP_FDIR &= ~SD_WP_MASK;
  SD_WP_IOCON = 0x20;  /* pin has neither pull-up nor pull-down */

  _SdPowerDown();

  /* enable DMA , little endian */
  LPC_SC->PCONP |= (1<<29);
  LPC_GPDMA->Config = 1;
  LPC_GPDMA->Sync = 0;   /* DMA sync enable */
  LPC_GPDMA->IntErrClr = 3;
  LPC_GPDMA->IntTCClear = 3;
  LPC_GPDMACH0->CConfig = 0;
  LPC_GPDMACH0->CConfig = 0;

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
  /* Init power state */
  LPC_SC->SCS |= (1<<3);
  LPC_MCI->POWER = 0;

  SD_CMD_IOCON = 0x22;

  _MCI_BUS_DLY();
  LPC_MCI->MASK0 = 0;        /* Disable all interrupts for now */
  LPC_MCI->COMMAND = 0;
  _MCI_BUS_DLY();
  LPC_MCI->DATACTRL = 0;
  _MCI_BUS_DLY();
  LPC_MCI->CLEAR = 0x7FF;    /* clear all pending interrupts */
  LPC_MCI->CLOCK &= ~(1<<11);
  _MCI_BUS_DLY();

  // Clock Freq. Identification Mode < 400kHz
  _SdSetClockFreq(IdentificationModeClock);
}

/*************************************************************************
 * Function Name: _SdSendCmd
 * Parameters: SdCmdInd_t ComdInd,uint32_t Arg
 *
 * Return: SdState_t
 *
 * Description: SD/MMC commands implement
 *
 *************************************************************************/
static
SdState_t _SdSendCmd(SdCmdInd_t ComdInd,uint32_t * pArg)
{
uint32_t Status;
union {
  uint32_t Data;
  struct
  {
    uint32_t CMDINDEX   : 6;
    uint32_t RESP   : 1;
    uint32_t LONGRSP    : 1;
    uint32_t INTERRUPT  : 1;
    uint32_t PENDING    : 1;
    uint32_t ENABLE     : 1;
    uint32_t            :21;
  };
} Command;


  /* the command engine must be disabled when we modify the argument or the
     peripheral resends */
  while (LPC_MCI->STATUS & (1<<11))
  {
    /* Command in progress */
    LPC_MCI->COMMAND = 0;
    _MCI_BUS_DLY();
  }
  Command.Data = 0;

  /* Send command code */
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
      Command.RESP = 1;
    }
  }
  Command.ENABLE = 1;

  /*  Send command's arguments */
  if(_SdCmd[ComdInd].Arg != SdNoArg)
  {
    LPC_MCI->ARGUMENT = *pArg;
  }
  else
  {
    LPC_MCI->ARGUMENT = 0;
  }

  /* Send command */
  LPC_MCI->COMMAND = Command.Data;
  _MCI_BUS_DLY();

  // Wait command respond
  while((Status = LPC_MCI->STATUS & 0x000000C5) == 0);

  LPC_MCI->CLEAR = Status;
  // Command timeout
  if(Status & (1UL << 2))
  {
    LPC_MCI->COMMAND = 0;
    _MCI_BUS_DLY();
    return(SdNoResponse);
  }

  // Command CRC fault
  if(Status & (1UL << 0))
  {
    switch(LPC_MCI->COMMAND & 0x3F)
    {
    // Ignore CRC Error
    case  1: // CMD1
    case 41: // ACMD42
    case 12: // CMD12
      LPC_MCI->COMMAND = 0;
      _MCI_BUS_DLY();
      LPC_MCI->ARGUMENT = 0xFFFFFFFF;
      break;
    default:
      LPC_MCI->COMMAND = 0;
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
      *pArg = LPC_MCI->RESP0;
      break;
    case SdR2:
      *pArg++ = LPC_MCI->RESP0;
      *pArg++ = LPC_MCI->RESP1;
      *pArg++ = LPC_MCI->RESP2;
      *pArg++ = LPC_MCI->RESP3;
      break;
    default:
      if(_SdCmd[ComdInd].TxData != LPC_MCI->RESP_CMD)
      {
        return(SdCardError);
      }
      *pArg = LPC_MCI->RESP0;
    }
  }
  LPC_MCI->COMMAND = 0;
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
uint32_t i,res;
volatile uint32_t Dly;
uint8_t MmcSdCid[16];
uint32_t tmp;
  _Tnac = 1;
  if(!_SdPresent())
  {
    if(LPC_MCI->POWER & 0x3)
    {
      /* Set to power down state */
      _SdPowerDown();
    }
    return(SdNoPresent);
  }

  _SdPowerDown();
  SdDly_1ms(100);

  tmp = LPC_MCI->POWER & ~0x3;
  LPC_MCI->POWER = tmp | 0x02;    /* power up */
  
  _MCI_BUS_DLY();
  while(((LPC_MCI->POWER & 0x3) != 0x02));
  
  SdDly_1ms(10);
  LPC_MCI->POWER = tmp | 0x01;    /* power on */
   
  _MCI_BUS_DLY();
  SdDly_1ms(100);
  LPC_MCI->CLOCK &= ~(1<<9);
  SdDly_1ms(1);
  LPC_MCI->CLOCK |= (1<<9);
  _MCI_BUS_DLY();

  /* CMD0 (Go to IDLE) */
  SD_CMD_IOCON = 0x32;
  LPC_MCI->POWER |= (1<<6);
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
  if(_SdSendCmd(CMD2,(uint32_t *)MmcSdCid) != SdOk)
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
  LPC_MCI->POWER &= ~(1<<6);
  SD_CMD_IOCON = 0x22;
  _MCI_BUS_DLY();

  // Read CSD
  _MmcSdCsd[0] = 0;
  _MmcSdCsd[1] = 0;
  _MmcSdCsd[2] = _CardRCA >> 16;
  _MmcSdCsd[3] = _CardRCA >> 24;
  if(_SdSendCmd(CMD9,(uint32_t *)_MmcSdCsd) != SdOk)
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
    /* Use wide bus for SD */
    LPC_MCI->CLOCK |= (1<<11);
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
uint32_t Freq;
uint64_t Tmp;
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
 * Parameters: uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: SdState_t
 *
 * Description: Read from SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdRead(uint8_t * pData, uint32_t Add, uint32_t Length)
{
uint32_t Data;
SdState_t Status = SdOk;
DataCtrl_t DataCtrl = {0};

  if(Length == 0)
  {
    return(Status);
  }

  // Set read time out
  LPC_MCI->DATATMR = _Tnac;
  // Clear all related flags
  LPC_MCI->CLEAR = 0x0000072A;

  // DMA init
  // set source address
  LPC_GPDMACH0->CSrcAddr = (uint32_t)LPC_MCI->FIFO;
  // set destination address
  LPC_GPDMACH0->CLLI = 0;         // no linked list
  LPC_GPDMACH0->CControl = (2<<12) |  // Source burst size 8 words
                           (2<<15) |  // Destination burst size
                           (2<<18) |  // source width 32 bits
                           (0<<21) |  // destination width 8 bits
                           (0<<26) |  // source address no increment
                           (1<<27) |  // destination address increment
                           (1<<31) ;  // enable terminal count interrupt

  while(Length)
  {
    // Set data size for one block
    LPC_MCI->DATALEN = _SdDskCtrlBlk.BlockSize;
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

    LPC_GPDMACH0->CDestAddr = (uint32_t)pData;
    // Peripheral to memory. Peripheral control
    LPC_GPDMACH0->CConfig = (1<<0) |    /* channel Enabled */
             (GPDMA_SD_TO_MAT1<<1) |    /* Source peripheral*/
                           (6<<11) |    /* Peripheral to memory Source peripheral*/
                           (1<<16) ;    /* enable locked transfers*/


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
    LPC_MCI->DATACTRL = DataCtrl.Data;
    _MCI_BUS_DLY();

    // wait receive complete
    while(LPC_MCI->STATUS & (1<<13));

    LPC_GPDMACH0->CConfig = ~(1<<0);
    if(LPC_GPDMA->RawIntErrStat & (1<<0))
    {
      // clear error flag and terminate transfer
      LPC_GPDMA->IntErrClr = (1<<0);
      return(SdDmaError);
    }

    LPC_GPDMA->IntTCClear = (1<<0);

    LPC_MCI->DATACTRL = 0;
    _MCI_BUS_DLY();

    // data timeout
    if(LPC_MCI->STATUS & (1<<3))
    {
      LPC_MCI->CLEAR = 1 << 3;
      return(SdNoResponse);
    }
    // Start bit not detected on all data signals in wide bus mode.
    if(LPC_MCI->STATUS & (1<<9))
    {
      LPC_MCI->CLEAR = 1 << 9;
      return(SdCardError);
    }

    // CRC error
    if(LPC_MCI->STATUS & (1<<1))
    {
      LPC_MCI->CLEAR = 1 << 1;
      return(SdCardError);
    }
    // rx overrun
    if(LPC_MCI->STATUS & (1<<5))
    {
      LPC_MCI->CLEAR = 1 << 5;
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
    LPC_MCI->CLEAR = (1UL << 8) | (1UL << 10);
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
 * Parameters: const uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: SdState_t
 *
 * Description: Write to SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdWrite(const uint8_t * pData, uint32_t Add, uint32_t Length)
{
uint32_t Data;
SdState_t Status = SdOk;
DataCtrl_t DataCtrl = {0};

  if(!Length)
  {
    return(Status);
  }

  // Set write time out
  LPC_MCI->DATATMR = _Twr;
  // Clear all related flags
  LPC_MCI->CLEAR = 0x0000072A;

  // DMA init
  // set source address
  LPC_GPDMACH0->CDestAddr = (uint32_t)LPC_MCI->FIFO;
  LPC_GPDMACH0->CLLI = 0;                  // no linked list
  LPC_GPDMACH0->CControl = (2<<12) |  // Source burst size 8 words
                           (2<<15) |  // Destination burst size
                           (0<<18) |  // source width 8 bits
                           (2<<21) |  // destination width 32 bits
                           (1<<26) |  // source address increment
                           (0<<27) |  // destination address no increment
                           (1<<31) ;  // enable terminal count interrupt

  while(Length)
  {
    // Set data size for one block
    LPC_MCI->DATALEN = _SdDskCtrlBlk.BlockSize;

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
    LPC_GPDMACH0->CSrcAddr = (uint32_t)pData;
    // Peripheral to memory. Peripheral control
    LPC_GPDMACH0->CConfig = (1<<0) |    /* channel Enabled */
             (GPDMA_SD_TO_MAT1<<6) |    /* destination periferial*/
                           (5<<11) |    /* Peripheral to memory Source */
                           (1<<16) ;    /* enable locked transfers*/

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
    LPC_MCI->DATACTRL = DataCtrl.Data;
    _MCI_BUS_DLY();

    // wait transfer complete
    while(LPC_MCI->STATUS & (1<<12));

    LPC_GPDMACH0->CConfig = ~(1<<0);
    if(LPC_GPDMA->RawIntErrStat & (1<<0))
    {
      // clear error flag and terminate transfer
      LPC_GPDMA->IntErrClr = (1<<0);
      return(SdDmaError);
    }

    LPC_GPDMA->IntTCClear = (1<<0);

    LPC_MCI->DATACTRL = 0;
    _MCI_BUS_DLY();

    // data timeout
    if(LPC_MCI->STATUS & (1<<3))
    {
      LPC_MCI->CLEAR = 1 << 3;
      return(SdNoResponse);
    }
    // Start bit not detected on all data signals in wide bus mode.
    if(LPC_MCI->STATUS & (1<<9))
    {
      LPC_MCI->CLEAR = 1 << 9;
      return(SdCardError);
    }

    // CRC error
    if(LPC_MCI->STATUS & (1<<1))
    {
      LPC_MCI->CLEAR = 1 << 1;
      return(SdCardError);
    }
    // tx underrun
    if(LPC_MCI->STATUS & (1<<4))
    {
      LPC_MCI->CLEAR = 1 << 4;
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
    LPC_MCI->CLEAR = (1UL << 8) | (1UL << 10);
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
 * Parameters: const uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: SdState_t
 *
 * Description: Verify data in SD/MMC
 *
 *************************************************************************/
static inline
SdState_t _SdVerify(const uint8_t * pData, uint32_t Add, uint32_t Length)
{
#pragma segment="USB_DMA_RAM"
#pragma location="USB_DMA_RAM"
static __no_init uint8_t TempBuffer[2048];  // maximum block length is 2048
SdState_t Status = SdOk;

  while(Length)
  {
    Status = _SdRead(TempBuffer,Add,_SdDskCtrlBlk.BlockSize);
    if(Status != SdOk)
    {
      break;
    }
    for(uint32_t i = 0; i < _SdDskCtrlBlk.BlockSize; ++i)
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
 * Return: uint32_t
 *
 * Description: Return Last error
 *
 *************************************************************************/
uint32_t SdGetLastError(void)
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
uint32_t Data = _CardRCA;
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
 * Parameters: uint8_t * pData,uint32_t BlockStart,
 *             uint32_t BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: MMC/SD disk I/O
 *
 *************************************************************************/
DiskStatusCode_t SdDiskIO (uint8_t * pData,uint32_t BlockStart,
                              uint32_t BlockNum, DiskIoRequest_t IoRequest)
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

