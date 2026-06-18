/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : AT91SAM9261_init.c
 *    Description : Define init module
 *
 *    History :
 *    1. Date        : May, 3 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4174 $
 **************************************************************************/
#include <NXP/iolh75401.h>
#include "board.h"

#pragma segment="INTVEC_TCM_SRAM"
#pragma segment=".intvec"

#pragma location="INTVEC_TCM_SRAM"
#pragma data_alignment=4
static __root __no_init char Vect[64];

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
unsigned long *pDst, *pStr;
  // Disable watchdog
  WDT_CTRL_bit.EN = 0;
  // Init I/O DATA8-16, ADDR16-23, CS1-3, BLE0, BLE1, WAIT
  IOCON_EBI_MUX = 0x7FFF;
  // Initialize CS0 - Flash, CS3 - CPLD, CS2 - RAM
  // IDCY = 8, WST1 = 8, WST2 = 8, WP = 1, BM = 0, MW = 16 bit
  SMC_BCR0 = 0x14001CE7;
  // IDCY = 1,  WST1 = 1,  WST2 = 1,  WP = 0, BM = 0, MW = 16 bit
  SMC_BCR2 = 0x10000400;
  // IDCY = 16, WST1 = 32, WST2 = 32, WP = 0, BM = 0, MW = 16 bit
  SMC_BCR3 = 0x1000FFEF;

  // HCLK Init
  RCPC_SysClkPrescaler_bit.HCLK = HCLK_DIV;
  USR_LED_ON();

  // Copy INTVEC from Flash to TCM SRAM
  for(pStr = __segment_begin(".intvec"), pDst = __segment_begin("INTVEC_TCM_SRAM");
      pStr <= __segment_end(".intvec"); )
  {
    *pDst++ = *pStr++;
  }

  // Remap
  RCPC_Remap_bit.REMAP = 1;
  return(1);
}
