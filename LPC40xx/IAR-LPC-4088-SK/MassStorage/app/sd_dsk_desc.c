/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sd_dsk_desc.c
 *    Description : MMC/SD descriptor
 *
 *    History :
 *    1. Date        : August 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "disk.h"
#include "lun.h"
#include "scsi_desc.h"
#include "sd_card_mode.h"

#pragma data_alignment=4
const uint8_t SdDskInquiry[] =
{
// 0 PERIPHERAL QUALIFIER PERIPHERAL DEVICE TYPE
  SbcDirectAccess,
//1 RMB
  SpcInquiryRemovableMedium,
// 2 VERSION
  SpcInquiryStandartVersion,
// 3 AERC Obsolete NORMACA HISUP RESPONSE DATA FORMAT
  0x02,
// 4 ADDITIONAL LENGTH (n-4)
  36-4,
// 5 SCCS
  0x00,
// for parallel SCSI only
// 6 BQUE ENCSERV VS MULTIP MCHNGR Obsolete Obsolete ADDR16†
  0x00,
// 7 RELADR Obsolete WBUS16† SYNC† LINKED Obsolete CMDQUE VS
  0x00,
// 8 - 15 VENDOR IDENTIFICATION
  'I','A','R',' ','S','y','s','.',
// 16 - 31 PRODUCT IDENTIFICATION
  'L','P','C','4','0','8','8',' ','S','t','o','r','a','g','e',' ',
// 32 - 35 PRODUCT REVISION LEVEL
  '1','.','0','0',
};

const LunFpnt_t Sd_LunFunDesc =
{
  .DiskInitFpnt   = SdDiskInit,
  .DiskStatusFpnt = SdGetDiskCtrlBkl,
  .DiskIoFpnt     = SdDiskIO,
  .DiskInquiry    = (uint8_t *)SdDskInquiry,
  .DiskInquirySize = 36
};
