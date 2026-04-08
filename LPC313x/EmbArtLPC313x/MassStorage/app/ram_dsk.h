/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ram_dsk.h
 *    Description : Define Ram Disk module
 *
 *    History :
 *    1. Date        : November 15, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "includes.h"

#ifndef __RAM_DISK_H
#define __RAM_DISK_H

#define RAM_DISK_LUN        0

#define RAM_DSK_BLOCK_SIZE  512
#define RAM_DSK_BLOCK_NUMB  65535

/*************************************************************************
 * Function Name: RamDiskInit
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Init RAM disk
 *
 *************************************************************************/
void RamDiskInit (void);

/*************************************************************************
 * Function Name: RamDiskInfo
 * Parameters:  pInt8U pData, DiskInfoType_t DiskInfoType
 *
 * Return: Int32U
 *
 * Description: Return pointer to Info stucture of the disk
 * (Inquiry or Format capacity)
 *
 *************************************************************************/
Int32U RamDiskInfo (pInt8U pData, DiskInfoType_t DiskInfoType);

/*************************************************************************
 * Function Name: RamDiskStatus
 * Parameters:  void
 *
 * Return: pDiskCtrlBlk_t
 *
 * Description: Return pointer to status stucture of the disk
 *
 *************************************************************************/
pDiskCtrlBlk_t RamDiskStatus (void);

/*************************************************************************
 * Function Name: RamDiskIO
 * Parameters: pInt8U pData,Int32U BlockStart,
 *             Int32U BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: Ram disk I/O
 *
 *************************************************************************/
DiskStatusCode_t RamDiskIO (pInt8U pData,Int32U BlockStart,
                              Int32U BlockNum, DiskIoRequest_t IoRequest);


#endif // __RAM_DISK_H
