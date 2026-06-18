/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : PWM.c
 *    Description : 
 *
 *    History :
 *    1. Date        : 28.3.2008 
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
/**definitions **/
//PWM prescaler values
#define TMR2_PRESCALE 0                
//PWM Clock
#define TMR2_CLK PCLK/(TMR2_PRESCALE+1)
//PWM Frequency
#define PWM_FREQ 20000UL  //20kHz
//PWM PERIOD
#define PWM_PERIOD  TMR2_CLK/PWM_FREQ
//Sine Frequecy definitions
#define _10Hz   ((10*65536)/PWM_FREQ)
#define _50Hz   ((50*65536)/PWM_FREQ)
#define _100Hz  ((100*65536)/PWM_FREQ)
#define _250Hz  ((250*65536)/PWM_FREQ)
/** default settings **/

/** public data **/

/** public functions **/
extern void PWM_Init (void);
extern void SetSineFreq(unsigned int Freq);
extern int GetSineFreq(void);
