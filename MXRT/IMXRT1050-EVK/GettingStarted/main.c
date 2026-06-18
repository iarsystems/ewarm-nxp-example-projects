/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : October 2017
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for NXP IMXRT1050-EVK board. It shows basic use
 * of GPIO and the System Timer.
 * It toggles the Green LED D18.
 *
 *    $Revision: $
 **************************************************************************/
#include <NXP\MIMXRT1052.h>

#define SysTick_CSR (*((uint32_t *) 0xE000E010))
#define SysTick_RVR (*((uint32_t *) 0xE000E014))

#if (defined (__FLASH__) || defined (__QSPI_FLASH__))
__ramfunc
#endif
void SysTick_Handler(void )
{
  GPIO1_DR ^= (1<<9); /*toggle pin*/
}

int main()
{
  /*enable IMUX clock*/
  CCM_CCGR4 |= (3<<2);
  /*make GPIO_AD_B0_09 pin GPIO*/
  IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_09 = 5;
  /*Init pin*/
  IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_09 = 0x000010B0;
  /*enable GPIO1 clock*/
  CCM_CCGR1 |= (3<<26);
  /*make GPIO1_IO09 output*/
  GPIO1_GDIR |= (1<<9);
  
  SysTick_RVR = 0x00FFFFFF;     /* set SysTick timer reload value*/    
  SysTick_CSR = 0x7;            /* start SysTick timer with Interrupt using core clock */
    
  while( 1 ) {
  }
    
  return 0;
}
