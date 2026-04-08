/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_vs1002.h
 *    Description : VS1002 Driver include file
 *
 *    History :
 *    1. Date        : August 10, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "drv_vs1002_cnfg.h"

#ifndef  __DRV_VS1002_H
#define  __DRV_VS1002_H

#define VS1002_ReadCmd     0x03
#define VS1002_WriteCmd    0x02

#define VS1002_VS1002_REV  2
#define VS1053_REV  4

#define VS1002_BUFFER_SIZE  2048

typedef enum _Dma_BurstSize_t
{
  Dma_1_Transfer = 0, Dma_4_Transfer, Dma_8_Transfer, Dma_16_Transfer,
  Dma_32_Transfer, Dma_64_Transfer, Dma_128_Transfer, Dma_254_Transfer
} Dma_BurstSize_t;

typedef enum _Dma_Width_t
{
  Dma_Byte = 0, Dma_HalfWord, Dma_Word
} Dma_Width_t;

typedef struct _DmaLli_t
{
  uint32_t DMA_SrcAddr;
  uint32_t DMA_DstAddr;
  uint32_t DMA_NextLli;
  union
  {
    uint32_t DMA_Ctrl;
    struct
    {
      uint32_t TransferSize :12;
      uint32_t SBSize       : 3;
      uint32_t DBSize       : 3;
      uint32_t SWidth       : 3;
      uint32_t DWidth       : 3;
      uint32_t              : 2;
      uint32_t SInc         : 1;
      uint32_t DInc         : 1;
      uint32_t Prot0        : 1;
      uint32_t Prot1        : 1;
      uint32_t Prot2        : 1;
      uint32_t TermCntIE    : 1;
    };
  };
} DmaLli_t, *pDmaLli_t;

//#define VS1002_VOL_STEP    2     // 0.5 dB

typedef enum _VS1002_RegAddr_t
{
  VS1002_MODE = 0, VS1002_STATUS, VS1002_BASS, VS1002_CLOCKF, VS1002_DECODE_TIME, VS1002_AUDATA,
  VS1002_WRAM, VS1002_WRAMADDR, VS1002_HDAT0, VS1002_HDAT1, VS1002_AIADDR, VS1002_VOL,
  VS1002_AICTRL0,VS1002_AICTRL1, VS1002_AICTRL2, VS1002_AICTRL3
} VS1002_RegAddr_t;

typedef enum _VS1002_Status_t
{
  VS1002_Pass = 0,
  VS1002_Fault,
  VS1002_WrongRev,
  VS1002_NotComm,
} VS1002_Status_t;

typedef enum _VS1002_Cmd_t
{
  VS1002_CmdPowerUp = 0, VS1002_CmdPowerDown, VS1002_CmdGetRevision , VS1002_CmdSetClkReg,
  VS1002_CmdSetModeReg, VS1002_CmdSetVol, VS1002_CmdTstSin, VS1002_CmdPlay,
  VS1002_CmdPlayStop, VS1002_CmdWideStereo, VS1002_CmdLoudness, VS1002_CmdReset
} VS1002_Cmd_t;

typedef enum VS1002_TransferStatus_t
{
  VS1002_NotDataTransfer = 0, VS1002_DataTransferProgress,
  VS1002_DataTransferComplete, VS1002_DataTransferCompleteWithError,
} VS1002_TransferStatus_t;

typedef enum _VS1002_PlaySpeed_t
{
  VS1002_PlayNorm = 0, VS1002_PlayIncreaseSpeed, VS1002_PlayDecreaseSpeed,
} VS1002_PlaySpeed_t;

typedef struct _VS1002_Stream_t
{
  uint8_t * pStream;
  uint32_t Size;
  VS1002_PlaySpeed_t PlaySpeed;
} VS1002_Stream_t, *pVS1002_Stream_t;

typedef enum _VS1002_StreamId_t
{
  MPG_2_5_1_4a = 0, MPG_2_5_1_4b, MPG_2_0_1_2, ISO_11172_3_1_0
} VS1002_StreamId_t;

extern void DelayResolution100us(uint32_t Dealy );
#define VS1002_SpiBufferDrainDly() for(volatile uint32_t drain_dly = 15; drain_dly; --drain_dly)

/*************************************************************************
 * Function Name: VS1002_SendCmd
 * Parameters: VS1002_Cmd_t Cmd, uint32_t * pData
 * Return: none
 *
 * Description: Send command to the MP3 module (VS1002)
 *
 *************************************************************************/
void VS1002_SendCmd (VS1002_Cmd_t Cmd, uint32_t * pData);

#endif  /* __DRV_VS1002_H */
