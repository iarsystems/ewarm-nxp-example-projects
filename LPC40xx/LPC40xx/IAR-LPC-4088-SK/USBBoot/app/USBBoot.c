/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : USBBoot.c
 *    Description : IAR-LPC4088-SK USB Bootloader Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. The USB Bootloader example
 * implements USB Mass Storage Device and enables both, the microcontroller's 
 * flash memory and the onboard SPIFI NOR, to be accessed by PC via USB.
 * The first 24KB of the microcontroller's flash are used by the USB bootloader.
 * The rest of the flash can be used by a user program. On start up the USB
 * bootloader will check for valid user program at address 0x00006000. If there
 * is no user code or BUT1 button is pressed (if CRP3 is selected the bootloader
 * will not check for pressed BUT1 button) the USB Mass Storage will start. Else
 * the User program is started.
 *  
 * User Program Build:
 *  In order to be able to load user program with the USB bootloader you should do:
 *  1. Link the user program at start address 0x6000. You can do this by simply
 *     using the $PROJ_DIR$\..\common\config\LPC4088_UserFlash.icf file.
 *  2. Build a binary image of the user program: Select "Project\Options\Output 
 *     Converter" Check the "Generate additional output" box and select "binary"
 * 
 * To download a user program and a SPIFI image:
 *  1. Program the USB Bootloader example on the target board (IAR-LP4088-SK). 
 *  2. Reset the board with BUT1 button pressed.
 *  3. Connect the board to PC with USB cable. Two disk drives will appear on 
 *     your PC. If you connect it for a first time Windows will automatically
 *     load the proper device driver. The microcontroller's flash device name
 *     depends of the Code Read Protection level (CPR) you have selected for USB
 *     Bootloader example. The SPIFI NOR flash device is named SPIFI FLASH
 *  4. The CPR disk drive contains two files USBOOT.BIN and FIRMWARE.BIN. Delete 
 *     the FIRMWARE.BIN file (this will erase the user flash array).
 *  6. Copy the user program bin file to the "CRP" disk.
 *  7. The SPIFI FLASH disk drive contains only one file SPIFIIMG.BIN. Delete it
 *     to erasethe SPIFI NOR flash memory. Copy a spifi image file to SPIFI FLASH
 *     disk.
 * 
 *  8. Reset the board. The user program should start.
 *  
 *  To load new user program repeat from step 2.
 *  
 * You can find TouchDemo, AudioDevice, LCD demo and spifiimg bin files at
 *    $PROJ_DIR$\bin
 *  
 * Note: The USBBOOT.BIN can be also delited from the disk. This will erase the 
 * first 24KB flash (if CRP is disabled) or the whole flash memory (if CRP1,  
 * CPR2 or CRP3 is enalbed). Since the USB Bootloader resides in the first 24KB
 * flash the demo will hang. This however can be used to disable CRP if it was 
 * previously enabled. The other way is to use ISP.
 * 
 * Note: If CRP3 is selected, no future factory testing can be performed on the 
 *       device
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
#include "fat.h"
#include "DiskImg.h"
#include "Flash_disk.h"
#include "lun.h"
#include "dsk_desc.h"
#include "Flash.h"
#include "spifi_dsk_desc.h"
#include "spifi_disk.h"
#include "spifi_rom_api.h"

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t Lun0Buffer[512];

/** public data **/
volatile uint32_t CriticalSecCntr;
//SPIFIobj obj;
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

  if( 0 == FlashBlankCheck((void *) USER_PROGRAM_START,SECTOR_SIZE))
  {
/*If CRP3 is selected, no future factory testing can be
performed on the device*/
#if (CRP != CRP3)
    if(BUT1_MASK & BUT1_FIO)
#endif
    {
      register func user_code_entry;
      
      /* Set stack pointer with the first world of the user progrtam */
      __set_SP(*((uint32_t *) USER_PROGRAM_START));
	
      user_code_entry = * ((func *)USER_PROGRAM_START+1);
      user_code_entry();
    }
  }

  /*Disable interrupts*/
  __disable_interrupt();
    
  /* Init SCSI module */
  ScsiInit();

  /* LUNs Init */
  LunInit(DISK_LUN,(LunFpnt_t*)&Flash_LunFunDesc);
  LunInit(SPIFI_DISK_LUN,(LunFpnt_t*)&Spifi_LunFunDesc);

  /*Enable interrupts*/
  __enable_interrupt();

  /* Init Disk activity LED */
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  /* SCSI init push init disk driver message for an each LUN*/
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
  }
}
/** private functions **/
