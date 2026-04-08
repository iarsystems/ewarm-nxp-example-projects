
#include <stdio.h>
#include "lpc_types.h"
#include "lpc313x_i2c_driver.h"
#include "joystick.h"

extern INT_32 dev_uart;
/* device handles */
static INT_32 dev_i2c0;

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
  rx_data[0] = 0;

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


  return ((~rx_data[0]) & 0x1f);
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

void joyInit(void)
{
  /* open I2C0 */
  dev_i2c0 = i2c_open((void *)I2C0, 0);

  /* initialize pca9532 */
  pca9532_init();


}

UNS_8 joyState(void)
{  
  return readKeys();
}

