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

// GPIO Port 2:
#define JS_DOWN_MASK    (1U<<4)
#define JS_RIGHT_MASK   (1U<<3)
#define JS_LEFT_MASK    (1U<<1)
#define JS_UP_MASK      (1U<<2)
#define JS_CENTER_MASK  (1U<<0)

#define JS_BTN_PRESSED(mask) (!(P2_INP_STATE & mask))

// PCA9532:
#define BTN_1 (1UL<<0)
#define BTN_2 (1UL<<1)
#define BTN_3 (1UL<<2)
#define BTN_4 (1UL<<3)



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
  // Configure the joystick pins as GPIO pins
  P2_MUX_SET_bit.EMC_D_SEL = 1;
  
  // Enable I2C1 clock
  clkpwr_clk_en_dis(CLKPWR_I2C1_CLK, 1);
  
  // Install I2C1 IRQ
  SIC1_IRQInstall(i2c1_user_interrupt,
                  Sub1I2C_1_INT,
                  HIGH,
                  LEVEL,
                  TYPE_IRQ);
  
  I2C_dev0 = pca9532_open((void *)I2C1, 0);
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

  // Read buttons connected to PCA9532 chip
  pca9532_ioctl(I2C_dev0, PCA9532_GET_LED, 0x00, (INT_32 *)&tmp);
  
  if(!(tmp & BTN_1))
  {
    Buttons.B1 = 1;
  }
  if(!(tmp & BTN_2))
  {
    Buttons.B2 = 1;
  }
  if(!(tmp & BTN_3))
  {
    Buttons.B3 = 1;
  }
  if(!(tmp & BTN_4))
  {
    Buttons.B4 = 1;
  }
  
  // Read buttons connected directly to GPIO pins
  if (JS_BTN_PRESSED(JS_RIGHT_MASK))
  {
    Buttons.JsRight = 1;
  }
  if(JS_BTN_PRESSED(JS_LEFT_MASK))
  {
    Buttons.JsLeft = 1;
  }
  if(JS_BTN_PRESSED(JS_UP_MASK))
  {
    Buttons.JsUp = 1;
  }
  if(JS_BTN_PRESSED(JS_DOWN_MASK))
  {
    Buttons.JsDown = 1;
  }
  if(JS_BTN_PRESSED(JS_CENTER_MASK))
  {
    Buttons.JsCenter = 1;
  }

  return(Buttons);
}
