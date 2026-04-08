/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/* Include files */
#include "LowPowerAppStates.h"

/* Declaration of module wide FUNCTIONs - NOT for use in other modules */

void vfnLowPowerDemoIDLEState(void);
void vfnLowPowerDemoReadState(void);
void vfnLowPowerDemoCheckValueState(void);
void vfnLowPowerDemoGoLowPowerState(void);
void vfnLowPowerAlarmState(void);
void vfnSampleAccel(void);
void vfnEnterLowPower(void);
void uart(uint8 u8state);
void i2c(uint8 u8state);
void vfnlptmrInit(int count, int clock_source);

/* Definition of module wide VARIABLEs - NOT for use in other modules*/
sSM sMLowPowerDemoState;
uint8 gu8lpTimerEventFlag = 0;
uint16 gu16PotVValue;
uint16 gu16PotmVValue;
uint8 gu8AppSelected;

/* Actual system clock frequency */
extern int mcg_clk_hz;
extern int mcg_clk_khz;
extern int core_clk_khz;
extern int periph_clk_khz;
extern int pll_clk_khz;
extern signed char result[20];
extern uint8 gu8lpTimerEventFlag;
uint32 gu32AccelSampling;
uint16 gu16AlarmDelay = 0;
uint8 gu8AlarmFlag = FALSE;
uint16 gu16SecondsCounter = 0;
tADC_Config Master_Adc_Config;

void (*const vpfnLowPowerDemoStateMachine[]) (void) = {
	vfnLowPowerDemoIDLEState,
	vfnLowPowerDemoReadState,
	vfnLowPowerDemoCheckValueState,
	vfnLowPowerDemoGoLowPowerState, vfnLowPowerAlarmState
};

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Idle state do nothing
 */
