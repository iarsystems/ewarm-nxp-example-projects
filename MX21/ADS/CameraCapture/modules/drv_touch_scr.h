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
 *    1. Date        : 14 May, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __DRV_TOUCH_SCR_H
#define __DRV_TOUCH_SCR_H

#define AD7873_ID         0x800
#define AD7873_READ_ID    0xE0
#define AD7873_READ_X     0x90
#define AD7873_READ_Y     0xD0

#define MEASURE_REPEAT    4
#define ACQUIRE_DLY       100   // [ms]

// display upper left corner
#define LEFT_UP_X         250
#define LEFT_UP_Y         210

// display down right corner
#define RIGHT_DOWN_X      3850
#define RIGHT_DOWN_Y      3850

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
void Gpt2_Handler (void);

/*************************************************************************
 * Function Name: Touch_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen interrupt handler
 *
 *************************************************************************/
void Touch_Handler (void);

/*************************************************************************
 * Function Name: AD7873_Transfer
 * Parameters: Int32U Data
 *
 * Return: Int32U
 *
 * Description: Send a data by SPI1 and return the received data from SPI1
 *
 *************************************************************************/
Int32U AD7873_Transfer (Int32U Data);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: Int32U 1 - pass
 *                0 - fault
 *
 * Description: Init SPI, Ad7873 and PE10 intr by falling edge
 *
 *************************************************************************/
Int32U TouchScrInit (void);


/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData -  X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData);

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

#endif // __DRV_TOUCH_SCR_H
