


#include "platform.h"
#include "board.h"

extern int ee_printf(const char *fmt, ...);

#define SWD 0
#define ADC 1
/* Pin muxing table, only items that need changing from their default pin
   state are in this table. Not every pin is mapped. */
STATIC const PINMUX_GRP_T pinmuxing_lowpower[] = {
	/* UART */
	{0, 0,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* UART0 RX */
	{0, 1,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* UART0 TX */

	{0, 2,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 3,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	
	{0, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 5,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 6,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 7,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	
	{0, 8,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 9,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 10,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 11,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 12,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 13,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 14,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 15,  (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	#if SWD
	{0, 16,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
        {0, 17,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	#endif
        {0, 18,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 19,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 20,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 21,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 22,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 23,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 24,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 25,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 26,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 27,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 28,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
		
	#if ADC
	{0, 29,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 30,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{0, 31,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 0,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 1,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 2,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 3,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 5,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 6,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 7,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 8,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	#endif
	{1, 9,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 10,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 11,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 12,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 13,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 14,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 15,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 16,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
	{1, 17,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}, /* NC */
};

void SetupUnusedPins(void)
{
	#if 1
	
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);
	#else
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 0, 0);
	#endif
	
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 2);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 3);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 4);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 4, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 5);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 5, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 6);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 6, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 7);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 7, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 8);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 9);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 9, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 10);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 10, 0);
	
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 11);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 11);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 11, 0);
	
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 12);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 12);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 12, 0);
	
	
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 13);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 13, 0);
	
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 14);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 14);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 14, 0);

	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 15);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 15, 0);
#if SWD
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 16);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 16);  // swd pin
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 16, 0);   // swd pin
 //Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 17);
 	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 17);  // swd pin
 	Chip_GPIO_SetPinState(LPC_GPIO, 0, 17, 0);   // swd pin
#endif
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 18);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 18, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 19);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 19, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 20);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 20, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 21);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 21, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 22);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 22, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 23);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 23, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 24);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 24, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 25);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 25, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 26);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 26, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 27);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 27, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 28);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, 0);

	#if ADC
	
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 29);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 29, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 30);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 30, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 31);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 31, 0);

	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 1, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 2);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 2, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 3);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 3, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 4);
	Chip_GPIO_SetPinState(LPC_GPIO, 1,4, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 5);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 5, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 6);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 6, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 7);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 7, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 8);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 8, 0);
	#endif
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 9);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 9, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 10);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 10, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 11);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 11, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 12);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 12, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 13);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 13, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 14);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 14, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 15);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 15, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 16);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 16, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 17);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 17, 0);	 
}

void ConfigureUnusedPins(void)
{
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO0);
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO1);
	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing_lowpower, sizeof(pinmuxing_lowpower) / sizeof(PINMUX_GRP_T));
	SetupUnusedPins();
	
}



/**
 * Send a character to the system UART
 * 
 * @param c  Character to send
 */
void portable_send_char(char c) {

    platform_uart_send_byte(c);
}

/**
 * Initializes functionality specific to the chip/platform
 *
 * This function calls target specific functionality to initialize the system
 * in order to execute the benchmark.  It also does does various checks for 
 * portability errors.  Key functionality that must be initialized/powered up:
 *   System clocks
 *   UART for program output
 *   Miscellaneous 
 * 
 * @param p     Not used
 * @param argc  Not used
 * @param argv  Not used
 */
void portable_init() {

    platform_power_init();

    platform_clocks_init();

    /* Initialize UART so that coremarks results can be displayed */
    platform_uart_init();
    ee_printf("\r\ninit_UART() completed...\r\n");

}


void enable_SRAM0(bool enable) {
	
	if (enable == true) {
	   LPC_SYSCON->PDRUNCFGCLR[0] = (uint32_t)1 << 13;
	} else {
		LPC_SYSCON->PDRUNCFGSET[0] = (uint32_t)1 << 13;
	}
	
}

void enable_SRAM1(bool enable) {
	
	if (enable == true) {
		LPC_SYSCON->AHBCLKCTRLSET[0] = (uint32_t)1 << 3;
	  LPC_SYSCON->PDRUNCFGCLR[0] = (uint32_t)1 << 14;
	} else {
		LPC_SYSCON->AHBCLKCTRLCLR[0] = (uint32_t)1 << 3;
		LPC_SYSCON->PDRUNCFGSET[0] = (uint32_t)1 << 14;
	}
	
	
}

void enable_SRAM2(bool enable) {
	
	if (enable == true) {
		LPC_SYSCON->AHBCLKCTRLSET[0] = (uint32_t)1 << 4;
	  LPC_SYSCON->PDRUNCFGCLR[0] = (uint32_t)1 << 15;
	} else {
		LPC_SYSCON->AHBCLKCTRLCLR[0] = (uint32_t)1 << 4;
		LPC_SYSCON->PDRUNCFGSET[0] = (uint32_t)1 << 15;
	}
	
}

void enable_SRAMX(bool enable) {

	if (enable == true) {
	   LPC_SYSCON->PDRUNCFGCLR[0] = (uint32_t)1 << 16;
	} else {
		LPC_SYSCON->PDRUNCFGSET[0] = (uint32_t)1 << 16;
	}

}

