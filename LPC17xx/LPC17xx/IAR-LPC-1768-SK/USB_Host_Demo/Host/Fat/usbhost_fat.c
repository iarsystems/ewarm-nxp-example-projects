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
* File            : usbhost_fat.c
* Programmer(s)   : Ravikanth.P
* Version         :
* Date            : 27, November 2008
* Author          : Stanimir Bonev
* Description     : Modify
*   FAT32 support added
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                           INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include  "usbhost_fat.h"
#include  "io_cache.h"

/*
**************************************************************************************************************
*                                      LOCAL TYPE DEFINITIONS
**************************************************************************************************************
*/

#define FAT_DATE_START_YEAR   1980
#define CREATE_DATA_TIME      (1UL << 0)
#define WRITE_DATA_TIME       (1UL << 1)
#define ACCESS_DATA           (1UL << 2)

#define FILE_DELETE_MARK      0xE5

typedef union fat_date
{
  USB_INT16U  Value;
  struct
  {
    USB_INT16U  Day   : 5;
    USB_INT16U  Month : 4;
    USB_INT16U  Year  : 7;
  };
} FAT_DATE;

typedef union fat_time
{
  USB_INT16U  Value;
  struct
  {
    USB_INT16U  Seconds : 5;
    USB_INT16U  Minutes : 6;
    USB_INT16U  Hours   : 5;
  };
} FAT_TIME;

#pragma pack(1)
typedef union fat_date_time
{
  USB_INT32U Data;
  struct
  {
    FAT_TIME Time;
    FAT_DATE Date;
  };
} FAT_DATE_TIME;
#pragma pack()

typedef union fat32_extfl
{
  USB_INT16U Flags;
  struct
  {
    USB_INT16U ActFAT : 4;
    USB_INT16U        : 3;
    USB_INT16U Valid  : 1;
    USB_INT16U        : 8;
  };
} FAT32_EXTFL;

typedef union fat32_fsver
{
  USB_INT16U Version;
  struct
  {
    USB_INT08U Minor;
    USB_INT08U Major;
  };
} FAT32_FSVER;

typedef struct fat_desc
{
  USB_INT08S FATType;
  USB_INT16U BytsPerSec;
  USB_INT32U BytsPerClus;
  USB_INT08U SecPerClus;
  USB_INT32U FATStartSec;
  USB_INT32U FATMultShift;
  USB_INT08U FATsNum;
  USB_INT32U FATSz;
  USB_INT32U RootDirStartSec;
  USB_INT32U RootDirSec;
  USB_INT32U RootClus;
  USB_INT32U FirstDataSec;
  USB_INT32U BootSecStart;
  USB_INT32U FSInfoSecStart;
  USB_INT32U FSLastClus;
  USB_INT32U FSNextFreeClus;
} FAT_DESC;

/*
**************************************************************************************************************
*                                              LOCAL FUNCTION PROTOTYPES
**************************************************************************************************************
*/

static
USB_INT32S  FAT_FindEntry       (         USB_INT08U *ent_name_given,
                                          FILE_ENTRY *entry);
static
void        FAT_GetSFN          (volatile USB_INT08U *entry,
                                          USB_INT08U *name);
static
void        FAT_GetSfnName      (volatile USB_INT08U *entry,
                                          USB_INT08U *name);
static
void        FAT_GetSfnExt       (volatile USB_INT08U *entry,
                                          USB_INT08U *ext_ptr);
static
USB_INT32S  FAT_StrCaseCmp      (         USB_INT08U *str1,
                                          USB_INT08U *str2);
static
USB_INT32U  FAT_ChkEntType      (volatile USB_INT08U *ent);
static
USB_INT32U  FAT_ClusRead        (         USB_INT32U  curr_clus,
                                          USB_INT32U  clus_offset,
                                 volatile USB_INT08U *buffer,
                                          USB_INT32U  num_bytes);
static
USB_INT32U  FAT_GetNextClus     (         USB_INT32U  clus_no);
static
USB_INT32U  FAT_ClusWrite       (         USB_INT32U  curr_clus,
                                          USB_INT32U  clus_offset,
                                 volatile USB_INT08U *buffer,
                                          USB_INT32U  num_bytes);
static
void        FAT_UpdateEntry     (         FILE_ENTRY *entry);
static
void        FAT_UpdateFAT       (         USB_INT32U  curr_clus,
                                          USB_INT32U  value);
static
USB_INT32U  FAT_GetFreeClus     (void);
static
USB_INT32S  FAT_GetFreeEntry    (         FILE_ENTRY *entry);
static
void        FAT_PutSFN          (         USB_INT08U *ent_name_given,
                                          FILE_ENTRY *entry);
static
USB_INT32S  FAT_CreateEntry     (         USB_INT08U *ent_name_given,
                                          FILE_ENTRY *entry);

static
USB_INT32U  FAT_GetEndClus      (         USB_INT32U  clus_no);

static
USB_INT08U *FAT_GetNexRootEntry (         USB_INT08U *buf,
                                          USB_INT32U *p_sec_num);
static
void        FAT_UpdateFSInfo    (         USB_INT32S  ClusNum,
                                          USB_INT32U  NextFreeClus);
#ifdef FAT_TIME_DATE_ENA
static
USB_INT08U  FAT_GetDateTime     (         FAT_DATE   *p_date,
                                          FAT_TIME   *p_time);
static
void        FAT_UpdateDateTime  (         FILE_ENTRY *entry,
                                          USB_INT32U  flags);
#endif
static
void        FAT_FileRelease     (         FILE_ENTRY *entry);
static
USB_INT08S  FAT_File_GetAttr    (         FILE_ENTRY *entry);
static
void        FAT_File_SetAttr    (         FILE_ENTRY *entry,
                                          USB_INT08S  attr);

/*
**************************************************************************************************************
*                                              GLOBAL VARIABLES
**************************************************************************************************************
*/

static  FAT_DESC    FAT_Desc;
#pragma section=".iSRAM"
#pragma location=".iSRAM"
static  FILE_ENTRY  FAT_FileEntry[2];

/*
**************************************************************************************************************
*                                         INITIALIZE THE FILE SYSTEM
*
* Description: This function initializes the FAT16 file system
*
* Arguments  : None
*
* Returns    : OK                     if Success
*              ERR_INVALID_BOOTSIG    if Failed
*
**************************************************************************************************************
*/

