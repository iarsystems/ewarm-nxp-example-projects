/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : IAR-LPC1788-SK Touch Demo
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board. It shows basic use of I/O,
 * timers, interrupts, LCD controller and LCD touch screen.
 *
 * A cursor is shown and moves when the screen is touched.
 *
 * Jumpers:
 *  PWR_SEL    - depending on the power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 1543 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

volatile Int32U Tick;

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/** external data **/
#pragma section=".intvec"
/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL0 and clocks' dividers. PLL = 96MHz,
 *               CPU - 96MHz, PCLK - 48 MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /*Sys Clock Select as CPU clock
    divider 1:1*/
  CLK_SetCpuClk(CPUSEL_CLKSYS,1);
  /*Select IRC oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_IRCOSC);
  /*Enable Main oscilator*/
  CLK_MainOscSet(CLK_ENABLE,MOSCRNG_1_20MHZ);
  /*Select Main oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_MOSC);
  /*Enable set PLL 96Hz (M = 8; P = 1)*/
  CLK_SetMainPll(CLK_ENABLE, 8-1, 0);
  /*Set peripheral divider 1:2. Peripheral clock 48MHz*/
  CLK_SetPeriphClk(2);
  /*PLL0 out is used as the input to the CPU clock divider
  divider 1:1. CPU runs at 96MHz*/
  CLK_SetCpuClk(CPUSEL_CLKPLL,1);
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return:
 *
 * Description: Low level system init (clock, flash accelerator,
 *              SDRAM and vector table address)
 *
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*if debug is in SDRAM
    clock and SDRAM initialization is made
    in mac file*/
#ifndef SDRAM_DEBUG
  int cpuclk;
  /* Flash accelerator safe value*/
  FLASHCFG = (5UL<<12) | 0x3AUL;
  /*Clock Init*/
  InitClock();
  /*Flash accelerator init*/
  cpuclk = CLK_GetClock(CLK_CPU);

  if( cpuclk < 20000000 ){
    FLASHCFG = (0x0UL<<12) | 0x3AUL;
  } else if( cpuclk < 40000000 ){
    FLASHCFG = (0x1UL<<12) | 0x3AUL;
  } else if( cpuclk< 60000000 ){
    FLASHCFG = (0x2UL<<12) | 0x3AUL;
  } else if( cpuclk < 80000000 ){
    FLASHCFG = (0x3UL<<12) | 0x3AUL;
  } else if( cpuclk < 100000000 ){
    FLASHCFG = (0x4UL<<12) | 0x3AUL;
  }
  /*SDRAM init*/
  SDRAM_Init();
#endif
  /*Set vector table location*/
  VTOR  = (unsigned int)__segment_begin(".intvec");

  return  1;
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
Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
ToushRes_t XY_Touch;
Boolean Touch = FALSE;

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD and copy picture in video RAM*/
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Copy cursor in curosor RAM*/
  GLCD_Copy_Cursor ((Int32U *)Cursor, 0, sizeof(Cursor)/sizeof(Int32U));
  /*Cursor coordinates are synchronized
    cursor size is 32/32 pix*/
  GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_32);
  /*Set initial cursor position*/
  GLCD_Move_Cursor(cursor_x, cursor_y);
  /*Enable Cursor*/
  GLCD_Cursor_En(0);

  /* Init touch screen */
  TouchScrInit();

  /*Enable interrupts*/
  __enable_interrupt();

  /*Init USB Host Link LED*/
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetWindow(55,195,268,218);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fTouch screen demo");

  while(1)
  {
    if(TouchGet(&XY_Touch))
    {
      cursor_x = XY_Touch.X;
      cursor_y = XY_Touch.Y;
      GLCD_Move_Cursor(cursor_x, cursor_y);
      if (FALSE == Touch)
      {
        Touch = TRUE;
        USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
      }
    }
    else if(Touch)
    {
      USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
      Touch = FALSE;
    }
  }
}
/** private functions **/
