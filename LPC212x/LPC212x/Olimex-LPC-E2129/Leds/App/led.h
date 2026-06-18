/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : led.h
 *    Description :
 *
 *    History :
 *    1. Date: June 15th, 2006
 *       Author: Todor Atanasov
 *       Description: Created
 *
 *    $Revision: 28 $
 **************************************************************************/
#define LED1_ON()   IO0CLR_bit.P0_8 = 1
#define LED1_OFF()  IO0SET_bit.P0_8 = 1
#define LED2_ON()   IO0CLR_bit.P0_10 = 1
#define LED2_OFF()  IO0SET_bit.P0_10 = 1
#define LED3_ON()   IO0CLR_bit.P0_11 = 1
#define LED3_OFF()  IO0SET_bit.P0_11 = 1

// init leds port
void LedsInit(void);

