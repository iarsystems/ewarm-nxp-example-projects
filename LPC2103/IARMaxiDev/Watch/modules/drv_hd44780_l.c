/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_hd44780_l.c
 *    Description : I/O function
 *
 *    History :
 *    1. Date        : 4, January 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "drv_hd44780_l.h"

#define SHR_DATA  24
#define SHR_CLK   25
#define SHR_LACH  26

typedef union
{
  Int8U Data;
  struct
  {
    Int8U Light : 1;
    Int8U RW    : 1;
    Int8U RS    : 1;
    Int8U E     : 1;
    Int8U DB    : 4;
  };
} bmShiftData_t;

bmShiftData_t bmShiftData;

/*************************************************************************
 * Function Name: ShiftDataRefresh
 * Parameters: none
 *
 * Return: none
 *
 * Description: Shift bmShiftData
 *
 *************************************************************************/
void ShiftDataRefresh(void)
{
  // Set Fast mask
  FIOMASK &= ~((1 << SHR_DATA) | (1 << SHR_CLK) | (1 << SHR_LACH));
  // shift data to LCD
  for (Int32U i = 0x80; i; i >>= 1)
  {
    FIOCLR = (1 << SHR_CLK) | (1 << SHR_LACH);
    if(bmShiftData.Data & i)
    {
      FIOSET = 1 << SHR_DATA;
    }
    else
    {
      FIOCLR = 1 << SHR_DATA;
    }
    FIOSET = 1 << SHR_CLK;
  }
  FIOCLR = 1 << SHR_CLK;
  FIOSET = 1 << SHR_LACH;
  FIOCLR = 1 << SHR_LACH;
}

/*************************************************************************
 * Function Name: HD44780_IO_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init GPIO and clear shift register
 *
 *************************************************************************/
void HD44780_IO_Init (void)
{
  // Set to GPIO
  PINSEL1 &= ~((3 << ((SHR_DATA - 16)*2))|
              (3 << ((SHR_CLK  - 16)*2)) |
              (3 << ((SHR_LACH - 16)*2)));
  // Enable high speed GPIO
  SCS_bit.GPIO0M = 1;
  // Set Fast mask
  FIOMASK &= ~((1 << SHR_DATA) | (1 << SHR_CLK) | (1 << SHR_LACH));
  // Set to outputs and clear
  FIODIR |= (1 << SHR_DATA) | (1 << SHR_CLK) | (1 << SHR_LACH);
  FIOCLR  = (1 << SHR_DATA) | (1 << SHR_CLK) | (1 << SHR_LACH);
  bmShiftData.Data = 0;
  ShiftDataRefresh();
}

/*************************************************************************
 * Function Name: HD44780SetRS
 * Parameters: Boolean Data
 * Return: none
 * Description: Set RS signal
 *
 *************************************************************************/
void HD44780SetRS (Boolean Data)
{
  bmShiftData.RS = Data;
  ShiftDataRefresh();
}

/*************************************************************************
 * Function Name: HD44780SetE
 * Parameters: Boolean Data
 * Return: none
 * Description: Set E signal
 *
 *************************************************************************/
void HD44780SetE (Boolean Data)
{
  bmShiftData.E = Data;
  ShiftDataRefresh();
}

/*************************************************************************
 * Function Name: HD44780WrIO
 * Parameters: Int8U Data
 * Return: none
 * Description: Write to HD44780 I/O
 *
 *************************************************************************/
void HD44780WrIO (Int8U Data)
{
  HD44780SetE(1);
  bmShiftData.DB = Data;
  ShiftDataRefresh();
  HD44780SetE(0);
}

/*************************************************************************
 * Function Name: HD44780SetBackLight
 * Parameters: Boolean Data
 * Return: none
 * Description: Set backlight
 *
 *************************************************************************/
void HD44780SetBackLight (Boolean Data)
{
  bmShiftData.Light = Data;
  ShiftDataRefresh();
}
