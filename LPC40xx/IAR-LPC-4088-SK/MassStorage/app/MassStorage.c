/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : MassStorage.c
 *    Description : IAR-LPC4088-SK MassStorage Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. It implements a MMC/SD card drive.
 * The first free drive letters will be used. For example, if your PC 
 * configuration includes two hard disk partitions (in C:\ and D:\) and a CD-ROM
 * drive (in E:\), the memory card drive will appear as F:\.
 *
 * The USB Host LED will indicate drive activity
 * 
 * Jumpers:
 *  PWR_SEL    - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 * 
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "spifi_rom_api.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_buffer.h"
#include "scsi_ll.h"
#include "scsi.h"
#include "disk.h"
#include "lun.h"
#include "sd_card_mode.h"
#include "sd_dsk_desc.h"

volatile uint32_t Tick;

#define TIMER0_TICK_PER_SEC   2
#define UPDATE_SHOW_DLY       1
#define DLY_100US             500

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t Lun0Buffer[512];

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/** public data **/
volatile uint32_t CriticalSecCntr;
SPIFIobj obj;
/** external data **/
#pragma section=".intvec"
/** public functions **/
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
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  /*SPIFI initialization*/
  LPC_SC->PCONP |= (1<<16);     /* enable SPIFI clock */
  LPC_SC->PCONP |= (1<<15);     /* enable IOCON clock */

  /*SPIFI Configure Pins*/
  SPIFI_IO0_IOCON = 0x35;
  SPIFI_IO1_IOCON = 0x35;
  SPIFI_IO2_IOCON = 0x35;
  SPIFI_IO3_IOCON = 0x35;
  SPIFI_CLK_IOCON = 0x35;
  SPIFI_CS_IOCON = 0x35;
	/* Initialize SPIFI driver */
	spifi_init(&obj, 7, S_RCVCLK | S_FULLCLK, SPIFIClock/1000000);
  
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
void TIMER0_IRQHandler (void)
{
  ++Tick;
  /*clear match channel 0 interrupt */
  LPC_TIM0->IR = (1<<0);
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: TMR0_Init
 * Parameters: uint32_t tps - Ticks per second
 *
 * Return: none
 *
 * Description: Timer 0 start
 *
 *************************************************************************/
void TMR0_Init(uint32_t tps)
{
  Tick = 0;
  /* Enable TIM0 clock*/
  LPC_SC->PCONP |= (1<<1);

  /*Init Timer 0 */
  LPC_TIM0->TCR = (1<<1);       /* counting  disable, set reset */
  LPC_TIM0->TCR = 0;            /* release reset */
  LPC_TIM0->CTCR = 0;           /* Timer Mode: every rising PCLK edge */
  LPC_TIM0->MCR = (1<<0) |      /* Enable Interrupt on MR0*/
                  (1<<1) |      /* Enable reset on MR0 */
                  (0<<2) ;      /* Disable stop on MR0 */
  /*set timer 0 period*/
  LPC_TIM0->PR = 0;
  LPC_TIM0->MR0 = PeripheralClock/(tps);
  /* init timer 0 interrupt */
  LPC_TIM0->IR = (1<<0);        /* clear pending interrupt */
  LPC_TIM0->TCR = (1<<0);       /* counting Enable */
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_EnableIRQ(TIMER0_IRQn);
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
  LPC_TIM0->TCR &= ~(1<<0);
  /* Enable NVIC TMR0 Interrupt */
  NVIC_DisableIRQ(TIMER0_IRQn);
  /* Timer 0 Clock disable */
  LPC_SC->PCONP &= ~(1<<1);
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile uint32_t j = DLY_100US; j; j--)
    {
    }
  }
}

#define BMP_IMG_ADDR  0x28000000
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
pPic_t pPic; 

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Load Picture from bmp file to SDRAM*/
  pPic = GLCD_LoadPic((bmp_t *)BMP_IMG_ADDR);
  /*Init LCD and copy picture ot VRAM*/
  GLCD_Init (pPic, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Copy cursor in curosor RAM*/
  
  /* Init SCSI module */
  ScsiInit();

  /* LUNs Init */
  LunInit(SD_DISK_LUN,(LunFpnt_t*)&Sd_LunFunDesc);

  /*Enable interrupts*/
  __enable_interrupt();

  /*Start timer 0*/ 
  TMR0_Init(TIMER0_TICK_PER_SEC);

  /* Init Disk activity LED */
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  /* Init SD card driver
   SCSI init push init SD driver message for each LUN*/
  for(uint32_t i = 0; i < SCSI_LUN_NUMB; i++)
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
 

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x0000CCff);
  GLCD_SetWindow(126,124,354,148);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fMass storage device\r\n");

  GLCD_SetFont(&Terminal_9_12_6,0x0000FF,0x0000CCff);
  GLCD_SetWindow(126,224,246,235);
  GLCD_TextSetPos(0,0);

  while(1)
  {
    for(uint32_t i = 0; i < SCSI_LUN_NUMB; i++)
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
