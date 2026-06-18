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
/*
 * File:		hw_trig_test.c
 * Purpose:		Demo adc triggered by PDB module
 */

#include "common.h"
#include "adc_demo.h"
#include "hw_trig_test.h"
#include  "adc16.h"

/* Global filtered output for ADC1 used by ISR and by demo code print routine */
extern tADC_Config Master_Adc_Config;
extern uint32_t irq_sw;

uint32 exponentially_filtered_result1 = 0;
static int8_t cycle_flags = 0;
static volatile unsigned result0A, result0B;

/*
 * The folowing code demonstrates the hardware trigger capability of the ADC.
 * For the ADC there are two hardware trigger channels, therefore the two
 * status&control "SC1" registers and two result registers are tested.
 *
 * [Why the ADC need two sets of some registers?  So that while one is in use,
 * the other set can be used. It alows a ping-pong approach that will be
 * demonstrated here.]
 *
 * A total of two triggering signals are generated from the PDB hardware
 * and the code causes every one of those to be routed to the correct ADC
 * trigger channel.
 *
 * The PDB is set in continuous, software triggered mode , counting from 0 to
 * 0xFFFF from a  frequency slow enough to allow a very long cycle during which
 * there is printed something about the two ADC trigger events.
 *
 * A PDB interrupt is setup at the beginning of each PDB cycle (PDB_IDLY = 0).
 * It is set to signal by toggling "PIN", at the beginning of each cycle.
 * This will be visible as an LED.
 * These two PDB delays will trigger two triggers on ADC1. The ADC, for each
 * "trigger-SC1x-result" group, will generate an end of conversion interrupt
 * and set a bit in a global variable to flag execution of that trigger.
 * Background code will check for all the two triggers and conversions to end,
 * Then the printf on a single line the two results from adc is shown. The
 * effect of an exponential filter is shown on subsequent lines. Both the
 * filtered, and unfiltered conversion results are displayed. The filter is a
 * simple exponential filter that decays an impulse exponentially.
 *
 * Upon the execution of each trigger, the ADC interrupt will signal the "A"
 * trigger with a high on a dedicated pin and a "B" trigger via a low level
 * on same pin.
 *
 * Again, the ADC has a set of A registers and a set of B registers for those
 * items that benefit from ping-ponging (double buffering).
 *
 * "PIN1" will be used by ADC1 isr. By resetting both marking pins in the
 * initial PDB and having "A" trigger happen before "B"  the sequences can be
 * seen.
 */

