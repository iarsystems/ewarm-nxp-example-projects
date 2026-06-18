/**********************************************************************
* $Id: I2c_MasterSlave.c 4271 2015-08-17 14:00:36Z oscarmo $		I2c_MasterSlave.c	2012-08-31
*//**
* @file		I2c_MasterSlave.c
* @brief	This example describes how to configure I2C as master device
* @version	1.0
* @date		31. Aug. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc18xx_i2c.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg.h"
#include "debug_frmwrk.h"



/* Example group ----------------------------------------------------------- */
/** @defgroup I2c_Master	I2c_MasterSlave
 * @ingroup I2C_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define I2CDEV_UDA1380_ADDR		(0x34>>1)
#define UDA1380_CLOCK_CONFIGURE	0x00

/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR	(0x90>>1)
/** Max buffer length */
#define BUFFER_SIZE			0xFF

/************************** PRIVATE VARIABLES *************************/
char menu[] =
		"********************************************************************************\n\r"
		" Hello NXP Semiconductors \n\r"
		" I2C Master - Slave demo \n\r"
		"\t - MCU: LPC18xx \n\r"
		"\t - Core: ARM CORTEX-M3 \n\r"
		"\t - Communicate via: UART0 - 115200 bps \n\r"
		"\n\rPress 1-5 to select I2C running mode or 'ESC' to terminate the demo:\n\r\n\r"
		"\t 1 - I2C Polling   Master\n\r"
		"\t 2 - I2C Interrupt Master\n\r"
		"\t 3 - I2C Polling   Slave \n\r"
		"\t 4 - I2C Interrupt Slave \n\r"
		"\t 5 - Communicate with UDA1380 \n\r"
		"********************************************************************************\n\r";
char menu1[] = "\n\r \t - Press x to exit this mode!\n\r";
char menu2[] = "\n\rI2C demo terminated!";
char menu3[] = "\t - Press c to continue...\n\r";

Bool complete;
Bool isMasterMode = TRUE;
/** These global variables below used in interrupt mode - Slave device ----------------*/
uint8_t Master_Buf[BUFFER_SIZE];
uint8_t master_test[3];

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
void Buffer_Init(uint8_t* buffer, uint8_t type);

/*-------------------------PRIVATE FUNCTIONS-----------------------------*/
/*********************************************************************//**
 * @brief		I2C Interrupt Handler
 * @param[in]	None
 *
 * @return 		None
 **********************************************************************/
void I2C1_IRQHandler(void)
{
	if (isMasterMode)// Master Mode
	{
		I2C_Interrupt_MasterHandler(LPC_I2C1);
		if (I2C_MasterTransferComplete(LPC_I2C1)){
			complete = TRUE;
		}
	}
	else// Slave Mode
	{
		I2C_Interrupt_SlaveHandler(LPC_I2C1);
		if (I2C_SlaveTransferComplete(LPC_I2C1))
		{
			complete = TRUE;
		}
	}
}

/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG_(menu);
}

/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	buffer	buffer to initialize
 * @param[in]	type:
 * 				- 0: Initialize buffer with 0
 * 				- 1: Initialize buffer with increment value from 0
 * @return 		None
 **********************************************************************/
void Buffer_Init(uint8_t* buffer, uint8_t type)
{
	uint8_t i;

	if (type)
	{
		for (i = 0; i < BUFFER_SIZE; i++) {
			buffer[i] = i;
		}
	}
	else
	{
		for (i = 0; i < BUFFER_SIZE; i++) {
			buffer[i] = 0;
		}
	}
}

