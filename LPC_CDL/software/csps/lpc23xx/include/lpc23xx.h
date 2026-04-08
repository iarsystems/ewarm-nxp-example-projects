/***********************************************************************
 * $Id: lpc23xx.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx family chip definitions
 *
 * Description:
 *     This file contains chip specific information such as the
 *     physical addresses defines for the LPC23xx registers, clock
 *     frequencies, and other chip information.
 *
 * Notes:
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
#ifndef LPC23xx_CHIP_H
#define LPC23xx_CHIP_H

#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

/* Digital to analog converter controller base address */
#define DAC_BASE			0xE006C000
	
/* Analog to Digital converter controller base address */
#define ADC0_BASE			0xE0034000
	
/* Watch dog timer controller base address */
#define WDT_BASE			0xE0000000
	
/* Reatime clock controller base address */
#define RTC_BASE			0xE0024000
	
/* Timer/ Counter 0 controller base address */
#define TC0_BASE			0xE0004000
/* Timer/ Counter 1 controller base address */
#define TC1_BASE			0xE0008000
/* Timer/ Counter 2 controller base address */
#define TC2_BASE			0xE0070000
/* Timer/ Counter 3 controller base address */
#define TC3_BASE			0xE0074000

/* Pulse Width Modulator Controller base address */
#define PWM_BASE					0xE0018000
	
/* I2C 0 Controller base address */
#define I2C0_BASE					0xE001C000
/* I2C 1 Controller base address */
#define I2C1_BASE					0xE005C000
/* I2C 2 Controller base address */
#define I2C2_BASE					0xE0080000

/* General-purpose DMA Controller */
#define GPDMA_BASE_ADDR				0xFFE04000
/* DMA Channel 0 base address */
#define GPDMA_CH0_BASE_ADDR			(GPDMA_BASE_ADDR + 0x100)
/* DMA Channel 1 base address */
#define GPDMA_CH1_BASE_ADDR			(GPDMA_BASE_ADDR + 0x120)

/* CAN Acceptance Filter RAM Base Address */
#define CAN_AFRAM_BASE_ADDR   		0xE0038000
/* CAN Acceptant Filter base address */
#define CAN_AF_BASE_ADDR			0xE003C000
/* CAN Central base address */
#define CAN_CENTRAL_BASE_ADDR		0xE0040000
/* CAN 1 Controller */
#define CAN1_BASE_ADDR				0xE0044000
/* CAN 2 Controller */
#define CAN2_BASE_ADDR				0xE0048000

/* Watchdog timer base address */
#define WDT_BASE					0xE0000000

/* Universal Asynchronous Receiver Transmitter 0 base address */
#define UART0_BASE					0xE000C000
/* Universal Asynchronous Receiver Transmitter 1 base address */
#define UART1_BASE					0xE0010000

/* General purpose IO base address */
#define GPIO_BASE					0xE0028000
/* Fast IO base address */
#define FIO_BASE					0x3FFFC000
/* General purpose IO 0 base address */
#define GPIO0_BASE					(GPIO_BASE + 0x00)
/* General purpose IO 1 base address */
#define GPIO1_BASE					(GPIO_BASE + 0x10)
/* Fast IO 0 base address */
#define FIO0_BASE					(FIO_BASE + 0x00)
/* Fast IO 1 base address */
#define FIO1_BASE					(FIO_BASE + 0x20)
/* Fast IO 2 base address */
#define FIO2_BASE					(FIO_BASE + 0x40)
/* Fast IO 3 base address */
#define FIO3_BASE					(FIO_BASE + 0x60)
/* Fast IO 4 base address */
#define FIO4_BASE					(FIO_BASE + 0x80)
/* GPIO overall Interrupt Status base address */
#define IOINTSTA_BASE				(GPIO_BASE + 0x80)
/* GPIO 0 interrupt base address */
#define IO0INT_BASE					(GPIO_BASE + 0x84)
/* GPIO 2 interrupt base address */
#define IO2INT_BASE					(GPIO_BASE + 0xA4)

/* Pin connect block base address */
#define PINSEL_BASE					0xE002C000

/* Vector interrupt control base address */
#define VIC_BASE					0xFFFFF000

/* System control block base address */
#define SCB_BASE					0xE01FC000

/* SSP 0 base address */
#define SSP0_BASE			0xE0068000
/* SSP 1 base address */
#define SSP1_BASE			0xE0030000

/* SPI 0 base address */
#define SPI0_BASE			0xE0020000

/* MCI base address */
#define MCI_BASE_ADDR		0xE008C000

#ifdef __cplusplus
}
#endif

#endif /* LPC23xx_CHIP_H */
