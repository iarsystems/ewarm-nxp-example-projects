/**********************************************************************
* $Id: FlashOS.h 28 2014-01-27 14:45:13Z danielru $		FlashOS.h			2011-06-02
*//**
* @file		FlashOS.h
* @brief	Data structures and entries for Flash Programming Functions
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#define VERS       1           // Interface Version 1.01

#define UNKNOWN    0           // Unknown
#define ONCHIP     1           // On-chip Flash Memory
#define EXT8BIT    2           // External Flash Device on 8-bit  Bus
#define EXT16BIT   3           // External Flash Device on 16-bit Bus
#define EXT32BIT   4           // External Flash Device on 32-bit Bus
#define EXTSPI     5           // External Flash Device on SPI

#define SECTOR_NUM 512         // Max Number of Sector Items
#define PAGE_MAX   65536       // Max Page Size for Programming

struct FlashSectors  {
  unsigned long   szSector;    // Sector Size in Bytes
  unsigned long AddrSector;    // Address of Sector
};

#define SECTOR_END 0xFFFFFFFF, 0xFFFFFFFF

struct FlashDevice  {
   unsigned short     Vers;    // Version Number and Architecture
   char       DevName[128];    // Device Name and Description
   unsigned short  DevType;    // Device Type: ONCHIP, EXT8BIT, EXT16BIT, ...
   unsigned long    DevAdr;    // Default Device Start Address
   unsigned long     szDev;    // Total Size of Device
   unsigned long    szPage;    // Programming Page Size
   unsigned long       Res;    // Reserved for future Extension
   unsigned char  valEmpty;    // Content of Erased Memory

   unsigned long    toProg;    // Time Out of Program Page Function
   unsigned long   toErase;    // Time Out of Erase Sector Function

   struct FlashSectors sectors[SECTOR_NUM];
};

#define FLASH_DRV_VERS (0x0100+VERS)   // Driver Version, do not modify!

// Flash Programming Functions (Called by FlashOS)
extern          int  Init        (unsigned long adr,   // Initialize Flash
                                  unsigned long clk,
                                  unsigned long fnc);
extern          int  UnInit      (unsigned long fnc);  // De-initialize Flash
extern          int  BlankCheck  (unsigned long adr,   // Blank Check
                                  unsigned long sz,
                                  unsigned char pat);
extern          int  EraseChip   (void);               // Erase complete Device
extern          int  EraseSector (unsigned long adr);  // Erase Sector Function
extern          int  ProgramPage (unsigned long adr,   // Program Page Function
                                  unsigned long sz,
                                  unsigned char *buf);
extern unsigned long Verify      (unsigned long adr,   // Verify Function
                                  unsigned long sz,
                                  unsigned char *buf);
