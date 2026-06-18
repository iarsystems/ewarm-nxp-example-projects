/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : drv_clk.c
 *    Description : NXP LPC1342 Clock Drv
 *
 *    History :
 *    1. Date        : January 2012
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <NXP\iolpc1342.h>
#include "drv_clk.h"
#include "board.h"
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/
const uint32_t Fclkana[16] = {
  0,
  500000UL,
  800000UL,
  1100000UL,
  1400000UL,
  1600000UL,
  1800000UL,
  2000000UL,
  2200000UL,
  2400000UL,
  2600000UL,
  2700000UL,
  2900000UL,
  3100000UL,
  3200000UL,
  3400000UL,
};
/** public functions **/
/*************************************************************************
 * Function Name: CLK_MainOscSet
 * Parameters: clk_enable_t enable
 *             clk_enable_t osc_bypass
 *             clk_moscrng_t range             
 *
 * Return: void
 *
 * Description: Select main osc range and mode.
  *             Use this functions only when main clock is taken from IRC
 *
 *************************************************************************/
void CLK_MainOscSet(clk_enable_t enable, clk_enable_t osc_bypass, clk_moscrng_t range)
{
  /*power down Mian oscilator*/
  PDRUNCFG_bit.SYSOSC_PD = 1;
  if(CLK_ENABLE == enable)
  {
    /*Set oscilator mode*/
    if(CLK_ENABLE == osc_bypass)
    {
      SYSOSCCTRL_bit.BYPASS = 1;  /*Bypass enabled. PLL input (sys_osc_clk) is fed
                                    directly from the XTALIN and XTALOUT pins */
    }
    else
    {
      SYSOSCCTRL_bit.BYPASS = 0;  /*Oscillator is not bypassed*/
    }
    /*Set osclilator range*/
    SYSOSCCTRL_bit.FREQRANGE = range;
    /*Power on Main oscilator*/
    PDRUNCFG_bit.SYSOSC_PD = 0;   
  }
}

/*************************************************************************
 * Function Name: CLK_SetWDTOsc
 * Parameters: clk_enable_t wdtenable
 *             uint32_t freq
 *             uint32_t div
 *             
 * Return: void
 *
 * Description: Set WDT Osc frequency and divider
 *
 *************************************************************************/
void CLK_SetWDTOsc(clk_enable_t wdtenable, uint32_t freq, uint32_t div)
{
  /*power down WDT oscilator*/
  PDRUNCFG_bit.WDTOSC_PD = 1;
  if(CLK_ENABLE == wdtenable)
  {
    /* Set WDT Osc frequency and divider */
    WDTOSCCTRL = (div & 0x1F) | 
                ((freq & 0xF)<<5);
    /*power on WDT oscilator*/
    PDRUNCFG_bit.WDTOSC_PD = 0;
  }
}

/*************************************************************************
 * Function Name: CLK_SetIRCOsc
 * Parameters: clk_enable_t ircenable
 *             
 * Return: void
 *
 * Description: Enable/Disable IRC Osc 
 *
 *************************************************************************/
void CLK_SetIRCOsc(clk_enable_t ircenable)
{
  if(CLK_ENABLE == ircenable)
  {
    /*Power on IRC osc*/
    PDRUNCFG_bit.IRC_PD = 0;
  }
  else
  {
    /*Power down IRC osc*/
    PDRUNCFG_bit.IRC_PD = 1;
  }
}
/*************************************************************************
 * Function Name: CLK_SetPLLInClk
 * Parameters: clk_pllinsel_t pllinsel 
 *             
 * Return: void
 *
 * Description: Select Pll In clock source
 *
 *
 *************************************************************************/
void CLK_SetPLLInClk(clk_pllinsel_t pllinsel)
{  
  /*Clear update bit*/
  SYSPLLCLKUEN = 0;
  /*Select pllin clock source*/
  SYSPLLCLKSEL_bit.SEL = pllinsel; 
  /*Set update bit*/
  SYSPLLCLKUEN = 1;
}

/*************************************************************************
 * Function Name: CLK_SetSysClkDiv
 * Parameters:clk_mainsel_t mainsel 
 *            uint32_t sysdiv
 *             
 * Return: void
 *
 * Description: Select main clock and set sys divider
 *              
 *
 *************************************************************************/
