/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : temperature.h
 *    Description : Temperature conversion module include file
 *
 *    History :
 *    1. Date        : July 25, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef  __TEMP_CONV_H
#define  __TEMP_CONV_H
#include "includes.h"

#define KELVIN_TO_CELSIUS_OFFSET      (Flo32)(-273.15)
#define KELVIN_TO_FAHRENHEIT_OFFSET   (Flo32)(-459.67)
#define CELSIUS_TO_FAHRENHEIT_OFFSET  (Flo32)(32)
#define GRADUS_TO_FAHRENHEIT_GRADUS   (Flo32)(1.8)

typedef enum
{
  KELVIN = 0,CELSIUS,FAHRENHEIT
} tTemperatureUnits;

Flo32 TempConverter(Flo32 Temperature,tTemperatureUnits CurrUnit , tTemperatureUnits Unit);

#endif  /* __TEMP_CONV_H */
