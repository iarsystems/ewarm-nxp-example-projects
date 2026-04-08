/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : spifi_disk.h
 *    Description : spifi_disk driver
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __SPIFI_DISK_H
#define __SPIFI_DISK_H

#define SPIFI_DISK_LUN       1
#define SPIFI_DEF_BLK_SIZE   512

/*************************************************************************
 * Function Name: SpifiDiskInit
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Init SPIFI Disk
 *
 *************************************************************************/
void SpifiDiskInit (void);

/*************************************************************************
 * Function Name: SpifiGetDiskCtrlBkl
 * Parameters:  none
 *
 * Return: pDiskCtrlBlk_t
 *
 * Description: Return pointer to status structure of the disk
 *
 *************************************************************************/
pDiskCtrlBlk_t SpifiGetDiskCtrlBkl (void);

/*************************************************************************
 * Function Name: SpifiDiskIO
 * Parameters: uint8_t * pData,uint32_t BlockStart,
 *             uint32_t BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: Spifi Disk I/O
 *
 *************************************************************************/
DiskStatusCode_t SpifiDiskIO (uint8_t * pData,uint32_t BlockStart,
                              uint32_t BlockNum, DiskIoRequest_t IoRequest);


#endif //__SPIFI_DISK_H
