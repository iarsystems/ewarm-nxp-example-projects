/***********************************************************************
 * i2c_test.c 2008-07-28 --- HieuNguyen
 *
 * Project: I2C driver example
 *
 * Description:
 * - This example code sets LPC2368 as a I2C master.
 * - Use SC16IS750/760 Demo Board with SC16IS740/750/760 
 * Single UART with I2C-bus/SPI interface, 64 bytes of transmit
 * and receive FIFOs, IrDA SIR built-in support
 * - First, send some data to config SC16IS740/750/760 for using IO function
 * (ON/OFF LED driver).
 * - In the main-loop, read pressed keycode to turn ON or OFF all LED 
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
 **********************************************************************/
#include "lpc23xx_i2c_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_scb_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_tc_driver.h"
#include "lpc23xx_uart_driver.h"

/* Definition of internal regs of SC16IS750/760 */
#define IODIR		0x0A
#define IOSTATE		0x0B
#define IOCON		0x0E
#define SLVADDR		0x90
#define REGS_ADDR(n) (n << 3)

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"I2C test demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use I2C function in MASTER mode \n\r"
" To communicate with SC16IS750/760 Demo Board (use SC16IS740/750/760) \n\r"
" Use IO function to turn ON/OFF LEDs \n\r"
"********************************************************************************\n\r";
STATIC S32 i2cdev, uartdev;
/* Define array data with match data to set internal regs value of SC16IS740/750/760 */
U8 iocon_cfg[2] = {REGS_ADDR(IOCON), 0x00};
U8 iodir_cfg[2] = {REGS_ADDR(IODIR), 0xFF};
U8 iostate_cfg_0[2] = {REGS_ADDR(IOSTATE), 0x00};
U8 iostate_cfg_1[2] = {REGS_ADDR(IOSTATE), 0xFF};
U8 state_reg[1] = {REGS_ADDR(IOSTATE)};
U32 readval[1];

/* Complete flag of successful transmission in ASYNC mode */
S32 volatile complete = 0;

/* Init UART0 */
S32 UART0_Init (void)
{
	// Set pins for UART0 function
    PINSEL_Set(PORT_0, 2, FUNC_1);
    PINSEL_Set(PORT_0, 3, FUNC_1);
    
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uartdev = UART_Init (UART_PORT_0, 115200, 8, 0, 1)) == 0x00)
    {
        return _ERROR;
    }
    
    return _NO_ERROR;
}

/* Read pressed key */
CHAR UART0Read(void)
{
	U32 len = 0;
	CHAR readkey[1];
	
     while (len == 0)
     {
         len = UART_ReadPoll (uartdev, readkey, sizeof(readkey));
     }
     
     return readkey[0];
}

/* User call back function */
void usr_cbs (void)
{
	complete = 1;
	//UART_PutHex (uartdev, I2C_GetStatus (i2cdev)); UART_Puts_ (uartdev, "");
}

int c_entry (void)
{
	U8 ReadKey;
	
	mcb2300_init();
	
	/* Init UART0 */
	if (UART0_Init() == _ERROR)
	{
		return 0;
	}
	
	// Set pin function according to I2C1 controller
	PINSEL_Set (PORT_0, 19, FUNC_3);
	PINSEL_Set (PORT_0, 20, FUNC_3);
	
	/* Set internal pull-up resistor since these pin 
	 * are not open-drain in I2C function */
	PINSEL_SetResistorMode (PORT_0, 19, PINMODE_PULLUP);
	PINSEL_SetResistorMode (PORT_0, 20, PINMODE_PULLUP);
	
	// Init I2C1 device, master mode, 100KHz
	if ((i2cdev = I2C_Init (I2C_PORT_1, I2C_MASTER_MODE, 0x00, 50000)) == 0x00)
	{
		return 0;
	}
	
	// Install IRQ for I2C device used in ASYNC mode
	if (VIC_InstallIRQ (I2C1_INT,(void *) I2C1_ISR, 0x01) == _ERROR)
	{
		return 0;
	}
	    
	// Config Call back function used in ASYNC mode 
	I2C_SetCBS (i2cdev, (void *)usr_cbs);
	
	/* Welcome screen */
	UART_WritePoll (uartdev, menu1, sizeof(menu1));
	UART_Puts_ (uartdev, "Press '1' to start");
	
	/* Wait for "1" key pressed to start */
	while (UART0Read() != '1');
	
	/* Send some data */
	complete = 0;
	/* For IO configuration on SC16IS740/750/760 */
	I2C_WriteAsync(i2cdev, SLVADDR, iocon_cfg, sizeof (iocon_cfg));
	
	/* Wait for complete == 1 from user callback function */
	while (complete == 0);
	
	complete = 0;
	/* For IO direction configuration on SC16IS740/750/760 */
	I2C_WriteAsync (i2cdev, SLVADDR, iodir_cfg, sizeof (iodir_cfg));
	
	/* Wait for complete == 1 from user callback function */
	while (complete == 0);
	
	complete = 0;
	/* For IO state configuration on SC16IS740/750/760 */
	I2C_WriteAsync (i2cdev, SLVADDR, iostate_cfg_0, sizeof (iostate_cfg_0));
	
	/* Wait for complete == 1 from user callback function */
	while (complete == 0);
	
	UART_Puts_ (uartdev, "Press 1 to turn ON, press 2 to turn OFF");
	
	/* Main loop */
	while (1)
	{
		// Enter value 
		ReadKey = UART0Read();
		
		if (ReadKey == '1')
		{
			I2C_WritePoll (i2cdev, SLVADDR, &iostate_cfg_0, sizeof (iostate_cfg_0));
			UART_Puts_ (uartdev, "LEDs are ON now");
		}
		else if (ReadKey == '2')
		{
			I2C_WritePoll (i2cdev, SLVADDR, &iostate_cfg_1, sizeof (iostate_cfg_1));
			UART_Puts_ (uartdev, "LEDs are OFF now");
		}
		else if (ReadKey == 27)
		{
			UART_Puts_ (uartdev, "Demo program terminated!");
			break;
		}
		else
		{
			UART_Puts_ (uartdev, "Error input!");
		}		
	}
	
	// close I2C device
	I2C_Close (i2cdev);
	
	// Loop forever
	while (1);
	
	return (0);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
