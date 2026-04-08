/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ram_dsk_desc.c
 *    Description : RAM DISK descriptors
 *
 *    History :
 *    1. Date        : November 23, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#define  RAM_DISK_DESC_GLOBAL
#include "ram_dsk_desc.h"

#pragma data_alignment=4
const Int8U RamDskInquiry[] =
{
// 0 PERIPHERAL QUALIFIER PERIPHERAL DEVICE TYPE
  SbcDirectAccess,
//1 RMB
  SpcInquiryNotRemovableMedium,
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
  'L','P','C','3','1','5','x',' ','S','t','o','r','a','g','e',' ',
// 32 - 35 PRODUCT REVISION LEVEL
  '1','.','0','0',
};
