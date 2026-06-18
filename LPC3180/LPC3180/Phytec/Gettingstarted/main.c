/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 12, September 2006
 *       Author      : Stanimir Bonev
 *       Description :
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Phytec LPC3180 evaluation board. It shows
 * basic use of CP15, I/O, timer and the interrupt controllers.
 * It starts by blinking LED_D305 and LED_D310. The buttom S302 change
 * blinking speed.
 *  The jumpers settings are regarding Phytec LPC3180 user manual (default)
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <nxp/iolpc3180.h>
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

// System OSC 13MHz
#define OSC           (13000000UL)
// ARM_CLK 208MHz
#define CORE_CLK      (OSC*16)
// HCLK 104MHz
#define AHB_CLK       (CORE_CLK/2)
// PER_CLK 13MHz
#define PER_CLK       (CORE_CLK/16)
// RTC_CLK
#define RTC_CLK       (32768UL)

// TICK_PER_SECOND
#define TICK_SEC_S1   (100  * (PER_CLK/(13*1000)))
#define TICK_SEC_S1_5 (150  * (PER_CLK/(13*1000)))
#define TICK_SEC_S3   (300  * (PER_CLK/(13*1000)))
#define TICK_SEC_1S   (1000 * (PER_CLK/(13*1000)))

#define LED_D305      (1UL << 2)
#define LED_D306      (1UL << 3)
#define LED_D308      (1UL << 5)
#define LED_D310      (1UL << 6)

#define BUTTON_DLY    5000

volatile int ButtonDly = 0;
unsigned int flag = 1;

/*************************************************************************
 * Function Name: HSTimerHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: High speed timer handler
 *
 *************************************************************************/
static void HSTimerHandler (void)
{
static int count;
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag
  if(++count & 1)
  {
    PIO_OUTP_CLR = LED_D305;
    PIO_OUTP_SET = LED_D310;
  }
  else
  {
    PIO_OUTP_CLR = LED_D310;
    PIO_OUTP_SET = LED_D305;
  }
}

#ifndef BOARD_REV_1
/*************************************************************************
 * Function Name: Gpi07_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_03 handler
 *
 *************************************************************************/
static void Gpi07_handler (void)
{
  ButtonDly = BUTTON_DLY;
  SIC2_RSR_bit.GPI_07 = 1;              // clear interrupt flag (for edge only)
}

#else
/*************************************************************************
 * Function Name: Gpi03_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_03 handler
 *
 *************************************************************************/
static void Gpi03_handler (void)
{
  ButtonDly = BUTTON_DLY;
  SIC2_RSR_bit.GPI_03 = 1;              // clear interrupt flag (for edge only)
}
#endif // BOARD_REV_1

/*************************************************************************
 * Function Name: Sir2Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sub2 Interrupt Controller handler
 *
 *************************************************************************/