/*********************************************************************//**
 * @brief		I2C master application in polling mode
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_I2C_Polling_Master(void)
{
	I2C_M_SETUP_Type transferMCfg;
	uint8_t buffer = 0xFF, regAddr = 0xBA;
	uint8_t Master_Buf[BUFFER_SIZE];
	uint8_t *pdat;
	uint32_t i = 0;

	/* Enable Master I2C operation */
	I2C_Cmd(LPC_I2C1, I2C_MASTER_MODE, ENABLE);


	while (1) {
		_DBG_("\n\r\n\rI2C Polling Master is running...\n\r");
		_DBG_(menu1);
		_DBG_(menu3);

		while (1)
		{
			buffer = _DG;
			if (buffer == 'x' || buffer == 'X')
			{
				/* Disable Master I2C operation */
				I2C_Cmd(LPC_I2C1, I2C_MASTER_MODE, DISABLE);
				return;
			}
			else if (buffer == 'c' || buffer == 'C')
			{
				break;
			}
		}
		/* Transmit -------------------------------------------------------- */
		_DBG_("\n\rPress '1' to transmit");
		while (1)
		{
			buffer = _DG;
			if (buffer == '1')
				break;
		}
		_DBG_("\n\rTransmitting...");

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 1);

		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.tx_data = Master_Buf;
		transferMCfg.tx_length = sizeof(Master_Buf);
		I2C_MasterTransmitData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING);


		/* Receive -------------------------------------------------------- */
		_DBG_("\n\rPress '2' to receive");
		while (1)
		{
			buffer = _DG;
			if (buffer == '2')
				break;
		}
		_DBG_("\n\rReceiving...");

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 0);

		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.rx_data = Master_Buf;
		transferMCfg.rx_length = sizeof(Master_Buf);
		I2C_MasterReceiveData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING);

		pdat = Master_Buf;
		// Verify
		for (i = 0; i < sizeof(Master_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Master_Buf)){
			_DBG_("\n\rVerify successfully!");
		}

		/* Transmit and receive -------------------------------------------------------- */
		_DBG_("\n\rPress '3' to Transmit, then repeat start and receive...");
		while (1)
		{
			buffer = _DG;
			if (buffer == '3')
				break;
		}

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 0);

		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.tx_data = &regAddr;
		transferMCfg.tx_length = 1;
		transferMCfg.rx_data = Master_Buf;
		transferMCfg.rx_length = sizeof(Master_Buf);
		transferMCfg.retransmissions_max = 3;
		transferMCfg.tx_count = 0;
		transferMCfg.rx_count = 0;
		transferMCfg.retransmissions_count = 0;
		I2C_MasterTransferData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING);

		pdat = Master_Buf;
		// Verify
		for (i = 0; i < sizeof(Master_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Master_Buf)){
			_DBG_("\n\rVerify successfully!");
		}
	}
}