USB_INT32S  FAT_Init (void)
{
USB_INT32U  tot_sec;
USB_INT16U  root_ent_cnt;
USB_INT16U  boot_sig;
USB_INT32S  rc;
USB_INT32U  data_sec;
USB_INT32U  tot_data_clus;

FAT32_FSVER fs_ver;
FAT32_EXTFL fs_flags;

  FAT_Desc.FATType = FAT_INVALID;
  IOC_Read(0, 1, (USB_INT08U *)FATBuffer);
  boot_sig = ReadLE16U(&FATBuffer[510]);
  if (boot_sig != 0xAA55)
  {
      rc = ERR_INVALID_BOOT_SIG;
  }
  else
  {
    if (FATBuffer[0] != 0xEB && FATBuffer[0] != 0xE9)
    {
        FAT_Desc.BootSecStart = ReadLE32U(&FATBuffer[454]);
        IOC_Read(FAT_Desc.BootSecStart, 1, (USB_INT08U *)FATBuffer);
    }
    FAT_Desc.BytsPerSec     = ReadLE16U(&FATBuffer[11]);          /* Bytes per cluster              */
    FAT_Desc.SecPerClus     = FATBuffer[13];                      /* Sectors per cluster            */
                                                                  /* FAT1 Start address             */
    FAT_Desc.FATStartSec    = ReadLE16U(&FATBuffer[14]) + FAT_Desc.BootSecStart;
    FAT_Desc.FATsNum        = FATBuffer[16];                      /* Number of FAT copies           */

    tot_sec                 =  ReadLE16U(&FATBuffer[19]);         /* Total FAT16 sectors            */
    if (0 == tot_sec)                                             /* Get total sectors in partition */
    {
      tot_sec               = ReadLE32U(&FATBuffer[32]);          /* Total FAT32 sectors            */
    }

    FAT_Desc.FATSz          = ReadLE16U(&FATBuffer[22]);          /* Size of the FAT table          */
    if (0 == FAT_Desc.FATSz)
    {
      FAT_Desc.FATSz        = ReadLE32U(&FATBuffer[36]);          /* Size of the FAT table          */
    }

    root_ent_cnt            = ReadLE16U(&FATBuffer[17]);          /* Root entry count               */
                                                                  /* Bytes per cluster              */
    FAT_Desc.BytsPerClus    = (FAT_Desc.BytsPerSec * FAT_Desc.SecPerClus);

                                                                  /* determinate FAT type           */
                                             /* Calculate nub res of data sector on current partition */
                                                              /* Sectors occupied by root directory */
    FAT_Desc.RootDirSec     = ((root_ent_cnt * 32) + FAT_Desc.BytsPerSec-1)/FAT_Desc.BytsPerSec;
    FAT_Desc.RootDirStartSec= FAT_Desc.FATStartSec +
                             (FAT_Desc.FATSz * FAT_Desc.FATsNum);
                                                                  /* First data sector              */
    FAT_Desc.FirstDataSec   = FAT_Desc.RootDirStartSec + FAT_Desc.RootDirSec;

    data_sec = tot_sec - (FAT_Desc.FATStartSec +
               (FAT_Desc.FATsNum * FAT_Desc.FATSz) + FAT_Desc.RootDirSec);
    tot_data_clus = data_sec / (FAT_Desc.SecPerClus);  /*       Get total data clusters in the disk */
                                                       /* If total data clusters < 4085 and - FAT12 */
                                                       /*     < 65525, then it is FAT16 file system */
                                                       /*              else it is FAT32 file system */
    FAT_Desc.FSLastClus     = tot_data_clus+1;         /*                Calculate last FAT cluster */
    FAT_Desc.FSNextFreeClus = 0;

    if (tot_data_clus < 4085)
    {
        rc = ERR_FAT_NOT_SUPPORTED;
        PRINT_Err(rc);
    }
    if (tot_data_clus < 65525)
    {
      FAT_Desc.FATType      = FAT_16;
      FAT_Desc.FATMultShift = 1; // *2
      rc = OK;
    }
    else
    {
      fs_ver.Version   = ReadLE16U(&FATBuffer[42]);
      if (fs_ver.Version <= FAT32_SUP_VER)
      {
        FAT_Desc.FATType          =  FAT_32;
        FAT_Desc.FSInfoSecStart   =  ReadLE16U(&FATBuffer[48]) + FAT_Desc.BootSecStart;
        FAT_Desc.RootClus         =  ReadLE32U(&FATBuffer[44]);
        FAT_Desc.RootDirStartSec += (FAT_Desc.RootClus - 2) * FAT_Desc.SecPerClus;
        FAT_Desc.FATMultShift     =  2; // *4
        fs_flags.Flags            =  ReadLE16U(&FATBuffer[40]);
        if (fs_flags.Valid)
        {
          FAT_Desc.FATsNum = 1;
          FAT_Desc.FATStartSec += fs_flags.ActFAT*FAT_Desc.FATSz;
        }
        // Check FSInfo signatures
        IOC_Read(FAT_Desc.FSInfoSecStart, 1, (USB_INT08U *)FATBuffer);
        if(   0x41615252 != ReadLE32U(&FATBuffer[0])
           || 0x61417272 != ReadLE32U(&FATBuffer[484]))
        {
          // Invalid FSInfo record
          FAT_Desc.FSInfoSecStart = 0;
        }
        else
        {
          FAT_Desc.FSNextFreeClus = ReadLE32U(&FATBuffer[492]);
        }

        rc = OK;
      }
      else
      {
        rc = ERR_FAT_NOT_SUPPORTED;
        PRINT_Err(rc);
      }
    }
  }
  return (rc);
}

/*
**************************************************************************************************************
*                                                OPENING A FILE
*
* Description: This function stores the attributes of a file, such as starting cluster, file size,
*              sector where the file entry is stored and offset of the entry in that sector
*
* Arguments  : file_name    Name of the file. The file must be in root directory.
*
* Returns    : pointer to the entry    On Success
*              NULL                    On Failure
*
**************************************************************************************************************
*/

USB_INT32U  FILE_Open (   USB_INT08U  *file_name,
                          USB_INT08U   flags)
{
    USB_INT32U   rc;
    FILE_ENTRY  *entry;


    if (FAT_FileEntry[0].Status == 0) {          /*            If Status = 0 means, we can open a file      */
        entry = &FAT_FileEntry[0];
    } else if (FAT_FileEntry[1].Status == 0) {
        entry = &FAT_FileEntry[1];
    } else {
        return NULL;
    }
    if (flags == RDONLY) {                       /* Search for a file. If it doesn't exist, don't create it */
        rc = FAT_FindEntry(file_name, entry);
        if (rc == MATCH_FOUND) {
        #ifdef FAT_TIME_DATE_ENA
          FAT_UpdateDateTime(entry, ACCESS_DATA);
        #endif
            entry->Status = RDONLY;
            rc = (USB_INT32U)entry;
        }
        else {
          rc = NULL;
        }
    } else {                                     /* Search for a file. If it doesn't exist, create it       */
        rc = FAT_CreateEntry(file_name, entry);
        if(rc >= 0) {
          entry->Status = RDWR;
          rc = (USB_INT32U)entry;
        }
        else {
          rc = NULL;
        }
    }
    return (rc);
}

