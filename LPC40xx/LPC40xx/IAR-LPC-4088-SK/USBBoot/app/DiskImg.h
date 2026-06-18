/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : DiskImg.h
 *    Description : Disk Image header file
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Create
 *    $Revision: 4280 $
 **************************************************************************/
#ifndef __DISKIMG_H
#define __DISKIMG_H

#define NONPROT 0xFFFFFFFF
#define CRP1  0x12345678
#define CRP2  0x87654321
/*If CRP3 is selected, no future factory testing can be
performed on the device*/
#define CRP3  0x43218765
//#define CRP   NONPROT


#define FLASH_MEMORY_SIZE  0x80000
#define USB_BOOT_START     0x0000
#define USB_BOOT_SIZE      0x6000
#define USER_PROGRAM_START (USB_BOOT_START + USB_BOOT_SIZE)
#define USER_PROGRAM_SIZE  (FLASH_MEMORY_SIZE - USB_BOOT_SIZE)

#define SECTOR_SIZE      512
#define BOOT_SECT_SIZE 	 SECTOR_SIZE
#define RSVD_SECTORS     ((BOOT_SECT_SIZE+SECTOR_SIZE-1)/SECTOR_SIZE)
#define ROOT_ENR_CNT     16
#define FAT_DATA_SIZE	   (SECTOR_SIZE * 2)
#define FAT_DIR_SIZE     ROOT_ENR_CNT*32  /*This should be multiple
                                            of SECTOR_SIZE*/
#define FAT_SIZE          (RSVD_SECTORS*SECTOR_SIZE+FAT_DATA_SIZE+FAT_DIR_SIZE)
#define TOTAL_MEMORY_SIZE (FLASH_MEMORY_SIZE+FAT_SIZE)
#define TOTAL_SECT        TOTAL_MEMORY_SIZE/SECTOR_SIZE
#define SECT_PER_CLUSTER 2

#define TOTAL_CLUSTERS  TOTAL_SECT/SECT_PER_CLUSTER

extern const boot_t BootSect;
extern const dir_etnry_t  DirEntry[ROOT_ENR_CNT];

#endif /*__DISKIMG_H*/
