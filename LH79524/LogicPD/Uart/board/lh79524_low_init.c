/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh79524_low_init.c
 *    Description : Low level init driver
 *
 *    History :
 *    1. Date        : May, 10 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4174 $
 **************************************************************************/
#include <limits.h>
#include <NXP/iolh79524.h>
#include "board.h"
#include "ttbl.h"
#include "arm720t_cp15_drv.h"

#pragma section = "RAMFUNC"
#pragma section = "RAMFUNC_init"

#define EMC_CLOCK(hclk) \
  (hclk/(((HCLK2SDCLK)>>8)+1))

#define EMC_DIV(trp,hclk) \
  (((unsigned long long)trp * (unsigned long long)EMC_CLOCK(hclk))/1000000000LL)

#define EMC_RFR_DIV(trp,hclk) \
  (((unsigned long long)trp * (unsigned long long)EMC_CLOCK(hclk))/16000000000LL)

/*************************************************************************
 * Function Name: InitEMC
 * Parameters: none
 *
 * Return: int
 *
 * Description: This function is used for EMC initialization
 *
 *************************************************************************/
#pragma location="RAMFUNC"
static __ramfunc void InitEMC (void)
{
  // Disable EMC
  EMC_CONTROL_bit.ENABLE = 0;
  // HCLK to SDCLK Ratio 1:1
  EMC_CONFIG_bit.CLK = HCLK2SDCLK;
  // nCS1 - Flash
  SCONFIG1_bit.PM = 1;  // Asynchronous page mode enabled
  SCONFIG1_bit.P  = 1;  // Write protected
  SCONFIG1_bit.B  = 1;  // Read and write buffer disabled
  // Wait Output Enable
  SWAITOEN1_bit.WAITOEN = EMC_DIV(FLASH_WAIT_OEN,HCLK_FREQ);
  // Read Access Delay
  SWAITRD1_bit.WAITRD = EMC_DIV(FLASH_WAIT_RD,HCLK_FREQ);
  // Asynchronous Page Mode Delay
  SWAITPAGE1_bit.WAITPAGE = EMC_DIV(FLASH_WAIT_PAGE,HCLK_FREQ);

  // nCS2 - CPLD slow peripheries
  SCONFIG2_bit.MW = 1;  // 16-bit
  SCONFIG2_bit.PC = 0;  // active LOW Chip Select
  SCONFIG2_bit.PM = 0;  // Asynchronous page mode disable
  SCONFIG2_bit.B  = 0;  // Read and write buffer disabled
  SCONFIG2_bit.P  = 0;  // Not Write protected
  SCONFIG2_bit.BLS= 1;  // The active bits in nBLE[3:0] are LOW
  // Wait Write Enable
  SWAITWEN2_bit.WAITWEN = EMC_DIV(CPLD_WAIT_WEN,HCLK_FREQ);
  // Wait Output Enable
  SWAITOEN2_bit.WAITOEN = EMC_DIV(CPLD_WAIT_OEN,HCLK_FREQ);
  // Read Access Delay
  SWAITRD2_bit.WAITRD   = EMC_DIV(CPLD_WAIT_RD,HCLK_FREQ);
  // Write Wait States
  SWAITWR2_bit.WAITWR   = EMC_DIV(CPLD_WAIT_WR,HCLK_FREQ);
  // Bus Turnaround Cycles
  STURN2_bit.BTC        = EMC_DIV(CPLD_WAIT_TURN,HCLK_FREQ);

  // nCS3 - CPLD Fast peripheries
  SCONFIG3_bit.MW = 1;  // 16-bit
  SCONFIG3_bit.PC = 0;  // active LOW Chip Select
  SCONFIG3_bit.PM = 0;  // Asynchronous page mode disable
  SCONFIG3_bit.B  = 0;  // Read and write buffer disabled
  SCONFIG3_bit.P  = 0;  // Not Write protected
  SCONFIG3_bit.BLS= 1;  // The active bits in nBLE[3:0] are LOW
  // Wait Write Enable
  SWAITWEN3_bit.WAITWEN = EMC_DIV(CPLD_WAIT_WEN,HCLK_FREQ);
  // Wait Output Enable
  SWAITOEN3_bit.WAITOEN = EMC_DIV(CPLD_WAIT_OEN,HCLK_FREQ);
  // Read Access Delay
  SWAITRD3_bit.WAITRD   = EMC_DIV(CPLD_WAIT_RD,HCLK_FREQ);
  // Write Wait States
  SWAITWR3_bit.WAITWR   = EMC_DIV(CPLD_WAIT_WR,HCLK_FREQ);
  // Bus Turnaround Cycles
  STURN3_bit.BTC        = EMC_DIV(CPLD_WAIT_TURN,HCLK_FREQ);

  // Enable EMC
  EMC_CONTROL_bit.ENABLE = 1;
}

