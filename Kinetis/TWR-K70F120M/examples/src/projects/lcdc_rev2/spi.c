/*
 * File:		lcdc.c
 * Purpose:		LCD Controller Validation Tests
 * Author:              Anthony Huereca
 *
 */

#include "spi.h"
#include "lptmr.h"

/********************************************************************/

void spi_init()
{
  /* clock gate */
  SIM_SCGC3 |=SIM_SCGC3_DSPI2_MASK ;

  //Turn on SPI2 Mux
  PORTD_PCR11 = PORT_PCR_MUX(2); //SPI2_PCS0
  PORTD_PCR12 = PORT_PCR_MUX(2); //SPI2_SCK
  PORTD_PCR13 = PORT_PCR_MUX(2); //SPI2_SOUT
  PORTD_PCR14 = PORT_PCR_MUX(2); //SPI2_SIN

  SPI2_MCR &= (~ SPI_MCR_MDIS_MASK);
  SPI2_MCR  = SPI_MCR_HALT_MASK | SPI_MCR_CLR_TXF_MASK | SPI_MCR_CLR_RXF_MASK | SPI_MCR_DIS_RXF_MASK;
  //SPI2_CTAR0  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(0);
  //SPI2_CTAR1  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(0);
  SPI2_CTAR0  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(2);
  SPI2_CTAR1  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(2);


  // Receive FIFO overflow disable
  SPI2_MCR |= SPI_MCR_ROOE_MASK;

  // Set CS0-7 inactive high
  SPI2_MCR |= SPI_MCR_PCSIS(0xFF);
  SPI2_MCR |= SPI_MCR_MSTR_MASK;

  // Disable interrupts
  SPI2_RSER = 0;

  // Clear all flags
  SPI2_SR |=  SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK;

  // Enable SPI
  SPI2_MCR &= (~ SPI_MCR_HALT_MASK);
}

/*
 * To set the LCD register to read/write, send out 0x70 and then the register #
 */
void SpiRegSet(unsigned char reg)
{
  unsigned short data;

  //Set the data to write out by concatenating 0x70 with the register value.
  data=0x7000 | reg;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0, and send out on MISO
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag
}


/*
 * To write data to a LCD register, send out 0x72 and then value
 * Make sure to set the register index first via SpiRegSet()
 */
void SpiSendDataWord(unsigned char value)
{
  unsigned short data;

  //Set the data to write out by concatenating 0x72 with the data to write
  data=0x7200 | value;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag
}


/*
 * To read data from a LCD register, this function sets the register index,
 * and then sends out 0x73 and wait for reply
 */
unsigned short SpiReadDataWord(unsigned char reg)
{
  unsigned short value, data;
  unsigned short data2;

  //Set register pointer
  data=0x7000 | reg;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag


  //Tell it to do a read by sending out 0x73
  data=0x7300;
  data2=0x0000;

  //Halt SPI from sending out anything
  SPI2_MCR  |= SPI_MCR_HALT_MASK;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0, write two words at once to have CW low long enough
  //  to let data be recieved.
  SPI2_PUSHR = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data2);

  //Read from TWR-MEM
  //SPI2_PUSHR = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA(0x9F00);
  //SPI2_PUSHR = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA(0x0000);

  //Send out both data words back to back using the FIFO
  SPI2_MCR &= (~ SPI_MCR_HALT_MASK);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  //while (!(SPI2_SR & SPI_SR_RCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag

  time_delay_ms(5);

  //Data returned from LCD should be in POPR now
  value = SPI2_POPR ;  //garbage
  value = SPI2_POPR ;  //good data

  return value;
}
