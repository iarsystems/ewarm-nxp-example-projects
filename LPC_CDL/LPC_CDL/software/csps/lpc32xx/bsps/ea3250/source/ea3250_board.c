/***********************************************************************
 * $Id: ea3250_board.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Embedded Artists LPC3250 board functions
 *
 * Description:
 *     This file contains driver support for Embedded Artists LPC3250
 *     board functions.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

#include "ea3250_board.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "board_i2c_eeprom_driver.h"


EA_HW_CFG_T eaHwCfg;

/***********************************************************************
 * Private functions
 **********************************************************************/



/***********************************************************************
 * Public functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: ea3250_board_init
 *
 * Purpose: Initializes basic board functions
 *
 * Processing:
 *     Does nothing
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_board_init(void)
{
  ;
}

/***********************************************************************
 *
 * Function: ea3250_sdpower_enable
 *
 * Purpose: Enable or disable SDMMC power
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_sdpower_enable(BOOL_32 enable)
{
  UNS_32 set, clr;

  /* active low */

  if (enable == TRUE)
  {
    set = 0;
    clr = P3_STATE_GPO(1);
  }
  else
  {
    set = P3_STATE_GPO(1);
    clr = 0;
  }

  gpio_set_gpo_state(set, clr);
}



/***********************************************************************
 *
 * Function: ea3250_lcd_backlight_enable
 *
 * Purpose: Enables or disables the LCD backlight
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     enable : TRUE ro enable, FALSE to disable
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_lcd_backlight_enable(BOOL_32 enable)
{
  // TODO
}

/***********************************************************************
 *
 * Function: ea3250_lcd_power_enable
 *
 * Purpose: Enables or disables the LCD power
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     enable : TRUE to enable, FALSE to disable
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_lcd_power_enable(BOOL_32 enable)
{
  if (enable != FALSE)
  {
    /* Active low to enable */
	  gpio_set_gpo_state(0x00, _BIT(14));
  }
  else
  {
    /* Active high to disable */
	  gpio_set_gpo_state(_BIT(14), 0x00);
  }
}

INT_32 ea3250_read_config(void) 
{  
  board_i2c_init(0);
  return board_i2c_read(EA_EEPROM_HW_CFG_OFS, (UNS_8*)&eaHwCfg, sizeof(eaHwCfg));
}

INT_32 ea3250_write_config(EA_HW_CFG_T* cfg) 
{  
  board_i2c_init(0);
  cfg->magic = EA_BOARD_MAGIC; 
  return board_i2c_write(EA_EEPROM_HW_CFG_OFS, (UNS_8*)cfg, sizeof(EA_HW_CFG_T));
}
