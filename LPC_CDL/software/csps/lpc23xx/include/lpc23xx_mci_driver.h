/***********************************************************************
 * $Id: lpc23xx_mci_driver.h 28 2014-01-27 14:45:13Z danielru $
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
#ifndef LPC23XX_MCI_DRIVER_H_
#define LPC23XX_MCI_DRIVER_H_

#ifdef __cplusplus
EXTERN "C"
{
#endif
#include "lpc23xx_mci.h"

/* Enable/Disable DMA for MCI */
#define MCI_DMA_ENABLED 1

EXTERN __IO U32 MCI_DataErrorProcess_count;
EXTERN __IO U32 MCI_DATA_END_InterruptService_count;
EXTERN __IO U32 MCI_FIFOInterruptService_count;
EXTERN __IO U32 MCI_CmdProcess_count;

EXTERN __IO U32 CmdCRCErrCount;
EXTERN __IO U32 CmdTimeoutErrCount;
EXTERN __IO U32 CmdRespEndCount;
EXTERN __IO U32 CmdSentCount;
EXTERN __IO U32 CmdActiveCount;

EXTERN __IO U32 DataCRCErrCount;
EXTERN __IO U32 DataTimeoutErrCount;
EXTERN __IO U32 DataTxUnderrunErrCount;
EXTERN __IO U32 DataRxOverrunErrCount;
EXTERN __IO U32 DataStartbitErrCount;

EXTERN __IO U32 DataEndCount;
EXTERN __IO U32 DataBlockEndCount;
EXTERN __IO U32 MCI_Block_End_Flag;

EXTERN __IO U32 DataTxActiveCount;
EXTERN __IO U32 DataRxActiveCount;

EXTERN __IO U32 DataFIFOCount;
EXTERN __IO U32 DataRxFIFOCount;
EXTERN __IO U32 DataTxFIFOCount;

EXTERN __IO U32 TXBlockCounter, RXBlockCounter;

EXTERN __IO U32 MCI_CardType;
EXTERN __IO U32 CardRCA;

EXTERN __IO S8 *WriteBlock; /* treat WriteBlock as a constant address */
EXTERN __IO S8 *ReadBlock; /* treat ReadBlock as a constant address */

/* DMA address is used in MCI */
typedef enum {
	MCI_DMA_SRC = 0x7FD00000, /* This is the area original data is stored
	 or data to be written to the SD/MMC card. */
	MCI_DMA_DST = 0x7FD01000, /* This is the area, after reading from the SD/MMC*/
} MCI_DMA_ADDR;

/* SD/MMC Command list, per MMC spec. SD Memory Card Spec. Simplified version */
typedef enum {
	GO_IDLE_STATE = 0, /* GO_IDLE_STATE(MMC) or RESET(SD) */
	SEND_OP_COND = 1, /* SEND_OP_COND(MMC) or ACMD41(SD) */
	ALL_SEND_CID = 2, /* ALL SEND_CID */
	SET_RELATIVE_ADDR = 3, /* SET_RELATE_ADDR */
	SET_ACMD_BUS_WIDTH = 6, /* SET ACMD BUS WIDTH (SD) */
	SELECT_CARD = 7, /* SELECT/DESELECT_CARD */
	SEND_CSD = 9, /* SEND_CSD */
	STOP_TRANSMISSION = 12, /* Stop either READ or WRITE operation */
	SEND_STATUS = 13, /* SEND_STATUS */
	SET_BLOCK_LEN = 16, /* SET_BLOCK_LEN */
	READ_SINGLE_BLOCK = 17, /* READ_SINGLE_BLOCK */
	READ_MULTIPLE_BLOCK = 18, /* Read Multiple Blocks */
	WRITE_BLOCK = 24, /* WRITE_BLOCK */
	WRITE_MULTIPLE_BLOCK = 25, /* WRITE_MULTIPLE_BLOCK */
	SEND_APP_OP_COND = 41, /* ACMD41 for SD card */
	APP_CMD = 55, /* APP_CMD */
} MCI_CMD;

/* MCI Clock Rate*/
typedef enum {
	/* Slow rate is used when Card is init
	The rate value is defined in MCI_CLK_RATE */
	MCI_SLOW_RATE = 1,
	/* Normal rate for data transfer
	The rate value is defined in MCI_CLK_RATE */
	MCI_NORMAL_RATE = 2,
} MCI_CLOCK_RATE;

/* MCI Command response type*/
typedef enum {
	EXPECT_NO_RESP = 0, /* Command no response */
	EXPECT_SHORT_RESP = 1, /* Command short response (1 word)*/
	EXPECT_LONG_RESP = 2, /* Command long response (4 words)*/
} MCI_CMD_RES_TYPE;

