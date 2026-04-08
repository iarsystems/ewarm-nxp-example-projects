/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : buttons.h
 *    Description :
 *
 *    History :
 *    1. Date: June 15th, 2006
 *       Author: Todor Atanasov
 *       Description: Created
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <NXP/iolpc2129.h>

#define BUT1_STATE     IO0PIN_bit.P0_15
#define BUT2_STATE     IO0PIN_bit.P0_9
#define PRESSED             0
#define RELEASED            1
#define ON_KEY_PRESSED      0
#define ON_KEY_RELEASED     1

// init buttons' port
void InitButtons(void);

