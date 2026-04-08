
#include "platform.h"

#include "power_lib_5411x.h"

#define BUFSIZE 							256


/* PORTME:  Define various clock parameters. Note that for CoreMark testing the
 * system clock will always be provided by the FRO.  Note that TIMER_RES_DIVIDER
 * is a prescaler used as input to the timer used for benchmark timing.  Set it
 * to a value to ensure that the timer doesn't wrap before the benchmark is complete 
 */
#define TIMER_RES_DIVIDER 8 // PCLK divisor for input to system timer (make it a power of 2)
//#define SYS_CLK_RATE_HZ 12000000
//#define SYS_CLK_RATE_HZ 48000000
#define SYS_CLK_RATE_HZ 96000000


#define BAUD_RATE  9600		// Used for user terminal

#define EE_TICKS_PER_SEC (SYS_CLK_RATE_HZ / TIMER_RES_DIVIDER)


#if defined(NIOBE) || defined(NIOBE2)
#define cpu_id_addr    0xE000ED00
#define cpu_id         (* (uint32_t *) cpu_id_addr)
#define cpu_id_mask    0xfffffff0
#define cpu_id_CM0     0x410cc601    // CM0 identifier
#define cpu_id_CM4     0x410fc241    // CM4 identifier
#endif



/* Define multiprocessor bit positions */
#define MC_M4_BOOT            (1<<0)
#define MC_M4_SWD             (1<<1)
#define MC_CM4_CLK_ENABLE     (1<<2)
#define MC_CM0_CLK_ENABLE     (1<<3)
#define MC_CM4_RESET_ENABLE   (1<<4)
#define MC_CM0_RESET_ENABLE   (1<<5)
#define MC_CM4_SLEEPCON_OWNER (1<<6)

// CPU STAT register
#define MC_M4_SLEEPING        (1<<0)
#define MC_M0_SLEEPING        (1<<1)
#define MC_M4_LOCKUP          (1<<2)
#define MC_M0_LOCKUP          (1<<3)


/* Ring buffer size */
#define UART_RB_SIZE 64

/* Set the default UART, IRQ number, and IRQ handler name */
#define LPC_USART       LPC_USART0
#define LPC_IRQNUM      USART0_IRQn
#define LPC_UARTHNDLR   USART0_IRQHandler

/* Default baudrate for testing */
#define UART_TEST_DEFAULT_BAUDRATE 115200

void platform_timer_init(void) {

    /* Reset timer and enable clock to timer */
    LPC_SYSCON->PRESETCTRLSET[1] = (uint32_t) 1 << 26;
    LPC_SYSCON->PRESETCTRLCLR[1] = (uint32_t) 1 << 26;
    LPC_SYSCON->AHBCLKCTRLSET[1] = (uint32_t) 1 << 26;


    /* Reset timer counter and clear terminal count */
    LPC_TIMER0->TCR = 0;
    LPC_TIMER0->TC = 1;
    LPC_TIMER0->TCR = (uint32_t) 1 << 1;
    while (LPC_TIMER0->TC != 0) { // Wait for terminal count to clear 
    }

    /* Set to timer mode */
    LPC_TIMER0->CTCR = 0x0;

    /* Set prescaler */
    LPC_TIMER0->PR = TIMER_RES_DIVIDER - 1;

    /* Start counting */
    LPC_TIMER0->TCR = 0x1;

}

volatile uint32_t delay;

uint32_t platform_timer_get_timer_count() {

    return LPC_TIMER0->TC;

}

void set_baud_rate(LPC_USART_T *pUART, uint32_t pclk, uint32_t baud_rate) {
	
	pUART->OSR = 0xf;
	pUART->BRG = (pclk / 16 / baud_rate) - 1;;
		
}




