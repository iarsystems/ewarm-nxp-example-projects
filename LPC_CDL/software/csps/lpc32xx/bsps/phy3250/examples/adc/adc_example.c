/***********************************************************************
 * $Id: adc_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: NXP PHY3250 simple ADC example
 *
 * Description:	This simple ADC example uses interrupts to measure the 
 *              level on ADIN2. The two clocking schemes are used
 *              --> RTC Clock and PERIPH_CLK 
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

#include "lpc_types.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc_irq_fiq.h"
#include "lpc32xx_chip.h"
#include "phy3250_board.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_adc_driver.h"

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

/* ADC device handle */
STATIC INT_32 adcdev;

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
	adc_int = 1;	
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
	
    volatile UNS_32 i; 
	
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
	phy3250_board_init();

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

	/* select channel for converion. 
	   The Phytec board has a POT on ADIN2 pin */
	adc_ioctl(adcdev, ADC_CH_SELECT, ADCSEL_CH_2);   
	
	/* start A/D convert */
	adc_ioctl(adcdev, START_CONVERSION, 0);   

	/* wait for conversion to complete */
	while (!adc_int);

	/* reset the interrupt flag */
	adc_int = 0;

/* ADC conversion, PERIPH_CLK mode. */

/* The Maximum conversion speed is 400kHz for the ADC 
   13MHz/400kHz = ~33 */

	/* disable 32KHz clock to ADC block */
    clkpwr_clk_en_dis(CLKPWR_ADC_CLK,0);

    /* Get the clock frequency for the peripheral clock*/
    i = clkpwr_get_base_clock_rate(CLKPWR_PERIPH_CLK);
	
	/* compute the divider needed to ensure we are at or
       below 400KHz ADC Sampling rate*/
	i = (i / 400000)+1;

	/* Set the ADC to run off the PERIPH_CLK and
       400KHz sampling rate */
	clkpwr_setup_adc_ts(CLKPWR_ADCCTRL1_PCLK_SEL, i) ;

	/* select channel for converion */
	adc_ioctl(adcdev, ADC_CH_SELECT, ADCSEL_CH_2);   
	
	/* start A/D convert */
	adc_ioctl(adcdev, START_CONVERSION, 0);   

	/* wait for conversion to complete */
	while (!adc_int);

	/* reset the interrupt flag */
	adc_int = 0;
	
    /* Disable ADC interrupt in the interrupt controller */
    int_disable(IRQ_TS_IRQ);

	/* close the ADC */
	adc_close(adcdev);

	/* Loop here forever */
    while (1);
}
