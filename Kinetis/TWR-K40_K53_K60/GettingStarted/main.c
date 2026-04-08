/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 27, August 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for TWR-K40/TWR-K53/TWR-K60/TWR-K70/TWR-KL25Z48M boards. It
 * shows basic use of GPIO and system timer.
 *  It toggles LED D16 (PC7) of TWR-K40 and TWR-K53, D16 (PA11) of TWR-K60,
 * D7 (PTA4) of TWR-K70 15 times per second and LED1 (D501) on TWR-KL25Z48M.
 *
 *   Note: The flashloader keep controller in unlocked state. For locking the
 * controller only 00 (10 is unlock combination at adress 0x40C bits 0,1) is
 * valid combination
 *
 *    $Revision: 16 $
 **************************************************************************/

#include <intrinsics.h>
#include "board.h"

#ifdef FLASH
#pragma section="FlashConfig"
typedef union _FlashConfig_t
{
  unsigned int Data[4];
  struct {
    unsigned int BackDoorKey[2];
    unsigned int Protection;
    unsigned int Config;
  };
} FlashConfig_t;

#pragma location = "FlashConfig"
__root const FlashConfig_t Config @ "FlashConfig" =
{
#if !defined(KE15)
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
#else
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFEFFFF,
#endif
};
#endif

#pragma section=".intvec"
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
  
  /* Set vector table location */
  SCB_VTOR = (unsigned int)__segment_begin(".intvec");
  /* Init Leds*/
  /* Enable clock */
  LED_SIM_SCGC_REG = LED_SIM_SCGC_MASK;
  LED_PCR_REG = PORT_PCR_MUX(1);        /* Alternative 1 (GPIO) */;

  LED_PSOR_REG = LED_MASK;              /* Set output high */
  LED_POER_REG = LED_MASK;              /* Output Enable */

  /* Init System tick times */
  /* Clear pending interrupt*/
  volatile unsigned int dummy = SYST_CSR;
  /*clear count register*/
  SYST_CVR = 0;
  /*Set period*/
  SYST_RVR = (SYS_CLK/TICK_PER_SEC);
  /*Clock source - System Clock*/
  SYST_CSR |= SysTick_CSR_CLKSOURCE_MASK;
  /*Enable interrupt*/
  SYST_CSR |= SysTick_CSR_TICKINT_MASK;
  /*Start Sys Timer*/
  SYST_CSR |= SysTick_CSR_ENABLE_MASK;

  /* Main loop */
  while(1)
  {
  }
}

/*************************************************************************
 * Function Name: SysTick_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: System timer interrupt handler
 *
 *************************************************************************/
void SysTick_Handler(void)
{
  /* Clear interrupt*/
  volatile unsigned int dummy = SYST_CSR;
  LED_PTOR_REG = LED_MASK;
}