/*
**************************************************************************************************************
*                                                 FINDING AN ENTRY
*
* Description: This function searches for a file name in the root directory
*
* Arguments  : ent_name_given    Pointer to the file name to be searched.
*              entry             Pointer to the entry structure. The attributes of the file are stored in this
*                                structure if the file was found in the root directory.
*
* Returns    : MATCH_FOUND       if the file was found in the root directory.
*              MATCH_NOT_FOUND   if the file was not found in the root directory.
*
**************************************************************************************************************
*/
static
USB_INT32S  FAT_FindEntry (USB_INT08U  *ent_name_given,
                           FILE_ENTRY  *entry)
{
USB_INT32U   sec_num;
USB_INT08U  *buf = NULL;
USB_INT08U   ent_type;
USB_INT08U   ent_name_read[13];

    while(1)
    {
      if(NULL == (buf =FAT_GetNexRootEntry (buf,&sec_num)))
      {
        break;
      }
      ent_type = FAT_ChkEntType(buf);                  /* Check for the entry type                    */
      if (ent_type == SFN_ENTRY) {                     /* If it is short entry get short file name    */
          FAT_GetSFN(buf, ent_name_read);
                                              /* Compare given name with this name case insensitively */
          if (FAT_StrCaseCmp(ent_name_given, ent_name_read) == MATCH_FOUND) {
            if(FAT_32 == FAT_Desc.FATType)
            {
              entry->CurrClus  =  (Int32U)ReadLE16U(&buf[26]);   /* If they are same, get starting cluster      */
              entry->CurrClus |= ((Int32U)ReadLE16U(&buf[20])) << 16;   /* If they are same, get starting cluster      */
            }
            else
            {
              entry->CurrClus = ReadLE16U(&buf[26]);   /* If they are same, get starting cluster      */
            }
              entry->FileSize = ReadLE32U(&buf[28]);   /* Get file size                               */
              entry->EntrySec = sec_num;          /* Get sector number where the file name is located */
                                          /* Get offset in this sector where the file name is located */
              entry->EntrySecOffset = buf - FATBuffer;
              return (MATCH_FOUND);
          }
      }
      if (ent_type == LAST_ENTRY) {    /* If it is the last entry, no more entries will exist. Return */
          return (MATCH_NOT_FOUND);
      }
    }
    return (MATCH_NOT_FOUND);
}

/*
**************************************************************************************************************
*                                       GET SHORT FILE NAME AND EXTENSION OF A FILE
*
* Description: This function reads the short file name and extension corresponding to a file
*
* Arguments  : ent_buf    buffer which contains the 32 byte entry of a file
*              name       buffer to store the file name and extension of a file
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_GetSFN (volatile  USB_INT08U  *entry,
                            USB_INT08U  *name)
{
    USB_INT08U   ext[4];                          /* Buffer to store the extension of a file                */
    USB_INT08U  *ext_ptr;


    ext_ptr = ext;

    FAT_GetSfnName(entry, name);                  /* Get file name into "name" buffer                       */
    FAT_GetSfnExt(entry, ext_ptr);                /* Get extension into "ext" buffer                        */

    while (*name) {                               /* Go to the end of the file name                           */
        name++;
    }
    if (*ext_ptr) {                               /* If the extension exists, put a '.' character           */
        *name = '.';
        name++;
    }
    while (*ext_ptr) {                            /* Append the extension to the file name                  */
        *name = *ext_ptr;
        name++;
        ext_ptr++;
    }
    *name = '\0';
}

/*
**************************************************************************************************************
*                                          GET SHORT FILE NAME OF A FILE
*
* Description: This function reads the short file name of a file
*
* Arguments  : ent_buf  buffer which contains the 32 byte entry of a file
*              name     buffer to store the short file name of a file
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_GetSfnName (volatile  USB_INT08U  *entry,
                                USB_INT08U  *name)
{
    USB_INT32U  cnt;


    cnt = 0;
    while (cnt < 8) {
        *name = *entry;                     /* Get first 8 characters of an SFN entry                       */
        name++;
        entry++;
        cnt++;
    }
    *name = 0;
    name--;
    while (*name == 0x20) {                 /* If any spaces exist after the file name, replace them with 0 */
        *name = 0;
        name--;
    }
}

/*
**************************************************************************************************************
*                                       GET EXTENSION OF A FILE
*
* Description: This function reads the extension of a file
*
* Arguments  : ent_buf    buffer which contains the 32 byte entry of a file
*              ext_ptr    buffer to store the extension of a file
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_GetSfnExt (volatile  USB_INT08U  *entry,
                               USB_INT08U  *ext_ptr)
{
    USB_INT32U  cnt;


    cnt = 0;
    while (cnt < 8) {                  /* Go to the beginning of the file extension                          */
        entry++;
        cnt++;
    }
    cnt = 0;
    while (cnt < 3) {                  /* Get 3 characters from there                                       */
        *ext_ptr = *entry;
        ext_ptr++;
        entry++;
        cnt++;
    }
    *ext_ptr = 0;
    ext_ptr--;
    while (*ext_ptr == ' ') {          /* If any spaces exist after the file extension, replace them with 0 */
        *ext_ptr = 0;
        ext_ptr--;
    }
}

/*
**************************************************************************************************************
*                                       CASE INSENSITIVE COMPARISON OF STRINGS
*
* Description: This function compares two strings case insensitively
*
* Arguments  : str1               Pointer to the first string
*              str2               Pointer to the second string
*
* Returns    : MATCH_FOUND        if both the strings are same
*              NATCH_NOT_FOUND    if both the strings are different
*
**************************************************************************************************************
*/
static
USB_INT32S  FAT_StrCaseCmp (USB_INT08U  *str1,
                            USB_INT08U  *str2)
{
    while (*str1 && *str2) {
        if (*str1 == *str2 || *str1 == (*str2 + 32) || *str1 == (*str2 - 32)) {
            str1++;
            str2++;
            continue;
        } else {
            return (MATCH_NOT_FOUND);
        }
    }
    if (*str1 == 0 && *str2 == 0) {
        return (MATCH_FOUND);
    } else {
        return (MATCH_NOT_FOUND);
    }
}

/*
**************************************************************************************************************
*                                       CHECK TYPE OF THE ENTRY
*
* Description: This function checks the type of file entry.
*
* Arguments  : ent           Pointer to the buffer containing the entry
*
* Returns    : LAST_ENTRY    if the entry is last entry
*              FREE_ENTRY    if the entry is free entry
*              LFN_ENTRY     if the entry is long file name entry
*              SFN_ENTRY     if the entry is short file name entry
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_ChkEntType (volatile  USB_INT08U  *ent)
{
    if (ent[0] == 0x00) {                              /* First byte is 0 means it is the last entry        */
        return (LAST_ENTRY);
    }

    if (ent[0] == 0xE5) {                              /* First byte is 0xE5 means it is the free entry     */
        return (FREE_ENTRY);
    }

    if (0x0F == ent[11]) {                             /* If 11th byte of an entry is 0x0F, it is LFN       */
        return (LFN_ENTRY);

    } else {
        return (SFN_ENTRY);                            /* Else it is the SFN                                */
    }
}

/*
**************************************************************************************************************
*                                       READ DATA REQUESTED BY THE USER
*
* Description: This function reads data requested by the application from the file pointed by file descriptor.
*
* Arguments  : fd                  file descriptor that points to a file
*              buffer              buffer into which the data is to be read
*              num_bytes           number of bytes requested by the application
*
* Returns    : total_bytes_read    Total bytes actually read.
*
**************************************************************************************************************
*/