/*********************************************************************//**
 * @brief		I2C master application using interrupt
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_I2C_Interrupt_Master(void)
{
	I2C_M_SETUP_Type transferMCfg;
	uint8_t buffer = 0xFF, regAddr = 0xBA;
	uint8_t Master_Buf[BUFFER_SIZE];
	uint8_t *pdat;
	uint32_t i = 0;

	/* Enable Master I2C operation */
	I2C_Cmd(LPC_I2C1, I2C_MASTER_MODE, ENABLE);


	while (1) {
		_DBG_("\n\r\n\rI2C Interrupt Master is running...\n\r");
		_DBG_(menu1);
		_DBG_(menu3);

		while (1)
		{
			buffer = _DG;
			if (buffer == 'x' || buffer == 'X')
			{
				/* Disable Master I2C operation */
				I2C_Cmd(LPC_I2C1, I2C_MASTER_MODE, DISABLE);
				return;
			}
			else if (buffer == 'c' || buffer == 'C')
			{
				break;
			}
		}
		/* Transmit -------------------------------------------------------- */
		_DBG_("\n\rPress '1' to transmit");
		while (1)
		{
			buffer = _DG;
			if (buffer == '1')
				break;
		}
		_DBG_("\n\rTransmitting...");

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 1);

		complete = FALSE;
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.tx_data = Master_Buf;
		transferMCfg.tx_length = sizeof(Master_Buf);
		I2C_MasterTransmitData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_INTERRUPT);
		while(!complete);

		/* Receive -------------------------------------------------------- */
		_DBG_("\n\rPress '2' to receive");
		while (1)
		{
			buffer = _DG;
			if (buffer == '2')
				break;
		}
		_DBG_("\n\rReceiving...");

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 0);

		complete = FALSE;
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.rx_data = Master_Buf;
		transferMCfg.rx_length = sizeof(Master_Buf);
		I2C_MasterReceiveData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_INTERRUPT);
		 while(!complete);

		pdat = Master_Buf;
		// Verify
		for (i = 0; i < sizeof(Master_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Master_Buf)){
			_DBG_("\n\rVerify successfully!");
		}

		/* Transmit and receive -------------------------------------------------------- */
		_DBG_("\n\rPress '3' to Transmit, then repeat start and receive...");
		while (1)
		{
			buffer = _DG;
			if (buffer == '3')
				break;
		}

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 0);

		complete = FALSE;
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
		transferMCfg.tx_data = &regAddr;
		transferMCfg.tx_length = 1;
		transferMCfg.rx_data = Master_Buf;
		transferMCfg.rx_length = sizeof(Master_Buf);
		transferMCfg.retransmissions_max = 3;
		transferMCfg.tx_count = 0;
		transferMCfg.rx_count = 0;
		transferMCfg.retransmissions_count = 0;
		I2C_MasterTransferData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_INTERRUPT);
		 while(!complete);

		pdat = Master_Buf;
		// Verify
		for (i = 0; i < sizeof(Master_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Master_Buf)){
			_DBG_("\n\rVerify successfully!");
		}
	}
}

/*********************************************************************//**
 * @brief		I2C slave application in polling mode
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_I2C_Polling_Slave (void)
{
	I2C_S_SETUP_Type transferSCfg;
	uint8_t buffer = 0xFF, regAddr = 0xBA;
	uint8_t Slave_Buf[BUFFER_SIZE], S_buff = 0;
	uint8_t *pdat;
	uint32_t i = 0;

	/* Enable Slave I2C operation */
	I2C_Cmd(LPC_I2C1, I2C_SLAVE_MODE, ENABLE);

	while (1) {
		_DBG_("\n\r\n\rI2C Polling Slave is running...\n\r");
		_DBG_(menu1);
		_DBG_(menu3);

		while (1)
		{
			buffer = _DG;
			if (buffer == 'x' || buffer == 'X')
			{
				/* Disable Slave I2C operation */
				I2C_Cmd(LPC_I2C1, I2C_SLAVE_MODE, DISABLE);
				return;
			}
			else if (buffer == 'c' || buffer == 'C')
			{
				break;
			}
		}
		/* Receive -------------------------------------------------------- */
		_DBG_("\n\rPress '1' to Start");

		while (1)
		{
			buffer = _DG;
			if (buffer == '1')
				break;
		}
		_DBG_("\n\rReceiving...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 0);

		transferSCfg.rx_data = Slave_Buf;
		transferSCfg.rx_length = sizeof(Slave_Buf);
		I2C_SlaveReceiveData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_POLLING);

		pdat = Slave_Buf;
		// Verify
		for (i = 0; i < sizeof(Slave_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Slave_Buf)){
			_DBG_("\n\rVerify successfully!");
		}

		/* Transmit -------------------------------------------------------- */
		_DBG_("\n\rTransmitting...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 1);

		transferSCfg.tx_data = Slave_Buf;
		transferSCfg.tx_length = sizeof(Slave_Buf);
		I2C_SlaveTransmitData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_POLLING);

		/* Receive and transmit -------------------------------------------------------- */
		_DBG_("\n\rStart Receive, wait for repeat start and transmit...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 1);

		transferSCfg.rx_data = &S_buff;
		transferSCfg.rx_length = 1;
		I2C_SlaveReceiveData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_POLLING);
		if (S_buff == regAddr)
		{
			transferSCfg.tx_data = Slave_Buf;
			transferSCfg.tx_length = sizeof(Slave_Buf);
			I2C_SlaveTransmitData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_POLLING);
		}
	}
}

