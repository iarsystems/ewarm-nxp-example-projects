/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name      : buttons.c
 *    Description    : Buttons module
 *
 *    History :
 *    1. Date        : January 11, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "buttons.h"

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void)
{
  // Enable Fast GPIO0,1
  SCS_bit.GPIOM = 1;
  // set to inputs the buttons' pins
  B1_FDIR &= ~B1_MASK;
  B2_FDIR &= ~B2_MASK;
  JS_FDIR &= ~(JS_RIGHT_MASK | JS_LEFT_MASK | JS_UP_MASK |\
               JS_DOWN_MASK | JS_CENTER_MASK);
}

/*************************************************************************
 * Function Name: GetButtons
 * Parameters: none
 * Return: Buttons_t
 * Description: Return current buttons states
 *
 *************************************************************************/
Buttons_t GetButtons (void)
{
Buttons_t Buttons = {0};
  if(!(B1_FIO & B1_MASK))
  {
    Buttons.B1 = 1;
  }
  if(!(B2_FIO & B2_MASK))
  {
    Buttons.B2 = 1;
  }
  if(!(JS_FIO & JS_RIGHT_MASK))
  {
    Buttons.JsRight = 1;
  }
  if(!(JS_FIO & JS_LEFT_MASK))
  {
    Buttons.JsLeft = 1;
  }
  if(!(JS_FIO & JS_UP_MASK))
  {
    Buttons.JsUp = 1;
  }
  if(!(JS_FIO & JS_DOWN_MASK))
  {
    Buttons.JsDown = 1;
  }
  if(!(JS_FIO & JS_CENTER_MASK))
  {
    Buttons.JsCenter = 1;
  }
  return(Buttons);
}
