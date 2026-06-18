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

#ifndef LOWPOWERAPPSTATES_H_
#define LOWPOWERAPPSTATES_H_

/* Include files */
#include "common.h"
#include "lptmr.h"
#include "uart.h"
#include "gpio.h"
#include "i2c.h"
#include "lptmr.h"
#include "adc16.h"
#include "smc.h"

/*
 * Definition of module wide MACROs / #DEFINE-CONSTANTs
 * - NOT for use in other modules
 */

#define ACC					(1)
#define POT					(0)

#define ADC_POT_CHANNEL		(18)	/* ADC1_SE18 single - dedicated */
#define TWO_VOLTS_VALUE		(2)

#define ACCEL_SAMPLING_TIMES	(0)
#define ALARM_DELAY				(10)	/* 1 sec */

/* (!(resultz < -45 && resultz > -85)) */
#define HORIZONTAL_POSITION		(!(result[4] > 45 && result[4] < 85))
#define CLK_OUT_DISABLE			PORTC_PCR3 = ( PORT_PCR_MUX(0));

#define LPTMR_USE_IRCLK 0
#define LPTMR_USE_LPOCLK 1
#define LPTMR_USE_ERCLK32 2
#define LPTMR_USE_OSCERCLK 3

/* Declaration of module wide TYPEs - NOT for use in other modules */

/* State Machine */
typedef struct
{
	uint8 ActualState;
	uint8 PrevState;
	uint8 NextState;
	uint8 ErrorState;
} sSM;

enum
{
	LP_DEMO_IDLE_STATE = 0,
	LP_DEMO_READ_STATE,
	LP_DEMO_CHECK_VALUE_STATE,
	LP_DEMO_GO_LOW_POWER_STATE,
	LP_DEMO_ALARM_STATE,
};

/* Definition of module wide EXTERN VARIABLEs - NOT for use in other modules */
extern uint8 gu8AppSelected;
extern sSM sMLowPowerDemoState;
extern void (*const vpfnLowPowerDemoStateMachine[]) (void);

/*
 * Declaration of module wide FUNCTIONs PROTOTYPE
 * - NOT for use in other modules
 */
void vfnAccelConfig(void);
void vfnMCGandUARTConfig(void);
void vfnAccelRead(void);
void vfnLPTMRConfig(void);
void vfnLowPowerConfig(void);
void vfnEnableClockGates(void);
void vfnADCConfig(void);
uint16 u16fnADCRead(uint8 channel);

#endif		/* LOWPOWERAPPSTATES_H_ */
