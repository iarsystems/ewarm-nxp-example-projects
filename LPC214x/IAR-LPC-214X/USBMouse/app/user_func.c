/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   	 : user_func.c
 *    Description    : Define user module
 *
 *    History :
 *    1. Date        : June 13, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/

#include "user_func.h"

LPC_INT8U ButtCount1,ButtCount2,ButtFlags;
int TickSysFlag;

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void)
{
  /* Init buttons Inputs */
  IO0DIR_bit.P0_15 = IO0DIR_bit.P0_16 = 0;
  /* Init button's variables */
  ButtFlags = ButtCount1 = ButtCount2 = 0;
}

/*************************************************************************
 * Function Name: Buttons
 * Parameters: none
 * Return: none
 * Description: Imlement buttons
 *
 *************************************************************************/
void Buttons (void)
{
  if (!IO0PIN_bit.P0_15)
  {
    if(ButtCount1 > BUTT_DEB)
    {
      if((ButtFlags & BUTT_FLAG_1) == 0)
      {
        ButtFlags |= BUTT_FLAG_1+BUTT_EVENT_1;
      }
    }
    else
    {
      ++ButtCount1;
    }
  }
  else
  {
    if(ButtCount1 == 0)
    {
      if(ButtFlags & BUTT_FLAG_1)
      {
        ButtFlags &= ~(BUTT_FLAG_1+BUTT_EVENT_1);
      }
    }
    else
    {
      --ButtCount1;
    }
  }
  if (!IO0PIN_bit.P0_16)
  {
    if(ButtCount2 > BUTT_DEB)
    {
      if((ButtFlags & BUTT_FLAG_2) == 0)
      {
        ButtFlags |= BUTT_FLAG_2+BUTT_EVENT_2;
      }
    }
    else
    {
      ++ButtCount2;
    }
  }
  else
  {
    if(ButtCount2 == 0)
    {
      if(ButtFlags & BUTT_FLAG_2)
      {
        ButtFlags &= ~(BUTT_FLAG_2+BUTT_EVENT_2);
      }
    }
    else
    {
      --ButtCount2;
    }
  }
}

/*************************************************************************
 * Function Name: GetButtonsEvent
 * Parameters: none
 * Return: LPC_INT8U
 * Description: Return buttons event
 *
 *************************************************************************/
LPC_INT8U GetButtonsEvent (void)
{
LPC_INT8U Events = ButtFlags & (BUTT_EVENT_1|BUTT_EVENT_2);
  return Events;
}

/*************************************************************************
 * Function Name: SetSysTickFlag
 * Parameters: void* arg
 * Return: none
 *
 * Description: Set system tick flag and scan buttons
 *
 *************************************************************************/
void SetSysTickFlag (void* arg)
{
int* pFlag = (int *)arg;
  *pFlag = 1;
  Buttons();
}

/*************************************************************************
 * Function Name: ADC_Init
 * Parameters: none
 * Return: none
 * Description: Init ADC module
 *
 *************************************************************************/
void ADC_Init (void)
{
  /* ADC Channel 3 is connected to pin P0_30 */
  PINSEL1_bit.P0_30 = 1;
  /* ADC Settings */
  AD0CR_bit.SEL = 1<<3;
  AD0CR_bit.CLKDIV = SYS_GetFpclk()/MAX_ADC_FREQ;
  AD0CR_bit.BURST = 0;
  AD0CR_bit.PDN = 1;
  AD0CR_bit.START = 0;
}

/*************************************************************************
 * Function Name: ADC_Measure
 * Parameters: none
 * Return: unsigned int
 * Description: Measure voltage on ADC Channel 3
 *
 *************************************************************************/
unsigned int ADC_Measure (void)
{
  /* ADC Conversion start */
  AD0CR_bit.START = 1;
  for(volatile Int32U i = 100; i; i--);
  /* Wait result */
  while((AD0DR&0x80000000) == 0);
  return(AD0DR_bit.VVDDA);
}
