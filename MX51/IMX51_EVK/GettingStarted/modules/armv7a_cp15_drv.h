/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : armv7a_cp15_drv.h
 *    Description : Definitions of a driver for the CP15 of ARMv7-A
 *
 *    History :
 *    1. Date        : September, 8 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    2. Date        : October,  2008
 *       Author      : Stoyan Choynev
 *       Description : Port for ARM1136JF. The driver is backwards compatible with ARMv5 or earlier
 *                     processors
 *
 *    3. Date        : March,  2012
 *       Author      : Atanas Uzunov
 *       Description : Port for ARMv7-A architecture.
 *                     Added cache maintenance functions.
 *
 *    $Revision: 16 $
 **************************************************************************/
#include <intrinsics.h>
#include "arm_comm.h"

#ifndef __ARMV7A_CP15_DRV_H
#define __ARMV7A_CP15_DRV_H

#define L1_ENTRIES_NUMB       4096
#define L2_ENTRIES_NUMB        256

#define DCACHE_CLEAN_AND_INVALIDATE  1
#define DCACHE_INVALIDATE            2
#define DCACHE_CLEAN                 3

#define TSB_INVALID { 0, 0, 0, 0 }
#define TTB_INVALID { 0, TableInvalid }

#define L1_PAGE_TABLE_ENTRY(Numb, VirtAddr ,PhAddr, Domain, NS) \
  { Numb, VirtAddr, PhAddr, \
  ((Domain << 5) | (NS << 3) | \
    TtL1PageTable)}

#define L1_SECTION_ENTRY(Numb, VirtAddr ,PhAddr, NS, nG, S, AP2, TEX, AP01, Domain, XN, C, B) \
  { Numb, VirtAddr, PhAddr, \
  ((NS << 19) | (nG << 17) | (S << 16) | (AP2 << 15) | (TEX << 12) | (AP01 << 10) | (Domain << 5) | (XN << 4) | (C << 3) | (B << 2) | \
    TtL1Section)}

#define L1_SUPERSECTION_ENTRY(Numb, VirtAddr, PhAddr, ExtBaseAddr, NS, nG, S, AP2, TEX, AP01, Domain, XN, C, B) \
  { Numb*16, VirtAddr, PhAddr, \
  (((ExtBaseAddr&0x0FUL) << 20) | (((ExtBaseAddr&0xF0UL)>>4) << 5) | (NS << 19) | (nG << 17) | (S << 16) | (AP2 << 15) | (TEX << 12) | (AP01 << 10) | (Domain << 5) | (XN << 4) | (C << 3) | (B << 2) | \
    TtL1SuperSection)}

#define L2_LARGE_PAGE_ENTRY(Numb, VirtAddr ,PhAddr, XN, TEX, nG, S, AP2, AP01, C, B) \
  { Numb*16, VirtAddr, PhAddr, \
  ((XN << 15) | (TEX << 12) | (nG << 11) | (S << 10) | (AP2 << 9) | (AP01 << 4) | (C << 3) | (B << 2) | \
    TtL2LargePage)}

#define L2_SMALL_PAGE_ENTRY(Numb, VirtAddr ,PhAddr, XN, TEX, nG, S, AP2, AP01, C, B) \
  { Numb, VirtAddr, PhAddr, \
  ((nG << 11) | (S << 10) | (AP2 << 9) | (TEX<<6) | (AP01 << 4) | (C << 3) | (B << 2) |  (XN << 0) | \
    TtL2SmallPage)}

// CP15 Registers
// ID register
#define CP15_ID           0

// Control register
#define CP15_CTRL         1
// CP15 Control register bits
#define CP15_CTRL_M      (1UL <<  0)    // MMU enable/disable
#define CP15_CTRL_A      (1UL <<  1)    // Alignment fault enable/disable
#define CP15_CTRL_C      (1UL <<  2)    // DCache enable/disable
#define CP15_CTRL_Z      (1UL << 11)    // Program flow prediction
#define CP15_CTRL_I      (1UL << 12)    // ICache enable/disable
#define CP15_CTRL_V      (1UL << 13)    // Location of exception vectors
#define CP15_CTRL_EE     (1UL << 25)    // CPSR E bit on exception
#define CP15_CTRL_NMFI   (1UL << 27)    // FIQ enable bit (1 - FIQ cannot be masked) READ-ONLY
#define CP15_CTRL_TRE    (1UL << 28)    // TEX remap functionality bit. (TEX enabled/disabled)
#define CP15_CTRL_AFE    (1UL << 29)    // Access Flag Enable bit.
#define CP15_CTRL_TE     (1UL << 30)    // Thumb Exception enable bit.

