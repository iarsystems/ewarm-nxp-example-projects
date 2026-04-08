/***********************************************************************
 * $Id: slcnand_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: SLC NAND controller test
 *
 * Description:
 *     SLC NAND controller test
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
#include "lpc_arm922t_cp15_driver.h"
#include "lpc_irq_fiq.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_dma_driver.h"
#include "lpc_nandflash_params.h"
#include "lpc32xx_slcnand_driver.h"
#include "lpc32xx_clcdc_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_uart_driver.h"
#include "ea3250_board.h"

#include "uart.h"


/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

/* Device handles */
static INT_32 slcnanddev;

/* UART buffer */
static char buff[512];

const NANDFLASH_PARAM_T k9f1g08u0a_9 =
{
  BUS_WIDTH_8,                   /* Bus width */
  BLOCK_PAGE_LARGE,              /* Block page (small/large) */
  ADDR_CYCLES_4,                 /* Address cycles (3/4/5) */
};

/* SLC NAND flash */
#define SLCNAND_FLASH k9f1g08u0a_9


/* FLASH data buffer */
UNS_32 buffer [(2048+64) / sizeof(UNS_32)];

/* Enable the following define to allow block erase/write operations
   to occur in the example. Enabling these operations may erase
   important information in the FLASH. Use with care! */
//#define USEWRITE


/***********************************************************************
 *
 * Function: dump_data
 *
 * Purpose: Dump some values to the console
 *
 * Processing:
 *     Converts the values in the passed array to strings and displays
 *     them in the console.
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
void dump_data(char *str,
			   void *data,
			   int bytes)
{
  int i = 0;

  sprintf(buff, "%s\r\n%3x: ", str, 0);
  uart_output((UNS_8*)buff);

  for (i = 0; i < bytes; i++) {
    if (i > 0 && (i%16) == 0) {
      sprintf(buff, "\r\n%3x: ", i);
      uart_output((UNS_8*)buff);    
    }

    sprintf(buff, "%02x ", ((char*)data)[i]);
    uart_output((UNS_8*)buff);    


    
  }
  uart_output("\r\n"); 
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
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void c_entry(void)
{
  SLC_TAC_T tac;
  UNS_32 flashid;
  SLC_BLOCKPAGE_T blockpage;
  int idx = 0;

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


  /* Setup DMA */
  dma_init();

  /* Enable interrupts in ARM core */
  enable_irq_fiq();

  uart_output_init();

  sprintf(buff, "Testing print\r\n");
  uart_output("Opening SLC NAND\r\n");

  slcnanddev = slcnand_open(SLCNAND, (INT_32) &SLCNAND_FLASH);
  if (slcnanddev == 0)
  {
    uart_output("Error opening NAND SLC driver\r\n");
    goto testfail;
  }

  /* Setup SLC NAND controller timing */
  tac.w_rdy = 14;
  tac.w_width = 5;
  tac.w_hold = 2;
  tac.w_setup = 1;
  tac.r_rdy = 14;
  tac.r_width = 4;
  tac.r_hold = 2;
  tac.r_setup = 1;

  uart_output("Setting SLC NAND timing\r\n");
  if (slcnand_ioctl(slcnanddev, SLC_SET_TIMING, (INT_32) &tac)
                    == _ERROR)
  {
    uart_output("Error setting SLC NAND timing\r\n");
    goto testfail;
  }

  /* Get SLC NAND flash id */
  if (slcnand_ioctl(slcnanddev, SLC_READ_ID,
                    (INT_32) &flashid) == _ERROR)
  {
    uart_output("Error reading NAND ID\r\n");
    goto testfail;
  }
  flashid = flashid & 0xFFFF;

  sprintf(buff, "Detected Flash ID is: %x\r\n", flashid);
  uart_output((UNS_8*)buff);



#ifdef USEWRITE
  memset(((UNS_8*)buffer)+2048, 0xff, 64);
  /* Erase block 100 */
  if (slcnand_ioctl(slcnanddev, SLC_ERASE_BLOCK, 100) == _ERROR)
  {
    uart_output("Error erasing block 100\r\n");
    goto testfail;
  }

  /* Generate a test pattern for the page */
  for (idx = 0; idx < (2048 / sizeof(UNS_32)); idx++)
  {
	  buffer [idx] = idx;

  }
  
  /* Write SLC NAND flash page
                    dma disabled, ecc disabled */
  blockpage.dma = 0;
  blockpage.ecc = 0;
  blockpage.block_num = 100;
  blockpage.page_num = 0;
  blockpage.buffer = (UNS_8 *) buffer;
  if (slcnand_ioctl(slcnanddev, SLC_WRITE_PAGE,
                    (INT_32) &blockpage) == _ERROR)
  {
    uart_output("Error writing block 100/page 0\r\n");
    goto testfail;
  }

  /* Clear buffer for test pattern read */
  for (idx = 0; idx < (2048 / sizeof(UNS_32)); idx++)
  {
	  buffer [idx] = 0;//idx;
  }
#endif

  /* Read SLC NAND flash page
                    dma disabled, ecc disabled */
  blockpage.dma = 0;
  blockpage.ecc = 0;
  blockpage.block_num = 100;
  blockpage.page_num = 0;
  blockpage.buffer = (UNS_8 *) buffer;
  if (slcnand_ioctl(slcnanddev, SLC_READ_PAGE,
                  (INT_32) &blockpage) == _ERROR)
  {
    uart_output("Error reading block 100/page 0\r\n");
    goto testfail;
  }

  /* Dump first 32 bytes of data from page */
  dump_data("First 32 bytes of data from block 100/page 0", buffer, 32);

#ifdef USEWRITE
  /* Verify test pattern for the page */
  for (idx = 0; idx < (2048 / sizeof(UNS_32)); idx++)
  {
	  if (buffer [idx] != idx)
	  {
      uart_output("Block 100/page 0 read failed!\r\n");
	    goto testfail;
	  }
  }
#endif

testfail:
  uart_output("NAND SLC driver example complete\r\n");
  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  slcnand_close(slcnanddev);
  timer_wait_ms(TIMER_CNTR0, 8000);

  /* Leave the display on so the results can still be viewed after
    the example is complete */
//  while (1);
}
