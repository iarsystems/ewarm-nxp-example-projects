/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *
 *    Description:
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *    to develop code for for the Embedded Artists LPC3250 evaluation board.
 *    It implements a RAM based 15MB drive. The first available drive letter
 *    will be used. For example, if your PC configuration includes two hard disk
 *    partitions (C: and D:) and a CD-ROM drive (E:), the memory card drive will
 *    appear as F:.
 *
 *    A LED will indicate drive activity.
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include "includes.h"

#define OSC           (13000000UL)  // System OSC 13MHz

#define CORE_CLK      (OSC*16)      // ARM_CLK 208MHz
#define AHB_CLK       (CORE_CLK/2)  // HCLK 104MHz
#define PER_CLK       (CORE_CLK/16) // PER_CLK 13MHz
#define RTC_CLK       (32768UL)     // RTC_CLK

#define LED1          (1UL << 1)

#define LED_ON()      (P3_OUTP_SET = LED1)
#define LED_OFF()     (P3_OUTP_CLR = LED1)

#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

volatile Boolean Tick = FALSE;

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
  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache
  
  // Init Interrupt Controller.
  INTC_Init();

  // Init RAM disk
  LunInit(RAM_DISK_LUN,RamDiskInit,RamDiskStatus,RamDiskIO,RamDiskInfo);

  // Init SCSI module
  ScsiInit();

  // Enable interrupts
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
