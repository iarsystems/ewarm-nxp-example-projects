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
 *    1. Date        : October 30, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for the Micro-Blox LPC2478 board.
 *   It implements a MMC/SD card drive. The first free drive letters will be
 *  used. For example, if your PC configuration includes two hard disk partitions
 *  (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 *  appear as F:\.
 *
 * Jumpers:
 *   On Main board
 *    J13 - 2-3
 *    J14 - 2-3
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
 *    J24 - 2-3
 *    J26 - 2-3
 *    J31 - 2-3
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

#define TIMER0_TICK_PER_SEC   2
#define UPDATE_SHOW_DLY       1
#define DLY_100US             500

volatile Boolean TickFlag;

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

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

  // Init SCSI module
  ScsiInit();

  // LUNs Init
  LunInit(SD_DISK_LUN,SdDiskInit,SdGetDiskCtrlBkl,SdDiskIO);

  __enable_interrupt();

  // Init SD card driver
  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    LunImp(i);
  }
  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      LunImp(i);
    }
    if (TickFlag)
    {
      TickFlag = 0;
      // Update MMC/SD card status
      SdStatusUpdate();
    }
  }
}
