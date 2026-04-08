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
 *    1. Date        : October 31, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Micro-Blox LPC2478 board.
 *  It implements usb audio device with one output.
 *
 * Jumpers:
 *   On Main board
 *    J13 - 2-3
 *    J14 - 2-3
 *    J16 - 7-8
 *    J16 - 9-10
 *    J16 - 11-12
 *    J16 - 13-14
 *    J16 - 15-16
 *    J16 - 17-18
 *    J16 - 19-20
 *    J17 - 2-3
 *    J18 - 1-2
 *    J18 - 3-4
 *    J18 - 5-6
 *    J18 - 7-8
 *    J48 - filled
 *    J49 - 3-4
 *    J50 - 5-6
 *    J58 - 1-2
 *    J59 - 1-2
 *
 *   On CPU board
 *    J1  - 1-2
 *    J3  - 1-2
 *    J7  - filled
 *    J10 - 1-2
 *    J12 - 1-2
 *    J13 - 1-2
 *    J15 - 2-3
 *    J16 - 1-2
 *    J18 - 2-3
 *    J20 - 2-3
 *    J24 - 2-3
 *    J26 - 2-3
 *    J27 - 1-2
 *    J28 - 2-3
 *    J31 - 2-3
 *    J32 - 2-3
 *    J33 - 2-3
 *    J34 - 2-3
 *    J35 - 2-3
 *    J37 - filled
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

#define DLY_100US             500

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

  // Init Audio
  UsbAudioClassInit();

  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
  }
}