void platform_uart_init() {
  

    Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

    /* Enable 12 MHz FRO  as clock source to Flexcomm 0 */
	  LPC_SYSCON->FXCOMCLKSEL[0] = 0x0;

	if (SYS_CLK_RATE_HZ == 96000000) {//Clocks are already set in platform_clocks_init()
          LPC_SYSCON->FROCTRL |= (1<<31)| (0xEF0000); 	 // FRO Frequency set to 96 MHz
	}
	
	

		Chip_UART_Init(DEBUG_UART);


	/* Setup UART */
    // todo ret = Chip_UART_Init(LPC_USART);
    // todo LPC_ASSERT(ret == 0, __FILE__, __LINE__);
		set_baud_rate(LPC_USART, 12000000, BAUD_RATE);
    // Chip_UART_SetBaud(LPC_USART, 9600);
    Chip_UART_ConfigData(LPC_USART, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);

    Chip_UART_Enable(LPC_USART);

	#if 0
	  a = 0xaa;
		for (i = 0; i < 100; i++) {
		   Chip_UART_SendBlocking(LPC_USART, &a, 1);
		}
#endif		
}

void platform_uart_send_byte(uint8_t c) {
	
	Chip_UART_SendBlocking(LPC_USART, &c, 1);

}

void platform_clocks_init(void) {

	
	LPC_SYSCON->MAINCLKSELA = 0x0;  	// Clock source is FRO 12 MHz;
	LPC_SYSCON->MAINCLKSELB = 0x0;  	// Clock source for main clock is MAINCLKSELA
	LPC_SYSCON->CLKOUTSELA = 0x0;     // Clock out source is main clock
	LPC_SYSCON->CLKOUTDIV = 3;				// Divide clock out by 4
	
  Chip_POWER_SetVoltage(POWER_LOW_POWER_MODE, SYS_CLK_RATE_HZ);
#if SYS_CLK_RATE_HZ == 12000000

	  Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_FRO12MHZ);
     LPC_SYSCON->FLASHCFG = 0x000a; // 1 Flash wait state, prefetch disabled, accelerator enabled
		//LPC_SYSCON->FLASHCFG = 0x001a; // 1 Flash wait state, all buffers may be used for I&D fetches, accelerator enabled
#elif SYS_CLK_RATE_HZ == 48000000
     LPC_SYSCON->FLASHCFG = 0x201a; // 3 Flash wait state, prefetch disabled, accelerator enabled  
		//LPC_SYSCON->FLASHCFG = 0x201a; // 3 Flash wait states, all buffers may be used for I&D fetches, accelerator enabled
    Chip_Clock_SetFROHFRate(SYSCON_FRO48MHZ_FREQ);
    Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_FROHF);
#elif SYS_CLK_RATE_HZ == 96000000
     LPC_SYSCON->FLASHCFG = 0x601a; // 7 Flash wait state, prefetch disabled, accelerator enabled
		//LPC_SYSCON->FLASHCFG = 0x401a; // 5 Flash wait states, all buffers may be used for I&D fetches, accelerator enabled
    Chip_Clock_SetFROHFRate(SYSCON_FRO96MHZ_FREQ);
    Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_FROHF);
    

#else

#error Invalid SYS_RATE_HZ	
#endif

   // TODO SystemCoreClockUpdate();


}

void prepare_for_powerdown(void)
{
	/* Switch clock to 12MHz for safe exit from power down */
	Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_FRO12MHZ);
  LPC_SYSCON->FLASHCFG = 0x000a; // 1 Flash wait state, prefetch disabled, accelerator enabled
	
	/* Use alternate LP control register for powerdown. Regular LP mode can be used during active low power
     configuration.	*/

		 * (uint32_t *) (((uint32_t) LPC_PMU) + 0x58) =    // ALT LP level
	    (POWER_LP_V0700<<0) |   //VD1
	    (POWER_LP_V1200<<2) |   //VD2
	    (POWER_LP_V0700<<4) |   //VD3
	    (POWER_LP_V0700<<6) |   //VD8
	    (POWER_LP_V1200<<8) |   //VD9	
	    (POWER_FINE_LP_V_M050<<10); 	 