// Translation table base address (alignment 16KB)
#define CP15_TTB_ADDR     2

// Domain access control register
#define CP15_DA_CTRL      3

#define CP15_DA_CTRL_D0(Val)  ((Val & 0x3) <<  0)
#define CP15_DA_CTRL_D1(Val)  ((Val & 0x3) <<  2)
#define CP15_DA_CTRL_D2(Val)  ((Val & 0x3) <<  4)
#define CP15_DA_CTRL_D3(Val)  ((Val & 0x3) <<  6)
#define CP15_DA_CTRL_D4(Val)  ((Val & 0x3) <<  8)
#define CP15_DA_CTRL_D5(Val)  ((Val & 0x3) << 10)
#define CP15_DA_CTRL_D6(Val)  ((Val & 0x3) << 12)
#define CP15_DA_CTRL_D7(Val)  ((Val & 0x3) << 14)
#define CP15_DA_CTRL_D8(Val)  ((Val & 0x3) << 16)
#define CP15_DA_CTRL_D9(Val)  ((Val & 0x3) << 18)
#define CP15_DA_CTRL_D10(Val) ((Val & 0x3) << 20)
#define CP15_DA_CTRL_D11(Val) ((Val & 0x3) << 22)
#define CP15_DA_CTRL_D12(Val) ((Val & 0x3) << 24)
#define CP15_DA_CTRL_D13(Val) ((Val & 0x3) << 25)
#define CP15_DA_CTRL_D14(Val) ((Val & 0x3) << 28)
#define CP15_DA_CTRL_D15(Val) ((Val & 0x3) << 30)

// CP15 fault status register
#define CP15_FAULT_STAT   5

// CP15 fault address register
#define CP15_FAULT_ADDR   6

// CP15 Cache operations
#define CP15_CACHE_OPR    7

// CP15 TLB operation
#define CP15_TLB_OPR      8

// CP15 Cache lockdown
#define CP15_CACHE_LD     9

// CP15 TLB lockdown
#define CP15_TLB_LD       10

// CP15 VBAR
#define CP15_VBAR         12

// CP15 Process identifier register
#define CP15_PROCESS_ID   13

// CP15 c15 register
#define CP15_C15_REG      15

typedef enum {
  DomainNoAccess = 0, DomainClient, DomainManager = 3,
} MmuDomainType_t;

typedef enum
{
  TtL1Invalid = 0, TtL1PageTable, TtL1Section, TtL1SuperSection = 0x40002,
} TtL1EntryType_t;

typedef enum
{
  TtL2Invalid = 0, TtL2LargePage, TtL2SmallPage,
} TtL2EntryType_t;

typedef enum
{
  TableInvalid = 0, TableL1, TableL2_PageTable,
} TableType_t;

typedef union _TtEntry_t
{
  Int32U Data;
  struct
  {
    Int32U Type     : 2;
    Int32U          : 3;
    Int32U Domain   : 4;
    Int32U          :23;
  };
} TtEntry_t, *pTtEntry_t;

typedef struct _TtSectionBlock_t
{
  Int32U NubrOfSections;
  Int32U VirtAddr;
  Int32U PhysAddr;
  TtEntry_t Entry;
} TtSectionBlock_t, * pTtSectionBlock_t;

typedef struct _TtTableBlock_t
{
  pInt32U     TableAddr;
  TableType_t TableType;
} TtTableBlock_t, * pTtTableBlock_t;

#define TT_ENTRY_INVALID          0

#define TTL1_SECTION_PADDR_MASK   0xFFF00000
#define TTL1_S_SECTION_PADDR_MASK 0xFF000000
#define TTL1_S_SECTION_EXT35_32_PADDR_MASK 0x00F00000
#define TTL1_S_SECTION_EXT39_36_PADDR_MASK 0x000001E0
#define TTL1_PT_PADDR_MASK        0xFFFFFC00

#define TTL2_LP_PADDR_MASK        0xFFFF0000
#define TTL2_SP_PADDR_MASK        0xFFFFF000

/*************************************************************************
 * Function Name: CP15_GetID
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the ID register
 *
 *************************************************************************/
