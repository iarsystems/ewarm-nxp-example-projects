/*****************************************************************************
 *   i2s.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __I2S_H
#define __I2S_H

#define BUFSIZE     0x1000
#define RXFIFO_EMPTY  0
#define TXFIFO_FULL   8

extern __irq __nested __arm void I2SHandler( void );
extern DWORD I2SInit( void );

#endif /* end __I2S_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