#define PDRUNCFG1_SEL_ALTLPCTRL     (1UL << 31)

 *((volatile uint32_t *) 0x40000604) |= PDRUNCFG1_SEL_ALTLPCTRL;
}
//#define DEEP_SLEEP
void platform_power_init(void) {

	
	#define PDRUNCFG_LP_VD1           (1 << 2)	
	#define PDRUNCFG_LP_VD2           (1 << 27)
        #define PDRUNCFG_LP_VD3           (1 << 28)
        #define PDRUNCFG_LP_VD8           (1UL << 29)
	#define PDRUNCFG_LP_VDDFLASH      (1 << 12)
	
#if SYS_CLK_RATE_HZ == 12000000	
	 Chip_POWER_SetVDLevel(POWER_VD1, POWER_V0950); // Chip_POWER_SetVDLevel is part of a library
	 Chip_POWER_SetVDLevel(POWER_VD2, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD3, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD8, POWER_V0950);
	
		 * (uint32_t *) (((uint32_t) LPC_PMU) + 0x40) = 
	    (POWER_LP_V0700<<0) |   //VD1
	    (POWER_LP_V1200<<2) |   //VD2
//	    (POWER_LP_V1200<<4) |   //VD3
	    (POWER_LP_V0700<<4) |   //VD3
//	    (POWER_LP_V1200<<6) |   //VD8
	    (POWER_LP_V0700<<6) |   //VD8
	    (POWER_LP_V1200<<8) |   //VD9	
//	    (POWER_FINE_LP_V_P050<<10);
	    (POWER_FINE_LP_V_P050<<10);

	
#ifndef DEEP_SLEEP
          //Unknown flash mode! See table 89 "Flash power configurations" NXP UM10914 Rev. 0.63
	  LPC_SYSCON->PDRUNCFGSET[0] =  PDRUNCFG_LP_VD1      //  (1 << 2)       Reserved	
                                      | PDRUNCFG_LP_VD2      //  (1 << 27)      Reserved
                                      | PDRUNCFG_LP_VD3      //  (1 << 28)      Reserved
                                      | PDRUNCFG_LP_VD8      //  (1UL << 29)    Reserved  
                                      | PDRUNCFG_LP_VDDFLASH;//  (1 << 12)      LP_VDDFLASH
#endif
	
#elif SYS_CLK_RATE_HZ == 48000000	
	 Chip_POWER_SetVDLevel(POWER_VD1, POWER_V0900);
	 Chip_POWER_SetVDLevel(POWER_VD2, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD3, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD8, POWER_V0950);
	
	
	 * (uint32_t *) (((uint32_t) LPC_PMU) + 0x40) = 
	    (POWER_LP_V1200<<0) |   //VD1
	    (POWER_LP_V1200<<2) |   //VD2
	    (POWER_LP_V1200<<4) |   //VD3
	    (POWER_LP_V1200<<6) |   //VD8
	    (POWER_LP_V1200<<8) |   //VD9	
	    (POWER_FINE_LP_V_P100<<10);


#ifndef DEEP_SLEEP
                //Unknown flash mode! See table 89 "Flash power configurations" NXP UM10914 Rev. 0.63
		LPC_SYSCON->PDRUNCFGSET[0] =  PDRUNCFG_LP_VD2 	   //  (1 << 27)     Reserved              
                                            | PDRUNCFG_LP_VD3      //  (1 << 28)     Reserved               
                                            | PDRUNCFG_LP_VD8      //  (1UL << 29)   Reserved                 
                                            | PDRUNCFG_LP_VDDFLASH;//  (1 << 12)     LP_VDDFLASH                       
#endif
	
#elif SYS_CLK_RATE_HZ == 96000000
 // make sure we are NOT in LP mode
  LPC_SYSCON->PDRUNCFGCLR[0] =  PDRUNCFG_LP_VD1      //  (1 << 2)       Reserved
                              | PDRUNCFG_LP_VD2      //  (1 << 27)      Reserved
                              | PDRUNCFG_LP_VD3      //  (1 << 28)      Reserved
                              | PDRUNCFG_LP_VD8      //  (1UL << 29)    Reserved  
                              | PDRUNCFG_LP_VDDFLASH;//  (1 << 12)      LP_VDDFLASH

	 Chip_POWER_SetVDLevel(POWER_VD1, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD2, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD3, POWER_V1200);
	 Chip_POWER_SetVDLevel(POWER_VD8, POWER_V1200);	
#else
#error Invalid SYS_RATE_HZ	
#endif

	
#define PDRUNCFG1_PD_ALT_FLASH_IBG  (1UL << 28)
#define PDRUNCFG1_SEL_ALT_FLASH_IBG (1UL << 29)
#define PDRUNCFG_PD_FLASH_BG      (1 << 25)

  // use the alternative flash bandgap and turn off the original one          
  LPC_SYSCON->PDRUNCFGCLR[1] = PDRUNCFG1_PD_ALT_FLASH_IBG;
  LPC_SYSCON->PDRUNCFGSET[1] = PDRUNCFG1_SEL_ALT_FLASH_IBG;
  LPC_SYSCON->PDRUNCFGSET[0] = PDRUNCFG_PD_FLASH_BG;


	
	
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 7);	// FLASH (only needed for FLASH write/erase/initialisation
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 10);	// SPIFI
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 11);	// INPUTMUX
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 13);	// IOCON
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 14);	// GPIO0
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 15);	// GPIO1
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 18);	// PINT	
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 19);	// GINT	
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 20);	// DMA	
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 21);	// CRC		
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 22);	// WWDT		
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 23);	// RTC			
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 26);	// MAILBOX		
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 27);	// ADC0		

	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 0);	// MRT		
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 2);	// SCT0
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 10);	// UTICK
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 12);	// FLEXCOMM1
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 13);	// FLEXCOMM2
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 14);	// FLEXCOMM3	
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 15);	// FLEXCOMM4
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 16);	// FLEXCOMM5
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 17);	// FLEXCOMM6
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 18);	// FLEXCOMM7
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 19);	// DMIC
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 22);	// CT32B2
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 25);	// USB
	// LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 26);	// CT32B0 Don't disable as it is used for Coremark timing
	LPC_SYSCON->AHBCLKCTRLCLR[1] = ((uint32_t)1 << 27);	// CT32B1	
	