uint8_t Hw_Trig_Test(void)
{
	/*
	 * Notes:
	 *	PDB settings : continous mode, started by sotware trigger. This means
	 *	that once the software "pulls the trigger" by setting a certain bit,
	 *	the PDB starts counting and handing out four triggers per cycle of its
	 *	counter.
	 *
	 *	PDB settings: CH0_DLY0, CH0_DLY1 set to different values to distinguish
	 *	effect on ADC1_Rx register need to provide 2 different voltages to
	 *	convert at two ADC1 channels PDB counter clock prescaled to allow time
	 *	for printf's and slow down things to they are visible, each trigger.
	 *
	 *	Using adiclk= BUS ,  and adidiv/4 to get  12,25MHz on Tower
	 *	demonstration. visibility of PDB start trigger is obtained by
	 *	generating a toggling edge on GPIOxx with PDBisr set to trigger
	 *	immediatly at zero value of PDB counter.
	 *
	 *	Conversion end of the ADC and channel within the ADC ( A,B ) will be
	 *	done by toggling second GPIO pin inside ADCisr  ( this pin is also
	 *	reset by PDB isr )
	 */

	/*
	 * The System Integration Module largely determines the role of the
	 * different ball map locations on Kinetis. When an external pin is used,
	 * the System Integration Module should be consulted and invoked as needed.
	 * System integration module registers start with SIM_
	 */

	/*
	 * Turn on the ADC1 clock as well as the PDB clocks to test
	 * ADC triggered by PDB
	 */
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;
	SIM_SCGC6 |= SIM_SCGC6_PDB_MASK;

	/* Configure System Integration Module for defaults as far as ADC */
	/* selects PDB not ALT trigger */
	SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK | SIM_SOPT7_ADC1PRETRGSEL_MASK);

	SIM_SOPT7 = SIM_SOPT7_ADC1TRGSEL(0);	/* applies only in case of ALT trigger, in which case */
	/* PDB external pin input trigger for ADC */

	/* PDB configured below */
	PDB0_SC = 0x00000000;

	/*
	 * Configure the Peripheral Delay Block (PDB):
	 * enable PDB, pdb counter clock = busclock / 20 , continous triggers,
	 * sw trigger , and use prescaler too
	 */
	/* Enable PDB */
	PDB0_SC |= PDB_SC_PDBEN_MASK | PDB_SC_TRGSEL(0xF) | PDB_SC_PRESCALER(1);

	/* Enable continuous mode */
	PDB0_SC |= PDB_SC_CONT_MASK;

	/* the software trigger, PDB_SC_SWTRIG_MASK is not triggered at this time. */

	/* channel 0 pretrigger 0 and 1 enabled and delayed */
	PDB0_CH1C1 = PDB_C1_EN(3) | PDB_C1_TOS(3) |
				 PDB_SC_PRESCALER(5) | PDB_SC_MULT(2);

	/* Set CH1 DLY0/1 */
	PDB0_CH1DLY0 = ADC1_DLYA;
	PDB0_CH1DLY1 = ADC1_DLYB;	/* delay the conversion time = 76us */

	/* Set PDB_MOD to 156us corresponding to sampling frequency Fs=6.4KHz */
	PDB0_MOD = 0xFFFF;

	/* Set interrupt delay value */
	PDB0_IDLY = 0;

	PDB0_SC |= PDB_SC_PDBIE_MASK		/* PDB Interrupt Enable */
	    | PDB_SC_PDBEIE_MASK
	    | PDB_SC_LDOK_MASK;		/* Need to ok the loading or it will not load
								 * certain regsiters! */
	/* the software trigger, PDB_SC_SWTRIG_MASK is not triggered at this time. */


	/* ADC configured below */

	/* setup the initial ADC default configuration */
	Master_Adc_Config.CONFIG1 = ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) |
								ADLSMP_LONG | ADC_CFG1_MODE(MODE_16) |
								ADC_CFG1_ADICLK(ADICLK_BUS_2);
	Master_Adc_Config.CONFIG2 = MUXSEL_ADCA | ADACKEN_DISABLED |
								ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_20);
	Master_Adc_Config.COMPARE1 = 0x1234u;
	Master_Adc_Config.COMPARE2 = 0x5678u;
	Master_Adc_Config.STATUS2 = ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER |
								ACREN_ENABLED | DMAEN_DISABLED |
								ADC_SC2_REFSEL(REFSEL_EXT);
	Master_Adc_Config.STATUS3 = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED |
								ADC_SC3_AVGS(AVGS_32);
	Master_Adc_Config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31);
	Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31);

	/*
	 * Configure ADC as it will be used, but becuase ADC_SC1_ADCH is 31,
	 * the ADC will be inactive.  Channel 31 is just disable function.
	 * There really is no channel 31.
	 */

	ADC_Config_Alt(ADC1_BASE_PTR, &Master_Adc_Config);	/* config ADC */

	/* Calibrate the ADC in the configuration in which it will be used: */
	ADC_Cal(ADC1_BASE_PTR);		/* do the calibration */

	/*
	 * The structure still has the desired configuration.  So restore it.
	 * Why restore it?  The calibration makes some adjustments to the
	 * configuration of the ADC.  The are now undone:
	 */
	Master_Adc_Config.CONFIG1 = ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) |
								ADLSMP_LONG | ADC_CFG1_MODE(MODE_16) |
								ADC_CFG1_ADICLK(ADICLK_BUS_2);
	Master_Adc_Config.CONFIG2 = MUXSEL_ADCA | ADACKEN_ENABLED |
								ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_20);
	Master_Adc_Config.COMPARE1 = 0x1234u;
	Master_Adc_Config.COMPARE2 = 0x5678u;
	Master_Adc_Config.STATUS2 = ADTRG_HW | ACFE_DISABLED | ACFGT_GREATER |
								ACREN_DISABLED | DMAEN_DISABLED |
								ADC_SC2_REFSEL(REFSEL_EXT);
	Master_Adc_Config.STATUS3 = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED |
								ADC_SC3_AVGS(AVGS_4);
	Master_Adc_Config.STATUS1A = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(0);

	Master_Adc_Config.STATUS1B = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(18);	/* ADC1_DM1 */

	/* config the ADC again to desired conditions */
	ADC_Config_Alt(ADC1_BASE_PTR, &Master_Adc_Config);

	/* ADC1 using the PDB trigger in ping pong */

	/*
	 * The potentiometer is wired to channel 21 on ADC1.  That is the one used
	 * to calculate the change of the potentiometer below.
	 */

	printf("Running ADC1 HARDWARE TRIGGER by PDB\n");
	printf("- ADC1 A,B is the POT. Vary the POT setting.\n");
	printf("- Press SW3 to exit\n");

	/* Enable the ADC and PDB interrupts in NVIC */
	enable_irq(INT_ADC1 - 16);	/* ready for this interrupt. */
	enable_irq(INT_PDB0 - 16);	/* ready for this interrupt. */

	cycle_flags = 0;
	PDB0_SC |= PDB_SC_SWTRIG_MASK;	/* kick off the PDB  - just once */

	/*
	 * The system is now working!!!!  The PDB is *continuously* triggering ADC
	 * conversions.  Now, to display the results!  The line above was the
	 * SOFTWARE TRIGGER...
	 */

	/*
	 * The demo will continue as long as no character
	 * is pressed on the terminal.
	 */

	/* as long as no operater intervention, keep running this: */
	while (1) {
		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;		/* clear for next use */
			/* if SW3 (PTA4) pressed..., exit this demo */
			break;
		}

		while (cycle_flags != (ADC1A_DONE | ADC1B_DONE));	/* wait for one complete cycle */
		printf("- R0A=%6d  R0B=%6d   POT=%6d\r",
		       result0A, result0B,
		       exponentially_filtered_result1);
	}

	/* disable the PDB */

	PDB0_SC = 0;

	/* Disable the ADC and PDB interrupts in NVIC */
	disable_irq(INT_ADC1 - 16);	/* through with this interrupt. */
	disable_irq(INT_PDB0 - 16);	/* through with this interrupt. */

	printf("\nExit ADC1 Demo\n\n");

	return 0;
}

