/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_buzzer.c
 *    Description : Buzzer driver module
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "drv_buzzer.h"

volatile Int32U Duration;
Int16U Tone;

/*************************************************************************
 * Function Name: PWM_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PWM interrupt handler
 *
 *************************************************************************/
void PWM_Handler (void)
{
  PWMSR_bit.FE = 1;
  if(Duration == 1)
  {
    PWMIR_bit.FIE = 0;
    PWMSAR = 0;
     --Duration;
     return;
  }
  while(PWMSR_bit.FIFOAV < 4)
  {
    if(Duration == 1)
    {
      break;
    }
    --Duration;
    PWMSAR = Tone;
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
  Duration = 0; Tone =0;
  // clocking PWM module
  PCCR0_bit.PWM_EN = 1;
  PCCR0_bit.GPIO_EN = 1;
  // Assign PWM pin (by default is GPIO PE5)
  PTE_GIUS_bit.PIN5 = 0;PTE_GPR_bit.PIN5 = 0;
  // PWM reset
  PWMCR_bit.SWR = 1;
  while(PWMCR_bit.SWR);
  // set prescaler
  PWMCR_bit.PRESCALER = 1;
  // FIFO empty flag is set when there are more than
  // or equal to 3 empty slots in FIFO
  PWMCR_bit.FWM = 2;
  // Use each sample eight times
  PWMCR_bit.REPEAT = 3;
  // IP clock is selected
  PWMCR_bit.CLKSRC = 1;
  PWMSAR = 0;
  // Init Interrupt handler
  AITC_SetupIntr(PWM_Handler,FALSE,INT_PWM,1);
  AITC_EnableIntSource(INT_PWM);
  // Enable PWM
  PWMCR_bit.EN = 1;
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
    Duration = (Int32U)((3KHZ/8)  * 0.1)+1;
    Tone = FCLK/((PCDR1_bit.PERDIV1+1)*3KHZ*2);
    break;
  case 1: // Long High freq
    Duration = ((3KHZ/8)  * 1)+1;
    Tone = FCLK/((PCDR1_bit.PERDIV1+1)*3KHZ*2);
    break;
  case 2: // Short Low freq
    Duration = (Int32U)((1KHZ/8)  * 0.1)+1;
    Tone = FCLK/((PCDR1_bit.PERDIV1+1)*1KHZ*2);
    break;
  case 3: // Long Low freq
    Duration = ((1KHZ/8)  * 1)+1;
    Tone = FCLK/((PCDR1_bit.PERDIV1+1)*1KHZ*2);
    break;
  default:
    return(FALSE);
  }
  PWMPR = Tone;
  Tone >>= 1;
  PWMSAR = Tone;
  // FIFO Empty interrupt enable
  PWMIR_bit.FIE = 1;
  return(TRUE);
}
