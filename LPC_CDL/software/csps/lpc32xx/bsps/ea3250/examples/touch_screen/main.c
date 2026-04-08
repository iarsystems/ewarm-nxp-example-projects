/**********************************************************************
 * $Id: main.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Touch screen example
 *
 * Description:
 *     Draws color bars on the LCD. Requires RGB565 mode.
 *
 **********************************************************************
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
 *********************************************************************/

#include <stdio.h>
#include <string.h>

#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_swim.h"
#include "lpc_lcd_params.h"
#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_board.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_clcdc_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_spi_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc_swim_font.h"
#include "lpc_swim.h"
#include "lpc_lcd_params.h"

#include "touch.h"

#include "uart.h"

#define EA_LCD_FRAME_BUF 0x81000000
#define LCD_DISPLAY      ea_QVGA_v2


const LCD_PARAM_T ea_QVGA_v2 =
{
  28,       /* Horizontal back porch */
  10,       /* Horizontal front porch */
  2,        /* HSYNC pulse width */
  240,      /* Pixels per line */
  3,        /* Vertical back porch */
  2,        /* Vertical front porch */
  2,        /* VSYNC pulse width */
  320,      /* Lines per panel */
  0,        /* Do not invert output enable */
  1,        /* Invert panel clock */
  1,        /* Invert HSYNC */
  1,        /* Invert VSYNC */
  1,        /* AC bias frequency (not used) */
  16,       /* Bits per pixel */
  /*5213000*/5000000/*176366*/,  /* Optimal clock rate (Hz) */
  TFT,   	  /* LCD panel type */
  0,        /* Single panel display */
  0,        /* HRTFT CLS enable flag */
  0,        /* HRTFT SPS enable flag */
  0,        /* HRTFT LP to PS delay */
  0,        /* HRTFT polarity delay */
  0,        /* HRTFT LP delay */
  0,        /* HRTFT SPL delay */
  0         /* HRTFT SPL to CLKS delay */
};


#define QVGA_CS_LOW   gpio_set_gpo_state(0x00, P3_STATE_GPO(4))
#define QVGA_CS_HIGH  gpio_set_gpo_state(P3_STATE_GPO(4), 0x00);
#define QVGA_RESET_RS gpio_set_gpo_state(0x00, P3_STATE_GPO(5));
#define QVGA_SET_RS   gpio_set_gpo_state(P3_STATE_GPO(5), 0x00);

static void 
writeToReg(INT_32 spidev, UNS_16 addr, UNS_16 data)
{
  SPI_CONFIG_T spicfg;
  UNS_32 status;
  UNS_8 datao [2];

  spicfg.transmitter = TRUE;
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);


  QVGA_RESET_RS;
  QVGA_CS_LOW;
  datao[0] = 0;
  datao[1] = (addr & 0xff);
  spi_write(spidev, datao, 2);
  spi_ioctl(spidev, SPI_DELAY, 1);
  QVGA_CS_HIGH;


  QVGA_SET_RS;
  QVGA_CS_LOW;
  datao[0] = (data >> 8);
  datao[1] = (data & 0xff);
  spi_write(spidev, datao, 2);
  spi_ioctl(spidev, SPI_DELAY, 1);
  QVGA_CS_HIGH; 


  QVGA_RESET_RS;
  QVGA_CS_LOW;
  datao[0] = 0;
  datao[1] = 0x22;
  spi_write(spidev, datao, 2);
  spi_ioctl(spidev, SPI_DELAY, 1);
  QVGA_CS_HIGH;

  /* wait for FIFO to be empty */
  do
  {
    status = spi_ioctl(spidev, SPI_GET_STATUS, SPI_RAW_INTS_ST);
  }
  while ((status & SPI_STAT_BE) == 0);

}


