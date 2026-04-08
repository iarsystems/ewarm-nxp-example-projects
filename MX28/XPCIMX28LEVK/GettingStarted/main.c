/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June 28, 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Logic/Freescale iMX28 XPCIMX28LEVK board. It shows
 * basic use of I/O, system initialization (PLL, POWER, EMI, MMU, ICOLL)
 * and timer.
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_icoll.h"

/*************************************************************************
 * Function Name: TMR0_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer0 interrupt handler
 *
 *************************************************************************/
static
void TMR0_Handler(void)
{
  static Int32U count;
  // Clear interrupt flag
  HW_TIMROT_TIMCTRL0_CLR = 1<<15;
  LED2_TOGGLE();
  if(count & 0x1)
  {
    LED3_TOGGLE();
  }
  ++count;
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

  ICOLL_Init();
  /* Init LEDs' outputs */
  HW_PINCTRL_MUXSEL6_bit.BANK3_PIN04 = 3;
  HW_PINCTRL_DRIVE12_bit.BANK3_PIN04_V = 1; // 3.3V
  HW_PINCTRL_DRIVE12_bit.BANK3_PIN04_MA = 0; // 4mA
  /* Enable output */
  HW_PINCTRL_DOE3_SET = 1 << 4;
  /* LED3 off*/
  HW_PINCTRL_DOUT3_CLR = 1 << 4;
  HW_PINCTRL_MUXSEL6_bit.BANK3_PIN05 = 3;
  HW_PINCTRL_DRIVE12_bit.BANK3_PIN05_V = 1; // 3.3V
  HW_PINCTRL_DRIVE12_bit.BANK3_PIN05_MA = 0; // 4mA
  /* Enable output */
  HW_PINCTRL_DOE3_SET = 1 << 5;
  /* LED2 off*/
  HW_PINCTRL_DOUT3_CLR = 1 << 5;

  /* Init TIMROT T0*/
  /* Reset TIMROT module*/
  HW_TIMROT_ROTCTRL_bit.SFTRST = 1;
  for(volatile int i = 100; i; i--);
  HW_TIMROT_ROTCTRL_bit.SFTRST = 0;
  /* Enable clock*/
  for(volatile int i = 100; i; i--);
  HW_TIMROT_ROTCTRL_bit.CLKGATE = 0;

  HW_TIMROT_TIMCTRL0_bit.PRESCALE = 0;
  HW_TIMROT_TIMCTRL0_bit.RELOAD = 1;
  HW_TIMROT_TIMCTRL0_bit.UPDATE = 0;
  HW_TIMROT_TIMCTRL0_bit.IRQ_EN = 1;
  HW_TIMROT_FIXED_COUNT0 = TIMER_CLOCK_FREQ / TICK_PER_SEC;

  /*Setup Timer 0 interrupt*/
  ICOLL_SetupIntr(TMR0_Handler, 0, INT_TIMER0, LEVEL3);
  /*Enable Interrupt in ICOLL*/
  ICOLL_EnableIntr(INT_TIMER0);
  /*Start timer. Clock from 24MHz*/
  HW_TIMROT_TIMCTRL0_bit.SELECT = 0xF;

  __enable_interrupt();
  while(1)
  {

  }
}