USB_INT32U  FILE_Read (          USB_INT32U   fd,
                       volatile  USB_INT08U  *buffer,
                                 USB_INT32U   num_bytes)
{
    USB_INT32U   total_bytes_to_read;            /* Total bytes requested by the application                */
    USB_INT32U   total_bytes_read;               /* Total bytes read                                        */
    USB_INT32U   bytes_read;                     /* Bytes read from one cluster                             */
    USB_INT32U   bytes_to_read;                  /* Bytes to be read in one cluster                         */
    FILE_ENTRY  *entry;                          /* Entry that contains the file attribute information      */
    USB_INT32U   next_clus;                      /* Next cluster of the current cluster in the cluster chain*/

    entry = (FILE_ENTRY *)fd;                    /* Get file entry from file descriptor                     */
    total_bytes_read = 0;

    if (entry->FileSize == 0) {
        return (0);
    }
    if (num_bytes < entry->FileSize) {
        total_bytes_to_read = num_bytes;
    } else {
        total_bytes_to_read = entry->FileSize;
    }
    do {
        next_clus = FAT_GetNextClus(entry->CurrClus);     /* Get next cluster                               */
        if (next_clus == 0) {                             /* If the current cluster is the last cluster     */
                                                          /* If the offset is at the end of the file        */
            if (entry->CurrClusOffset == (entry->FileSize % FAT_Desc.BytsPerClus)) {
                return (0);                               /* No more bytes to read                          */
            }                               /* If requested number is > remaining bytes in the last cluster */
            if (total_bytes_to_read > ((entry->FileSize % FAT_Desc.BytsPerClus) - entry->CurrClusOffset)) {
                total_bytes_to_read = (entry->FileSize % FAT_Desc.BytsPerClus) - entry->CurrClusOffset;
            }
            bytes_to_read = total_bytes_to_read;
                                         /* If requested number is > remaining bytes in the current cluster */
        } else if (total_bytes_to_read > (FAT_Desc.BytsPerClus - entry->CurrClusOffset)) {
            bytes_to_read = FAT_Desc.BytsPerClus - entry->CurrClusOffset;
        } else {
            bytes_to_read = total_bytes_to_read;
        }
        bytes_read = FAT_ClusRead(entry->CurrClus,       /* Read bytes from a single cluster                */
                                  entry->CurrClusOffset,
                                  buffer,
                                  bytes_to_read);
        buffer              += bytes_read;
        total_bytes_read    += bytes_read;
        total_bytes_to_read -= bytes_read;
                                             /* If the cluster offset reaches end of the cluster, make it 0 */
        if (entry->CurrClusOffset + bytes_read == FAT_Desc.BytsPerClus) {
            entry->CurrClusOffset = 0;
        } else {
            entry->CurrClusOffset += bytes_read;        /* Else increment the cluster offset                */
        }
        if (entry->CurrClusOffset == 0) {
            entry->CurrClus = (next_clus > 0) ? next_clus : entry->CurrClus;
        }
    } while (total_bytes_to_read);
    return (total_bytes_read);
}

/*
**************************************************************************************************************
*                                                 READ FROM ONE CLUSTER
*
* Description: This function reads the data from a single cluster.
*
* Arguments  : curr_clus         Current cluster from which the data has to read
*              clus_offset       Position in the current cluster from which the data has to read
*              buffer            Buffer into which the data has to read
*              num_bytes         Number of bytes to read
*
* Returns    : tot_bytes_read    Total bytes read from the current cluster
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_ClusRead (          USB_INT32U   curr_clus,
                                    USB_INT32U   clus_offset,
                          volatile  USB_INT08U  *buffer,
                                    USB_INT32U   num_bytes)
{
    USB_INT32U  tot_bytes_read;                              /* total bytes read in the current cluster     */
    USB_INT32U  n_bytes;                                     /* Bytes to read in the current sector         */
    USB_INT32U  start_sec;                                   /* Starting sector of the current cluster      */
    USB_INT32U  sec_num;                                     /*Current sector number                        */
    USB_INT32U  num_sec;                                     /* Number of sectors to be read                */
    USB_INT32U  sec_offset;                                  /* Offset in the current sector                */
    USB_INT32U  cnt;


    tot_bytes_read = 0;
    start_sec  = ((curr_clus - 2) * FAT_Desc.SecPerClus) + FAT_Desc.FirstDataSec;
    sec_num    = start_sec + (clus_offset / FAT_Desc.BytsPerSec);
    num_sec    = num_bytes / FAT_Desc.BytsPerSec;
    sec_offset = clus_offset % FAT_Desc.BytsPerSec;

    if (sec_offset) {                                 /* If the sector offset is at the middle of a sector  */
        IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);           /* Read the first sector                              */
        n_bytes = (FAT_Desc.BytsPerSec - sec_offset <= num_bytes) ?
                  (FAT_Desc.BytsPerSec - sec_offset) : num_bytes;
        for (cnt = sec_offset; cnt < sec_offset + n_bytes; cnt++) {
            *buffer = FATBuffer[cnt];                 /* Copy the required bytes to user buffer             */
             buffer++;
        }
        tot_bytes_read += n_bytes;
        clus_offset    += n_bytes;
        num_bytes      -= n_bytes;
        sec_num++;
    }

    if (num_bytes / FAT_Desc.BytsPerSec) {         /* Read all the remaining full sectors                */

        num_sec = num_bytes / FAT_Desc.BytsPerSec;
        IOC_Read(sec_num, num_sec, (USB_INT08U *)buffer);
        buffer         += (num_sec * FAT_Desc.BytsPerSec);
        tot_bytes_read += (num_sec * FAT_Desc.BytsPerSec);
        clus_offset    += (num_sec * FAT_Desc.BytsPerSec);
        num_bytes      -= (num_sec * FAT_Desc.BytsPerSec);
        sec_num        += num_sec;
    }

    if (num_bytes) {                                  /* Read the last sector for the remaining bytes       */
        IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);
        for (cnt = 0; cnt < num_bytes; cnt++) {
            *buffer = FATBuffer[cnt];                 /* Copy the required bytes to user buffer             */
             buffer++;
        }
        tot_bytes_read += num_bytes;
    }

    return (tot_bytes_read);
}

/*
**************************************************************************************************************
*                                       WRITE THE DATA REQUESTED BY THE USER
*
* Description: This function writes data requested by the application to the file pointed by file descriptor
*
* Arguments  : fd                     file descriptor that points to a file
*              buffer                 buffer from which the data is to be written
*              num_bytes              number of bytes requested by the application
*
* Returns    : total_bytes_written    Total bytes actually written
*
**************************************************************************************************************
*/

