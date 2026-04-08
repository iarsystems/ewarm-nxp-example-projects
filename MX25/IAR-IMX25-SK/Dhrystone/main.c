/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 16, October 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR  KSK-i.MX25 evaluation board.
 *  It runs dhrysrone test bench. The results are showed in SPY Terminal I/O
 * window (Veiw->Terminal I/O).
 *
 *  Jumpers:
 *  Set the jumpers and the switches regarding the IAR KSK-i.MX25 User’s Manual
 *
 *    $Revision: 16 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

extern  void dystone (void);

/*************************************************************************
 * Function Name: GetTimersTick
 * Parameters: none
 *
 * Return: unsigned long
 *
 * Description: Return count of both cascade conected timers
 *
 *************************************************************************/
unsigned long GetTimersTick (void)
{
  return(GPT1CNT);
}

/*************************************************************************
 * Function Name: GetTickPerMSec
 * Parameters: none
 *
 * Return: float
 *
 * Description: Return number of timer ticks for msecond
 *
 *************************************************************************/
float GetTickPerMSec (void)
{
  return((((float)HCLK /(PCDR1_bit.PERDIV5+1))/1000.0));
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{

  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_ICache(FALSE);

  // Init GTIM
  // Enable GPT1 clock
  CGCR0_bit.ipg_per_gpt  = 1;
  CGCR1_bit.ipg_clk_gpt1 = 1;

  // Disable timer
  GPT1CR = 0;
  // Disable interrupt
  GPT1IR_bit.OF1IE = 0;
  // Reset Timer1
  GPT1CR_bit.SWR = 1;
  while(GPT1CR_bit.SWR);
  // ipg_clk_highfreq - PERCLK5
  GPT1CR_bit.CLKSRC= 2;
  // reset counters
  GPT1CR_bit.ENMOD = 1;
  // freeruning counter
  GPT1CR_bit.FRR = 1;

  // Init the Timer's period
  GPT1PR_bit.PRESCALER = 0;
  // Enable GPT1
  GPT1CR_bit.EN = 1;

  // Disable Instruction and Data cache
  CP15_InvalAllCache();
  CP15_ICache(FALSE);
  printf("I cache - disable, MMU translation - disable, D cache - disable\n");
  dystone();
  // Instruction cache enable Data cache disable
  CP15_InvalAllCache();
  CP15_ICache(TRUE);
  printf("I cache - enable, MMU translation - disable, D cache - disable\n");
  dystone();
  // Instruction cache enable MMU translation enable Data cache disable
  CP15_InvalAllCache();
  CP15_InvalAllTbl();
  CP15_Mmu(TRUE);            // Enable MMU
  printf("I cache - enable, MMU translation - enable, D cache - disable\n");
  dystone();
  // Instruction cache enable MMU translation enable Data cache enable
  CP15_InvalAllCache();
  CP15_InvalAllTbl();
  CP15_DCache(TRUE);
  printf("I cache - enable, MMU translation - enable, D cache - enable\n");
  dystone();
  while(1)
  {
  }
}
