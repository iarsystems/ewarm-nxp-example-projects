
#include <stdio.h>
#include <string.h>
#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc313x_chip.h"
#include "ea3131_board.h"
#include "lpc313x_intc_driver.h"
#include "lpc313x_cgu_driver.h"
#include "lpc313x_i2c_driver.h"
#include "lpc313x_uart_driver.h"
#include "lpc313x_timer_driver.h"
#include "lpc313x_adc10b_driver.h"


/* device handles */
static INT_32 dev_i2c0;
static INT_32 dev_uart;
static INT_32 dev_adc;


/* buffer for the uart when printing strings */
static char buff[512];       

/* Joystick defines */
#define KEY_CENTER 0x01
#define KEY_UP     0x02
#define KEY_RIGHT  0x04
#define KEY_LEFT   0x08
#define KEY_DOWN   0x10


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
static void uart_string_write(INT_32 devid, char *pbuff)
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
  i2c_ioctl(dev_i2c0, I2C_MASTER_TXRX, (INT_32)&i2c_mtxrx_setup);
  
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
  i2c_ioctl(dev_i2c0, I2C_MASTER_TX, (INT_32)&i2c_mtx_setup);
  while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
}

/***********************************************************************
 *
 * Function: readKeys
 *
 * Purpose: Read the state of the joystick keys
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Joystick key state (any of the KEY_* values)
 *
 * Notes: None
 *
 **********************************************************************/
static UNS_8
readKeys(void)
{
  static volatile I2C_MTXRX_SETUP_T i2c_mtxrx_setup;
  UNS_8 rx_data[2];
  UNS_8 tx_data[2];

  tx_data[0] = 0x00;

  //read current register value
  i2c_mtxrx_setup.addr_mode = ADDR7BIT;
  i2c_mtxrx_setup.sl_addr = 0xc0 >> 1;
  i2c_mtxrx_setup.tx_data = &tx_data[0];
  i2c_mtxrx_setup.tx_length = 1;
  i2c_mtxrx_setup.rx_data = &rx_data[0];
  i2c_mtxrx_setup.rx_length = 1;
  i2c_mtxrx_setup.retransmissions_max = 1;
  i2c_ioctl(dev_i2c0, I2C_MASTER_TXRX, (INT_32)&i2c_mtxrx_setup);
  
  while (( i2c_mtxrx_setup.status & I2C_SETUP_STATUS_DONE) == 0);

  return rx_data[0];
}

/***********************************************************************
 *
 * Function: adcRead
 *
 * Purpose: ADC read
 *
 * Parameters:
 *    devid : ADC device ID
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void adcRead(INT_32 devid)
{
  ADC_REGS_T *adcregsptr;
  ADC_CFG_T *adccfgptr = (ADC_CFG_T *) devid;
  adcregsptr = adccfgptr->regptr;

  /* start conversion scan */
  adc_ioctl(devid, ADC_START_CONVERSION, 0);

  /* Wait until a conversion is done*/
  while(adcregsptr->adc_int_status_reg == 0);
 
  /* clear interrupt */
  adcregsptr->adc_int_clear_reg = 1;

  if (adcregsptr->adc_csel_reg & 0x00000F00)
    adccfgptr->rx[2] = (UNS_16) adcregsptr->adc_r2_reg & ADCDAT_VALUE_MASK;

  while(adcregsptr->adc_int_status_reg == 0);
  
  adc_ioctl(dev_adc, ADC_STOP_CONVERSION, 0);
}

/***********************************************************************
 *
 * Function: pca9532_init
 *
 * Purpose: Initialize PCA9532
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
static void pca9532_init(void)
{
#define DATA_LENGTH		16
  static I2C_SETUP_T i2c_setup[2];
  static volatile I2C_MTX_SETUP_T   i2c_mtx_setup;


  UNS_8  tx_data[DATA_LENGTH];

  /* formally assign a 7-bit slave address 0xc0 to I2C1    */
  /* I2C1 clock is 100 kHz, 50% duty cycle, high pin drive */
  /* This is a master only test, for both TX and RX. 
  Slave address is used to generated packet header on the bus,
  not used for slave address register setting. */
  i2c_setup[0].addr_mode = ADDR7BIT;