void CLK_SetMainClk(clk_mainsel_t mainsel, uint32_t sysdiv)
{
  /*Clear update bit*/
  MAINCLKUEN = 0;
  /* Select IRC OSC as main clock*/
  MAINCLKSEL_bit.SEL = MAINSEL_IRCOSC;
  /*Set update bit*/
  MAINCLKUEN = 1;
  /* Set sys ahb divider*/
  SYSAHBCLKDIV_bit.DIV  = sysdiv & 0xFF; 
  /*Clear update bit*/
  MAINCLKUEN = 0;
  /* Select main clock */
  MAINCLKSEL_bit.SEL = mainsel; 
  /*Set update bit*/
  MAINCLKUEN = 1;
}

/*************************************************************************
 * Function Name: CLK_SetSSP0Clk
 * Parameters: uint32_t div
 *             
 * Return: void
 *
 * Description: set SSP0 clock divider
 *
 *
 *************************************************************************/
void CLK_SetSSP0Clk(uint32_t div)
{
  /*SSP0 clock divider set*/
  SSP0CLKDIV_bit.DIV = div & 0xFF;
}

/*************************************************************************
 * Function Name: CLK_SetUARTClk
 * Parameters: uint32_t div
 *             
 * Return: void
 *
 * Description: set UART clock divider
 *
 *
 *************************************************************************/
void CLK_SetUARTClk(uint32_t div)
{
  /*UART clock divider set*/
  UARTCLKDIV_bit.DIV = div & 0xFF;
}

/*************************************************************************
 * Function Name: CLK_SetUsbClk
 * Parameters: clk_usbsel_t cpusel
 *             uint32_t div
 *             
 * Return: void
 *
 * Description: Select usb clock and set usb divider
 *              
 *
 *************************************************************************/
void CLK_SetUsbClk(clk_usbsel_t usbsel, uint32_t div)
{
  /*Clear update bit*/
  USBCLKUEN = 0;
  /* Select USB clock */
  USBCLKSEL_bit.SEL = usbsel; 
  /*Set update bit*/
  USBCLKUEN = 1;
  /* Set USB divider*/
  USBCLKDIV_bit.DIV = div & 0xFF; 
}

/*************************************************************************
 * Function Name: CLK_SetOutClk
 * Parameters: clk_outsel_t outsel
 *             uint32_t div
 *             
 * Return: void
 *
 * Description: set Clock Out
 *              
 *
 *************************************************************************/
void CLK_SetOutClk(clk_outsel_t outsel, uint32_t div)
{
  /*Clear update bit*/
  CLKOUTUEN = 0;
  /* Select Clock Out */
  CLKOUTCLKSEL_bit.SEL = outsel; 
  /*Set update bit*/
  CLKOUTUEN = 1;
  /* Set Clockout divider*/
  CLKOUTDIV_bit.DIV = div & 0xFF; 
}

/*************************************************************************
 * Function Name: CLK_SetPll
 * Parameters: clk_enable_t pllenalbe
 *             uint32_t pllm 
 *             uint32_t pllp 
 *
 * Return: void
 *
 * Description: set pll 
 *
 *
 *************************************************************************/
void CLK_SetPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp)
{
  /*Power Down SYS PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /**/
  if(CLK_ENABLE == pllenable)
  {
    /*Configure PLL */
    SYSPLLCTRL =  (pllm & 0x1F)     |   /*MSEL*/
                  ((pllp & 0x3)<<5) ;   /*PSEL*/
             
    /*Power Up PLL*/
    PDRUNCFG_bit.SYSPLL_PD = 0;
    /*Wain until PLL locks*/
    while(!(SYSPLLSTAT_bit.LOCK));
  }
}

/*************************************************************************
 * Function Name: CLK_SetUsbPll
 * Parameters: clk_enable_t pllenalbe
 *             uint32_t pllm 
 *             uint32_t pllp 
 *
 * Return: void
 *
 * Description: set alt pll 
 *
 *
 *************************************************************************/
void CLK_SetUsbPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp)
{
  /*Power Down USB PLL*/
  PDRUNCFG_bit.USBPLL_PD = 1;
  /**/
  if(CLK_ENABLE == pllenable)
  {
    /*Clear update bit*/
    USBPLLCLKUEN = 0;
    /*Select Main oscilator as USB PLL in clock*/
    USBPLLCLKSEL_bit.SEL = USBPLLSEL_MOSC;
    /*Set update bit*/
    USBPLLCLKUEN = 1;
    /*Configure PLL */
    USBPLLCTRL =  (pllm & 0x1F)     |   /*MSEL*/
                  ((pllp & 0x3)<<5) ;   /*PSEL*/
             
    /*Power Up PLL*/
    PDRUNCFG_bit.USBPLL_PD = 0;
    /*Wain until PLL locks*/
    while(!(USBPLLSTAT_bit.LOCK));
  }
}

