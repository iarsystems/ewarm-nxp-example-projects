/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : low_level_init.c
 *    Description : Low level init procedure
 *
 *
 *    $Revision: 16 $
 **************************************************************************/
#include <NXP\MIMXRT1052.h>

extern uint32_t __vector_table[];

/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{    
  /*WDT disable*/
  RTWDOG_CNT = 0xD928C520;  
  RTWDOG_TOVAL = 0xFFFF;    
  RTWDOG_CS   = 0x2120;     
   
  return 1;
}
