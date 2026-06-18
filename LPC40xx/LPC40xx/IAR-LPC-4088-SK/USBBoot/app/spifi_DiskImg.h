/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : Spifi_DiskImg.h
 *    Description : Spifi Disk Image header file
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Create
 *    $Revision: 4271 $
 **************************************************************************/
#ifndef __SPIFI_DISKIMG_H
#define __SPIFI_DISKIMG_H

#define SPIFI_MEMORY_SIZE       0x200000

#define SPIFI_SECTOR_SIZE      512
#define SPIFI_BOOT_SECT_SIZE 	 SPIFI_SECTOR_SIZE
#define SPIFI_RSVD_SECTORS     ((SPIFI_BOOT_SECT_SIZE+SPIFI_SECTOR_SIZE-1)/SPIFI_SECTOR_SIZE)
#define SPIFI_ROOT_ENR_CNT     16
#define SPIFI_FAT_DATA_SIZE	   (SPIFI_SECTOR_SIZE * 2)
#define SPIFI_FAT_DIR_SIZE     SPIFI_ROOT_ENR_CNT*32  /*This should be multiple
                                                             of SECTOR_SIZE*/
#define SPIFI_FAT_SIZE          (SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE+SPIFI_FAT_DATA_SIZE+SPIFI_FAT_DIR_SIZE)
#define SPIFI_TOTAL_MEMORY_SIZE (SPIFI_MEMORY_SIZE+SPIFI_FAT_SIZE)
#define SPIFI_TOTAL_SECT        SPIFI_TOTAL_MEMORY_SIZE/SPIFI_SECTOR_SIZE
#define SPIFI_SECT_PER_CLUSTER  8

#define SPIFI_TOTAL_CLUSTERS  SPIFI_TOTAL_SECT/SPIFI_SECT_PER_CLUSTER

extern const boot_t Spifi_BootSect;
extern const dir_etnry_t  Spifi_DirEntry[SPIFI_ROOT_ENR_CNT];

#endif /*__SPIFI_DISKIMG_H*/
