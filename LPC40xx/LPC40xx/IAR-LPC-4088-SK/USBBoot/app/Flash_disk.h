/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : disk.h
 *    Description : disk driver header file
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __FLASH_DISK_H
#define __FLASH_DISK_H

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
 * Parameters: uint8_t * pData,uint32_t BlockStart,
 *             uint32_t BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description: MMC/SD disk I/O
 *
 *************************************************************************/
DiskStatusCode_t DiskIO (uint8_t * pData,uint32_t BlockStart,
                              uint32_t BlockNum, DiskIoRequest_t IoRequest);


#endif //__FLASH_DISK_H
