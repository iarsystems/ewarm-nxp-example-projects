/***********************************************************************
 * $Id: mcb2300_init.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: MCB2300 board definitions (BSP)
 *
 * Description:
 *     This file contains the structure definitions and manifest
 *     constants for the MCB2300 component.
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
#ifndef MCB2300_INIT_H_
#define MCB2300_INIT_H_

/* Initilize system clock for LPC23xx on MCB2300 board */
/* Clock in MCB2300 definition
 * FOSC = 12000000
 * MSEL = 12
 * NSEL = 1
 * CLKSRC = 0x01 (Main OSC)
 * CCLKSEL = 6
 * USBCLKSEL = 6
 * FCCO = FOSC * MSEL * NSEL * 2 = 288000000
 * FCCLK = FCCO / (CCLKSEL + 1) = 48000000
 * FUSB = FCCO / (USBSEL + 1) = 48000000 
 * FPLCK (Clock value of each peripheral are
 * depend on private clock divider value. 
 * FPCLK = FCCLK / CLKDIV 
 * Memory acelerate module set to 3 - 
 * - MAM fetch cycles are 3 CCLKs in duration 
 * Init (refresh) Vector interrupt control 
 * PINSEL set to default (GPIO input with pulled up resistor enabled */
void mcb2300_init (void);

#endif /*MCB2300_INIT_H_*/