/*********************************************************************//**
 * @brief		I2C slave application using interrupt
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_I2C_Interrupt_Slave (void)
{
	I2C_S_SETUP_Type transferSCfg;
	uint8_t buffer = 0xFF, regAddr = 0xBA;
	uint8_t Slave_Buf[BUFFER_SIZE], S_buff = 0;
	uint8_t *pdat;
	uint32_t i = 0;

	/* Enable Slave I2C operation */
	I2C_Cmd(LPC_I2C1, I2C_SLAVE_MODE, ENABLE);

	while (1) {
		_DBG_("\n\r\n\rI2C Interrupt Slave is running...\n\r");
		_DBG_(menu1);
		_DBG_(menu3);

		while (1)
		{
			buffer = _DG;
			if (buffer == 'x' || buffer == 'X')
			{
				/* Disable Slave I2C operation */
				I2C_Cmd(LPC_I2C1, I2C_SLAVE_MODE, DISABLE);
				return;
			}
			else if (buffer == 'c' || buffer == 'C')
			{
				break;
			}
		}
		/* Receive -------------------------------------------------------- */
		_DBG_("\n\rPress '1' to Start");

		while (1)
		{
			buffer = _DG;
			if (buffer == '1')
				break;
		}
		_DBG_("\n\rReceiving...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 0);

		complete = FALSE;
		transferSCfg.rx_data = Slave_Buf;
		transferSCfg.rx_length = sizeof(Slave_Buf);
		I2C_SlaveReceiveData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_INTERRUPT);
		 while(!complete);

		pdat = Slave_Buf;
		// Verify
		for (i = 0; i < sizeof(Slave_Buf); i++){
			if (*pdat++ != i){
				_DBG_("\n\rVerify error!");
				break;
			}
		}
		if (i == sizeof(Slave_Buf)){
			_DBG_("\n\rVerify successfully!");
		}

		/* Transmit -------------------------------------------------------- */
		_DBG_("\n\rTransmitting...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 1);

		complete = FALSE;
		transferSCfg.tx_data = Slave_Buf;
		transferSCfg.tx_length = sizeof(Slave_Buf);
		I2C_SlaveTransmitData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_INTERRUPT);
		 while(!complete);

		/* Receive and transmit -------------------------------------------------------- */
		_DBG_("\n\rStart Receive, wait for repeat start and transmit...");

		/* Initialize buffer */
		Buffer_Init(Slave_Buf, 1);

		complete = FALSE;
		transferSCfg.rx_data = &S_buff;
		transferSCfg.rx_length = 1;
		I2C_SlaveReceiveData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_INTERRUPT);
		 while(!complete);

		if (S_buff == regAddr)
		{
			complete = FALSE;
			transferSCfg.tx_data = Slave_Buf;
			transferSCfg.tx_length = sizeof(Slave_Buf);
			I2C_SlaveTransmitData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_INTERRUPT);
			 while(!complete);
		}
	}
}

