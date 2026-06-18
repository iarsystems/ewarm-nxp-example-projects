/***********************************************************************
 * $Id: eth_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: EA LPC3250 Ethernet poll example
 *
 * Description:	This simple example that illustrate the use of the
 *              Ethernet interface
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
 **********************************************************************/

#include <stdio.h>
#include <string.h>

#include "lpc_types.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc_irq_fiq.h"
#include "lpc32xx_chip.h"
#include "ea3250_board.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_adc_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_timer_driver.h"

#include "ethapi.h"
#include "uart.h"

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);


INT_32 uartdev;


/*
 * Local IP address
 */
UNS_8 localIP[4] = {192,168,5,241};

/*
 * The Ethernet address (MAC) assigned to the local host
 */
UNS_8 mac[6] = {0x00, 0x06, 0x98, 0x01, 0x16, 0x36};

extern UNS_8 inBuf[];

/* ADC variables*/
volatile UNS_32 adc_int, adc_data;



/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point from the startup code
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void c_entry(void)
{		
  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)
	(IRAM_BASE + (256 * 1024) - (16 * 1024)));

  /* Initialize interrupt system */
  int_initialize(0xFFFFFFFF);

  /* Install standard IRQ dispatcher at ARM IRQ vector */
  int_install_arm_vec_handler(IRQ_VEC, (PFV) lpc32xx_irq_handler);

	/* Setup miscellaneous board functions */
	ea3250_board_init();
	
  /* enable interupts */
  enable_irq();

  uart_output_init();
  
  if (TRUE == ethIf_init(mac)) {
    uart_output("MAC initialized\r\n");
  }
  else {
    uart_output("Failed to initialized MAC controller\r\n");
  }

  while(1)
  {
    UNS_16 len = 0;

    len = ethIf_poll(inBuf, INBUF_LEN);

    if(len > 0)
    {
      ethInput(inBuf, len);
    }
  }


}
