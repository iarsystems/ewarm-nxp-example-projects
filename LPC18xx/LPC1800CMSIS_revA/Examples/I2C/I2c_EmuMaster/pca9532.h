/**********************************************************************
* $Id: pca9532.h 28 2014-01-27 14:45:13Z danielru $		pca9532.h	 2012-05-15
*//**
* @file		pca9532.h
* @brief	Definitions to talk to a PCA9532 port expander using GPIOs
* @version	1.0
* @date		15. May 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  
* This copyright, permission, and disclaimer notice must appear in all 
* copies of this code.
**********************************************************************/
#ifndef __PCA9532_H__
#define __PCA9532_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>


/* The INNOLUX module includes a PCA9532 I2C port expander to control the signals 
   for power, backlight and contrast.
	 Pins #13 and #14 of the display connector are supposed to be connected to I2C.
	 At the LCD connector on the Hitex board there is no hardware I2C bus connected,
	 but the two signals connected to pins #13 and #14 can be configured as GPIOs 
	 to perform an I2C emulation.
	 LCDVD9 = P4_8 = GPIO5_12 (function 4) = SDA                                 
	 LCDVD8 = P7_5 = GPIO3_13 (function 0) = SCL 
 */ 
	/*
#define SDA_GPIOPORT     (5)
#define SDA_GPIOPIN      (12)
#define SCL_GPIOPORT     (3)
#define SCL_GPIOPIN      (13)
*/
 
/** I2C address of port expander (as 7-bit address). */
#define I2C_ADDRESS_PCA9532     (0x64)
 
 
 
// Control and port registers of the PC9532
#define PCA9532_REG_INPUT0  0
#define PCA9532_REG_INPUT1  1
#define PCA9532_REG_PSC0    2
#define PCA9532_REG_PWM0    3
#define PCA9532_REG_PSC1    4
#define PCA9532_REG_PWM1    5
#define PCA9532_REG_LS0     6
#define PCA9532_REG_LS1     7
#define PCA9532_REG_LS2     8
#define PCA9532_REG_LS3     9

// Port bits in REG_LS0
#define    PCA9532_PIN0_HIGH     0
#define    PCA9532_PIN0_LOW      1
#define    PCA9532_PIN1_HIGH     0
#define    PCA9532_PIN1_LOW      4


// Port bits in REG_LS1
#define    PCA9532_PIN4_HIGH     0
#define    PCA9532_PIN4_LOW      1
#define    PCA9532_PIN6_HIGH     0
#define    PCA9532_PIN6_LOW      16


// Port bits in REG_LS2
#define    PCA9532_PIN8_HIGH     0
#define    PCA9532_PIN8_LOW      1
#define    PCA9532_PIN8_PWM0     2
#define    PCA9532_PIN8_PWM1     3


#define PCA9532_WRITE  0
#define PCA9532_READ   1


/* Macros for write/read of the I2C emulation pins                             */
/*
#define SET_SCL_HIGH     (LPC_GPIO_PORT->DIR[SCL_GPIOPORT] &= ~(1 << SCL_GPIOPIN))
#define SET_SCL_LOW      (LPC_GPIO_PORT->DIR[SCL_GPIOPORT] |=  (1 << SCL_GPIOPIN))
#define SET_SDA_HIGH     (LPC_GPIO_PORT->DIR[SDA_GPIOPORT] &= ~(1 << SDA_GPIOPIN))
#define SET_SDA_LOW      (LPC_GPIO_PORT->DIR[SDA_GPIOPORT] |=  (1 << SDA_GPIOPIN))
#define READ_SCL         (LPC_GPIO_PORT->PIN[SCL_GPIOPORT] & (1 << SCL_GPIOPIN))
#define READ_SDA         (LPC_GPIO_PORT->PIN[SDA_GPIOPORT] & (1 << SDA_GPIOPIN))
*/

/* Common address definition in pca9532.c */
//#define I2C_ADDRESS_SLAVE       I2C_ADDRESS_PCA9532 


bool pca9532_write_reg (uint8_t regnum, uint8_t length, uint8_t value[]);
bool pca9532_read_reg (uint8_t regnum, uint8_t length, uint8_t value[]);
void pca9532_init( void );
void pca9532_deinit( void );



#ifdef __cplusplus
}
#endif

#endif
