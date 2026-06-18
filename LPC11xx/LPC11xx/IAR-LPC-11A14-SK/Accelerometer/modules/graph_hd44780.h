/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : graph_hd44780.h
 *    Description : HD44780 pseudo-graphic library
 *                  header file
 *    History :
 *    1. Date        : September 22, 2011 
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *
 *    $Revision: 4271 $
 **************************************************************************/
#ifndef __GRAPH_HD44780_H
#define __GRAPH_HD44780_H

/** include files **/
#include <stdint.h>

/** definitions **/
#define IMAGE_WIDTH     3
#define IMAGE_HEIGHT    3 
#define CHAR_WIDTH      6
#define CHAR_HEIGHT     8
#define CHAR_COUNT      8

/** public data **/ 

/** public functions **/

/*************************************************************************
 * Function Name: HD44780_DrawXY
 * Parameters: Int8U X - x coordinate, Int8U Y - y coordinate
 *
 * Return: none
 *
 * Description: Draws previously defined pseudo image, at text LCD display.
 *
 *************************************************************************/
void HD44780_DrawXY(Int8U X, Int8U Y);

#endif
