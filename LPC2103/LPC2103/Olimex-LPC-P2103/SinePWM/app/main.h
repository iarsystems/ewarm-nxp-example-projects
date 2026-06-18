/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : Main.h
 *    Description : 
 *
 *    History :
 *    1. Date        : 28.3.2008 
 *       Author      : Stoyan Choynev
 *       Description : 
 *
 *    $Revision: 4271 $
 **************************************************************************/
#ifndef   __MAIN_H
  #define __MAIN_H
/** include files **/

/**definitions **/
//Oscilator Frequency[Hz]
#define FOSC    14745600UL
//System Clock[Hz]
#define CCLK    44236800UL
//Periferial Clock[Hz]
#define PCLK    CCLK
//PLL MSEL
#define PLL_M_VAL 2
//PLL PSEL
#define PLL_P_VAL 1
//APB divider
#define APBDIV_VAL 1  //Same as System Clock

/** default settings **/

/** public data **/

/** public functions **/

#endif //__MAIN_H