static void
display_init(void)
{
  SPI_CONFIG_T spicfg;
  INT_32 spidev = 0;



  /* Open SPI */
  spicfg.databits = 8;
  spicfg.highclk_spi_frames = TRUE;
  spicfg.usesecond_clk_spi = TRUE;
  spicfg.spi_clk = 1000000;
  spicfg.msb = FALSE;
  spicfg.transmitter = TRUE;
  spicfg.busy_halt = FALSE;
  spicfg.unidirectional = TRUE;

  spidev = spi_open(SPI1, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_ENABLE, 1);

  writeToReg (spidev, 0x00,0x0001);
  timer_wait_ms(TIMER_CNTR0, 20);
  writeToReg (spidev, 0x03,0xA2A4);
  writeToReg (spidev, 0x0C,0x0004);
  writeToReg (spidev, 0x0D,0x0308);
  writeToReg (spidev, 0x0E,0x3000);
  timer_wait_ms(TIMER_CNTR0, 50);
  writeToReg (spidev, 0x1E,0x00AF);
  writeToReg (spidev, 0x01,0x2B3F);
  writeToReg (spidev, 0x02,0x0600);
  writeToReg (spidev, 0x10,0x0000);
  writeToReg (spidev, 0x07,0x0233);
  writeToReg (spidev, 0x0B,0x0039);
  writeToReg (spidev, 0x0F,0x0000);
  timer_wait_ms(TIMER_CNTR0, 50);

  writeToReg (spidev, 0x30,0x0707);
  writeToReg (spidev, 0x31,0x0204);
  writeToReg (spidev, 0x32,0x0204);
  writeToReg (spidev, 0x33,0x0502);
  writeToReg (spidev, 0x34,0x0507);
  writeToReg (spidev, 0x35,0x0204);
  writeToReg (spidev, 0x36,0x0204);
  writeToReg (spidev, 0x37,0x0502);
  writeToReg (spidev, 0x3A,0x0302);
  writeToReg (spidev, 0x3B,0x0302);

  writeToReg (spidev, 0x23,0x0000);
  writeToReg (spidev, 0x24,0x0000);

  writeToReg (spidev, 0x48,0x0000);
  writeToReg (spidev, 0x49,0x013F);
  writeToReg (spidev, 0x4A,0x0000);
  writeToReg (spidev, 0x4B,0x0000);

  writeToReg (spidev, 0x41,0x0000);
  writeToReg (spidev, 0x42,0x0000);

  writeToReg (spidev, 0x44,0xEF00);
  writeToReg (spidev, 0x45,0x0000);
  writeToReg (spidev, 0x46,0x013F);
  timer_wait_ms(TIMER_CNTR0, 50);

  writeToReg (spidev, 0x44,0xEF00);
  writeToReg (spidev, 0x45,0x0000);
  writeToReg (spidev, 0x4E,0x0000);
  writeToReg (spidev, 0x4F,0x0000);
  writeToReg (spidev, 0x46,0x013F);

  spi_close(spidev);

}

void
calibrateStart(SWIM_WINDOW_T *win);

/**********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point from the startup code
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
 *********************************************************************/
void c_entry(void)
{
  SWIM_WINDOW_T win1;
  COLOR_T *fblog;
  INT_32 lcddev;

  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)
	(IRAM_BASE + (256 * 1024) - (16 * 1024)));

  /* Setup miscellaneous board functions */
  ea3250_board_init();

  uart_output_init();

  /* Setup LCD muxing for STN Color 16BPP */
  clkpwr_setup_lcd(CLKPWR_LCDMUX_TFT16, 1);

  /* Enable clock to LCD block (HCLK_EN)*/
  clkpwr_clk_en_dis(CLKPWR_LCD_CLK, 1);

  /* Setup LCD paramaters in the LCD controller */
  lcddev = lcd_open(CLCDC, (INT_32) &LCD_DISPLAY);

  /* Upper Panel Frame Base Address register */
  lcd_ioctl(lcddev, LCD_SET_UP_FB, EA_LCD_FRAME_BUF); 

  /* Enable LCD controller and power signals */
  lcd_ioctl(lcddev, LCD_PWENABLE, 1);

  /* Enable LCD backlight */
  ea3250_lcd_backlight_enable(TRUE);

  /* Enable LCD power */
  ea3250_lcd_power_enable(TRUE);


  /* setup mux and clkpwr for SPI */
  GPIO->p_mux_clr = P_SPI1DATAIO_SSP0_MOSI |
                  P_SPI1DATAIN_SSP0_MISO | P_SPI1CLK_SCK0;
  CLKPWR->clkpwr_spi_clk_ctrl |= CLKPWR_SPICLK_USE_SPI1;

  display_init();

  touch_init();

  /* Set frame buffer address */
  fblog = (COLOR_T *) 
          cp15_map_physical_to_virtual(EA_LCD_FRAME_BUF);

  /* Create a SWIM window */
  swim_window_open(&win1, LCD_DISPLAY.pixels_per_line,
      LCD_DISPLAY.lines_per_panel, fblog, 0, 0,
	(LCD_DISPLAY.pixels_per_line - 1), 
	(LCD_DISPLAY.lines_per_panel - 1),1, WHITE, BLACK, BLACK);


  /*
   * NOTE: This is not a good example of how you would actually
   *       write a touch screen driver. There is often a bit of
   *       noice involved when reading from the screen so a 
   *       more correct driver would collect several samples
   *       before calibrating/outputting anything on the screen.
   *
   *       If there is noice when calibrating the screen you 
   *       would, e.g., always get an offset when outputting
   *       values on the display.
   */

  calibrateStart(&win1);
  swim_clear_screen(&win1, BLACK);

  while(1) 
  {
    INT_32 x, y, z = 0;

    touch_xyz(&x, &y, &z);
    swim_put_pixel(&win1, x, y);
    timer_wait_ms(TIMER_CNTR0, 2);


  }



}
