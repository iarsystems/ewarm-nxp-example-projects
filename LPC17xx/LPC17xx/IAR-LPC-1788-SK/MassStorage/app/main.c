/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : IAR-LPC1788-SK USB Mass Storage
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board. It implements a MMC/SD card drive.
 * The first free drive letters will be used. For example, if your PC configuration
 * includes two hard disk partitions (in C:\ and D:\) and a CD-ROM drive (in E:\),
 * the memory card drive will appear as F:\.
 *
 * The USB Host LED will indicate drive activity.
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

#define TIMER0_TICK_PER_SEC   2
#define UPDATE_SHOW_DLY       1
#define DLY_100US             500

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

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
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
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
int Dly = UPDATE_SHOW_DLY;
DiskStatusCode_t StatusHold = (DiskStatusCode_t) -1;
uint32_t Size,Tmp;

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
  /*Configure USB Clock*/
  CLK_SetUsbClk(USBSEL_CLKPLL,USB_CLK_DIV);

  /* Init SCSI module */
  ScsiInit();

  /* LUNs Init */
  LunInit(SD_DISK_LUN,SdDiskInit,SdGetDiskCtrlBkl,SdDiskIO);

  /*Enable interrupts*/
  __enable_interrupt();

  /*Start timer 0*/
  TMR0_Init(TIMER0_TICK_PER_SEC);

  /* Init Disk activity LED */
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  /* Init SD card driver
   SCSI init push init SD driver message for an each LUN*/
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    /* Implement LUNs messages */
    if(LunImp(i))
    {
      USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
    }
    else
    {
      USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
    }
  }

  /* Soft connection enable */
  USB_ConnectRes(TRUE);


  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetFont(&Terminal_9_12_6,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(100,195,310,218);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fMass storage device\r\n");


  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      /* Implement LUNs messages */
      if(LunImp(i))
      {
        USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
      }
      else
      {
        USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
      }
    }
    if(Tick)
    {
      Tick = 0;
      /* Update MMC/SD card status */
      SdStatusUpdate();
      if(Dly-- == 0)
      {
        /* LCD show */
        Dly = UPDATE_SHOW_DLY;
        /* Current state of MMC/SD show */
        pDiskCtrlBlk_t pMMCDiskCtrlBlk = SdGetDiskCtrlBkl();
        if(StatusHold != pMMCDiskCtrlBlk->DiskStatus)
        {

          StatusHold = pMMCDiskCtrlBlk->DiskStatus;
          switch (pMMCDiskCtrlBlk->DiskStatus)
          {
          case DiskCommandPass:
            /* Calculate MMC/SD size [MB] */
            Size = (pMMCDiskCtrlBlk->BlockNumb * pMMCDiskCtrlBlk->BlockSize);
            Tmp  = Size/1000000;
            Tmp += ((Size%1000000)<1000000/2)?0:1;
            if(pMMCDiskCtrlBlk->DiskType == DiskMMC)
            {
              GLCD_print("MMC Card - %d MB\r",Tmp);
            }
            else
            {
              GLCD_print("SD Card - %d MB\r",Tmp);
            }
            for(int j = 0; j < 512; j++)
            {
              Lun0Buffer[j] = j;
            }
            break;
          default:
            GLCD_print("Pls, Insert Card\r");
          }
        }
      }
    }
  }
}
/** private functions **/
