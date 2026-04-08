/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_MMA7660FC.h
 *    Description : MMA7660FC driver include file
 *
 *    History :
 *    1. Date        : 3 February, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

#ifndef __DRV_MMA7660FC_H
#define __DRV_MMA7660FC_H

#define MMA7660FC_SLAVE_ADDR  0x98

typedef enum _MMA7660FC_Reg_t
{
  MMA7660FC_XOUT = 0, MMA7660FC_YOUT, MMA7660FC_ZOUT,MMA7660FC_TILT, MMA7660FC_SRST,
  MMA7660FC_SPCNT, MMA7660FC_INTSU, MMA7660FC_MODE, MMA7660FC_SR, MMA7660FC_PDET,
  MMA7660FC_PD
} MMA7660FC_Reg_t;

typedef struct _Acc_Data_t
{
  Int8S X;
  Int8S Y;
  Int8S Z;
} Acc_Data_t, * pAcc_Data_t;

Boolean MMA7660FC_Init (void);
Boolean MMA7660FC_GetAccData (pAcc_Data_t pData);

#endif // __DRV_MMA7660FC_H
