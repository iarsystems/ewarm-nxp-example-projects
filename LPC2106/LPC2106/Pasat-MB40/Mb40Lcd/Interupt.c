// -------------------------------------------------------------------
// -----------------------LPC2106 interrupt routines------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

//#include "ma_uart0.h"    /* UART0 Module driver header file */
//#include "ma_uart0.h"    /* UART1 Module driver header file */
//#include "ma_gpio.h"     /* GPIO  Module driver header file */
//#include "ma_spi.h"      /* SPI   Module driver header file */
//#include "ma_i2c.h"      /* I2C   Module driver header file */
//#include "ma_pcb.h"     /* PCB Module driver header file */
#include "ma_vic.h"
#include "ma_tmr.h"

#include "interupt.h"
#include "key_api.h"

#include "string.h"
#include "stdio.h"

extern U8 BitNr;
extern U8 Data8;


#define VIC_TIMER1_bit (1 << VIC_TIMER1)

void Install_IntHandlers(void)
{

  // Setup interrupt controller.
  // Toto je inde VICProtection = 0;
  // Disable all interrupts
  VICIntEnClear = 0xffffffff;

  VICIntSelect &= ~VIC_TIMER1_bit; 	// IRQ on timer 1 line.
  //VICVectAddr0 = (unsigned int)&TimerInterrupt;
  MA_SetISRAddress_VIC((U8*)MA_IntHandler_Timer1Capture,0);

  VICVectCntl0 = 0x20 | VIC_TIMER1; 	// Enable vector interrupt for timer 1.
  VICIntEnable |= VIC_TIMER1_bit; 	// Enable timer 1 interrupt.

}

//---------------------------------------------------------------------------
// This function is the high level language interrupt handler for the
// timer 1 interrupts.
// Replaces original MakeApp code - handles falling edge of CAP1.1
//---------------------------------------------------------------------------
void MA_IntHandler_Timer1Capture( void ) 
{
    U32 IntSrc;

    /*--- Handle user code on function entry ---*/
    ENTER_MA_INTHANDLER_TIMER1;

    IntSrc = T1IR;

     if ( IntSrc & T1IR_CR1INT )
    {
        INSIDE_MA_INTHANDLER_TIMER1_CR1;

		if (BitNr < 11 && BitNr > 2)    // Bits 3 to 10 are data. Parity bit,
   		{                               // start and stop bits are ignored.
			Data8 >>= 1;
        	if(IOPIN & 0x1000)
        		Data8 |= 0x80;         // Set to 1 if P0.12=H
   		}

   		if (--BitNr == 0)               // All 11 bits received
   		{
        	Key_Decode(Data8);
        	BitNr = 11;
    	}
        
        /*--- Clear interrupt flag ---*/
        T1IR_bit.CR1INT = 1;            
    }
    
    /*--- Handle user code on function exit ---*/
    EXIT_MA_INTHANDLER_TIMER1;
 
} /* MA_IntHandler_TIMER1 */
