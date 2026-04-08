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
 *    1. Date        : 19, January 2006
 *       Author      : Stanimir Bonev
 *       Description : Main module
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the NOHAU LPC3100 evaluation board. It shows
 * basic use of I/O, timer and the interrupt controllers.
 * It starts by blinking D3 and D4. The buttom S3 change blinking speed.
 *  The jumpers settings are regarding  Nohau LPC3100 user manual
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <nxp/iolpc3180.h>

// System OSC 13MHz
#define OSC         (13000000UL)
// ARM_CLK 208MHz
#define CORE_CLK    (OSC*16)
// HCLK 104MHz
#define AHB_CLK     (CORE_CLK/2)
// PER_CLK 13MHz
#define PER_CLK     (CORE_CLK/16)
// RTC_CLK
#define RTC_CLK     (32768UL)
// TICK_PER_SECOND
#define TICK_SEC    (5UL)
#define TICK_SEC_S1 (8UL)
#define TICK_SEC_S2 (10UL)

#define LED_D3      (1UL << 9)
#define LED_D4      (1UL << 10)

#define BUTTON_DLY  500000

volatile int ButtonDly = 0;

/*************************************************************************
 * Function Name: MilisecTimerHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Millisecond timer handler
 *
 *************************************************************************/
static void MilisecTimerHandler (void)
{
static int count;
  MSTIM_INT_bit.MATCH0_INT = 1; // Clear interrupt flag
  if(++count & 1)
  {
    PIO_OUTP_CLR = LED_D4;
    PIO_OUTP_SET = LED_D3;
  }
  else
  {
    PIO_OUTP_CLR = LED_D3;
    PIO_OUTP_SET = LED_D4;
  }
}

/*************************************************************************
 * Function Name: Gpi06_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_06 handler
 *
 *************************************************************************/
static void Gpi06_handler (void)
{
  ButtonDly = BUTTON_DLY;
  SIC2_RSR_bit.GPI_06 = 1;              // clear interrupt flag (for edge only)
}

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
  if (SIC2_SR_bit.GPI_06)
  {
    Gpi06_handler();
  }
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
  if(MIC_SR_bit.MSTIMER_INT)
  {
    MilisecTimerHandler();
  }
  else if (MIC_SR_bit.Sub2IRQn)
  {
    Sic2Handler();
  }
}

/*************************************************************************
 * Function Name: ClockInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Pll, HCLK, PERIPH_CLK,
 *  Pll_clk_out = Osc * 16         = 208MHz
 *  HCLK        = 1/2 Pll_clk_out  = 104MHz
 *  PERIPH_CLK  = 1/16 Pll_clk_out = 13MHz
 *
 *************************************************************************/
void ClockInit (void)
{
  // Set Clk dividers
  HCLKDIV_CTRL_bit.HCLK = 2-1;        // 1/2  Pll_clk_out
  HCLKDIV_CTRL_bit.PERIPH_CLK = 16-1; // 1/16 Pll_clk_out
  // PLL Init - OSC * 16 = 208MHz
  HCLKPLL_CTRL_bit.BYPASS   = 0;      // OSC connected to PLL input
  HCLKPLL_CTRL_bit.DIRECT   = 1;      // CCO connected to PLL_Clk output
  HCLKPLL_CTRL_bit.FEEDBACK = 0;      // CC0 connected to N divider input
  HCLKPLL_CTRL_bit.N = 0;             // set divide
  HCLKPLL_CTRL_bit.M = 16;            // set multiplier
  HCLKPLL_CTRL_bit.POWER_DOWN = 1;    // Enable Pll
  // Wait until PLL lock
  while(!HCLKPLL_CTRL_bit.PLL_LOCK);
  // Connect Pll_clk_out
  PWR_CTRL_bit.RUN_MODE = 1;
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
  // Init Clocks
  ClockInit();
  // Set LEDs off (D3 - GPO_09, D4 GPO_10)
  PIO_OUTP_SET = LED_D3 | LED_D4;
  // Init Timer
  MSTIM_CTRL = 2;                   // Reset and stop timer counters
  MSTIM_MCTRL_bit.RESET_COUNT0 = 1; // Reset timer counter on match 0
  MSTIM_MCTRL_bit.MR0_INT = 1;      // Enable interrupt by match 0
  MSTIM_MATCH0 = RTC_CLK/TICK_SEC;  // Set timer period
  // Init timer interrupts
  MIC_APR_bit.MSTIMER_INT = 1;      // on a low level signal
  MIC_ATR_bit.MSTIMER_INT = 0;      // Set level sensitive interrupt
  MIC_ITR_bit.MSTIMER_INT = 0;      // assign to IRQ line
  MIC_ER_bit.MSTIMER_INT  = 1;      // Enable Millisecond timer interrupts
  MSTIM_CTRL = 1;                   // Enable millisecond timer and release reset
  // Init button (S3 - GPI6)
  // Init timer interrupt
  MIC_APR_bit.MSTIMER_INT = 1;      // on a high level signal
  MIC_ATR_bit.MSTIMER_INT = 0;      // Set level sensitive interrupt
  MIC_ITR_bit.MSTIMER_INT = 0;      // assign to IRQ line
  MIC_ER_bit.MSTIMER_INT  = 1;      // Enable Millisecond timer interrupts
  // Init GPI_06 interrupt
  SIC2_APR_bit.GPI_06     = 0;      // on a falling edge signal
  SIC2_ATR_bit.GPI_06     = 1;      // Set edge sensitive interrupt
  SIC2_ITR_bit.GPI_06     = 0;      // assign to IRQ line
  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub2IRQn    = 0;      // on a low level signal
  MIC_ATR_bit.Sub2IRQn    = 0;      // Set level sensitive interrupt
  MIC_ITR_bit.Sub2IRQn    = 0;      // assign to IRQ line
  SIC2_ER_bit.GPI_06      = 1;      // Enable GPI_06 interrupt
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
      switch (++Speed)
      {
      case 1:
        MSTIM_CTRL   = 0;                   // Stop timer counters
        MSTIM_MATCH0 = RTC_CLK/TICK_SEC_S1; // Set timer period
        break;
      case 2:
        MSTIM_CTRL   = 0;                   // Stop timer counters
        MSTIM_MATCH0 = RTC_CLK/TICK_SEC_S2; // Set timer period
        break;
      default:
        Speed        = 0;
        MSTIM_CTRL   = 2;                   // Stop timer counters
        MSTIM_MATCH0 = RTC_CLK/TICK_SEC;    // Set timer period
      }
      MSTIM_COUNTER  = 0;                   // reset counter
      MSTIM_CTRL     = 1;                   // Enable millisecond timer and release reset
    }
  };
} // main(void)
