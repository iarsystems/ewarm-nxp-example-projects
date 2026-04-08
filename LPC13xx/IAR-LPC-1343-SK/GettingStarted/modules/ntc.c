/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ntc.c
 *    Description : NTC termistor module
 *
 *    History :
 *    1. Date        : July 28, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "ntc.h"

// NTC device depends parameters
#define A   (Flo64)-14.6337
#define B   (Flo64) 4791.842
#define C   (Flo64)-115334
#define D   (Flo64)-3730535

#define A1  (Flo64)3.354016E-3
#define B1  (Flo64)2.569355E-4
#define C1  (Flo64)2.626311E-6
#define D1  (Flo64)0.675278E-7

/*************************************************************************
 * Function Name: NtcRatioToTemperature
 * Parameters: Flo32 Ratio
 *
 * Return: Flo32
 *
 * Description: Calculate temperature by Rt to Rref ratio
 *
 *************************************************************************/
Flo32 NtcRatioToTemperature (Flo32 Ratio)
{
Flo64 Temp,TempLn;
  if(Ratio==0)
  {
    return(0);
  }
  TempLn = logl(((Flo64)1-Ratio)/Ratio);
  Temp = A1 + TempLn*B1 + TempLn*TempLn*C1+TempLn*TempLn*TempLn*D1;
  return((Flo32)((Flo64)1/Temp));
}

/*************************************************************************
 * Function Name: NtcTemperatureToRatio
 * Parameters: Flo32 Ratio
 *
 * Return: Flo32
 *
 * Description: Calculate the ratio from temperature
 *
 *************************************************************************/
Flo32 NtcTemperatureToRatio (Flo32 T)
{
Flo64 Temp;
  Temp = exp(A + B/T + C/(T*T) + D/(T*T*T));
  return((Flo32)((Flo64)1/(Temp+1)));
}

