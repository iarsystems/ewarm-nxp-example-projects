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
 *    1. Date        : January 7, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-2378-SK board. It implements USB HID mouse.
 *  The joystick control up, down, left, right and left mouse button, B1
 * is the left mouse button too and B2 is the right mouse button.
 *
 * Jumpers:
 *  PWR_SEL     - depending of power source
 *  USBC/USBSC  - USBSC
 *  RST_E       - unfilled
 *  ISP_E       - unfilled
 *
 *  Joystick    - left, right, up, down movement, center left button
 *  BUT1    - left button
 *  BUT2    - right button
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *  If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
 * this will disable trace port too.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define TIMER0_TICK_PER_SEC   50
#define MOUSE_DELTA           4
#define DLY_100US             500

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

volatile Boolean TickFlag = FALSE;

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
  TickFlag = TRUE;
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
Buttons_t ButtCtrl;
Int8S X = 0, Y = 0;
Int8U Buttons = 0, ButtonsHold = 0;

  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES = 3;    // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled

  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  VIC_Init();
  // Init buttons
  ButtonsInit();

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
  VIC_SetVectoredIRQ(Timer0IntrHandler,0,VIC_TIMER0);
  VICINTENABLE |= 1UL << VIC_TIMER0;
  T0TCR_bit.CE = 1;     // counting Enable

  // Initialize USB
  // HID USB
  HidInit();

  __enable_interrupt();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(20,116,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fHID Class Mouse\r");

  while(1)
  {
    if(TickFlag)
    {
      TickFlag = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured)
      {
        ButtCtrl = GetButtons();
        // Buttons and Joystick processing
        if(ButtCtrl.JsUp)
        {
          Y -= MOUSE_DELTA;
        }
        else if(ButtCtrl.JsDown)
        {
          Y += MOUSE_DELTA;
        }

        if(ButtCtrl.JsRight)
        {
          X += MOUSE_DELTA;
        }
        else if(ButtCtrl.JsLeft)
        {
          X -= MOUSE_DELTA;
        }

        if(ButtCtrl.JsCenter || ButtCtrl.B1)
        {
          Buttons |= 1;
        }
        else
        {
          Buttons &= ~1;
        }

        if(ButtCtrl.B2)
        {
          Buttons |= 2;
        }
        else
        {
          Buttons &=~2;
        }

        if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
        {
          UsbWakeUp();  // wake-up
          // Send report
          HidMouseSendReport(X,Y,Buttons);
          ButtonsHold = Buttons;
          Y = X = Buttons = 0;
        }
      }
    }
  }
}
