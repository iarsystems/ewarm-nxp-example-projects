/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sd_dsk_desc.h
 *    Description : Define MMC/SD DISK descriptors
 *
 *    History :
 *    1. Date        : May 15, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "arm_comm.h"
#include "scsi_desc.h"

#ifndef __DSK_DESC_H
#define __DSK_DESC_H

#define SizeOfInquiryDescMmcDsk     36

extern const Int8U SdDskInquiry[];

#endif // __MMC_DSK_DESC_H
