/***********************************************************************
 * $Id: lpc23xx_mci_driver.c 28 2014-01-27 14:45:13Z danielru $
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
#include "lpc23xx_mci_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_dma_driver.h"
#include <string.h>

/* MCI device configuration structure type */
typedef struct {
	BOOL32 init;
	MCI_Type *regptr;
	__IO U8* WriteDataBuffer;  /* Used in Interrupt Mode */
	__IO U8* ReadDataBuffer;   /* Used in Interrupt Mode */
} MCI_DRVDAT_T;

__IO U32 MCI_DataErrorProcess_count; /* Data error process count */
__IO U32 MCI_DATA_END_InterruptService_count; /* Data end interrupt service count */
__IO U32 MCI_FIFOInterruptService_count; /* FIFO interrupt service count */
__IO U32 MCI_CmdProcess_count; /* Command process count */

__IO U32 CmdCRCErrCount; /* Command CRC error count */
__IO U32 CmdTimeoutErrCount; /* Command Timeout error count */
__IO U32 CmdRespEndCount; /* Command response end count */
__IO U32 CmdSentCount; /* Command send count */
__IO U32 CmdActiveCount; /* Command active count */

__IO U32 DataCRCErrCount; /* Data CRC error count */
__IO U32 DataTimeoutErrCount; /* Data Timeout error count */
__IO U32 DataTxUnderrunErrCount; /* Data transmit underrun error count */
__IO U32 DataRxOverrunErrCount; /* Data receive overrun error count */
__IO U32 DataStartbitErrCount; /* Data start bit error count */

__IO U32 DataEndCount; /* Data end count */
__IO U32 DataBlockEndCount; /* Data block end count */
__IO U32 MCI_Block_End_Flag; /* Block End Flag */

__IO U32 DataTxActiveCount; /* Data transmit active count */
__IO U32 DataRxActiveCount; /* Data receive active count */

__IO U32 DataFIFOCount; /* Data FIFO count */
__IO U32 DataRxFIFOCount; /* Data Receive FIFO count */
__IO U32 DataTxFIFOCount; /* Data Transmit FIFO count */

__IO U32 MCI_CardType; /* Card type */
__IO U32 CardRCA; /* Card RCA (Relative Card Address)*/

__IO U32 SDStatRead = 0; /* SD read status */

__IO S8 *WriteBlock = (S8 *) (MCI_DMA_SRC); /* treat WriteBlock as a constant address */
__IO S8 *ReadBlock = (S8 *) (MCI_DMA_DST); /* treat ReadBlock as a constant address */

__IO U32 TXBlockCounter = 0, RXBlockCounter = 0; /* TX/RX block count*/

U32 MCI_SendStatus(void);
U32 MCI_SendACMDBusWidth(U32 busWidth);

/* MCI device configuration structure data */
STATIC MCI_DRVDAT_T mcidrvdat;


/**********************************************************************
 * Function: 	MCI_ReadFIFO
 *
 * Purpose: 	Read MCI_FIFO to data buffer.
 * 				This function is used in Interrupt Mode
 *
 * Parameter:
 * 			dst: data buffer
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_ReadFIFO(U32 *dst) {
	__IO U32 i;

	for (i = 0; i < 8; i++) {
		*dst = MCI_FIFO;
		dst++;
	}
	return;
}

/**********************************************************************
 * Function: 	MCI_WriteFIFO
 *
 * Purpose: 	Write data from data buffer to MCI FIFO.
 * 				This function is used in Interrupt Mode
 *
 * Parameter:
 * 			src: data buffer
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_WriteFIFO(U32 *src) {
	__IO U32 i;

	for (i = 0; i < 8; i++) {
		MCI_FIFO = *src;
		src++;
	}
	return;
}

/**********************************************************************
 * Function: MCI_BusMode
 *
 * Purpose: Set MCI bus mode (Open drain or not)
 *
 * Parameter:
 * 		bus_mode: bus mode
 *
 * Return: None
 *
 *********************************************************************/
void MCI_BusMode(U32 bus_mode) {
	__IO U32 i;

	if (bus_mode == OPEN_DRAIN) {
		mcidrvdat.regptr->POWER |= MCI_POWER_OPENDRAIN;
	} else {
		mcidrvdat.regptr->POWER &= ~MCI_POWER_OPENDRAIN;
	}

	/* Delay after switching mode */
	for (i = 0; i < 1000; i++)
		;
	return;
}

/**********************************************************************
 * Function: 	MCI_TXEnable
 *
 * Purpose: 	Enable MCI data transmit
 *
 * Parameter: 	None
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_TXEnable(void) {

#if MCI_DMA_ENABLED
	mcidrvdat.regptr->MASK0 |= ((DATA_END_INT_MASK) | (ERR_TX_INT_MASK)); /* Enable TX interrupts only */
	mcidrvdat.regptr->MASK1 |= ((DATA_END_INT_MASK) | (ERR_TX_INT_MASK)); /* Enable TX interrupts only */
#else
	mcidrvdat.regptr->MASK0 |= ((FIFO_TX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_TX_INT_MASK)); /* FIFO TX interrupts only */
	mcidrvdat.regptr->MASK1 |= ((FIFO_TX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_TX_INT_MASK)); /* FIFO TX interrupts only */
#endif
	return;
}

/**********************************************************************
 * Function: 	MCI_TXDisable
 *
 * Purpose: 	Disable MCI data transmit
 *
 * Parameter: 	None
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_TXDisable(void) {

#if MCI_DMA_ENABLED
	mcidrvdat.regptr->MASK0 &= ~((DATA_END_INT_MASK) | (ERR_TX_INT_MASK)); /* Enable TX interrupts only */
	mcidrvdat.regptr->MASK1 &= ~((DATA_END_INT_MASK) | (ERR_TX_INT_MASK)); /* Enable TX interrupts only */
#else
	mcidrvdat.regptr->MASK0 &= ~((FIFO_TX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_TX_INT_MASK)); /* FIFO TX interrupts only */
	mcidrvdat.regptr->MASK1 &= ~((FIFO_TX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_TX_INT_MASK)); /* FIFO TX interrupts only */
#endif
	return;
}

/**********************************************************************
 * Function: 	MCI_RXEnable
 *
 * Purpose: 	Enable MCI data receive
 *
 * Parameter: 	None
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_RXEnable(void) {

#if MCI_DMA_ENABLED
	mcidrvdat.regptr->MASK0 |= ((DATA_END_INT_MASK) | (ERR_RX_INT_MASK)); /* Enable RX interrupts only */
	mcidrvdat.regptr->MASK1 |= ((DATA_END_INT_MASK) | (ERR_RX_INT_MASK)); /* Enable RX interrupts only */
