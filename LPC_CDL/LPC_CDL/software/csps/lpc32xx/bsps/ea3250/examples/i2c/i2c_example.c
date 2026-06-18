/***********************************************************************
 * $Id: i2c_example.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: I2C example
 *
 * Description:
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

#include "lpc_types.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc_irq_fiq.h"
#include "lpc32xx_chip.h"
#include "ea3250_board.h"
#include "lpc32xx_hstimer_driver.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_i2c_driver.h"
#include "lpc32xx_timer_driver.h"

/***********************************************************************
 * I2C example private functions
 **********************************************************************/
/***********************************************************************
 *
 * Function: delay
 *
 * Purpose: generate a delay
 *
 * Processing:
 *     A local software counter counts up to the specified count.
 *
 * Parameters:
 *    cnt : number to be counted
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void delay(UNS_32 cnt)
{
    UNS_32 i = cnt;
    while (i != 0) i--;
    return;
}

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);

/* I2C device handles */
static INT_32 i2cdev1, i2cdev2;


/***********************************************************************
 *
 * Function: setLed
 *
 * Purpose: Change state of a LED
 *
 * Parameters:
 *    led : the number of the LED to change
 *    on : TRUE if the LED should be turned on; otherwise FALSE
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void
setLed(int led, int on)
{
  static volatile I2C_MTX_SETUP_T   i2c_mtx_setup;
  static volatile I2C_MTXRX_SETUP_T i2c_mtxrx_setup;

  UNS_8 rx_data[2];
  UNS_8 tx_data[2];

  tx_data[0] = 0x08;
  tx_data[1] = 0x00;

  //adjudt address if LED >= 5
  if (led >= 5)
    tx_data[0] = 0x09;

  //read current register value
  i2c_mtxrx_setup.addr_mode = ADDR7BIT;
  i2c_mtxrx_setup.sl_addr = 0xc0 >> 1;
  i2c_mtxrx_setup.tx_data = &tx_data[0];
  i2c_mtxrx_setup.tx_length = 1;
  i2c_mtxrx_setup.rx_data = &rx_data[0];
  i2c_mtxrx_setup.rx_length = 1;
  i2c_mtxrx_setup.retransmissions_max = 1;
  i2c_ioctl(i2cdev1, I2C_MASTER_TXRX, (INT_32)&i2c_mtxrx_setup);
  
  while (( i2c_mtxrx_setup.status & I2C_SETUP_STATUS_DONE) == 0);

  //update for new register value
  switch (led)
  {
  case 1:
  case 5:
    rx_data[0] &= ~0x03;
    if (on == 1)
      rx_data[0] |= 0x01;
    break;
  case 2:
  case 6:
    rx_data[0] &= ~0x0c;
    if (on == 1)
      rx_data[0] |= 0x04;
    break;
  case 3:
  case 7:
    rx_data[0] &= ~0x30;
    if (on == 1)
      rx_data[0] |= 0x10;
    break;
  case 4:
  case 8:
    rx_data[0] &= ~0xc0;
    if (on == 1)
      rx_data[0] |= 0x40;
    break;
  default:
    break;
  }
  tx_data[1] = rx_data[0];

  i2c_mtx_setup.addr_mode = ADDR7BIT;
  i2c_mtx_setup.sl_addr = 0xc0 >> 1;
  i2c_mtx_setup.tx_data = &tx_data[0];
  i2c_mtx_setup.tx_length = 2;
  i2c_mtx_setup.retransmissions_max = 1;
  i2c_ioctl(i2cdev1, I2C_MASTER_TX, (INT_32)&i2c_mtx_setup);
  while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
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
 extern int mtx_irq;
 extern int mtx_send;
 extern int mtx_stat[];
 extern UNS_32 mtx_length[];
 extern UNS_32 mtx_count[];
 extern UNS_32 mtx_txfifo[];
 extern UNS_32 mtx_txfifo_cnt[];
 extern int mtx_retr;

void c_entry(void)
{
  static I2C_SETUP_T i2c_setup[2];
  static volatile I2C_MTX_SETUP_T   i2c_mtx_setup;
  static I2C_MTXRX_SETUP_T i2c_mtxrx_setup;
  static I2C_MRX_SETUP_T   i2c_mrx_setup;
  UNS_8  rx_data[16], tx_data[16];

  /* Disable interrupts in ARM core */
  disable_irq();

  /* Setup miscellaneous board functions */
  ea3250_board_init();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)
                     (IRAM_BASE + (256 * 1024) - (16 * 1024)));

  /* Initialize interrupt system */
  int_initialize(0xFFFFFFFF);

  /* Install standard IRQ dispatcher at ARM IRQ vector */
  int_install_arm_vec_handler(IRQ_VEC, (PFV) lpc32xx_irq_handler);

  /* Enable I2C1/2 clock */
  clkpwr_clk_en_dis(CLKPWR_I2C1_CLK,1);
  clkpwr_clk_en_dis(CLKPWR_I2C2_CLK,1);

  /* install default I2C1 & I2C2 interrupt handlers */
  int_install_ext_irq_handler(IRQ_I2C_1, 
                              (PFV) i2c1_user_interrupt, ACTIVE_LOW, 1);
  int_install_ext_irq_handler(IRQ_I2C_2, 
                              (PFV) i2c2_user_interrupt, ACTIVE_LOW, 1);  

  /* Enable IRQ interrupts in the ARM core */
  enable_irq();

  /* open I2C1 */
  i2cdev1 = i2c_open(I2C1, 0);

  /* formally assign a 7-bit slave address 0x50 to I2C1    */
  /* I2C1 clock is 100 kHz, 50% duty cycle, high pin drive */
  i2c_setup[0].addr_mode  = ADDR7BIT;
