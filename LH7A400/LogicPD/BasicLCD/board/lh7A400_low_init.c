/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh7A400_low_init.c
 *    Description : Low level init driver
 *
 *    History :
 *    1. Date        : May, 10 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include <limits.h>
#include <NXP/iolh7A400.h>
#include "board.h"
#include "arm922t_cp15_drv.h"
#include "ttbl.h"

#define EMC_DIV(trp,hclk) \
  (((unsigned long long)trp * (unsigned long long)hclk)/1000000000LL)

/*************************************************************************
 * Function Name: InitEMC
 * Parameters: none
 *
 * Return: int
 *
 * Description: This function is used for EMC initialization
 *
 *************************************************************************/
static void InitEMC (void)
{
  // nCS0 - Flash
  SMC_BCR0_bit.WP   = 1;  // Write protection enable
  SMC_BCR0_bit.PME  = 1;  // Page Mode Enable
  // Read/Write Access Delay
  SMC_BCR0_bit.WST1 = EMC_DIV(FLASH_WAIT_RD,HCLK_FREQ);
  // Asynchronous Page Mode Delay
  SMC_BCR0_bit.WST2 = EMC_DIV(FLASH_WAIT_PAGE,HCLK_FREQ);
  // nCS6 - CPLD slow peripheries
  SMC_BCR6_bit.WP   = 0;  // Write protection disable
  SMC_BCR6_bit.PME  = 0;  // Page Mode Disable
  SMC_BCR6_bit.MW   = 1;  // 16 bit
  // Read/Write Access Delay
  SMC_BCR6_bit.WST1 = EMC_DIV(CPLD_WAIT_RD,HCLK_FREQ);
  // Bus Turnaround Cycles
  SMC_BCR6_bit.IDCY = EMC_DIV(CPLD_WAIT_TURN,HCLK_FREQ);

  // nCS7 - CPLD Fast peripheries
  SMC_BCR7_bit.WP   = 0;  // Write protection disable
  SMC_BCR7_bit.PME  = 0;  // Page Mode Disable
  SMC_BCR7_bit.MW   = 1;  // 16 bit
  // Read/Write Access Delay
  SMC_BCR7_bit.WST1 = EMC_DIV(CPLD_WAIT_RD,HCLK_FREQ);
  // Bus Turnaround Cycles
  SMC_BCR7_bit.IDCY = EMC_DIV(CPLD_WAIT_TURN,HCLK_FREQ);
}

/*************************************************************************
 * Function Name: Dly_us
 * Parameters: unsigned long Dly
 *
 * Return: none
 *
 * Description: Delay for Dly usec
 *
 *************************************************************************/
static void Dly_us(unsigned long Dly)
{
  while(Dly--)
  {
    for(volatile int i = 100; i; i--);
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
__CSC_CLKSET_bits SysPll;
  // Disable MMU
  CP15_Mmu(FALSE);
  // ICache enable
  CP15_ICache(TRUE);
  // Disable WDT
  WDT_CTL_bit.EN = 0;
  // Init EMC
  InitEMC();
  // Set core clock mode
  CP15_SetClkMode(PC15_SYNC_MODE);
  // Set PLL, HCLK, FCLK
  // Init PLL
  *(unsigned long*)&SysPll = CSC_CLKSET;

  SysPll.HCLKDIV  = HCLK_DIV-1; // HCLK Divisor
  // PCLK Divisor
  switch(PCLK_DIV)
  {
  case 2:   // HCLK/2
    SysPll.PCLKDIV  = 0;
    break;
  case 4:   // HCLK/4
    SysPll.PCLKDIV  = 1;
    break;
  default : // HCLK/8
    SysPll.PCLKDIV  = 2;
  };

  SysPll.PREDIV   = PRE_DIV;
  SysPll.MAINDIV1 = MAIN_DIV1;
  SysPll.MAINDIV2 = MAIN_DIV2;
  switch(POST_DIV)
  {
  case 1:   // divide-by-1
    SysPll.PS = 0;
    break;
  case 2:   // divide-by-2
    SysPll.PS = 1;
    break;
  case 4:   // divide-by-4
    SysPll.PS = 2;
    break;
  default : // divide-by-8
    SysPll.PS = 3;
  };
  SysPll.SMCROM   = 0;          // HCLK gate to SMC is enabled
  CSC_CLKSET      = *(unsigned long *)&SysPll;
  // Lie the CPLD to control the Data buffer direction signal
  GPIO_PGDD &= ~0x5;
  GPIO_PGD  |=  0x5;

  // Init SDRAM controller and memory
  // DELAY to allow power and clocks to stabilize ~100 us
  Dly_us(100);
  // Init Chip Select Configuration Registers
  SDRAMC_SDCSC0_bit.EBW           = 0;  // External Bus Width 32 bits
  SDRAMC_SDCSC0_bit.BankCount     = 1;  // Bank Count 4
  SDRAMC_SDCSC0_bit.SROM512       = 0;  // SROM Page Depth 512
  SDRAMC_SDCSC0_bit.SROMLL        = 0;  // SROM Lookalike - Normal
  SDRAMC_SDCSC0_bit._2KPAGE       = 0;  // 2K PAGE Depth
  SDRAMC_SDCSC0_bit.CasLat        = 1;  // CAS latency is 2
  SDRAMC_SDCSC0_bit.WBL           = 0;  // Write burst length is the same as the read burst length
  SDRAMC_SDCSC0_bit.RasToCas      = 2;  // RAS latency is 2
  SDRAMC_SDCSC0_bit.AutoPrecharge = 1;  // Disable auto precharge

  SDRAMC_GBLCNFG = 1;
  // JEDEC General SDRAM Initialization Sequence
  SDRAMC_GBLCNFG_bit.CKSD = 0;
  SDRAMC_GBLCNFG_bit.CKE = 1;
  SDRAMC_GBLCNFG_bit.MRS = 1;       // NOP
  Dly_us(200);
  SDRAMC_GBLCNFG_bit.MRS = 0;       // PRECHARGE ALL
  Dly_us(100);
  SDRAMC_GBLCNFG_bit.Initialize = 0;
  SDRAMC_RFSHTMR_bit.REFCNT = 10;   // AUTO REFRESH
  Dly_us(100);
  SDRAMC_RFSHTMR_bit.REFCNT = EMC_DIV(SDRAM_OPER_REFRESH,HCLK_FREQ);
  SDRAMC_GBLCNFG_bit.MRS = 1;       // SDRAM MODE
  // Burst Length - 4, CAS - 2, Burst Type - Sequential
  volatile unsigned long Dummy = *(unsigned long *)(DMEM_CS0_BASE_ADDR + (0x22UL << 10));
  SDRAMC_GBLCNFG_bit.MRS = 0;       // OPERATION

  // Init MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain(0x0000000F); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache
  return(1);
}