/*************************************************************************
 * Function Name: Dly_us
 * Parameters: unsigned long Dly
 *
 * Return: none
 *
 * Description: Delay for usec microSeconds base on Timer 0
 *
 * Notes: Max delay 10000 [us]
 *
 *************************************************************************/
static void Dly_us(unsigned long Dly)
{
register unsigned long long Div = HCLK_FREQ*Dly/2000000;
register unsigned long i;

  if(Dly > 10000)
  {
    return;
  }

  // Find a divider and a compare value
  for(i = 0; Div > UINT_MAX;++i,Div >>= 1);

  // Stop Timer 0 Count
  TIMER_CTRL0_bit.CS  = 0;
  // Timer counter reset
  TIMER_CTRL0_bit.CCL = 0;
  TIMER_CTRL0_bit.SEL = i;
  T0CMP1 = Div;
  // Clears counter when CNT0 for Timer 0 and T0CMP1 for Timer 0 match
  CMP_CAP_CTRL = (1UL << 14);
  TIMER_STATUS0_bit.CMP1_ST = 1;
  // Start Timer 0 Count
  TIMER_CTRL0_bit.CS  = 1;
  while(!TIMER_STATUS0_bit.CMP1_ST);
  // Stop Timer 0 Count
  TIMER_CTRL0_bit.CS  = 0;
}

/*************************************************************************
 * Function Name: CopySegment
 * Parameters: unsigned long SrcStartAddress, uunsigned long SegmentSize
 *
 * Return: none
 *
 * Description: Copy egment a form Flash to a SDRAM memory
 *
 * Notes: Max delay 10000 [us]
 *
 *************************************************************************/
