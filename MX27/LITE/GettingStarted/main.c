/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : November 6, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Logic/Freescale iMX27 Lite board. It shows basic use
 * of I/O, system initialization (PLL, MAX, AIPI, EMI, MMU, AITC) and timers.
 *
 *
 *  Note: Connect power supply to the LITEKIT and press the power on button (S3),
 * before starting debug sesion.
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_aitc.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

static volatile Int32U Gpt1Counter;

/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
static
void GPT1_Handler(void)
{
  // Clear interrupt flag
  TSTAT1_bit.COMP = 1;
  ++Gpt1Counter;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main(void)
{
AITC_Ctrl_t AITC_CtrlInit;
Int32U LedState = 0;

#ifdef MMU_ENA
  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache
#endif

  // LEDs Init
  PTC_GIUS_bit.PIN25 = 1;   // Pin utilized for GPIO function
  PTC_GIUS_bit.PIN26 = 1;
  PTC_OCR2_bit.PIN25 = 3;   // Data Register output selected
  PTC_OCR2_bit.PIN26 = 3;
  PTC_DDIR_bit.PIN25 = 1;   // Pin operates as an output.
  PTC_DDIR_bit.PIN26 = 1;
  // LEDs OFF
  LED1_OFF();
  LED2_OFF();

  // Init AITC
  AITC_CtrlInit.IntrPriorityMask = AITC_NOMASKED_INTR;
  AITC_CtrlInit.VectTbl = TableHighAddr;
  AITC_CtrlInit.PrioritySticky   = FALSE;
  AITC_CtrlInit.NormIntrRisePriority = FALSE;
  AITC_CtrlInit.FastIntrRisePriority = TRUE;
  AITC_Init(&AITC_CtrlInit);

  // Init GTIM
  Gpt1Counter = 0;
  // Enable GPT1 clock
  PCCR0_bit.GPT1_EN = 1;
  TCTL1 = 0x412;
  // Init the Timer's period
  TPRER1_bit.PRESCALER = 0;
  TCMP1 = MPLL_CLK / ((PCDR1_bit.PERDIV1+1) * GPT1_TICK_PER_SEC);
  // Init GPT1 interrupt
  AITC_SetupIntr(GPT1_Handler,FALSE,INT_GPT1,1);
  AITC_EnableIntSource(INT_GPT1);
  // Enable GPT1
  TCTL1_bit.TEN = 1;

  // Enable interrupts
  AITC_EnableIRQ();
  AITC_EnableFIQ();
  __enable_interrupt();
  while(1)
  {
    if(Gpt1Counter >= 20)
    {
      Gpt1Counter = 0;
      if(++LedState & 1)
      {
        // Led STAT1 ON
        LED1_ON();
      }
      else
      {
        // Led STAT1 OFF
        LED1_OFF();
      }
      if(LedState & 2)
      {
        // Led STAT2 ON
        LED2_ON();
      }
      else
      {
        // Led STAT2 OFF
        LED2_OFF();
      }
    }
  }
}
