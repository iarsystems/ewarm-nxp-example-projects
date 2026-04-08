/***********************************************************************
 * $Id: ea3250_board.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Embedded Artists 3250 board definitions
 *
 * Description:
 *     This file contains board specific information such as the
 *     chip select wait states, and other board specific information.
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

#ifndef EA3250_BOARD_H
#define EA3250_BOARD_H

#include "lpc_types.h"
#include "lpc32xx_chip.h"
#include "lpc32xx_emc.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 * System configuration defines
 **********************************************************************/


typedef struct 
{
  UNS_32 magic;    /* magic number  */
  UNS_8  boardver; /* board version */
  UNS_32 dramcfg;  /* SDRAM configuration */
  UNS_8 mac[6];    /* mac address   */ 
} EA_HW_CFG_T;

#define EA_BOARD_MAGIC 0xEA3250AE

/* DRAM type mask (8 bits) */
#define EA_DRAM_TYPE_MASK  0xFF
#define EA_DRAM_TYPE_MT46H32M16LF 0
/* DRAM size mask (4 bits) */ 
#define EA_DRAM_SIZE_MASK _SBF(8, 0x0F)
#define EA_DRAM_SIZE_32M  _SBF(8, 0)
#define EA_DRAM_SIZE_64M  _SBF(8, 1)
#define EA_DRAM_SIZE_128M _SBF(8, 2)


/***********************************************************************
 * I2C EEPROM
 **********************************************************************/

#define EA_EEPROM_SIZE 0x8000
#define EA_EEPROM_HW_CFG_OFS  (EA_EEPROM_SIZE-0x80)

/***********************************************************************
 * NAND, NOR FLASH, and SDIO timing information
 **********************************************************************/


/* K9F1G08U0A-P device */
#define EA_NAND_TCEA_DELAY   28571428
#define EA_NAND_BUSY_DELAY   10000000
#define EA_NAND_NAND_TA      33333333
#define EA_NAND_RD_HIGH      99999999
#define EA_NAND_RD_LOW       66666666
#define EA_NAND_WR_HIGH      99999999
#define EA_NAND_WR_LOW       66666666



/***********************************************************************
 * Functions
 **********************************************************************/

extern EA_HW_CFG_T eaHwCfg;

/* Miscellaneous board setup functions */
void ea3250_board_init(void);

/* Enable or disable SDMMC power */
void ea3250_sdpower_enable(BOOL_32 enable);

/* Enables or disables the LCD backlight */
void ea3250_lcd_backlight_enable(BOOL_32 enable);

/* Enables or disables the LCD power */
void ea3250_lcd_power_enable(BOOL_32 enable);

INT_32 ea3250_read_config(void);
INT_32 ea3250_write_config(EA_HW_CFG_T* eaHwCfg);

#ifdef __cplusplus
}
#endif

#endif /* EA3250_BOARD_H */