USB_INT32U  FILE_Write (          USB_INT32U   fd,
                        volatile  USB_INT08U  *buffer,
                                  USB_INT32U   num_bytes)
{
    USB_INT32U   total_bytes_to_write;                      /* Total bytes requested by application         */
    USB_INT32U   total_bytes_written;                       /* Total bytes written                          */
    USB_INT32U   bytes_written;                             /* Bytes written in a single cluster            */
    USB_INT32U   bytes_to_write;                            /* Bytes to write in a single cluster           */
    FILE_ENTRY  *entry;                               /* Entry that contains the file attribute information */
    USB_INT32U   free_clus;                                 /* Free cluster available in the disk           */


    entry                = (FILE_ENTRY *)fd;
    total_bytes_written  = 0;
    total_bytes_to_write = num_bytes;

    if (num_bytes) {
        if (entry->FileSize == 0) {
            free_clus = FAT_GetFreeClus();
            FAT_UpdateFAT(free_clus, (FAT_Desc.FATType == FAT_16)?0xFFFF:0x0FFFFFFF);
            FAT_UpdateFSInfo(1,FAT_GetFreeClus());
            entry->CurrClus  = free_clus;
            IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
            WriteLE16U(&FATBuffer[(entry->EntrySecOffset) + 26], free_clus);
            WriteLE16U(&FATBuffer[(entry->EntrySecOffset) + 20], free_clus>>16);
            IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);
        }
    } else {
        return (0);
    }
    entry->CurrClus = FAT_GetEndClus(entry->CurrClus);           /* Make the current cluster as end cluster */
    entry->CurrClusOffset = entry->FileSize % FAT_Desc.BytsPerClus;   /* Move cluster offset to file end */
    do {
        if (total_bytes_to_write > FAT_Desc.BytsPerClus - entry->CurrClusOffset) {
            bytes_to_write = FAT_Desc.BytsPerClus - entry->CurrClusOffset;
        } else {
            bytes_to_write = total_bytes_to_write;
        }
        bytes_written = FAT_ClusWrite(entry->CurrClus,
                                      entry->CurrClusOffset,
                                      buffer,
                                      bytes_to_write);
        buffer               += bytes_written;
        total_bytes_written  += bytes_written;
        total_bytes_to_write -= bytes_written;
        entry->FileSize      += bytes_written;
        if (entry->CurrClusOffset + bytes_written == FAT_Desc.BytsPerClus) {
            entry->CurrClusOffset = 0;
        } else {
            entry->CurrClusOffset += bytes_written;
        }
        if (entry->CurrClusOffset == 0) {
            free_clus = FAT_GetFreeClus();
            if (free_clus == 0) {
                return (total_bytes_written);
            }
            FAT_UpdateFAT(entry->CurrClus, free_clus);
            FAT_UpdateFAT(free_clus, (FAT_Desc.FATType == FAT_16)?0xFFFF:0x0FFFFFFF);
            FAT_UpdateFSInfo(1,FAT_GetFreeClus());
            entry->CurrClus = free_clus;
        }
    } while (total_bytes_to_write);
    return (total_bytes_written);
}

/*
**************************************************************************************************************
*                                               WRITE TO ONE CLUSTER
*
* Description: This function writes the data to a single cluster.
*
* Arguments  : curr_clus         Current cluster into which the data has to write
*              clus_offset       Position in the current cluster from which the data has to write
*              buffer            Buffer from which the data has to write
*              num_bytes         Number of bytes to write
*
* Returns    : tot_bytes_read    Total bytes written into the current cluster
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_ClusWrite (          USB_INT32U   curr_clus,
                                     USB_INT32U   clus_offset,
                           volatile  USB_INT08U  *buffer,
                                     USB_INT32U   num_bytes)
{
  USB_INT32U  tot_bytes_written;
  USB_INT32U  n_bytes;
  USB_INT32U  start_sec;
  USB_INT32U  sec_num;
  USB_INT32U  num_sec;
  USB_INT32U  sec_offset;
  USB_INT32U  cnt;


  tot_bytes_written = 0;
  start_sec  = ((curr_clus - 2) * FAT_Desc.SecPerClus) + FAT_Desc.FirstDataSec;
  sec_num    = start_sec + (clus_offset / FAT_Desc.BytsPerSec);
  num_sec    = num_bytes / FAT_Desc.BytsPerSec;
  sec_offset = clus_offset % FAT_Desc.BytsPerSec;

  if (sec_offset) {
    IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);
    n_bytes = (FAT_Desc.BytsPerSec - sec_offset <= num_bytes) ?
              (FAT_Desc.BytsPerSec - sec_offset) : num_bytes;
    for (cnt = sec_offset; cnt < (sec_offset + n_bytes); cnt++) {
      FATBuffer[cnt] = *buffer;
      buffer++;
    }
    IOC_Write(sec_num, 1, (const USB_INT08U *)FATBuffer);
    tot_bytes_written += n_bytes;
    clus_offset       += n_bytes;
    num_bytes         -= n_bytes;
    sec_num++;
  }
  if (num_bytes / FAT_Desc.BytsPerSec) {
    num_sec = num_bytes / FAT_Desc.BytsPerSec;
    IOC_Write(sec_num, num_sec, (const USB_INT08U *)buffer);
    buffer            += (num_sec * FAT_Desc.BytsPerSec);
    tot_bytes_written += (num_sec * FAT_Desc.BytsPerSec);
    clus_offset       += (num_sec * FAT_Desc.BytsPerSec);
    num_bytes         -= (num_sec * FAT_Desc.BytsPerSec);
    sec_num           += num_sec;
  }
  if (num_bytes) {
        IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);

    for (cnt = 0; cnt < num_bytes; cnt++) {
      FATBuffer[cnt] = *buffer;
      buffer++;
    }
        IOC_Write(sec_num, 1, (const USB_INT08U *)FATBuffer);
    tot_bytes_written += num_bytes;
  }
  return (tot_bytes_written);
}

/*
**************************************************************************************************************
*                                              GET NEXT CLUSTER
*
* Description: This function returns next cluster of the current cluster in the cluster chain. If the current
*              cluster is the last cluster then this function returns 0
*
* Arguments  : clus_no    The cluster number for which the next cluster to be found
*
* Returns    : next_clus  if the current cluster is not the last cluster
*              0          if the current cluster is the last cluster
*                         Note: In practical cluster number 0 doesn't exist
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_GetNextClus (USB_INT32U  clus_no)
{
    USB_INT32U  sec_num;
    USB_INT32U  ent_offset;
    USB_INT32U  next_clus;

                                          /* Get the sector number in the FAT that contains current cluster */
    sec_num = FAT_Desc.FATStartSec + ((clus_no << FAT_Desc.FATMultShift) / FAT_Desc.BytsPerSec);
                                   /* Get the sector offset in the FAT where the current cluster is located */
    ent_offset = (clus_no << FAT_Desc.FATMultShift) % FAT_Desc.BytsPerSec;

    IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);                      /* Read that sector                        */
    if (FAT_16 == FAT_Desc.FATType)
    {
      next_clus = ReadLE16U(&FATBuffer[ent_offset]);         /* Read the next cluster                   */
      if (  next_clus >= 0xFFF8)
      {      /* If that value is in between 0xFFF8 and 0xFFFF */
          next_clus = 0;                                     /* Current cluster is the end cluster      */
      }
    }
    else
    {
      next_clus = ReadLE32U(&FATBuffer[ent_offset]);         /* Read the next cluster                   */
      if (  next_clus >= 0x0FFFFFF8)
      {      /* If that value is in between 0x0FFFFFF8 and 0xFFFFFFFF */
          next_clus = 0;                                     /* Current cluster is the end cluster      */
      }
    }
    return (next_clus);

}

