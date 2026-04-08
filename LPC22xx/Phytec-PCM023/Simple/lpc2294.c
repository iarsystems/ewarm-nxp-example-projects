/*
  File: LPC2294.c

  To provide the lpc2294 specific uart, i/o port and timer interface
  function.

*/

#include <intrinsics.h>
#include "lpc2294.h"
#include <NXP/iolpc2294.h>

#define BAUDRATE    9600
#define BAUDRATEDIVISOR (PCLKFREQ/(BAUDRATE*16))

#define VIC_TIMER0_bit (1 << VIC_TIMER0)
#define VIC_UART0_bit  (1 << VIC_UART0)

// Pointers to interrupt callback functions.
static void (*timer_function)(void);
static void (*uart0rx_function)(unsigned char);
static void (*uart0tx_function)(void);

//
// Interrupt handlers.
//

//Dummy interrupt handler, called as default in irqHandler() if no other
//vectored interrupt is called.
static void DefDummyInterrupt(void)
{}

// Timer interrupt handler
static void TimerInterrupt(void)
{
  (*timer_function)(); // Call timer callback function.

  T0IR = 0xff; // Clear timer 0 interrupt line.
}

//UART0 interrupt handler
static void UART0Interrupt(void)
{
  switch(U0IIR_bit.IID)
  {
  case 0x1:  //THRE interrupt
    (*uart0tx_function)(); //Call tx buffer empty callback function
    break;
  case 0x2:  //Receive data available
    (*uart0rx_function)(U0RBR);    //Call received byte callback function
    break;
  case 0x0:  //Modem interrupt
  case 0x3:  //Receive line status interrupt (RDA)
  case 0x6:  //Character time out indicator interrupt (CTI)
  default:
    break;
  }
}

// IRQ exception handler. Calls the interrupt handlers.
__irq __arm void IRQ_Handler(void)
{
  void (*interrupt_function)();
  unsigned int vector;

  vector = VICVectAddr;   // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function.

  VICVectAddr = 0;        // Clear interrupt in VIC.
}


//
// System initialization.
//
void LPC2294SystemInit(void)
{
#ifdef iRAM
  MEMMAP = 2;             // Map interrupt vectors to internal ram
#else
#ifdef iFLASH             // Map interrupt vectors to internal flash
  MEMMAP = 1;
#else
  BCFG0 = 0x20003CE3;     // BCFG0: Flash Bus Configuration
  BCFG1 = 0x20003CE3;     // BCFG1: Ram Bus Configuration
  PINSEL2 = 0x0E6149E4;   // PINSEL2: CS0, CS1, CS2, OE, WE, BLS0..3, D0..31, A2..23, JTAG
#ifdef xFLASH
  MEMMAP = 3;             // Map interrupt vectors to the first external device (flash in this case)
#else
  MEMMAP = 2;             // Map interrupt vectors to internal ram for debugging from external ram
#endif
#endif
#endif
}

//
// Interrupt controller initalization.
//

// Reset all interrupts
void LPC2294InitVIC()
{
  // Setup interrupt controller.
  VICProtection = 0;
  // Disable all interrupts
  VICIntEnClear = 0xffffffff;
  VICDefVectAddr = (unsigned int)&DefDummyInterrupt;
}

// Setup Timer interrupt
void LPC2294InitTimerInterrupt(void(*timer_func)())
{
  // Setup timer callback function.
  timer_function = timer_func;

  VICIntSelect &= ~VIC_TIMER0_bit; // IRQ on timer 0 line.
  VICVectAddr1 = (unsigned int)&TimerInterrupt;
  VICVectCntl1 = 0x20 | VIC_TIMER0; // Enable vector interrupt for timer 0.
  VICIntEnable = VIC_TIMER0_bit;    // Enable timer 0 interrupt.
}

// Setup UART interrupt
void LPC2294InitUART0Interrupt(void(*uart0rx_func)(unsigned char),
                               void(*uart0tx_func)())
{
  // Setup uart1 callback functions.
  uart0rx_function = uart0rx_func;
  uart0tx_function = uart0tx_func;

  VICIntSelect &= ~VIC_UART0_bit;  // IRQ on UART0.
  VICVectAddr5 = (unsigned int)&UART0Interrupt;
  VICVectCntl5 = 0x20 | VIC_UART0; // Enable vector interrupt for UART0.
  VICIntEnable = VIC_UART0_bit;    // Enable UART 0 interrupt.
}

//
// Timer functions.
//

// Setup Timer
void LPC2294InitTimer()
{
  T0TCR = 0; // Disable timer 0.
  T0TCR = 2; // Reset timer 0.
  T0TCR = 0;
  T0IR = 0xff; // Clear timer 0 interrupt line.
  T0PR = 0; // Prescaler is set to no division.
  T0MR0 = PCLKFREQ / 300; // Count up to this value. To generate 1000KHz.
  T0MCR = 3; // Reset and interrupt on MR0 (match register 0).
  T0CCR = 0; // Capture is disabled.
  T0EMR = 0; // No external match output.
}

// Start Timer
void LPC2294StartTimer()
{
  T0TCR = 1; // Enable timer 0.
}

//
// Parallel I/O functions.
//

void LPC2294InitPIO()
{
  //Uses GPIO pins P0.8
  PINSEL0_bit.P0_8 = 0x00;
  IO0DIR = 0x00000100;      /* P0.8 defined as Output */
}

//
// LED output drivers.
//

void LPC2294LedClear(void)
{
  IO0SET = 0x00000100;  /* Turn LED Off (P0.8 = 1) */
}

void LPC2294LedSet(void)
{
  IO0CLR = 0x00000100;  /* Turn LED On  (P0.8 = 0) */
}

//
// UART functions
//

void LPC2294InitUART0()
{
  //Set pins for use with UART
  PINSEL0_bit.P0_0 = 0x01;                  /* Enable RxD0 and TxD0              */
  PINSEL0_bit.P0_1 = 0x01;                  /* Enable RxD0 and TxD0              */

  //Set the FIFO enable bit in the FCR register. This bit must be set for
  //proper UART operation.
  U0FCR = 1;

  //Set baudrate
  U0LCR_bit.DLAB = 1;
  U0DLL = BAUDRATEDIVISOR & 0x00ff;
  U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
  U0LCR_bit.DLAB = 0;

  //Set mode
  U0LCR_bit.WLS = 0x3;   //8 bit word length
  U0LCR_bit.SBS = 0x0;   //1 stop bit
  U0LCR_bit.PE  = 0x0;   //No parity

  //Enable UART0 interrupts
  U0IER_bit.RDAIE  = 1;  //Enable byte received interrupt
  U0IER_bit.THREIE = 1;  //Enable tx buf empty interrupt
}

//Transmits one byte via UART0
void LPC2294UART0TxByte(unsigned char byte)
{
  while(U0LSR_bit.THRE != 1);
    U0THR = byte;
}