#else
	mcidrvdat.regptr->MASK0 |= ((FIFO_RX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_RX_INT_MASK)); /* FIFO RX interrupts only */
	mcidrvdat.regptr->MASK1 |= ((FIFO_RX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_RX_INT_MASK)); /* FIFO RX interrupts only */
#endif
	return;
}

/**********************************************************************
 * Function: 	MCI_RXDisable
 *
 * Purpose: 	Disable MCI data receive
 *
 * Parameter: 	None
 *
 * Return: 		None
 *
 *********************************************************************/
void MCI_RXDisable(void) {

#if MCI_DMA_ENABLED
	mcidrvdat.regptr->MASK0 &= ~((DATA_END_INT_MASK) | (ERR_RX_INT_MASK)); /* Enable TX interrupts only */
	mcidrvdat.regptr->MASK1 &= ~((DATA_END_INT_MASK) | (ERR_RX_INT_MASK)); /* Enable TX interrupts only */
#else
	mcidrvdat.regptr->MASK0 &= ~((FIFO_RX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_RX_INT_MASK)); /* FIFO TX interrupts only */
	mcidrvdat.regptr->MASK1 &= ~((FIFO_RX_INT_MASK) | (DATA_END_INT_MASK)
			| (ERR_RX_INT_MASK)); /* FIFO TX interrupts only */
#endif
	return;
}

