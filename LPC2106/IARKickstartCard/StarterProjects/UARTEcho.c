
/*
 * $Revision: 28 $
 */
 
/*
  UART Echo
  UART and button example for the Kickstart card.
  
  Echoes all bytes from UART1 and sends a * after each echoed byte.
  When button INT2 is pressed led DS3 is turned off. When it is pressed
  again, DS3 is turned on.

  Uses UART interrupts for both send and receive. Uses button interrupt
  to turn led DS3 on and off.

  The simplest way to run this example is to connect serial port P1 on the
  Kickstart card to a free serial port on your PC and run a terminal program.
  (If you run it using the ROM-monitor you need two serial ports on your PC.)

  Terminal settings:
  ------------------
  Communication speed: 9600 bps
  Number of data bits:    8
  Number of stop bits:    1
  Parity:              none
  
  Jumper settings:
  ----------------
  Set JP3 (enables UART1).
  Set JP5 (enables INT2 button).
  Set jumper p0.2 in the LED jumper block. (The other jumpers in the LED jumper
  block can be set as in the other examples, but then some other LED:s than DS3
  might flash.)

  For ROM-monitor:
  Set JP4 (enables UART0, used for ROM-monitor communication).

  For J-Link or Standalone:
  Set JP6 to JTAG1.
*/

#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include "timer.h"
#include "lpc210x.h"
#include <NXP/iolpc2106.h>

static unsigned char txbufemptyflag = 1;

static unsigned char sendbufferfullflag = 0;
static unsigned char sendbuffer;

static unsigned char receivebufferflag = 0;
static unsigned char receivebuffer = 0;


//Uses only one LED in this example
void initLED()
{
  PINSEL0_bit.P0_2 = 0;
  IODIR |= 0x00000004;
  IOCLR |= 0x0000ffff;
}

void turnLEDOn()
{
  IOSET_bit.P0_2 = 1;
}

void turnLEDOff()
{
  IOCLR_bit.P0_2 = 1;
}

// The handler for the INT2 button.
// Called from the IRQ handler exactly once for each button press.
void UserButtonHandler()
{
  static unsigned char ledflag = 1;
  ledflag ^= 1;  //Toggle ledflag
  if(ledflag)
  {
    //Turn on LED
    turnLEDOn();
  }
  else
  {
    //Turn off LED
    turnLEDOff();
  }
}

//Tx buf empty callback
void txBufEmpty()
{
  if(sendbufferfullflag)
  {
    //Sends the byte in sendbuffer
    sendbufferfullflag = 0;
    txbufemptyflag = 0;
    LPC210xUART1TxByte(sendbuffer);
  }
  else
  {
    txbufemptyflag = 1;
  }
}

//Byte received callback
void byteReceived(unsigned char byte)
{
  receivebufferflag = 1;
  receivebuffer = byte;
}

//Sends byte if possible, else places it in sendbuffer.
//Returns 1 on success, 0 if sendbuffer full.
unsigned char sendByte(unsigned char byte)
{
  if(sendbufferfullflag)
    return 0;
  
  if(txbufemptyflag)
  {
    // UART tx buffer empty, sends byte
    txbufemptyflag = 0;
    LPC210xUART1TxByte(byte);
  }
  else
  {
    // Puts byte in buffer
    sendbufferfullflag = 1;
    sendbuffer = byte;
  }
  return 1;
}

//Tries to get a byte from the UART.
//Result in *byte.
//Returns 1 if byte available, else 0.
unsigned char getByte(unsigned char *byte)
{
  if(receivebufferflag)
  {
    *byte = receivebuffer;
    receivebufferflag = 0;
    return 1;
  }
  return 0;
}


int main(void)
{
  unsigned char byte;

  // System initialization, this will map the exception vectors.
  LPC210xSystemInit();

  //Set up UART
  LPC210xInitUART1();

  //Set up a LED.
  initLED();

  // First disable interrupts.
  __disable_interrupt();
  // Setup interrupt controller.
  LPC210xInitVIC();
  LPC210xInitUART1Interrupt(byteReceived, txBufEmpty);
  LPC210xInitExternalInterrupt2(UserButtonHandler);

  // Setup external interrupt, used for button.
  LPC210xInitEINT2();

  // Enable interrupts.
  __enable_interrupt();

  // Turn on LED
  turnLEDOn();

  // Loop forever.
  for (;;)
  {
    if(getByte(&byte))
    {
      while(!sendByte(byte)); //Echo byte
      while(!sendByte('*'));  //Send *
    }
  }
}
