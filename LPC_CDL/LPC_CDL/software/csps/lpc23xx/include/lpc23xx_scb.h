/***********************************************************************
* $Id: lpc23xx_scb.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx System control block definition
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: System control block
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
#ifndef LPC23XX_SYSCTRLBLK_H_
#define LPC23XX_SYSCTRLBLK_H_

#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                System Control Block (SCB)  register structure               		  */
/******************************************************************************/
typedef struct
{
	__IO U32 MAMCR;				/* Memory Acceleration Module */
	__IO U32 MAMTIM;			/* Memory Accelerator Module Timing */
	__IO U32 RESERVED0[30];		/* Reserved */	
	__IO U32 PLLCON; 			/* PLL Control */
	__IO U32 PLLCFG;			/* PLL Configuration */
	__IO U32 PLLSTAT; 			/* PLL status */
	__IO U32 PLLFEED; 			/* PLL feed */
	__IO U32 RESERVED1[12]; 	/* Reserve */
	__IO U32 PCON; 				/* Power control */
	__IO U32 PCONP; 			/* Power control for peripheral */
	__IO U32 RESERVED2[15]; 	/* Reserve */
	__IO U32 CCLKCFG; 			/* CPU clock configuration register */
	__IO U32 USBCLKCFG; 		/* USB clock configuration register */
	__IO U32 CLKSRCSEL; 		/* Clock source select register */
	__IO U32 RESERVED3[12]; 	/* Reserve */
	__IO U32 EXTINT; 			/* External interrupt flag */
	__IO U32 INTWAKE; 			/* Interrupt wake up register */
	__IO U32 EXTMODE; 			/* External interrupt mode */
	__IO U32 EXTPOLAR; 			/* External interrupt polar */
	__IO U32 RESERVED4[12]; 	/* Reserve */
	__IO U32 RSID;				/* Reset Source Identification */
	__IO U32 RESERVED5[7];		/* Reserve */
	__IO U32 SCS;				/* System Control and Status */
	__IO U32 ICRTRIM; 			/* ICR trim register */
	__IO U32 PCLKSEL0; 			/* Peripheral Clock Selection register 0 */
	__IO U32 PCLKSEL1; 			/* Peripheral Clock Selection register 1 */
} SCB_Type;

/* Macro pointing SCB structure to SCB base address */
#define SCB ((SCB_Type *) SCB_BASE)

/* External interrupt definition */
/* External interrupt 0 */
#define EXTINT0		_BIT(0)
/* External interrupt 1 */
#define EXTINT1		_BIT(1)
/* External interrupt 2 */
#define EXTINT2		_BIT(2)
/* External interrupt 3 */
#define EXTINT3		_BIT(3)

/* External interrupt level mode selection argument definition */
#define	EXTINT_MODE_LEVEL 		0 		/* Level mode */
#define	EXTINT_MODE_EDGE 		1		/* Edge mode  */

/* External interrupt polar mode selection argument definition */
#define EXTINT_POLAR_LOW_FALL 	0 		/* Low or falling */
#define	EXTINT_POLAR_HIGH_RISE 	1 		/* High or rising */

/* System Controls and Status definition */
/* GPIO access mode selection definition */
#define GPIOM			_BIT(0)
/* External Memory Controller Reset Disable bit definition */
#define EMCRD			_BIT(1)
/* MCIPWR pin control bit definition */
#define MCIPWRAL		_BIT(3)
/* Main oscillator range select bit definition */
#define OSCRANGE		_BIT(4)
/* Main oscillator enable bit definition */
#define OSCEN			_BIT(5)
/* Main oscillator status bit definition */
#define OSCSTAT			_BIT(6)

/* MAM definition */
/* MAM functions disabled */
#define MAM_MODE_DIS		0x00	
/* MAM functions partially enabled */
#define MAM_MODE_PARTIAL	0x01
/* MAM functions fully enabled */
#define MAM_MODE_FULL		0x02


