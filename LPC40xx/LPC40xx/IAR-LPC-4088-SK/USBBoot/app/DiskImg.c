/* Disk Image */
#include "fat.h"
#include "DiskImg.h"

#define YEAR  2012
#define MONTH 11
#define DAY   5

#define SEC     0
#define MIN    45
#define HOUR   15

const boot_t BootSect = {
  .BS_jmpBoot[0] = 0xEB,
  .BS_jmpBoot[1] = 0x3C,
  .BS_jmpBoot[2] = 0x90,
  .BS_OEMName = "MSWIN4.1",
  .BPB_BytsPerSec = SECTOR_SIZE,
  .BPB_SecPerClus = SECT_PER_CLUSTER,
  .BPB_RsvdSecCnt = RSVD_SECTORS,
  .BPB_NumFATs = 1,
  .BPB_RootEntCnt = ROOT_ENR_CNT,
  .BPB_TotSec16 = TOTAL_SECT,
  .BPB_Media = 0xF8,
  .BPB_FATSz16 = 2,
  .BPB_SecPerTrk = 1,
  .BPB_NumHeads = 1,
  .BPB_HiddSec = 0,
  .BPB_TotSec32 = 0,
  .BS_DrvNum = 0x80,
  .BS_Reserved1 = 0,
  .BS_BootSig = 0,
  .BS_VolID = 0,
#if (CRP == NONPROT)
  .BS_VolLab = "CRP DISABLE",
#elif (CRP == CRP1)
  .BS_VolLab = "CRP1 ENABLE",
#elif (CRP == CRP2)
  .BS_VolLab = "CRP2 ENABLE",
#elif (CRP == CRP3)
  .BS_VolLab = "CRP3 ENABLE",
#endif
  .BS_FilSysType = "FAT12   ",
  .BS_signature = 0xAA55,
};

const dir_etnry_t DirEntry[ROOT_ENR_CNT] = 
{
  {
#if (CRP == NONPROT)
    .DIR_Name = "CRP DISABLE",
#elif (CRP == CRP1)
    .DIR_Name = "CRP1 ENABLE",
#elif (CRP == CRP2)
    .DIR_Name = "CRP2 ENABLE",
#elif (CRP == CRP3)
    .DIR_Name = "CRP3 ENABLE",
#endif
    .DIR_Attr = ATTR_VOLUME_ID | ATTR_ARCHIVE,
    .DIR_NTRes = 0x0,
    .DIR_CrtTimeTenth = 0,
    .DIR_CrtTime = 0,
    .DIR_CrtDate = 0,
    .DIR_LstAccDate = 0,
    .DIR_FstClusHI = 0,
    .DIR_WrtTime = 0,
    .DIR_WrtDate = 0,
    .DIR_FstClusLO = 0,
    .DIR_FileSize = 0,
  },
  {
    .DIR_Name = "USBBOOT BIN",
    .DIR_Attr = ATTR_READ_ONLY | ATTR_SYSTEM | ATTR_ARCHIVE,
    .DIR_NTRes = 0x0,
    .DIR_CrtTimeTenth = 0,
    .DIR_CrtTime = 0,
    .DIR_CrtDate = 0,
    .DIR_LstAccDate = 0,
    .DIR_FstClusHI = 0,
    .DIR_WrtTime = (HOUR<<11)|(MIN<<5)|(SEC/2),
    .DIR_WrtDate = ((YEAR-1980)<<9)|(MONTH<<5)|(DAY),
    .DIR_FstClusLO = 0x2,
    .DIR_FileSize = USB_BOOT_SIZE,
  },
  {
    .DIR_Name = "FIRMWAREBIN",
    .DIR_Attr = ATTR_ARCHIVE,
    .DIR_NTRes = 0x0,
    .DIR_CrtTimeTenth = 0,
    .DIR_CrtTime = 0,
    .DIR_CrtDate = 0,
    .DIR_LstAccDate = 0,
    .DIR_FstClusHI = 0,
    .DIR_WrtTime = (HOUR<<11)|(MIN<<5)|(SEC/2),
    .DIR_WrtDate = ((YEAR-1980)<<9)|(MONTH<<5)|(DAY),
    .DIR_FstClusLO = 0x2 + USER_PROGRAM_START/SECTOR_SIZE/SECT_PER_CLUSTER,
    .DIR_FileSize = USER_PROGRAM_SIZE,
  },
  {0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5},
  {0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5}
};
