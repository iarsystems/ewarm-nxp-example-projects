/*
** ###################################################################
**     Processor:           Vybrid
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    Vybrid RM Rev. 1, Draft B Feb 2012
**     Version:             rev. 1.0, 2012-03-15
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2012 Freescale Semiconductor, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2012-03-15)
**         Initial version
**
** ###################################################################
*/

/**
 * @file Vybrid
 * @version 1.0
 * @date 2012-03-15
 * @brief Device specific configuration file for Vybrid (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MVF50GS10MK50.h"

#define DISABLE_WDOG    1

#define CLOCK_SETUP     0
/* Predefined clock setups
   0 ... Default
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 24000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             24000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            528000000u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            100000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            8000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 2) */


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

__no_init uint32_t SystemCoreClock;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit(void)
{
#if (CLOCK_SETUP == 0)
  // Setup the clocks to run in synchronous mode
  CCM->CLPCR &= ~CCM_CLPCR_FXOSC_PWRDWN_MASK;
  while(!(CCM_CSR_FXOSC_RDY_MASK & CCM->CSR));
  CCM->CCSR |= CCM_CCSR_FAST_CLK_SEL_MASK;

  // using PLL1 PFD3
  //Enable CCM, SRC, and more
  CCM->CCGR4 |= CCM_CCGR4_CG10(1); // WKPU ungate
  CCM->CCGR4 |= CCM_CCGR4_CG11(1); // CCM ungate
  CCM->CCGR4 |= CCM_CCGR4_CG12(1); // GPC ungate
  CCM->CCGR4 |= CCM_CCGR4_CG14(1); // SRC ungate
  CCM->CCGR3 |= CCM_CCGR3_CG0(1);  // ANADIG ungate

  //Enable IOMUX
  CCM->CCGR2 |= CCM_CCGR2_CG8(1);  // IOMUX Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG9(1);  // Port A Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG10(1); // Port B Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG11(1); // Port C Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG12(1); // Port D Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG13(1); // Port E Controller ungate

  /*
  CCM->CCGR0 = 0xFFFFFFFF;
  CCM->CCGR1 = 0xFFFFFFFF;
  CCM->CCGR2 = 0xFFFFFFFF;
  CCM->CCGR3 = 0xFFFFFFFF;
  CCM->CCGR4 = 0xFFFFFFFF;
  CCM->CCGR5 = 0xFFFFFFFF;
  CCM->CCGR6 = 0xFFFFFFFF;
  CCM->CCGR7 = 0xFFFFFFFF;
  CCM->CCGR8 = 0x3FFFFFFF;//Turn off GPU since causes immediate interrupt
  CCM->CCGR9 = 0xFFFFFFFF;
  CCM->CCGR10 = 0xFFFFFFFF;
  CCM->CCGR11 = 0xFFFFFFFF;
 */

  /* enable PLLs in Anadig */
  ANADIG->PLL1_CTRL=0x00002001;
  while(!(ANADIG_PLL1_CTRL_LOCK_MASK & ANADIG->PLL1_CTRL));//wait until lock
  ANADIG->PLL2_CTRL=0x00002001;
  while(!(ANADIG_PLL2_CTRL_LOCK_MASK & ANADIG->PLL2_CTRL));//wait until lock
  ANADIG->PLL4_CTRL=0x00002031; //PLL4
  while(!(ANADIG_PLL4_CTRL_LOCK_MASK & ANADIG->PLL4_CTRL));//wait until lock
  ANADIG->PLL5_CTRL=0x00002001; //PLL5
  while(!(ANADIG_PLL5_CTRL_LOCK_MASK & ANADIG->PLL5_CTRL));//wait until lock
  ANADIG->PLL6_CTRL=0x00002028; //PLL6
  while(!(ANADIG_PLL6_CTRL_LOCK_MASK & ANADIG->PLL6_CTRL));//wait until lock

  //Use 396MHz CA5, 400 MHz DDR, 132MHz CM4, and 66MHz bus
  CCM->CCSR=0x0003FF24; //PLL1 uses PLL1_PFD3, enable all PLL1 and PLL2, select Fast Clock, and sys_clock_sel use PLL1
  CCM->CACRR=0x00000810; //ARM_DIV=0 (div by 1), BUS_DIV=2 (div by 3), ipg_div value is 1 (div by 2)

  SystemCoreClock = DEFAULT_SYSTEM_CLOCK;
  SystemCoreClockUpdate();

#elif (CLOCK_SETUP == 1)

#elif (CLOCK_SETUP == 2)

#endif /* (CLOCK_SETUP == 2) */
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void)
{
  uint8_t sys_clk_select;  /* Variable to store output clock frequency of the MCG module */
  uint8_t temp_select;
  uint32_t MFI = 0;
  uint32_t PLLout = 0;

  uint32_t PLL2_NUM=ANADIG->PLL2_NUM;
  uint32_t PLL2_DENOM=ANADIG->PLL2_DENOM;
  uint32_t PLL1_NUM=ANADIG->PLL1_NUM;
  uint32_t PLL1_DENOM=ANADIG->PLL1_DENOM;

  sys_clk_select = (CCM->CCSR & CCM_CCSR_SYS_CLK_SEL_MASK);

  switch (sys_clk_select)
  {
    case 0:  // CASE 0:  FAST Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      SystemCoreClock = 24000;
      break;
    case 1:  // CASE 1:  SLOW Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      SystemCoreClock = 32;
      break;
    case 2:  // CASE 2:  PLL2 PFD Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      temp_select = (CCM->CCSR & CCM_CCSR_PLL2_PFD_CLK_SEL_MASK);
      MFI = 20 + (2 * (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_DIV_SELECT_MASK));
      switch (temp_select)
      {
        case 0:
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            SystemCoreClock = 24000;
          else
            SystemCoreClock = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          break;
        case 1:  //CASE 1!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
           PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD1_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD1_FRAC_SHIFT);
          break;
        case 2:   //CASE 2!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
         SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD2_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD2_FRAC_SHIFT);
         break;
        case 3:  //CASE 3!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
           PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
         SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD3_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD3_FRAC_SHIFT);
         break;
        case 4:  //CASE 4!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD4_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD4_FRAC_SHIFT);
          break;
        default:
          break;
      }
    case 3:  // CASE 3:  PLL2 Main Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
        SystemCoreClock = 24000;
      else
      {
        MFI = 20 + (2 * (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_DIV_SELECT_MASK));
        SystemCoreClock = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
      }
      break;
    case 4:  // CASE 4:  PLL1 PFD Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      temp_select = (CCM->CCSR & CCM_CCSR_PLL1_PFD_CLK_SEL_MASK)>>CCM_CCSR_PLL1_PFD_CLK_SEL_SHIFT ;
      MFI = 20 + (2 * (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_DIV_SELECT_MASK));
      switch (temp_select)
      {
        case 0:
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            SystemCoreClock = 24000;
          else
            SystemCoreClock = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          break;
        case 1:  //CASE 1!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD1_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD1_FRAC_SHIFT);
          break;
        case 2:   //CASE 2!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD2_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD2_FRAC_SHIFT);
          break;
        case 3:  //CASE 3!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18)/ ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD3_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD3_FRAC_SHIFT);
          break;
        case 4:  //CASE 4!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD4_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD4_FRAC_SHIFT);
          break;
        default:
          break;
      }
      break;
    case 5:
      SystemCoreClock = 480000;
      break;
    default:
      break;
  }
}
