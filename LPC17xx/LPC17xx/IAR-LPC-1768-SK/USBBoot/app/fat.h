/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : fat.h
 *    Description : fat12/16 definitions
 *
 *    History :
 *    1. Date        : November, 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *    $Revision: 4271 $
 **************************************************************************/
#include <stdint.h>

#ifndef __FAT_H
#define __FAT_H

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN 	  0x02
#define ATTR_SYSTEM 	  0x04
#define ATTR_VOLUME_ID 	0x08
#define ATTR_DIRECTORY	0x10
#define ATTR_ARCHIVE  	0x20
#define ATTR_LONG_NAME 	ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID

#pragma pack(1)

typedef struct boot_s
{
  uint8_t   BS_jmpBoot[3];
  char      BS_OEMName[8];
  uint16_t  BPB_BytsPerSec;
  uint8_t   BPB_SecPerClus;
  uint16_t  BPB_RsvdSecCnt;
  uint8_t   BPB_NumFATs;
  uint16_t  BPB_RootEntCnt;
  uint16_t  BPB_TotSec16;
  uint8_t   BPB_Media;
  uint16_t  BPB_FATSz16;
  uint16_t  BPB_SecPerTrk;
  uint16_t  BPB_NumHeads;
  uint32_t  BPB_HiddSec;
  uint32_t  BPB_TotSec32;
  uint8_t   BS_DrvNum;
  uint8_t   BS_Reserved1;
  uint8_t   BS_BootSig;
  uint32_t  BS_VolID;
  char      BS_VolLab[11];
  char      BS_FilSysType[8];
  uint8_t   BootStrap[448];
  uint16_t  BS_signature;
} boot_t, * pboot_t;


typedef struct dir_entry_s
{
  char      DIR_Name[11];
  uint8_t   DIR_Attr;
  uint8_t   DIR_NTRes;
  uint8_t   DIR_CrtTimeTenth;
  uint16_t  DIR_CrtTime;
  uint16_t  DIR_CrtDate;
  uint16_t  DIR_LstAccDate;
  uint16_t  DIR_FstClusHI;
  uint16_t  DIR_WrtTime;
  uint16_t  DIR_WrtDate;
  uint16_t  DIR_FstClusLO;
  uint32_t  DIR_FileSize;
} dir_etnry_t, * pdir_entry_t;

#pragma pack()

#endif /*__FAT_H*/
