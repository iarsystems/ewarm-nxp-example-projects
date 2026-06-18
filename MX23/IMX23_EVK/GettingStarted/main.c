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
 *    1. Date        : May 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for Freescale IMX23 EVK board REV.B . It shows basic use
 * of I/O, system initialization (PLL, EMI, MMU, ICOLL) and timers.
 *
 * Switches:
 *    S36(BOOT MODE)  - 0110 (DEBUG)
 *    S14(USB5V) - ON
 *    S22(DEBUG) - ON
 *    S1 (HOLD)  - OFF
 *    S12(BATT SUPPLY)  - REG (Left)
 *
 *
 *    Connect 5V power to J6.
 *    Connect the board USB port (J4) to USB Host (PC)
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
#include "math.h"
/** local definitions **/
#define PWM_PERIOD_1MS      (23999)
#define TIMER_TICK_2MS      (47999)
#define LED_BLINK_TIME      (2) /*in seconds*/
#define LED_BLINK_PERIOD    ((LED_BLINK_TIME*24MHZ/(TIMER_TICK_2MS+1))/2)
#define EXP_COEF            0.02017 /*this is ln(PWM_PERIOD_1MS)/LED_BLINK_PERIOD*/
/** external functions **/

/** external data **/

/** internal functions **/
static void TMR0_Handler(void);
/** public data **/

/** private data **/
static volatile Int32U Tick;
/** public functions **/

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
  int cntr = 0;
  int dir = 0;
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

  /*Init Interrupt controller*/
  ICOLL_Init();
  /*Timer 0 Init*/
  HW_CLKCTRL_XTAL_bit.TIMROT_CLK32K_GATE = 0;
  HW_TIMROT_ROTCTRL_bit.CLKGATE = 0;
  HW_TIMROT_ROTCTRL_bit.SFTRST = 0;
  
  HW_TIMROT_TIMCTRL0_bit.PRESCALE = 0;
  HW_TIMROT_TIMCTRL0_bit.RELOAD = 1;
  HW_TIMROT_TIMCTRL0_bit.UPDATE = 0;
  HW_TIMROT_TIMCTRL0_bit.IRQ_EN = 1;

  HW_TIMROT_TIMCOUNT0_bit.FIXED_COUNT = TIMER_TICK_2MS;

  /*Setup Timer 0 interrupt*/
  ICOLL_SetupIntr(TMR0_Handler, 0, INT_TIMER0, LEVEL3);
  /*Enable Interrupt in ICOLL*/
  ICOLL_EnableIntr(INT_TIMER0);
  /*Start timer. Clock from 24MHz*/
  HW_TIMROT_TIMCTRL0_bit.SELECT = 0xC;
  
  /*Enable PWM*/
  HW_PWM_CTRL_bit.CLKGATE = 0;
  HW_PWM_CTRL_bit.SFTRST = 0;
    
  HW_PWM_ACTIVE1 = 0;                     /**/
  HW_PWM_PERIOD1 = (0<<20) |              /*CDIV 1:1*/
                   (0<<18) |              /*Inactive State hi-Z*/
                   (3<<16) |              /*Active State High*/
     (PWM_PERIOD_1MS << 0) ;              /*Period 1ms*/
  
  HW_PINCTRL_MUXSEL3_bit.BANK1_PIN27 = 0; /*pin PWM1 */
  
  /*Enable PWM clock*/
  HW_CLKCTRL_XTAL_bit.PWM_CLK24M_GATE = 0;
  /*PWM0 and PWM 1 Config*/
  HW_PWM_CTRL_bit.PWM1_ENABLE = 1;
   
  while(1)
  {
    if(Tick)
    {
      Tick = 0;
      if(dir)
      {/*Decrement counter*/
        if(0 >= --cntr)
        {
          dir = 0;
        }
      }
      else
      {/*Increment counter*/
        if(LED_BLINK_PERIOD <= ++cntr)
        {
          dir = 1;
        }
      }
      /*set new LED brightness*/
      HW_PWM_ACTIVE1_bit.INACTIVE = (int)exp(EXP_COEF*cntr);
      HW_PWM_PERIOD1 = HW_PWM_PERIOD1;
    }
  }
}
/** private functions **/
/*************************************************************************
 * Function Name: TMR0_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
static void TMR0_Handler(void)
{
  // Clear interrupt flag
  HW_TIMROT_TIMCTRL0_CLR = 1<<15;
  Tick = 1;
}
