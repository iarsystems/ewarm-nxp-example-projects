/**********************************************************************
* $Id: FlashDev.c 28 2014-01-27 14:45:13Z danielru $		FlashDev.c			2011-06-02
*//**
* @file		FlashDev.c
* @brief	Flash Programming Functions adapted
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
#include "FlashOS.H"        // FlashOS Structures


struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "LHF00L28 Flash",         // Device Name
   EXT16BIT,                   // Device Type
   0x1C000000,                   // Device Start Address
   0x200000,                   // Device Size in Bytes (2MB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   4800,                        // Program Page Timeout 100 mSec
   8000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x20000, 0x000000,          // Sector Size 128kB (1 Sector)
   0x10000, 0x1E0000,          // Sector Size  8kB (2 Sectors)
   0x02000, 0x1F0000,          // Sector Size 8kB (8 Sector)
//   0x10000, 0x010000,          // Sector Size 64kB (31 Sectors)
   SECTOR_END
};
