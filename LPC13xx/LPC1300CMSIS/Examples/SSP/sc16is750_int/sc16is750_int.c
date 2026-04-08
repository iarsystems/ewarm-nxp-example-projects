/**
 * @file	: sc16is750_int.c
 * @purpose	: An example of SSP using interrupt mode - SPI frame format to test
 * 				the SSP driver.
 * 				Using SSP at mode SSP master/8bit on LPC13xx to communicate with
 * 				SC16IS750/760 Demo Board
 * @version	: 1.0
 * @date	: 3. April. 2009
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
#include "lpc13xx_ssp.h"
#include "lpc13xx_gpio.h"
#include "lpc13xx_pinsel.h"
#include "lpc13xx_timer.h"

/************************** PRIVATE MACROS *************************/

#define USE_CS 0

/* Idle char */
#define IDLE_CHAR	0xFF

// Define macro used in command when using SSP with SC16IS740
#define SC16IS740_WR_CMD(x)		((uint8_t) (x << 3))
#define SC16IS740_RD_CMD(x)		((uint8_t) ((x << 3) | 0x80))

// Define register address of SC16IS740
#define SC16IS740_IODIR_REG		0x0A
#define SC16IS740_IOSTATE_REG	0x0B
#define SC16IS740_IOCON_REG		0x0E

uint8_t iocon_cfg[2] = {SC16IS740_WR_CMD(SC16IS740_IOCON_REG), 0x00};
uint8_t iodir_cfg[2] = {SC16IS740_WR_CMD(SC16IS740_IODIR_REG), 0xFF};
uint8_t iostate_on[2] = {SC16IS740_WR_CMD(SC16IS740_IOSTATE_REG), 0x00};
uint8_t iostate_off[2] = {SC16IS740_WR_CMD(SC16IS740_IOSTATE_REG), 0xFF};
uint8_t sspreadbuf[2];

/* Status Flag indicates current SSP transmission complete or not */
	FlagStatus complete;

/************************** PRIVATE TYPES *************************/
/************************** PRIVATE VARIABLES *************************/

// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;
volatile unsigned long SysTickCnt;      /* SysTick Counter */


/************************** PRIVATE FUNCTIONS *************************/
void SSP_IRQHandler(void);
void CS_Init(void);
void CS_Force(int32_t state);
void print_menu(void);
void SSPCallBack(void);


void SysTick_Handler (void) {           /* SysTick Interrupt Handler (1ms)    */
  SysTickCnt++;
}

void Delay (unsigned long tick) {       /* Delay Function                     */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}


/*********************************************************************//**
 * @brief 		SSP0 Interrupt used for reading and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void SSP_IRQHandler(void)
{
    // Call Std int handler
	SSP_StdIntHandler();
}


/*********************************************************************//**
 * @brief 		Initialize CS pin as GPIO function to drive /CS pin
 * 				due to definition of CS_PORT_NUM and CS_PORT_NUM
 * @param		None
 * @return		None
 ***********************************************************************/
void CS_Init(void)
{
	/* port0, bit 2 is set to GPIO output and high */
	GPIO_SetDir( PORT0, 2, 1 );
	GPIO_SetValue( PORT0, 2);
}


/*********************************************************************//**
 * @brief 		Drive CS output pin to low/high level to select slave device
 * 				via /CS pin state
 * @param[in]	state State of CS output pin that will be driven:
 * 				- 0: Drive CS pin to low level
 * 				- 1: Drive CS pin to high level
 * @return		None
 ***********************************************************************/
void CS_Force(int32_t state)
{
	if (state){
		GPIO_SetValue( PORT0, 2);
	}else{
		GPIO_ClearValue( PORT0, 2);
	}
}

/**
 * @brief 		User SSP callback function
 * @param[in]	None
 * @return 		None
 */
void SSPCallBack(void)
{
	// Set Complete Flag
	complete = SET;
}

/*********************************************************************//**
 * @brief	Main SSP program body
 **********************************************************************/
int c_entry(void)
{
	FlagStatus exitflag;
	SSP_DATA_SETUP_Type xferConfig;

	// Initialize System clock
	SystemInit();
	SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */

	/*
	 * Initialize SPI pin connect
	 */
	/* MISO on P0.8 */
	LPC_IOCON->PIO0_8 = PINSEL_PIO_FUNC(1) | PINSEL_PIO_MODE(2);
	/* MOSI on P0.9 */
	LPC_IOCON->PIO0_9 = PINSEL_PIO_FUNC(1) | PINSEL_PIO_MODE(2);
	/* SCK on P2.11 */
	LPC_IOCON->PIO2_11 = PINSEL_PIO_FUNC(1) | PINSEL_PIO_MODE(2);
	/* Selects SCK function for pin PIO2_11/SCK */
	LPC_IOCON->SCK_LOC = 01;

#if USE_CS
	/* SSEL on P0.2 */
	LPC_IOCON->PIO0_2 = PINSEL_PIO_FUNC(1) | PINSEL_PIO_MODE(2);
#else
	/* GPIO on P0.2 */
	//LPC_IOCON->PIO0_2 = PINSEL_PIO_FUNC(0) | PINSEL_PIO_MODE(2);
	LPC_IOCON->PIO0_2 = PINSEL_PIO_FUNC(0) | PINSEL_PIO_MODE(0);
#endif

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP0, &SSP_ConfigStruct);

	// Initialize /CS pin to GPIO function
	CS_Init();

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP0, ENABLE);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(SSP0_IRQn, ((0x01<<3)|0x01));
    /* Enable SSP0 interrupt */
    NVIC_EnableIRQ(SSP0_IRQn);

	/* First, send some command to reset SC16IS740 chip via SSP bus interface
	 * note driver /CS pin to low state before transferring by CS_Enable() function
	 */
    complete = RESET;
	CS_Force(0);
	xferConfig.tx_data = iocon_cfg;
	xferConfig.rx_data = sspreadbuf;
	xferConfig.length = sizeof (iocon_cfg);
	xferConfig.callback = SSPCallBack;
	SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_INTERRUPT);
	while (complete == RESET);
	CS_Force(1);

	complete = RESET;
	CS_Force(0);
	xferConfig.tx_data = iodir_cfg;
	xferConfig.rx_data = sspreadbuf;
	xferConfig.length = sizeof (iodir_cfg);
	xferConfig.callback = SSPCallBack;
	SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_INTERRUPT);
	while (complete == RESET);
	CS_Force(1);

	// Reset exit flag
	exitflag = RESET;

	while (1)
	{
		// LEDs are ON now...
		CS_Force(0);
		xferConfig.tx_data = iostate_on;
		xferConfig.rx_data = sspreadbuf;
		xferConfig.length = sizeof (iostate_on);
		SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_POLLING);
		CS_Force(1);
		Delay(2000);

		// LEDs are OFF now...
		CS_Force(0);
		xferConfig.tx_data = iostate_off;
		xferConfig.rx_data = sspreadbuf;
		xferConfig.length = sizeof (iostate_off);
		SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_POLLING);
		CS_Force(1);
		Delay(2000);
	}

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
