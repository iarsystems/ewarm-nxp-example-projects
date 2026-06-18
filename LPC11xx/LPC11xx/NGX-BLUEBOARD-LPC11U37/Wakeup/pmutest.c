/****************************************************************************
 *   $Id: pmutest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Uxx PMU (Power Management Unit) example
 *
 *   Description:
 *     This file contains PMU slave test modules, main entry, to test PMU
 *     slave APIs.
*
****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.

* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.

****************************************************************************/

#include "LPC11Uxx.h"     /* LPC11xx Peripheral Registers */
#include "gpio.h"
#include "timer32.h"
#include "pmu.h"
#include "glcd_api.h"

#define TEST_TIMER_NUM    0   /* 0 or 1 for 32-bit timers only */

/*****************************************************************************
** Function name:   GINT0_IRQHandler
**
** Descriptions:    Use one GPIO pin as interrupt source
**
** parameters:      None
**
** Returned value:    None
**
*****************************************************************************/
void GINT0_IRQHandler(void)
{
  glcd_clear();
  init_xy_axis(PAGE0,COL0);
  lcd_display_string((unsigned char *)"ENTERING INTO");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)"DEEP POWER-DOWN");
  init_xy_axis(PAGE0 + 4,COL0);
  lcd_display_string((unsigned char *)"MODE...........");
  glcd_delay(80000);
  glcd_clear();

  #if !TEST_DEEPPOWERDOWN
  PMU_Init();
  regVal = LPC_SYSCON->PDSLEEPCFG;
  regVal &= ~(WDT_OSC_PD | BOD_PD);
  PMU_Sleep( MCU_POWER_DOWN, regVal );
#else
  PMU_DeepPowerDown();
#endif


}


/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t bitPattern[2];
  uint32_t eventPattern[2];
#if !TEST_DEEPPOWERDOWN
  uint32_t regVal;
#endif

  SystemInit();
  init_glcd();
  glcd_clear();
  back_light_on();
  init_xy_axis(PAGE0,COL0);
  lcd_display_string((unsigned char *)" NGX TECHNOLOGIES");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)" BANGALORE");
  init_xy_axis(PAGE0 + 4,COL0);
  lcd_display_string((unsigned char *)" BLUEBOARD LPC11U37");
  glcd_delay(200000);

  GPIOInit();

  bitPattern[0] = 0x1<<7;     /* PIO0_7 enabled  External Interrupt switch*/
  eventPattern[1] = 0x0<<7;   /* PIO0_7 falling edge */
  /* GINT0_INT sources OR together */
  GPIOSetGroupedInterrupt( GROUP0, &bitPattern[0], 0, 0, &eventPattern[0] );
  while (1)                             /* Loop forever */
  {}
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
