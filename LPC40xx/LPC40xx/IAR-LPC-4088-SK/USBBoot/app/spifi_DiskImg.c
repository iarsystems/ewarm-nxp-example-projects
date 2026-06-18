/* SPIFI Disk Image */
#include "fat.h"
#include "spifi_DiskImg.h"

#define YEAR  2012
#define MONTH 11
#define DAY   5

#define SEC     0
#define MIN    45
#define HOUR   15

const boot_t Spifi_BootSect = {
  .BS_jmpBoot[0] = 0xEB,
  .BS_jmpBoot[1] = 0x3C,
  .BS_jmpBoot[2] = 0x90,
  .BS_OEMName = "MSWIN4.1",
  .BPB_BytsPerSec = SPIFI_SECTOR_SIZE,
  .BPB_SecPerClus = SPIFI_SECT_PER_CLUSTER,
  .BPB_RsvdSecCnt = SPIFI_RSVD_SECTORS,
  .BPB_NumFATs = 1,
  .BPB_RootEntCnt = SPIFI_ROOT_ENR_CNT,
  .BPB_TotSec16 = SPIFI_TOTAL_SECT,
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
  .BS_VolLab = "SPIFI FLASH",
  .BS_FilSysType = "FAT12   ",
  .BS_signature = 0xAA55,
};

const dir_etnry_t Spifi_DirEntry[SPIFI_ROOT_ENR_CNT] = 
{
  {
    .DIR_Name = "SPIFI FLASH",
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
    .DIR_Name = "SPIFIIMGBIN",
    .DIR_Attr = ATTR_ARCHIVE,
    .DIR_NTRes = 0x0,
    .DIR_CrtTimeTenth = 0,
    .DIR_CrtTime = 0,
    .DIR_CrtDate = 0,
    .DIR_LstAccDate = 0,
    .DIR_FstClusHI = 0,
    .DIR_WrtTime = (HOUR<<11)|(MIN<<5)|(SEC/2),
    .DIR_WrtDate = ((YEAR-1980)<<9)|(MONTH<<5)|(DAY),
    .DIR_FstClusLO = 0x2,
    .DIR_FileSize = SPIFI_MEMORY_SIZE,
  },
  {0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5},
  {0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5},{0xE5}
};
