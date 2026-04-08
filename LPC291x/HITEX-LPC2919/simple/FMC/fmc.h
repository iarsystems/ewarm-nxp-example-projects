/*****************************************************************************
 *   fmc.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __FMC_H 
#define __FMC_H

#define FMC_INTERRUPT_ENABLED	1

/* Flash Memory range */
#define FLASH_BEGIN_ADDR        0x20000000
#define LPC2919_FLASH_SIZE		0xC0000
#define LPC2917_FLASH_SIZE		0x80000
#define FLASH_LA_SEC_BEGIN_ADDR	0x20000000
#define FLASH_SM_SEC_BEGIN_ADDR	0x20010000

/* For LPC29xx family MCUs, they may have different flash size
and small and large sector number, page number, they all have 
the same page size. */  
#define FMC_PAGE_SIZE 			512
#define FMC_SM_PAGE_NUM			16
#define FMC_LA_PAGE_NUM			128
#define FMC_SM_SECTOR_NUM		8
#define LPC2917_LA_SECTOR_NUM	7
#define LPC2919_LA_SECTOR_NUM	11
#define MAX_SECTOR_NUM			19

#define FMC_BUFFER_SIZE			(FMC_PAGE_SIZE * FMC_SM_PAGE_NUM)			

/*  Sector num		Address range
	11 				0x20000000~20001FFF
 	12 				0x20002000~20003FFF
	13 				0x20004000~20005FFF
	14 				0x20006000~20007FFF
 	15 				0x20008000~20009FFF
	16 				0x2000A000~2000BFFF
	17 				0x2000C000~2000DFFF
 	18 				0x2000E000~2000FFFF
	0 				0x20010000~2001FFFF
	1 				0x20020000~2002FFFF
 	2 				0x20030000~2003FFFF
	3 				0x20040000~2004FFFF
	4 				0x20050000~2005FFFF
 	5 				0x20060000~2006FFFF
	6 				0x20070000~2007FFFF
	7 				0x20080000~2008FFFF
 	8 				0x20090000~2009FFFF
	9 				0x200A0000~200AFFFF
	10				0x200B0000~200BFFFF
*/
#define SECTOR_ADDR_11		0x20000000
#define SECTOR_ADDR_12		0x20002000
#define SECTOR_ADDR_13		0x20004000
#define SECTOR_ADDR_14		0x20006000
#define SECTOR_ADDR_15		0x20008000
#define SECTOR_ADDR_16		0x2000A000
#define SECTOR_ADDR_17		0x2000C000
#define SECTOR_ADDR_18		0x2000E000

#define SECTOR_ADDR_0		0x20010000
#define SECTOR_ADDR_1		0x20020000
#define SECTOR_ADDR_2		0x20030000
#define SECTOR_ADDR_3		0x20040000
#define SECTOR_ADDR_4		0x20050000
#define SECTOR_ADDR_5		0x20060000
#define SECTOR_ADDR_6		0x20070000
#define SECTOR_ADDR_7		0x20080000
#define SECTOR_ADDR_8		0x20090000
#define SECTOR_ADDR_9		0x200A0000
#define SECTOR_ADDR_10		0x200B0000

/* unprotected by writing an even value to its base address */
/* protected by writing an odd value to its base address */
#define UNPROTECT	0x00000000
#define PROTECT		0xFFFFFFFF

#define EN_T		0x00008000

#define EOE			(0x01<<0)	/* End of Erasing */
#define EOB			(0x01<<1)	/* End of Burning */
#define EOM			(0x01<<2)	/* Signature Generation */

#define FMC_CPU_CLK				10000000
#define FMC_PERIPH_CLK			10000000

/* 66Khz+/-20% as the CRA clock feed to FMC */
#define FMC_CRA_CLOCK	66000

/* in mil second */
#define FMC_SEC_ERASE_TIME		100
#define FMC_PAGE_PROG_TIME		1

extern void FMC_Init( void );
extern void FMC_UnprotectSector( DWORD SectorAddr );
extern void FMC_EraseSector( DWORD SectorAddr );
extern void FMC_WritePage( DWORD addrWrite, BYTE* pBuf, DWORD pSize );
extern void FMC_Write( void );
extern void FMC_GetSignature( void );

#endif /* end __FMC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