//  i2c_setup[0].sl_addr    = 0xc0 >> 1;
  i2c_setup[0].rate_option= I2C_RATE_RELATIVE;
  i2c_setup[0].rate       = 100000;
  i2c_setup[0].low_phase  = 50;
  i2c_setup[0].high_phase = 50;
  i2c_ioctl(dev_i2c0, I2C_SETUP, (INT_32)&i2c_setup[0]);


  /* Write 9 bytes to PCA9532 and init registers, starting with reg 0x02 + auto increment = 0x10 */

  tx_data[0] = 0x12;								  
  tx_data[1] = 0x97;
  tx_data[2] = 0x40;
  tx_data[3] = 0x00;
  tx_data[4] = 0x04;
  tx_data[5] = 0x00;
  tx_data[6] = 0x00;
  tx_data[7] = 0x00;
  tx_data[8] = 0; //0x4b;
  i2c_mtx_setup.addr_mode = ADDR7BIT;
  i2c_mtx_setup.sl_addr = 0xc0 >> 1;
  i2c_mtx_setup.tx_data = &tx_data[0];
  i2c_mtx_setup.tx_length = 9;
  i2c_mtx_setup.retransmissions_max = 1;
  i2c_ioctl(dev_i2c0, I2C_MASTER_TX, (INT_32)&i2c_mtx_setup); 

  while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
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
 * Function: adc_init
 *
 * Purpose: Initialize ADC
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
static void adc_init(void)
{
  dev_adc = adc_open((void*)ADC_BASE, 0);
  adc_ioctl(dev_adc, ADC_SELECT_CHANNEL_2, 10); 
  adc_ioctl(dev_adc, ADC_SELECT_CONTINUOUS_SCAN_MODE, 0); 

  /* start A/D convert */
//  adc_ioctl(dev_adc, ADC_START_CONVERSION, 0);

  /* stop A/D convert */
//  adc_ioctl(dev_adc, ADC_STOP_CONVERSION, 0);  

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
  UNS_8 ledCnt = 1;
  INT_8 keys = 0;
  INT_8 dir = 0;
  INT_32 delay = 0;
  ADC_CFG_T *adccfgptr = NULL;
  UNS_16 lastAdcValue = 0;

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

  /* set base frequency for the domain AHB0_APB1 belongs to PLL1*/
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

  /* open I2C0 */
  dev_i2c0 = i2c_open((void *)I2C0, 0);

  /* initialize pca9532 */
  pca9532_init();

  /* initialize adc */
  adc_init();
  adccfgptr = (ADC_CFG_T*)dev_adc;

  delay = 100;
  dir = 0;

  sprintf(buff, "Starting loop\r\n");
  uart_string_write(dev_uart, buff); 

  while(1)
  {

    if (dir == 1) {

      switch (ledCnt) {
      case 1:  setLed(1, 1); ledCnt++; break;
      case 2:  setLed(2, 1); ledCnt++; break;
      case 3:  setLed(3, 1); ledCnt++; break;
      case 4:  setLed(4, 1); ledCnt++; break;
      case 5:  setLed(5, 1); ledCnt++; break;
      case 6:  setLed(6, 1); ledCnt++; break;
      case 7:  setLed(7, 1); ledCnt++; break;
      case 8:  setLed(8, 1); ledCnt++; break;
      case 9:  setLed(1, 0); ledCnt++; break;
      case 10: setLed(2, 0); ledCnt++; break;
      case 11: setLed(3, 0); ledCnt++; break;
      case 12: setLed(4, 0); ledCnt++; break;
      case 13: setLed(5, 0); ledCnt++; break;
      case 14: setLed(6, 0); ledCnt++; break;
      case 15: setLed(7, 0); ledCnt++; break;
      case 16: setLed(8, 0); ledCnt=1; break;
      default: ledCnt=1; break;
      }
    }
    else {
      switch (ledCnt) {
      case 1:  setLed(8, 1); ledCnt++; break;
      case 2:  setLed(7, 1); ledCnt++; break;
      case 3:  setLed(6, 1); ledCnt++; break;
      case 4:  setLed(5, 1); ledCnt++; break;
      case 5:  setLed(4, 1); ledCnt++; break;
      case 6:  setLed(3, 1); ledCnt++; break;
      case 7:  setLed(2, 1); ledCnt++; break;
      case 8:  setLed(1, 1); ledCnt++; break;
      case 9:  setLed(8, 0); ledCnt++; break;
      case 10: setLed(7, 0); ledCnt++; break;
      case 11: setLed(6, 0); ledCnt++; break;
      case 12: setLed(5, 0); ledCnt++; break;
      case 13: setLed(4, 0); ledCnt++; break;
      case 14: setLed(3, 0); ledCnt++; break;
      case 15: setLed(2, 0); ledCnt++; break;
      case 16: setLed(1, 0); ledCnt=1; break;
      default: ledCnt=1; break;
      }
    }


    /* control the speed of the LEDs from the Trimpot */
    adcRead(dev_adc);
    if (adccfgptr->rx[2] > lastAdcValue + 10) {

  	  if (delay > 30)
  	    delay -= 10;

    } else if (adccfgptr->rx[2] < lastAdcValue - 10) {

  	  if (delay < 200)
  	    delay += 10;

    }
    lastAdcValue = adccfgptr->rx[2];

    timer_wait_ms(NULL, delay);

  	keys = readKeys();

  	/* check if center key pressed -> stop the leds... */
  	if ((keys & KEY_CENTER) == 0)
  	{
  	  while ((readKeys() & KEY_CENTER) == 0);
  	}
  
  	/* check if right key pressed -> direction = right */
  	if ((keys & KEY_RIGHT) == 0)
  	{
  	  dir = 1;
  	}
  
  	/* check if up key pressed -> increase speed */
  	if ((keys & KEY_UP) == 0)
  	{
  	  if (delay > 30)
  	    delay -= 10;
  	}
  
  	/* check if down key pressed -> decrease speed */
  	if ((keys & KEY_DOWN) == 0)
  	{
  	  if (delay < 200)
  	    delay += 10;
  	}
  
  	/* check if left key pressed -> direction = left */
  	if ((keys & KEY_LEFT) == 0)
  	{
  	  dir = 0;
  	}
    
  }

}
