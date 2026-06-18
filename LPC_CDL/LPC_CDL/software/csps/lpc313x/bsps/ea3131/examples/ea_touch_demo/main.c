
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc313x_chip.h"
#include "ea3131_board.h"
#include "lpc313x_sysreg.h"
#include "lpc313x_mpmc.h"
#include "lpc313x_intc_driver.h"
#include "lpc313x_cgu_driver.h"
#include "lpc313x_i2c_driver.h"
#include "lpc313x_uart_driver.h"
#include "lpc313x_timer_driver.h"
#include "lpc313x_adc10b_driver.h"
#include "lpc313x_pwm_driver.h"

#include "lcd_hw.h"
#include "lcd_grph.h"
#include "touch.h"


/* device handles */
static INT_32 dev_uart;
static INT_32 dev_pwm;


/* buffer for the uart when printing strings */
static char buff[512];  

void calibrateStart(void);

/***********************************************************************
 *
 * Function: uart_string_write
 *
 * Purpose: Output a string on the UART
 *
 * Parameters:
 *    devid : UART device ID
 *    pbuff : string to output
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void uart_string_write(INT_32 devid, char *pbuff)
{
  UNS_32 len = strlen(pbuff);
  UNS_32 written;

  while (len > 0)
  {
    written = uart_write(devid, pbuff, len);
    len -= written;
    pbuff += written;
  }
}



/***********************************************************************
 *
 * Function: uart_init
 *
 * Purpose: Initialize UART
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void uart_init(void)
{
  UART_CONTROL_T ucntl;

  /* Setup UART for 115.2K, 8 data bits, no parity, 1 stop bit */
  ucntl.baud_rate = 115200;
  ucntl.parity = UART_PAR_NONE;
  ucntl.databits = 8;
  ucntl.stopbits = 1;

  dev_uart = uart_open((void *) UART_BASE, (INT_32) & ucntl);
}

/***********************************************************************
 *
 * Function: board_lcd_init
 *
 * Purpose: Initialize LCD
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void board_lcd_init(void)
{
  /* Static Memory Configuration for nCS0 */
  MPMC->static_mem[0].config = MPMC_STATIC_CFG_MW16 | MPMC_STATIC_CFG_BLS ;
  /* Static Memory Write Enable Delay for nCS0 */
  MPMC->static_mem[0].wtwen  = 2;
  /* Static Memory Output Enable Delay for nCS0 */
  MPMC->static_mem[0].wtoen  = 1;
  /* Static Memory Read Delay for nCS0 */
  MPMC->static_mem[0].wtrd   = 15;
  /* Static Memory Page Mode Read Delay for nCS0 */
  MPMC->static_mem[0].wtpg   = 2;
  /* Static Memory Write Delay for nCS0 */
  MPMC->static_mem[0].wtwr   = 4 /*2*/;
  /* Static Memory Turn Around Delay for nCS0 */
  MPMC->static_mem[0].wtturn = 2;

  lcd_init();
}

#define BACKLIGHT_Y   (DISPLAY_HEIGHT - 60)
#define SLIDER_Y      (BACKLIGHT_Y + 35)
#define SLIDER_MARGIN 20
#define SLIDER_H      20
#define BACKLIGHT_PERC_MAX 35

static UNS_16 lastSliderX = SLIDER_MARGIN; 