//  i2c_setup[0].sl_addr    = 0x60;
  i2c_setup[0].rate_option= I2C_RATE_RELATIVE;
  i2c_setup[0].rate       = 100000;
  i2c_setup[0].low_phase  = 50;
  i2c_setup[0].high_phase = 50;
  i2c_setup[0].pins_drive = I2C_PINS_HIGH_DRIVE;
  i2c_ioctl((UNS_32) i2cdev1, I2C_SETUP, (UNS_32) &i2c_setup[0]);

  /* Write 9 bytes to PCA9532 and init registers, starting with reg 0x02 + auto increment = 0x10 */

  tx_data[0] = 0x12;								  
  tx_data[1] = 151;   /*PSC0 = blink 1s*/
  tx_data[2] = 256/3; /*PWM0 1/3 duty cycle*/
  tx_data[3] = 0;     /*PSC1 = blink 1/152s*/
  tx_data[4] = 256/32;/*PWM1 = 1/32 duty cycle*/
  tx_data[5] = 0x00;  /*LS0 outputs 0 - 3 High impedance*/
  tx_data[6] = 0x00;  /*LS1 outputs 4 - 7 High impedance*/
  tx_data[7] = 0xE4;  /*LS2 output 8 High impedance (LED1 OFF)
                            output 9 LOW (LED2 ON)
                            output 10 blinks at PWM0 rate 
                            output 11 blinks at PWM1 rate*/
                            
  tx_data[8] = 0xE4;  /*LS2 output 12 High impedance (LED1 OFF)
                            output 13 LOW (LED2 ON)
                            output 14 blinks at PWM0 rate 
                            output 15 blinks at PWM1 rate*/
  i2c_mtx_setup.addr_mode = ADDR7BIT;
  i2c_mtx_setup.sl_addr = 0x60;
  i2c_mtx_setup.tx_data = &tx_data[0];
  i2c_mtx_setup.tx_length = 9;
  i2c_mtx_setup.retransmissions_max = 10;
  i2c_ioctl(i2cdev1, I2C_MASTER_TX, (INT_32)&i2c_mtx_setup); 

  while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0 /*&& mtx_irq < 100000*/);

  i2c_close(i2cdev1);
}