/*
**************************************************************************************************************
*                                               GET FREE CLUSTER
*
* Description: This function returns the free cluster if available
*
* Arguments  : None
*
* Returns    : free_clus    if available
*              0            if not available(means the disk is full)
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_GetFreeClus (void)
{
    USB_INT32U  num_sec;
    USB_INT32U  cnt;
    USB_INT32U  sec_num;
    USB_INT32U  free_clus;
    USB_INT32U  size = (1 << FAT_Desc.FATMultShift);

    sec_num = FAT_Desc.FATStartSec + (FAT_Desc.FSNextFreeClus << FAT_Desc.FATMultShift) / FAT_Desc.BytsPerSec;
    num_sec = FAT_Desc.FATSz;

    while (sec_num < (FAT_Desc.FATStartSec + num_sec))
    {
      IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);
      for (cnt = 0; cnt < FAT_Desc.BytsPerSec; cnt += size)
      {
        if (ReadLE32U(&FATBuffer[cnt]) == 0) {
            free_clus = (((sec_num - FAT_Desc.FATStartSec) * FAT_Desc.BytsPerSec) + cnt) / size;
            if (free_clus > FAT_Desc.FSLastClus)
            {
              return (0);
            }
            return (free_clus);
        }
      }
      sec_num++;
    }

    return (0);
}

/*
**************************************************************************************************************
*                                       UPDATE FILE ALLOCATION TABLE
*
* Description: This function updates the file allocation table
*
* Arguments  : curr_clus    Offset of the current cluster number in the file allocation table
*              value        Value with which this offset to be updated
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_UpdateFAT (USB_INT32U  curr_clus,
                     USB_INT32U  value)
{
    USB_INT32U  sec_num;
    USB_INT32U  sec_offset;
    USB_INT32U  cell_temp;
    USB_INT08U  fat_table;

    sec_num = FAT_Desc.FATStartSec + (curr_clus << FAT_Desc.FATMultShift) / FAT_Desc.BytsPerSec;
    sec_offset = (curr_clus << FAT_Desc.FATMultShift) % FAT_Desc.BytsPerSec;
    fat_table = 0;
    do
    {
      IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);
      if (FAT_16 == FAT_Desc.FATType)
      {
        WriteLE16U(&FATBuffer[sec_offset], value);
      }
      else
      {
        value     &= 0x0FFFFFFF;
        cell_temp  = ReadLE32U(&FATBuffer[sec_offset]);
        cell_temp &= 0xF0000000;
        value |= cell_temp;
        WriteLE32U(&FATBuffer[sec_offset], value);
      }
      IOC_Write(sec_num, 1, (const USB_INT08U *)FATBuffer);

      sec_num += FAT_Desc.FATSz;
    }
    while(++fat_table < FAT_Desc.FATsNum);
}

/*
**************************************************************************************************************
*                                              UPDATE THE FILE ENTRY
*
* Description: This function updates the file entry that is located in the root directory
*
* Arguments  : entry    Pointer to the FILE ENTRY structure which contains the information about the file
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_UpdateEntry (FILE_ENTRY  *entry)
{
    USB_INT32U  sec_num;
    USB_INT32U  offset;

    sec_num = entry->EntrySec;
    offset  = entry->EntrySecOffset;
    IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);
    WriteLE32U(&FATBuffer[offset + 28], entry->FileSize);
    IOC_Write(sec_num, 1, (const USB_INT08U *)FATBuffer);
}

/*
**************************************************************************************************************
*                                              CREATING AN ENTRY
*
* Description: This function creates a file entry in the root directory if the file does not exist
*
* Arguments  : ent_name_given    The file name with which the entry is to be created
*              entry             Pointer to FILE ENTRY structure
*
* Returns    : OK                If the entry already exists or successfully created if it doesn't exists
*              ERROR             If failed to create the entry
*
**************************************************************************************************************
*/
static
USB_INT32S  FAT_CreateEntry (USB_INT08U  *ent_name_given,
                             FILE_ENTRY  *entry)
{
    USB_INT32S  rc;

    rc = FAT_FindEntry(ent_name_given, entry);        /* Find for the given file name in the root directory */
    if (rc == MATCH_FOUND) {                          /* If match found, return                             */
      if(FILE_ATTR_READ_ONLY & FAT_File_GetAttr(entry))
      {
        return (ERR_FILE_READ_ONLY);
      }
      return (rc);
    } else {
        rc = FAT_GetFreeEntry(entry);                 /* Else get a free entry from the root directory      */
        if (rc != OK) {
            return (rc);
        } else {
            FAT_PutSFN(ent_name_given, entry);        /* Store the given short file name in that entry      */
            return (rc);
        }
    }
}

/*
**************************************************************************************************************
*                                                GET GREER ENTRY
*
* Description: This function searches for a free entry in the root directory. If a free entry is found, the
*              sector number and sector offset where the entry is located will be stored
*
* Arguments  : entry    Pointer to FILE_ENTRY structure
*
* Returns    : OK       If a free entry is found
*              ERROR    If no free entry is found
*
**************************************************************************************************************
*/
static
USB_INT32S  FAT_GetFreeEntry (FILE_ENTRY  *entry)
{
USB_INT08U  *buf = NULL;
USB_INT32U  ent_type;
USB_INT32U  sec_num;
USB_INT32U  free_clus;
USB_INT32U  i;

  do
  {
    if(NULL == (buf = FAT_GetNexRootEntry(buf,&sec_num)))
    {
      if(FAT_16 == FAT_Desc.FATType)
      {
        return (ERR_ROOT_DIR_FULL);
      }

      free_clus = FAT_GetFreeClus();

      if (free_clus == 0) {
          return (ERR_ROOT_DIR_FULL);
      }

      sec_num -= FAT_Desc.FirstDataSec;
      FAT_UpdateFAT((sec_num/FAT_Desc.SecPerClus) + 2, free_clus);
      FAT_UpdateFAT(free_clus, 0x0FFFFFFF);
      FAT_UpdateFSInfo(1,FAT_GetFreeClus());

      sec_num = (free_clus-2)*FAT_Desc.SecPerClus + FAT_Desc.FirstDataSec;
      // Clear new added cluster
      for (i = 0; i < FAT_Desc.BytsPerSec; i++)
      {
        *(FATBuffer+i) = 0x00;
      }
      for(i = 0; i < FAT_Desc.SecPerClus; i++)
      {
        IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
      }
      buf = (USB_INT08U *)FATBuffer;
      break;
    }
    ent_type = FAT_ChkEntType(buf);
  }
  while(   ent_type != FREE_ENTRY
        && ent_type != LAST_ENTRY);

  entry->EntrySec       = sec_num;
  entry->EntrySecOffset = buf - FATBuffer;
  return (OK);
}

