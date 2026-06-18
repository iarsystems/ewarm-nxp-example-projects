/***********************************************************************
 * $Id: ea3250_nand_boot.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Embedded Artists LPC3250 NAND device definitions
 *
 * Description:
 *     This file contains board specific information such as the
 *     NAND device commands, geometry structures, etc.
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

#ifndef EA3250_NAND_BOOT_H
#define EA3250_NAND_BOOT_H

#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* The bad block marker provided by the vendor is in the spare data
   area at byte 1 (offset 0). Page size is 2048 bytes  */
#define NAND_BADBLOCK_OFFS     (NAND_PAGE_SIZE + 0)

/* LPC3250 MLC controller index implementation of NAND bad block - to
   prevent erasures of data blocks that use the vendor implemented
   bad block index, the software checks BOTH the vendor and LPC3250
   bad block indices for markers. */
#define NAND_LPCBADBLOCK_OFFS  NAND_BADBLOCK_OFFS


/* Good block marker flag */
#define NAND_GOOD_BLOCK_MARKER 0xFF



/***********************************************************************
 * NAND support functions
 **********************************************************************/

STATUS nand_boot_init(void);
void nand_boot_deinit(void);
STATUS nand_boot_store_app1(UNS_8* pBuffer, UNS_32 size);
STATUS nand_boot_store_app2(UNS_8* pBuffer, UNS_32 size);


#ifdef __cplusplus
}
#endif

#endif /* EA3250_NAND_BOOT_H */
