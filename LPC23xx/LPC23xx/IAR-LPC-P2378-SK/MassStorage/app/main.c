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
 *    1. Date        : February 28, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for the IAR LPC-P2378-SK board.
 *   It implements a MMC/SD card drive. The first free drive letters will be
 *  used. For example, if your PC configuration includes two hard disk partitions
 *  (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 *  appear as F:\.
 *   The STATUS LED will indicate drive activity.
 *   The UART 0 is used to send debbug messages. It settings are - 115200, 8bits,
 *  1 stop, no parity
 *
 * Jumpers:
 *  PWR_SEL     - depending of power source
 *  USBC/USBSC  - USBSC
 *  RST_E       - unfilled
 *  ISP_E       - unfilled
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

#define TIMER0_TICK_PER_SEC   2
#define UPDATE_SHOW_DLY       ((Int32U)(0.5 * TIMER0_TICK_PER_SEC))
#define DLY_100US             500

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;

volatile Boolean TickFlag = FALSE;

const UartLineCoding_t  UartLineCoding =
{
  115200,UART_ONE_STOP_BIT,UART_NO_PARITY, UART_WORD_WIDTH_8
};

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[2048];

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
 * Function Name: putchar
 * Parameters: int c
 * Return: int
 *
 * Description: put char on UART0
 *		
 *************************************************************************/
int putchar(int c)
{
  while(!UartWrite(UART_0,(pInt8U)&c,1));
  return(c);
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
Int32U Dly = UPDATE_SHOW_DLY;
DiskStatusCode_t StatusHold = (DiskStatusCode_t) -1;
Int32U Size,Tmp;

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

  // Status LED init
  STATUS_LED_FDIR |= STATUS_LED_MASK;
  STATUS_LED_FSET = STATUS_LED_MASK;

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

  // Init SCSI module
  ScsiInit();

  // LUNs Init
  LunInit(SD_DISK_LUN,SdDiskInit,SdGetDiskCtrlBkl,SdDiskIO);

  // Init UART 0
  assert(UartInit(UART_0,4,NORM));
  UartSetLineCoding(UART_0,UartLineCoding);

  __enable_interrupt();

  // Init SD card driver
  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    if(LunImp(i))
    {
      STATUS_LED_FCLR = STATUS_LED_MASK;
    }
    else
    {
      STATUS_LED_FSET = STATUS_LED_MASK;
    }
  }

  // Soft connection enable
  USB_ConnectRes(TRUE);

  printf("IAR Embedded Workbench for ARM\r\n"
         "IAR-LPC-P2378-SK board\r\n"
         "Mass storage demo\r\n");

  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      if(LunImp(i))
      {
        STATUS_LED_FCLR = STATUS_LED_MASK;
      }
      else
      {
        STATUS_LED_FSET = STATUS_LED_MASK;
      }
    }
    if (TickFlag)
    {
      TickFlag = 0;
      // Update MMC/SD card status
      SdStatusUpdate();
      if(Dly-- == 0)
      {
        // LCD show
        Dly = UPDATE_SHOW_DLY;
        // Current state of MMC/SD show
        pDiskCtrlBlk_t pMMCDiskCtrlBlk = SdGetDiskCtrlBkl();
        if(StatusHold != pMMCDiskCtrlBlk->DiskStatus)
        {

          StatusHold = pMMCDiskCtrlBlk->DiskStatus;
          switch (pMMCDiskCtrlBlk->DiskStatus)
          {
          case DiskCommandPass:
            // Calculate MMC/SD size [MB]
            Size = (pMMCDiskCtrlBlk->BlockNumb * pMMCDiskCtrlBlk->BlockSize);
            Tmp  = Size/1000000;
            Tmp += ((Size%1000000)<1000000/2)?0:1;
            if(pMMCDiskCtrlBlk->DiskType == DiskMMC)
            {
              printf("MMC Card - %d MB\r\n",Tmp);
            }
            else
            {
              printf("SD Card - %d MB\r\n",Tmp);
            }
            break;
          default:
            printf("Pls, Insert Card\r\n");
          }
        }
      }
    }
  }
}