/* Peripheral Clock Selection bit position definition */
/* Peripheral clock divider bit position for WDT */
#define	PCLK_WDT  		0
/* Peripheral clock divider bit position for TIMER0 */
#define	PCLK_TIMER0  	2
/* Peripheral clock divider bit position for TIMER1 */
#define	PCLK_TIMER1  	4
/* Peripheral clock divider bit position for UART0 */
#define	PCLK_UART0  	6
/* Peripheral clock divider bit position for UART1 */
#define	PCLK_UART1  	8
/* Peripheral clock divider bit position for PWM1 */
#define	PCLK_PWM1  		12
/* Peripheral clock divider bit position for I2C0 */
#define	PCLK_I2C0  		14
/* Peripheral clock divider bit position for SPI */
#define	PCLK_SPI  		16
/* Peripheral clock divider bit position for RTC */
#define	PCLK_RTC  		18
/* Peripheral clock divider bit position for SSP1 */
#define	PCLK_SSP1  		20
/* Peripheral clock divider bit position for DAC */
#define	PCLK_DAC  		22
/* Peripheral clock divider bit position for ADC */
#define	PCLK_ADC  		24
/* Peripheral clock divider bit position for CAN1 */
#define	PCLK_CAN1 		26
/* Peripheral clock divider bit position for CAN2 */
#define	PCLK_CAN2 		28
/* Peripheral clock divider bit position for ACF */
#define	PCLK_ACF  		30
/* Peripheral clock divider bit position for BAT_RAM */
#define	PCLK_BAT_RAM  	32
/* Peripheral clock divider bit position for GPIO */
#define	PCLK_GPIO  		34
/* Peripheral clock divider bit position for PCB */
#define	PCLK_PCB  		36
/* Peripheral clock divider bit position for  I2C1 */
#define	PCLK_I2C1  		38 
/* Peripheral clock divider bit position for SSP0 */ 
#define	PCLK_SSP0  		42
/* Peripheral clock divider bit position for TIMER2 */
#define	PCLK_TIMER2  	44
/* Peripheral clock divider bit position for  TIMER3 */
#define	PCLK_TIMER3  	46
/* Peripheral clock divider bit position for UART2 */
#define	PCLK_UART2  	48
/* Peripheral clock divider bit position for UART3 */
#define	PCLK_UART3  	50
/* Peripheral clock divider bit position for I2C2 */
#define	PCLK_I2C2  		52
/* Peripheral clock divider bit position for I2S */
#define	PCLK_I2S  		54
/* Peripheral clock divider bit position for MCI  */
#define	PCLK_MCI  		56
/* Peripheral clock divider bit position for SYSCON */
#define	PCLK_SYSCON  	60


/* Macro for Peripheral Clock Selection register bit values
 * Note: When CCLK_DIV_8, Peripheral’s clock is selected to 
 * PCLK_xyz = CCLK/8 except for CAN1, CAN2, and CAN filtering 
 * when ’11’selects PCLK_xyz = CCLK/6 */
/* Peripheral clock divider is set to 4 from CCLK */
#define	CCLK_DIV_4  0
/* Peripheral clock divider is the same with CCLK */
#define	CCLK_DIV_1  1
/* Peripheral clock divider is set to 2 from CCLK */
#define	CCLK_DIV_2  2


/* Clock source selection multiplexer definition */
/* Internal RC oscillator */
#define CLKSRCSEL_IRC		0x00
/* Main oscillator */
#define CLKSRCSEL_MAINOSC	0x01
/* RTC oscillator */
#define CLKSRCSEL_RTC		0x02

/* PLL control definition */
/* PLL control enable */
#define PLL_CON_ENABLE		0x01
/* PLL control connect */
#define PLL_CON_CONNECT		0x02


/* PLL status definition */
/* PLL MSEL get value  */
#define PLL_MSEL_GET(n)		((U32)(n & 0x3FFF))
/* PLL NSEL get value  */
#define PLL_NSEL_GET(n)		((U32)((n >> 16) & 0xFF))
/* PLL status enable bit */
#define PLL_STAT_PLLE		_BIT(24)
/* PLL status Connect bit */
#define PLL_STAT_PLLC		_BIT(25)
/* PLL status lock */
#define PLL_STAT_PLOCK		_BIT(26)


/* Macro to set peripheral clock of each type 
 * p: position of two bits that hold divider of peripheral clock 
 * n: value of divider of peripheral clock  to be set */
#define PCLK_PCLKSEL_SET(p,n)	_SBF(p,n) 
/* Macro to mask peripheral clock of each type */
#define PCLK_PCLKSEL_MASK(p)	_SBF(p, 0x03)
/* Macro to get peripheral clock of each type */
#define PCLK_PCLKSEL_GET(p, n)	((U32)((n >> p) & 0x03))

