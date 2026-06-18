/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : bot.h
 *    Description : Define USB Mass storage device bulk-only transport module
 *
 *    History :
 *    1. Date        : September 8, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __BOT_H
#define __BOT_H

#define CbwSignature 0x43425355
#define CswSignature 0x53425355

typedef enum _BotState_t
{
  BotWaitCbw = 0, BotEpDataOut, BotEpDataIn, BotSendCsw, BotFatalError,
} BotState_t;

typedef enum _MassStorageClassRequest_t
{
  UsbBotInterfaceGetMaxLun = 0xFE,
  MassStorageClassReqReset ,
} MassStorageClassRequest_t;

typedef enum _BotToUserStatus_t
{
  BotPass = 0, BotError, BotNotReady, BotNotAlignment
} BotToUserStatus_t;

typedef enum _CSWStatus_t
{
  CswPassed = 0, CswFailed, CswPhaseError
} CSWStatus_t;

typedef enum _BotDataDir_t
{
  BotDataFormHostToDev = 0,BotDataFormDevToHost,
} BotDataDir_t;

typedef union _CbwFlags_t
{
  uint8_t Data;
  struct
  {
    uint8_t     : 7;
    uint8_t Dir : 1;
  };
} CbwFlags_t, *pCbwFlags_t;

#pragma pack(1)

typedef union _Cbw_t
{
  uint8_t Data[31];
  struct
  {
    uint32_t      dCBWSignature;
    uint32_t      dCBWTag;
    uint32_t      dCBWDataTransferLength;
    CbwFlags_t  bmCBWFlags;
    uint8_t       bCBWLUN;
    uint8_t       bCBWCBLength;
    uint8_t       CBWCB[16];
  };
} Cbw_t, * pCbw_t;

typedef union _Csw_t
{
  uint8_t Data[13];
  struct
  {
    uint32_t dCSWSignature;
    uint32_t dCSWTag;
    uint32_t dCSWDataResidue;
    uint8_t  bCSWStatus;
  };
} Csw_t, * pCsw_t;

typedef union _BotStatus_t
{
  uint8_t Flags;
  struct
  {
    uint8_t PhaseError  : 1;
    uint8_t CmdFault    : 1;
    uint8_t BotStatus   : 1;
    uint8_t DataComplete: 1;
    uint8_t ShortPacket : 1;
  };
} BotStatus_t, * pBotStatus_t;

#define bPhaseErrorMask_t       0x01
#define bCmdFaultMask_t         0x02
#define bBotStatusMask_t        0x04
#define bBotDataCompleteMask_t  0x10

#pragma pack()

#endif //__BOT_H
