/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : LCD.c
 *    Description : IAR-LPC1788-SK LCD Example
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board. It shows basic use of I/O,
 * timer, interrupt and LCD controllers.
 *
 * The IAR, NXP and Olimex logos appear on the LCD. The cursor moves as
 * the board position is changed(the acceleration sensor is used).
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
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TMR0_IRQHandler (void)
{
  ++Tick;
  /*clear interrupt*/
  T0IR_bit.MR0INT = 1;
  /*Dummy read*/
  T0IR;
}

/*************************************************************************
 * Function Name: TMR0_Init
 * Parameters: Int32U tps - Ticks per second
 *
 * Return: none
 *
 * Description: Timer 0 start
 *
 *************************************************************************/
void TMR0_Init(Int32U tps)
{
  Tick = 0;
  /* Enable TIM0 clock*/
  PCONP_bit.PCTIM0 = 1;

  /*Init Timer 0 */
  T0TCR_bit.CE = 0;      /* counting  disable */
  T0TCR_bit.CR = 1;      /* set reset */
  T0TCR_bit.CR = 0;      /* release reset */
  T0CTCR_bit.CTM = 0;    /* Timer Mode: every rising PCLK edge */
  T0MCR_bit.MR0I = 1;    /* Enable Interrupt on MR0*/
  T0MCR_bit.MR0R = 1;    /* Enable reset on MR0 */
  T0MCR_bit.MR0S = 0;    /* Disable stop on MR0 */
  /*set timer 0 period*/
  T0PR = 0;
  T0MR0 = CLK_GetClock(CLK_PERIPH)/(tps);
  /* init timer 0 interrupt */
  T0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  T0TCR_bit.CE = 1;     /* counting Enable */
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER0);
}

/*************************************************************************
 * Function Name: TMR0_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 stop
 *
 *************************************************************************/
void TMR0_Stop(void)
{
  /* counting  disable */
  T0TCR_bit.CE = 0;
  /* Enable NVIC TMR0 Interrupt */
  NVIC_IntDisable(NVIC_TIMER0);
  /* Timer 0 Clock disable */
  PCONP_bit.PCTIM0 = 0;
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
int cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
uint8_t Smb380Id, Smb380Ver;

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD and copy picture in video RAM*/
  GLCD_Init (LogoPic.pPicStream, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Copy cursor in curosor RAM*/
  GLCD_Copy_Cursor ((Int32U *)Cursor, 0, sizeof(Cursor)/sizeof(Int32U));
  /*Cursor coordinates are synchronized
    cursor size is 64/64 pix*/
  GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_64);
  /*Set initial cursor position*/
  GLCD_Move_Cursor(cursor_x, cursor_y);
  /*Enable Cursor*/
  GLCD_Cursor_En(0);

  /*Init the accelerometer sensor*/
  SMB380_Init();
  /*Read ID*/
  SMB380_GetID(&Smb380Id, &Smb380Ver);

  SMB380_Data_t XYZT;

  /*Start timer 0 100 tick per second*/
  TMR0_Init(100);
  /*Enable interrupts*/
  __enable_interrupt();

  while(1)
  {

    if(Tick)
    {
      Tick = 0;
      /*Get accelerometer data*/
      SMB380_GetData (&XYZT);
      /*New cursor position*/
      cursor_x += XYZT.AccX/512;
      cursor_y += XYZT.AccY/512;
      /*correct edge x position */
      if((C_GLCD_H_SIZE - CURSOR_H_SIZE/2) < cursor_x)
      {
        cursor_x = C_GLCD_H_SIZE - CURSOR_H_SIZE/2;
      }

      if(-(CURSOR_H_SIZE/2) > cursor_x)
      {
        cursor_x = -(CURSOR_H_SIZE/2);
      }

      /*correct edge y position */
      if((C_GLCD_V_SIZE - CURSOR_V_SIZE/2) < cursor_y)
      {
        cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE/2);
      }

      if(-(CURSOR_V_SIZE/2) > cursor_y)
      {
        cursor_y = -(CURSOR_V_SIZE/2);
      }
      /*Set new cursor position*/
      GLCD_Move_Cursor(cursor_x, cursor_y);
    }
  }
}
/** private functions **/
