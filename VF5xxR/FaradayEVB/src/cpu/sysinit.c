/*
 * File:        sysinit.c
 *
 * Notes:
 *
 */

#include "sysinit.h"
#include "common.h"
#include "serial.h"
#include "ddr.h"

int_hdlr_t interrupt_handlers[144];

void write_vbar(void);

void sysinit (void)
{
#ifndef DDR_CFG
  // Initialize the clocks using CMSIS
  SystemInit();
#endif

  // Setup the Output clocks for debugging purposes
  clk_out_setup();

  // Initialize the interrupts
  interrupts_init();

  //DDR Init
#ifndef DDR_CFG
  ddr_iomux();
  ddr_init2();
#endif

  //Initialize UART
  uart_init(BUS_CLK_KHZ, TERMINAL_BAUD);
}

// Reading the value from VBAR register
void read_vbar(void)
{
  __asm("MRC p15, 0, R3, c12, c0, 0");       //Read VBAR into R3
}

void interrupts_init(void)
{
  CA5INTD.ICDIPR[0].R = (((12<<3)<<24) + ((13<<3)<<16) + ((14<<3)<<8) + (15<<3));
  CA5INTD.ICDIPR[1].R = ((( 8<<3)<<24) + ((9<<3)<<16) + ((10<<3)<<8) + (11<<3));
  CA5INTD.ICDIPR[2].R = ((( 4<<3)<<24) + (( 5<<3)<<16) + (( 6<<3)<<8) + ( 7<<3));
  CA5INTD.ICDIPR[3].R = ((( 0<<3)<<24) + (( 1<<3)<<16) + (( 2<<3)<<8) + ( 3<<3));

  // Configure priority mask to lowest value (highest number) - enables all priorities
  GIC->PMR[0] = 0xFF;

  // Enable both secure and non-secure interrupts at CPU interface & distributor
  // Also enable secure id_read & acknowledge of non-secure interrupts
  GIC->ICR[0] = 0x7;
  CA5INTD.ICDDCR.R = 0x3;

  // Program the VBAR with the Vector Table Base Address
  write_vbar();
  read_vbar();
}


void enable_interrupt(int vector_number, void (*f)())
{
  //Configure interrupt as non-secure
  CA5INTD.ICDISR[vector_number/32].R |= 1<<(vector_number%32);

  //Enable interrupt
  CA5INTD.ICDISER[vector_number/32].R |= 1<<(vector_number%32);

  //Direct interrupt to core0 (A5)
  MSCM->IRSPRC[vector_number-32] |= MSCM_IRSPRC_CP0En_MASK;  //vector# - 32

  //Assign interrupt handler
  interrupt_handlers[vector_number]=f;
}


/********************************************************************/
void clk_out_setup(void)
{
  IOMUXC->SINGLE.PTB10=0x00603062; //110 (CKO1)
  IOMUXC->SINGLE.PTB11=0x00603062; //110 (CKO2)

  // Setup CKO1
  CCM->CCOSR = 0x00;
  CCM->CCOSR |= CCM_CCOSR_CKO1_SEL(CKO1_CM4_ROOT_CLK);
  CCM->CCOSR |= CCM_CCOSR_CKO1_DIV(0x09);    // Divide by 10
  CCM->CCOSR |= CCM_CCOSR_CKO1_EN_MASK;

  // Setup CKO2              // Necessary to get slow clock sources out
  CCM->CCOSR |= CCM_CCOSR_CKO2_SEL(CKO2_PLL1_MAIN_CLK);
  CCM->CCOSR |= CCM_CCOSR_CKO2_DIV(0x09);    // Divide by 10
  CCM->CCOSR |= CCM_CCOSR_CKO2_EN_MASK;
}