__arm Int32U CP15_GetID(void);

/*************************************************************************
 * Function Name: CP15_GetCacheType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the Cache type
 *
 *************************************************************************/
__arm Int32U CP15_GetCacheType(void);

/*************************************************************************
 * Function Name: CP15_GetTCMType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TCM type
 *
 *************************************************************************/
__arm Int32U CP15_GetTCMType(void);

/*************************************************************************
 * Function Name: CP15_GetTLBType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TLB type
 *
 *************************************************************************/
__arm Int32U CP15_GetTLBType(void);

/*************************************************************************
 * Function Name: CP15_GetSCUBase
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the Cache type
 *
 *************************************************************************/
__arm Int32U CP15_GetSCUBase(void);

/*************************************************************************
 * Function Name: CP15_GetTTBR0
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TTBR0 register
 *
 *************************************************************************/
__arm Int32U CP15_GetTTBR0(void);

/*************************************************************************
 * Function Name: CP15_GetTTBR1
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TTBR1 register
 *
 *************************************************************************/
__arm Int32U CP15_GetTTBR1(void);

/*************************************************************************
 * Function Name: CP15_GetControl
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the CP15 control register
 *
 *************************************************************************/
__arm Int32U CP15_GetControl(void);

/*************************************************************************
 * Function Name: CP15_SetControl
 * Parameters: Int32U Ctrl
 *
 * Return: Int32U
 *
 * Description: Sets the CP15 control register
 *
 *************************************************************************/
__arm void CP15_SetControl(Int32U Ctrl);

/*************************************************************************
 * Function Name: CP15_GetDomains
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the MMU domain access register
 *
 *************************************************************************/
__arm Int32U CP15_GetDomains(void);

/*************************************************************************
 * Function Name: CP15_SetDomains
 * Parameters: Int32U DomainAccess
 *
 * Return: none
 *
 * Description: Sets the MMU domain access register
 *
 *************************************************************************/
__arm void CP15_SetDomains(Int32U DomainAccess);

/*************************************************************************
 * Function Name: CP15_MaintainDCacheSetWay
 * Parameters: Int32U level - level of cache
 *             Int32U maint - maintenance type
 *
 * Return: none
 *
 * Description: Maintain all data cache lines by Set/Way
 *
 *************************************************************************/
__arm void CP15_MaintainDCacheSetWay(Int32U level, Int32U maint);

/*************************************************************************
 * Function Name: CP15_MaintainAllDCache
 * Parameters: Int32U oper - type of maintenance, one of:
 *                          DCACHE_CLEAN_AND_INVALIDATE
 *                          DCACHE_INVALIDATE
 *                          DCACHE_CLEAN
 *
 * Return: none
 *
 * Description: Maintenance of all data cache
 *
 *************************************************************************/
__arm void CP15_MaintainAllDCache(Int32U oper);

/*************************************************************************
 * Function Name: CP15_InvalidateICache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the whole instruction cache
 *
 *************************************************************************/
__arm void CP15_InvalidateICache(void);

/*************************************************************************
 * Function Name: CP15_InvalidateICache_SMP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the whole instruction cache - Inner Shareable
 *
 *************************************************************************/
__arm void CP15_InvalidateICache_SMP(void);

/*************************************************************************
 * Function Name: CP15_InvalidateBranchPredictors
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the branch prediction array
 *
 *************************************************************************/
__arm void CP15_InvalidateBranchPredictors(void);

/*************************************************************************
 * Function Name: CP15_InvalidateBranchPredictors_SMP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the branch prediction array Inner Shareable
 *
 *************************************************************************/
__arm void CP15_InvalidateBranchPredictors_SMP(void);

/*************************************************************************
 * Function Name: CP15_InvalidateAllTLBs
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate all TLBs
 *
 *************************************************************************/
__arm void CP15_InvalidateAllTLBs(void);

/*************************************************************************
 * Function Name: CP15_SetTTBR0
 * Parameters: pInt32U pTtb
 *
 * Return: none
 *
 * Description: Sets the CP15 TTBR0 base address register
 *
 *************************************************************************/
__arm void CP15_SetTTBR0(pInt32U pTtb);

/*************************************************************************
 * Function Name: CP15_SetTTBR1
 * Parameters: pInt32U pTtb
 *
 * Return: none
 *
 * Description: Sets the CP15 TTBR1 base address register
 *
 *************************************************************************/