void sleep_measurement() {

#ifndef RUN_FROM_RAM
	enable_SRAMX(false);
	enable_SRAM0(true);
	enable_SRAM1(false);
	enable_SRAM2(false);
#else
	enable_SRAMX(true);
	enable_SRAM0(true);
	enable_SRAM1(true);
	enable_SRAM2(true);
#endif
	Chip_POWER_EnterPowerMode(POWER_SLEEP, SYSCON_PDRUNCFG_PD_SRAM0);
	
}


extern void prepare_for_powerdown(void);
void power_down_measurement() {
	
	enable_SRAMX(true);
	enable_SRAM0(true);
	enable_SRAM1(true);
	enable_SRAM2(true);
	
	/* Prepare for powerdown first */
	prepare_for_powerdown();
	
	Chip_POWER_EnterPowerMode(POWER_POWER_DOWN, 
								  (SYSCON_PDRUNCFG_PD_SRAM0 | SYSCON_PDRUNCFG_PD_SRAM1 | 
									SYSCON_PDRUNCFG_PD_SRAM2 | SYSCON_PDRUNCFG_PD_SRAMX));
	
}


void deep_power_down_measurement() {
	
	enable_SRAMX(true);
	enable_SRAM0(true);
	enable_SRAM1(true);
	enable_SRAM2(true);
	
	Chip_POWER_EnterPowerMode(POWER_DEEP_POWER_DOWN, 
								  (SYSCON_PDRUNCFG_PD_SRAM0 | SYSCON_PDRUNCFG_PD_SRAM1 | 
									SYSCON_PDRUNCFG_PD_SRAM2 | SYSCON_PDRUNCFG_PD_SRAMX));
	
}

//#define DEEP_SLEEP 
#define UTICK_WAKEUP 1
#define PINT_WAKEUP 0
/* GPIO pin for PININT interrupt */
#define GPIO_PININT_PIN     24	/* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT    0	/* GPIO port number mapped to PININT */
#define GPIO_PININT_INDEX   PININTSELECT0	/* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */
                            
void PININT_IRQ_HANDLER(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT0));
	LPC_GPIO->NOT[1] = (1<<10);
} 

void UTICK_IRQHandler(void)
{
	Chip_UTICK_ClearInterrupt(LPC_UTICK);
	Board_LED_Set(1, true);
}



int main( void ) {
	uint32_t i;
	portable_init ();

	// Uncomment the test you want to run
#ifdef DEEP_SLEEP
	// Uncomment the test you want to run
	ConfigureUnusedPins(); // Minimize leakage on LPCXpresso board 
	Board_LED_Set(0, false);
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
#endif		
	ee_printf("Entering reduced power state...\r\n");
	
	sleep_measurement();

#ifdef DEEP_SLEEP
#if PINT_WAKEUP
Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_INPUTMUX);	
Chip_PININT_Init(LPC_PININT);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT_PIN);//OK

	/* Configure pin as GPIO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_PININT_PORT, GPIO_PININT_PIN,
						 (IOCON_FUNC0 | IOCON_DIGITAL_EN  | IOCON_GPIO_MODE));//OK
                                                //    FUNC    |     DIGIMODE      |     I2CSLEW

	/* Configure pin interrupt selection for the GPIO pin in Input Mux Block */
	Chip_INMUX_PinIntSel(GPIO_PININT_INDEX, GPIO_PININT_PORT, GPIO_PININT_PIN);//OK

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
        Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX));//OK
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX)); //OK
        //Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX)); //Orirginal Place
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));

	NVIC_DisableIRQ(PININT_NVIC_NAME);
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	/* Enable interrupt in the NVIC */
	NVIC_EnableIRQ(PININT_NVIC_NAME);
	//NVIC_DisableIRQ(PININT_NVIC_NAME);
	
	/* Enable wakeup for PININT0 */
	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_PINT0);

#endif //#if PINT_WAKEUP

#if UTICK_WAKEUP
	/* Enable the power to the Watchdog Oscillator,
	   UTick timer ticks are driven by watchdog oscillator */
	Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_WDT_OSC);

	/* Initialize UTICK driver */
	Chip_UTICK_Init(LPC_UTICK);

	/* Clear UTICK interrupt status */
	Chip_UTICK_ClearInterrupt(LPC_UTICK);

	/* Set the UTick for a delay of 8000mS and in repeat mode */
	Chip_UTICK_SetDelayMs(LPC_UTICK, 8000, true);

	/* Enable Wake up from deep sleep mode due to UTick */
	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_UTICK);

	/* Enable UTICK interrupt */
	NVIC_EnableIRQ(UTICK_IRQn);
#endif //#if UTICK_WAKEUP	

	deep_sleep_measurement(); //Is FRO12Mhz selected? see p.92 (Ch. 5.3.4.2) of UM
	while(1);
#endif //DEEP_SLEEP
	
	/* Change Pin settings to minimize leakage in powerdown */
	for ( i=0;i<32;i++) {
		if ( i!=29 ) {
			Chip_IOCON_PinMuxSet(LPC_IOCON, 0, i, IOCON_FUNC0|IOCON_MODE_PULLDOWN);
		}
	}
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, IOCON_FUNC0|IOCON_MODE_PULLUP);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);
	
	//power_down_measurement();
	deep_power_down_measurement();
}