#ifndef RUN_FROM_RAM	        
        
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 4);  	// SRAM2
	LPC_SYSCON->PDRUNCFGSET[0] = ((uint32_t)1 << 15);	// SRAM2

#ifndef RAM_RAMX_SRAM0	
	LPC_SYSCON->PDRUNCFGSET[0] = ((uint32_t)1 << 16);	// SRAMX	// TODO
#endif

	#define PDRUNCFG_PD_VDDHV_ENA     (1 << 18)

#ifdef RAM_RAMX_SRAM0	 
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 8);			// FMC	
	LPC_SYSCON->PDRUNCFGSET[0] = PDRUNCFG_PD_VDDHV_ENA;
#endif

#ifdef RAM_SRAM0_SRAM0 
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 8);			// FMC	
	LPC_SYSCON->PDRUNCFGSET[0] = PDRUNCFG_PD_VDDHV_ENA;
#endif

#ifdef RAM_SRAM0_SRAM1
	LPC_SYSCON->AHBCLKCTRLCLR[0] = ((uint32_t)1 << 8);			// FMC	
	LPC_SYSCON->PDRUNCFGSET[0] = PDRUNCFG_PD_VDDHV_ENA;
#endif


#endif //#ifndef RUN_FROM_RAM	        
        
	LPC_SYSCON->PDRUNCFGSET[0] = ((uint32_t)1 << 22);			// PLL0
	

//#else


//#endif
	
	
}
