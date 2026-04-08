/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : temperature.c
 *    Description : Temperature conversion module
 *
 *    History :
 *    1. Date        : July 25, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "temperature.h"

/*************************************************************************
 * Function Name: TempConverter
 * Parameters: Flo32 Temperature,
 *             tTemperatureUnits CurrUnit ,
 *             tTemperatureUnits Unit
 * Return: Float
 * Description: Convert the temperature form "CurrUnit" to "Unit" units
 *
 *************************************************************************/
Flo32 TempConverter(Flo32 Temperature,tTemperatureUnits CurrUnit , tTemperatureUnits Unit)
{
  if(CurrUnit != Unit)
  {
    switch(CurrUnit)
    {
    case KELVIN:
      if(Unit == CELSIUS)
      {
        Temperature += KELVIN_TO_CELSIUS_OFFSET;
      }
      else
      {
        Temperature *= GRADUS_TO_FAHRENHEIT_GRADUS;
        Temperature += KELVIN_TO_FAHRENHEIT_OFFSET;
      }
      break;
    case CELSIUS:
      if(Unit == KELVIN)
      {
        Temperature -= KELVIN_TO_CELSIUS_OFFSET;
      }
      else
      {
        Temperature *= GRADUS_TO_FAHRENHEIT_GRADUS;
        Temperature += CELSIUS_TO_FAHRENHEIT_OFFSET;
      }
      break;
    case FAHRENHEIT:
      if(Unit == KELVIN)
      {
        Temperature -= KELVIN_TO_FAHRENHEIT_OFFSET;
        Temperature /= GRADUS_TO_FAHRENHEIT_GRADUS;
      }
      else
      {
        Temperature -= CELSIUS_TO_FAHRENHEIT_OFFSET;
        Temperature /= GRADUS_TO_FAHRENHEIT_GRADUS;
      }
      break;
    }
  }
  return(Temperature);
}
