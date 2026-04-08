/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : USBMouse.c
 *    Description : IAR-LPC4088-SK USBMouse Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. It implements a USB HID mouse.
 * Connect the board OTG to PC. The mouse cursor moves as the board position is
 * changed(the acceleration sensor is used)
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
#include "smb380_drv.h"
#include "spifi_rom_api.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_buffer.h"

#include "hid_mouse.h"

volatile uint32_t Tick;

#define TIMER0_TICK_PER_SEC   50

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
pPic_t pPic; 
uint8_t Smb380Id, Smb380Ver;
SMB380_Data_t XYZT;
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
  /*HID USB*/
  HidInit();

  /*Enable interrupts*/
  __enable_interrupt();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);
 
  /*Init Acceleration sensor*/
  SMB380_Init();
  // Get Acceleration sensor ID and Version
  SMB380_GetID(&Smb380Id, &Smb380Ver);
   
  /*Start timer 0*/ 
  TMR0_Init(TIMER0_TICK_PER_SEC);

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x0000CCff);
  GLCD_SetWindow(144,124,336,148);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fHID class device");

  while(1)
  {
    if(Tick)
    {
      Tick = 0;
      
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured &&
         !UsbCoreReq(UsbCoreReqDevSusState))
      {
        SMB380_GetData (&XYZT);
        XYZT.AccX /= 256;
        XYZT.AccY /= 256;
        XYZT.AccZ /= 256;
        HidMouseSendReport(XYZT.AccX,XYZT.AccY,0);
      }
      // USB Wake-up
      if (0 < XYZT.AccZ)
      {
        UsbWakeUp();
      }
    }
  }
}
/** private functions **/