__arm void CP15_SetTTBR1(pInt32U pTtb);

/*************************************************************************
 * Function Name: CP15_GetDFaultStatus
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the data fault status register
 *
 *************************************************************************/
__arm Int32U CP15_GetDFaultStatus(void);

/*************************************************************************
 * Function Name: CP15_GetIFaultStatus
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the instruction fault status register
 *
 *************************************************************************/
__arm Int32U CP15_GetIFaultStatus(void);

/*************************************************************************
 * Function Name: CP15_GetDFaultAddress
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the data fault address register
 *
 *************************************************************************/
__arm Int32U CP15_GetDFaultAddress(void);

/*************************************************************************
 * Function Name: CP15_GetIFaultAddress
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the instruction fault address register
 *
 *************************************************************************/
__arm Int32U CP15_GetIFaultAddress(void);

/*************************************************************************
 * Function Name: CP15_GetFcsePid
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the MMU Process identifier FCSE PID register (deprecated)
 *
 *************************************************************************/
__arm Int32U CP15_GetFcsePid(void);

/*************************************************************************
 * Function Name: CP15_GetContextID
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the context identifier register
 *
 *************************************************************************/
__arm Int32U CP15_GetContextID(void);

/*************************************************************************
 * Function Name: CP15_SetFcsePid
 * Parameters: Int32U FcsePid
 *
 * Return: none
 *
 * Description: Sets the MMU Process identifier FCSE PID register (deprecated)
 *
 *************************************************************************/
__arm void CP15_SetFcsePid(Int32U FcsePid);

/*************************************************************************
 * Function Name: CP15_SetContextID
 * Parameters: Int32U
 *
 * Return: none
 *
 * Description: Sets the context identifier register
 *
 *************************************************************************/
__arm void CP15_SetContextID(Int32U Context);

/*************************************************************************
 * Function Name: CP15_InitMmuTtb
 * Parameters: pTtSectionBlock_t pTtSB, pTtTableBlock_t pTtTB
 *
 * Return: Boolean
 *
 *  Returns error if MMU is enabled or if target
 * Translation Table address is not 16K aligned. Clear the
 * Translation Table area. Build the Translation Table from the
 * initialization data in the Section Block array. Return no error.
 *
 * Description:  Initializes the MMU tables.
 *
 *************************************************************************/
Boolean CP15_InitMmuTtb(const TtSectionBlock_t * pTtSB,
                        const TtTableBlock_t * pTtTB);

/*************************************************************************
 * Function Name: CP15_Mmu
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the MMU
 *
 *************************************************************************/
void CP15_Mmu(Boolean Enable);

/*************************************************************************
 * Function Name: CP15_AllCache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable both data and instruction cache
 *
 *************************************************************************/
void CP15_AllCache(Boolean Enable);

/*************************************************************************
 * Function Name: CP15_InvalidateDCache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidates the data cache
 *
 *************************************************************************/
void CP15_InvalidateDCache(void);

/*************************************************************************
 * Function Name: CP15_FlushDCache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Flushes the data cache
 *
 *************************************************************************/
void CP15_FlushDCache(void);

/*************************************************************************
 * Function Name: CP15_ICache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the instruction cache
 *
 *************************************************************************/
void CP15_ICache(Boolean Enable);

/*************************************************************************
 * Function Name: CP15_DCache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the data cache
 *
 *************************************************************************/
void CP15_DCache(Boolean Enable);

/*************************************************************************
 * Function Name: CP15_BranchPrediction
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the branch (program flow) prediction.
 *
 *************************************************************************/
void CP15_BranchPrediction(Boolean Enable);

/*************************************************************************
 * Function Name: CP15_GetVectorBase
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Get Vector Base Register (VBAR)
 *
 *************************************************************************/
__arm Int32U CP15_GetVectorBase(void);

/*************************************************************************
 * Function Name: CP15_SetVectorBase
 * Parameters: Int32U
 *
 * Return: none
 *
 * Description: Set Vector Base Register (VBAR)
 *
 *************************************************************************/
__arm void CP15_SetVectorBase(Int32U vector);

/*************************************************************************
 * Function Name: CP15_SetHighVectors
 * Parameters: Boolean
 *
 * Return: none
 *
 * Description: Select High or Low vectors base in CP15 control register
 *
 *************************************************************************/
__arm void CP15_SetHighVectors(Boolean Enable);

#endif // __ARMV7A_CP15_DRV_H