/*
**************************************************************************************************************
*                                              PUT SHORT FILE NAME
*
* Description: This function fills the file entry with the short file name given by the user
*
* Arguments  : ent_name_given    File name given by the user
*              entry             Pointer to the FILE_ENTRY structure
*
* Returns    : None
*
**************************************************************************************************************
*/
static
void  FAT_PutSFN (USB_INT08U  *ent_name_given,
                  FILE_ENTRY  *entry)
{
    USB_INT32U  idx;

                                           /* Read the sector from root directory containing the free entry */
    IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
    for (idx = 0; idx < 8; idx++) {        /* Fill the first eight characters of the entry with file name   */
        if (*ent_name_given == '.') {
            while (idx < 8) {
                FATBuffer[entry->EntrySecOffset + idx] = 0x20;
                idx++;
            }
            ent_name_given++;
        } else {
            FATBuffer[entry->EntrySecOffset + idx] = *ent_name_given;
            ent_name_given++;
        }
    }

    for (idx = 8; idx < 11; idx++) {                      /* Fill the next 3 characters with file extension */
        if (*ent_name_given == '.') {
            while (idx < 11) {
                FATBuffer[entry->EntrySecOffset + idx] = 0x20;
                idx++;
            }
        } else {
            FATBuffer[entry->EntrySecOffset + idx] = *ent_name_given;
            ent_name_given++;
        }
    }
    FATBuffer[entry->EntrySecOffset + idx] = 0x20;
    for (idx = 12; idx < 32; idx++) {                           /* Fill all the remaining bytes with 0's    */
        FATBuffer[entry->EntrySecOffset + idx] = 0;
    }
#ifdef FAT_TIME_DATE_ENA
    // Update Time and Date
    FAT_DATE date;
    FAT_TIME time;

    FATBuffer[entry->EntrySecOffset + 13] = FAT_GetDateTime(&date,&time);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 16],date.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 14],time.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 18],date.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 24],date.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 22],time.Value);

#endif

    IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);                 /* Write the sector into the root directory */
}

/*
**************************************************************************************************************
*                                                  FILE CLOSE
*
* Description: This function closes the opened file by making all the elements of FILE_ENTRY structure to 0
*
* Arguments  : fd    File descriptor which points to the file to be closed
*
* Returns    : None
*
**************************************************************************************************************
*/

void  FILE_Close (USB_INT32U  fd)
{
    FILE_ENTRY  *entry;

    entry = (FILE_ENTRY *)fd;
    IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
    WriteLE32U(&FATBuffer[entry->EntrySecOffset + 28], entry->FileSize);    /* Update the file size         */
  #ifdef FAT_TIME_DATE_ENA
    FAT_DATE date;
    FAT_TIME time;
    if (RDWR == entry->Status)
    {
      FAT_GetDateTime(&date,&time);
      WriteLE16U(&FATBuffer[entry->EntrySecOffset + 18],date.Value);
      WriteLE16U(&FATBuffer[entry->EntrySecOffset + 24],date.Value);
      WriteLE16U(&FATBuffer[entry->EntrySecOffset + 22],time.Value);
    }
  #endif // FAT_TIME_DATE_ENA

    IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);

    entry->CurrClus       = 0;
    entry->CurrClusOffset = 0;
    entry->FileSize       = 0;
    entry->EntrySec       = 0;
    entry->EntrySecOffset = 0;
    entry->Status         = 0;
}

/*
**************************************************************************************************************
*                                               GET END CLUSTER
*
* Description: This function end cluster in the cluster chain of a cluster
*
* Arguments  : clus_no    Starting cluster of the cluster chain in which end cluster to be found
*
* Returns    : End cluster in the cluster chain
*
**************************************************************************************************************
*/
static
USB_INT32U  FAT_GetEndClus (USB_INT32U  clus_no)
{
    USB_INT32U  next_clus;


    next_clus = clus_no;
    while (next_clus) {
        next_clus = FAT_GetNextClus(clus_no);
        if (next_clus) {
            clus_no = next_clus;
        }
    }
    return (clus_no);
}

/*
**************************************************************************************************************
*                                               GET NEXT ROOT ENTRY
*
* Description: This function return next root directory entry
*
* Arguments  : buf previous entry pointer (if NULL get first entry)
*            : p_sec_num return file name's sector
*
* Returns    : next root directory entry
*
**************************************************************************************************************
*/
static
USB_INT08U  * FAT_GetNexRootEntry (USB_INT08U  *buf, USB_INT32U * p_sec_num)
{
static USB_INT32U   clus_num;
static USB_INT32U   sec_num;
static USB_INT32U   root_dir_sec;

  if(NULL != p_sec_num)
  {
    *p_sec_num = sec_num;
  }

  if(NULL == buf)
  {
    if(FAT_32 == FAT_Desc.FATType)
    {
      // Calculate size in sectors of the root directory
      root_dir_sec = FAT_Desc.SecPerClus;
      clus_num = FAT_Desc.RootClus;
    }
    else
    {
      root_dir_sec = FAT_Desc.RootDirSec;
    }
    sec_num = FAT_Desc.RootDirStartSec;                    /* For all the sectors in root directory       */
    IOC_Read(sec_num, 1, (USB_INT08U *)FATBuffer);                    /* Read one sector                             */
    if(NULL != p_sec_num)
    {
      *p_sec_num = sec_num;
    }
    return((USB_INT08U *)FATBuffer);
  }

  if(sec_num >= (FAT_Desc.RootDirStartSec + root_dir_sec))
  {
    if(FAT_32 == FAT_Desc.FATType)
    {
      if((clus_num = FAT_GetNextClus(clus_num)))
      {
        sec_num = ((clus_num-2) * FAT_Desc.SecPerClus)+ FAT_Desc.FirstDataSec;
        root_dir_sec = FAT_Desc.SecPerClus;
        IOC_Read(++sec_num, 1, (USB_INT08U *)FATBuffer);              /* Read one sector                             */
        if(NULL != p_sec_num)
        {
          *p_sec_num = sec_num;
        }
        return((USB_INT08U *)FATBuffer);
      }
    }
    return(NULL);
  }
  buf = buf + 32;
  if(buf >= (FATBuffer + FAT_Desc.BytsPerSec))
  {
    IOC_Read(++sec_num, 1, (USB_INT08U *)FATBuffer);                  /* Read one sector                             */
    buf = (USB_INT08U  *)FATBuffer;
    if(NULL != p_sec_num)
    {
      *p_sec_num = sec_num;
    }
  }
  return(buf);
}

