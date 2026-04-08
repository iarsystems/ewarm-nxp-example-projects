/***********************************************************************
 * $Id: ea3250_startup.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Embedded Artists LPC3250 board startup code
 *
 * Description:
 *     This file contains startup code used with the Embedded Artists
 *     LPC3250 board.
 *
 ***********************************************************************
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
 **********************************************************************/

#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_startup.h"
#include "ea3250_board.h"
#include "lpc32xx_emc.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_i2c.h"

int lowslew = 1;

struct clkfreqs {
	UNS_32 freq;
	UNS_32 div;
};

#define MAXFREQS 6
struct clkfreqs freqstouse[MAXFREQS] = {
	{266000000, 20},
	{250000000, 19},
	{233000000, 18},
	{225000000, 17},
	{215000000, 17},
	{208000000, 16}
};
UNS_32 realrates[MAXFREQS];

/*
 * LTC3447 is an I2C controllable volgate regulator
 */

#define LTC3447_I2C_ADDR 0x66 

/* 
 * Core voltage values. Note that 0.9V is valid if running in low-power 
 * mode (13 MHz), but then you would have to do some more changes
 * to the initialization code. 
 */
typedef enum
{
  LTC3447_VOLTAGE_1_20V = 0x18,
  LTC3447_VOLTAGE_MIN   = LTC3447_VOLTAGE_1_20V,
  LTC3447_VOLTAGE_1_22V,
  LTC3447_VOLTAGE_1_24V,
  LTC3447_VOLTAGE_1_26V,
  LTC3447_VOLTAGE_1_28V,
  LTC3447_VOLTAGE_1_30V,
  LTC3447_VOLTAGE_1_33V,
  LTC3447_VOLTAGE_1_35V,
  LTC3447_INVALID
} LTC3447_VOLTAGE_T;


/***********************************************************************
 * Startup code private data
 **********************************************************************/

/* Embedded Artists LPC3250 MMU virtual mapping table */


