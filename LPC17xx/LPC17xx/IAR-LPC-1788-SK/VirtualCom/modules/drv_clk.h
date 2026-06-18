/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : drv_clk.h
 *    Description : NXP LPC1788 Clock Drv header file
 *
 *    History :
 *    1. Date        : June 2011
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
#define WDTOSC  (500000UL)
/*clk typedef*/
typedef enum
{
  CLK_SYS,
  CLK_PLL,
  CLK_ALTPLL,
  CLK_CPU,
  CLK_PERIPH,
  CLK_USB,
  CLK_EMC
} clk_t;

/*sysclk select typedef*/
typedef enum
{
  SYSSEL_IRCOSC = 0,      /* Internal RC oscilator*/
  SYSSEL_MOSC,            /* Main oscilator*/
} clk_syssel_t;

/* cpu clock sel typedef */
typedef enum
{
  CPUSEL_CLKSYS = 0,       /* select sys clock as cpu source */
  CPUSEL_CLKPLL            /* select pll clock as cpu soruce */
} clk_cpusel_t;

/* usb clock sel typedef */
typedef enum
{
  USBSEL_CLKSYS = 0,       /* select sys clock as usb source */
  USBSEL_CLKPLL,           /* select pll clock as usb soruce */
  USBSEL_CLKALTPLL,        /* select alt pll clock as usb soruce */
} clk_usbsel_t;

/* clockout sel typedef */
typedef enum
{
  OUTSEL_CLKCPU = 0,       /* select cpu clock as clock out*/
  OUTSEL_MOSC,             /* select main oscilator as clock out */
  OUTSEL_IRCOSC,           /* select internal RC oscilator as clock out */
  OUTSEL_CLKUSB,           /* select usb clock as clock out */
  OUTSEL_RTCOSC,           /* select RTC oscilator as clock out */
  OUTSEL_WDTOSC = 6        /* select WDT oscilator as clock out */
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
 * Parameters: clk_enable_t osc_enable
 *             clk_moscrng_t range
 *
 * Return: void
 *
 * Description: Main Oscilator enable/disable. Select main osc range
 *              Use this functions only when sys clock is taken from IRC
 *
 *************************************************************************/
void CLK_MainOscSet(clk_enable_t osc_enable, clk_moscrng_t range);

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
void CLK_SetSysClk(clk_syssel_t syssel);

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
void CLK_SetCpuClk(clk_cpusel_t cpusel, uint32_t div);

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
void CLK_SetPeriphClk(uint32_t div);

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
void CLK_SetEmcClk(uint32_t div);

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
void CLK_SetMainPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp);

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
void CLK_SetAltPll(clk_enable_t pllenable, uint32_t pllm, uint32_t pllp);

/*************************************************************************
 * Function Name: CLK_GetClock
 * Parameters: clk_t clock - clock to get
 *
 * Return: int clock frequency in Hz
 *
 * Description: get clock frequency
 *
 *************************************************************************/
int CLK_GetClock(clk_t clock);



#endif /* __DRV_CLK_H */