void vfnLowPowerDemoIDLEState(void)
{
	sMLowPowerDemoState.ActualState = LP_DEMO_READ_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_IDLE_STATE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Read the Potentiometer value or the Accelerometer position
 */
void vfnLowPowerDemoReadState(void)
{
	if (gu8AppSelected)
		vfnSampleAccel();
	else {
		gu16PotVValue = u16fnADCRead(ADC_POT_CHANNEL);
		gu16PotVValue = (gu16PotVValue * 330) / 4095;
		gu16PotmVValue = gu16PotVValue % 100;
		gu16PotVValue = gu16PotVValue / 100;
	}
	sMLowPowerDemoState.ActualState = LP_DEMO_CHECK_VALUE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_READ_STATE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Depending the obtained value on past state go to sleep or send the alarm
 */
void vfnLowPowerDemoCheckValueState(void)
{
	if (gu8AppSelected) {
		if (HORIZONTAL_POSITION)
			sMLowPowerDemoState.ActualState = LP_DEMO_ALARM_STATE;
		else
			sMLowPowerDemoState.ActualState =
			    LP_DEMO_GO_LOW_POWER_STATE;
	} else {
		if (gu16PotVValue >= TWO_VOLTS_VALUE)
			sMLowPowerDemoState.ActualState = LP_DEMO_ALARM_STATE;
		else
			sMLowPowerDemoState.ActualState =
			    LP_DEMO_GO_LOW_POWER_STATE;
	}

	sMLowPowerDemoState.PrevState = LP_DEMO_CHECK_VALUE_STATE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Reset counters and flags and go to LLS mode
 */
void vfnLowPowerDemoGoLowPowerState(void)
{
	gu8AlarmFlag = FALSE;
	gu16SecondsCounter = 0;

	vfnEnterLowPower();

	sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_GO_LOW_POWER_STATE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Every Second the LED toggle and a message will be send thru UART
 */
void vfnLowPowerAlarmState(void)
{
	if (!gu8AlarmFlag) {
		gu8AlarmFlag = TRUE;
		uart(ON);
		vfnLEDsOutPutConfig();
		gu16SecondsCounter = 0;
	}

	if (gu8lpTimerEventFlag) {
		gu16AlarmDelay++;
		gu8lpTimerEventFlag = FALSE;
	}

	if (gu16AlarmDelay == ALARM_DELAY) {
		gu16SecondsCounter++;
		LED1_TOGGLE;
		if (gu8AppSelected)
			printf("Horizontal position (For %d Sec)\n\r",
			       gu16SecondsCounter);
		else
			printf("Potentiometer value = %d.%2dv)\n\r",
			       gu16PotVValue, gu16PotmVValue);

		gu16AlarmDelay = 0;
	}

	sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_ALARM_STATE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Change the MCG and UART to use internal clock at 21000MHz
 */
void vfnMCGandUARTConfig(void)
{
	uint8 op_Mode;
	int i;

	// waste some cycles
	for (i = 0; i < 8000; i++);

	// avoid send unwanted byte while changing the clock
	UART_C2_REG(TERM_PORT) &= ~UART_C2_TE_MASK;

	op_Mode = what_mcg_mode();
	if (op_Mode == PEE) {
		mcg_clk_hz = pee_pbe(CLK0_FREQ_HZ);
		mcg_clk_hz = pbe_fbe(CLK0_FREQ_HZ);
		mcg_clk_hz = fbe_fbi(4000000, 1);
		mcg_clk_hz = fbi_fei(32000);
	}

	/*
	 * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
	mcg_clk_khz = mcg_clk_hz / 1000;
	core_clk_khz =
	    mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28) +
			   1);
	periph_clk_khz =
	    mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) +
			   1);

	/*
	 * Check platform file (tower.h) and sysinit.c
	 * to make sure they are consistence
	 */
	if (TERM_PORT == UART0_BASE_PTR) {
#ifdef UART0_ALT1
		PORTA_PCR1 = PORT_PCR_MUX(2);	/* RX */
		PORTA_PCR2 = PORT_PCR_MUX(2);	/* TX */
#endif
#ifdef UART0_ALT2
		PORTA_PCR14 = PORT_PCR_MUX(3);	/* TX */
		PORTA_PCR15 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef UART0_ALT3
		PORTB_PCR16 = PORT_PCR_MUX(3);	/* TX */
		PORTB_PCR17 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef UART0_ALT4
		PORTD_PCR6 = PORT_PCR_MUX(3);	/* TX */
		PORTD_PCR7 = PORT_PCR_MUX(3);	/* RX */
#endif
	}

	if (TERM_PORT == UART1_BASE_PTR) {
#ifdef UART1_ALT1
		PORTC_PCR3 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR4 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART1_ALT2
		PORTE_PCR0 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR1 = PORT_PCR_MUX(3);	/* RX */
#endif
	}

	if (TERM_PORT == UART2_BASE_PTR) {
		PORTD_PCR2 = PORT_PCR_MUX(3);	/* RX */
		PORTD_PCR3 = PORT_PCR_MUX(3);	/* TX */
	}

	if (TERM_PORT == UART3_BASE_PTR) {
#ifdef UART3_ALT1
		PORTB_PCR10 = PORT_PCR_MUX(3);	/* RX */
		PORTB_PCR11 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART3_ALT2
		PORTC_PCR16 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR17 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART3_ALT3
		PORTE_PCR4 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR5 = PORT_PCR_MUX(3);	/* RX */
#endif
	}

	if (TERM_PORT == UART4_BASE_PTR) {
#ifdef UART4_ALT1
		PORTC_PCR14 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR15 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART4_ALT2
		PORTE_PCR24 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR25 = PORT_PCR_MUX(3);	/* RX */
#endif
	}

	if (TERM_PORT == UART5_BASE_PTR) {
#ifdef UART5_ALT1
		PORTD_PCR8 = PORT_PCR_MUX(3);	/* RX */
		PORTD_PCR9 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART5_ALT2
		PORTE_PCR8 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR9 = PORT_PCR_MUX(3);	/* RX */
#endif
	}

	if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
		uart_init(TERM_PORT, core_clk_khz, 9600);
	else
		uart_init(TERM_PORT, periph_clk_khz, 9600);
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Configure Accelerometer
 */
void vfnAccelConfig(void)
{
	/* Initialize I2C */
	init_I2C();
	/* Configure accelemoter sensor */
	I2CWriteRegister(0x2A, 0x01);
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Take some samples for the Tower position
 */
void vfnSampleAccel(void)
{
	while (gu32AccelSampling++ <= ACCEL_SAMPLING_TIMES) {
		vfnAccelRead();
	}
	gu32AccelSampling = 0;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Read tower position
 */
void vfnAccelRead(void)
{
	/* Look at status of accelerometer */
	I2CReadMultiRegisters(0x01, 6);
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Configure LPTMR to interrupt every 100ms
 */
void vfnLPTMRConfig(void)
{
	vfnlptmrInit(100, LPTMR_USE_LPOCLK);	/* 100 */
	enable_irq(INT_LPTimer - 16);
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Enable LLWU module and allow LPTMR as wake up module
 */
void vfnLowPowerConfig(void)
{
	/* Enable all operation modes because this is a write once register */
	SMC_PMPROT = SMC_PMPROT_AVLLS_MASK |
				 SMC_PMPROT_ALLS_MASK |
				 SMC_PMPROT_AVLP_MASK;

	LLWU_ME |= (1 << LLWU_ME_WUME0_SHIFT);	/* enable LPTMR to wake up */

	enable_irq(INT_LLW - 16);
}

 /*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Disable UART and configure GPIO as input to reduce current consumption and go to sleep
 */
void vfnEnterLowPower(void)
{
	vfnLEDsInPutConfig();
	uart(OFF);
	//enter_lls();
	enter_wait();
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Disable UART pins
 */
void uart(uint8 u8state)
{
	if (u8state) {
		PORTA_PCR14 = PORT_PCR_MUX(3);	/* UART is alt3 function for this pin PORTE_PCR16 */
		PORTA_PCR15 = PORT_PCR_MUX(3);	/* UART is alt3 function for this pin PORTE_PCR17 */

	} else {
		PORTA_PCR14 = PORT_PCR_MUX(0);	/* Analog is alt0 function for this pin */
		PORTA_PCR15 = PORT_PCR_MUX(0);	/* Analog is alt0 function for this pin */
	}
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Enable clock gates
 */
void vfnEnableClockGates(void)
{
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
	SIM_SCGC5 |=
	    (SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK |
	     SIM_SCGC5_PORTB_MASK);
	SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;	/* Turn on clock to I2C` module */
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Configure Low Power Timer to use Low Power Oscillator as source
 */
void vfnlptmrInit(int count, int clock_source)
{
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	LPTMR0_PSR = (LPTMR_PSR_PRESCALE(0) |		/* 0000 is div 2 */
				  LPTMR_PSR_PBYP_MASK |			/* LPO feeds directly to LPT */
				  LPTMR_PSR_PCS(clock_source));	/* use the choice of clock */

	LPTMR0_CMR = LPTMR_CMR_COMPARE(count);		/*Set compare value */

	LPTMR0_CSR = (LPTMR_CSR_TCF_MASK |			/* Clear any pending interrupt */
				  LPTMR_CSR_TIE_MASK |			/* LPT interrupt enabled */
				  LPTMR_CSR_TPS(0) |			/* TMR pin select */
				  !LPTMR_CSR_TPP_MASK |			/* TMR Pin polarity */
				  !LPTMR_CSR_TFC_MASK |			/* Timer Free running counter
												 * is reset whenever TMR
												 * counter equals compare */
				  !LPTMR_CSR_TMS_MASK);			/* LPTMR0 as Timer */

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	/*Turn on LPT and start counting */
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Low power timer interrupt.
 * Note: Use this function to avoid modify the common one on the driver
 */
void lptmr_app_isr(void)
{
	//LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;
	/* write 1 to TCF to clear the LPT timer compare flag */
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	LPTMR0_CSR =
	    (LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK);

	gu8lpTimerEventFlag = TRUE;
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Configure ADC for single ended
 */
void vfnADCConfig(void)
{
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;

	/* Initialize ADC1 */
	/* Do calibration first with 32 h/w averages */
	Master_Adc_Config.CONFIG1 =
	    ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) | ADLSMP_LONG |
	    ADC_CFG1_MODE(MODE_16)
	    | ADC_CFG1_ADICLK(ADICLK_BUS_2);
	Master_Adc_Config.CONFIG2 =
	    MUXSEL_ADCA | ADACKEN_ENABLED | ADHSC_HISPEED |
	    ADC_CFG2_ADLSTS(ADLSTS_20);
	Master_Adc_Config.COMPARE1 = 0x1234u;
	Master_Adc_Config.COMPARE2 = 0x5678u;
	Master_Adc_Config.STATUS2 =
	    ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_ENABLED |
	    DMAEN_DISABLED | ADC_SC2_REFSEL(REFSEL_EXT);
	Master_Adc_Config.STATUS3 =
	    CAL_OFF | ADCO_SINGLE | AVGE_ENABLED | ADC_SC3_AVGS(AVGS_32);
	//Master_Adc_Config.PGA = PGAEN_DISABLED | PGACHP_NOCHOP | PGALP_NORMAL | ADC_PGA_PGAG(PGAG_64);
	Master_Adc_Config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(ADC_POT_CHANNEL);
	Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(ADC_POT_CHANNEL);

	ADC_Config_Alt(ADC1_BASE_PTR, &Master_Adc_Config);	/* config ADC */
	ADC_Cal(ADC1_BASE_PTR);			/* do the calibration */

	/*
	 * Now do normal ADC configuration with 4 h/w averages
	 * and h/w trigger from PDB
	 */
	Master_Adc_Config.CONFIG1 =
	    ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) | ADLSMP_LONG |
	    ADC_CFG1_MODE(MODE_12)
	    | ADC_CFG1_ADICLK(ADICLK_BUS_2);
	Master_Adc_Config.CONFIG2 =
	    MUXSEL_ADCA | ADACKEN_ENABLED | ADHSC_HISPEED |
	    ADC_CFG2_ADLSTS(ADLSTS_20);
	Master_Adc_Config.COMPARE1 = 0x1234u;
	Master_Adc_Config.COMPARE2 = 0x5678u;
	Master_Adc_Config.STATUS2 =
	    ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_DISABLED |
	    DMAEN_DISABLED | ADC_SC2_REFSEL(REFSEL_EXT);
	Master_Adc_Config.STATUS3 =
	    CAL_OFF | ADCO_SINGLE | AVGE_ENABLED | ADC_SC3_AVGS(AVGS_4);
	//Master_Adc_Config.PGA = PGAEN_DISABLED | PGACHP_NOCHOP | PGALP_NORMAL | ADC_PGA_PGAG(PGAG_64);

	Master_Adc_Config.STATUS1B =
	    AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(ADC_POT_CHANNEL);

	// config the ADC again to default conditions
	ADC_Config_Alt(ADC1_BASE_PTR, &Master_Adc_Config);
}

/*
 * @return none
 *
 * <b>Description:</b>
 * \par
 * Low power timer interrupt.
 * Read the value on the selected channel
 */
uint16 u16fnADCRead(uint8 channel)
{
	ADC1_SC1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(channel);	/* reads POT signal */
	while ((ADC1_SC1A & ADC_SC1_COCO_MASK) == 0) {
	};
	return ADC1_RA;
}
