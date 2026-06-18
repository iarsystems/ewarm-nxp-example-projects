/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 04.2011
 *       Author      : Stoyan Choynev
 *       Description : initial vestion
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale iMX35 PDK board. It shows basic use
 * of I/O, system initialization (PLL, EMI, MMU, AITC) and timers.
 *
 *
 *    $Revision: 7859 $
 **************************************************************************/
/** include files **/
#include <NXP/iomcimx35.h>
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_aitc.h"
#include "arm1136jf_cp15_drv.h"
#include "ttbl.h"
/** local definitions **/
static volatile Int8U Gpt1Counter ;
/** default settings **/

/* GPT1 Tick per second*/
#define GPT_TICK_PER_SEC   4

/** external functions **/
#ifndef SDRAM_DEBUG
extern void Init_Clock (void);
extern void Init_SDRAM (void);
#endif /* SDRAM_DEBUG */
/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/

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
  GPTSR_bit.OF1= 1;

  ++Gpt1Counter;

  LED_OFF(LED1|LED2);

  LED_ON(Gpt1Counter & (LED1|LED2));
}

extern void CmdInOperation(unsigned short cmd);
extern void AddrInOperation(unsigned int Column, unsigned int Row);
extern void NandGetStatus(void);
extern unsigned int NandReadPage(unsigned int Page);
extern unsigned int NandCheckBlock(unsigned int Block);
extern unsigned int NandFindValidBlock(unsigned int Block);
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

  CP15_Mmu(FALSE);            // Disable MMU
  CP15_ICache(TRUE);          //Enable ICache

#ifdef IRAM_DEBUG
  /* Init Clock */
  Init_Clock();
  /* Init SDRAM */
  Init_SDRAM();
#endif
  // Init MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(FALSE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache
  //Init CPLD
  WEIM_CSCR5U = 0x0000DCF6;
  WEIM_CSCR5L = 0x444A4541;
  WEIM_CSCR5A = 0x44443302;
  // LEDs Init
  LED_OFF((LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8));

  // Init AITC
  AITC_CtrlInit.IntrPriorityMask = AITC_NOMASKED_INTR;
  AITC_CtrlInit.PrioritySticky   = FALSE;
  AITC_CtrlInit.NormIntrRisePriority = FALSE;
  AITC_CtrlInit.FastIntrRisePriority = TRUE;
  AITC_Init(&AITC_CtrlInit);

  // Init GPT
  Gpt1Counter = 0;
  // Select CKIL for timer clock
  GPTCR_bit.CLKSRC = 4;
  // Init the Timer prescaler
  GPTPR_bit.PRESCALER = 0;
  // Init timer tick interval
  GPTOCR1 = CKIL/GPT_TICK_PER_SEC;
  // Init GPT1 interrupt
  AITC_SetupIntr(GPT1_Handler,FALSE,INT_GPT,1);
  AITC_EnableIntSource(INT_GPT);
  //Enable Interrup on compare
  GPTIR_bit.OF1IE = 1;
  // Enable GPT1
  GPTCR_bit.EN = 1;

  // Enable interrupts
  AITC_EnableIRQ();
  AITC_EnableFIQ();
  __enable_interrupt();

  while(1)
  {
  }
}
