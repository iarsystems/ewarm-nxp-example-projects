/***********************************************************************
 * $Id: adc_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: EA LPC3250 simple ADC example
 *
 * Description:	This simple ADC example uses interrupts to measure 
 *              ADIN0, ADI01 (accelerometer) and ADIN2 (trimpot)
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

#include "uart.h"

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

/* ADC device handle */
static INT_32 adcdev;

/* UART buffer */
static char buff[512];

/* ADC variables*/
volatile UNS_32 adc_int, adc_data;



/***********************************************************************
 *
 * Function: adc_user_interrupt
 *
 * Purpose: ADC user interrupt handler
 *
 * Processing:
 *     On an interrupt read 
 *     the data from the ADC
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

void adc_user_interrupt(void)
{
	/* A read of the ADC data register will clear the ADC interrupt */

	/* Read the ADC result */
	adc_data = (ADC->adc_dat & TSC_ADCDAT_VALUE_MASK) ;
    
	/* Set the Flag to tell main application we got the interrupt*/
	adc_int++ /* = 1*/;	
}

static UNS_32 read_adc0(void)
{
  adc_int = 0;

	/* select channel for converion */
	adc_ioctl(adcdev, ADC_CH_SELECT, ADCSEL_CH_0);   
	
	/* start A/D convert */
	adc_ioctl(adcdev, START_CONVERSION, 0);   

	/* wait for conversion to complete */
	while (!adc_int);

  return adc_data;
}

static UNS_32 read_adc1(void)
{
  adc_int = 0;

	/* select channel for converion */
	adc_ioctl(adcdev, ADC_CH_SELECT, ADCSEL_CH_1);   
	
	/* start A/D convert */
	adc_ioctl(adcdev, START_CONVERSION, 0);   

	/* wait for conversion to complete */
	while (!adc_int);

  return adc_data;
}

static UNS_32 read_adc2(void)
{
  adc_int = 0;

	/* select channel for converion */
	adc_ioctl(adcdev, ADC_CH_SELECT, ADCSEL_CH_2);   
	
	/* start A/D convert */
	adc_ioctl(adcdev, START_CONVERSION, 0);   

	/* wait for conversion to complete */
	while (!adc_int);

  return adc_data;
}

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

  /* Install adc handler as a IRQ interrupts */
  int_install_irq_handler(IRQ_TS_IRQ, (PFV) adc_user_interrupt);

  /* Enable ADC interrupt in the interrupt controller */
  int_enable(IRQ_TS_IRQ);

/* ADC conversion, RTC CLK mode. */

  /* Enable 32KHz clock to ADC block */
  clkpwr_clk_en_dis(CLKPWR_ADC_CLK,1);

	/* Open the ADC device */
	adcdev = adc_open(ADC , 0);     

  /* Reset flag */
  adc_int = 0;
	
  /* enable interupts */
  enable_irq();

  uart_output_init();


  while(1) 
  {

    /* a0 & a1 -> accelerometer */
    /* a2 -> trimpot */

    UNS_32 a0 = read_adc0();
    UNS_32 a1 = read_adc1();
    UNS_32 a2 = read_adc2();

    sprintf(buff, "a0=%d, a1=%d, a2=%d\r\n", a0,a1,a2);
    uart_output((UNS_8*)buff);

    timer_wait_ms(TIMER_CNTR0, 500);

  }

	/* close the ADC */
//	adc_close(adcdev);

	/* Loop here forever */
//  while (1);
}
