/***********************************************************************
 * $Id: main.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: SDMMC driver example with interrupts (FIFO mode)
 *
 * Description:
 *     A SD card controller driver example using SD/MMC.
 *
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

#include <stdio.h>
#include <string.h>

#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_board.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_sdcard_driver.h"
//#include "sdmmc_dma_example.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "uart.h"

#include "diskio.h"
#include "ff.h"

static INT_32 timer1dev;

/* UART buffer */
static char buff[512];

static FILINFO Finfo;
static FATFS lFatfs[1];

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

static UNS_32 msecs = 0;

/***********************************************************************
 *
 * Function: timer1_user_interrupt
 *
 * Purpose: Timer 1 interrupt handler
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
static void timer1_user_interrupt(void)
{
  /* Clear latched timer interrupt */
  timer_ioctl(timer1dev, TMR_CLEAR_INTS, TIMER_CNTR_MTCH_BIT(0));
  msecs += 10;
  disk_timerproc();
}


static void test_fat(void)
{
  DWORD p2;
  WORD w1;
  BYTE res, b1;
  DIR dir;
  
  uart_output("Testing FAT\r\n");

  
  res = f_mount(0, &lFatfs[0]);
  if (res != FR_OK) {
    sprintf(buff, "Failed to mount 0: %d \r\n", res);
    uart_output((UNS_8*)buff);
    return;
  }
  
  res = f_opendir(&dir, "/");
  if (res) {
    sprintf(buff, "Failed to open /: %d \r\n", res);
    uart_output((UNS_8*)buff);
    return;
  }


//#if 0  
  if (disk_ioctl(0, GET_SECTOR_COUNT, &p2) == RES_OK) {
    sprintf(buff, "Drive size: %d \r\n", p2);
    uart_output((UNS_8*)buff);
  }
  
  if (disk_ioctl(0, GET_SECTOR_SIZE, &w1) == RES_OK) {
    sprintf(buff, "Sector size: %d \r\n", w1);
    uart_output((UNS_8*)buff);
  }
  
  if (disk_ioctl(0, GET_BLOCK_SIZE, &p2) == RES_OK) {
    sprintf(buff, "Erase block size: %d \r\n", p2);
    uart_output((UNS_8*)buff);
  }
  
  if (disk_ioctl(0, MMC_GET_TYPE, &b1) == RES_OK) {
    sprintf(buff, "Card type: %d \r\n", b1);
    uart_output((UNS_8*)buff);
  }
//#endif 
  
  for(;;) {
    res = f_readdir(&dir, &Finfo);
    if ((res != FR_OK) || !Finfo.fname[0]) break;
  
      uart_output((UNS_8*)&(Finfo.fname[0]));
      uart_output("\r\n");  
  }


}


/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Always returns 1
 *
 * Notes: None
 *
 **********************************************************************/
void c_entry(void)
{
  TMR_PSCALE_SETUP_T pscale;
  TMR_MATCH_SETUP_T msetup;


  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  /* Setup miscellaneous board functions */
  ea3250_board_init();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)
                     (IRAM_BASE + (256 * 1024) - (16 * 1024)));

  /* Initialize interrupt system */
  int_initialize(0xFFFFFFFF);

  /* Install standard IRQ dispatcher at ARM IRQ vector */
  int_install_arm_vec_handler(IRQ_VEC, (PFV) lpc32xx_irq_handler);

  int_install_irq_handler(IRQ_TIMER1, (PFV) timer1_user_interrupt);

  /* Open timers - this will enable the clocks for all timers when
     match control, match output, and capture control functions
     disabled. Default clock will be internal. */
  timer1dev = timer_open(TIMER_CNTR1, 0);


  /******************************************************************/
  /* Setup timer 1 for a 100Hz match rate                            */

  /* Use a prescale count time of 100uS                             */
  pscale.ps_tick_val = 0; /* Use ps_us_val value */
  pscale.ps_us_val = 100; /* 100uS */
  timer_ioctl(timer1dev, TMR_SETUP_PSCALE, (INT_32) &pscale);

  /* Use a match count value of 100 (100 * 100uS = 10mS (100Hz))  */
  msetup.timer_num = 0; /* Use match register set 0 (of 0..3) */
  msetup.use_match_int = TRUE; /* Generate match interrupt on match */
  msetup.stop_on_match = FALSE; /* Do not stop timer on match */
  msetup.reset_on_match = TRUE; /* Reset timer counter on match */
  msetup.match_tick_val = 99; /* Match is when timer count is 100 */
  timer_ioctl(timer1dev, TMR_SETUP_MATCH, (INT_32) &msetup);

  /* Clear any latched timer 1 interrupts and enable match
   interrupt */
  timer_ioctl(timer1dev, TMR_CLEAR_INTS,
              (TIMER_CNTR_MTCH_BIT(0) | TIMER_CNTR_MTCH_BIT(1) |
               TIMER_CNTR_MTCH_BIT(2) | TIMER_CNTR_MTCH_BIT(3) |
               TIMER_CNTR_CAPT_BIT(0) | TIMER_CNTR_CAPT_BIT(1) |
               TIMER_CNTR_CAPT_BIT(2) | TIMER_CNTR_CAPT_BIT(3)));

  msecs = 0;
  timer_ioctl(timer1dev, TMR_ENABLE, 1);
  int_enable(IRQ_TIMER1);

  /* Enable SDMMC power */
  ea3250_sdpower_enable(TRUE);


  dma_init();
  //enable_irq();
  enable_irq_fiq();

  uart_output_init();

  test_fat();


  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  
  while(1);
}


