/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name      : buttons.h
 *    Description    : Buttons include header
 *
 *    History :
 *    1. Date        : January 11, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "includes.h"

#ifndef  __BUTTONS_H
#define  __BUTTONS_H

typedef union _Buttons_t
{
  Int32U Data;
  struct
  {
    Int32U JsUp     : 1;
    Int32U JsDown   : 1;
    Int32U JsRight  : 1;
    Int32U JsLeft   : 1;
    Int32U JsCenter : 1;
    Int32U B1       : 1;
    Int32U B2       : 1;
    Int32U          :25;
  };
} Buttons_t, *pButtons_t;

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void);

/*************************************************************************
 * Function Name: GetButtons
 * Parameters: none
 * Return: Buttons_t
 * Description: Return current buttons states
 *
 *************************************************************************/
Buttons_t GetButtons (void);

#endif  /* __BUTTONS_H */
