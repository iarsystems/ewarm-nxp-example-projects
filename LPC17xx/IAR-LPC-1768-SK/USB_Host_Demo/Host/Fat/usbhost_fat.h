/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_fat.h
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/

#ifndef  USBHOST_FAT_H
#define  USBHOST_FAT_H

/*
**************************************************************************************************************
*                                       INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include  "usbhost_inc.h"

/*
**************************************************************************************************************
*                                      FAT DEFINITIONS
**************************************************************************************************************
*/

#define    FAT_TIME_DATE_ENA        1

#define    FAT_32                   0
#define    FAT_12                   2
#define    FAT_16                   1
#define    FAT_INVALID             -1

#define    LAST_ENTRY               1
#define    FREE_ENTRY               2
#define    LFN_ENTRY                3
#define    SFN_ENTRY                4

#define    RDONLY                   1
#define    RDWR                     2

#define    FAT32_SUP_VER            0

#define    FILE_ATTR_READ_ONLY     (1<<0)
#define    FILE_ATTR_HIDDEN        (1<<1)
#define    FILE_ATTR_SYSTEM        (1<<2)
#define    FILE_ATTR_VOL_ID        (1<<3)
#define    FILE_ATTR_DIR           (1<<4)
#define    FILE_ATTR_ARCH          (1<<5)

typedef  struct  file_entry {
    USB_INT32U  FileSize;                    /* Total size of the file                                      */
    USB_INT32U  CurrClus;                    /* Current cluster of the cluster offset                       */
    USB_INT32U  CurrClusOffset;              /* Current cluster offset                                      */
    USB_INT32U  EntrySec;                    /* Sector where the file entry is located                      */
    USB_INT32U  EntrySecOffset;              /* Offset in the entry sector from where the file is located   */
    USB_INT08U  Status;                      /* File's open status                                          */
} FILE_ENTRY;


#define FAT_PREPARE_DATE_TIME()              // used for data and time integrity
#define FAT_GET_DAY()               04
#define FAT_GET_MONTH()             12
#define FAT_GET_YEAR()              2008

#define FAT_GET_10MSEC()            99
#define FAT_GET_SEC()               33
#define FAT_GET_MINUTE()            57
#define FAT_GET_HOUR()              15


/*
**************************************************************************************************************
*                                       FUNCTION PROTOTYPES
**************************************************************************************************************
*/

USB_INT32S  FAT_Init            (void);

USB_INT32U  FILE_Open           (         USB_INT08U *file_name,
                                          USB_INT08U  flags);
USB_INT32U  FILE_Read           (         USB_INT32U  fd,
                                 volatile USB_INT08U *buffer,
                                          USB_INT32U  num_bytes);
USB_INT32U  FILE_Write          (         USB_INT32U  fd,
                                 volatile USB_INT08U *buffer,
                                          USB_INT32U  num_bytes);
void        FILE_Close          (         USB_INT32U  fd);
USB_INT32S  FILE_GetAttr        (         USB_INT08U *file_name,
                                          USB_INT08U *attr);
USB_INT32S  FILE_SetAttr        (         USB_INT08U *file_name,
                                          USB_INT08U  attr);
USB_INT32S  FILE_Delete         (         USB_INT08U *file_name);

#endif