static void CopySegment (unsigned long SrcStartAddress, unsigned long DstStartAddress,
                         unsigned long SegmentSize)
{
unsigned char * pStr = (unsigned char *)SrcStartAddress;
unsigned char * pDst = (unsigned char *)DstStartAddress;
  while(SegmentSize--)
  {
    *pDst++ = *pStr++;
  }
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
__rcpc_syspllcntl_bits SysPll;
volatile unsigned long Dummy;

  // Disable MMU
  CP15_Mmu(FALSE);

  // Disable WDT
  WDT_CTL_bit.EN = 0;
  // Init EMC
  // Copy RAMFUNC_init from Flash to RAMFUNC
  CopySegment((unsigned long)__segment_begin("RAMFUNC_init"),
              (unsigned long)__segment_begin("RAMFUNC"),
              (unsigned long)__segment_size("RAMFUNC_init"));
  InitEMC();

  // Set PLL, HCLK, FCLK
  RCPC_CTRL_bit.LOCK = 1;       // All RCPC registers that are accessible
  SYSCLKPRE_bit.HDIV= HCLK_DIV/2;
  CPUCLKPRE_bit.FDIV= FCLK_DIV/2;
  // Init PLL
  *(unsigned long*)&SysPll = SYSPLLCNTL;
  SysPll.SYSFRANGE  = (PLL_FREQ >= (100 MHZ));
  SysPll.SYSPREDIV  = PRE_DIV;
  SysPll.SYSLOOPDIV = FB_DIV;
  SYSPLLCNTL = *(unsigned long *)&SysPll;
  // Set Memory Bus
  MUXCTL7_bit.PC0   = 1; // A16
  MUXCTL7_bit.PC1   = 1; // A17
  MUXCTL7_bit.PC2   = 1; // A18
  MUXCTL7_bit.PC3   = 1; // A19
  MUXCTL7_bit.PC4   = 1; // A20
  MUXCTL7_bit.PC5   = 1; // A21
  MUXCTL7_bit.PC6   = 1; // A22
  MUXCTL7_bit.PC7   = 1; // A23
  MUXCTL10_bit.PK5  = 1; // D21
  MUXCTL10_bit.PD5  = 1; // D13
  MUXCTL10_bit.PK6  = 1; // D22
  MUXCTL10_bit.PD6  = 1; // D14
  MUXCTL10_bit.PK7  = 1; // D23
  MUXCTL10_bit.PD7  = 1; // D15
  MUXCTL10_bit.PN2  = 1; // D24
  MUXCTL10_bit.PN3  = 1; // D25
  MUXCTL11_bit.PD1  = 1; // D9
  MUXCTL11_bit.PK1  = 1; // D17
  MUXCTL11_bit.PK2  = 1; // D18
  MUXCTL11_bit.PD2  = 1; // D10
  MUXCTL11_bit.PK3  = 1; // D19
  MUXCTL11_bit.PD3  = 1; // D11
  MUXCTL11_bit.PK4  = 1; // D20
  MUXCTL11_bit.PD4  = 1; // D12
  MUXCTL12_bit.PD0  = 1; // D8
  MUXCTL12_bit.PK0  = 1; // D16
  MUXCTL14 = 0x00000000; // nBLE1, nBLE2, nBLE3, nCS0, nCS1, nCS2, nCS3
  MUXCTL15 = 0x00000000; // nBLE0
  MUXCTL19_bit.PL5  = 1; // D29
  MUXCTL19_bit.PL6  = 1; // D30
  MUXCTL19_bit.PL7  = 1; // D31
  MUXCTL19_bit.PE7  = 1; // nWAIT
  MUXCTL20_bit.PN0  = 1; // D26
  MUXCTL20_bit.PN1  = 1; // D27
  MUXCTL20_bit.PL4  = 1; // D28

  // Init SDRAM controller and memory
  // DELAY to allow power and clocks to stabilize ~100 us
  Dly_us(100);
  DYNMRCON_bit.RDS = 1; // Command Delayed Strategy
  // 128Mbx2 (2M x 16), 4 banks, row length = 12, column length = 9
  DYNCFG0 = (1UL << 7) | (2UL << 9) | (1UL << 12) | (1UL << 14);
  DYNRASCAS0_bit.RAS  = 2; // tRAS 2
  DYNRASCAS0_bit.CAS  = 2; // tCAS 2
  PRECHARGE_bit.tRP   = EMC_DIV(SDRAM_TRP,HCLK_FREQ);
  DYNM2PRE_bit.tRAS   = EMC_DIV(SDRAM_TRAS,HCLK_FREQ);
  REFEXIT_bit.tSREX   = EMC_DIV(SDRAM_TREX,HCLK_FREQ);
  DOACTIVE_bit.tAPR   = EMC_DIV(SDRAM_TARP,HCLK_FREQ);
  DIACTIVE_bit.tDAL   = EMC_DIV(SDRAM_TDAL,HCLK_FREQ);
  DWRT_bit.tWR        = EMC_DIV(SDRAM_TWR,HCLK_FREQ);
  DYNACTCMD_bit.tRC   = EMC_DIV(SDRAM_TRC,HCLK_FREQ);
  DYNAUTO_bit.tRFC    = EMC_DIV(SDRAM_TRFC,HCLK_FREQ);
  DYNREFEXIT_bit.tXSR = EMC_DIV(SDRAM_TXSR,HCLK_FREQ);
  DYNACTIVEAB_bit.tRRD= EMC_DIV(SDRAM_TRRD,HCLK_FREQ);
  DYNAMICMRD_bit.tMRD = EMC_DIV(SDRAM_TMRD,HCLK_FREQ);
  DYNMCTRL            = 0x2;  // Clock enable
  // Delay ~100 us
  Dly_us(100);
  DYNMCTRL            = 0x183;// issue SDRAM NOP (no operation) command
  DYNMCTRL_bit.INIT   = 0x3;  // issue SDRAM NOP (no operation) command
  // Delay ~200 us
  Dly_us(200);
  DYNMCTRL_bit.INIT   = 0x2;  // issue SDRAM PALL (precharge all) command
  // Delay ~250 us
  Dly_us(250);
  DYNMCTRL_bit.INIT   = 0x3;  // issue SDRAM NOP (no operation) command
  DYNMREF_bit.REFRESH = 1;
  Dly_us(100);
  DYNMREF_bit.REFRESH = EMC_RFR_DIV(SDRAM_OPER_REFRESH,HCLK_FREQ);
  DYNMCTRL_bit.INIT   = 0x1;  // issue SDRAM MODE command
  // Burst Length - 4, CAS - 2, Burst Type - Sequential
  Dummy = *(unsigned long *)(DMEM_CS0_BASE_ADDR + (0x22UL << 11));
  DYNCFG0_bit.B = 1;          // Read and Write Buffer Enable
  DYNMCTRL_bit.INIT   = 0x0;  // issue SDRAM NORMAL operation command
  DYNCFG0_bit.B = 1;          // Read and Write Buffer Enable

  // Remap
  REMAP_bit.REMAP = 0;

  // Init MMU
  CP15_Mmu(FALSE);      			// Diasble MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain(0x0000000F); // Set domains
  CP15_Mmu(TRUE);       			// Enable MMU
  CP15_Cache(TRUE);           // Enable Cache
  CP15_MmuBuffer(TRUE);       // Enable Write buffer

  return(1);
}
