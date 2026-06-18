/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_hd44780_l.c
 *    Description : I/O function
 *
 *    History :
 *    1. Date        : 13, September 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "drv_hd44780_l.h"

/*************************************************************************
 * Function Name: HD44780_IO_Init
 * Parameters: none
 * Return: none
 * Description: Init IO ports directions and level
 *
 *************************************************************************/
void HD44780_IO_Init (void)
{
  // Init LCD IO lines
  // LCD Backlight - Low
  LCD_BL_FDIR   |= LCD_BL_MASK;
  // E  Output - Low
  LCD_E_FDIR    |= LCD_E_MASK;
  HD44780SetE(0);
  // RS Output - Low
  LCD_RS_FDIR   |= LCD_RS_MASK;
  HD44780SetRS(0);
#if HD4780_WR > 0
  // WR Output - Low
  LCD_RW_FDIR   |= LCD_RW_MASK;
  HD44780SetRW(0);
#endif

  // Data bits - inputs
  LCD_DATA_FDIR &= ~LCD_DATA_MASK;

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
  if(Data)
  {
    LCD_RS_FSET = LCD_RS_MASK;
  }
  else
  {
    LCD_RS_FCLR = LCD_RS_MASK;
  }
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
  if(Data)
  {
    LCD_E_FSET = LCD_E_MASK;
  }
  else
  {
    LCD_E_FCLR = LCD_E_MASK;
  }
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
Int32U Tmp = (Int32U)Data << LCD_DATA_SHIFT;
  // Set Direction
  LCD_DATA_FDIR |= LCD_DATA_MASK;
#if HD4780_WR > 0
  HD44780SetRW(0);
#endif
  // Write Data
  // Clock E
  HD44780SetE(1);

  LCD_DATA_FSET =    Tmp & LCD_DATA_MASK;
  LCD_DATA_FCLR =  (~Tmp) & LCD_DATA_MASK;
  HD44780_BUS_DLY();
  HD44780SetE(0);
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780SetRW
 * Parameters: Boolean Data
 * Return: none
 * Description: Set R/W signal
 *
 *************************************************************************/
void HD44780SetRW (Boolean Data)
{
  if(Data)
  {
    LCD_RW_FSET = LCD_RW_MASK;
  }
  else
  {
    LCD_RW_FCLR = LCD_RW_MASK;
  }
}

/*************************************************************************
 * Function Name: HD44780RdIO
 * Parameters: none
 * Return: Int8U
 * Description: Read from HD44780 I/O
 *
 *************************************************************************/
Int8U HD44780RdIO (void)
{
Int8U Data;
  // Set Direction
  LCD_DATA_FDIR &= ~LCD_DATA_MASK;
  HD44780SetRW(1);
  // Read Data
  HD44780SetE(1);
  HD44780_BUS_DLY();
  Data = (LCD_DATA_FIO & LCD_DATA_MASK) >> LCD_DATA_SHIFT;
  HD44780SetE(0);
  // Clock E
  return Data;
}
#endif
