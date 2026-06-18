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
 * Basic function the LPTMR will wake up the MCU every 100ms depending on the
 * selected application the Potentiometer or the Accelerometer will be read.
 *
 * In the case of the potentiometer the MCU must go back to sleep if the read
 * voltage is under 2v. For the accelerometer the MCU goes back to sleep while
 * the tower board is on vertical position (normal tower position). If those
 * conditions are not fulfilled a message must be send thru UART every second
 * and LED D8 on the tower must be toggling.
 */

/* Include files */
#include "common.h"
#include "uart.h"
#include "LowPowerAppStates.h"

void main(void)
{
	uint8 u8AppSelect;

	CLK_OUT_DISABLE;	/* Disable Clock out to reduce consumption */

	printf("\nPlease Select the number for the component application you\n"
		   "want to run. Use reset button to back this menu.\n"
		   "Change terminal baudrate to 9600 and choose one of the selection\n"
		   "from below\n\n");
	printf(" 1. Accelerometer\n"
		   "    - In this application the MCU will be sleep while the board\n"
		   "      is on vertical position if not a message will be send\n"
		   "      thru UART and LED D8 will toggle\n");
	printf(" 2. Potentiometer\n"
		   "    - In this application the MCU will be sleep while the\n"
		   "      Potentiometer will be under 2v if not a message will be\n"
		   "      send thru UART and LED D8 will toggle\n\n");
	printf("Select:\n\n");

	vfnMCGandUARTConfig();
	vfnEnableClockGates();
	vfnLowPowerConfig();
	vfnLEDsOutPutConfig();

	u8AppSelect = uart_getchar(TERM_PORT);
	printf("Revert the Terminal baudrate back to 115200 before issuing a reset\n");
	if (u8AppSelect == '1') {
		gu8AppSelected = ACC;
		printf("Accelerometer application starts. Move tower to horizontal\n"
			   "position to wake up the MCU\n");
	} else {
		gu8AppSelected = POT;
		printf("Potentiometer application starts. Increment potentiometer\n"
			   "value to wake up the MCU\n");
	}

	/* Depending on the chosen component configure the control modules */
	if (gu8AppSelected)
		vfnAccelConfig();
	else
		vfnADCConfig();

	vfnLPTMRConfig();

	/* Initialize machine state */
	sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;

	while (1) {
		/* Call to machine state */
		vpfnLowPowerDemoStateMachine[sMLowPowerDemoState.ActualState] ();
	}
}
