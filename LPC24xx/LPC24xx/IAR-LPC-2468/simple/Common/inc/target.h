/*****************************************************************************
 *   target.h:  Header file for NXP LPC230x Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TARGET_H
#define __TARGET_H

#ifdef __cplusplus
   extern "C" {
#endif

/* If you want to use USB, change "define USE_USB" from 0 to 1 */

#define USE_USB         1

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if USE_USB   /* 1 is USB, 0 is non-USB related */
/* Fcck = 48Mhz, Fosc = 288Mhz, and USB 48Mhz */
/* Errata Flash.1 */
#define PLL_MValue      23      // +1
#define PLL_NValue      1       // +1
#define CCLKDivValue    5       // +1
#define USBCLKDivValue  5       // +1

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc  12000000
/* System frequence,should be less than 60MHz. */
#define Fcclk 48000000
#define Fcco  288000000
#else

/* Fcck = 48Mhz, Fosc = 288Mhz, and USB 48Mhz */
#define PLL_MValue      23
#define PLL_NValue      1
#define CCLKDivValue    5
#define USBCLKDivValue  5

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc  12000000
/* System frequence,should be less than 60MHz. */
#define Fcclk 48000000
#define Fcco  288000000

#endif

/* APB clock frequence , must be 1/2/4 multiples of ( Fcclk/4 ). */
/* If USB is enabled, the minimum APB must be greater than 16Mhz */
#if USE_USB
#define Fpclk (Fcclk / 2)
#else
#define Fpclk (Fcclk / 4)
#endif

/******************************************************************************
** Function name:   TargetInit
**
** Descriptions:    Initialize the target board; it is called in a
**        necessary place, change it as needed
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
extern void TargetInit(void);
extern void ConfigurePLL( void );
extern void TargetResetInit(void);

#ifdef __cplusplus
   }
#endif

#endif /* end __TARGET_H */
/******************************************************************************
**                            End Of File
******************************************************************************/
