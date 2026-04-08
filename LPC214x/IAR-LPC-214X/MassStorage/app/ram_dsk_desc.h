/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ram_dsk_desc.h
 *    Description : Define RAM DISK descriptors
 *
 *    History :
 *    1. Date        : November 23, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "arm_comm.h"
#include "scsi_desc.h"
#include "ram_dsk.h"

#ifndef __RAM_DSK_DESC_H
#define __RAM_DSK_DESC_H

#ifdef  RAM_DISK_DESC_GLOBAL
#define RAM_DISK_DESC_EXTERN
#else
#define RAM_DISK_DESC_EXTERN extern
#endif

#define SizeOfInquiryDescRamDsk     36
#define SizeOfFormatCapacityRamDsk  12

RAM_DISK_DESC_EXTERN const Int8U RamDskInquiry[];
RAM_DISK_DESC_EXTERN const Int8U RamDskFormatCapacity[];
#endif // __RAM_DSK_DESC_H
