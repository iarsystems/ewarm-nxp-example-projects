/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : scsi_cmd.h
 *    Description : Definition of SCSI commands (SPC, SBP, MMC command set)
 *
 *    History :
 *    1. Date        : September 27, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : January 27, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Add Unit Start/Stop command definitions
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __SCSI_CMD_H
#define __SCSI_CMD_H

// Data Command block definition
typedef enum _ScsiCommands_t
{
  ScsiTestUnitReady     = 0x00,
  ScsiRequestSense      = 0x03,
  ScsiInquiry           = 0x12,
  ScsiModeSelect6       = 0x15,
  ScsiModeSense6        = 0x1A,
  ScsiMediaStartStop    = 0x1B,
  ScsiMediaRemoval      = 0x1E,
  ScsiReadFormatCapcity = 0x23,
  ScsiReadCapacity10    = 0x25,
  ScsiRead10            = 0x28,
  ScsiWrite10           = 0x2A,
  ScsiFerify10          = 0x2F,
} ScsiCommands_t;

typedef enum _SpcSkd_t
{
  NoSense = 0, RecoveredError, NotReady, MediumError, HardwareError,
  IllegalRequest, UnitAttention, DataProtect, BlankCheck, VendorSpecific,
  CopyAborted, AbortedCommand, VolumeOverflow = 0xD, Miscompare,
} SpcSkd_t;

typedef enum _SpcAscAscq_t
{
  NoAdditionalSenseInformation                    = 0x0000UL,
  LogicalUnitNotReadyCauseNotReportable           = 0x0004UL,
  InvalidFieldInCdb                               = 0x0024UL,
  InvalidCommandOperationCode                     = 0x0020UL,
  NotReadyToReadyChangeMediumMayHaveChanged       = 0x0028UL,
  MediumNotPresent                                = 0x003AUL,
  MiscompareDuringVerifyOperation                 = 0x001DUL
} SpcAscAscq_t;

typedef enum _ScsiStatusCode_t
{
  ScsiCommandNoKey = 0, ScsiMediamNotReady, ScsiInvalidCbd,
  ScsiUnknowCommand, ScsiMediaNotPresent, ScsiMediaChanged,
  ScsiMiscompare, ScsiFatalError,
} ScsiStatusCode_t;

typedef enum _StartStop_t
{
  UnitEjected = 0, UnitStarted,
} StartStop_t;

typedef enum _FormatCapacityDescType_t
{
  UnformattedMedia = 1, FormattedMedia, NoMediaPresent,
} FormatCapacityDescType_t;

typedef union _ScsiCmdField_t
{
  uint8_t Cmd;
  struct
  {
    uint8_t CommandCode : 5;
    uint8_t GroupCode   : 3;
  };
} ScsiCmdField_t, * pScsiCmdField_t;

typedef union _SpcControl_t
{
  uint8_t Control;
  struct
  {
    uint8_t Link        : 1;
    uint8_t             : 1;
    uint8_t NACA        : 1;
    uint8_t             : 3;
    uint8_t VendorSpec  : 2;
  };
} SpcControl_t, *pSpcControl_t;

#pragma pack(1)
// Commands definitions

typedef struct _Spc3TestUnitReady_t
{
  uint8_t  OperationCode;
  uint8_t  Reserved0;
  uint8_t  Reserved1;
  uint8_t  Reserved2;
  uint8_t  Reserved3;
  SpcControl_t  Control;
} Spc3TestUnitReady_t, *pSpc3TestUnitReady_t;

typedef struct _Spc3RequestSense_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t DESC : 1;
    uint8_t      : 7;
  };
  uint8_t Reserved0;
  uint8_t Reserved1;
  uint8_t AllocationLenght;
  SpcControl_t Control;
} Spc3RequestSense_t, * pSpc3RequestSense_t;

typedef struct _Spc3Inquiry_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t EVPD : 1;
    uint8_t      : 7;
  };
  uint8_t PageCode;
  uint8_t AllocationLenght[2];
  SpcControl_t Control;
} Spc3Inquiry_t, * pSpc3Inquiry_t;

typedef struct _Spc3ModeSelect6_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t SP   : 1;
    uint8_t      : 3;
    uint8_t PF   : 1;
    uint8_t      : 3;
  };
  uint8_t Reserved0;
  uint8_t Reserved1;
  uint8_t Reserved2;
  uint8_t ParameterListLenght;
  SpcControl_t Control;
} Spc3ModeSelect6_t, * pSpc3ModeSelect6_t;

typedef struct _Spc3ModeSense6_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t      : 3;
    uint8_t DBD  : 1;
    uint8_t      : 4;
  };
  struct
  {
    uint8_t PageCode : 6;
    uint8_t PC       : 2;
  };
  uint8_t SubPageCode;
  uint8_t AllocationLenght;
  SpcControl_t Control;
} Spc3ModeSense6_t, * pSpc3ModeSense6_t;

typedef struct _Spc3MediaRemoval_t
{
  uint8_t OperationCode;
  uint8_t Reserved0;
  uint8_t Reserved1;
  uint8_t Reserved2;
  struct
  {
    uint8_t Prevent : 2;
    uint8_t         : 6;
  };
  SpcControl_t Control;
} Spc3MediaRemoval_t, * pSpc3MediaRemoval_t;

