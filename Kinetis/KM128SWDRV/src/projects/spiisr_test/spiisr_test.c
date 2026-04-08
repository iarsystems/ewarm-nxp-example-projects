/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * spiisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* SPI signal defines                                                         */
#define SPI0_SS         PIN1            /* PTD1 */
#define SPI0_SCK        PIN2            /* PTD2 */
#define SPI0_MOSI       PIN3            /* PTD3 */
#define SPI0_MISO       PIN4            /* PTD4 */

/* callback function                                                          */
static void spi0_callback (SPI_CALLBACK_SRC module, SPI_CALLBACK_TYPE type);

/* global variables                                                           */
uint16 spiBuff[16];                               /* Tx/Rx buffer             */
uint16 spi_tx_cnt, spi_rx_cnt, spi_cnt, spi_cnt2; /* SPI counters             */

void main (void)
{
  /* SIM initialization                                                       */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);     /* full configuration       */

  /* SPI0 initialization                                                      */
  PORT_Init (PORTD, PORT_MODULE_ALT3_MODE, SPI0_MOSI | SPI0_MISO | SPI0_SCK);
  SPI_Init  (SPI0, SPI_MODULE_DIV2_8B_IRQ_CONFIG,NULL, PRI_LVL1, spi0_callback);

  /* SS pin configuration as a basic GPIO pin                                 */
  PORT_Init (PORTD, PORT_MODULE_LED_MODE, SPI0_SS);
  GPIO_Init (GPIOD, GPIO_OUT_LOGIC0_MODE, SPI0_SS);
  GPIO_Set  (GPIOD, SPI0_SS);

  EnableInterrupts();     /* enable all interrupts                            */

  spi_tx_cnt = 1; spi_rx_cnt = 0; spiBuff[0]=0x06; spi_cnt=0;
  SPI_TxIsrEnable(SPI0);  /* start the 1st packet with 1 Tx byte, no Rx byte  */

  spi_tx_cnt = 1; spi_rx_cnt = 1; spiBuff[0]=0x05; spi_cnt=0;
  SPI_TxIsrEnable(SPI0);  /* start the 2nd packet with 1 Tx byte, 1 Rx byte   */

  spi_tx_cnt = 2; spi_rx_cnt = 8; spiBuff[0]=0x03; spiBuff[1]=0; spi_cnt=0;
  SPI_TxIsrEnable(SPI0);  /* start the 3rd packet with 2 Tx bytes, 8 Rx bytes */

  while(1);
}

/* SPI interrupt callback implementation                                      */
#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
static void spi0_callback  (SPI_CALLBACK_SRC module, SPI_CALLBACK_TYPE type)
{
  if (module == SPI0_CALLBACK)
  {
    if (type == SPI_TX_CALLBACK)        /* TX IRQ                             */
    {
      if (spi_tx_cnt) {                 /* Tx state?                          */
        GPIO_Clr  (GPIOD,SPI0_SS );     /* managing the slave select          */
        SPI_PutByte(SPI0,spiBuff[spi_cnt]);   /* sending the data             */
        spi_cnt++;
        if (!--spi_tx_cnt)              /* to recognize last Tx byte          */
          spi_cnt = (uint16)-1;
      }
      else {
        if (spi_rx_cnt) {               /* Rx state?                          */
          spi_rx_cnt--;                 /* Rx counter is decremented here!    */
          SPI_PutByte(SPI0,0xFF);       /* dummy Tx data in case of reading   */
        }
      }
    }
    if (type == SPI_RX_CALLBACK)        /* RX IRQ                             */
    {
      register uint8 temp;
      temp = SPI_GetByte(SPI0);         /* save received byte to temp         */
      if (!spi_tx_cnt) {                /* transmit or receive state now?     */
        if (spi_cnt == (uint16)-1)      /* 1-st Rx byte after transmitting    */
          spi_cnt++;
        else spiBuff[spi_cnt++] = temp; /* save Rx byte to buffer             */
      }
      if (!spi_rx_cnt) {                /* end of receiving?                  */
        SPI_TxIsrDisable(SPI0);         /* disable Tx IRQ                     */
        GPIO_Set  (GPIOD,SPI0_SS );     /* managing the slave select          */
      }
    }
    if (type == SPI_MATCH_CALLBACK)     /* MATCH IRQ                          */
    {
      /* Place your code here                                                 */
    }
    if (type == SPI_FAULT_CALLBACK)     /* FAULT (error) IRQ                  */
    {
      /* Place your code here                                                 */
    }
  }
}