/* Power Mode Control definition */
/* Idle mode control bit */
#define PCON_PM0_IDL	0x01
/* Power-down mode control bit */
#define PCON_PM1_PD		0x02
/* Brown Out Power-down Mode */
#define PCON_BODPDM		0x04
/* Brown Out Global Disable */
#define PCON_BOGD		0x08
/* Brown Out Reset Disable */
#define PCON_BORD		0x10
/* Power mode control bit 2 */
#define PCON_PM2		0x80

/* Macro for Interrupt Wakeup source */
/* External interrupt 0 of wake up source */
#define	EXTWAKE0  	_BIT(0)
/* External interrupt 1 of wake up source */
#define	EXTWAKE1  	_BIT(1)
/* External interrupt 2 of wake up source */
#define	EXTWAKE2  	_BIT(2)
/* External interrupt 3 of wake up source */
#define	EXTWAKE3  	_BIT(3)
/* Wake-up on LAN interrupt */
#define	ETHWAKE	 	_BIT(4)
/* USB interrupt of wake up source */
#define	USBWAKE	 	_BIT(5)
/* Wake-up on CAN activity */
#define	CANWAKE	 	_BIT(6)
/* Wake-up on GPIO0 activity */
#define	GPIO0WAKE  	_BIT(7)
/* Wake-up on GPIO2 activity */
#define	GPIO2WAKE  	_BIT(8)
/* BOD interrupt of wake up source */
#define	BODWAKE	 	_BIT(14)
/* RTC interrupt of wake up source */
#define	RTCWAKE	 	_BIT(15)

/* Macro for Power Control for Peripherals source */
/* Timer/Counter 0 power/clock control bit */
#define	 PCTIM0	 	_BIT(1)
/* Timer/Counter 1 power/clock control bit */
#define	 PCTIM1	 	_BIT(2)
/* UART0 power/clock control bit */
#define	 PCUART0  	_BIT(3)
/* UART1 power/clock control bit */ 
#define	 PCUART1  	_BIT(4)
/* PWM1 power/clock control bit */
#define	 PCPWM1	 	_BIT(6)
/* The I2C0 interface power/clock control bit */
#define	 PCI2C0	 	_BIT(7)
/* The SPI interface power/clock control bit */
#define	 PCSPI  	_BIT(8)
/* The RTC power/clock control bit */
#define	 PCRTC  	_BIT(9)
/* The SSP1 interface power/clock control bit */
#define	 PCSSP1	 	_BIT(10)
/* External Memory Controller */
#define	 PCEMC  	_BIT(11)
/* A/D converter 0 (ADC0) power/clock control bit */
#define	 PCAD  		_BIT(12)
/* CAN Controller 1 power/clock control bit */
#define	 PCAN1  	_BIT(13)
/* CAN Controller 2 power/clock control bit */
#define	 PCAN2  	_BIT(14)
/* The I2C1 interface power/clock control bit */
#define	 PCI2C1  	_BIT(19)
/* The SSP0 interface power/clock control bit */
#define	 PCSSP0		_BIT(21)
/* Timer 2 power/clock control bit */
#define	 PCTIM2	 	_BIT(22)
/* Timer 3 power/clock control bit */
#define	 PCTIM3	 	_BIT(23)
/* UART 2 power/clock control bit */
#define	 PCUART2  	_BIT(24)
/* UART 3 power/clock control bit */
#define	 PCUART3  	_BIT(25)
/* I2C interface 2 power/clock control bit */
#define	 PCI2C2	 	_BIT(26)
/*I2S interface power/clock control bit*/
#define	 PCI2S  	_BIT(27)
/*SD card interface power/clock control bit*/
#define	 PCSDC  	_BIT(28)
/*GP DMA function power/clock control bit*/
#define	 PCGPDMA  	_BIT(29)
/*Ethernet block power/clock control bit*/
#define	 PCENET	 	_BIT(30)
/*USB interface power/clock control bit*/
#define	 PCUSB  	_BIT(31)

/* Macro for system clock type argument */
/* Freq of OSC */
#define	SYSCLK_FOSC 	0
/* Freq of CCO */
#define	SYSCLK_FCCO 	1
/* Freq of CCLK */
#define	SYSCLK_FCCLK 	2


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_SYSCTRLBLK_H_*/