/*
**************************************************************************************************************
*                                               UPDATE FS INFO
*
* Description: This function update FS Info record on FAT32
*
* Arguments  : ClusNum allocated/released clusters
*            : NextFreeClus next free cluster
*
* Returns    : none
*
**************************************************************************************************************
*/
static
void FAT_UpdateFSInfo (USB_INT32S ClusNum, USB_INT32U NextFreeClus)
{
USB_INT32U temp;

  if(NextFreeClus)
  {
    FAT_Desc.FSNextFreeClus = NextFreeClus;
  }

  if(   FAT_32 != FAT_Desc.FATType
     || 0 == FAT_Desc.FSInfoSecStart)
  {
    return;
  }

  // Read FSInfo record
  IOC_Read(FAT_Desc.FSInfoSecStart, 1, (USB_INT08U *)FATBuffer);

  if(0xFFFFFFFF != (temp = ReadLE32U(&FATBuffer[488])))
  {
    WriteLE32U(&FATBuffer[488], temp - ClusNum);
  }

  if(NextFreeClus)
  {
    // Update only if the current free cluster is lower from that in the record
    if((temp = ReadLE32U(&FATBuffer[488])) > NextFreeClus)
    {
      WriteLE32U(&FATBuffer[492], NextFreeClus);
    }
  }

  // Write updated FS Info record
  IOC_Write(FAT_Desc.FSInfoSecStart, 1, (const USB_INT08U *)FATBuffer);
}

#ifdef FAT_TIME_DATE_ENA
/*
**************************************************************************************************************
*                                               GET SYSTEM DATA and TIME
*
* Description: This function return system date and time in FAT date/time format
*
* Arguments  : p_date system date
*            : p_time system time
*
* Returns    : Tenths of second
*
**************************************************************************************************************
*/
static
USB_INT08U FAT_GetDateTime (FAT_DATE * p_date, FAT_TIME * p_time)
{
USB_INT08U msecond;

  FAT_PREPARE_DATE_TIME();
  p_date->Day   = FAT_GET_DAY();
  p_date->Month = FAT_GET_MONTH();
  // calculate years offset
  p_date->Year  = FAT_GET_YEAR() - FAT_DATE_START_YEAR;

  msecond = FAT_GET_SEC();
  // second resolution is 2 sec
  p_time->Seconds = msecond >> 1;
  p_time->Minutes = FAT_GET_MINUTE();
  p_time->Hours   = FAT_GET_HOUR();

  msecond = (msecond & 1)?100:0;
  msecond+= FAT_GET_10MSEC();

  return(msecond);
}

/*
**************************************************************************************************************
*                                    UPDATE FILE DATA/TIME FIELDS
*
* Description: This function update file's data and time filed regarding update flags
*
* Arguments  : entry Pointer to the FILE_ENTRY structure
*            : flags update type flags
*
* Returns    : none
*
**************************************************************************************************************
*/
static
void FAT_UpdateDateTime (FILE_ENTRY * entry, USB_INT32U flags)
{
FAT_DATE    date;
FAT_TIME    time;
USB_INT08U  msec;

  msec = FAT_GetDateTime(&date,&time);
  IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
  if(CREATE_DATA_TIME & flags)
  {
    FATBuffer[entry->EntrySecOffset + 13] = msec;
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 16],date.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 14],time.Value);
  }
  if(WRITE_DATA_TIME & flags)
  {
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 24],date.Value);
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 22],time.Value);
  }
  if(ACCESS_DATA & flags)
  {
    WriteLE16U(&FATBuffer[entry->EntrySecOffset + 18],date.Value);
  }
  IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);

}
#endif

/*
**************************************************************************************************************
*                                 GET FILE ATTRIBUTES
*
* Description: This function return file's attributes
*
* Arguments  : Name of the file. The file must be in root directory.
*            : flags pointer to file's attributes variable
*
* Returns    :
*
**************************************************************************************************************
*/
USB_INT32S  FILE_GetAttr        (         USB_INT08U *file_name,
                                          USB_INT08U *attr)
{
USB_INT32S rs;
FILE_ENTRY entry;

  rs = FAT_FindEntry(file_name, &entry);
  if (rs == MATCH_FOUND)
  {
    IOC_Read(entry.EntrySec, 1, (USB_INT08U *)FATBuffer);
    *attr = FATBuffer[entry.EntrySecOffset + 11];
  }
  return(rs);
}

/*
**************************************************************************************************************
*                                 GET FILE ATTRIBUTES
*
* Description: This function return file's attributes
*
* Arguments  : Name of the file. The file must be in root directory.
*
* Returns    :
*
**************************************************************************************************************
*/
static
USB_INT08S FAT_File_GetAttr(FILE_ENTRY * entry)
{
  IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
  return(FATBuffer[entry->EntrySecOffset + 11]);
}

/*
**************************************************************************************************************
*                                 SET FILE ATTRIBUTES
*
* Description: This function return file's attributes
*
* Arguments  : Name of the file. The file must be in root directory.
*            : flags file's attributes
*
* Returns    :
*
**************************************************************************************************************
*/
USB_INT32S  FILE_SetAttr        (         USB_INT08U *file_name,
                                          USB_INT08U  attr)
{
USB_INT32S rs;
FILE_ENTRY entry;

  rs = FAT_FindEntry(file_name, &entry);
  if (rs == MATCH_FOUND)
  {
    FAT_File_SetAttr(&entry,attr);
  }
  return(rs);
}

/*
**************************************************************************************************************
*                                 SET FILE ATTRIBUTES
*
* Description: This function return file's attributes
*
* Arguments  : Name of the file. The file must be in root directory.
*            : flags file's attributes
*
* Returns    :
*
**************************************************************************************************************
*/
static
void FAT_File_SetAttr (FILE_ENTRY * entry, USB_INT08S attr)
{
  IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
  FATBuffer[entry->EntrySecOffset + 11] = attr;
  IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);
}

/*
**************************************************************************************************************
*                                 FILE DELETE
*
* Description: This function delete file from root directory
*
* Arguments  : Name of the file. The file must be in root directory.
*
* Returns    :
*
**************************************************************************************************************
*/
USB_INT32S  FILE_Delete ( USB_INT08U *file_name)
{
FILE_ENTRY entry;
USB_INT32S rs;

  rs = FAT_FindEntry(file_name, &entry);
  if (rs == MATCH_FOUND)
  {
    if(FILE_ATTR_READ_ONLY & FAT_File_GetAttr(&entry))
    {
      return (ERR_FILE_READ_ONLY);
    }
    FAT_FileRelease(&entry);
  }
  return(rs);
}

/*
**************************************************************************************************************
*                                 FILE RELEASE
*
* Description: This function delete file from root directory
*
* Arguments  : entry Pointer to the FILE_ENTRY structure
*
* Returns    :
*
**************************************************************************************************************
*/
static
void FAT_FileRelease(FILE_ENTRY * entry)
{
USB_INT32S clust, clust_count, next_clust;

  IOC_Read(entry->EntrySec, 1, (USB_INT08U *)FATBuffer);
  FATBuffer[entry->EntrySecOffset] = FILE_DELETE_MARK;
  IOC_Write(entry->EntrySec, 1, (const USB_INT08U *)FATBuffer);
  // Release file's clusters from FAT table
  clust       = entry->CurrClus;
  clust_count = 0;
  do
  {
    next_clust = FAT_GetNextClus(clust);
    FAT_UpdateFAT(clust,0);
    clust = next_clust;
    --clust_count;
  }
  while(clust);
  FAT_UpdateFSInfo(clust_count,entry->CurrClus);
}