static void Sic2Handler (void)
{
#ifndef BOARD_REV_1
  if (SIC2_SR_bit.GPI_07)
  {
    Gpi07_handler();
  }
#else
  if (SIC2_SR_bit.GPI_03)
  {
    Gpi03_handler();
  }
#endif // BOARD_REV_1
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
  if(MIC_SR_bit.HSTIMER_INT)
  {
    HSTimerHandler();
  }
  else if (MIC_SR_bit.Sub2IRQn)
  {
    Sic2Handler();
  }
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
int main(void)
{
int Speed = 0;

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

  // Disable all interrupts
  MIC_ER  = 0;
  SIC1_ER = 0;
  SIC2_ER = 0;

  // Set LEDs off (LED_D305 - GPO3  LED_D306 - GPO4 LED_D308 GPO5  LED_D310 - GPO6)
  PIO_OUTP_SET = LED_D305 | LED_D306 | LED_D308 | LED_D310;

  // Init High speed timer
  TIMCLK_CTRL_bit.HST_CLK_ENA= 1;   // HSTimer clock enable
  HSTIM_CTRL_bit.COUNT_ENAB  = 0;   // Stop counting
  HSTIM_MCTRL_bit.MR0_INT    = 1;   // unmask MATCH0 intr flag
  HSTIM_MCTRL_bit.RESET_COUNT0=1;   // Enable reset of Timer Counter on Match 0
  HSTIM_MCTRL_bit.STOP_COUNT0 =0;   // Disable the stop functionality on Match 0
  HSTIM_PMATCH = 13-1;              // Set prescaler 1us
  HSTIM_CTRL_bit.RESET_COUNT = 1;   // Reset the counter
  while(HSTIM_COUNTER);
  HSTIM_CTRL_bit.RESET_COUNT = 0;   // release reset of the counter
  HSTIM_MATCH0 = TICK_SEC_S1;       // set period
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag
  // Init timer interrupts
  MIC_APR_bit.HSTIMER_INT    = 1;   // Interrupt is generated on a high level signal or rising edge
  MIC_ATR_bit.HSTIMER_INT    = 0;   // Interrupt is level sensitive
  MIC_ITR_bit.HSTIMER_INT    = 0;   // The interrupt is routed to the IRQ output of the interrupt controller
  MIC_ER_bit.HSTIMER_INT     = 1;   // Enable Milisecond timer interrupts
  HSTIM_CTRL_bit.COUNT_ENAB  = 1;   // Enable counting

#ifndef BOARD_REV_1
  // Init button (S302 - GPI7)
  // Init GPI_07 interrupt
  SIC2_APR_bit.GPI_07     = 1;      // Interrupt is generated on a high level signal or rising edge
  SIC2_ATR_bit.GPI_07     = 1;      // Interrupt is edge sensitive
  SIC2_ITR_bit.GPI_07     = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_RSR_bit.GPI_07     = 1;      // clear interrupt flag (for edge only)
  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub2IRQn    = 0;      // Interrupt is generated on a low level signal or falling edge
  MIC_ATR_bit.Sub2IRQn    = 0;      // Interrupt is level sensitive
  MIC_ITR_bit.Sub2IRQn    = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_ER_bit.GPI_07      = 1;      // Enable GPI_07 interrupt
#else
  // Init button (S302 - GPI3)
  // Init GPI_03 interrupt
  SIC2_APR_bit.GPI_03     = 1;      // Interrupt is generated on a high level signal or rising edge
  SIC2_ATR_bit.GPI_03     = 1;      // Interrupt is edge sensitive
  SIC2_ITR_bit.GPI_03     = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_RSR_bit.GPI_03     = 1;      // clear interrupt flag (for edge only)
  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub2IRQn    = 0;      // Interrupt is generated on a low level signal or falling edge
  MIC_ATR_bit.Sub2IRQn    = 0;      // Interrupt is level sensitive
  MIC_ITR_bit.Sub2IRQn    = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_ER_bit.GPI_03      = 1;      // Enable GPI_03 interrupt
#endif // BOARD_REV_1

  MIC_ER_bit.Sub2IRQn     = 1;      // Enable SIC2 interrupts
  __enable_interrupt();
  // main loop
  while(1)
  {
    if(ButtonDly)
    {
      // Button Dly
      for(;ButtonDly;--ButtonDly);
      // Change Speed
      // Stop and reset the HS timer counter
      HSTIM_CTRL_bit.COUNT_ENAB  = 0;
      HSTIM_CTRL_bit.RESET_COUNT = 1;
      while(HSTIM_COUNTER);
      HSTIM_CTRL_bit.RESET_COUNT = 0;
      switch (++Speed)
      {
      case 1:
        HSTIM_MATCH0 = TICK_SEC_S3;         // Set timer period
        break;
      case 2:
        HSTIM_MATCH0 = TICK_SEC_S1_5;       // Set timer period
        break;
      default:
        Speed        = 0;
        HSTIM_MATCH0 = TICK_SEC_S1;         // Set timer period
      }
      HSTIM_CTRL_bit.COUNT_ENAB  = 1;       // Enable counting
    }
  };
} // main(void)
