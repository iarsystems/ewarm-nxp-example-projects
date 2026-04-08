/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * spi_test.c
 ******************************************************************************/
#include "drivers.h"

/* SPI signal defines                                                         */
#define SPI1_SS         PIN3            /* PTF3 */
#define SPI1_SCK        PIN4            /* PTF4 */
#define SPI1_MOSI       PIN6            /* PTF6 */
#define SPI1_MISO       PIN5            /* PTF5 */

static void spi1_ss_callback (SS_CONTROL ss_pin); /* SS pin callback          */
uint8 spi_buff[16];                               /* Tx/Rx buffer             */

void main (void)
{
  /* SIM initialization                                                       */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* SPI1 initialization                                                      */
  PORT_Init (PORTF, PORT_MODULE_ALT2_MODE, SPI1_MOSI|SPI1_MISO|SPI1_SCK);
  SPI_Init (SPI1, SPI_MODULE_DIV12_8B_POLL_CONFIG, spi1_ss_callback);

  spi_buff[0] = 0x06;                             /* fill Tx buffer           */
  SPI_CommByte (SPI1,spi_buff,1,NULL,0);          /* 1 Tx byte, no receiving  */

  spi_buff[0] = 0x05;                             /* fill Tx buffer           */
  SPI_CommByte (SPI1,spi_buff,1,spi_buff,1);      /* 1 Tx byte, 1 Rx byte     */

  spi_buff[0] = 0x03; spi_buff[1] = 0x00;         /* fill Tx buffer           */

  SPI_CommByte (SPI1,spi_buff,2,spi_buff,8);      /* 2 Tx bytes, 8 Rx bytes   */

  while(1);
}

/* callback function implementation for manual driving of slave select pin    */
static void spi1_ss_callback (SS_CONTROL ss_pin)
{
  if      (ss_pin == SS_SET  ) { GPIO_Set (GPIOF,SPI1_SS); }
  else if (ss_pin == SS_CLEAR) { GPIO_Clr (GPIOF,SPI1_SS); }
  else if (ss_pin == SS_INIT )
  {
    PORT_Init (PORTF, PORT_MODULE_LED_MODE, SPI1_SS);
    GPIO_Init (GPIOF, GPIO_OUT_LOGIC0_MODE, SPI1_SS);
    GPIO_Set  (GPIOF, SPI1_SS);
  }
}
