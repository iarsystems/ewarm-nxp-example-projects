/***********************************************************************
 * $Id: lpc23xx_mci.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx MCI driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx MCI.
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
 *********************************************************************/
#ifndef LPC23XX_MCI_H_
#define LPC23XX_MCI_H_

#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * MCI Module Register Structure
 **********************************************************************/
/* MCI Module Register Structure */
typedef struct {
	__IO U32 POWER; /* MCI Power control register */
	__IO U32 CLOCK; /* MCI Clock control register */
	__IO U32 ARGUMENT; /* MCI Argument register */
	__IO U32 COMMAND; /* MCI Command register */
	__IO U32 RESPCMD; /* MCI Response command register */
	__IO U32 RESPONSE0; /* MCI Response 0 register */
	__IO U32 RESPONSE1; /* MCI Response 1 register */
	__IO U32 RESPONSE2; /* MCI Response 2 register */
	__IO U32 RESPONSE3; /* MCI Response 3 register */
	__IO U32 DATATIMER; /* MCI Data timer register */
	__IO U32 DATALENGTH; /* MCI Data control register */
	__IO U32 DATACTRL; /* MCI Data control register */
	__IO U32 DATACNT; /* MCI Data counter */
	__IO U32 STATUS; /* MCI Status register */
	__IO U32 CLEAR; /* MCI Clear register */
	__IO U32 MASK0; /* MCI Interrupt 0 mask register */
	__IO U32 MASK1; /* MCI Interrupt 1 mask register */
	__IO U32 FIFOCNT; /* MCI FIFO counter */
} MCI_Type;
#define MCI_FIFO	(*(__IO U32 *)(0xE008C080)) /* MCI Data FIFO register */

/***********************************************************************
 * MCI Power control register definitions
 **********************************************************************/
/* MCI Power off */
#define MCI_POWER_OFF 	0x00000000
/* MCI Power up */
#define MCI_POWER_UP 	0x00000002
/* MCI Power on */
#define MCI_POWER_ON	0x00000003
/* MCI Open Drain */
#define MCI_POWER_OPENDRAIN	_BIT(6)
/* MCI Rod */
#define MCI_POWER_ROD 	_BIT(7)

/***********************************************************************
 * MCI Clock control register definitions
 **********************************************************************/
/* MCI Clock divide (ClkDiv) macro
 MCLCLK frequency = MCLK/[2x(ClkDiv +1)])*/
#define MCI_CLOCK_CLKDIV(n)	_SBF(0, (n & 0xFF))
/* Enable MCI bus clock */
#define MCI_CLOCK_ENABLE	_BIT(8)
/* Disable MCI clock output when bus is idle */
#define MCI_CLOCK_PWRSAVE	_BIT(9)
/* Enable bypass of clock divide logic */
#define MCI_CLOCK_BYPASS	_BIT(10)
/* Enable wide bus mode */
#define MCI_CLOCK_WIDEBUS	_BIT(11)

/***********************************************************************
 * MCI Command register definitions
 **********************************************************************/
/* Command index macro */
#define MCI_COMMAND_CMDINDEX(n)	_SBF(0, (n & 0x3F))
/* CPSM Response status
 if set, CPSM waits for a response */
#define MCI_COMMAND_RESPONSE	_BIT(6)
/* CPSM Receive status
 if set, CPSM receives a 136 bit long response */
#define MCI_COMMAND_LONGRSP		_BIT(7)
/* CPSM Interrupt status
 if set, CPSM disables command timer and waits for interrupt request */
#define MCI_COMMAND_INTERRUPT	_BIT(8)
/* CPSM Pending status
 if set, CPSM waits for CmdPend before it starts sending a command */
#define MCI_COMMAND_PENDING		_BIT(9)
/* CPSM Enable
 if set, CPSM is enabled */
#define MCI_COMMAND_ENABLE		_BIT(10)

/***********************************************************************
 * MCI Command Response register definitions
 **********************************************************************/
/* Command Response index macro */
#define MCI_RESPCMD_CMDINDEX(n)	_SBF(0, (n & 0x3F))

/***********************************************************************
 * MCI Data control register definitions
 **********************************************************************/
