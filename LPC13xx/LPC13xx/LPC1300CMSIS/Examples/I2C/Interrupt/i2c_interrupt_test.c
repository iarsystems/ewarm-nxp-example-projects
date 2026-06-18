/**
 * @file	: i2c_interrupt_test.c
 * @purpose	: An example of I2C using interrupt mode to test the I2C driver.
 * 				Using I2C at mode I2C master/8bit on LPC13xx to communicate with
 * 				SC16IS750/760 Demo Board
 * @version	: 1.0
 * @date	: 15. April. 2009
 * @author	: HieuNguyen
 *----------------------------------------------------------------------------
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
#include "lpc13xx_i2c.h"
#include "lpc13xx_uart.h"
#include "lpc13xx_nvic.h"
#include "lpc13xx_gpio.h"

/************************** PRIVATE MACROS *************************/


/* Definition of internal register of SC16IS750/760 */
#define IODIR		0x0A
#define IOSTATE		0x0B
#define IOCON		0x0E
//#define SLVADDR		0x90
#define SLVADDR		(0x90>>1)
#define REGS_ADDR(n) (n<<3)


#define I2CDEV LPC_I2C


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"I2C demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART -  9600bps \n\r"
" Communicate with I2C function on SC16IS750/760 Demo Board\n\r"
" Use IO function on SC16IS740/750/760 chip to turn ON/OFF LEDs\n\r"
"Press '1' to turn ON LEDs, '2' to turn OFF LEDs \n\r"
"********************************************************************************\n\r";

uint8_t menu2[] = "Demo terminated! \n\r";
uint8_t menu3[] = "LEDs are ON now  \n\r";
uint8_t menu4[] = "LEDs are OFF now \n\r";

/* Define array data with match data to set internal register value of SC16IS740/750/760 */
uint8_t iocon_cfg[2] = {REGS_ADDR(IOCON), 0x00};
uint8_t iodir_cfg[2] = {REGS_ADDR(IODIR), 0xFF};
uint8_t iostate_cfg_0[2] = {0x58, 0x00};//{REGS_ADDR(IOSTATE), 0x00};
uint8_t iostate_cfg_1[2] = {0x58, 0xff};//{REGS_ADDR(IOSTATE), 0xFF};

FlagStatus complete;

/************************* PRIVATE FUNCTIONS *************************/
void I2C_IRQHandler(void);

void print_menu(void);
void Error_Loop(uint8_t ErrorCode);
void user_callback(void);


/*********************************************************************//**
 * @brief 		Main I2C interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void I2C_IRQHandler(void){
	// just call std int handler
	I2C_StdIntHandler();
}
/*********************************************************************//**
 * @brief	Print Welcome Screen Menu subroutine
 * @param	None
 * @return	None
 **********************************************************************/
void print_menu(void)
{
	UART_Send(menu1, sizeof(menu1), BLOCKING);
}

/*********************************************************************//**
 * @brief		A subroutine that will be called if there's any error
 * 				on I2C operation
 * @param[in]	ErrorCode Error Code Input
 * @return 		None
 **********************************************************************/
void Error_Loop(uint8_t ErrorCode)
{
	/*
	 * Insert your code here...
	 */
	while(1);
}

/*********************************************************************//**
 * @brief		A call back routine, will be called after I2C operation
 * 				complete
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void user_callback(void)
{
	complete = SET;
}

/*********************************************************************//**
 * @brief	Main I2C interrupt program body
 **********************************************************************/
int c_entry(void)
{
	uint8_t tmpchar[2] = {0, 0};
	FlagStatus exitflag;
	I2C_M_SETUP_Type transferCfg;
	uint8_t SC16IS_RegStat;
	uint32_t idx, len;

	// Initialize System clock
	SystemInit();

	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif

    /* Disable I2C interrupt */
    NVIC_DisableIRQ(I2C_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(I2C_IRQn, ((0x01<<3)|0x01));

	UART_Init();
	// print welcome screen
	print_menu();

	/* I2C block ------------------------------------------------------------------- */
	// Initialize I2C peripheral
	I2C_Init(I2CDEV, 100000);

	/* Enable I2C operation */
	I2C_Cmd(I2CDEV, ENABLE);

	// test
	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = NULL;
	transferCfg.tx_length = 0;
	transferCfg.rx_data = &SC16IS_RegStat;
	transferCfg.rx_length = 1;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);



	/* Configure SC16IS750 ---------------------------------------------------------- */
	/* First, send some command to reset SC16IS740 chip via I2C bus interface */
	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)iocon_cfg;
	transferCfg.tx_length = sizeof(iocon_cfg);
	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);

	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)iodir_cfg;
	transferCfg.tx_length = sizeof(iodir_cfg);
	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);

	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)iostate_cfg_0;
	transferCfg.tx_length = sizeof(iostate_cfg_0);
	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);


	/* Validate value of SC16IS750 register ------------------------------------------ */
	/* This section will dump out value of register that set through I2C bus */
	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = NULL; //(uint8_t *)iocon_cfg;
	transferCfg.tx_length = 0;//1;
	transferCfg.rx_data = &SC16IS_RegStat;
	transferCfg.rx_length = 1;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);

	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)iodir_cfg;
	transferCfg.tx_length = 1;
	transferCfg.rx_data = &SC16IS_RegStat;
	transferCfg.rx_length = 1;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);

	complete = RESET;
	transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)iostate_cfg_0;
	transferCfg.tx_length = 1;
	transferCfg.rx_data = &SC16IS_RegStat;
	transferCfg.rx_length = 1;
	transferCfg.retransmissions_max = 2;
	transferCfg.callback = user_callback;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
	while (complete == RESET);


	// Reset exit flag
	exitflag = RESET;

    /* Read some data from the buffer */
	while (exitflag == RESET){

       len = 0;
        while (len == 0)
        {
            len = UART_Receive(tmpchar, sizeof(tmpchar), NONE_BLOCKING);
        }

        /* Got some data */
        idx = 0;
        while (idx < len)
        	{
    	if (tmpchar[idx] == 27){
			/* ESC key, set exit flag */
			exitflag = SET;
			UART_Send(menu2, sizeof(menu2), BLOCKING);
		}
		else if (tmpchar[idx] == 'r'){
			print_menu();
		}
		else{
			if (tmpchar[idx] == '1'){
				// LEDs are ON now...
				UART_Send(menu3, sizeof(menu3), BLOCKING);
				transferCfg.sl_addr7bit = SLVADDR;
				transferCfg.tx_data = (uint8_t *)iostate_cfg_0;
				transferCfg.tx_length = sizeof(iostate_cfg_0);
				transferCfg.rx_data = NULL;
				transferCfg.rx_length = 0;
				transferCfg.retransmissions_max = 2;
				if (I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING) \
						== ERROR){
					Error_Loop(transferCfg.status);
				}
			}
			else if (tmpchar[idx] == '2')
			{
				// LEDs are OFF now...
				UART_Send(menu4, sizeof(menu4), BLOCKING);
				transferCfg.sl_addr7bit = SLVADDR;
				transferCfg.tx_data = (uint8_t *)iostate_cfg_1;
				transferCfg.tx_length = sizeof(iostate_cfg_1);
				transferCfg.rx_data = NULL;
				transferCfg.rx_length = 0;
				transferCfg.retransmissions_max = 2;
				if (I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING) \
						== ERROR){
					Error_Loop(transferCfg.status);
				}
			}

		}
 			            idx++;
    }
    }
    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy() == SET);

    // DeInitialize UART peripheral
    UART_DeInit();

    /* Loop forever */
    while(1);
    return 1;
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



