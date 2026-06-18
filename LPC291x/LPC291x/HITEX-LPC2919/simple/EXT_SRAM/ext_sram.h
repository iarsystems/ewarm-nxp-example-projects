/*****************************************************************************
 *   ext_sram.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __EXT_SRAM_H 
#define __EXT_SRAM_H

#define SRAM_BASE_ADDR         0x40000000
#define SRAM_SIZE              0x00080000

#define SMC_SMBIDCYR_OFFSET    0x00000000
#define SMC_SMBWST1R_OFFSET    0x00000004
#define SMC_SMBWST2R_OFFSET    0x00000008
#define SMC_SMBWSTOENR_OFFSET  0x0000000C
#define SMC_SMBWSTWENR_OFFSET  0x00000010
#define SMC_SMBCR_OFFSET       0x00000014
#define SMC_MEMORY_BANK_OFFSET 0x0000001C

#define SMC_SMBCR_MW_8BIT      0x00000000
#define SMC_SMBCR_MW_16BIT     0x00000040
#define SMC_SMBCR_MW_32BIT     0x00000080

#define SMC_SMBCR_BM_MSK       0x00000020
#define SMC_SMBCR_WP_MSK       0x00000010
#define SMC_SMBCR_CSPOL_MSK    0x00000008
#define SMC_SMBCR_RBLE_MSK     0x00000001

typedef BYTE Smc_MemoryBankNumber_t, *pSmc_MemoryBankNumber_t;

typedef enum Smc_MemoryWidth
{
   Smc_8Bit,
   Smc_16Bit,
   Smc_32Bit
} Smc_MemoryWidth_t, *pSmc_MemoryWidth_t;

typedef enum Smc_BurstMode
{
   Smc_NonBurst,
   Smc_Burst
} Smc_BurstMode_t, *pSmc_BurstMode_t;

typedef enum Smc_CSPolarity
{
   Smc_CSActiveLow,
   Smc_CSActiveHigh
} Smc_CSPolarity_t, *pSmc_CSPolarity_t;

typedef enum Smc_ReadByteLane
{
   Smc_BLSasWE,
   Smc_BLSasBLS
} Smc_ReadByteLane_t, *pSmc_ReadByteLane_t;

typedef enum Smc_AccessAllowance
{
   Smc_ReadWrite,
   Smc_ReadOnly
} Smc_AccessAllowance_t, *pSmc_AccessAllowance_t;

typedef struct Smc_Config
{
   Smc_MemoryWidth_t     memoryWidth;
   Smc_BurstMode_t       burstMode;
   Smc_AccessAllowance_t writeProtect;
   Smc_CSPolarity_t      csPolarity;
   Smc_ReadByteLane_t    readByteLane;
} Smc_Config_t, *pSmc_Config_t;

typedef struct Timing
{
   BYTE idleCycles;
   BYTE waitStatesRead;
   BYTE waitStatesWrite;
   BYTE waitStatesOEN;
   BYTE waitStatesWEN;
} Smc_Timing_t, *pSmc_Timing_t;

extern void SCU_SetPort (void);
extern void Smc_SetConfig (Smc_MemoryBankNumber_t memoryBankNumber, pSmc_Config_t pSmcConfig);
extern void Smc_SetTiming (Smc_MemoryBankNumber_t memoryBankNumber, pSmc_Timing_t pSmcTiming);

#endif /* end __EXT_SRAM_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