/*************************************************************************
 * Function Name: CLK_GetClock
 * Parameters: clk_t clock - clock to get
 *
 * Return: int clock frequency in Hz 
 *
 * Description: get clock frequency
 *
 *************************************************************************/
uint32_t CLK_GetClock(clk_t clock)
{
uint32_t freq = 0;
  
  switch(clock)
  {
  case  CLK_PLLIN:
    switch(SYSPLLCLKSEL_bit.SEL)
    {
    case PLLINSEL_IRCOSC:
      /*IRC is on*/
      if(0 == PDRUNCFG_bit.IRC_PD)
      {
        freq = IRCOSC;
      }
      break;
    case PLLINSEL_MOSC:
      /*Main oscilator is on*/
      if(0 == PDRUNCFG_bit.SYSOSC_PD)
      {
        freq = MOSC;
      }
      break;
    }

    break;

  case CLK_PLL:
      /*SYS PLL is on*/
      if(0 == PDRUNCFG_bit.SYSPLL_PD)
      {
        freq = CLK_GetClock(CLK_PLLIN) * (SYSPLLCTRL_bit.MSEL+1);
      }
    break;
    
  case  CLK_USBPLL:
      /*USB PLL is on*/
      if(0 == PDRUNCFG_bit.USBPLL_PD)
      {
        freq = MOSC * (USBPLLCTRL_bit.MSEL+1);
      }
    break;
    
  case CLK_MAIN:
    switch(MAINCLKSEL_bit.SEL)
    {
    case MAINSEL_IRCOSC:
        freq = IRCOSC;
      break;
    case MAINSEL_CLKPLLIN:
        freq = CLK_GetClock(CLK_PLLIN);
      break;
    case MAINSEL_WDTOSC:
        freq = CLK_GetClock(CLK_WDTOSC);
      break;
    case MAINSEL_CLKPLL:
        freq = CLK_GetClock(CLK_PLL);
      break;
    }
    break;
    
  case CLK_SYS:
    freq = CLK_GetClock(CLK_MAIN)/SYSAHBCLKDIV_bit.DIV;     
    break;
    
  case CLK_USB:
    /*USB divider is on*/
    if(USBCLKDIV_bit.DIV)
    {
      switch(USBCLKSEL_bit.SEL)
      {
      case USBSEL_CLKUSBPLL:
        freq = CLK_GetClock(CLK_USBPLL);
        break;
      case USBSEL_CLKMAIN:
        freq = CLK_GetClock(CLK_MAIN);
        break;
      }
      freq /= USBCLKDIV_bit.DIV;
    }
    break;
    
  case CLK_SSP0:
    /*SSP0 divider is on*/
    if(SSP0CLKDIV_bit.DIV)
    {
      freq = CLK_GetClock(CLK_MAIN)/SSP0CLKDIV_bit.DIV;
    }
    break;

  case CLK_UART:
    /*UART divider is on*/
    if(UARTCLKDIV_bit.DIV)
    {
      freq = CLK_GetClock(CLK_MAIN)/UARTCLKDIV_bit.DIV;
    }
    break;
    
  case CLK_CLKOUT:
    /*USB divider is on*/
    if(CLKOUTDIV_bit.DIV)
    {
      switch(CLKOUTCLKSEL_bit.SEL)
      {
      case OUTSEL_IRCOSC:
        freq = IRCOSC;
        break;
      case OUTSEL_CLKMAIN:
        freq = CLK_GetClock(CLK_MAIN);
        break;
      case OUTSEL_WDTOSC:
        freq = CLK_GetClock(CLK_WDTOSC);
        break;
      }
      freq /= CLKOUTDIV_bit.DIV;
    }
    break;

  case CLK_WDTOSC:
    /*WDT Osc is on*/
    if(0 == PDRUNCFG_bit.WDTOSC_PD)
    {
      freq = Fclkana[WDTOSCCTRL_bit.FREQSEL]/(2*(1 + WDTOSCCTRL_bit.DIVSEL));
    }
    break;
  }
  
  return freq;
}
