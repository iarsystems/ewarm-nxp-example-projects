/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : September 13, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-2468 board. It implements USB HID mouse.
 *  When host install needed driver a mouse's cursor begin moved
 * in rectangle shape. The WAKE-UP button is used for USB resume when device
 * is suspended.
 *
 * Controls:
 *  K1 - USB resume, when device is suspended
 *
 * Jumpers:
 *  J5      - depending of power source
 *  ISP     - unfilled
 *  nRESET  - unfilled
 *  EINT0   - filled
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

#define TIMER0_TICK_PER_SEC   50
#define DLY_100US             500

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
__irq void Timer0IntrHandler (void)
{
  Update = TRUE;
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  VICADDRESS = 0;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
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
  // Init Ê1
  K1_FDIR &= ~K1_MASK;

  // Init Time0
  PCONP_bit.PCTIM0 = 1; // Enable TMR0 clk
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
  VIC_SetVectoredIRQ((void(*)())Timer0IntrHandler,0,VIC_TIMER0);
  VICINTENABLE |= 1UL << VIC_TIMER0;
  T0TCR_bit.CE = 1;     // counting Enable

  // Initialize USB
  // HID USB
  HidInit();

  __enable_interrupt();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  HD44780_PowerUpInit();
  // Show message on the LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "   HID Class    ");
  LCD_LIGHT_ON();
  while(1)
  {
    if(Update)
    {
      Update = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured &&
         !UsbCoreReq(UsbCoreReqDevSusState))
      {
        // Mouse pointer state machine
        switch(State)
        {
        case 0:
          HidMouseSendReport(0,-2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 1:
          HidMouseSendReport(2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 2:
          HidMouseSendReport(0,2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 3:
          HidMouseSendReport(-2,0,0);
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
      // USB Wake-up
      if (!(K1_FIO & K1_MASK))
      {
        UsbWakeUp();
      }
    }
  }
}
