/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : drv_clk.h
 *    Description : NXP LPC1342 Clock Drv header file
 *
 *    History :
 *    1. Date        : January 2012
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#ifndef __DRV_CLK_H
#define __DRV_CLK_H

/** definitions **/
#define IRCOSC  (12000000UL)
/*clk typedef*/
typedef enum
{
  CLK_PLLIN,
  CLK_PLL,
  CLK_USBPLL,
  CLK_MAIN,
  CLK_SYS,
  CLK_USB,
  CLK_SSP0,
  CLK_UART,
  CLK_CLKOUT,
  CLK_WDTOSC,
  CLK_WDT,
} clk_t;

/*sysclk select typedef*/
typedef enum
{
  PLLINSEL_IRCOSC = 0,      /* Internal RC oscilator*/
  PLLINSEL_MOSC,            /* Main oscilator*/
} clk_pllinsel_t;

/* main clock sel typedef */
typedef enum
{
  MAINSEL_IRCOSC = 0,       /* select IRC OSC as main source */
  MAINSEL_CLKPLLIN,         /* select pllin clock as main source */
  MAINSEL_WDTOSC,           /* select WDT OSC  as main source */
  MAINSEL_CLKPLL,           /* select pll clock as main source */
} clk_mainsel_t;

/* usbpll clock sel typedef */
typedef enum
{
  USBPLLSEL_IRCOSC = 0,       /* select IRC OSC as usbpll source */
  USBPLLSEL_MOSC,             /* select Main oscilator as usbpll soruce */
} clk_usbpllsel_t;

/* usb clock sel typedef */
typedef enum
{
  USBSEL_CLKUSBPLL = 0,       /* select usbpll clock as usb source */
  USBSEL_CLKMAIN,             /* select main clock as usb soruce */
} clk_usbsel_t;

/* clockout sel typedef */
typedef enum
{
  OUTSEL_IRCOSC,           /* select internal RC oscilator as clock out */
  OUTSEL_MOSC,             /* select main oscilator as clock out */
  OUTSEL_WDTOSC,           /* select WDT oscilator as clock out */
  OUTSEL_CLKMAIN,          /* select main clock as clock out */
} clk_outsel_t;

/* main oscilator range typedef */
typedef enum
{
  MOSCRNG_1_20MHZ = 0,     /* The frequency range of the main oscillator is 1 MHz to 20 MHz*/
  MOSCRNG_15_25MHZ         /* The frequency range of the main oscillator is 15 MHz to 25 MHz.*/
} clk_moscrng_t;

/*clock module enable disable typedef*/
typedef enum
{
  CLK_ENABLE,             /**/
  CLK_DISABLE             /**/
} clk_enable_t;

/** default settings **/

/** public data **/

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
void CLK_MainOscSet(clk_enable_t enable, clk_enable_t osc_bypass, clk_moscrng_t range);

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
void CLK_SetWDTOsc(clk_enable_t wdtenable, uint32_t freq, uint32_t div);

/*************************************************************************
 * Function Name: CLK_SetIRCOsc
 * Parameters: clk_enable_t ircenable
 *             
 * Return: void
 *
 * Description: Enable/Disable IRC Osc 
 *
 *************************************************************************/
void CLK_SetIRCOsc(clk_enable_t ircenable);

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
void CLK_SetPLLInClk(clk_pllinsel_t pllinsel);

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
void CLK_SetMainClk(clk_mainsel_t mainsel, uint32_t sysdiv);

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
void CLK_SetSSP0Clk(uint32_t div);

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
void CLK_SetUARTClk(uint32_t div);

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
void CLK_SetUsbClk(clk_usbsel_t usbsel, uint32_t div);

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
void CLK_SetOutClk(clk_outsel_t outsel, uint32_t div);

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
void CLK_SetPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp);

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
void CLK_SetUsbPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp);

/*************************************************************************
 * Function Name: CLK_GetClock
 * Parameters: clk_t clock - clock to get
 *
 * Return: int clock frequency in Hz 
 *
 * Description: get clock frequency
 *
 *************************************************************************/
uint32_t CLK_GetClock(clk_t clock);

#endif /* __DRV_CLK_H */