/* Data transfer enable */
#define MCI_DATACTRL_ENABLE		_BIT(0)
/* Data transfer direction */
#define MCI_DATACTRL_DIRECTION	_BIT(1)
/* Data transfer mode */
#define MCI_DATACTRL_MODE		_BIT(2)
/* Enable DMA */
#define MCI_DATACTRL_DMAENABLE	_BIT(3)
/* Data block length macro */
#define MCI_DTATCTRL_BLOCKSIZE(n)	_SBF(4, (n & 0xF))

/***********************************************************************
 * MCI Status register definitions
 **********************************************************************/
/* Command response received (CRC check failed) */
#define MCI_CMDCRCFAIL		_BIT(0)
/* Data block sent/received (CRC check failed) */
#define MCI_DATACRCFAIL		_BIT(1)
/* Command response timeout */
#define MCI_CMDTIMEOUT		_BIT(2)
/* Data timeout */
#define MCI_DATATIMEOUT		_BIT(3)
/* Transmit FIFO underrun error */
#define MCI_TXUNDERRUN		_BIT(4)
/* Receive FIFO overrun error */
#define MCI_RXOVERRUN		_BIT(5)
/* Command response received (CRC check passed) */
#define MCI_CMDRESPEND		_BIT(6)
/* Command sent (no response required) */
#define MCI_CMDSENT			_BIT(7)
/* Date end (data counter is zero) */
#define MCI_DATAEND			_BIT(8)
/* Start bit not detected on all data signals in wide bus mode */
#define MCI_STARTBITERR		_BIT(9)
/* Data block sent/received (CRC check passed) */
#define MCI_DATABLOCKEND	_BIT(10)
/* Command transfer in progress */
#define MCI_CMDACTIVE		_BIT(11)
/* Data transmit in progress */
#define MCI_TXACTIVE		_BIT(12)
/* Data receive in progress */
#define MCI_RXACTIVE		_BIT(13)
/* Transmit FIFO half empty */
#define MCI_TXFIFOHALFEMPTY	_BIT(14)
/* Receive FIFO half full */
#define MCI_RXFIFOHALFFULL	_BIT(15)
/* Transmit FIFO full */
#define MCI_TXFIFOFULL		_BIT(16)
/* Receive FIFO full */
#define MCI_RXFIFOFULL		_BIT(17)
/* Transmit FIFO empty */
#define MCI_TXFIFOEMPTY		_BIT(18)
/* Receive FIFO empty */
#define MCI_RXFIFOEMPTY		_BIT(19)
/* Data available in transmit FIFO */
#define MCI_TXDATAAVLBL		_BIT(20)
/* Data available in receive FIFO */
#define MCI_RXDATAAVLBL		_BIT(21)

#define CMD_INT_MASK      (MCI_CMDCRCFAIL | MCI_CMDTIMEOUT | MCI_CMDRESPEND \
			             | MCI_CMDSENT     | MCI_CMDACTIVE)

#define DATA_ERR_INT_MASK	(MCI_DATACRCFAIL | MCI_DATATIMEOUT | MCI_TXUNDERRUN \
			               | MCI_RXOVERRUN | MCI_STARTBITERR)

#define ACTIVE_INT_MASK ( MCI_TXACTIVE | MCI_RXACTIVE)

#define FIFO_INT_MASK		(MCI_TXFIFOHALFEMPTY | MCI_RXFIFOHALFFULL \
                           | MCI_TXFIFOFULL  | MCI_RXFIFOFULL \
			               | MCI_TXFIFOEMPTY | MCI_RXFIFOEMPTY \
						   | MCI_DATABLOCKEND )

#define	FIFO_TX_INT_MASK (MCI_TXFIFOHALFEMPTY)
#define	FIFO_RX_INT_MASK (MCI_RXFIFOHALFFULL)

#define DATA_END_INT_MASK    (MCI_DATAEND | MCI_DATABLOCKEND)

#define ERR_TX_INT_MASK (MCI_DATACRCFAIL | MCI_DATATIMEOUT | MCI_TXUNDERRUN | MCI_STARTBITERR)
#define ERR_RX_INT_MASK (MCI_DATACRCFAIL | MCI_DATATIMEOUT | MCI_RXOVERRUN  | MCI_STARTBITERR)

/* Macros pointing to MCI registers */
#define MMC_SD  ((MCI_Type *)(MCI_BASE_ADDR))

#ifdef __cplusplus
}
#endif

#endif /* LPC23XX_MCI_H_ */
