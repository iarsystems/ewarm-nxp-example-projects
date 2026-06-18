/**
 * @file	: sc16is750_polling.c
 * @purpose	: An example of SSP using polling mode to test the SSP driver.
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

TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;

/************************** PRIVATE MACROS *************************/

#define USE_CS 0

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




/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;
volatile unsigned long SysTickCnt;      /* SysTick Counter */

/************************** PRIVATE FUNCTIONS *************************/
void CS_Init(void);
void CS_Force(int32_t state);
void print_menu(void);


void SysTick_Handler (void) {           /* SysTick Interrupt Handler (1ms)    */
  SysTickCnt++;
}

void Delay (unsigned long tick) {       /* Delay Function                     */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
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
	GPIO_SetDir( PORT3, 0, 1 );
	GPIO_SetValue( PORT0, 2);
	GPIO_SetValue( PORT3, 0);
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
		GPIO_SetValue( PORT3, 0);
	}else{
		GPIO_ClearValue( PORT0, 2);
		GPIO_ClearValue( PORT3, 0);
	}
}


/*********************************************************************//**
 * @brief	Main SSP program body
 **********************************************************************/
int c_entry(void)
{
	//__IO FlagStatus exitflag;
	SSP_DATA_SETUP_Type xferConfig;

	// Initialize System clock
	SystemInit();
	SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */

// Initialize timer 0, prescale count time of 1ms
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will not reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = FALSE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = TRUE;
	//do no thing for external output
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	// Set Match value, count value is time (timer * 1000uS =timer mS )
	TIM_MatchConfigStruct.MatchValue   = (SystemCoreClock/100 - 1);


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
	LPC_IOCON-> SCK_LOC = 01;

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

	TIM_Init(LPC_TMR16B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TMR16B0,&TIM_MatchConfigStruct);
	NVIC_EnableIRQ(TIMER_16_0_IRQn);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP0, ENABLE);

	/* First, send some command to reset SC16IS740 chip via SSP bus interface
	 * note driver /CS pin to low state before transferring by CS_Enable() function
	 */
	CS_Force(0);
	xferConfig.tx_data = iocon_cfg;
	xferConfig.rx_data = sspreadbuf;
	xferConfig.length = sizeof (iocon_cfg);
	SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_POLLING);
	CS_Force(1);

	CS_Force(0);
	xferConfig.tx_data = iodir_cfg;
	xferConfig.rx_data = sspreadbuf;
	xferConfig.length = sizeof (iodir_cfg);
	SSP_ReadWrite(LPC_SSP0, &xferConfig, SSP_TRANSFER_POLLING);
	CS_Force(1);


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