/*********************************************************************//**
 * @brief		Communicate with UDA1380
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_I2C_UDA1380(void)
{
	FlagStatus exitflag = RESET;
	I2C_M_SETUP_Type transferMCfg;
	uint8_t buffer = 0;

	/* Enable Master I2C operation */
	I2C_Cmd(LPC_I2C0, I2C_MASTER_MODE, ENABLE);

	/* Reset UDA1380 on board Hitex */
	scu_pinmux(8,2,MD_PUP, FUNC0);
	GPIO_SetDir(4, 1<<2, 1);
	GPIO_ClearValue(4, 1<<2);

	_DBG_(menu1);

	while (exitflag == RESET) {

		while (1)
		{
			buffer = _DG;
			if (buffer == 'x' || buffer == 'X')
			{
				/* Disable Slave I2C operation */
				I2C_Cmd(LPC_I2C1, I2C_SLAVE_MODE, DISABLE);
				return;
			}
			else if (buffer == 'c' || buffer == 'C')
			{
				break;
			}
		}
		/* Transmit -------------------------------------------------------- */
		_DBG_("Press '1' to transmit 2 bytes to UDA1380's 0x00_register...");
		while (_DG != '1');

		/* Initialize buffer */
		master_test[0] = UDA1380_CLOCK_CONFIGURE;
		master_test[1] = 0x05; /* EN_INT & EN_DEC */
		master_test[2] = 0x02; /* PLL1            */

		transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
		transferMCfg.tx_data = master_test;
		transferMCfg.tx_length = 3;
		I2C_MasterTransmitData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

		/* Transmit and receive -------------------------------------------------------- */
		_DBG_("Press '2' to read UDA1380's 0x00_register...");
		while (_DG != '2');

		/* Initialize buffer */
		Buffer_Init(Master_Buf, 0);
		master_test[0] = UDA1380_CLOCK_CONFIGURE;
		master_test[1] = 0x05; /* EN_INT & EN_DEC */
		master_test[2] = 0x02; /* PLL1            */

		transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
		transferMCfg.tx_data = master_test;
		transferMCfg.tx_length = 1;
		transferMCfg.rx_data = Master_Buf;
		transferMCfg.rx_length = 2;
		transferMCfg.retransmissions_max = 3;
		transferMCfg.tx_count = 0;
		transferMCfg.rx_count = 0;
		transferMCfg.retransmissions_count = 0;
		I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

		// Verify
		if(
			(Master_Buf[0]!=master_test[1])
			||(Master_Buf[1]!=master_test[2])
		)
			_DBG_("Verify error!");
		else
			_DBG_("Verify successfully!");
	}

	/* Disable Master I2C operation */
	I2C_Cmd(LPC_I2C0, I2C_MASTER_MODE, DISABLE);
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	I2C_OWNSLAVEADDR_CFG_Type OwnSlavAdr;
	FlagStatus exitflag;
	uint8_t buffer = 0xFF;

	SystemInit();
	CGU_Init();

	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	print_menu();

	/* I2C block ------------------------------------------------------------------- */
	// Initialize Slave I2C peripheral
	I2C_Init(LPC_I2C0, 100000);
	I2C_Init(LPC_I2C1, 100000);

	/* Set  Own slave address for I2C1 device */
	OwnSlavAdr.GeneralCallState = DISABLE;
	OwnSlavAdr.SlaveAddrChannel= 0;
	OwnSlavAdr.SlaveAddrMaskValue = 0xFF;
	OwnSlavAdr.SlaveAddr_7bit = I2CDEV_S_ADDR;
	I2C_SetOwnSlaveAddr(LPC_I2C1, &OwnSlavAdr);

	exitflag = RESET;
	/* Read some data from the buffer */
	while (exitflag == RESET) {
		buffer = _DG;

		switch (buffer)
		{
			case '1': //Master Polling Mode
				App_I2C_Polling_Master();
				print_menu();
				break;
			case '2': //Master Interrupt Mode
				isMasterMode = TRUE;
				App_I2C_Interrupt_Master();
				print_menu();
				break;
			case '3': //Slave Polling Mode
				App_I2C_Polling_Slave();
				print_menu();
				break;
			case '4': //Slave Interrupt Mode
				isMasterMode = FALSE;
				App_I2C_Interrupt_Slave();
				print_menu();
				break;
			case '5': //Communicate with UDA1380
				App_I2C_UDA1380();
				print_menu();
				break;
			case 27:
				/* ESC key, set exit flag */
				_DBG_(menu2);
				exitflag = SET;
				break;
			default:
				break;
		}
	}

	I2C_DeInit(LPC_I2C0);
	I2C_DeInit(LPC_I2C1);

	return 0;
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */
