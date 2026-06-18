/***********************************************************************
 * $Id: spi_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: SPI NOR example
 *
 * Description:
 *     A simple SPI polled driver example.
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
#include "ea3250_spinor.h"

#include "lpc32xx_ssp_driver.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_spi_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_clkpwr.h"

#include "uart.h"

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

/* UART buffer */
static char buff[512];


static UNS_8 s1l_cfg[] =
{
0x65,0x61,0x33,0x32,0x35,0x30,0x3e,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x02,0x00,0x00,0x00,0x00,0xc2,0x01,0x00,
0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
0x5c,0x00,0x00,0x00,0x9c,0xdb,0x02,0x00,
0x00,0x00,0xfa,0x83,0x00,0x00,0xfa,0x83,
0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x78,0x09,0x00,0x00,0x40,0x00,0x03,0x00,
0xc4,0xb2,0x03,0x08,0xc8,0xb2,0x03,0x08,
0x00,0x00,0xfa,0x83,0x00,0x00,0xfa,0x83,
0x22,0x61,0x97,0xad,0x00,0x00,0x00,0x00,
};




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
 * Returns: Always returns 1, or <0 on an error
 *
 * Notes: None
 *
 **********************************************************************/
int c_entry(void)
{
  INT_32 pageSize = 0;

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

  /* The SPI can not control the SSEL signal, it will be driven
     by write/read routines as needed and accessed as an output
     GPIO pin. This EEPROM can be use with SPI mode 3 when the
     chip select must remain asserted for the entire cycle.     */
  GPIO->p2_mux_clr = P2_GPIO05_SSEL0;
  GPIO->p2_dir_set = P3_STATE_GPIO(5);
  GPIO->p3_outp_set = P3_STATE_GPIO(5);

  /* Select SPI1 pins */
  GPIO->p_mux_clr = P_SPI1DATAIO_SSP0_MOSI |
                    P_SPI1DATAIN_SSP0_MISO | P_SPI1CLK_SCK0;
  CLKPWR->clkpwr_spi_clk_ctrl |= CLKPWR_SPICLK_USE_SPI1;

  /* Enable IRQ interrupts in the ARM core */
  enable_irq();

  uart_output_init();

  if ((pageSize = spinor_init()) < 0) 
  {
    /* Failed to initialize SPI NOR */
    uart_output("Failed to initialize SPI NOR\r\n");  
    return -1; 
  }

  sprintf(buff, "NOR flash page size=%d\r\n", pageSize);
  uart_output((UNS_8*)buff);  

/* test reading and writing */
{
//#define DO_WRITE



#define BUF_SZ 512
#define NOR_OFFSET 0 //(4*1024*1024-(64*1024)) 
  UNS_8 data[BUF_SZ];
  UNS_8 wrbuf[BUF_SZ];
  STATUS s;
  int i = 0;
/*
{
  for (i = 0; i < 4*1024*1024; i++) {
    spinor_block_erase(i);
    s = spinor_read(data, NOR_OFFSET, pageSize);
    if (data[0] == 0xff) {
      sprintf(buff, "erased i = %d\r\n", i);
      uart_output((UNS_8*)buff);
      break;            
    }    
  }

  while(1);
}
*/


#ifdef DO_WRITE
  memset (wrbuf, 0, BUF_SZ);
  //memcpy (wrbuf, s1l_cfg, sizeof(s1l_cfg) / sizeof(UNS_8));
  for ( i = 0; i < pageSize; i++)
    wrbuf[i] = i;
  
  spinor_block_erase(NOR_OFFSET);
  s = spinor_write_page(wrbuf, NOR_OFFSET);
  if (s < 0)
    uart_output("Failed to write to flash\r\n");  
    
#endif

  s = spinor_read(data, NOR_OFFSET, pageSize);
  if (s < 0)
    uart_output("Failed to read from flash\r\n");  

  for (i = 0; i < pageSize; i++)
  {
    if (i > 0 && (i % 8) == 0)
      uart_output("\r\n");

    sprintf(buff, "%2x%c", data[i], (((i+1)%8)==0) ? ' ' : ':');
    uart_output((UNS_8*)buff);      
  }

  uart_output("\r\n");
#ifdef DO_WRITE

  for ( i = 0; i < pageSize; i++)
  {
    if (data[i] != (UNS_8)(i))
    {
      sprintf(buff, "Verification failed of read data %x != %x\r\n", 
        i, data[i]);
      uart_output((UNS_8*)buff);
      break;
    }    
  }

#endif
}

  /* Disable interrupts in ARM core */
  disable_irq_fiq();


  return 1;
}