typedef struct _Sbc2ReadCapacity10_t
{
  uint8_t OperationCode;
  uint8_t Reserved0;
  uint8_t LogicalBockAddress[4];
  uint8_t Reserved1;
  uint8_t Reserved2;
  struct
  {
    uint8_t PMI     : 1;
    uint8_t         : 7;
  };
  SpcControl_t Control;
} Sbc2ReadCapacity10_t, * pSbc2ReadCapacity10_t;

typedef struct _Sbc2Read10_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t Obsolete  : 1;
    uint8_t FUA_NV    : 1;
    uint8_t Reserved  : 1;
    uint8_t FUA       : 1;
    uint8_t DPO       : 1;
    uint8_t RDPROTECT : 3;
  };
  uint8_t LogicalBockAddress[4];
  struct
  {
    uint8_t CroupNumb : 5;
    uint8_t           : 3;
  };
  uint8_t TransferLength[2];
  SpcControl_t Control;
} Sbc2Read10_t, * pSbc2Read10_t;

typedef struct _Sbc2Write10_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t Obsolete  : 1;
    uint8_t FUA_NV    : 1;
    uint8_t Reserved  : 1;
    uint8_t FUA       : 1;
    uint8_t DPO       : 1;
    uint8_t WRPROTECT : 3;
  };
  uint8_t LogicalBockAddress[4];
  struct
  {
    uint8_t CroupNumb : 5;
    uint8_t           : 3;
  };
  uint8_t TransferLength[2];
  SpcControl_t Control;
} Sbc2Write10_t, * pSbc2Write10_t;

typedef struct _Sbc2Verify10_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t Obsolete  : 1;
    uint8_t BYTCHK    : 1;
    uint8_t Reserved  : 2;
    uint8_t DPO       : 1;
    uint8_t VRPROTECT : 3;
  };
  uint8_t LogicalBockAddress[4];
  struct
  {
    uint8_t CroupNumb : 5;
    uint8_t           : 3;
  };
  uint8_t TransferLength[2];
  SpcControl_t Control;
} Sbc2Verify10_t, * pSbc2Verify10_t;

typedef struct _Mmc3ReadFormatCapacity_t
{
  uint8_t OperationCode;
  uint8_t Reserved0;
  uint8_t Reserved1;
  uint8_t Reserved2;
  uint8_t Reserved3;
  uint8_t Reserved4;
  uint8_t Reserved5;
  uint8_t AllocationLenght[2];
  SpcControl_t Control;
} Mmc3ReadFormatCapacity_t, * pMmc3ReadFormatCapacity_t;

// Command responses definitions
typedef struct _Spc3RequestSenseResponse_t
{
  struct
  {
    uint8_t ResponceCode : 7;
    uint8_t Valid        : 1;
  };
  uint8_t Obsolete;
  struct
  {
    uint8_t SenseKey : 4;
    uint8_t Reserved : 1;
    uint8_t ILI      : 1;
    uint8_t EOM      : 1;
    uint8_t FILEMARK : 1;
  };
  uint8_t Information[4];
  uint8_t AddSenseKeyLength;
  uint8_t CommSpecInfo[4];
  uint8_t Asc;
  uint8_t Ascq;
  uint8_t Fruc;
  uint8_t Sks[3];
} Spc3RequestSenseResponse_t, *pSpc3RequestSenseResponse_t;

typedef struct _Spc3ModeSense6Response_t
{
  uint8_t ModeDataLength;
  uint8_t MediumType;
  struct
  {
    uint8_t         : 4;
    uint8_t DPOFUA  : 1;
    uint8_t         : 2;
    uint8_t WP      : 1;
  };
  uint8_t BlockDeskLength;
} Spc3ModeSense6Response_t, * pSpc3ModeSense6Response_t;

typedef struct _Sbc2ReadCapacity10Response_t
{
  uint8_t LogicalBlockAddress[4];
  uint8_t BlockLength[4];
} Sbc2ReadCapacity10Response_t, * pSbc2ReadCapacity10Response_t;

typedef struct _Mmc3FormatCapDescriptor_t
{
  uint8_t NumberofBlocks[4];
  struct
  {
    uint8_t DescriptorType : 2;
    uint8_t FormatType     : 6;
  };
  uint8_t BlockLength[3];
} Mmc3FormatCapDescriptor_t, *pMmc3FormatCapDescriptor_t;

typedef struct _Mmc3FormatCapResponse_t
{
  uint8_t Reserved0;
  uint8_t Reserved1;
  uint8_t Reserved2;
  uint8_t CapacityListLength;
  Mmc3FormatCapDescriptor_t MaximumDescriptor;
} Mmc3FormatCapResponse_t, * pMmc3FormatCapResponse_t;

typedef struct _Spc3UnitStartStop_t
{
  uint8_t OperationCode;
  struct
  {
    uint8_t IMMED : 1;
    uint8_t       : 7;
  };
  uint8_t Reserved0;
  uint8_t Reserved1;
  struct
  {
    uint8_t START : 1;
    uint8_t LOEJ  : 1;
    uint8_t       : 2;
    uint8_t PC    : 4;
  };
  SpcControl_t Control;
} Spc3UnitStartStop_t, *pSpc3UnitStartStop_t;
#pragma pack()

#endif // __SCSI_CMD_H
