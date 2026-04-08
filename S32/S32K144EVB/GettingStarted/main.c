/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : October 2015
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for Freescale Sk32144EVB-Q100 board. It shows basic use
 * of GPIO and the System Timer.
 * It toggles the Blue Color of the triple LED D11.
 *
 *  Note: The default flash loader will prevent writing at addresses
 * 0x408-0x40F and will program the FSEC and FOPT bytes with 0xFE and 0x7D
 * respectively. This will keep the microcontroller unsecured and will allow it
 * to boot from the flash memory. If you need to program the FSEC, FOPT, FEPROT 
 * and FDPROT you must use the flash loader with --enable_config_write parameter
 *
 *    $Revision: $
 **************************************************************************/
#include <intrinsics.h>
#include "device_registers.h" 

void SysTick_Handler(void ) {
  PTD->PTOR |= 0x1;  // Toggle Blue
}

int main()
{
   __disable_interrupt();

  S32_SysTick->RVR = 0x00FFFFFF; /* set SysTick timer reload value*/    
  S32_SysTick->CSR = 0x7;        /* start SysTick timer with Interrupt using core clock */

  __enable_interrupt();  
    
  //set Peripheral Clock Control for PORTD
  PCC->PCCn[PCC_PORTD_INDEX] |= 0x40000000;
  
  // set Pin Control Register
  PORTD->PCR[0]  = 0x00000100;
  PORTD->PCR[15] = 0x00000100;
  PORTD->PCR[16] = 0x00000100;
  
  // set Port Data Direction Register 
  PTD->PDDR |= (0x1<<0) | (0x1<<15) | (0x1<<16);

  // set PORT output 
  PTD->PSOR |= (0x1<<0) | (0x1<<15) | (0x1<<16);
    
  while( 1 ) {
  }
    
  return 0;
}
