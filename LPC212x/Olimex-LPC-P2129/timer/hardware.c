#include "hardware.h"

#define FOSC 14745600
#define VALUEOFM 4            //implies PLLCFG_bit.MSEL=3
#define CCLK (FOSC*VALUEOFM)  //58.9824 MKz
#define PCLK (CCLK/4)         //implies VPBDIV_bit.VPBDIV=0;

//FCCO = 2 * VALUEOFP = 156-302 MHz, so VALUEOFP=2 implies PLLCFG_bit.PSEL=1;

#define BAUDRATE    9600
#define BAUDRATEDIVISOR (PCLKFREQ/(BAUDRATE*16))
#define VIC_TIMER0_bit (1 << VIC_TIMER0)
#define VIC_EINT0_bit  (1 << VIC_EINT0)
#define VIC_EINT1_bit  (1 << VIC_EINT1)
#define VIC_EINT2_bit  (1 << VIC_EINT2)
#define VIC_UART1_bit  (1 << VIC_UART1)

/*-------------------------------------------------------------------------------------------------------------------------------*/
void CPUinit(void)
{
  PLLCFG_bit.MSEL=3;    //See PLL calculations in #define section at top of this file
  PLLCFG_bit.PSEL=1;    //See PLL calculations in #define section at top of this file
  PLLFEED=0xAA;
  PLLFEED=0x55;
  PLLCON_bit.PLLE=1;                 // Enable the PLL
  PLLFEED=0xAA;
  PLLFEED=0x55;
  while(!(PLLSTAT & PLOCK));  // Wait for PLL to lock

  //Init MAM & Flash memory fetch
  MAMCR_bit.MODECTRL=2;
  MAMTIM_bit.CYCLES=4;
  APBDIV_bit.APBDIV=0;   //See PLL calculations in #define section at top of this file

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
//INTERRUPT initialisation and handling functions
/*-------------------------------------------------------------------------------------------------------------------------------*/
void INTERRUPTSinit(void)
{
#ifdef SRAM_VIA_JLINK
  MEMMAP = 2;
#endif
  //PINSEL1_bit.P0_16=0x1; // Set P16 pin to generate EINT0
  //PINSEL0_bit.P0_14=0x2; // Set P14 pin to generate EINT1
  //PINSEL0_bit.P0_15=0x2; // Set P15 pin to generate EINT2
  // Setup interrupt controller.
  VICProtection = 0;

  // Disable ALL interrupts
  VICIntEnClear = 0xffffffff;
  VICDefVectAddr = (U16)&DefDummyInterrupt;
  VICDefVectAddr = (U16)&TimerInterrupt;
  VICIntSelect &= ~VIC_TIMER0_bit; // IRQ on timer 0 line.
  VICVectAddr1 = (U16)&TimerInterrupt;
  VICVectCntl1 = 0x20 | VIC_TIMER0; // Enable vector interrupt for timer 0.
  VICIntEnable = VIC_TIMER0_bit;    // Enable timer 0 interrupt.

  //VICIntSelect &= ~VIC_EINT0_bit;  // P16 Pin can be switched for int
  //VICVectAddr2 = (U16)&ExternalInterrupt0;
  //VICVectCntl2 = 0x20 | VIC_EINT0; // Enable vector interrupt for EINT 0.
  //VICIntEnable = VIC_EINT0_bit;    // Enable EINT 0 interrupt.

  //VICIntSelect &= ~VIC_EINT1_bit;  // P14 Pin can be switched for int
  //VICVectAddr3 = (U16)&ExternalInterrupt1;
  //VICVectCntl3 = 0x20 | VIC_EINT1; // Enable vector interrupt for EINT1.
  //VICIntEnable = VIC_EINT1_bit;    // Enable EINT 1 interrupt.

  //VICIntSelect &= ~VIC_EINT2_bit;  // P15 Pin can be switched for int
  //VICVectAddr4 = (U16)&ExternalInterrupt2;
  //VICVectCntl4 = 0x20 | VIC_EINT2; // Enable vector interrupt for EINT2.
  //VICIntEnable = VIC_EINT2_bit;    // Enable EINT 2 interrupt.

  //VICIntSelect &= ~VIC_UART1_bit;  // IRQ on UART1.
  //VICVectAddr5 = (U16)&UART1Interrupt;
  //VICVectCntl5 = 0x20 | VIC_UART1; // Enable vector interrupt for UART1.
  //VICIntEnable = VIC_UART1_bit;    // Enable UART 1 interrupt.
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
__irq __arm void IRQ_Handler(void)
{
  void (*interrupt_function)();
  U16 vector;

  vector = VICVectAddr;                     // GET VECTOR ADDRESS FROM VIC CONTROLLER
  interrupt_function = (void(*)())vector;

  (*interrupt_function)();                  // Call vectored interrupt function.

  VICVectAddr = 0;
  //A_LED1_OFF;                          // Clear interrupt in VIC.
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
static void DefDummyInterrupt()
{
}

//Default interrupt handler, called as default in irqHandler()
/*-------------------------------------------------------------------------------------------------------------------------------*/
static void TimerInterrupt()
{
  static U8 led_old_state;

  if (led_old_state==OFF)
  {
    A_LED1_ON;
    A_LED2_OFF;
    led_old_state=ON;
  }
  else
  {
  A_LED1_OFF;
  A_LED2_ON;
  led_old_state=OFF;
  }
  T0IR = 0xff; // Clear timer 0 interrupt line.
}

/*-----------------------------------------------------------------------------*/
//static void ExternalInterrupt0()
//{
//EXTINT_bit.EINT0 = 1;          // Try to reset external interrupt flag.
//if(EXTINT_bit.EINT0) return;   // If flag was reset then Interrupt has passed
//A_LED1_ON;
//}
/*-----------------------------------------------------------------------------*/
//static void ExternalInterrupt1()
//{
//EXTINT_bit.EINT1 = 1;         // Try to reset external interrupt flag.
//if(EXTINT_bit.EINT1) return;  // If flag was reset then Interrupt has passed
//A_LED1_OFF;
//}
/*-----------------------------------------------------------------------------*/
//static void ExternalInterrupt2()
//{
//EXTINT_bit.EINT2 = 1;         // Try to reset external interrupt flag.
//if(EXTINT_bit.EINT2) return;  // If flag was reset then Interrupt has passed
//A_LED2_ON;
//}
/*-----------------------------------------------------------------------------*/
//static void UART1Interrupt()
//{
//  switch(U1IIR_bit.IID)
//  {
//  case 0x1:  //THRE interrupt
//    A_LED1_ON;   //tx buffer empty handling code
//    break;
//  case 0x2:  //Receive data available
//    A_LED2_ON;     //received byte handling code
//    break;
//  case 0x0:  //Modem interrupt
//  case 0x3:  //Receive line status interrupt (RDA)
//  case 0x6:  //Character time out indicator interrupt (CTI)
//  default:
//    break;
//  }
//}

/*-------------------------------------------------------------------------------------------------------------------------------*/
void TIMER0initstart()
{
  T0TCR = 0; // Disable timer 0 (1 bit register!)
  T0CCR = 0; // All Capture types disabled
  T0EMR = 0; // All external match type disabled
  T0PC = 0; // Prescaler is set to no division.
  T0MR0 = PCLK/4; // Count up to this value. PCLK would be 1 Hz
  T0MCR = 3; // Reset and interrupt on MR0 (match register 0).
  //
  T0TCR = 1; // Enable timer 0  (1 bit register!)
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
