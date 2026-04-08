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
 *    1. Date        : November , 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *DESCRIPTION
 *===========
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LP1768-SK board.
 *  The USB Bootloader example implements USB Mass Storage Device on the
 * microcontroller's flash memory. This enables the whole microcontroller's flash
 * to be accessed by PC via USB. The first 16KB are used by the USB bootloader.
 * The rest of the flash can be used by user program.
 *  On start up the USB bootloader will check for valid user program at address 
 * 0x00004000. If there is no user code or WAKE-UP button is pressed (if CRP3 is 
 * selected the bootloader will not check for pressed WAKE-UP button) the USB Mass
 * Storage is starded. Else the User program is started.
 *  
 * User Program Build:
 *  In order to be able to load user program with the USB bootloader you should do:
 *  1. Link the user program at start address 0x4000. You can do this by simply
 *     using the <installation-root>\arm\examples\NXP\LPC17xx\IAR-LPC-1768-SK
 *     \USBBoot\config\LPC1768_UserFlash.icf file.
 *  2. Build a binary image of the user program: Select Project\Options\Output Converter
 *     Check the "Generate additional output" box and select "binary"
 *
 * To download a user program:
 *  1. Program the USB Bootloader example on the target board (IAR-LP1768-SK). 
 *  2. Reset the board with WAKE_UP button pressed.
 *  3. Connect the board to PC with USB cable. A disk drive will appear on your PC.
 *    If you connect it for a first time Windows will automatically load the proper
 *    device driver. Device name depends of the Code Read Protection level (CPR) you
 *    have selected for USB Bootloader example (defined in DiskImg.h).
 *  4. The disk drive contains two files USBOOT.BIN and FIRMWARE.BIN. Delete the 
 *    FIRMWARE.BIN file (this will erase the user flash array).
 *  6. Copy the user program bin file to the "CRP " disk. 
 *  
 *  7. Reset the board. The user program should start.
 *  
 *  To load new user program repeat from step 2.
 *  
 * You can find some ot the IAR-LPC1768-SK examples bin files at
 *    <installation-root>\arm\examples\NXP\LPC17xx\IAR-LPC-1768-SK\USBBoot\bin
 *  
 * Note: The USBBOOT.BIN can be also delited from the disk. This will erase the first
 * 16KB flash (if CRP is disabled) or the whole flash memory (if CRP1, CPR2 or CRP3 
 * is enalbed). Since the USB Bootloader resides in the first 16KB flash the demo will
 * hang. This however can be used to disable CRP if it was previously enabled. The other
 * way is to use ISP.
 *
 * Note: If CRP3 is selected, no future factory testing can be performed on the device
 *
 *CONFIGURATION
 *=============
 *
 *   Make sure that the following jumpers are correctly configured on the
 *  IAR LPC-1768-SK evaluation board:
 *
 *  PWR_SEL     - depending of power source
 *  USBD-       - USB Device (1-2)  
 *  USBD+       - USB Device (1-2)  
 *  RST_E       - unfilled
 *  ISP_E       - unfilled
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#include "fat.h"
#include "DiskImg.h"
#include "Flash.h"

#define DLY_100US             500

