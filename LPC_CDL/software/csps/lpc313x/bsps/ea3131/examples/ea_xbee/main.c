

#include <stdio.h>
#include <string.h>
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
#include "lpc313x_ioconf_driver.h"

#include "lcd_hw.h"
#include "lcd_grph.h"


/* device handles */
static INT_32 dev_uart;


/* buffer for the uart when printing strings */
static char buff[512];       

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

  /* Setup UART for 9.6K, 8 data bits, no parity, 1 stop bit */
  ucntl.baud_rate = 9600;
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


/*****************************************************************************
 *
 * Description:
 *    
 *
 ****************************************************************************/
UNS_8
testXBeeComm(void)
{
  UNS_32 rxChars = 0;
  UNS_32 timeStamp;
	UNS_8 xpos=0,ypos=0;

  sprintf(buff, "***************************************");
  lcd_putString(0, 0, (UNS_8*)buff);
  sprintf(buff, "*                                     *");
  lcd_putString(0, 8, (UNS_8*)buff);
  sprintf(buff, "* Communication with the XBee ZigBee  *");
  lcd_putString(0, 16, (UNS_8*)buff);
  sprintf(buff, "* module is demonstrated              *\n");
  lcd_putString(0, 24, (UNS_8*)buff);
  sprintf(buff, "*                                     *");
  lcd_putString(0, 32, (UNS_8*)buff);
  sprintf(buff, "***************************************");
  lcd_putString(0, 40, (UNS_8*)buff);
  sprintf(buff, "* Communication with XBee module below:");
  lcd_putString(0, 48, (UNS_8*)buff);

  gpio_set_outpin_low(IOCONF_MUX1, 4);   //RTS output set low
	  
  timer_wait_ms(NULL, 1100);
  sprintf(buff, "+++");
  uart_string_write(dev_uart, buff);

  timer_wait_ms(NULL, 1100);
  sprintf(buff, "ATVL\r\n");
  uart_string_write(dev_uart, buff);
  rxChars = 0;

  //get current time
  timeStamp = 0;

  while(timeStamp < 3000)
  {
    UNS_8 rxChar;

    
    //check if any character has been received
    if (1 == uart_read(dev_uart, &rxChar, 1))
    {
      rxChars++;
      if (rxChar == '\r')
      {
        sprintf(buff, "\n%c", rxChar);
		    ypos+=8;
		    xpos=0;
	    }
      else
	    {
        sprintf(buff, "%c", rxChar);
	      xpos+=8;
	    }

      lcd_putString(50+xpos, 60+ypos, (UNS_8*)buff);
    }

    timer_wait_ms(NULL, 1);
	  timeStamp++;
  }
  
  if (rxChars > 75)
  {
    sprintf(buff, "***************************************");
    lcd_putString(0, 200, (UNS_8*)buff);
    sprintf(buff, "*  XBee module detected!!!            *");
    lcd_putString(0, 208, (UNS_8*)buff);
    sprintf(buff, "***************************************");
    lcd_putString(0, 216, (UNS_8*)buff);

    return 1; //= XBee module present
  }
  else
  {
    sprintf(buff, "***************************************");
    lcd_putString(0, 200, (UNS_8*)buff);
    sprintf(buff, "*  No XBee module detected...         *");
    lcd_putString(0, 208, (UNS_8*)buff);
    sprintf(buff, "***************************************");
    lcd_putString(0, 216, (UNS_8*)buff);

    for(timeStamp=0; timeStamp<10; timeStamp++)
    {
	    gpio_set_outpin_high(IOCONF_GPIO, 2);
	    timer_wait_ms(NULL, 100);
	    gpio_set_outpin_low(IOCONF_GPIO, 2);
	    timer_wait_ms(NULL, 100);
	  }

    return 0; //= no XBee module present
  }
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

  lcd_fillScreen(BLACK);

  testXBeeComm();


}