/******************************************************************************
* Function:		MCI_CheckStatus
*
* Purpose:		MCI Check status before and after the block read and
*						write. Right after the block read and write, this routine
*						is important that, even the FIFO is empty, complete
*						block has been sent, but, data is still being written
*						to the card, this routine is to ensure that the data
*						has been written based on the state of the card, not
*						by the length being set.
*
* Parameter:	None
*
* Return:		TRUE or FALSE
*
*****************************************************************************/
U32 MCI_CheckStatus(void) {
	U32 respValue;

	while (1) {
		if ((respValue = MCI_SendStatus()) == INVALID_RESPONSE) {
			break;
		} else {
			/* The only valid state is TRANS per MMC and SD state diagram.
			 RCV state may be seen, but, I have found that it happens
			 only when TX_ACTIVE or RX_ACTIVE occurs before the WRITE_BLOCK and
			 READ_BLOCK cmds are being sent, which is not a valid sequence. */
			if ((respValue & (0x0F << 8)) == 0x0900) {
				return (TRUE);
			}
		}
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_CmdHandler
*
* Purpose:		Called by MCI_ISR
*				To simplify the process, for card initialization, the
*					CMD interrupts are disabled.
*
* Parameter:	None
*
* Return:		None
*
******************************************************************************/
void MCI_CmdHandler(void) {
	U32 mci_status;

	mci_status = mcidrvdat.regptr->STATUS;
	if (mci_status & MCI_CMDCRCFAIL) {
		CmdCRCErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_CMDCRCFAIL;
	}
	if (mci_status & MCI_CMDTIMEOUT) {
		CmdTimeoutErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_CMDTIMEOUT;
	}
	/* Cmd Resp End or Cmd Sent */
	if (mci_status & MCI_CMDRESPEND) {
		CmdRespEndCount++;
		mcidrvdat.regptr->CLEAR = MCI_CMDRESPEND;
	}
	if (mci_status & MCI_CMDSENT) {
		CmdSentCount++;
		mcidrvdat.regptr->CLEAR = MCI_CMDSENT;
	}
	if (mci_status & MCI_CMDACTIVE) {
		CmdActiveCount++;
		mcidrvdat.regptr->CLEAR = MCI_CMDACTIVE;
	}
	return;
}

/******************************************************************************
* Function:		MCI_DataErrorHandler
*
* Purpose:		Called by MCI_ISR
*					Process data error.
*
* Parameter:	None
*
* Return:		None
*
******************************************************************************/
void MCI_DataErrorHandler(void) {
	U32 MCIStatus;

	MCIStatus = mcidrvdat.regptr->STATUS;
	if (MCIStatus & MCI_DATACRCFAIL) {
		DataCRCErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_DATACRCFAIL;
	}
	if (MCIStatus & MCI_DATATIMEOUT) {
		DataTimeoutErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_DATATIMEOUT;
	}
	/* Underrun or overrun */
	if (MCIStatus & MCI_TXUNDERRUN) {
		DataTxUnderrunErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_TXUNDERRUN;
	}
	if (MCIStatus & MCI_RXOVERRUN) {
		DataRxOverrunErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_RXOVERRUN;
	}
	/* Start bit error on data signal */
	if (MCIStatus & MCI_STARTBITERR) {
		DataStartbitErrCount++;
		mcidrvdat.regptr->CLEAR = MCI_STARTBITERR;
	}
	return;
}

/******************************************************************************
* Function:		MCI_DataEndHandler
*
* Purpose:		Called by MCI_ISR
*				This is the last interrupt module processing
*                   the block write and read to and from the MMC/SD card.
*
*               FIFO interrupts are also used when DMA is disabled
*				This routine simply clears the
*                   MCI_Block_End_Flag, and increments counters for debug
*
* Parameter:	None
*
* Returne:		None
*
******************************************************************************/
void MCI_DataEndHandler(void) {
	U32 mci_status;

	mci_status = mcidrvdat.regptr->STATUS;
	if (mci_status & MCI_DATAEND) /* Data end, and Data block end  */
	{
		DataEndCount++;
		mcidrvdat.regptr->CLEAR = MCI_DATAEND;
		return;
	}
	if (mci_status & MCI_DATABLOCKEND) {
		DataBlockEndCount++;
#if !MCI_DMA_ENABLED
		MCI_SendStop();
#endif
		mcidrvdat.regptr->CLEAR = MCI_DATABLOCKEND;
		MCI_RXDisable();
		MCI_Block_End_Flag = 0;
		return;
	}

	/* Tx active  */
	if (mci_status & MCI_TXACTIVE) {
		DataTxActiveCount++;
	}
	/* Rx active  */
	if (mci_status & MCI_RXACTIVE) {
		DataRxActiveCount++;
	}
	return;
}

/******************************************************************************
* Function:		MCI_FIFOHandler
*
* Purpose:		Called by MCI_ISR when using FIFO
*					interrupts and DMA is disabled
*
* Parameter:	None
*
* Return:		None
*
******************************************************************************/
void MCI_FIFOHandler(void) {
#if !MCI_DMA_ENABLED
	U32 MCIStatus;

	MCIStatus = mcidrvdat.regptr->STATUS;
	if (MCIStatus & (FIFO_TX_INT_MASK)) {
		DataTxFIFOCount++; /* if using TX_HALF_EMPTY remove one WriteFifo below */
		if (MCIStatus & MCI_TXFIFOHALFEMPTY) /* empty is multiple of 512 block size */
		{
			MCI_WriteFIFO((U32 *) &mcidrvdat.WriteDataBuffer[TXBlockCounter]); /* write 8 words to fifo */
			TXBlockCounter += 32;
		}
		if (TXBlockCounter == BLOCK_LENGTH) /* block complete */
		{
			TXBlockCounter = 0;
			mcidrvdat.regptr->MASK0 &= ~(FIFO_TX_INT_MASK); /* disable FIFO int until next block write */
			mcidrvdat.regptr->MASK1 &= ~(FIFO_TX_INT_MASK);
			/* wait for SD card to complete sending data i.e MCI_DATA_BLK_END interrupt */
		}
	} else if (MCIStatus & (FIFO_RX_INT_MASK)) {
		DataRxFIFOCount++;
		if (MCIStatus & MCI_RXFIFOHALFFULL) /* if using RX_HALF_FULL remove one ReadFIFO below */
		{
			MCI_ReadFIFO((U32 *) &mcidrvdat.ReadDataBuffer[RXBlockCounter]); /* read 8 words from fifo */
			RXBlockCounter += 32;
		}
		if (RXBlockCounter == BLOCK_LENGTH) /* block complete */
		{
			RXBlockCounter = 0;
		}
	}
#endif
	DataFIFOCount++;
	return;
}

/******************************************************************************
* Function:		MCI_ISR
*
* Purpose:		MCI interrupt handler
*				The handler to handle the block data write and read
*					not for the commands.
*
* Parameter:	None
*
* Returne:		None
*
******************************************************************************/
void MCI_ISR(void) {
	U32 MCI_Status;

	MCI_Status = mcidrvdat.regptr->STATUS;
	/* handle MCI_STATUS interrupt */
	if (MCI_Status & DATA_ERR_INT_MASK) {
		MCI_DataErrorHandler();
		MCI_DataErrorProcess_count++;
		VIC_Ack(); /* Acknowledge Interrupt */
		return;
	}
	if (MCI_Status & DATA_END_INT_MASK) {
		MCI_DataEndHandler();
		MCI_DATA_END_InterruptService_count++;
		VIC_Ack(); /* Acknowledge Interrupt */
		return;
	} else if (MCI_Status & FIFO_INT_MASK) {
		MCI_FIFOHandler();
		MCI_FIFOInterruptService_count++;
		VIC_Ack(); /* Acknowledge Interrupt */
		return;
	} else if (MCI_Status & CMD_INT_MASK) {
		MCI_CmdHandler();
		MCI_CmdProcess_count++;
		VIC_Ack(); /* Acknowledge Interrupt */
		return;
	}
	VIC_Ack(); /* Acknowledge Interrupt */
}

/******************************************************************************
* Function:		MCI_SetClockRate
*
* Purpose:		Set MCI clock rate, during initialization phase < 400K
*						during data phase < 20Mhz.
*
* Parameter:
* 			clockrate: Clock rate value to be set
*
* Return:		None
*
*****************************************************************************/
void MCI_SetClockRate(U32 clockrate) {

	U32 i, ClkValue = 0;

	if (clockrate == MCI_SLOW_RATE)
		ClkValue |= MCI_CLKDIV_SLOW;
	else if (clockrate == MCI_NORMAL_RATE)
		ClkValue |= MCI_CLKDIV_NORMAL;

	mcidrvdat.regptr->CLOCK &= 0; // Clear Clock Register
	mcidrvdat.regptr->CLOCK |= MCI_CLOCK_CLKDIV(ClkValue); // Set Clock rate
	mcidrvdat.regptr->CLOCK |= MCI_CLOCK_ENABLE; // Enable Clock

	for (i = 0; i < 0x100; i++)
		; /* delay 3MCLK + 2PCLK before next write */

	return;
}

/******************************************************************************
* Function:		MCI_SetBusWidth
*
* Purpose:		Set data bus width: 1-bit bus or 4-bit bus.
*
* Parameter:
* 			width:	bus width
*
* Return:		TRUE or FALSE
*
*****************************************************************************/
U32 MCI_SetBusWidth(U32 width) {
	__IO U32 i;

	for (i = 0; i < 0x10; i++)
		; /* delay 3MCLK + 2PCLK  */

	if (width == BUS_WIDTH_1BIT) {
		mcidrvdat.regptr->CLOCK &= ~MCI_CLOCK_WIDEBUS;
	}
	if (width == BUS_WIDTH_4BITS) {
		mcidrvdat.regptr->CLOCK |= MCI_CLOCK_WIDEBUS;
	}
	if (MCI_SendACMDBusWidth(BUS_WIDTH_4BITS) == FALSE) {
		return (FALSE);
	}

	return TRUE;
}

/******************************************************************************
* Function:		MCI_Start
*
* Purpose:		Set MCI clock and power registers, setup VIC for
*					data interrupt.
*
* Parameter:	None
*
* Return:		TRUE or FALSE
* 				if VIC table is full, return FALSE
*
*****************************************************************************/
S32 MCI_Start() {
	__IO U32 i;

	mcidrvdat.regptr = (MCI_Type *) MMC_SD;

	SCB_EnablePPWR(PCSDC); /* Enable clock to the MCI block */

	if ( mcidrvdat.regptr->CLOCK & (1 << 8)) {
		mcidrvdat.regptr->CLOCK &= ~(1 << 8);
	}
	if ( mcidrvdat.regptr->POWER & 0x02) {
		mcidrvdat.regptr->POWER = 0;
	}
	for (i = 0; i < 0x10000; i++)
		;

	/* Disable all interrupts for now */
	mcidrvdat.regptr->MASK0 = 0;
	mcidrvdat.regptr->MASK1 = 0;

	/* Set MICPWR pin is high */
	SCB_SetMCIPWR(TRUE);
	/* Set MCI Clock Divide = 1
	 * If CCLK = 48Mhz, MCI_CLOCK = 48Mhz*/
	SCB_SetPCLKDiv(PCLK_MCI, CCLK_DIV_1);

	/* Configure pin I/O for MCI */
	PINSEL_Set(PORT_0, 19, FUNC_2); // MCICLK pin
	PINSEL_Set(PORT_0, 20, FUNC_2); // MCICMD pin
	PINSEL_Set(PORT_0, 21, FUNC_2); // MCIPWR pin

	PINSEL_Set(PORT_0, 22, FUNC_2); // MCIDAT0 pin
	PINSEL_Set(PORT_2, 11, FUNC_2); // MCIDAT1 pin
	PINSEL_Set(PORT_2, 12, FUNC_2); // MCIDAT2 pin
	PINSEL_Set(PORT_2, 13, FUNC_2); // MCIDAT3 pin


	/*set up clocking default mode, clear any registers as needed */
	mcidrvdat.regptr->COMMAND = 0;
	mcidrvdat.regptr->DATACTRL = 0;
	mcidrvdat.regptr->CLEAR = 0x7FF;

	mcidrvdat.regptr->POWER = MCI_POWER_UP;

	while (!(mcidrvdat.regptr->POWER & MCI_POWER_UP))
		;

	for (i = 0; i < 5000; i++)
		;

	/* During identification phase, the clock should be less than
	 400Khz. Once we pass this phase, the normal clock can be set up
	 to 25Mhz on SD card and 20Mhz on MMC card. */
	MCI_SetClockRate(MCI_SLOW_RATE);
	mcidrvdat.regptr->POWER |= 0x01;

	for (i = 0; i < 50000; i++)
		;

	/* Install MCI interrupt handler */
	if (VIC_InstallIRQ(SD_MMC_INT, (void *) MCI_ISR, 2) == _ERROR) {
		return (_ERROR);
	}
	/* Enable MCI interrupt */
	VIC_EnableInt(SD_MMC_INT);
	/* During the initialization phase, to simplify the process, the CMD related
	 interrupts are disabled. The DATA related interrupts are enabled when
	 the FIFOs are used and just before WRITE_BLOCK READ_BLOCK cmds are issues, and
	 disabled after the data block has been written and read. Please also note,
	 before WRITE_BLOCK only TX related data interrupts are enabled, and before
	 READ_BLOCK only RX related data interrupts are enabled. */
	mcidrvdat.init = TRUE;
	return (TRUE);

}

/******************************************************************************
* Function:		MCI_SendCommand
*
* Purpose:		The routine is used to send a CMD to the card
* 					and get Command Response from card
*
* Parameter:
* 			cmd : Command Index in MCI_CMD enum
* 			arg : Command Argument
* 			expect_reponse : Command Response Type.
* 							 expect response flag could be  EXPECT_NO_RESP
* 															EXPECT_SHORT_RESP
* 															EXPECT_LONG_RESP
* 			allow_timeout :	Command Timeout
* 			cmd_resp : Command Response Data
*
* Return:		INVALID_RESPONSE : invalid response
* 				0				 : valid response
*
*****************************************************************************/
S32 MCI_SendCommand(U32 cmd, U32 arg, U32 expect_response, U32 allow_timeout, U32* cmd_resp){

	U32 CmdData = 0;
	U32 CmdStatus;
	U32 LastCmdIndex;

	CmdStatus = mcidrvdat.regptr->STATUS;

	/*set the command details, the CmdIndex should 0 through 0x3F only */
	CmdData |= MCI_COMMAND_CMDINDEX(cmd);

	switch (expect_response) {
	case EXPECT_SHORT_RESP:
		CmdData |= MCI_COMMAND_RESPONSE;
		break;
	case EXPECT_LONG_RESP:
		CmdData |= MCI_COMMAND_RESPONSE | MCI_COMMAND_LONGRSP;
		break;
	}

	if (allow_timeout) /* allow timeout or not */
		CmdData |= MCI_COMMAND_INTERRUPT;
	else
		CmdData &= ~MCI_COMMAND_INTERRUPT;

	/*send the command*/
	CmdData |= MCI_COMMAND_ENABLE;
	mcidrvdat.regptr->ARGUMENT = arg;
	mcidrvdat.regptr->COMMAND = CmdData;

	if (expect_response == EXPECT_NO_RESP) {
		while ((CmdStatus = mcidrvdat.regptr->STATUS) & MCI_CMDACTIVE) {
			mcidrvdat.regptr->CLEAR = 0x7FF;
		}
		return 0;
	}

	while (1) {
		CmdStatus = mcidrvdat.regptr->STATUS;
		if (CmdStatus & MCI_CMDTIMEOUT) {
			mcidrvdat.regptr->CLEAR = CmdStatus | MCI_CMDTIMEOUT;
			mcidrvdat.regptr->COMMAND = 0;
			mcidrvdat.regptr->ARGUMENT = 0xFFFFFFFF;
			return (CmdStatus);
		}
		if (CmdStatus & MCI_CMDCRCFAIL) {
			mcidrvdat.regptr->CLEAR = CmdStatus | MCI_CMDCRCFAIL;
			LastCmdIndex = mcidrvdat.regptr->COMMAND & 0x003F;
			/* ignore CRC error if it's a response for SEND_OP_COND or STOP_TRANSMISSION */
			if ((LastCmdIndex == SEND_OP_COND) || (LastCmdIndex
					== SEND_APP_OP_COND) || (LastCmdIndex == STOP_TRANSMISSION)) {
				mcidrvdat.regptr->COMMAND = 0;
				mcidrvdat.regptr->ARGUMENT = 0xFFFFFFFF;
				break;
			} else {
				return (CmdStatus);
			}
		}
		if (CmdStatus & MCI_CMDRESPEND) {
			mcidrvdat.regptr->CLEAR = CmdStatus | MCI_CMDRESPEND;
			break; /* cmd response is received, expecting response */
		}
	}

	if (MCI_RESPCMD_CMDINDEX(mcidrvdat.regptr->RESPCMD) != cmd) {
		/* If the response is not R1, in the response field, the Expected Cmd data
		 won't be the same as the CMD data in SendCmd(). Below four cmds have
		 R2 or R3 response. We don't need to check if MCI_RESP_CMD is the same
		 as the Expected or not. */
		if ((cmd != SEND_OP_COND) && (cmd
				!= SEND_APP_OP_COND) && (cmd != ALL_SEND_CID)
				&& (cmd != SEND_CSD)) {
			CmdStatus = INVALID_RESPONSE;
			return (INVALID_RESPONSE);
		}
	}

	if (expect_response == EXPECT_SHORT_RESP) {
		*cmd_resp = mcidrvdat.regptr->RESPONSE0;
	} else if (expect_response == EXPECT_LONG_RESP) {
		*cmd_resp = mcidrvdat.regptr->RESPONSE0;
		*(cmd_resp + 1) = mcidrvdat.regptr->RESPONSE1;
		*(cmd_resp + 2) = mcidrvdat.regptr->RESPONSE2;
		*(cmd_resp + 3) = mcidrvdat.regptr->RESPONSE3;
	}

	return (0); /* Read MCI_RESP0 register assuming it's not long response. */
}

/******************************************************************************
* Function:		MCI_GoIdleState
*
* Purpose:		CMD0, the very first command to be sent to initialize
*						either MMC or SD card.
*
* Parameter:	None
*
* Return:		TRUE or FALSE, TRUE if card has been initialized.
*
*****************************************************************************/
U32 MCI_GoIdleState(void) {
	U32 retryCount;
	U32 respStatus;
	U32 respValue[4];

	retryCount = 50;
	while (retryCount > 0) {
		/* Send CMD0 command repeatedly until the response is back correctly */
		respStatus = MCI_SendCommand(GO_IDLE_STATE, 0,
									EXPECT_NO_RESP, 0, (U32 *) respValue);
		if (respStatus == 0) {
			break;
		}
		retryCount--;
	}

	if (respStatus != 0) /* timeout, give up */
	{
		return (FALSE);
	}

	return (TRUE);
}

/******************************************************************************
* Function:		MCI_SendOpCond
*
* Purpose:		CMD1 for MMC
*
* Parameter:	None
*
* Return:		TRUE or FALSE, TRUE if card has response back before
*					timeout, FALSE is timeout on the command.
*
*****************************************************************************/
U32 MCI_SendOpCond(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	retryCount = 200; /* reset retry counter */
	while (retryCount > 0) {
		/* Send CMD1 command repeatedly until the response is back correctly */
		respStatus = MCI_SendCommand(SEND_OP_COND, OCR_INDEX,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		if (!(respStatus & MCI_CMDTIMEOUT) && (respValue[0] & 0x80000000)) {
			return (TRUE); /* response is back and correct. */
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}

	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendACMD
*
* Purpose:		CMD55, before sending an ACMD, call this routine first
*
* Parameter:	None
*
* Return:		TRUE or FALSE, TRUE if card has responded before timeout.
*					FALSE is timeout.
*
*****************************************************************************/
U32 MCI_SendACMD(void) {
	U32 i, retryCount;
	U32 CmdArgument;
	U32 respStatus;
	U32 respValue[4];

	if (MCI_CardType == SD_CARD) {
		CmdArgument = CardRCA; /* Use the address from SET_RELATIVE_ADDR cmd */
	} else /* if MMC or unknown card type, use 0x0. */
	{
		CmdArgument = 0x00000000;
	}

	retryCount = 20;
	while (retryCount > 0) {
		/* Send CMD55 command followed by an ACMD */
		respStatus = MCI_SendCommand(APP_CMD, CmdArgument,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		if (!respStatus && (respValue[0] & CARD_STATUS_ACMD_ENABLE)) /* Check if APP_CMD enabled */
		{
			return (TRUE);
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendACMDOpCond
*
* Purpose:		If SEND_OP_COND is timeout, it's not a MMC card, try
*					this combination to see if we can communicate with
*					a SD card.
*
* Parameter:	None
*
* Return:		TRUE or FALSE, TRUE if card has been initialized.
*
*****************************************************************************/
U32 MCI_SendACMDOpCond(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	/* timeout on SEND_OP_COND command on MMC, now, try SEND_APP_OP_COND
	 command to SD */
	retryCount = 100; /* reset retry counter */
	while (retryCount > 0) {
		mcidrvdat.regptr->POWER &= ~MCI_POWER_OPENDRAIN; /* Clear Open Drain output control for SD */
		for (i = 0; i < 3000; i++)
			;
		if (MCI_SendACMD() == FALSE) {
			retryCount--;
			continue;
		}
		/* Send ACMD41 command repeatedly until the response is back correctly */
		respStatus = MCI_SendCommand(SEND_APP_OP_COND, OCR_INDEX,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		if (!(respStatus & MCI_CMDTIMEOUT) && (respValue[0] & 0x80000000)) {
			return (TRUE); /* response is back and correct. */
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}

	return (FALSE);
}

/******************************************************************************
* Function:		MCI_CardInit
*
* Purpose:		Try CMD1 first for MMC, if it's timeout, try CMD55
*					and CMD41 for SD, if both failed, initialization failure,
*					bailout with unknown card type. Otherwise, return the
*					card type, either MMC or SD.
*
* Parameter:	None
*
* Return:		Card type.
*
*****************************************************************************/
U32 MCI_CardInit(void) {
	U32 i;

	/* Reset card using CMD0 */
	if (MCI_GoIdleState() == FALSE) {
		return (CARD_UNKNOWN);
	}

	MCI_BusMode(OPEN_DRAIN);

	for ( i = 0; i < 0x3000; i++ );

	/* Try CMD1 first for MMC, if it's timeout, try CMD55 and CMD41 for SD,
	 if both failed, initialization failure, timeout */
	if (MCI_SendOpCond() == TRUE) {
		MCI_CardType = MMC_CARD;
		return (MCI_CardType); /* Found the card, it's a MMC */
	}
	if (MCI_SendACMDOpCond() == TRUE) {
		MCI_CardType = SD_CARD;
		return (MCI_CardType); /* Found the card, it's a SD */
	}

	/* tried both MMC and SD card, give up */
	return (CARD_UNKNOWN);
}

/******************************************************************************
* Function:		MCI_CheckCID
*
* Purpose:		Send CMD2, ALL_SEND_CID
*
* Parameter:	None
*
* Return:		If not timeout, return TRUE.
*
*****************************************************************************/
U32 MCI_CheckCID(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	/* This command is normally after CMD1(MMC) or ACMD41(SD). */
	retryCount = 50; /* reset retry counter */
	while (retryCount > 0) {
		/* Send CMD2 command repeatedly until the response is back correctly */
		respStatus = MCI_SendCommand(ALL_SEND_CID, 0,
									EXPECT_LONG_RESP, 0, (U32 *) &respValue[0]);
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		if (!(respStatus & MCI_CMDTIMEOUT)) {
			return (TRUE); /* response is back and correct. */
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SetAddress
*
* Purpose:		Send CMD3, SET_RELATIVE_ADDR, should after CMD2
*
* Parameter:	None
*
* Return:		TRUE if response is back before timeout.
*
*****************************************************************************/
U32 MCI_SetAddress(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 CmdArgument;

	/* If it's a SD card, SET_RELATIVE_ADDR is to get the address
	 from the card and use this value in RCA, if it's a MMC, set default
	 RCA address. 0x00010000. */
	if (MCI_CardType == MMC_CARD) {
		CmdArgument = 0x00010000;
	} else /* If it's unknown or MMC_CARD, fix the RCA address */
	{
		CmdArgument = 0;
	}

	retryCount = 50; /* reset retry counter */
	while (retryCount > 0) {
		/* Send CMD3 command repeatedly until the response is back correctly */
		respStatus = MCI_SendCommand(SET_RELATIVE_ADDR, CmdArgument,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		/* It should go to IDEN state and bit 8 should be 1 */
		if (!(respStatus & MCI_CMDTIMEOUT) && ((respValue[0] & (0x0F << 8))
				== 0x0500)) {
			CardRCA = respValue[0] & 0xFFFF0000; /* Save the RCA value from SD card */
			return (TRUE); /* response is back and correct. */
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendCSD
*
* Purpose:		CMD9, SEND_CSD cmd, it should be sent only at
*					STBY state and after CMD3. See MMC and SD spec. state
*					diagram.
*
* Parameter:
* 			csd : CSD data buffer
*
* Return:		TRUE or FALSE
*
*****************************************************************************/
U32 MCI_SendCSD(U32 *csd) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 CmdArgument;

	if (MCI_CardType == SD_CARD) {
		CmdArgument = CardRCA;
	} else /* if MMC or unknown card type, use default RCA address. */
	{
		CmdArgument = 0x00010000;
	}

	retryCount = 50;
	while (retryCount > 0) {
		/* Send SET_BLOCK_LEN command before read and write */
		mcidrvdat.regptr->CLEAR |= (MCI_CMDTIMEOUT| MCI_CMDCRCFAIL
		| MCI_CMDRESPEND);
		respStatus = MCI_SendCommand(SEND_CSD, CmdArgument,
									EXPECT_LONG_RESP, 0, (U32 *) &respValue[0]);
		if (!respStatus) {
			for (i = 0; i < 4; i++) {
				csd[i] = respValue[i];
			}
			return (TRUE);
		}
		for (i = 0; i < 50; i++)
		;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SelectCard
*
* Purpose:		CMD7, SELECT_CARD, should be after CMD9, the state
*						will be inter-changed between STBY and TRANS after
*						this cmd.
*
* Parameter:	None
*
* Return:		return FALSE if response times out.
*
*****************************************************************************/
U32 MCI_SelectCard(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 CmdArgument;

	if (MCI_CardType == SD_CARD) {
		CmdArgument = CardRCA;
	} else /* if MMC or unknown card type, use default RCA addr. */
	{
		CmdArgument = 0x00010000;
	}

	retryCount = 50;
	while (retryCount > 0) {
		/* Send SELECT_CARD command before read and write */
		mcidrvdat.regptr->CLEAR |= (MCI_CMDTIMEOUT| MCI_CMDCRCFAIL
		| MCI_CMDRESPEND);

		respStatus = MCI_SendCommand(SELECT_CARD, CmdArgument,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0700)) {
			/* Should be in STANDBY state now and ready */
			return (TRUE);
		}
		for (i = 0; i < 50; i++)
		;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendStatus
*
* Purpose:		CMD13, SEND_STATUS, the most important cmd to
*						debug the state machine of the card.
*
* Parameter:	None
*
* Return:		Response value(card status), true if the ready bit
*					is set in the card status register, if timeout, return
*					INVALID_RESPONSE 0xFFFFFFFF.
*
*****************************************************************************/
U32 MCI_SendStatus(void) {
	U32 retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 CmdArgument;

	if (MCI_CardType == SD_CARD) {
		CmdArgument = CardRCA;
	} else /* if MMC or unknown card type, use default RCA addr. */
	{
		CmdArgument = 0x00010000;
	}

	/* Note that, since it's called after the block write and read, this timeout
	 is important based on the clock you set for the data communication. */
	retryCount = 2000;
	while (retryCount > 0) {
		/* Send SELECT_CARD command before read and write */
		mcidrvdat.regptr->CLEAR |= (MCI_CMDTIMEOUT| MCI_CMDCRCFAIL
		| MCI_CMDRESPEND);
		respStatus = MCI_SendCommand(SEND_STATUS, CmdArgument,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		if (!respStatus && (respValue[0] & (1 << 8))) { /* The ready bit should be set, it should be in either TRAN or RCV state now */
			return (respValue[0]);
		}
		retryCount--;
	}
	return (INVALID_RESPONSE);
}

/******************************************************************************
* Function:		MCI_SetBlockLength
*
* Purpose:		CMD16, SET_BLOCKLEN, called after CMD7(SELECT_CARD)
*					called in the TRANS state.
*
* Parameter:
* 			blockLength : The length of the data block to be written or read
*
* Return:		TRUE or FALSE, return TRUE if ready bit is set, and it's
*					in TRANS state.
*
*****************************************************************************/
U32 MCI_SetBlockLength(U32 blockLength) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	retryCount = 50;
	while (retryCount > 0) {
		/* Send SET_BLOCK_LEN command before read and write */
		mcidrvdat.regptr->CLEAR |= (MCI_CMDTIMEOUT| MCI_CMDCRCFAIL
		| MCI_CMDRESPEND);
		respStatus = MCI_SendCommand(SET_BLOCK_LEN, blockLength,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		/* bit 9 through 12 should be in transfer state now. bit 8 is ready. */
		if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900)) {
			return (TRUE);
		}
		for (i = 0; i < 50; i++)
		;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendACMDBusWidth
*
* Purpose:		ACMD6, SET_BUS_WIDTH, if it's SD card, we can
*					use the 4-bit bus instead of 1-bit. This cmd
*					can only be called during TRANS state.
*					Since it's a ACMD, CMD55 APP_CMD needs to be
*					sent out first.
*
* Parameter:
* 			busWidth : Bus width value, 1-bit is 0, 4-bit is 10
*
* Return:		TRUE or FALSE, TRUE if the card is still in the
*					TRANS state after the cmd.
*
*****************************************************************************/
U32 MCI_SendACMDBusWidth(U32 busWidth) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	retryCount = 0x20; /* reset retry counter */
	while (retryCount > 0) {
		if (MCI_SendACMD() == FALSE) {
			continue;
		}
		/* Send ACMD6 command to set the bus width */
		respStatus = MCI_SendCommand(SET_ACMD_BUS_WIDTH, busWidth, EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900)) {
			return (TRUE); /* response is back and correct. */
		}
		for (i = 0; i < 0x20; i++)
			;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendStop
*
* Purpose:		CMD12, STOP_TRANSMISSION. if that happens, the card is
*					maybe in a unknown state that need a warm reset.
*
* Parameter:	None
*
* Return:		TRUE or FALSE, TRUE if, at least, the card status
*					shows ready bit is set.
*
*****************************************************************************/
U32 MCI_SendStop(void) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];

	retryCount = 50;
	while (retryCount > 0) {
		mcidrvdat.regptr->CLEAR = 0x7FF;
		respStatus = MCI_SendCommand(STOP_TRANSMISSION, 0,
									EXPECT_SHORT_RESP, 0, (U32 *) respValue);
		/* ready bit, bit 8, should be set in the card status register */
		if (!respStatus && (respValue[0] & (1 << 8))) {
			return (TRUE);
		}
		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}
	return (FALSE);
}

/******************************************************************************
* Function:		MCI_SendWriteBlock
*
* Purpose:		CMD24, WRITE_BLOCK, send this cmd in the TRANS state
*					to write a block of data to the card
*
* Parameter:
* 			blockAddress : address of block
* 			numOfBlock : number of block to be written
*
* Return:		TRUE or FALSE
*
*****************************************************************************/
U32 MCI_SendWriteBlock(U32 blockAddress, U32 numOfBlock) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 commandID;

	if (numOfBlock > 1)
		commandID = WRITE_MULTIPLE_BLOCK;
	else
		commandID = WRITE_BLOCK;

	retryCount = 0x20;
	while (retryCount > 0) {
		mcidrvdat.regptr->CLEAR = 0x7FF;

		respStatus = MCI_SendCommand(commandID, blockAddress * BLOCK_LENGTH, EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		/* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
		if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900)) {
			return (TRUE); /* ready and in TRAN state */
		}

		for (i = 0; i < 50; i++)
			;
		retryCount--;
	}
	return (FALSE); /* Fatal error */
}

/******************************************************************************
* Function:		MCI_SendReadBlock
*
* Purpose:		CMD17, READ_SINGLE_BLOCK, send this cmd in the TRANS
*					state to read a block of data from the card.
*
* Parameter:
* 			blockAddress : address of block
* 			numOfBlock :  number of block to be read
*
* Return:		TRUE of FALSE
*
*****************************************************************************/
U32 MCI_SendReadBlock(U32 blockAddress, U32 numOfBlock) {
	U32 i, retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 commandID;

// To Do: Read Multi-Block
//	if (numOfBlock > 1)
//		commandID = READ_MULTIPLE_BLOCK;
//	else
		commandID = READ_SINGLE_BLOCK;

	retryCount = 0x20;
	while (retryCount > 0) {
		mcidrvdat.regptr->CLEAR = 0x7FF;
		respStatus = MCI_SendCommand(commandID, blockAddress * BLOCK_LENGTH,
									EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
		/* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
		if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900)) {
			return (TRUE); /* ready and in TRAN state */
		}
		for (i = 0; i < 0x20; i++)
			;
		retryCount--;
	}
	return (FALSE); /* Fatal error */
}
/******************************************************************************
* Function:		MCI_WriteBlock
*
* Purpose:		Set MCI data control register, data length and data
*					timeout, send WRITE_BLOCK cmd, finally, enable
*					interrupt. On completion of WRITE_BLOCK cmd, TX_ACTIVE
*					interrupt will occurs, data can be written continuously
*					into the FIFO until the block data length is reached
*				In Interrupt mode, this function allows write one block
*					at a time
*
* Parameter:
* 			blockAdrress : address of block
* 			dataBuffer :  data buffer
* 			numOfBlock : number of block to be written
*
* Return:		TRUE or FALSE, if cmd times out, return FALSE and no
*					need to continue.
*
*****************************************************************************/
U32 MCI_WriteBlock(U32 blockAdrress, S8* dataBuffer, U32 numOfBlock) {
	__IO U32 i;
	__IO U32 j;
	U32 DataCtrl = 0;

	mcidrvdat.regptr->CLEAR = 0x7FF;
	mcidrvdat.regptr->DATACTRL = 0;
	for (i = 0; i < 20; i++)
		;

	/* Below status check is redundant, but ensure card is in TRANS state
	 before writing and reading to from the card length. */
	if (MCI_CheckStatus() != TRUE) {
		MCI_SendStop();
		return (FALSE);
	}

	mcidrvdat.regptr->DATATIMER = DATA_TIMER_VALUE;
	mcidrvdat.regptr->DATALENGTH = BLOCK_LENGTH;
	MCI_Block_End_Flag = 1;
	MCI_TXEnable();
	if (MCI_SendWriteBlock(blockAdrress, numOfBlock) == FALSE) {
		return (FALSE);
	}
#if MCI_DMA_ENABLED
	for (j = 0; j < numOfBlock; j++, dataBuffer = dataBuffer + BLOCK_LENGTH) {
		memcpy(WriteBlock, dataBuffer, DMA_SIZE);

		GPDMA_Config(0, GPDMA_M2P_P, DMA_SIZE, GPDMA_BSIZE_32, GPDMA_BSIZE_8, GPDMA_WIDTH_WORD,
						GPDMA_WIDTH_WORD, MCI_DMA_SRC, (U32) &MCI_FIFO, 0, GPDMA_PSRC_SDMMC);

		/* Write, block transfer, DMA, and data length */
		DataCtrl |= (MCI_DATACTRL_ENABLE| MCI_DATACTRL_DMAENABLE
		| MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
		mcidrvdat.regptr->DATACTRL = DataCtrl;

		for (i = 0; i < 10000; i++) {
			if (GPDMA->RAWINTTCSTATUS & 0x01) {
				/* Data transfer finished */
				break;
			}
		}
		if (i == 0) {
			MCI_SendStop();
			return FALSE;
			break;
		}
		if (numOfBlock == 1) {
			return TRUE;
		}
		for (i = 0; i < 10; i++)
		;
	}
	MCI_SendStop();
#else
	mcidrvdat.WriteDataBuffer = dataBuffer;
	/* Write, block transfer, and data length */
	DataCtrl |= (MCI_DATACTRL_ENABLE | MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
	mcidrvdat.regptr->DATACTRL = DataCtrl;
#endif

	return (TRUE);
}

/******************************************************************************
* Function:		MCI_ReadBlock
*
* Purpose:		Set MCI data control register, data length and data
*					timeout, send READ_SINGLE_BLOCK cmd, finally, enable
*					interrupt. On completion of READ_SINGLE_BLOCK cmd,
*					RX_ACTIVE interrupt will occurs, data can be read
*					continuously into the FIFO until the block data
*					length is reached.
*				This Function only allows read one block at a time
*
* Parameter:
* 			blockAdrress : address of block
* 			dataBuffer :  data buffer
* 			numOfBlock : number of block to be read
*
* Return:		TRUE or FALSE, if cmd times out, return FALSE and no
*					need to continue.
*
*****************************************************************************/
U32 MCI_ReadBlock(U32 blockAdrress, S8* dataBuffer, U32 numOfBlock) {
	__IO U32 i;
	U32 DataCtrl = 0;

	if (numOfBlock > 1)
		return FALSE;
	// To Do: Read Multi-Block

	mcidrvdat.regptr->CLEAR = 0x7FF;
	mcidrvdat.regptr->DATACTRL = 0;
	for (i = 0; i < 0x10; i++)
		;

	/* Below status check is redundant, but ensure card is in TRANS state
	 before writing and reading to from the card. */
	if (MCI_CheckStatus() != TRUE) {
		MCI_SendStop();
		return (FALSE);
	}
	MCI_RXEnable();

	mcidrvdat.regptr->DATATIMER = DATA_TIMER_VALUE;
	mcidrvdat.regptr->DATALENGTH = BLOCK_LENGTH;
	MCI_Block_End_Flag = 1;
	if (MCI_SendReadBlock(blockAdrress, numOfBlock) == FALSE) {
		return (FALSE);
	}

#if MCI_DMA_ENABLED
	GPDMA_Config(1, GPDMA_P2M_P, DMA_SIZE, GPDMA_BSIZE_8, GPDMA_BSIZE_32, GPDMA_WIDTH_WORD,
					GPDMA_WIDTH_WORD, (U32) &MCI_FIFO, MCI_DMA_DST, GPDMA_PSRC_SDMMC, 0);

	/* Write, block transfer, DMA, and data length */
	DataCtrl |= (MCI_DATACTRL_ENABLE| MCI_DATACTRL_DIRECTION
				| MCI_DATACTRL_DMAENABLE | MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
	mcidrvdat.regptr->DATACTRL = DataCtrl;

	for (i = 0; i < 10000; i++) {
		if (GPDMA->RAWINTTCSTATUS & 0x02) {
			/* Data transfer finished */
			break;
		}
	}
	if (i == 0 && i == 10000) {
		return FALSE;
	} else memcpy(dataBuffer, ReadBlock, DMA_SIZE);

#else
	mcidrvdat.ReadDataBuffer = dataBuffer;
	/* Read, enable, block transfer, and data length */
	DataCtrl |= (MCI_DATACTRL_ENABLE | MCI_DATACTRL_DIRECTION
			| MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
	mcidrvdat.regptr->DATACTRL = DataCtrl;
#endif

	for (i = 0; i < 0x10; i++);

	return (TRUE);
}

/******************************************************************************
* Function:		MCI_SDStatus (This function is used by ChaN FAT)
*
* Purpose:		reads the 512bit SD_STATUS (3-22) using ACMD13 (4-37)
*					Card must be in trans-state for this (4-7)
*					( SD-Status in ReadBlock when MCI_Block_End_Flag==1 )
*
* Parameter:	None
*
* Return:		TRUE or FALSE
*
*****************************************************************************/
U32 MCI_SDStatus(void) {
	__IO U32 i;
	U32 retryCount;
	U32 respStatus;
	U32 respValue[4];
	U32 DataCtrl = 0;

	if ((MCI_CardType == CARD_UNKNOWN) || (MCI_CardType == MMC_CARD)) {
		return (FALSE); // unsupported
	}

	mcidrvdat.regptr->CLEAR = 0x7FF;
	mcidrvdat.regptr->DATACTRL = 0;
	for (i = 0; i < 0x10; i++) {
		;
	}

	/* Below status check is redundant, but ensure card is in TRANS state
	 before writing and reading to from the card. */
	if (MCI_CheckStatus() != TRUE) {
		MCI_SendStop();
		return (FALSE);
	}

	MCI_RXEnable();

	mcidrvdat.regptr->DATATIMER = DATA_TIMER_VALUE;
	mcidrvdat.regptr->DATALENGTH = BLOCK_LENGTH;

	MCI_Block_End_Flag = 1;
	SDStatRead = 1;

	retryCount = 0x20;
	while (retryCount > 0) {
		mcidrvdat.regptr->CLEAR = 0x7FF;
		// prepare for ACMD (with CMD55)
		if (MCI_SendACMD() == TRUE) {
			// Send ACMD13
			respStatus = MCI_SendCommand(SEND_STATUS, 0,
					EXPECT_SHORT_RESP, 0, (U32 *) &respValue[0]);
			/* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
			if (!respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900)) {
				break; /* ready and in TRAN state */
			}
		}
		for (i = 0; i < 0x20; i++) {
			;
		}
		retryCount--;
	}
	if (retryCount == 0) {
		return (FALSE); // error sending ACMD13 or state not trans
	}

#if MCI_DMA_ENABLED
	GPDMA_Config(1, GPDMA_P2M_P, DMA_SIZE, GPDMA_BSIZE_8, GPDMA_BSIZE_32, GPDMA_WIDTH_WORD,
					GPDMA_WIDTH_WORD, (U32) &MCI_FIFO, MCI_DMA_DST, GPDMA_PSRC_SDMMC, 0);

	/* Write, block transfer, DMA, and data length */
	DataCtrl |= (MCI_DATACTRL_ENABLE| MCI_DATACTRL_DIRECTION
				| MCI_DATACTRL_DMAENABLE | MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
	mcidrvdat.regptr->DATACTRL = DataCtrl;

#else
	/* Read, enable, block transfer, and data length */
	DataCtrl |= (MCI_DATACTRL_ENABLE | MCI_DATACTRL_DIRECTION
			| MCI_DTATCTRL_BLOCKSIZE(DATA_BLOCK_LEN));
	mcidrvdat.regptr->DATACTRL = DataCtrl;
#endif

	// mci_debug_printf("MCI_SD_Status: DATA_CTRL set\n");
	for (i = 0; i < 0x10; i++) {
		;
	}

	return (TRUE);
}

/******************************************************************************
* Function:			MCI_PowerOff
*
* Descriptions:		disable power to MCI (Power-Reg)
*
* Parameter:		None
*
* Return:			None
*
*****************************************************************************/
void MCI_PowerOff(void) {
	__IO U32 i;

	mcidrvdat.regptr->POWER = 0;
	for (i = 0; i < 0x100; i++)
		;
	return;
}
