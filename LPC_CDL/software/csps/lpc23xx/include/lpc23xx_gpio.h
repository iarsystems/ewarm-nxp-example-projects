/***********************************************************************
 * $Id: lpc23xx_gpio.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx GPIO and FIO definitions
 *
 * Description:
 *     This file contains the structure definitions and manifest
 *     constants for the LPC23xx component: GPIO and FIO (Fast)
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
#ifndef LPC23XX_GPIO_H_
#define LPC23XX_GPIO_H_

#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 * GPIO Module Register Structure
 ********************************************************************/
typedef struct {
	__IO U32 IOPIN; 	/* GPIO port pin regs */
	__IO U32 IOSET; 	/* GPIO port output set regs */
	__IO U32 IODIR;	 	/* GPIO direction regs */
	__IO U32 IOCLR; 	/* GPIO port output clear regs */
} GPIO_Type;

/********************************************************************
 * GPIO Interrupt Module Register Structure
 ********************************************************************/
typedef struct {
	__IO U32 INTSTATR; 	/* GPIO Interrupt Status for Rising edge */
	__IO U32 INTSTATF; 	/* GPIO Interrupt Status for Falling edge */
	__IO U32 INTCLR; 	/* GPIO Interrupt Clear */	
	__IO U32 INTENR; 	/* GPIO Interrupt Enable for Rising edge */	
	__IO U32 INTENF; 	/* GPIO Interrupt Enable for Falling edge */	
} IOINT_Type;

/********************************************************************
 * GPIO Overall Interrupt Status Module Register Structure
 ********************************************************************/
typedef struct {
	__IO U32 INTSTAT; /* GPIO overall Interrupt Status */
} IOINTSTAT_Type;

/********************************************************************
 * FIO Module Register Structure
 ********************************************************************/
typedef struct {
	__IO U32 FIODIR; 		/* FIO port direction */
	__IO U32 RESERVED[3]; 	/* reserved */
	__IO U32 FIOMASK; 		/* FIO mask regs */
	__IO U32 FIOPIN; 		/* FIO port pin regs */
	__IO U32 FIOSET; 		/* FIO port ouput set regs */
	__IO U32 FIOCLR; 		/* FIO port ouput clear regs */
}FIO32_Type;

/* Macro pointing to GPIO0 registers address */
#define GPIO0	((GPIO_Type *)(GPIO0_BASE))
/* Macro pointing to GPIO1 registers address */
#define GPIO1	((GPIO_Type *)(GPIO1_BASE))

/* Macro pointing to FIO0 registers address */
#define FIO0	((FIO32_Type *)(FIO0_BASE))
/* Macro pointing to FIO1 registers address */
#define FIO1	((FIO32_Type *)(FIO1_BASE))
/* Macro pointing to FIO2 registers address */
#define FIO2 	((FIO32_Type *)(FIO2_BASE))
/* Macro pointing to FIO3 registers address */
#define FIO3	((FIO32_Type *)(FIO3_BASE))
/* Macro pointing to FIO4 registers address */
#define FIO4	((FIO32_Type *)(FIO4_BASE))

/* Macro pointing to IO0INT registers address */
#define IO0INT	((IOINT_Type *)(IO0INT_BASE))
/* Macro pointing to IO2INT registers address */
#define IO2INT	((IOINT_Type *)(IO2INT_BASE))
/* Macro pointing to IOINT status registers address */
#define IOINTSTAT	((IOINTSTAT_REGS_T *)(IOINTSTA_BASE))

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_GPIO_H_*/