volatile Boolean TickFlag = FALSE;

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &SETENA0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &CLRENA0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &CLRPEND0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(Int32U IntNumber, Int8U Priority)
{
volatile Int8U * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

#define FCCLK_FREQ 96000000
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 *              Fcckl = 96MHz
 *
 *************************************************************************/
void InitClock(void)
{
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLL0CON_bit.PLLC = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 4. Disable PLL
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1;   // Selects the main oscillator as a PLL clock source.
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 96/4 = 24MHz
  // 6. Set PLL settings 288 MHz
  PLL0CFG_bit.MSEL = 24-1;
  PLL0CFG_bit.NSEL = 2-1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 7. Enable PLL
  PLL0CON_bit.PLLE = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLL0STAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 3-1;            // 1/3 Fpll
  // 10. Connect the PLL
  PLL0CON_bit.PLLC = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
}

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  // Set to inputs
  FIO0DIR = 0;
  FIO1DIR = 0;
  FIO2DIR = 0;
  FIO3DIR = 0;
  FIO4DIR = 0;

  // clear mask registers
  FIO0MASK = 0;
  FIO1MASK = 0;
  FIO2MASK = 0;
  FIO3MASK = 0;
  FIO4MASK = 0;

  // Reset all GPIO pins to default primary function
  PINSEL0 = 0;
  PINSEL1 = 0;
  PINSEL2 = 0;
  PINSEL3 = 0;
  PINSEL4 = 0;
  PINSEL7 = 0;
  PINSEL8 = 0;
  PINSEL9 = 0;
}

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void)
{
Int32U Mul = 1, Div = 1, Osc, Fsclk;
  if(PLL0STAT_bit.PLLC)
  {
    // when PLL is connected
    Mul = PLL0STAT_bit.MSEL + 1;
    Div = PLL0STAT_bit.NSEL + 1;
  }

  // Find clk source
  switch(CLKSRCSEL_bit.CLKSRC)
  {
  case 0:
    Osc = I_RC_OSC_FREQ;
    break;
  case 1:
    Osc = MAIN_OSC_FREQ;
    break;
  case 2:
    Osc = RTC_OSC_FREQ;
    break;
  default:
    Osc = 0;
  }
  // Calculate system frequency
  Fsclk = Osc*Mul*2;
  Fsclk /= Div*(CCLKCFG+1);
  return(Fsclk);
}

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery)
{
Int32U Fpclk;
pInt32U pReg = (pInt32U)((Periphery < 32)?&PCLKSEL0:&PCLKSEL1);

  Periphery  &= 0x1F;   // %32
  Fpclk = SYS_GetFsclk();
  // find peripheral appropriate periphery divider
  switch((*pReg >> Periphery) & 3)
  {
  case 0:
    Fpclk /= 4;
    break;
  case 1:
    break;
  case 2:
    Fpclk /= 2;
    break;
  default:
    Fpclk /= 8;
  }
  return(Fpclk);
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

typedef void (* func)(void);
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
unsigned int n,m,cluster,index;

    // Flash accelerator init
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif

  if( 0 == FlashBlankCheck((void *) USER_PROGRAM_START,SECTOR_SIZE))
  {
/*If CRP3 is selected, no future factory testing can be
performed on the device*/
#if (CRP != CRP3)
    if(WAKE_UP_MASK & WAKE_UP_FIO)
#endif
    {
      VTOR = USER_PROGRAM_START;

      register func user_code_entry;
      
      /* Set stack pointer with the first world of the user progrtam
         sp = *((Int32U *) USER_PROGRAM_START);*/
      asm("mov sp,#0x4000");
      asm("ldr sp,[sp,#0]");
	
  	  user_code_entry = * ((func *)USER_PROGRAM_START+1);
      user_code_entry();
    }
  }
  // Init GPIO
  GpioInit();

  for(n=0;n<FAT_DATA_SIZE ;n++)
  {
    Fat[n] = 0xFF;
  }

  Fat[0]= 0xF8;
  Fat[1]= 0xFF;
  Fat[2]= 0xFF;
  /* Build Fat table*/
  for(int file = 1; file < ROOT_ENR_CNT; file++)
  {
    if(0xE5 != DirEntry[file].DIR_Name[0])
    {
      n = DirEntry[file].DIR_FstClusLO;
      m = n + (DirEntry[file].DIR_FileSize-1)/SECTOR_SIZE/SECT_PER_CLUSTER;
      for(;n <= m;n++)
      {
        if(n == m) cluster = 0xFFF;
        else cluster = n+1;
        index = n*3;
        
        if(index&0x1)
        { 
          index >>= 1;
          Fat[index] &= 0x0F;
          Fat[index] |= (cluster<<4)&0xF0;
          Fat[index+1] = (cluster>>4)&0xFF;
        }
        else
        {
          index >>= 1;
          Fat[index] = (cluster)&0xFF;
          Fat[index+1] &= 0xF0;
          Fat[index+1] |= (cluster>>8)&0x0F;
        }
      }
    }
  }

  // Init SCSI module
  ScsiInit();

  // LUNs Init
  LunInit(DISK_LUN,DiskInit,GetDiskCtrlBkl,DiskIO);

  __enable_interrupt();

  // Init SD card driver
  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    if(LunImp(i))
    {
    }
  }
  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      if(LunImp(i))
      {
      }
    }
  }
}
