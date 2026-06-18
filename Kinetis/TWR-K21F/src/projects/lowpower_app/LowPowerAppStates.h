/*
 * LowPowerAppStates.h
 *
 *  Created on: Jul 31, 2012
 *      Author: B23243
 */

#ifndef LOWPOWERAPPSTATES_H_
#define LOWPOWERAPPSTATES_H_

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"
#include "lptmr.h"
#include "uart.h"
#include "gpio.h"
#include "i2c.h"
#include "lptmr.h"
#include "adc16.h"
#include "smc.h"

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

#define ACC  (1)
#define POT  (0)


#define ADC_POT_CHANNEL (12)  //ADC0_SE12  single

#define TWO_VOLTS_VALUE (2)  

#define ACCEL_SAMPLING_TIMES   (0)
#define ALARM_DELAY            (10)  //1 sec

#define HORIZONTAL_POSITION    (!(result[4] > 45 && result[4] < 85))  //(!(resultz < -45 && resultz > -85))

#define CLK_OUT_DISABLE        PORTC_PCR3 = ( PORT_PCR_MUX(0));	


#define LPTMR_USE_IRCLK 0 
#define LPTMR_USE_LPOCLK 1
#define LPTMR_USE_ERCLK32 2
#define LPTMR_USE_OSCERCLK 3

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/* State Machine */
  typedef struct
  {
    uint8 ActualState;
    uint8 PrevState;
    uint8 NextState;
    uint8 ErrorState;
  }sSM;
  

  enum
  {
  	LP_DEMO_IDLE_STATE = 0,
  	LP_DEMO_READ_STATE,
  	LP_DEMO_CHECK_VALUE_STATE,
  	LP_DEMO_GO_LOW_POWER_STATE,
  	LP_DEMO_ALARM_STATE,
  };  

/*****************************************************************************************************
* Definition of module wide EXTERN VARIABLEs - NOT for use in other modules
*****************************************************************************************************/

extern uint8 gu8AppSelected;
  
extern sSM sMLowPowerDemoState;  
  
extern void ( * const vpfnLowPowerDemoStateMachine[])(void);
/*****************************************************************************************************
* Declaration of module wide FUNCTIONs PROTOTYPE - NOT for use in other modules
*****************************************************************************************************/

void vfnAccelConfig(void);
void vfnMCGandUARTConfig(void);
void vfnAccelRead(void);
void vfnLPTMRConfig(void);
void vfnLowPowerConfig(void);
void vfnEnableClockGates(void);
void vfnADCConfig(void);
uint16 u16fnADCRead(uint8 channel);


#endif /* LOWPOWERAPPSTATES_H_ */
