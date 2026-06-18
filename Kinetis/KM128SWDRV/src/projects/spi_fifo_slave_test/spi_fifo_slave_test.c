/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
*******************************************************************************
 * spi_fifo_slave_test.c
*******************************************************************************/
#include "drivers.h"

/* SPI signal defines                                                         */
#define SPI1_SS     PIN3  /* PTF3 */
#define SPI1_SCK    PIN4  /* PTF4 */
#define SPI1_MOSI   PIN5  /* PTF5 */
#define SPI1_MISO   PIN6  /* PTF6 */

/* RX FIFO Buffer near full flag callback                                     */
static void spi1_rxnf_callback (SPI_CALLBACK_SRC module, SPI_CALLBACK_TYPE type);

uint16 spi_buff[2];                           /* Rx buffer                    */
uint16 spi_data0[2] = {0x06, 0xFFFF};         /* Tx data buffer 0             */
uint16 spi_data1[2] = {0x07, 0xFF00};         /* Tx data buffer 1             */

void main (void)
{
  /* SIM initialization                                                       */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);/* Full configuration           */

  /* SPI1 initialization                                                      */
  PORT_Init (PORTF, PORT_MODULE_ALT2_MODE, SPI1_MOSI|SPI1_MISO|SPI1_SCK|SPI1_SS);
  SPI_Init  (SPI1, SPI_MODULE_DIV6_16B_FIFO_SLAVE_CPHA_CONFIG,NULL,PRI_LVL0,
             spi1_rxnf_callback);

  EnableInterrupts ();                        /* Enable all interrupts        */

  while(1);
}

/* SPI interrupt callback implementation                                      */
void spi1_rxnf_callback (SPI_CALLBACK_SRC module, SPI_CALLBACK_TYPE type)
{
  if (module == SPI1_CALLBACK)
  {
    if( type == SPI_RXFNF_CALLBACK)           /* RX FIFO Buffer near full IRQ */
    {
      SPI_GetWordFromFifo(SPI1,spi_buff,2);   /* Store data from FIFO Buffer  */
      /* If the received word is equal to "0xFFFF0001"                        */
      if((*(unsigned long*)spi_buff) == 0xFFFF0001)
      {
        /* Fill the TX FIFO Buffer by content of the spi_data0 buffer         */
        SPI_PutWordToFifo(SPI1,spi_data0,2);
      }
      /* If the received word is equal to "0xFFFF0002"                        */
      else if((*(unsigned long*)spi_buff) == 0xFFFF0002)
      {
        /* Fill the TX FIFO Buffer by content of the spi_data1 buffer         */
        SPI_PutWordToFifo(SPI1,spi_data1,2);
      }
    }
  }
}
