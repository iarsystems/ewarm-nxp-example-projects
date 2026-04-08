/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * spi_fifo_master.c
 ******************************************************************************/
#include "drivers.h"

/* SPI signal defines                                                         */
#define SPI1_SCK        PIN4            /* PTF4 */
#define SPI1_MOSI       PIN5            /* PTF5 */
#define SPI1_MISO       PIN6            /* PTF6 */

/* Callback functions declaration                                             */
static void syst_callback(void);
static void port_callback(PORT_CALLBACK_SRC src, uint8 pin);

uint16 spi_buff[2];                     /* Rx buffer                          */
uint16 spi_data0[2] = {0x0001, 0xFFFF}; /* Tx data buffer 0                   */
uint16 spi_data1[2] = {0x0002, 0xFFFF}; /* Tx data buffer 1                   */

volatile uint8_t msgFlag = 0;           /* Message flag                       */

void main (void)
{
  /* SIM initialization                                                       */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* Interrupt on button press initialization                                 */
  PORT_Init (PORTD, PORT_MODULE_BUTTON_IRQ_MODE, PIN0);
  GPIO_Init (GPIOD, GPIO_INP_MODE, PIN0);
  PORT_InstallCallback(PRI_LVL0, port_callback);

  /* SysTick timer init                                                       */
  SYST_Init (SYST_MODULE_RELOAD_TMR_DIV1_EN_CONFIG(1228), PRI_LVL0, syst_callback);

  /* SPI1 initialization                                                      */
  PORT_Init (PORTF, PORT_MODULE_ALT2_MODE, SPI1_MOSI | SPI1_MISO | SPI1_SCK);
  SPI_Init (SPI1, SPI_MODULE_DIV6_16B_FIFO_MASTER_CPHA_CONFIG, NULL, PRI_LVL1, NULL);

  /* Hold the SS pin low                                                      */
  PORT_Init (PORTF,PORT_MODULE_LED_MODE,PIN3);
  GPIO_Init (GPIOF, GPIO_OUT_LOGIC0_MODE, PIN3);
  GPIO_Clr  (GPIOF, PIN3);

  EnableInterrupts ();                   /* enable all interrupts             */

  while(1);
}

/* SysTick interrupt callback implementation                                  */
static void syst_callback(void)
{
  while(!SPI_TxCmpl(SPI1));
  if(msgFlag == 0)
  {
    SPI_PutWordToFifo(SPI1,spi_data0,2); /* Send data from spi_data0 buffer   */
  }
  else
  {
    SPI_PutWordToFifo(SPI1,spi_data1,2); /* Send data from spi_data1 buffer   */
  }
  if(!(SPI_RxFifoEmpty(SPI1)))           /* If RX FIFO buffer is not empty    */
  {
    SPI_GetWordFromFifo(SPI1,spi_buff,2);
  }
}

/* PORT interrupt callback implementation                                     */
static void port_callback(PORT_CALLBACK_SRC src, uint8 pin)
{
  if(src == PORTD_CALLBACK)
  {
    if(pin == PIN0) { msgFlag ^= 1; }
  }
}