/*
 * pdb_isr(void)
 *
 * use to signal PDB counter has restarted counting
 *
 * In:  n/a
 * Out: n/a
 */
void pdb_isr(void)
{
	LED0_TOGGLE		/* do this asap - show start of PDB cycle */
	if (PDB0_SC & PDB_SC_PDBIF_MASK) {
		PDB0_SC &= ~PDB_SC_PDBIF_MASK;	/* clear interrupt mask */
	}
	LED1_LOW

	cycle_flags = 0;

	return;
}

/*
 * adc1_isr(void)
 *
 * use to signal ADC1 end of conversion
 * In:  n/a
 * Out: exponentially filtered potentiometer reading!
 * The ADC1 is used to sample the potentiometer on the A side and the B side:
 * ping-pong.  That reading is filtered for an agregate of ADC1 readings:
 * exponentially_filtered_result1 thus the filtered POT output is available
 * for display.
 */
void adc1_isr(void)
{
	/* check which of the two conversions just triggered */
	if ((ADC1_SC1A & ADC_SC1_COCO_MASK) == ADC_SC1_COCO_MASK) {
		LED1_HIGH	/* do this asap */
		result0A = ADC1_RA;		/* this will clear the COCO bit that is also
								 * the interrupt flag */
		/*
		 * Begin exponential filter code for Potentiometer setting for
		 * demonstration of filter effect
		 */
		exponentially_filtered_result1 += result0A;
		exponentially_filtered_result1 /= 2;
		/*
		 * Spikes are attenuated 6dB, 12dB, 24dB, .. and so on untill they
		 * die out. End exponential filter code..  add f*sample, divide by
		 * (f+1).. f is 1 for this case.
		 */
		cycle_flags |= ADC1A_DONE;	/* mark this step done */
	} else if ((ADC1_SC1B & ADC_SC1_COCO_MASK) == ADC_SC1_COCO_MASK) {
		LED1_LOW
		result0B = ADC1_RB;
		/*
		 * Begin exponential filter code for Potentiometer setting for
		 * demonstration of filter effect
		 */
		exponentially_filtered_result1 += result0B;
		exponentially_filtered_result1 /= 2;
		/*
		 * Spikes are attenuated 6dB, 12dB, 24dB, .. and so on untill they
		 * die out. End exponential filter code..  add f*sample, divide by
		 * (f+1).. f is 1 for this case.
		 */

		cycle_flags |= ADC1B_DONE;
	}

	if (exponentially_filtered_result1 > 32768) {
		LED3_LOW;
	} else {
		LED3_HIGH;
	}

	return;
}
