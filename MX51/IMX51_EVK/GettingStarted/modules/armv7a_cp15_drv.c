 /*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *
 *    File name   : armv7a_cp15_drv.c
 *    Description : Driver for the CP15 of ARMv7-A
 *
 *    History :
 *    1. Date        : September, 8 2006
 *       Author      : Stanimir Bonev
 *       Description : Driver for the ARM926EJ's CP15
 *
 *    2. Date        : October,  2008
 *       Author      : Stoyan Choynev
 *       Description : Port for ARM1136JF. The driver is backwards compatible
 *                     with ARMv5 or earlier processors.
 *
 *    3. Date        : March,  2012
 *       Author      : Atanas Uzunov
 *       Description : Port for ARMv7-A architecture.
 *                     Added cache maintenance functions.
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "armv7a_cp15_drv.h"

/*************************************************************************
 * Function Name: CP15_GetID
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the ID register
 *
 *************************************************************************/
__arm Int32U CP15_GetID(void)
{
  return(__MRC(15,0,CP15_ID,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetCacheType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the Cache type
 *
 *************************************************************************/
__arm Int32U CP15_GetCacheType(void)
{
  return(__MRC(15,0,CP15_ID,0,1));
}

/*************************************************************************
 * Function Name: CP15_GetTCMType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TCM type
 *
 *************************************************************************/
__arm Int32U CP15_GetTCMType(void)
{
  return(__MRC(15,0,CP15_ID,0,2));
}

/*************************************************************************
 * Function Name: CP15_GetTLBType
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TLB type
 *
 *************************************************************************/
__arm Int32U CP15_GetTLBType(void)
{
  return(__MRC(15,0,CP15_ID,0,3));
}

/*************************************************************************
 * Function Name: CP15_GetSCUBase
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the Cache type
 *
 *************************************************************************/
__arm Int32U CP15_GetSCUBase(void)
{
  return(__MRC(15,4,CP15_C15_REG,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetTTBR0
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TTBR0 register
 *
 *************************************************************************/
__arm Int32U CP15_GetTTBR0(void)
{
  return(__MRC(15,0,CP15_TTB_ADDR,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetTTBR1
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the TTBR1 register
 *
 *************************************************************************/
__arm Int32U CP15_GetTTBR1(void)
{
  return(__MRC(15,0,CP15_TTB_ADDR,0,1));
}

/*************************************************************************
 * Function Name: CP15_GetControl
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the CP15 control register
 *
 *************************************************************************/
__arm Int32U CP15_GetControl(void)
{
  return(__MRC(15,0,CP15_CTRL,0,0));
}

/*************************************************************************
 * Function Name: CP15_SetControl
 * Parameters: Int32U Ctrl
 *
 * Return: Int32U
 *
 * Description: Sets the CP15 control register
 *
 *************************************************************************/
__arm void CP15_SetControl(Int32U Ctrl)
{
register Int32U Val = Ctrl;
  __MCR(15,0,Val,CP15_CTRL,0,0);
  __ISB();
}

/*************************************************************************
 * Function Name: CP15_GetDomains
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the MMU domain access register
 *
 *************************************************************************/
__arm Int32U CP15_GetDomains(void)
{
  return(__MRC(15,0,CP15_DA_CTRL,0,0));
}

/*************************************************************************
 * Function Name: CP15_SetDomains
 * Parameters: Int32U DomainAccess
 *
 * Return: none
 *
 * Description: Sets the MMU domain access register
 *
 *************************************************************************/
__arm void CP15_SetDomains(Int32U DomainAccess)
{
register Int32U Val = DomainAccess;
  __MCR(15,0,Val,CP15_DA_CTRL,0,0);
}

/*************************************************************************
 * Function Name: log2_n_up
 * Parameters: Int32U n
 *
 * Return: Int32S
 *
 * Description: Logarithm at base 2 , rounded up
 *
 *************************************************************************/
static Int32S log2_up(Int32U n)
{
  Int32S log = -1;
  Int32U t = n;
  while(t)
  {
    log++; t >>=1;
  }
  /* if n not power of 2 -> round up*/
  if ( n & (n - 1) ) log++;
  return log;
}

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
__arm void CP15_MaintainDCacheSetWay(Int32U level, Int32U maint)
{
register volatile Int32U Dummy;
register volatile Int32U ccsidr;
Int32U num_sets;
Int32U num_ways;
Int32U shift_way;
Int32U log2_linesize;
Int32U log2_num_ways;

  Dummy = level << 1;
  /* set csselr, select ccsidr register */
  __MCR(15,2,Dummy,CP15_ID,0,0);
  /* get current ccsidr register */
  ccsidr = __MRC(15,1,CP15_ID,0,0);
  num_sets = ((ccsidr & 0x0FFFE000) >> 13) + 1;
  num_ways = ((ccsidr & 0x00001FF8) >> 3) + 1;
  /*  get log2_linesize in bytes */
  log2_linesize = (ccsidr & 0x00000007) + 2 + 2;
  log2_num_ways = log2_up(num_ways);
  shift_way = 32 - log2_num_ways;
  for(int way = num_ways-1; way >= 0; way--)
    for(int set = num_sets-1; set >= 0; set--)
    {
      Dummy = (level << 1) | (set << log2_linesize) | (way << shift_way);
      switch (maint)
      {
        case DCACHE_CLEAN_AND_INVALIDATE:
             __MCR(15,0,Dummy,CP15_CACHE_OPR,14,2);
             break;

        case DCACHE_INVALIDATE:
             __MCR(15,0,Dummy,CP15_CACHE_OPR,6,2);
             break;

        case DCACHE_CLEAN:
             __MCR(15,0,Dummy,CP15_CACHE_OPR,10,2);
             break;
      }
    }
    __DSB();
    __ISB();
}

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
__arm void CP15_MaintainAllDCache(Int32U oper)
{
register volatile Int32U clidr;
Int32U cache_type;
  clidr =  __MRC(15,1,CP15_ID,0,1);
  for(Int32U i = 0; i<7; i++)
  {
    cache_type = (clidr >> i*3) & 0x7UL;
    if ((cache_type >= 2) && (cache_type <= 4))
    {
      CP15_MaintainDCacheSetWay(i,oper);
    }
  }
}

/*************************************************************************
 * Function Name: CP15_InvalidateICache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the whole instruction cache
 *
 *************************************************************************/
__arm void CP15_InvalidateICache(void)
{
register volatile Int32U Dummy = 0;
  __MCR(15,0,Dummy,CP15_CACHE_OPR,5,0);
}

/*************************************************************************
 * Function Name: CP15_InvalidateICache_SMP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the whole instruction cache - Inner Shareable
 *
 *************************************************************************/
__arm void CP15_InvalidateICache_SMP(void)
{
register volatile Int32U Dummy = 0;
  __MCR(15,0,Dummy,CP15_CACHE_OPR,1,0);
}

/*************************************************************************
 * Function Name: CP15_InvalidateBranchPredictors
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the branch prediction array
 *
 *************************************************************************/
__arm void CP15_InvalidateBranchPredictors(void)
{
register volatile Int32U Dummy = 0;
  __MCR(15,0,Dummy,CP15_CACHE_OPR,5,6);
  __ISB();
}

/*************************************************************************
 * Function Name: CP15_InvalidateBranchPredictors_SMP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate the branch prediction array Inner Shareable
 *
 *************************************************************************/
__arm void CP15_InvalidateBranchPredictors_SMP(void)
{
register volatile Int32U Dummy = 0;
  __MCR(15,0,Dummy,CP15_CACHE_OPR,1,6);
  __ISB();
}

/*************************************************************************
 * Function Name: CP15_InvalidateAllTLBs
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidate all TLBs
 *
 *************************************************************************/
__arm void CP15_InvalidateAllTLBs(void)
{
register volatile Int32U Dummy = 0;
  /* Invalidate entire unified TLB*/
  __MCR(15,0,Dummy,CP15_TLB_OPR,7,0);
  /* Invalidate entire data TLB*/
  __MCR(15,0,Dummy,CP15_TLB_OPR,6,0);
  /* Invalidate entire instruction TLB*/
  __MCR(15,0,Dummy,CP15_TLB_OPR,5,0);
  __DSB();
  __ISB();
}

/*************************************************************************
 * Function Name: CP15_SetTTBR0
 * Parameters: pInt32U pTtb
 *
 * Return: none
 *
 * Description: Sets the CP15 TTBR0 base address register
 *
 *************************************************************************/
__arm void CP15_SetTTBR0(pInt32U pTtb)
{
register volatile Int32U Val = (Int32U)pTtb;
  __MCR(15,0,Val,CP15_TTB_ADDR,0,0);
}

/*************************************************************************
 * Function Name: CP15_SetTTBR1
 * Parameters: pInt32U pTtb
 *
 * Return: none
 *
 * Description: Sets the CP15 TTBR1 base address register
 *
 *************************************************************************/
__arm void CP15_SetTTBR1(pInt32U pTtb)
{
register volatile Int32U Val = (Int32U)pTtb;
  __MCR(15,0,Val,CP15_TTB_ADDR,0,1);
}

/*************************************************************************
 * Function Name: CP15_GetDFaultStatus
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the data fault status register
 *
 *************************************************************************/
__arm Int32U CP15_GetDFaultStatus(void)
{
  return(__MRC(15,0,CP15_FAULT_STAT,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetIFaultStatus
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the instruction fault status register
 *
 *************************************************************************/
__arm Int32U CP15_GetIFaultStatus(void)
{
  return(__MRC(15,0,CP15_FAULT_STAT,0,1));
}

/*************************************************************************
 * Function Name: CP15_GetDFaultAddress
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the data fault address register
 *
 *************************************************************************/
__arm Int32U CP15_GetDFaultAddress(void)
{
  return(__MRC(15,0,CP15_FAULT_ADDR,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetIFaultAddress
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the instruction fault address register
 *
 *************************************************************************/
__arm Int32U CP15_GetIFaultAddress(void)
{
  return(__MRC(15,0,CP15_FAULT_ADDR,0,1));
}

/*************************************************************************
 * Function Name: CP15_GetFcsePid
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the MMU Process identifier FCSE PID register (deprecated)
 *
 *************************************************************************/
__arm Int32U CP15_GetFcsePid(void)
{
  return(__MRC(15,0,CP15_PROCESS_ID,0,0));
}

/*************************************************************************
 * Function Name: CP15_GetContextID
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Returns the context identifier register
 *
 *************************************************************************/
__arm Int32U CP15_GetContextID(void)
{
  return(__MRC(15,0,CP15_PROCESS_ID,0,1));
}

/*************************************************************************
 * Function Name: CP15_SetFcsePid
 * Parameters: Int32U FcsePid
 *
 * Return: none
 *
 * Description: Sets the MMU Process identifier FCSE PID register (deprecated)
 *
 *************************************************************************/
__arm void CP15_SetFcsePid(Int32U FcsePid)
{
register Int32U Val = FcsePid;
  __MCR(15,0,Val,CP15_PROCESS_ID,0,0);
}

/*************************************************************************
 * Function Name: CP15_SetContextID
 * Parameters: Int32U
 *
 * Return: none
 *
 * Description: Sets the context identifier register
 *
 *************************************************************************/
__arm void CP15_SetContextID(Int32U Context)
{
register Int32U Val = Context;
  __DSB();
  __MCR(15,0,Val,CP15_PROCESS_ID,0,1);
  __ISB();
}

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
                        const TtTableBlock_t * pTtTB)
{
Int32U i, pa, pa_inc, va_ind;
pInt32U pTtb;
TableType_t TableType;
  while(1)
  {
    TableType = pTtTB->TableType;
    switch(TableType)
    {
    case TableL1:
      pTtb = pTtTB->TableAddr;
      if((Int32U)pTtb & L1_ENTRIES_NUMB-1)
      {
        return(FALSE);
      }
      pa_inc = 0x100000;
      pa = L1_ENTRIES_NUMB;
      break;
    case TableL2_PageTable:
      pTtb = pTtTB->TableAddr;
      if((Int32U)pTtb & L2_ENTRIES_NUMB-1)
      {
        return(FALSE);
      }
      pa_inc = 0x1000;
      pa = L2_ENTRIES_NUMB;
      break;
    default:
      return(TRUE);
    }

    // Clear the entire Translation Table This results in LxD_TYPE_FAULT
    // being the default for any uninitialized entries.
    for(i = 0; i < pa; ++i)
    {
      *(pTtb+i) = TT_ENTRY_INVALID;
    }

    // Build the translation table from user provided pTtSectionBlock_t array
    while(pTtSB->NubrOfSections != 0)
    {
Int32U Entrys = pTtSB->NubrOfSections;
Int32U Data =  pTtSB->Entry.Data;
      pa = pTtSB->PhysAddr;

      switch(TableType)
      {
      case TableL1:
        va_ind = (pTtSB->VirtAddr >> 20) & (L1_ENTRIES_NUMB-1);

        if((va_ind + Entrys) > L1_ENTRIES_NUMB)
        {
          return(FALSE);
        }
        break;
      case TableL2_PageTable:
        va_ind = (pTtSB->VirtAddr >> 12) & (L2_ENTRIES_NUMB-1);
        if((va_ind + Entrys) > L2_ENTRIES_NUMB)
        {
          return(FALSE);
        }
        break;
      }
      for(i = 0; i < Entrys; ++i, ++va_ind)
      {
        switch(TableType)
        {
        case TableL1:
          switch(pTtSB->Entry.Type)
          {
          case TtL1PageTable:
            *(pTtb+va_ind) |= Data | (pa & TTL1_PT_PADDR_MASK);
            break;
          case TtL1Section:
            *(pTtb+va_ind) |= Data | (pa & TTL1_SECTION_PADDR_MASK);
            break;
          case TtL1SuperSection:
              *(pTtb+va_ind) |= Data | (pa & TTL1_S_SECTION_PADDR_MASK);
            break;
          default:
            return(FALSE);
          }
          break;
        case TableL2_PageTable:
          switch(pTtSB->Entry.Type)
          {
          case TtL2LargePage:
            *(pTtb+va_ind) |= Data | (pa & TTL2_LP_PADDR_MASK);
            break;
          case TtL2SmallPage:
            *(pTtb+va_ind) |= Data | (pa & TTL2_SP_PADDR_MASK);
            break;
          default:
            return(FALSE);
          }
          break;
        }
        pa += pa_inc;
      }
      ++pTtSB;
    }
    ++pTtSB;
    ++pTtTB;
  }
}

/*************************************************************************
 * Function Name: CP15_Mmu
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the MMU
 *
 *************************************************************************/
void CP15_Mmu(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_M;
  }
  else
  {
    Val &= ~(CP15_CTRL_M | CP15_CTRL_C);
  }
  CP15_SetControl(Val);
}

/*************************************************************************
 * Function Name: CP15_AllCache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable both data and instruction cache
 *
 *************************************************************************/
void CP15_AllCache(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_M | CP15_CTRL_C | CP15_CTRL_I;
  }
  else
  {
    Val &= ~(CP15_CTRL_C | CP15_CTRL_I);
  }
  CP15_SetControl(Val);
}

/*************************************************************************
 * Function Name: CP15_InvalidateDCache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Invalidates the data cache
 *
 *************************************************************************/
void CP15_InvalidateDCache(void)
{
  CP15_MaintainAllDCache(DCACHE_INVALIDATE);
}

/*************************************************************************
 * Function Name: CP15_FlushDCache
 * Parameters: none
 *
 * Return: none
 *
 * Description: Flushes the data cache
 *
 *************************************************************************/
void CP15_FlushDCache(void)
{
  CP15_MaintainAllDCache(DCACHE_CLEAN);
}

/*************************************************************************
 * Function Name: CP15_ICache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the instruction cache
 *
 *************************************************************************/
void CP15_ICache(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_I;
  }
  else
  {
    Val &= ~CP15_CTRL_I;
  }
  CP15_SetControl(Val);
}

/*************************************************************************
 * Function Name: CP15_DCache
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the data cache
 *
 *************************************************************************/
void CP15_DCache(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_M | CP15_CTRL_C;
  }
  else
  {
    Val &= ~CP15_CTRL_C;
  }
  CP15_SetControl(Val);
}

/*************************************************************************
 * Function Name: CP15_BranchPrediction
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable the branch (program flow) prediction.
 *
 *************************************************************************/
void CP15_BranchPrediction(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_Z;
  }
  else
  {
    Val &= ~CP15_CTRL_Z;
  }
  CP15_SetControl(Val);
}

/*************************************************************************
 * Function Name: CP15_GetVectorBase
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Get Vector Base Register (VBAR)
 *
 *************************************************************************/
__arm Int32U CP15_GetVectorBase(void)
{
  return(__MRC(15,0,CP15_VBAR,0,0));
}

/*************************************************************************
 * Function Name: CP15_SetVectorBase
 * Parameters: Int32U
 *
 * Return: none
 *
 * Description: Set Vector Base Register (VBAR)
 *
 *************************************************************************/
__arm void CP15_SetVectorBase(Int32U vector)
{
register volatile Int32U Val = vector;
  __MCR(15,0,Val,CP15_VBAR,0,0);
}

/*************************************************************************
 * Function Name: CP15_SetHighVectors
 * Parameters: Boolean
 *
 * Return: none
 *
 * Description: Select High or Low vectors base in CP15 control register
 *
 *************************************************************************/
__arm void CP15_SetHighVectors(Boolean Enable)
{
Int32U Val = CP15_GetControl();
  if(Enable)
  {
    Val |= CP15_CTRL_V;
  }
  else
  {
    Val &= ~CP15_CTRL_V;
  }
  CP15_SetControl(Val);
}
