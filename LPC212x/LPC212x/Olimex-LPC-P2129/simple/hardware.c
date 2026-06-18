#include "hardware.h"

/*-------------------------------------------------------------------------------------------------------------------------------*/
void Delay(U32 del)
{
  while (--del!=0);
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void CPUinit(void)
{
  PLLCFG_bit.MSEL=3;                 //M=3(4?),P=1(2?) PLLfreq = 235,9296 Mhz, CCLK = 58,9824 Mhz
  PLLCFG_bit.PSEL=1;
  PLLFEED=0xAA;
  PLLFEED=0x55;
  PLLCON_bit.PLLE=1;                 // Enable the PLL
  PLLFEED=0xAA;
  PLLFEED=0x55;
  while(!(PLLSTAT & PLOCK));  // Wait for PLL to lock

  //Init MAM & Flash memory fetch
  MAMCR_bit.MODECTRL=2;
  MAMTIM_bit.CYCLES=4;
  APBDIV_bit.APBDIV=1;   //pclk = cclk

  //GPIO init
  A_PULL_UP;
  A_LED1_OFF;
  A_LED2_OFF;
  PINSEL0=0; //P0 lower 16 bits all GPIO
  PINSEL1=0; //P0 upper 16 bits all GPIO

  //IODIR in binary 0000 0001 0000 0000 0011 0001 0000 0001     ...P0.12, P0.13 are output(LEDs) also TXD0,TXD1,TD2
  IO0DIR=0x01003101;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
U16 getprocessorClockFreq(void)
{
  return OSCILLATOR_CLOCK_FREQUENCY * (PLLCON & 1 ? (PLLCFG & 0xF) + 1 : 1);
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
U16 getperipheralClockFreq(void)
{
  U16 divider;
  switch (APBDIV_bit.APBDIV)
    {
      case 0:
        divider = 4;
        break;
      case 1:
        divider = 1;
        break;
      case 2:
        divider = 2;
        break;
    }
  return getprocessorClockFreq() / divider;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
//UART Driving functions
/*-------------------------------------------------------------------------------------------------------------------------------*/
void UART0Init(U16 baud)
{
  U16 divisor = getperipheralClockFreq() / (16 * baud);
  U0LCR_bit.DLAB=1; //Enable DLAB
  U0LCR_bit.WLS=3;  //8 bits
  U0LCR_bit.SBS=1;  //1 stop bit
  U0DLL = LSB(divisor);
  U0DLM = MSB(divisor);
  U0LCR_bit.DLAB=0; //Disable DLAB
  PINSEL0 = PINSEL0 & ~0xF | 0x5;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void UART1Init(U16 baud)
{
  U16 divisor = getperipheralClockFreq() / (16 * baud);
  U1LCR_bit.DLAB=1; //Enable DLAB
  U1LCR_bit.WLS=3;  //8 bits
  U1LCR_bit.SBS=1;  //1 stop bit
  U1DLL = LSB(divisor);
  U1DLM = MSB(divisor);
  U1LCR_bit.DLAB=0; //Disable DLAB
  PINSEL0_bit.P0_8=1;
  PINSEL0_bit.P0_9=1;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void UART0WriteChar(U16 chr)
{
  while (U0LSR_bit.THRE==0);
  U0THR = chr;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void UART1WriteChar(U16 chr)
{
  while (U1LSR_bit.THRE==0);
  U1THR = chr;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
U16 UART0ReadChar(void)
{
  while (U0LSR_bit.RDR==0);
  return U0RBR;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
U16 UART1ReadChar(void)
{
  while (U1LSR_bit.RDR==0);
  return U1RBR;
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void __putchar0(U8 ch0)
{
  while (ch0 == '\n')
  UART0WriteChar('\r');
  UART0WriteChar(ch0);
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void __putchar1(U8 ch1)
{
  if (ch1 == '\n')
  UART1WriteChar('\r');
  UART1WriteChar(ch1);
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