/***********************************************************************
 *
 * Function: adjustBacklight
 *
 * Purpose: Adjust the backlight
 *
 * Parameters: 
 *      x : x value of slider
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void adjustBacklight(UNS_16 x)
{
  UNS_16 perc = x - SLIDER_MARGIN;
  perc = perc * BACKLIGHT_PERC_MAX / (DISPLAY_WIDTH-2*SLIDER_MARGIN);
   
  pwm_ioctl(dev_pwm, PWM_DUTY_R, perc);
}

/***********************************************************************
 *
 * Function: moveSlider
 *
 * Purpose: Move the slider
 *
 * Parameters: 
 *      x : x value
 *      y : y value
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void moveSlider(UNS_16 x, UNS_16 y)
{
  if (y < SLIDER_Y-SLIDER_H/2 || y > SLIDER_Y+SLIDER_H/2)
    return;

  if (x < SLIDER_MARGIN || x > DISPLAY_WIDTH-SLIDER_MARGIN-1)
    return; 

  /* erase old */
  lcd_line(lastSliderX, SLIDER_Y-SLIDER_H/2, lastSliderX, SLIDER_Y+SLIDER_H/2, WHITE);
  
  lcd_line(lastSliderX, SLIDER_Y, lastSliderX, SLIDER_Y, BLACK);
  lcd_line(lastSliderX, SLIDER_Y+1, lastSliderX, SLIDER_Y+1, BLACK);   

  lcd_line(x, SLIDER_Y-SLIDER_H/2, x, SLIDER_Y+SLIDER_H/2, BLUE); 

  lastSliderX = x;

  adjustBacklight(x);

}

/***********************************************************************
 *
 * Function: drawBacklightArea
 *
 * Purpose: Draw the area for the backlight control
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
static void drawBacklightArea(void)
{

  lcd_line(0, BACKLIGHT_Y, DISPLAY_WIDTH-1, BACKLIGHT_Y, BLACK);
  lcd_putString(65, BACKLIGHT_Y+6, "Backlight control");

  lcd_line(SLIDER_MARGIN, SLIDER_Y, DISPLAY_WIDTH-SLIDER_MARGIN-1, SLIDER_Y, BLACK);
  lcd_line(SLIDER_MARGIN, SLIDER_Y+1, DISPLAY_WIDTH-SLIDER_MARGIN-1, SLIDER_Y+1, BLACK);

  moveSlider(lastSliderX, SLIDER_Y);
}

/***********************************************************************
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
 **********************************************************************/
void c_entry(void)
{
  CGU_FDIV_SETUP_T   fdiv_cfg;

  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  /* Setup miscellaneous board functions */
  ea3131_board_init();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)ISROM_MMU_TTB_BASE);

  /* Enable I2C system clock */
  cgu_clk_en_dis(CGU_SB_I2C0_PCLK_ID, TRUE);

  /* the quality of the I2C clock signal is better from PLL than
  from 12MHz FFAST. */
  /* set base frequency for the domain AHB0_APB1 belongs to PLL1.*/
  cgu_set_base_freq(CGU_SB_AHB0_APB1_BASE_ID, CGU_FIN_SELECT_HPPLL1);
  /* set I2C clock between 100KHz & 400KHz for write */
  fdiv_cfg.stretch = 1;
  fdiv_cfg.n = 1;
  fdiv_cfg.m = 4;

  /* change the frac divider corresponding to the clock.
  Note this changes clock fre for all clocks connect to
  this frac divider. */
  cgu_set_subdomain_freq(CGU_SB_I2C0_PCLK_ID, fdiv_cfg);

  /* Initialize interrupt system */
  int_initialize(0xFFFFFFFF);

  /* install default I2C0 & I2C1 interrupt handlers */
  int_install_irq_handler(IRQ_I2C0, (PFV)i2c0_user_interrupt);

  /* Enable IRQ interrupts in the ARM core */
  enable_irq();

  /* initialize uart */
  uart_init();

  /* setup LCD */
  board_lcd_init();

  touch_init();

  /* Open PWM */
  dev_pwm = pwm_open((void *)PWM, 0);
  // don't know why this is necessary...
  pwm_close(dev_pwm);
  dev_pwm = pwm_open((void *)PWM, 0);

  pwm_ioctl(dev_pwm, PWM_ENABLE, 1);
  pwm_ioctl(dev_pwm, PWM_DUTY_R, 0);

  lcd_fillScreen(WHITE);
  calibrateStart();
  lcd_fillScreen(WHITE);

  drawBacklightArea();

  while(1) {
    INT_32 x, y, z = 0;

    touch_xyz(&x, &y, &z);
    if (y < BACKLIGHT_Y) 
    {
      lcd_point(x, y, BLACK);
    }
    else 
    {
      moveSlider(x, y);      
    }
  }


}
