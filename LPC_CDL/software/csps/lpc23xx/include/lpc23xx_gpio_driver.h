/***********************************************************************
 * $Id: lpc23xx_gpio_driver.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx GPIO and FIO driver
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
#ifndef LPC23XX_GPIO_DRIVER_H_
#define LPC23XX_GPIO_DRIVER_H_

#include "lpc23xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/* Macro used in GPIO function */
/* GPIO selection arguments */
#define GPIO_0	0x00	/* GPIO 0 port */
#define GPIO_1	0x01	/* GPIO 1 port */

/* FIO selection arguments */
#define FIO_0 	0x00	/* FIO 0 port */
#define FIO_1 	0x01	/* FIO 0 port */
#define FIO_2 	0x02	/* FIO 0 port */
#define FIO_3 	0x03	/* FIO 0 port */
#define FIO_4 	0x04	/* FIO 0 port */

/* IO port for interrupt argument */
#define	IOINT_0		0x00	/* IO0 interrupt */
#define	IOINT_2 	0x02	/* IO2 interrupt */ 

/* Set direction for GPIO port as input */
void GPIO_SetDirInput (U32 GPIOportnum, U32 portval);
/* Set direction for GPIO port as output */
void GPIO_SetDirOutput (U32 GPIOportnum, U32 portval);

/* Set state on GPIO port if direction is OUTPUT */
/* Set state for GPIO as high level */
void GPIO_Set (U32 GPIOportnum, U32 portval);
/* Set state for GPIO as low level */
void GPIO_Clr (U32 GPIOportnum, U32 portval);

/* Read current state on GPIO port */
U32 GPIO_GetState (U32 GPIOportnum);

/* Set direction for FIO port */
/* Set direction for FIO port as input */
void FIO_SetDirInput32 (U32 FIOportnum, U32 portval);
/* Set direction for FIO port as output */
void FIO_SetDirOutput32 (U32 FIOportnum, U32 portval);

/* Set state on FIO port if direction is OUTPUT */
/* Set state for FIO as high level */
void FIO_Set32 (U32 FIOportnum, U32 portval);
/* Set state for FIO as low level */
void FIO_Clr32 (U32 FIOportnum, U32 portval);

/* Read current state on FIO port */
U32 FIO_GetState32 (U32 FIOportnum);

/* Set mask for FIO port */
/* Set mask for enhanced FIO port */
void FIO_SetMask32 (U32 FIOportnum, U32 portval);
/* Clear mask for enhanced FIO port */
void FIO_ClrMask32 (U32 FIOportnum, U32 portval);

#if 0
/* Set GPIO Interrupt Enable for Rising edge */
void ioint_set_r (IOINT_PORT_T iointport, U32 pin, BOOL_32 activate);

/* Set GPIO Interrupt Enable for Falling edge */
void ioint_set_f (IOINT_PORT_T iointport, U32 pin, BOOL_32 activate);

/* Get GPIO Interrupt Status for Rising edge */
U32 ioint_get_rsta (IOINT_PORT_T iointport);

/* Get GPIO Interrupt Status for Falling edge */
U32 ioint_get_fsta (IOINT_PORT_T iointport);

/* Clear GPIO interrupt */
void ioint_clr (IOINT_PORT_T iointport, U32 pin);
#endif


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_GPIO_DRIVER_H_*/
