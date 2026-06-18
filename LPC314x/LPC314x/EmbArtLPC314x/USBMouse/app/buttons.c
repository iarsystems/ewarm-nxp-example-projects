/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name      : buttons.c
 *    Description    : Buttons module
 *
 *    History :
 *    1. Date        : August 11, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "buttons.h"

#define JS_RIGHT_MASK   (1UL << 2)
#define JS_LEFT_MASK    (1UL << 3)
#define JS_UP_MASK      (1UL << 1)
#define JS_DOWN_MASK    (1UL << 4)
#define JS_CENTER_MASK  (1UL << 0)

Int32U I2C_dev0;

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void)
{
  // Enable I2C0 clock
  CGU_Run_Clock(I2C0_PCLK);
  // Install I2C0 IRQ
  INTC_IRQInstall(i2c0_user_interrupt,
                  I2C0_NINTR,
                  1,
                  0);
  I2C_dev0 = pca9532_open((void *)I2C0, 0);
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
Int32U tmp;

  pca9532_ioctl(I2C_dev0, PCA9532_GET_LED, 0x00, (INT_32 *)&tmp);

  if(!(tmp & JS_RIGHT_MASK))
  {
    Buttons.JsRight = 1;
  }
  if(!(tmp & JS_LEFT_MASK))
  {
    Buttons.JsLeft = 1;
  }
  if(!(tmp & JS_UP_MASK))
  {
    Buttons.JsUp = 1;
  }
  if(!(tmp & JS_DOWN_MASK))
  {
    Buttons.JsDown = 1;
  }
  if(!(tmp & JS_CENTER_MASK))
  {
    Buttons.JsCenter = 1;
  }
  return(Buttons);
}
