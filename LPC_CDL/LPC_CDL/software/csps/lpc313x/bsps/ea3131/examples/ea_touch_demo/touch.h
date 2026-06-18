/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 * Description:
 *    Touch sensor.
 *
 *****************************************************************************/

#ifndef _TOUCH__h
#define _TOUCH__h

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "lpc_types.h"

typedef struct
{
  UNS_16 x;
  UNS_16 y;
} tTouchPoint;


/******************************************************************************
 * Public variables
 *****************************************************************************/


/******************************************************************************
 * Public functions
 *****************************************************************************/

void touch_xyz(INT_32 *x, INT_32* y, INT_32* z);
UNS_32 touch_temp(void);
void touch_init(void);
void touch_calibrate(tTouchPoint ref1, tTouchPoint ref2, tTouchPoint ref3, 
  tTouchPoint scr1, tTouchPoint scr2, tTouchPoint scr3);


#endif
