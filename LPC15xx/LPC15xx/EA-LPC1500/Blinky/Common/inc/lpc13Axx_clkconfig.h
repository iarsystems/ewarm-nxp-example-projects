/****************************************************************************
 *   $Id: lpc13Axx_clkconfig.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC8xx software example
 *
 *   Description:
 *     This file contains definition and prototype for clock configuration.
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
#ifndef __CLKCONFIG_H 
#define __CLKCONFIG_H

#define CLKOUTCLK_SRC_IRC_OSC       0
#define CLKOUTCLK_SRC_SYS_OSC       1
#define CLKOUTCLK_SRC_WDT_OSC       2
#define CLKOUTCLK_SRC_MAIN_CLK      3
#define CLKOUTCLK_SRC_SCT_PLL0      4
#define CLKOUTCLK_SRC_ULP32K_CLK    5

#define SCTPLL_CLKSRC_IRC_OSC       0
#define SCTPLL_CLKSRC_SYS_OSC       1

#define CLKOUTB_SCT_PLL0            2
#define CLKOUTB_ULP32K_CLK          3

void WDT_CLK_Setup(void);
void CLKOUT_Setup( int32_t clksrc );
void SCT_PLL_Setup( uint32_t clksrc_sel, uint32_t sctpll_ctrl );
#endif /* end __CLKCONFIG_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
