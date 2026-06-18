/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : graph_hd44780.c
 *    Description : HD44780 pseudo-graphic library
 *
 *    History :
 *    1. Date        : September 22, 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "drv_hd44780.h"
#include "drv_hd44780_cnfg.h"
#include "drv_hd44780_l.h"
#include "graph_hd44780.h"

/******** Custom pseudo-image ***********/
const Int8U CUSTOM_IMAGE[IMAGE_HEIGHT] = { 0x7, 0x7, 0x7 }; 

/*************************************************************************
 * Function Name: HD44780_DrawXY
 * Parameters: Int8U X - x coordinate, Int8U Y - y coordinate
 *
 * Return: none
 *
 * Description: Draws previously defined pseudo image, at text LCD display.
 *
 *************************************************************************/
void HD44780_DrawXY(Int8U X, Int8U Y)
{   
  typedef Int8S CharType[CHAR_HEIGHT];
  /* Text buffer used to store the image */
  static CharType CharBuffer[CHAR_COUNT];
  static char LCDBuffer[CHAR_COUNT+1];
  Int8U i;    
  Int8U posx = X/CHAR_WIDTH;
  Int8U dotx = X%CHAR_WIDTH;
  Int8S shift = CHAR_WIDTH - (dotx + IMAGE_WIDTH);
  /* Empty draw buffer */
  for(Int8U ax = 0; ax < CHAR_COUNT; ax++)
    for(Int8U ay = 0; ay < CHAR_HEIGHT; ay++)
      CharBuffer[ax][ay] = 0x00;

  /* Correct x axis (remove one empty row in the begining) */
  X=X+1;
  /* Check boundaries of display */
  if (Y > (CHAR_HEIGHT - IMAGE_HEIGHT)) Y = CHAR_HEIGHT - IMAGE_HEIGHT;  
  if (X > ((CHAR_COUNT*CHAR_WIDTH) - IMAGE_WIDTH)) X = (CHAR_COUNT*CHAR_WIDTH) - IMAGE_WIDTH;  
  /* Draw image in the buffer */
  if (shift>=0)
  { 
   for(i=0;i<IMAGE_HEIGHT;i++)
    CharBuffer[posx][Y+i] = CUSTOM_IMAGE[i] << shift;      
  }
  else
  {
   shift = -shift;
   for(i=0;i<IMAGE_HEIGHT;i++)
     CharBuffer[posx][Y+i]   = CUSTOM_IMAGE[i] >> shift;   
   
   if (posx < CHAR_COUNT-1)
   {
     shift = CHAR_WIDTH - shift;
     for(i=0;i<IMAGE_HEIGHT;i++)
       CharBuffer[posx+1][Y+i]   = CUSTOM_IMAGE[i] << shift;          
   }
  } 
  /* Write buffer content to CGRAM */
  HD44780_WrCGRAM(CharBuffer[0],1);  
  HD44780_WrCGRAM(CharBuffer[1],2);  
  HD44780_WrCGRAM(CharBuffer[2],3);  
  HD44780_WrCGRAM(CharBuffer[3],4);  
  HD44780_WrCGRAM(CharBuffer[4],5);  
  HD44780_WrCGRAM(CharBuffer[5],6);  
  HD44780_WrCGRAM(CharBuffer[6],7);  
  HD44780_WrCGRAM(CharBuffer[7],8);  
  /* Prepare a string which represents the image */
  for (posx = 1; posx < CHAR_COUNT+1; posx++) LCDBuffer[posx-1]=posx;
  LCDBuffer[posx]=0;
  /* Show this string (image) */
  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)LCDBuffer);
}
