/*
 * @brief Manley 11U68 Sysinit file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

 #include "board.h"
 #include "string.h"

/* The System initialization code is called prior to the application and
   initializes the board for run-time operation. Board initialization
   includes clock setup and default pin muxing configuration. */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* IOCON pin definitions for pin muxing */
typedef struct {
	uint32_t port : 8;			/* Pin port */
	uint32_t pin : 8;			/* Pin number */
	uint32_t modefunc : 16;		/* Function and mode */
} PINMUX_GRP_T;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Pin muxing table, only items that need changing from their default pin
   state are in this table. Not every pin is mapped. */
STATIC const PINMUX_GRP_T pinmuxing[] = {
	/* Main oscillator */
	{2, 0,  (IOCON_FUNC1 | IOCON_MODE_INACT)},										/* XTALIN */
	{2, 1,  (IOCON_FUNC1 | IOCON_MODE_INACT)},										/* XTALOUT */

	/* Board LEDs */
	{2, 5,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* LED0 */
	{2, 10, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* LED1 */
	{2, 2,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* LED2 */

	/* UART0 and UART1 */
	{0, 18, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* U0_RXD */
	{0, 19, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* U0_TXD */
	{0, 13, (IOCON_FUNC4 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* RXD1 */
	{0, 14, (IOCON_FUNC4 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* TXD1 */

	/* Joystick button inputs */
	{0, 22, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* UP */
	{0, 7,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* LEFT */
	{0, 16, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* SELECT */
	{0, 23, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* DOWN */
	{1, 23, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* RIGHT */

	/* I2C and SPI */
	{0, 4,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_STDI2C_EN)},	/* SCL */
	{0, 5,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_STDI2C_EN)},	/* SDA */
	{0, 8,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* MISO */
	{0, 9,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* MOSI */
	{2, 7,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* SCLK */
	{1, 13, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* MEM_CS (SPI_CS) */

	/* Misc */
//	{?, ?,  (IOCON_FUNCX | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* Button B1?? */
	{0, 20, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* Button B2 */
	{0, 21, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)},	/* Button B3 */
	{0, 12, (IOCON_FUNC2 | IOCON_MODE_INACT )},						/* POS (ADC8 input from pot) */
};

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Setup system clocking */
STATIC void SystemSetupClocking(void)
{
	volatile int i;

	/* Powerup main oscillator */
	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_SYSOSC_PD);

	/* Wait for the OSC to stablize, no status
	   indication, dummy wait. */
	for (i = 0; i < 0x100; i++) {}

	/* Set system PLL input to main oscillator */
	Chip_Clock_SetSystemPLLSource(SYSCTL_PLLCLKSRC_MAINOSC);

	/* Power down PLL to change the PLL divider ratio */
	Chip_SYSCTL_PowerDown(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Setup PLL for main oscillator rate (FCLKIN = 12MHz) * 4 = 48MHz
	   MSEL = 3 (this is pre-decremented), PSEL = 1 (for P = 2)
	   FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 4 = 48MHz
	   FCCO = FCLKOUT * 2 * P = 48MHz * 2 * 2 = 192MHz (within FCCO range) */
	Chip_Clock_SetupSystemPLL(3, 1);

	/* Powerup system PLL */
	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Wait for PLL to lock */
	while (!Chip_Clock_IsSystemPLLLocked()) {}

	/* Set system clock divider to 1 */
	Chip_Clock_SetSysClockDiv(1);

	/* Setup FLASH access to 3 clocks */
	Chip_FMC_SetFLASHAccess(FLASHTIM_50MHZ_CPU);

	/* Set main clock source to the system PLL. This will drive 48MHz
	   for the main clock and 48MHz for the system clock */
	Chip_Clock_SetMainClockSource(SYSCTL_MAINCLKSRC_PLLOUT);

	/* Set USB PLL input to main oscillator */
	Chip_Clock_SetUSBPLLSource(SYSCTL_USBPLLCLKSRC_MAINOSC);
	/* Setup USB PLL  (FCLKIN = 12MHz) * 4 = 48MHz
	   MSEL = 3 (this is pre-decremented), PSEL = 1 (for P = 2)
	   FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 4 = 48MHz
	   FCCO = FCLKOUT * 2 * P = 48MHz * 2 * 2 = 192MHz (within FCCO range) */
	Chip_Clock_SetupUSBPLL(3, 1);

	/* Powerup USB PLL */
	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_USBPLL_PD);

	/* Wait for PLL to lock */
	while (!Chip_Clock_IsUSBPLLLocked()) {}
}

/* Sets up system pin muxing */
STATIC void SystemSetupMuxing(void)
{
	int i;

	/* Enable IOCON clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);

	for (i = 0; i < (sizeof(pinmuxing) / sizeof(PINMUX_GRP_T)); i++) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, pinmuxing[i].port, pinmuxing[i].pin,
							 pinmuxing[i].modefunc);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
	/* Setup system clocking and muxing */
	SystemSetupMuxing(); /* Muxing first as it sets up ext oscillator pins */
	SystemSetupClocking();
}
