/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */

#include "common.h"
#include "sysinit.h"
#include "uart.h"
#include "mcg.h"



/********************************************************************/

/* Actual system clock frequency */
int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int core_clk_mhz;
int periph_clk_khz;
int flash_clk_khz;


/********************************************************************/
void sysinit (void)
{
        /* Enable all of the port clocks. These have to be enabled to configure
         * pin muxing options, so most code will need all of these on anyway.
        */ 
        SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );


       /* Ramp up the system clock */
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */  
        if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
          PMC_REGSC |= PMC_REGSC_ACKISO_MASK;	
       
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)    //Core/system       120 Mhz
                        | SIM_CLKDIV1_OUTDIV2(1)    //Busclk            60 Mhz         
                        | SIM_CLKDIV1_OUTDIV3(2)    //FlexBus           40 Mhz
                        | SIM_CLKDIV1_OUTDIV4(4) ); //Flash             24 Mhz
       
       /* Initialize PLL */ 
       /* PLL will be the source for MCG CLKOUT so the core, system, and flash clocks are derived from it */ 
       mcg_clk_hz = pll_init(CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             MCGOUT);       /* Use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if (mcg_clk_hz < 0x100)
         while(1);

	/*
         * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
        mcg_clk_khz = mcg_clk_hz / 1000;
	core_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
  	periph_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  	/* For debugging purposes, enable the trace clock and/or FB_CLK so that
  	 * we'll be able to monitor clocks and know the PLL is at the frequency
  	 * that we expect.
  	 */               
          
	trace_clk_init();
  	clkout_init();
        
        /* Enable pin interrupt for the abort button - PTA4 */
        /* This pin could also be used as the NMI interrupt, but since the NMI
         * is level sensitive each button press will cause multiple interrupts.
         * Using the GPIO interrupt instead means we can configure for an edge
         * sensitive interrupt instead = one interrupt per button press.
         */
     //  enable_abort_button();
        
  	/* Enable the pins for the selected UART */
         if (TERM_PORT == UART0_BASE_PTR)
         {
           /* Enable the UART0_TXD function on PTA14 */
           PORTA_PCR14 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
           
           /* Enable the UART0_RXD function on PTA15 */
           PORTA_PCR15 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
         }

         if (TERM_PORT == UART1_BASE_PTR)
  	 {
           /* Enable the UART1_TXD function on PTE0 */
           PORTE_PCR0 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
           
           /* Enable the UART1_RXD function on PTE1 */
           PORTE_PCR1 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
           
           /* Enable the UART1_RTS function on PTE0 */
         //  PORTE_PCR3 = PORT_PCR_MUX(0x3) | PORT_PCR_DSE_MASK; // UART is alt3 function for this pin         
           
           /* Enable the UART1_CST function on PTE1 */
          // PORTE_PCR1 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
           
  	}

  	if (TERM_PORT == UART2_BASE_PTR)
  	{
#if (defined(TWR_K21D50M)) // if using TWR-K21D50M, use following UART assignments
          /* Enable the UART2_TXD function on PTC17 */
          PORTE_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART2_RXD function on PTC16 */
          PORTE_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin          
#else  // otherwise, use following UART assignments for Firebird daughter cards        
          /* Enable the UART2_TXD function on PTD3 */
          PORTD_PCR3 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART2_RXD function on PTD2 */
          PORTD_PCR2 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
#endif
  	}
  	
  	if (TERM_PORT == UART3_BASE_PTR)
  	{
#if (defined(TWR_K21D50M)) // if using TWR-K21D50M, use following UART assignments
          /* Enable the UART3_TXD function on PTC17 */
          PORTC_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART3_RXD function on PTC16 */
          PORTC_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
#else // otherwise, use following UART assignments for Firebird daughter cards
          /* Enable the UART3_TXD function on PTB10 */
          PORTB_PCR10 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART3_RXD function on PTB11 */
          PORTB_PCR11 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin                
#endif
                
  	}
#if (defined(TWR_K21F100M)) // MK21D only has 4 UARTs, ignore UART4 or higher        
  	if (TERM_PORT == UART4_BASE_PTR)
  	{
                 /* Enable the UART3_TXD function on PTC17 */
  		PORTE_PCR24 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
  		
  		/* Enable the UART3_RXD function on PTC16 */
  		PORTE_PCR25 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
  	}
#endif
        
#if (defined(TWR_K21F100M)) // if using TWR-K21F100M, use following UART assignments
        if(TERM_PORT == UART5_BASE_PTR)
  	{
          /* Enable the UART5_TXD function on PTD9 */
          PORTD_PCR8 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART5_RXD function on PTD8 */
          PORTD_PCR9 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          // add some ifdef 
#if(defined(HW_FLOW_CONTROL))
          /* Enable the UART5_RTS_B function on PTD10 */
          PORTD_PCR10 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
          /* Enable the UART5_CTS_B function on PTD11 */
          PORTD_PCR11 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
#endif
  	}
#endif
  	/* UART0 and UART1 are clocked from the core clock, but all other UARTs are
         * clocked from the peripheral clock. So we have to determine which clock
         * to send to the uart_init function.
         */
       
        if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
            uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
        else
  	    uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
}
/********************************************************************/
void trace_clk_init(void)
{
	/* Set the trace clock to the core clock frequency */
	SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;
        	
	/* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
	PORTA_PCR6 = ( PORT_PCR_MUX(0x7)
					| PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
}
/********************************************************************/
void clkout_init(void)
{
#if (defined(MCU_MK21F100)) // MK21F has FlexBus, MK21D does not
	/* Enable the clock to the FlexBus module */
        SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

 	/* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
	PORTC_PCR3 = ( PORT_PCR_MUX(0x5)
					| PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
        
#elif (defined(MCU_MK21DZ50)) // MK21D does not have FlexBus,  so output another clock instead
 	/* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
        SIM_SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_MASK; // clear clkoout field
        SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(2);    // select flash clock
	PORTC_PCR3 = ( PORT_PCR_MUX(0x5) | PORT_PCR_DSE_MASK );        
#endif
}
/********************************************************************/
void enable_abort_button(void)
{
    /* Configure the PTA4 pin for its GPIO function */
    PORTA_PCR4 = PORT_PCR_MUX(0x1); // GPIO is alt1 function for this pin
    
    /* Configure the PTA4 pin for rising edge interrupts */
    PORTA_PCR4 |= PORT_PCR_IRQC(0x9); 
    
    /* Enable the associated IRQ in the NVIC */
    enable_irq(87);      
}
/********************************************************************/

