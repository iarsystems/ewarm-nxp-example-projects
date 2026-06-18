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
#include <NXP\iolpc2103.h>
#include "main.h"
#include "PWM.h"
#include "interrupts.h"
#include "Sin_Table.h"
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
void PWM_ISR(void);
/** public data **/

/** private data **/
static unsigned int MatchA, MatchB, MatchC;
static unsigned short SinePhase, SineFreq;
static unsigned char SineLevel;
/** public functions **/
/*************************************************************************
 * Function Name: PWM_Init
 * Parameters:None
 *
 * Return:None
 *
 * Description:
 *
 *************************************************************************/
void PWM_Init (void)
{
  T2PR = TMR2_PRESCALE;           //Prescaler for TMR2
  PINSEL0_bit.P0_7 = 2;           //P0_7 PWM Output
  PINSEL0_bit.P0_8 = 2;           //P0_8 PWM output
  PINSEL0_bit.P0_9 = 2;           //P0_8 PWM output
  T2PWMCON_bit.PWM0ENA = 1;       //MAT2.0 Set to PWM mode
  T2PWMCON_bit.PWM1ENA = 1;       //MAT2.1 Set to PWM mode
  T2PWMCON_bit.PWM2ENA = 1;       //MAT2.2 Set to PWM mode
  T2MR3 = PWM_PERIOD;             //~20kHz PWM freq
  T2MCR = (3<<9);                 //Interrupt and reset on match 3
  //Install_IRQ(VIC_TIMER2, PWM_ISR, 0); //Install Interrupt
  Install_FIQ(VIC_TIMER2,PWM_ISR);        //Install Fast Interrupt
  SineFreq = _100Hz;
  SineLevel = 255;
  T2TCR = 1;     //Enable Timer
}

/*************************************************************************
 * Function Name: SetSineLevel
 * Parameters:Level - Desired Sine Level from 0 to 255
 *
 * Return:None
 *
 * Description:
 *
 *************************************************************************/
void SetSineLevel(unsigned int Level)
{
  SineLevel = Level;
}
/*************************************************************************
 * Function Name: SetSineFreq
 * Parameters:Freq - Desired Sine Frequency in Hz
 *
 * Return:None
 *
 * Description:
 *
 *************************************************************************/
void SetSineFreq(unsigned int Freq)
{
  SineFreq = ((Freq<<16)/PWM_FREQ);
}
/*************************************************************************
 * Function Name: GetSineFreq
 * Parameters:None
 *
 * Return:Current Sine Frequency in Hz
 *
 * Description:
 *
 *************************************************************************/
int GetSineFreq(void)
{
int Freq = (SineFreq*PWM_FREQ); 
  if((Freq & 0xFFFF)>32768) Freq +=65536;
  return (Freq>>16);
}

int GetSineLevel(void)
{
  return SineLevel;
}

/*************************************************************************
 * Function Name: PWM_ISR
 * Parameters:None
 *
 * Return:None
 *
 * Description:
 *
 *************************************************************************/
//Define user section 
//Used to put the PWM_ISR in the RAM for faster execution
#pragma section =".fiqisr"        
void PWM_ISR(void) @ ".fiqisr"
{
unsigned short reg;
  //Clear Interrupt flag
  T2IR = (1<<3);
  //Load 
  T2MR0 = MatchA ;
  T2MR1 = MatchB ;
  T2MR2 = MatchC ;
   
  //Next Phase
  SinePhase += SineFreq;
  //
  reg = SinePhase;
  //Read Value from the Sine Table. Use the 10 MS bits of the SinePhase. Mul with Sine Level
  //Load result in the Match Register
  MatchA = PWM_PERIOD - 1024 + ((Sin_Table[reg>>6]*SineLevel)>>8);  
  reg += 65536/3;    //+120°
  MatchB = PWM_PERIOD - 1024 + ((Sin_Table[reg>>6]*SineLevel)>>8);  
  reg += 65536/3;    //+120°
  MatchC = PWM_PERIOD - 1024 + ((Sin_Table[reg>>6]*SineLevel)>>8); 
}
/** private functions **/
