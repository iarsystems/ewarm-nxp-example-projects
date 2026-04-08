/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.c
 *
 *    GETTING STARTED
 *    ===============
 *
 *      1) Build and download the program.
 *
 *      2) Run the program.
 *
 *
 *    History :
 *    1. Date        : October 2014
 *       Author      : Stanimir Bonev
 *       Description : create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "NXP\iolpc54xxx.h"
#include "stdint.h"
#include "modules\drv_nvic.h"

#define IRC_FREQ       12000000
#define XTAL_FREQ      32768

volatile uint32_t SysTick;

#define LED_MASK  (1<<2)
#define LED_DIR   GPIO_DIR1
#define LED_PIN   GPIO_W34

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize PLL and clock dividers. FCCO = 320MHz,
 *              PLL1_OUT = 160MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /* IRC Oscillator */
  MAINCLKSELA_bit.SEL = 0;
  /* Use the clock source selected in MAINCLKSELA register. */
  MAINCLKSELB_bit.SEL = 0;
  AHBCLKDIV = 1;
}

/*************************************************************************
 * Function Name: RIT_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Repetitive interrupt timer handler
 *
 *************************************************************************/
void RIT_IRQHandler(void)
{
    SysTick = 1;
    /* Clear interrupt */
    RIT_CTRL_bit.RITINT = 1;
    NVIC_ClearPendingIRQ(NVIC_RIT);
}

/*************************************************************************
 * Function Name: RIT_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialization of repetitive interrupt timer
 *
 *************************************************************************/
void RITIMER_Init(void)
{
  /* Clock enable */
  AHBCLKCTRL1_bit.RIT = 1;
  RIT_COUNTER = 0;
  /* Value for 0.25s timer interrupt @ 12 MHz timer clock */
  RIT_COUNTER = 0;
  RIT_COUNTER_H = 0;
  RIT_COMPVAL = 3000000;
  RIT_COMPVAL_H = 0;
  RIT_CTRL_bit.RITEN = 1;
  RIT_CTRL_bit.RITENCLR = 1;
  /* Enable interrupt */
  NVIC_EnableIRQ(NVIC_RIT);
  NVIC_SetPriority(NVIC_RIT, 16);
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
int main()
{
  /* Configures Clock Generation Unit */
  InitClock();
  /* Configures Repetitve Interrupt Timer */
  RITIMER_Init();
  /* Configure the onboard Green LED - GPIO1.2 */
  AHBCLKCTRL0_bit.IOCON = 1;
  AHBCLKCTRL0_bit.GPIO1 = 1;
  IOCON_PIO1_2_bit.DIGIMODE = 1;
  IOCON_PIO1_2_bit.FUNC = 0;
  LED_DIR |= LED_MASK;
  LED_PIN = 0xFFFFFFFF;

  while(1)
  {
    if (SysTick)
    {
      SysTick = 0;
      /* Invert onboard LED */
      LED_PIN ^= 0xFFFFFFFF;
    }
  }
}
