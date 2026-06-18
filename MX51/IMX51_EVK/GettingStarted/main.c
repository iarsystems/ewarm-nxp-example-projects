/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 22.03.2012
 *       Author      : Atanas Uzunov
 *       Description : initial version
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale iMX51 EVK. It shows basic use
 * of I/O, system initialization, GP timer and TZIC interrupt controller.
 *  The Diagnostic LED D25 at GPIO2_6 starts to blink.
 *
 * Note: DDR clock and timings initialization is done in the .mac file. PLL2 is
 *       started at 665MHz. DDR frequency is 166MHz.
 *
 * COMPATIBILITY
 * =============
 *   The example project is compatible with Freescale iMX51 EVK board.
 *  By default, the project is configured to use the J-Link JTAG interface.
 *
 * CONFIGURATIONS
 * ===============
 * DDR_DEBUG:
 *   This configuration is intended for debugging in DDR
 *   Note: Initialization of the DDR controller is
 *         made in mcimx51evk_ddr2.mac file.
 *
 * GETTING STARTED
 * ===============
 *  1) Build and download the example.
 *
 *  2) Run the program.
 *
 *    $Revision: 7859 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx515.h>
#include <intrinsics.h>
#include <stdint.h>
#include "board.h"
#include "arm_comm.h"
#include "armv7a_cp15_drv.h"
#include "ttbl.h"

/** local definitions **/
static volatile uint8_t Gpt1Counter ;

/** default settings **/

/* GPT1 Tick per second*/
#define GPT_TICK_PER_SEC   8

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/
volatile uint8_t flag0;

/** public functions **/

/** private functions **/

/*************************************************************************
 * Function Name: IRQ_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void IRQ_Handler(void)
{
  __DSB();
  if (flag0 < 0xFF) flag0++;
  GPT_GPTSR_bit.OF1= 1;
  __DSB();
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
  /* Invalidate and enable I cache */
  CP15_InvalidateICache();
  CP15_ICache(TRUE);
  /* Invalidate and enable branch predictors */
  CP15_InvalidateBranchPredictors();
  CP15_BranchPrediction(TRUE);
  /* Disable MMU and D cache */
  CP15_Mmu(FALSE);
  /* Build L1 and L2 Translation tables */
  CP15_InitMmuTtb(TtSB,TtTB);
  /* Set base address of the MMU Translation table */
  CP15_SetTTBR0(L1Table);
  CP15_SetTTBR1(0);
  /* Set domains */
  CP15_SetDomains( (DomainManager << 2*1) | (DomainClient << 0) );
  /* Invalidate all TLB entries */
  CP15_InvalidateAllTLBs();
  /* Invalidate data cache */
  CP15_InvalidateDCache();
  /* Enable MMU */
  CP15_Mmu(TRUE);
  /* Enable data cache */
  CP15_DCache(TRUE);

  /* Diagnostic LED init - GPIO2[6] */
  IOMUXC_SW_MUX_CTL_PAD_EIM_D22_bit.MUX_MODE = 1;
  GPIO2_DR_bit.no6   = 0;
  GPIO2_GDIR_bit.no6 = 1;

  /* Init GPT variable*/
  Gpt1Counter = 0;
  /* Select CKIL for timer clock - Low Frequency Reference Clock */
  GPT_GPTCR_bit.CLKSRC = 4;
  /* Init the Timer prescaler */
  GPT_GPTPR_bit.PRESCALER = 0;
  /* Init timer tick interval */
  GPT_GPTOCR1 = CKIL/GPT_TICK_PER_SEC;
  /* Enable Interrupt on compare */
  GPT_GPTIR_bit.OF1IE = 1;
  /* Enable GPT1 */
  GPT_GPTCR_bit.EN = 1;

  /* Configure TZIC Interrupt Controller */
  TZIC_ENSET1_bit.INTENSET39 = 1;
  TZIC_INTSEC1_bit.SECURE39 = 1;
  TZIC_PRIOMASK_bit.MASK = 0x40;
  TZIC_PRIORITY9_bit.PRIO39 = 0x20;
  TZIC_INTCTRL = 0x80010000;

  __enable_interrupt();

  while(1)
  {
    /* If flag0 set from interrupt */
    if(flag0)
    {
      ++Gpt1Counter;
      GPIO2_DR_bit.no6 = 1 & Gpt1Counter;
      flag0 = 0;
    }
  }
}
