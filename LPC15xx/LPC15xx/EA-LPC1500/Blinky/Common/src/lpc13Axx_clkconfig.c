/****************************************************************************
 *   $Id: lpc13Axx_clkconfig.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx Clock Configuration example
 *
 *   Description:
 *     This file contains clock configuration code example which include 
 *     watchdog setup and debug clock out setup.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "LPC13Axx.h"
#include "lpc13Axx_clkconfig.h"

/*****************************************************************************
** Function name:		WDT_CLK_Setup
**
** Descriptions:		Configure WDT clock.
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void WDT_CLK_Setup ( void )
{
  /* Freq = 0.5Mhz, div_sel is 0x1F, divided by 64. WDT_OSC should be 7.8125khz */
  LPC_SYSCON->WDTOSCCTRL = (0x1<<5)|0x1F;
  LPC_SYSCON->PDRUNCFG &= ~PDEN_WDT_OSC;    /* Let WDT clock run */

  /* Enables clock for WDT */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_WDT;
  return;
}

/*****************************************************************************
** Function name:		CLKOUT_Setup
**
** Descriptions:		Configure CLKOUT for reference clock check.
** parameters:			clock source: irc_osc(0), sys_osc(1), wdt_osc(2),
**						main_clk(3).			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void CLKOUT_Setup ( int32_t clksrc )
{
	if ( (clksrc >= CLKOUTCLK_SRC_IRC_OSC) && (clksrc <= CLKOUTCLK_SRC_MAIN_CLK) )
  {
    LPC_SYSCON->CLKOUTCLKSELA = clksrc;
    LPC_SYSCON->CLKOUTCLKSELB = 0;
  }
  else if (clksrc == CLKOUTCLK_SRC_SCT_PLL0 )
  {
    LPC_SYSCON->CLKOUTCLKSELB = CLKOUTB_SCT_PLL0;
  }
  else if (clksrc == CLKOUTCLK_SRC_ULP32K_CLK )
  {
    LPC_SYSCON->CLKOUTCLKSELB = CLKOUTB_ULP32K_CLK;
  }
  LPC_SYSCON->CLKOUTDIV = 1;			/* Divided by 1 */
  return;
}

/*****************************************************************************
** Function name:		SCT_PLL_Setup
**
** Descriptions:		Configure SCT PLL clock.
** parameters:			clksrc_sel: 0 or 1 only
** 						
** Returned value:		None
** 
*****************************************************************************/
void SCT_PLL_Setup( uint32_t clksrc_sel, uint32_t sctpll_ctrl )
{
  if ( (clksrc_sel == SCTPLL_CLKSRC_IRC_OSC) || (clksrc_sel == SCTPLL_CLKSRC_SYS_OSC) ) {
		LPC_SYSCON->SCTPLLCLKSEL  = clksrc_sel;    /* Select PLL Input          */
	}
	else {
		/* illegal SCT clock source selection. */
		while ( 1 );
	}
  LPC_SYSCON->SCTPLLCTRL    = sctpll_ctrl;
  LPC_SYSCON->PDRUNCFG     &= ~PDEN_SCT_PLL;       /* Power-up SCT PLL          */
  while (!(LPC_SYSCON->SCTPLLSTAT & 0x01));	       /* Wait Until PLL Locked     */
	/* To monitor SCT PLL clock out */
	CLKOUT_Setup ( CLKOUTCLK_SRC_SCT_PLL0 );
  return;
}
