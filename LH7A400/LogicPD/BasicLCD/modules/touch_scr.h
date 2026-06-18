/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : touch_scr.h
 *    Description : Prototypes for the touch screen driver
 *
 *    History :
 *    1. Date        : 5 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4250 $
 **************************************************************************/
#include "includes.h"

#ifndef __TOUCH_SCR_H
#define __TOUCH_SCR_H

#define MAX_DELAY                 0xFFFF
// Acquire delay [us]
#define TC_ACQUIRE_TIME           100
#define TC_PEN_INTR_ACQUIRE_TIME  10

#define AD7843_READ_X             0x90
#define AD7843_READ_Y             0xD0

// display upper left corner
#define LEFT_UP_X                 310
#define LEFT_UP_Y                 270

// display down right corner
#define RIGHT_UP_X                3676
#define RIGHT_UP_Y                3570
typedef enum
{
  TS_NotTouch = 0,
  TS_MesureX1, TS_MesureX2, TS_MesureX3, TS_MesureX4,
  TS_MesureY1, TS_MesureY2, TS_MesureY3, TS_MesureY4,
  TS_PenCheckLast,
} TouchScrState_t;

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
 * Function Name: AD7843_Transfer
 * Parameters: Int32U Data
 *
 * Return: Int32U
 *
 * Description: Send a data by CPLD SPI and return the received data
 *
 *************************************************************************/
Int32U ADS7843_Transfer(Int32U Data);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init SPI and External interrupt - LOW-level triggered
 *
 *************************************************************************/
void TouchScrInit (void);

/*************************************************************************
 * Function Name: TouchScrStartMesure
 * Parameters: none
 *
 * Return: none
 *
 * Description: Starting the measurement
 *
 *************************************************************************/
void TouchScrStartMesure (void);

/*************************************************************************
 * Function Name: TouchImp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Implement touch - adc result filtering and set touch flag
 *
 *************************************************************************/
void TouchImp (void);

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchScrGetStatus (ToushRes_t * pData);
#endif // __TOUCH_SCR_H
