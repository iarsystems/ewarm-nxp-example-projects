/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : scsi_ll.c
 *    Description : USB Mass SCSI low level include file
 *
 *    History :
 *    1. Date        : August 23, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4271 $
 **************************************************************************/
#include "bot.h"
#include "scsi_cmd.h"

#ifndef __SCSI_LL_H
#define __SCSI_LL_H

#define SCSI_LUN_NUMB  1

extern __no_init Cbw_t Cbw;
extern __no_init Csw_t Csw;
extern __no_init Spc3RequestSenseResponse_t  Req2Sense;

#endif // __SCSI_LL_H
