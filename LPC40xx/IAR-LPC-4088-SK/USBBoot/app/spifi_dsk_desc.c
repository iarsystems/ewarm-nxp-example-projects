/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : spifi_dsk_desc.c
 *    Description : SPIFI disk descriptor
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 28 $
**************************************************************************/
#include "disk.h"
#include "lun.h"
#include "scsi_desc.h"
#include "spifi_disk.h"

#pragma data_alignment=4
const uint8_t SpifiDskInquiry[] =
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
  'L','P','C','4','0','8','8','-','S','K',' ','S','P','I','F','I',
// 32 - 35 PRODUCT REVISION LEVEL
  '1','.','0','0',
};

const LunFpnt_t Spifi_LunFunDesc =
{
  .DiskInitFpnt   = SpifiDiskInit,
  .DiskStatusFpnt = SpifiGetDiskCtrlBkl,
  .DiskIoFpnt     = SpifiDiskIO,
  .DiskInquiry    = (uint8_t *)SpifiDskInquiry,
  .DiskInquirySize = 36
};
