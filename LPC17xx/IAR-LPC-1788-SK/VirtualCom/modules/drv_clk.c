/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : drv_clk.c
 *    Description : NXP LPC1788 Clock Drv
 *
 *    History :
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <NXP\iolpc1788.h>
#include "drv_clk.h"
#include "board.h"
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/
/*************************************************************************
 * Function Name: CLK_MainOscSet
 * Parameters: clk_enable_t osc_enable
 *             clk_moscrng_t range
 *
 * Return: void
 *
 * Description: Main Oscilator enable/disable. Select main osc range
 *              Use this functions only when sys clock is taken from IRC
 *
 *************************************************************************/
void CLK_MainOscSet(clk_enable_t osc_enable, clk_moscrng_t range)
{
  /*Disable oscilator*/
  SCS_bit.OSCEN = 0;
  /*Set osclilator range*/
  SCS_bit.OSCRANGE = range;

  if(CLK_ENABLE == osc_enable)
  {/*Enable main osc*/
    SCS_bit.OSCEN = 1;
    /*Wait for OSC ready*/
    while(!SCS_bit.OSCSTAT);
  }
}

/*************************************************************************
 * Function Name: CLK_SetSysClk
 * Parameters: clk_syssel_t syssel - select
 *
 * Return: void
 *
 * Description: Select sys clock and PLL0 clock source
 *
 *
 *************************************************************************/
void CLK_SetSysClk(clk_syssel_t syssel)
{
  /*Select sysclk and PLL0 clock source*/
  CLKSRCSEL_bit.CLKSRC = syssel;
}

/*************************************************************************
 * Function Name: CLK_SetCpuClk
 * Parameters: clk_cpusel_t cpusel
 *             uint32_t div
 *
 * Return: void
 *
 * Description: Select cpu clock and set cpu divider
 *
 *
 *************************************************************************/
void CLK_SetCpuClk(clk_cpusel_t cpusel, uint32_t div)
{
  /* Sysclk is used as the input to the CPU clock divider*/
  CCLKSEL_bit.CCLKSEL = 0;
  /* Set cpu divider*/
  CCLKSEL_bit.CCLKDIV = div & 0x1F;
  /* Select CPU clock */
  CCLKSEL_bit.CCLKSEL = cpusel;
}

/*************************************************************************
 * Function Name: CLK_SetPeriphClk
 * Parameters: uint32_t div
 *
 * Return: void
 *
 * Description: set peripheral clock divider
 *
 *
 *************************************************************************/
void CLK_SetPeriphClk(uint32_t div)
{
  /*Peripheral clock divider set*/
  PCLKSEL_bit.PCLKDIV = div & 0x1F;
}

/*************************************************************************
 * Function Name: CLK_SetEmcClk
 * Parameters: uint32_t div
 *
 * Return: void
 *
 * Description: set emc clock divider
 *
 *
 *************************************************************************/
void CLK_SetEmcClk(uint32_t div)
{
  /*EMC clock divider set*/
  if(div)
  {/*use cpu clock /2 */
    EMCCLKSEL_bit.EMCDIV = 1;
  }
  else
  {/*use cpu clock*/
    EMCCLKSEL_bit.EMCDIV = 0;
  }
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
  /* Select USB clock */
  USBCLKSEL_bit.USBSEL = usbsel;
  /* Set USB divider*/
  USBCLKSEL_bit.USBDIV = div & 0x1F;
}

/*************************************************************************
 * Function Name: CLK_SetMainPll
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
void CLK_SetMainPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp)
{
  /*Disable Main PLL*/
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  if(CLK_ENABLE == pllenable)
  {
    /*Set PLL */
    PLL0CFG_bit.MSEL = pllm;
    PLL0CFG_bit.PSEL = pllp;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    /*Enable PLL*/
    PLL0CON_bit.PLLE = 1;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    /* Wait for the PLL to achieve lock*/
    while(!PLL0STAT_bit.PLOCK);
  }
}

/*************************************************************************
 * Function Name: CLK_SetAltPll
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
void CLK_SetAltPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp)
{
  /*Disable Main PLL*/
  PLL1CON_bit.PLLE = 0;
  PLL1FEED = 0xAA;
  PLL1FEED = 0x55;
  if(CLK_ENABLE == pllenable)
  {
    /*Set PLL */
    PLL1CFG_bit.MSEL = pllm;
    PLL1CFG_bit.PSEL = pllp;
    PLL1FEED = 0xAA;
    PLL1FEED = 0x55;
    /*Enable PLL*/
    PLL1CON_bit.PLLE = 1;
    PLL1FEED = 0xAA;
    PLL1FEED = 0x55;
    /* Wait for the PLL to achieve lock*/
    while(!PLL1STAT_bit.PLOCK);
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
int CLK_GetClock(clk_t clock)
{
uint32_t freq = 0;

  switch(clock)
  {
  case CLK_SYS:
      if(CLKSRCSEL_bit.CLKSRC)
      {/*Main Oscilator*/
        freq = MOSC;
      }
      else
      {/*Internal oscilator*/
        freq = IRCOSC;
      }
    break;
  case CLK_PLL:
    if(PLL0STAT_bit.PLLE_STAT)
    {/*PLL0 is enabled*/
      freq = CLK_GetClock(CLK_SYS)*(PLL0STAT_bit.MSEL+1);
    }
    break;
  case CLK_ALTPLL:
    if(PLL1STAT_bit.PLLE_STAT)
    {/*PLL1 is enabled*/
      freq = MOSC*(PLL1STAT_bit.MSEL+1);
    }
    break;
  case CLK_CPU:
    if(CCLKSEL_bit.CCLKSEL)
    {/*it is pll clock*/
      freq = CLK_GetClock(CLK_PLL);
    }
    else
    {/*it is sys clock*/
      freq = CLK_GetClock(CLK_SYS);
    }
    freq /= CCLKSEL_bit.CCLKDIV;
    break;
  case CLK_PERIPH:
    if(PCLKSEL_bit.PCLKDIV)
    {/*periphera devider is enabled*/
      if(CCLKSEL_bit.CCLKSEL)
      {/*it is pll clock*/
        freq = CLK_GetClock(CLK_PLL);
      }
      else
      {/*it is sys clock*/
        freq = CLK_GetClock(CLK_SYS);
      }
      freq /= PCLKSEL_bit.PCLKDIV;
    }
    break;
  case CLK_EMC:
    freq = CLK_GetClock(CLK_CPU);
    if(EMCCLKSEL_bit.EMCDIV)
    {/*emc is at half of cpu freqency*/
      freq /= 2;
    }
    break;
  case CLK_USB:
    if(USBCLKSEL_bit.USBDIV)
    {/*usb devider is enabled*/
      switch(USBCLKSEL_bit.USBSEL)
      {
      case 0:
        freq = CLK_GetClock(CLK_SYS);
        break;
      case 1:
        freq = CLK_GetClock(CLK_PLL);
        break;
      case 2:
        freq = CLK_GetClock(CLK_ALTPLL);
        break;
      }

      freq /= USBCLKSEL_bit.USBDIV;
    }
    break;
  }

  return freq;
}
