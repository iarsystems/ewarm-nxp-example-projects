/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 18 Oct. 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale MX21 ADS evaluation board based on
 * MC9328MX21. It shows basic use of I/O, timer and the interrupt controller.
 *  It blinks the LEDs STAT1 and STAT2.
 *
 *    $Revision: 16 $
**************************************************************************/
#include "includes.h"

volatile Int32U Gpt1Counter = 0;

/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: int
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
void GPT1_Handler (void)
{
  // Clear interrupt flag
  TSTAT1_bit.COMP = 1;
  ++Gpt1Counter;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: int
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
Int16U const * pBoardRev = (Int16U *)REV_BASE_ADDRESS;
Int32U LedState;
AITC_Ctrl_t AITC_CtrlInit;

  printf("\n\rEnter in main() section\n\r");

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
  CP15_Cache(TRUE);           // Enable DCache and ICache

  // Show Base and CPU boards revision
  printf("Base board revision : %0.2d \n\r",(*pBoardRev>>8));
  printf("CPU  board revision : %0.2d \n\r",(*pBoardRev&0xFF));

  // Init AITC
  AITC_CtrlInit.IntrPriorityMask = AITC_NOMASKED_INTR;
  AITC_CtrlInit.VectTbl = TableHighAddr;
  AITC_CtrlInit.PrioritySticky   = FALSE;
  AITC_CtrlInit.NormIntrRisePriority = FALSE;
  AITC_CtrlInit.FastIntrRisePriority = TRUE;
  AITC_Init(&AITC_CtrlInit);

  // Init GPT1
  // Enable GPT1 clock
  PCCR1_bit.GPT1_EN = 1;
  TCTL1 = 0x12;
  // Init the Timer's period
  TPRER1_bit.PRESCALER = 0;
  TCMP1 = MPLL_CLK / (PERDIV1 * GPT1_TICK_PER_SEC);
  // Init GPT1 interrupt
  AITC_SetupIntr(GPT1_Handler,FALSE,INT_GPT1,0);
  AITC_EnableIntSource(INT_GPT1);
  // Enable GPT1
  TCTL1_bit.TEN = 1;

  // Enable interrups
  AITC_EnableIRQ();
  AITC_EnableFIQ();
  __enable_interrupt();

  while(1)
  {
    if(Gpt1Counter > 20)
    {
      Gpt1Counter = 0;
      if(++LedState & 1)
      {
        // Led STAT1 ON
        LED_STAT1_ON();
      }
      else
      {
        // Led STAT1 OFF
        LED_STAT1_OFF();
      }
      if(LedState & 2)
      {
        // Led STAT2 ON
        LED_STAT2_ON();
      }
      else
      {
        // Led STAT2 OFF
        LED_STAT2_OFF();
      }
    }
  }
}

