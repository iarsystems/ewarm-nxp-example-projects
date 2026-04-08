/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_touch_scr.h
 *    Description : Touch screen driver include file
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#ifndef __DRV_TOUCH_SCR_H
#define __DRV_TOUCH_SCR_H

#include "arm_comm.h"

#define IMAGE_WIDTH         (480UL)
#define IMAGE_HEIGHT        (640UL)
#define IMAGE_BPP           (16UL)

// Touched to untouched time out
#define TS_MIN_PRESSURE   200

// display X axis corner
#define LEFT_X            2850
#define RIGHT_X           250

// display Y axis corner
#define UP_Y              180
#define DOWN_Y            2950

typedef union
{
  Int32U XY;
  struct
  {
    Int16U  X;
    Int16U  Y;
  };
} ToushRes_t;

/*************************************************************************
 * Function Name: Gpt2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
void Gpt_Handler (void);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void);

/*************************************************************************
 * Function Name: TouchGet
 * Parameters: ToushRes_t * pData -  X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData);

#endif // __DRV_TOUCH_SCR_H
