/*****************************************************************************
 *   wdt.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __WDT_H 
#define __WDT_H

#define WDT_DEBUG_ENABLED	1

#define WDT_KEY			0x251D8950
#define WDT_TO_VALUE	0xFFFF

#define WDEN		0x00000001
#define WDRESET		0x00000002

extern void WDTHandler(void);
extern void WDTInit( void );
extern void WDTFeed( void );

#endif /* end __WDT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
