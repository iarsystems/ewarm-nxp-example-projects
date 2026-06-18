/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description :
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for for a Embedded Artsists LPC313x evaluation board.
 *   It implements a RAM based 32MB drive. The first free drive letters will be
 *  used. For example, if your PC configuration includes two hard disk partitions
 *  (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 *  appear as F:\.
 *  The GPIO_BOOT2 LED will indicate drive activity.
 *
 *COMPATIBILITY
 *=============
 *
 *   The USB Mass storage example project is compatible with Embedded Artsists
 *  LPC313x evaluation board. By default the project is configured to use the
 *  J-Link JTAG interface.
 *
 *CONFIGURATION
 *=============
 *
 *  The Project contains the following configurations:
 *
 *  Debud_SDRAM: This configuration is intended for debugging in the external
 *               SDRAM.
 *
 * Debug_SPINOR_Boot_SDRAM: This confuguration will download the example in the
 *                          SPI NOR flash. The application is linked to run in
 *                          the external SDRAM and a second level bootloader is
 *                          linked to run in the internal RAM. The IROM
 *                          bootloader copies the secon level bootloader from
 *                          the SPI NOR to the interan RAM and starts it.
 *                          The second level bootloader copies the rest of the
 *                          application to SDRAM. The configuration uses
 *                          FlashEmbArtLPC313x_boot.xml flash loader to program
 *                          the secon level bootloader and
 *                          FlashEmbArtLPC313x_img.xml flash loader to program
 *                          the application.
 *
 *                          Note: This configuration uses a Hardware reset with
 *                          delay to start IROM bootloader and allow it to copy
 *                          the second level bootloader. The boot jumpers should
 *                          be configured for SPI NOR flash boot.
 *
 *  Jumpers:
 *   (Boot from SPI Flash)
 *    BOOT0      - L
 *    BOOT1      - L
 *    BOOT2      - H
 *
 *    History :
 *    1. Date        : 22.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include "includes.h"

/** local definitions **/
#define ISROM_MMU_TTBL  0x1201C000
#define SDRAM_BASE_ADDR 0x30000000
#define IRAM_BASE_ADDR  0x11028000

#define GPIO2_BOOT_SET  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_SET  = 4;}
#define GPIO2_BOOT_CLR  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_CLR  = 4;}

#define LED_ON()    GPIO2_BOOT_SET
#define LED_OFF()   GPIO2_BOOT_CLR

#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

/** external functions **/
extern void InitSDRAM(void);
extern void Dly_us(Int32U Dly);

/** internal functions **/

/*************************************************************************
 * Function Name: main
 * Parameters: None
 *
 * Return: None
 *
 * Description: Getting Started main loop
 *
 *************************************************************************/
void main(void)
{

  CP15_Mmu(FALSE);            // Disable MMU
  CP15_ICache(TRUE);          // Enable ICache

  // Init MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_SetTtb((Int32U *)ISROM_MMU_TTBL);  //Set translation table base address
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

#ifndef DBG_SDRAM
  /*Init Clocks only if debug is not in SDRAM.
    if DBG_SDRAM the Clock is init by bootloader
    or mac file*/
  InitClock();
  /*Init SDRAM only if debug is not in SDRAM.
    if DBG_SDRAM the SDRAM is init by bootloader
    or mac file*/
  InitSDRAM();
#endif /*DBG_SDRAM*/

  /*Init SPI Nor Flash*/
  InitSPINOR();
  /*Init Interrupt Controller.
    Arm Vector Copy to beginnint of the IRAM*/
  INTC_Init((Int32U *)IRAM_BASE_ADDR);
  /*Remap IRAM at address 0*/
  SYSCREG_ARM926_901616_LP_SHADOW_POINT = IRAM_BASE_ADDR;

  LunInit(RAM_DISK_LUN,RamDiskInit,RamDiskStatus,RamDiskIO,RamDiskInfo);

  // Init SCSI module
  ScsiInit();

  __enable_irq();

  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    if(LunImp(i))
    {
      LED_ON();
    }
    else
    {
      LED_OFF();
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
        LED_ON();
      }
      else
      {
        LED_OFF();
      }
    }
  }
}