TT_SECTION_BLOCK_T tt_init_basic[] = {
	/* 0x00000000  0x08000000	0x04000000	cb	IRAM cached */
    {64, 0x00000000, 0x00000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x04000000	0x04000000	0x04000000	u	Unused */
    {64, 0x04000000, 0x04000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x08000000	0x08000000	0x04000000	u	IRAM uncached */
    {64, 0x08000000, 0x08000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x0C000000	0x0C000000	0x04000000	u	IROM uncached */
    {64, 0x0C000000, 0x0C000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x10000000	0x10000000	0x10000000	u	Reserved */
    {256, 0x10000000, 0x10000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x20000000	0x20000000	0x10000000	u	Registers */
    {256, 0x20000000, 0x20000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x30000000	0x230000000	0x10000000	u	Registers */
    {256, 0x30000000, 0x30000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x40000000	0x40000000	0x10000000	u	Registers */
    {256, 0x40000000, 0x40000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x50000000	0x50000000	0x30000000	u	Reserved */
    {256, 0x50000000, 0x50000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
    {256, 0x60000000, 0x60000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
    {256, 0x70000000, 0x70000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0x80000000	0x80000000	0x10000000	u	DDR#0 uncached */
    {256, 0x80000000, 0x80000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* DDR 0x90000000	0x80000000	0x10000000	cb	DDR#0 cached */
	{256, 0x90000000, 0x80000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xA0000000	0xA0000000	0x10000000	u	DDR#1 uncached */
	{256, 0xA0000000, 0xA0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xB0000000	0xA0000000	0x10000000	cb	DDR#1 cached */
	{256, 0xB0000000, 0xA0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xC0000000	0xC0000000	0x10000000	u */
    {256, 0xC0000000, 0xC0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xD0000000	0xD0000000	0x10000000	u	 */
    {256, 0xD0000000, 0xD0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE0000000	0xE0000000	0x01000000	u	ERAM#0 uncached */
    {16, 0xE0000000, 0xE0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE1000000	0xE1000000	0x01000000	u	ERAM#1 uncached */
    {16, 0xE1000000, 0xE1000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE2000000	0xE2000000	0x01000000	u	ERAM#2 uncached */
    {16, 0xE2000000, 0xE2000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE3000000	0xE3000000	0x01000000	u	ERAM#3 uncached */
    {16, 0xE3000000, 0xE3000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE4000000	0xE0000000	0x01000000	cb	ERAM#0 cached */
    {16, 0xE4000000, 0xE0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE5000000	0xE1000000	0x01000000	cb	ERAM#1 cached */
    {16, 0xE5000000, 0xE1000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE6000000	0xE2000000	0x01000000	cb	ERAM#2 cached */
    {16, 0xE6000000, 0xE2000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE7000000	0xE3000000	0x01000000	cb	ERAM#3 cached */
    {16, 0xE7000000, 0xE3000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_CACHEABLE | ARM922T_L1D_BUFFERABLE |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xE8000000	0xE8000000	0x08000000	u	 */
    {128, 0xE8000000, 0xE0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
	/* 0xF0000000	0xF0000000	0x10000000	u */
    {256, 0xF0000000, 0xF0000000, 
        (ARM922T_L1D_AP_ALL | ARM922T_L1D_DOMAIN(0) |
        ARM922T_L1D_TYPE_SECTION)},
    {0, 0, 0, 0}  // Marks end of initialization array.  Required! 
};


/* MMU base table address */
TRANSTABLE_T *mmu_base_aadr;

/* Board memory configuration */
UNS_32 board_memcfg;

/***********************************************************************
 * Private functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: ea3250_gpio_setup
 *
 * Purpose: Setup GPIO and MUX states
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void ea3250_gpio_setup(void)
{
  /* CS for SPI-NOR Flash */
  GPIO->p2_dir_set = P3_STATE_GPIO(5);
  GPIO->p3_outp_set = P3_STATE_GPIO(5) /*| P3_STATE_GPO(4) | P3_STATE_GPO(5)*/;
  

  /* Sets the following signals muxes :
     GPIO_02 / KEY_ROW6 | (ENET_MDC)      ->KEY_ROW6 | (ENET_MDC)
     GPIO_03 / KEY_ROW7 | (ENET_MDIO)     ->KEY_ROW7 | (ENET_MDIO)
     GPO_21 / U4_TX | (LCDVD[3])          ->U4_TX | (LCDVD[3])
     EMC_D_SEL                            ->(D16 ..D31 used)
     GPIO_04 / SSEL1 | (LCDVD[22])        ->SSEL1 | (LCDVD[22])
     GPIO_05 / SSEL0                      ->SSEL0
    */
  GPIO->p2_mux_clr = (P2_SDRAMD19D31_GPIO | P2_GPIO05_SSEL0);
  GPIO->p2_mux_set = (P2_GPIO03_KEYROW7 | P2_GPIO02_KEYROW6 |
                       P2_GPO21_U4TX | P2_GPIO04_SSEL1);

  /* Sets the following signal muxes:
     I2S1TX_SDA / MAT3.1                  ->I2S1TX_SDA
     I2S1TX_CLK / MAT3.0                  ->I2S1TX_CLK
     I2S1TX_WS / CAP3.0                   ->I2S1TX_WS
     SPI2_DATIO / MOSI1 | (LCDVD[20])     ->MOSI1 | (LCDVD[20])
     SPI2_DATIN / MISO1 | (LCDVD[21])     ->MISO1 | (LCDVD[21])
     SPI2_CLK / SCK1 | (LCDVD[23])        ->SCK1 | (LCDVD[23])
     SPI1_DATIO / MOSI0                   ->MOSI0
     SPI1_DATIN / MISO0                   ->MISO0
     SPI1_CLK / SCK0                      ->SCK0
     (MS_BS) | MAT2.1 / PWM3.6            ->(MS_BS)
     (MS_SCLK) | MAT2.0 / PWM3.5          ->(MS_SCLK)
     U7_TX / MAT1.1 | (LCDVD[11])         ->MAT1.1 | (LCDVD[11])
     (MS_DIO3) | MAT0.3 / PWM3.4          ->(MS_DIO3)
     (MS_DIO2) | MAT0.2 / PWM3.3          ->(MS_DIO2)
     (MS_DIO1) | MAT0.1 / PWM3.2          ->(MS_DIO1)
     (MS_DIO0) | MAT0.0 / PWM3.1          ->(MS_DIO0)
  */
  GPIO->p_mux_set = (P_SPI2DATAIO_MOSI1 |
    P_SPI2DATAIN_MISO1 | P_SPI2CLK_SCK1 |
    P_SPI1DATAIO_SSP0_MOSI | P_SPI1DATAIN_SSP0_MISO |
    P_SPI1CLK_SCK0 | P_U7TX_MAT11);
  GPIO->p_mux_clr = (P_I2STXSDA1_MAT31 | P_I2STXCLK1_MAT30 |
    P_I2STXWS1_CAP30 | P_MAT21 |
    P_MAT20 | P_MAT03 | P_MAT02 |
    P_MAT01 | P_MAT00);

  /* Sets the following signal muxes:
     GPO_02 / MAT1.0 | (LCDVD[0])         ->GPO_02
     GPO_06 / PWM4.3 | (LCDVD[18])        ->PWM4.3 | (LCDVD[18])
     GPO_08 / PWM4.2 | (LCDVD[8])         ->PWM4.2 | (LCDVD[8])
     GPO_09 / PWM4.1 | (LCDVD[9])         ->PWM4.1 | (LCDVD[9])
     GPO_10 / PWM3.6 | (LCDPWR)           ->PWM3.6 | (LCDPWR)
     GPO_12 / PWM3.5 | (LCDLE)            ->PWM3.5 | (LCDLE)
     GPO_13 / PWM3.4 | (LCDDCLK)          ->PWM3.4 | (LCDDCLK)
     GPO_15 / PWM3.3 | (LCDFP)            ->PWM3.3 | (LCDFP)
     GPO_16 / PWM3.2 | (LCDENAB/LCDM)     ->PWM3.2 | (LCDENAB/LCDM)
     GPO_18 / PWM3.1 | (LCDLP)            ->PWM3.1 | (LCDLP)
  */
  GPIO->p3_mux_set = (P3_GPO6 | P3_GPO8 |
    P3_GPO9  | P3_GPO10_MC2B | P3_GPO12_MC2A |
    P3_GPO13_MC1B | P3_GPO15_MC1A | P3_GPO16_MC0B |
    P3_GPO18_MC0A);
  GPIO->p3_mux_clr = P3_GPO2_MAT10;

  /* Sets the following signal muxes:
     P0.0 / I2S1RX_CLK                    ->I2S1RX_CLK
      P0.1 / I2S1RX_WS                     ->I2S1RX_WS
     P0.2 / I2S0RX_SDA | (LCDVD[4])       ->I2S0RX_SDA | (LCDVD[4])
     P0.3 / I2S0RX_CLK | (LCDVD[5])       ->I2S0RX_CLK | (LCDVD[5])
     P0.4 / I2S0RX_WS | (LCDVD[6])        ->I2S0RX_WS | (LCDVD[6])
     P0.5 / I2S0TX_SDA | (LCDVD[7])       ->I2S0TX_SDA | (LCDVD[7])
     P0.6 / I2S0TX_CLK | (LCDVD[12])      ->I2S0TX_CLK | (LCDVD[12])
     P0.7 / I2S0TX_WS | (LCDVD[13])       ->I2S0TX_WS | (LCDVD[13])
  */
  GPIO->p0_mux_set = P0_GPOP0_I2SRXCLK1	| P0_GPOP1_I2SRXWS1	| P0_GPOP2_I2SRXSDA0 \
                   | P0_GPOP3_I2SRXCLK0 | P0_GPOP4_I2SRXWS0 | P0_GPOP5_I2STXSDA0 \
                   | P0_GPOP6_I2STXCLK0;
  /* Default mux configuation for P1 as follows:
     All signals  -> mapped to address lines (Clear) */
  GPIO->p1_mux_clr = P1_ALL;

  /* Some GPO and GPIO states and directions needs to be setup here:
     GPO_20                      -> Output (watchdog enable) low
     GPO_19                      -> Output (NAND write protect) high
     GPO_17                      -> Output (deep sleep set) low
     GPO_11                      -> Output (deep sleep exit) low
     GPO_05                      -> Output (SDMMC power control) low
     GPO_04                      -> Output (unused) low
     GPO_02                      -> Output (audio reset) low
     GPO_01                      -> Output (LED1) low
     GPIO_1                      -> Input (MMC write protect)
     GPIO_0                      -> Input (MMC detect)
  */
  gpio_set_gpo_state(P3_STATE_GPO(19),
    (P3_STATE_GPO(20) | P3_STATE_GPO(17) | P3_STATE_GPO(11) |
    P3_STATE_GPO(5) | P3_STATE_GPO(4) | P3_STATE_GPO(2) |
    P3_STATE_GPO(1)));

  /* Set default LCD type to TFT @ 16bpp */
  clkpwr_setup_lcd(CLKPWR_LCDMUX_TFT16, 1);

  /* Select RMII ethernet interface */
  //clkpwr_select_enet_iface(0, 1);

}
int freqidx = 0;


/***********************************************************************
 *
 * Function: ea3250_clock_setup
 *
 * Purpose: Setup system clocking
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void ea3250_clock_setup(UNS_32 clkrate, UNS_32 div)
{
  CLKPWR_HCLK_PLL_SETUP_T pllcfg;

	clkpwr_set_hclk_divs(CLKPWR_HCLKDIV_DDRCLK_STOP, 1, 2);
	clkpwr_set_mode(CLKPWR_MD_DIRECTRUN);
	clkpwr_pll_dis_en(CLKPWR_HCLK_PLL, 0);
	timer_wait_ms(TIMER_CNTR0, 2);

  /* Is the PLL397 or the oscillator being used for SYSCLK? */
  if (clkpwr_get_osc() == CLKPWR_PLL397_OSC)
  {
    /* PLL397 is being used, try to switch to the main oscillator */

    /* Enable the main oscillator */
    clkpwr_mainosc_setup(0, 1);

    /* Wait 100mS to allow the oscillator to power up */
    timer_wait_ms(TIMER_CNTR0, 100);

    /* Switch over the main oscillator and disable PLL397 */
    clkpwr_sysclk_setup(CLKPWR_MAIN_OSC, 0x50);
    clkpwr_pll397_setup(0, 0, 0);
  }
  else
  {
    /* Set bad phase timing only */
    clkpwr_sysclk_setup(CLKPWR_MAIN_OSC, 0x50);
  }

  /* Setup the HCLK PLL for 208MHz operation, but if a configuration
     can't be found, stay in direct run mode */

	realrates[freqidx] = clkpwr_find_pll_cfg(MAIN_OSC_FREQ, clkrate, 5, &pllcfg);
	if (realrates[freqidx] != 0)
  //if (clkpwr_find_pll_cfg(MAIN_OSC_FREQ, 208000000, 1, &pllcfg) != 0)
  {
    /* PLL configuration is valid, so program the PLL with the
       computed configuration data */
    clkpwr_hclkpll_setup(&pllcfg);

    /* Wait for PLL to lock */
    while (clkpwr_is_pll_locked(CLKPWR_HCLK_PLL) == 0);

    /* DDR divider is 2, PERIPH_CLK divider is 16, and HCLK divider
       is 2 */                                    
    //clkpwr_set_hclk_divs(CLKPWR_HCLKDIV_DDRCLK_STOP, 16, 2);
    clkpwr_set_hclk_divs(CLKPWR_HCLKDIV_DDRCLK_NORM, div, 2);

    /* Switch to run mode - the ARM core clock is HCLK_PLL (208MHz),
       HCLK is (HCLK_PLL / 2), and PERIPH_CLK is (HCLK / 16) */
    clkpwr_force_arm_hclk_to_pclk(0);
    clkpwr_set_mode(CLKPWR_MD_RUN);    
  }
}

/***********************************************************************
 *
 * Function: ea3250_mem_setup
 *
 * Purpose: Setup memory
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void ea3250_mem_setup(void)
{
	UNS_32 ddrclk_speed;

  /* Mirror IRAM at address 0x0 */
  CLKPWR->clkpwr_bootmap = CLKPWR_BOOTMAP_SEL_BIT;

	/* Enable HCLK and SDRAM bus clocks */
	clkpwr_clk_en_dis(CLKPWR_SDRAMDDR_CLK, 1);

	/* Enable buffers in AHB ports */
	EMC->emcahn_regs [0].emcahbcontrol = EMC_AHB_PORTBUFF_EN;
	EMC->emcahn_regs [2].emcahbcontrol = EMC_AHB_PORTBUFF_EN;
	EMC->emcahn_regs [3].emcahbcontrol = EMC_AHB_PORTBUFF_EN;
	EMC->emcahn_regs [4].emcahbcontrol = EMC_AHB_PORTBUFF_EN;

	/* Enable port timeouts */
/*
	EMC->emcahn_regs [0].emcahbtimeout = EMC_AHB_SET_TIMEOUT(100);
	EMC->emcahn_regs [2].emcahbtimeout = EMC_AHB_SET_TIMEOUT(400);
	EMC->emcahn_regs [3].emcahbtimeout = EMC_AHB_SET_TIMEOUT(400);
	EMC->emcahn_regs [4].emcahbtimeout = EMC_AHB_SET_TIMEOUT(400);
*/
  EMC->emcahn_regs [0].emcahbtimeout = EMC_AHB_SET_TIMEOUT(32);  
  EMC->emcahn_regs [2].emcahbtimeout = EMC_AHB_SET_TIMEOUT(32);  
  EMC->emcahn_regs [3].emcahbtimeout = EMC_AHB_SET_TIMEOUT(32);
  EMC->emcahn_regs [4].emcahbtimeout = EMC_AHB_SET_TIMEOUT(32);

	/* Get DDR clock speed */
	ddrclk_speed = clkpwr_get_clock_rate(CLKPWR_SDRAMDDR_CLK);

	/* 1 clock between nCS and nWE */
	EMC->emcstatic_regs[0].emcstaticwaitwen =
		EMC->emcstatic_regs[1].emcstaticwaitwen = 0;
	/* 1 clock between nCS and nOE */
	EMC->emcstatic_regs[0].emcstaticwait0en =
		EMC->emcstatic_regs[1].emcstaticwait0en = 0;
	/* First read delay for non-page and page mode */
	EMC->emcstatic_regs[0].emcstaticwaitrd =
		EMC->emcstatic_regs[1].emcstaticwaitrd =
		(ddrclk_speed / EXTMEM_OE_TO_OUT);
	/* Page mode burst read timing (not used) is configured for standard
	   read timing, used in page mode for reads 2, 3, and 4 */
	EMC->emcstatic_regs[0].emcstaticpage =
		EMC->emcstatic_regs[1].emcstaticpage = 
		(ddrclk_speed / EXTMEM_OE_TO_OUT);
	/* First write delay for non-page and page mode */
	EMC->emcstatic_regs[0].emcstaticwr =
		EMC->emcstatic_regs[1].emcstaticwr = 
		(ddrclk_speed / EXTMEM_WE_HOLD_TIME);
	/* Bus turnaround delay is time from release of nOE to high-z */
	EMC->emcstatic_regs[0].emcstaticturn =
		EMC->emcstatic_regs[1].emcstaticturn = 
		(ddrclk_speed / EXTMEM_NO_OE_TO_HIZ);



	/* Read the board SDRAM/DDR configuration GPIs - these bits indicate
	   the board memory configuration */
	/* GPI05                     GPI11
	   0=DDR16, 1=SDRAM32        0=mobile, 1=standard   0=standard, 1=mobile */
	board_memcfg = gpio_get_inppin_states();
	board_memcfg = ((board_memcfg & P3_IN_STATE_GPI_05) >> 5) |
		((board_memcfg & P3_IN_STATE_GPI_28_U3RI) >> 27);

	/* Static memory chip selects 0 and 1 have the same configuration
	   for a x16 interface with fast SRAM */
	switch (board_memcfg)
	{
		case DDR_STD:
		case DDR_MOBILE:
			/* Perform DDR initialization */

			EMC->emcstatic_regs[0].emcstaticconfig =
				EMC->emcstatic_regs[1].emcstaticconfig = EXTMEM_CFG16;
			ea3250_ddr_init(ddrclk_speed, board_memcfg);

			break;

		case SDR_STD:
		case SDR_MOBILE:
		default:

			break;
	}

}

/***********************************************************************
 *
 * Function: ea3250_misc_setup
 *
 * Purpose: Miscellaneous board setup
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_misc_setup(void)
{
  /* Start Activation Polarity Register for all Sources */
  (*(volatile UNS_32 *)(0x4000402C)) = 0x40000000;

  /* Disable all start enables */
  CLKPWR->clkpwr_p01_er = 0;
  CLKPWR->clkpwr_int_er = 0;
  CLKPWR->clkpwr_pin_er = 0;

  /* Set default start polarities for wakeup signals */
  CLKPWR->clkpwr_int_ap = (CLKPWR_INTSRC_TS_P_BIT |
                           CLKPWR_INTSRC_RTC_BIT | CLKPWR_INTSRC_MSTIMER_BIT);
  CLKPWR->clkpwr_pin_ap = 0;

  /* Clear and active (latched) wakeup statuses */
  CLKPWR->clkpwr_int_rs = 0xFFFFFFFF;
  CLKPWR->clkpwr_pin_rs = 0xFFFFFFFF;
}




static void ea3250_i2c_init(void)
{
  /* enable I2C1 clock */
  CLKPWR->clkpwr_i2c_clk_ctrl |= CLKPWR_I2CCLK_I2C1CLK_EN;

  /* software reset */
  I2C1->i2c_ctrl = I2C_RESET;

  I2C1->i2c_adr = LTC3447_I2C_ADDR;

  CLKPWR->clkpwr_i2c_clk_ctrl |= CLKPWR_I2CCLK_I2C2HI_DRIVE; 


  /* running with default values for clk_lo and clk_hi */
}

static void ea3250_set_core_voltage(LTC3447_VOLTAGE_T voltage)
{
  int n = 0;

  /* invalid value */
  if (voltage < LTC3447_VOLTAGE_MIN || voltage >= LTC3447_INVALID)
    return; 

  ea3250_i2c_init();
  
  /* send address */
  I2C1->i2c_txrx = I2C_START | (LTC3447_I2C_ADDR << 1); 

	I2C1->i2c_txrx = (voltage | I2C_STOP);  

	/* wait for transmit done (TDI) */
	while (((I2C1->i2c_stat & I2C_TDI) != I2C_TDI) && n++ < 100000);

	if (n>= 100000)
	{
    n = 200000;
	}

	I2C1->i2c_stat |= I2C_TDI; 

  /* disable I2C1 clock */
  CLKPWR->clkpwr_i2c_clk_ctrl &= ~CLKPWR_I2CCLK_I2C1CLK_EN;
}

/***********************************************************************
 * Public functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: ea3250_init
 *
 * Purpose: Main startup code entry point, called from reset entry code
 *
 * Processing:
 *     Call the individual board init functions to setup the system.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_init(void)
{
  CLKPWR_CLK_T clk;

  /* Setup GPIO and MUX states */
  ea3250_gpio_setup();

  /* Shut down all IP clocks as the default state */
  for (clk = CLKPWR_FIRST_CLK; clk < CLKPWR_LAST_CLK; clk++)
  {
    clkpwr_clk_en_dis(clk, 0);
  }

  /* Setup system clocks and run mode */
  ea3250_clock_setup(266000000, 20);
                                               
  timer_wait_ms(TIMER_CNTR0, 100);

  /* set the core voltage to 1.35 V */
  ea3250_set_core_voltage(LTC3447_VOLTAGE_1_35V);

  /* Setup memory */
  ea3250_mem_setup();

  /* Setup MMU table */
  cp15_init_mmu_trans_table(mmu_base_aadr, tt_init_basic);

  /* Setup miscellaneous functions */
  ea3250_misc_setup();
}