/* MCI clock frequency = MCLK/(2* (CMI_CLK_RATE +1) */
typedef enum {
	MCI_CLKDIV_SLOW = 59, /* if MCLK = 48Mhz, MCI_CLK = 400Khz */
#if MCI_DMA_ENABLED
	MCI_CLKDIV_NORMAL = 1, /* if MCLK = 48Mhz, MCI_CLK = 12MHz */
#else // Interrupt mode: in high rate, it occurs Underrun Error
	MCI_CLKDIV_NORMAL = 11 /* if MCLK = 48Mhz, MCI_CLK = 2MHz */
#endif
} MCI_CLK_RATE;

/* Bus width */
typedef enum {
	BUS_WIDTH_1BIT = 0, /* 1 bit bus width */
	BUS_WIDTH_4BITS = 10, /* 4 bits bus width */
} MCI_BUS_WIDTH;

/* Card type */
typedef enum {
	CARD_UNKNOWN = 0, /* Card unknow */
	MMC_CARD = 1, /* MMC Card */
	SD_CARD = 2, /* SD Card */
	SDHC_CARD = 3 /* SDHC Card */
} MCI_CARD_TYPE;

/* SD Block length is used in FAT ChaN */
#define SD_STATUS_BLOCK_LEN	6
#define SD_BLOCK_LENGTH     ( 1 << SD_STATUS_BLOCK_LEN)

/* Error code on the command response. */
#define INVALID_RESPONSE	0xFFFFFFFF

/* OCR (Operations Condition Register) Index*/
#define OCR_INDEX			0x00FF8000

/* SD/MMC bus status */
#define OPEN_DRAIN			1

#define CARD_STATUS_ACMD_ENABLE		1 << 5

/* SD/MMC time-out value */
#define DATA_TIMER_VALUE	0x10000

/* For the SD card I tested, the minimum block length is 512 */
/* For MMC, the restriction is loose, due to the variety of SD and MMC
 card support, ideally, the driver should read CSD register to find the
 speed and block length for the card, and set them accordingly. */
/* In this driver example, it will support both MMC and SD cards, it
 does read the information by send SEND_CSD to poll the card status,
 but, it doesn't configure them accordingly. this is not intended to
 support all the SD and MMC card. */

/* DATA_BLOCK_LEN table
 DATA_BLOCK_LEN			Actual Size( BLOCK_LENGTH )
 11						2048
 10						1024
 9						512
 8						256
 7						128
 6						64
 5						32
 4						16
 3						8
 2						4
 1						2
 */

/* To simplify the programming, please note that, BLOCK_LENGTH is a multiple
 of FIFO_SIZE */
#define DATA_BLOCK_LEN		9	/* Block size field in DATA_CTRL */
#define BLOCK_LENGTH		(1 << DATA_BLOCK_LEN)
#define DMA_SIZE		    (1 << DATA_BLOCK_LEN)

/* MCI FIFO Size */
#define FIFO_SIZE			16

/* MCI init function */
S32 MCI_Start();

/* MCI interrupt handler */
void MCI_ISR(void);

/* MCI set clock rate */
void MCI_SetClockRate(U32 clockrate);

/* Turn off power of MCI */
void MCI_PowerOff(void);

/* MCI send command */
S32 MCI_SendCommand(U32 cmd,
					U32 arg,
					U32 expect_response,
					U32 allow_timeout,
					U32* cmd_resp);

/* Card init function */
U32 MCI_CardInit(void);

/* Check CID (Card Identification Number) */
U32 MCI_CheckCID(void);

/* Set SD card address*/
/*  If it's a SD card, SET_RELATIVE_ADDR is to get the address
*   from the card and use this value in RCA (Relative Card Address),
*   if it's a MMC, set default RCA address. 0x00010000. */
U32 MCI_SetAddress(void);

/* Sends addressed card's CSD (Card Specific Data) */
U32 MCI_SendCSD(U32 *csd);

/* Toggle card between the standby and transfer states
 * In both case, the card is selected by its own RCA */
U32 MCI_SelectCard(void);

/* Set SD bus width ('00' = 1 bit, '10' = 4 bits bus) to be
 * used for data transfer */
U32 MCI_SetBusWidth(U32 width);

/* Set block length (in bytes) for all following block commands
 * (read, write, lock). Default block length is fixed 512 bytes*/
U32 MCI_SetBlockLength(U32 blockLength);

/* Get SD status (This function is used by ChaN FAT)*/
U32 MCI_SDStatus(void);

/* Sends addressed card's Status Register */
U32 MCI_SendStatus(void);

/* Forces the card to stop transmission */
U32 MCI_SendStop(void);

/* Write block command */
U32 MCI_WriteBlock(U32 blockAdrress,
					S8* dataBuffer,
					U32 numOfBlock);

/* Read block command */
U32 MCI_ReadBlock(U32 blockAdrress,
					S8* dataBuffer,
					U32 numOfBlock);

#ifdef __cplusplus
}
#endif

#endif /* LPC23XX_MCI_DRIVER_H_ */
