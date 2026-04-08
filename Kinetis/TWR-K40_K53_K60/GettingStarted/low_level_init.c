/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : low_level_init.c
 *    Description : Low level init procedure
 *
 *    History :
 *    1. Date        : 08, September 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
#if defined(KL25)
  /*Disable COP Watchdog*/
  SIM_COPC = 0;
#else
  unsigned int reg;
  /*Disable WWDT*/
  reg = WDOG_STCTRLH;
  reg &= ~1UL;
  /*Unlock sequence*/
  WDOG_UNLOCK  = 0xC520;
  WDOG_UNLOCK  = 0xD928;
  WDOG_STCTRLH = reg;  
#endif
  
  /* perform initialization*/
#if defined(KE15)
  /* Assign PE9 and PE10 to ext. OSC*/
  SIM_SCGC6 |= SIM_SCGC6_PORTE_MASK;
  PORTE_PCR9  = PORT_PCR_MUX(0);
  PORTE_PCR10 = PORT_PCR_MUX(0);

  OSC_CR = OSC_CR_HGO_MASK | OSC_CR_OSCOS_MASK | OSC_CR_OSCEN_MASK | (1U << 2) /*RANGE - High*/;
  while(!(OSC_CR_OSCINIT_MASK & OSC_CR));
  /* move to FEE mode 72MHZ, Bus 36MHz */
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV4(0);
  ICS_C2 = ICS_C2_BDIV(0) | ICS_C2_FRDIV(3);
  ICS_C1 = ICS_C1_RDIV(7);
#else
#if defined(K40) || defined(K20)
  /* DIV1-4 = /1*/
  SIM_CLKDIV1 = 0x00000000;
  MCG_C2 = (MCG_C2 & MCG_C2_RANGE_MASK) | MCG_C2_RANGE(1) | MCG_C2_EREFS_MASK;
#elif defined(K21) || defined(KL25)
  /* DIV1-4 = /1*/
  SIM_CLKDIV1 = 0x00000000;
  MCG_C2 = (MCG_C2 & MCG_C2_RANGE0_MASK) | MCG_C2_RANGE0(1) | MCG_C2_EREFS0_MASK;
#else
  /* DIV1-4 = /2*/
  SIM_CLKDIV1 = 0x11110000;
#endif
  MCG_C1 = (MCG_C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) | MCG_C1_CLKS(2);
#endif

  return 1;
}

