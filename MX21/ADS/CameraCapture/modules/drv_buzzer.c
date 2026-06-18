/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_buzzer.c
 *    Description : Buzzer functions
 *
 *    History :
 *    1. Date        : 14 May, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "drv_buzzer.h"

volatile Int32U Duration;
Int16U Tone;

/*************************************************************************
 * Function Name: Pwm_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PWM interrupt handler
 *
 *************************************************************************/
void Pwm_Handler (void)
{
  if(Duration == 1)
  {
    PWMC_bit.IRQEN = 0;
    PWMS = 0;
     --Duration;
     return;
  }
  while(PWMC_bit.FIFOAV)
  {
    if(Duration == 1)
    {
      break;
    }
    --Duration;
    PWMS = Tone;
  }
}

/*************************************************************************
 * Function Name: BuzzerInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init PWM
 *
 *************************************************************************/
void BuzzerInit (void)
{
  // Init
  Duration = 0; Tone = 0;
  // clocking PWM module
  PCCR1_bit.PWM_EN=1;
  // Assign PWM pin (by default is GPIO PE5)
  PTE_GIUS_bit.PIN5 = 0;PTE_GPR_bit.PIN5 = 0;
  // reset
  PWMC_bit.SWR = 1;
  while(PWMC_bit.SWR);
  PWMC = 0xC;
  PWMS = 0;
  // Init Interrupt handler
  AITC_SetupIntr(Pwm_Handler,FALSE,INT_PWM,0);
  AITC_EnableIntSource(INT_PWM);
  // Enable PWM
  PWMC_bit.EN = 1;
}

/*************************************************************************
 * Function Name: Beep
 * Parameters: Int32 ToneType
 *
 * Return: Boolean TRUE - tone generation start
 *                 FALSE - error
 *
 * Description: Make Beep
 *
 *************************************************************************/
Boolean Beep (Int32U ToneType)
{
  if(Duration)
  {
    return(FALSE);
  }
  switch(ToneType)
  {
  case 0: // Short High freq
    Duration = (((3UL KHZ)/16UL)/10UL)+1UL;
    Tone = FCLK/(PERDIV1*2*3KHZ);
    break;
  case 1: // Long High freq
    Duration = (((3UL KHZ)/16UL)*1UL)+1UL;
    Tone = FCLK/(PERDIV1*2*3KHZ);
    break;
  case 2: // Short Low freq
    Duration = (((1UL KHZ)/16UL)/10UL)+1UL;
    Tone = FCLK/(PERDIV1*2*1KHZ);
    break;
  case 3: // Long Low freq
    Duration = (((1UL KHZ)/16UL) * 1UL)+1UL;
    Tone = FCLK/(PERDIV1*2*1KHZ);
    break;
  default:
    return(FALSE);
  }
  PWMP = Tone;
  Tone >>= 1;
  PWMS = Tone;
  __no_operation();
  __no_operation();
  __no_operation();
  while(PWMC_bit.FIFOAV)
  {
    if(Duration == 1)
    {
      break;
    }
    --Duration;
    PWMS = Tone;
  }
  PWMC_bit.IRQEN = 1;
  return(TRUE);
}
