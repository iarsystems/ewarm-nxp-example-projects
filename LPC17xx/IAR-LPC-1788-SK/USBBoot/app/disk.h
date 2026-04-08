/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : disk.h
 *    Description : Disk common definitions module
 *
 *    History :
 *    1. Date        : November 15, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __DISK_H
#define __DISK_H

typedef enum _DiskStatusCode_t
{
  DiskCommandPass = 0, DiskNotReady, DiskNotPresent,
  DiskParametersError, DiskMiscompareError, DiskChanged,
  DiskUknowError,
} DiskStatusCode_t;

typedef enum _DiskIoRequest_t
{
  DiskWrite = 0, DiskRead, DiskVerify,
} DiskIoRequest_t;

typedef enum _DiskInfoType_t
{
  DiskInquiry = 0, DiskFormatCapacity,
} DiskInfoType_t;

typedef enum _DiskType_t
{
  DiskMMC, DiskSD_Spec1_x, DiskSD_Spec2_0, DiskUnknow,
} DiskType_t;

typedef struct _DiskCtrlBlk_t
{
  Int32U  BlockNumb;
  Int32U  BlockSize;
  DiskStatusCode_t DiskStatus;
  DiskType_t DiskType;
  Boolean WriteProtect;
  Boolean MediaChanged;
} DiskCtrlBlk_t, *pDiskCtrlBlk_t;

typedef void            (* DiskInitFpnt_t)(void);
typedef Int32U          (* DiskInfoFpnt_t)(pInt8U,DiskInfoType_t);
typedef pDiskCtrlBlk_t  (* DiskStatusFpnt_t)(void);
typedef DiskStatusCode_t(* DiskIoFpnt_t)(pInt8U,Int32U,Int32U,DiskIoRequest_t);

#define DISK_LUN       0
#define DEF_BLK_SIZE   512

/*************************************************************************
 * Function Name: SdDiskInit
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Init MMC/SD disk
 *
 *************************************************************************/
void DiskInit (void);

/*************************************************************************
 * Function Name: GetDiskCtrlBkl
 * Parameters:  none
 *
 * Return: pDiskCtrlBlk_t
 *
 * Description: Return pointer to status structure of the disk
 *
 *************************************************************************/
pDiskCtrlBlk_t GetDiskCtrlBkl (void);

/*************************************************************************
 * Function Name: DiskIO
 * Parameters: pInt8U pData,Int32U BlockStart,
 *             Int32U BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: MMC/SD disk I/O
 *
 *************************************************************************/
DiskStatusCode_t DiskIO (pInt8U pData,Int32U BlockStart,
                              Int32U BlockNum, DiskIoRequest_t IoRequest);

#endif //__LUN_H
