/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 29, October 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Micro-Blox LPC2478 board. It implements USB HID mouse.
 *  When host install needed driver a mouse's cursor begin moved in rectangle shape.
 *
 * Jumpers:
 *  On Main board
 *   J13 - 2-3
 *   J14 - 2-3
 *   J48 - filled
 *   J49 - 3-4
 *   J50 - 5-6
 *   J58 - 1-2
 *   J59 - 1-2
 *
 *  On CPU board
 *   J1  - 1-2
 *   J3  - 1-2
 *   J7  - filled
 *   J24 - 2-3
 *   J26 - 2-3
 *   J31 - 2-3
 *   J37 - filled
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  0x12345678
#define CRP2  0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  0x43218765

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;

#define TIMER0_TICK_PER_SEC   50
volatile Boolean Update;

/*************************************************************************
 * Function Name: Timer0IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Timer0IntrHandler (void)
{
  Update = TRUE;
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  VICADDRESS = 0;
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
Int32U State, Count;

  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled

  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  VIC_Init();
  // Enable TIM0 clocks
  PCONP_bit.PCTIM0 = 1; // enable clock
  // Init Time0
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 0;
  T0MR0 = SYS_GetFpclk(TIMER0_PCLK_OFFSET)/(TIMER0_TICK_PER_SEC);
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  VIC_SetVectoredIRQ(Timer0IntrHandler,0,VIC_TIMER0);
  VICINTENABLE |= 1UL << VIC_TIMER0;
  T0TCR_bit.CE = 1;     // counting Enable

  // Initialize USB
  // HID USB
  HidInit();

  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
    if(Update)
    {
      Update = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured &&
         !UsbCoreReq(UsbCoreReqDevSusState))
      {
        // Mouse pointer
        switch(State)
        {
        case 0:
          HidMouseSendReport(-2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 1:
          HidMouseSendReport(0,2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 2:
          HidMouseSendReport(2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 3:
          HidMouseSendReport(0,-2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        default:
          State = 0;
          Count = 100;
        }
      }
      else
      {
        // Reset state
        State = 0;
        Count = 100;
      }
    }
  }
}
