/*****************************************************************************
 *   ext_sram:  External SRAM module file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "timer.h"
#include "ext_sram.h"                

/******************************************************************************
** Function name:		SCU_SetPort
**
** Descriptions:		Sets the function of the specified port pins
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void SCU_SetPort (void)
{
   SFSP0_10 = 0x3 + (1 << 2); /* A8 */
   SFSP0_11 = 0x3 + (1 << 2); /* A9 */
   SFSP0_12 = 0x3 + (1 << 2); /* A10 */
   SFSP0_13 = 0x3 + (1 << 2); /* A11 */
   SFSP0_14 = 0x3 + (1 << 2); /* A12 */
   SFSP0_15 = 0x3 + (1 << 2); /* A13 */
   SFSP0_18 = 0x3 + (1 << 2); /* A14 */
   SFSP0_19 = 0x3 + (1 << 2); /* A15 */
   SFSP0_20 = 0x3 + (1 << 2); /* A16 */
   SFSP0_21 = 0x3 + (1 << 2); /* A17 */
   SFSP0_22 = 0x3 + (1 << 2); /* A18 */

   SFSP1_0 = 0x3 + (1 << 2); /* A0 */
   SFSP1_1 = 0x3 + (1 << 2); /* A1 */
   SFSP1_2 = 0x3 + (1 << 2); /* A2 */
   SFSP1_3 = 0x3 + (1 << 2); /* A3 */
   SFSP1_4 = 0x3 + (1 << 2); /* A4 */
   SFSP1_5 = 0x3 + (1 << 2); /* A5 */
   SFSP1_6 = 0x3 + (1 << 2); /* A6 */
   SFSP1_7 = 0x3 + (1 << 2); /* A7 */
   SFSP1_8 = 0x3 + (1 << 2); /* CS0 */
   SFSP1_12 = 0x3 + (1 << 2); /* OEN */
   SFSP1_13 = 0x3 + (1 << 2); /* WEN */
   SFSP1_14 = 0x3 + (1 << 2); /* D0 */
   SFSP1_15 = 0x3 + (1 << 2); /* D1 */
   SFSP1_16 = 0x3 + (1 << 2); /* D2 */
   SFSP1_17 = 0x3 + (1 << 2); /* D3 */
   SFSP1_18 = 0x3 + (1 << 2); /* D4 */
   SFSP1_19 = 0x3 + (1 << 2); /* D5 */
   SFSP1_20 = 0x3 + (1 << 2); /* D6 */
   SFSP1_21 = 0x3 + (1 << 2); /* D7 */
}

/******************************************************************************
** Function name:		Smc_SetConfig
**
** Descriptions:		configure specified memorny bank of SMC unit
**						
**
** parameters:			Memory Width,Burst Mode, write protect (i.e ReadOnly or ReadWrite),
**                      chip select polarity and Read byte lane
** Returned value:		None
** 
******************************************************************************/
void Smc_SetConfig (Smc_MemoryBankNumber_t memoryBankNumber, pSmc_Config_t pSmcConfig)
{
   DWORD smcConfig = 0;
   DWORD regs = 0;

   regs = SMC_BASE_ADDR + SMC_SMBCR_OFFSET + SMC_MEMORY_BANK_OFFSET * memoryBankNumber;

   switch (pSmcConfig->memoryWidth)
   {
      case Smc_8Bit:
         smcConfig |= SMC_SMBCR_MW_8BIT;
         break;
      case Smc_16Bit:
         smcConfig |= SMC_SMBCR_MW_16BIT;
         break;
      case Smc_32Bit:
         smcConfig |= SMC_SMBCR_MW_32BIT;
         break;
      default:
         break;
   }
   if (pSmcConfig->burstMode == Smc_Burst)
   {
      smcConfig |= SMC_SMBCR_BM_MSK;   
   }
   if (pSmcConfig->writeProtect == Smc_ReadOnly)
   {
      smcConfig |= SMC_SMBCR_WP_MSK;
   }
   if (pSmcConfig->csPolarity == Smc_CSActiveHigh)
   {
      smcConfig |= SMC_SMBCR_CSPOL_MSK;
   }
   if (pSmcConfig->readByteLane == Smc_BLSasBLS)
   {
      smcConfig |= SMC_SMBCR_RBLE_MSK;
   }

   *((volatile DWORD *)regs) = smcConfig;
}

/******************************************************************************
** Function name:		Smc_SetTiming
**
** Descriptions:		configure Timing parameters for specified memorny bank of SMC unit
**						
**
** parameters:			idleCycles,wait state for Read,wait state for Write,
**                      Wait states for OEN (output Enable) and	Wait state for for Write Enable
** Returned value:		None
** 
******************************************************************************/
void Smc_SetTiming (Smc_MemoryBankNumber_t memoryBankNumber, pSmc_Timing_t pSmcTiming)
{
   DWORD regs = 0;
   DWORD memoryBankOffset = 0;
   
   memoryBankOffset = SMC_MEMORY_BANK_OFFSET * memoryBankNumber;
    
   regs = SMC_BASE_ADDR + SMC_SMBIDCYR_OFFSET + memoryBankOffset;
   *((volatile DWORD *)regs) = pSmcTiming->idleCycles;
   
   regs = SMC_BASE_ADDR + SMC_SMBWST1R_OFFSET + memoryBankOffset;
   *((volatile DWORD *)regs) = pSmcTiming->waitStatesRead;

   regs = SMC_BASE_ADDR + SMC_SMBWST2R_OFFSET + memoryBankOffset;
   *((volatile DWORD *)regs) = pSmcTiming->waitStatesWrite;
    
   regs = SMC_BASE_ADDR + SMC_SMBWSTOENR_OFFSET + memoryBankOffset;
   *((volatile DWORD *)regs) = pSmcTiming->waitStatesOEN;

   regs = SMC_BASE_ADDR + SMC_SMBWSTWENR_OFFSET + memoryBankOffset;
   *((volatile DWORD *)regs) = pSmcTiming->waitStatesWEN;
}

