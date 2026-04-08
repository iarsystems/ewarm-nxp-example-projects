/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : IAR-LPC1788-SK USB Bootloader example
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board.
 *  The USB Bootloader example implements USB Mass Storage Device on the
 * microcontroller's flash memory. This enables the whole microcontroller's flash
 * to be accessed by PC via USB. The first 16KB are used by the USB bootloader.
 * The rest of the flash can be used by user program.
 *  On start up the USB bootloader will check for valid user program at address
 * 0x00004000. If there is no user code or BUT1 button is pressed (if CRP3 is
 * selected the bootloader will not check for pressed BUT1 button) the USB Mass
 * Storage is starded. Else the User program is started.
 *
 * User Program Build:
 *  In order to be able to load user program with the USB bootloader you should do:
 *  1. Link the user program at start address 0x4000. You can do this by simply
 *     using the <installation-root>\arm\examples\NXP\LPC17xx\IAR-LPC-1788-SK
 *     \USBBoot\config\LPC1788_UserFlash.icf file.
 *  2. Build a binary image of the user program: Select Project\Options\Output Converter
 *     Check the "Generate additional output" box and select "binary"
 *
 * To download a user program:
 *  1. Program the USB Bootloader example on the target board (IAR-LP1788-SK).
 *  2. Reset the board with BUT1 button pressed.
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
 * You can find TouchDemo bin file at
 *    <installation-root>\arm\examples\NXP\LPC17xx\IAR-LPC-1788-SK\USBBoot\bin
 *
 * Note: The USBBOOT.BIN can be also delited from the disk. This will erase the first
 * 16KB flash (if CRP is disabled) or the whole flash memory (if CRP1, CPR2 or CRP3
 * is enalbed). Since the USB Bootloader resides in the first 16KB flash the demo will
 * hang. This however can be used to disable CRP if it was previously enabled. The other
 * way is to use ISP.
 *
 * Note: If CRP3 is selected, no future factory testing can be performed on the device
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

#include "fat.h"
#include "DiskImg.h"
#include "Flash.h"

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

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
#endif
  /*Set vector table location*/
  VTOR  = (unsigned int)__segment_begin(".intvec");

  return  1;
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

  if( 0 == FlashBlankCheck((void *) USER_PROGRAM_START,SECTOR_SIZE))
  {
/*If CRP3 is selected, no future factory testing can be
performed on the device*/
#if (CRP != CRP3)
    if(BUT1_MASK & BUT1_FIO)
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

  /*Disable interrupts*/
  __disable_interrupt();
  /*Configure USB Clock*/
  CLK_SetUsbClk(USBSEL_CLKPLL,USB_CLK_DIV);

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

  /* Init SCSI module */
  ScsiInit();

  /* LUNs Init */
  LunInit(DISK_LUN,DiskInit,GetDiskCtrlBkl,DiskIO);

  /*Enable interrupts*/
  __enable_interrupt();

  /* Init Disk activity LED */
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  /* SCSI init push init disk driver message for an each LUN*/
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
  }
}
/** private functions **/
