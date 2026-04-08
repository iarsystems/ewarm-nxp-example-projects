/****************************************************************************
 *   $Id: ssp.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx SSP example
 *
 *   Description:
 *     This file contains SSP code header definition.
 *
 ****************************************************************************
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
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.

****************************************************************************/

#ifndef __SSP_H__
#define __SSP_H__

/* There are two SSP ports on this micro. The test program
only one at a time. Set below to 0 or 1 to test corressponding
SSP port. */
#define SSP_NUM         0

/* There are there modes in SSP: loopback, master or slave. */
/* Here are the combination of all the tests.
(1) LOOPBACK test:    LOOPBACK_MODE=1, TX_RX_ONLY=0, USE_CS=1;
(2) Serial EEPROM test: LOOPBACK_MODE=0, TX_RX_ONLY=0, USE_CS=0; (default)
(3) TX(Master) Only:  LOOPBACK_MODE=0, SSP_SLAVE=0, TX_RX_ONLY=1, USE_CS=1;
(4) RX(Slave) Only:   LOOPBACK_MODE=0, SSP_SLAVE=1, TX_RX_ONLY=0, USE_CS=1 */

#define LOOPBACK_MODE   0   /* 1 is loopback, 0 is normal operation. */
#define SSP_SLAVE       0   /* 1 is SLAVE mode, 0 is master mode */
#define TX_RX_ONLY      0   /* 1 is TX or RX only depending on SSP_SLAVE
                flag, 0 is either loopback mode or communicate
                with a serial EEPROM. */

/* if USE_CS is zero, set SSEL as GPIO that you have total control of the sequence */
/* When test serial SEEPROM(LOOPBACK_MODE=0, TX_RX_ONLY=0), set USE_CS to 0. */
/* When LOOPBACK_MODE=1 or TX_RX_ONLY=1, set USE_CS to 1. */

#define USE_CS          0
#define SSP_DEBUG       1

/* SPI read and write buffer size */
#define SSP_BUFSIZE     16
#define FIFOSIZE        8

#define SFLASH_INDEX    4
#define MAX_TIMEOUT     0xFF

/* SSP Status register */
#define SSPSR_TFE       (0x1<<0)
#define SSPSR_TNF       (0x1<<1)
#define SSPSR_RNE       (0x1<<2)
#define SSPSR_RFF       (0x1<<3)
#define SSPSR_BSY       (0x1<<4)

/* SSP CR0 register */
#define SSPCR0_DSS      (0x1<<0)
#define SSPCR0_FRF      (0x1<<4)
#define SSPCR0_SPO      (0x1<<6)
#define SSPCR0_SPH      (0x1<<7)
#define SSPCR0_SCR      (0x1<<8)

/* SSP CR1 register */
#define SSPCR1_LBM      (0x1<<0)
#define SSPCR1_SSE      (0x1<<1)
#define SSPCR1_MS       (0x1<<2)
#define SSPCR1_SOD      (0x1<<3)

/* SSP Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM   (0x1<<0)
#define SSPIMSC_RTIM    (0x1<<1)
#define SSPIMSC_RXIM    (0x1<<2)
#define SSPIMSC_TXIM    (0x1<<3)

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS   (0x1<<0)
#define SSPRIS_RTRIS    (0x1<<1)
#define SSPRIS_RXRIS    (0x1<<2)
#define SSPRIS_TXRIS    (0x1<<3)

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS   (0x1<<0)
#define SSPMIS_RTMIS    (0x1<<1)
#define SSPMIS_RXMIS    (0x1<<2)
#define SSPMIS_TXMIS    (0x1<<3)

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC    (0x1<<0)
#define SSPICR_RTIC     (0x1<<1)

/* ATMEL SEEPROM command set(some shared with SST26XXX) */
#define WREN            0x06    /* MSB A8 is set to 0, simplifying test */
#define WRDI            0x04
#define RDSR            0x05
#define WRSR            0x01
#define READ            0x03
#define WRITE           0x02

/* ATMEL 25DF041 extra commands */
#define BLOCK_ERASE     0x20
#define ATM_CHIP_ERASE  0x60
#define PROTECT_SEC     0x36
#define UNPROTECT_SEC   0x39

/* SST26XXX extra commands */
#define RSTEN           0x66
#define RST             0x99
#define SST_CHIP_ERASE  0xC7
#define RBPR            0x72
#define WBPR            0x42

/* Atmel Serial EEPROM(25xxx) RDSR status bit definition */
#define RDSR_RDY        (0x01<<0)
#define RDSR_WEN        (0x01<<1)
#define RDSR_EPE        (0x01<<5)

/* SST26XXX extra RDSR status bit definition */
#define RDSR_WPLD       (0x01<<4)
#define RDSR_BUSY       (0x01<<7)

/* If RX_INTERRUPT is enabled, the SSP RX will be handled in the ISR
SSPReceive() will not be needed. */
extern void SSP0_IRQHandler (void);
extern void SSP1_IRQHandler (void);
extern void SSP_IOConfig( uint8_t portNum );
extern void SSP_Init( uint8_t portNum );
extern void SSP_Send( uint8_t portNum, uint8_t *Buf, uint32_t Length );
extern void SSP_Receive( uint8_t portNum, uint8_t *buf, uint32_t Length );

#endif  /* __SSP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

