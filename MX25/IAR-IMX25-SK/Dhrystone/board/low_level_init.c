/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : low_level_init.c
 *    Description : low level init module - trim OSC amplitude
 *    set MPLL and SPLL 266MHz and 300MHz, init NAND and
 *    CSD0 (DDR RAM), init MUX and AITC
 *
 *    History :
 *    1. Date        : October 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include "board.h"

#pragma segment=".intvec"
/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Low level init code
 *
 *************************************************************************/
__arm void low_level_init (void)
{

	// configure peripheral enables for CGCR0
  CGCR0 = 0x1FFFFFFF;
	// configure peripheral enables for CGCR1
  CGCR1 = 0xFFFFFFFF;
	// configure peripheral enables for CGCR2
  CGCR2 = 0x000FDFFF;
  // Set Peripheral Clock Divider Registers
  PCDR0 = 0x23C83403;
  PCDR1 = 0x03030303;
  PCDR2 = 0x01010103;
  PCDR3 = 0x01010101;

  // AIPS
  //*(unsigned long *)0x43f00000 = 0x77777777;
  //*(unsigned long *)0x43f00004 = 0x77777777;
  //*(unsigned long *)0x53f00000 = 0x77777777;
  //*(unsigned long *)0x53f00004 = 0x77777777;

  // MUX
  // init Slave General Purpose Control Register
  SGPCR0 = SGPCR1 = SGPCR2 = SGPCR3 = SGPCR4 = 0x00000010;
  // init Master Priority Register
  MPR0 = MPR1 = MPR2 =  MPR3 = MPR4 = 0x00043210;
  // init Master General Purpose Control Register
  MGPCR0 = MGPCR1 = MGPCR2 = MGPCR3 = MGPCR4 = 0;
  MCR = 0;

#ifndef SDRAM_EXEC
  CCTL_bit.ARMSRC     = 0x0;
  CCTL_bit.CGCTL      = 0x0;
  CCTL_bit.MPLLBYPASS = 0x0;
  // FCLK = 266Hz, HCLK = 133 MHz
  CCTL_bit.ARMCLKDIV = 0x1;
  CCTL_bit.AHBCLKDIV = 0x1;

  // Init SDRAM 0x80000000 - 0x87FFFFFF
  // SDRAM 4x16Mx16 x 1
  // Row Address = 13
  // Col Address = 9
  // ESDMISC: enable SDR mode
  ESDMISC = 0x00000002;
  ESDMISC = 0x00000000;
  // ESDCFG0: timing config
  ESDCFG0 = 0x006AC73A;
  // ESDCTL0: Precharge Mode
  ESDCTL0 = 0x92116080;
  // issue Precharge All command
  *(volatile unsigned int *)0x80000400 = 0;
  // ESDCTL0: AutoRefresh Mode
  ESDCTL0 = 0xA2116080;
  // issue Autorefresh command
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  // ESDCTL0: Load Mode register Mode
  ESDCTL0 = 0xB2116080;
  // issue Load Mode Register command
  *(volatile unsigned char *)0x80000033= 0;
  // ESDCTL0: Normal Mode
  // Row Address  = 13
  // Col Address  = 9
  // Data width   = 16-bit
  // Refresh Rate = 8192/64ms
  // Burst Length = 8
  // - 2 : 16 bit mode
  //     : 16 bit - D[15:00]
  ESDCTL0 = 0x82116080;
  // issue normal access
  *(volatile unsigned int *)0x80000000 = 0;

#endif // SDRAM_EXEC

  UPCTL            = 0x84002800;
  CCTL_bit.USBDIV  = 3;
  // restart USB PLL
  // after changing the operating
  // frequencies
  CCTL_bit.UPLLRST = 1;
  while(!MPCTL_bit.LOCK);

  // AITC
  INTCNTL_bit.FIDIS = 1;  // Disable all fast interrupts
  INTCNTL_bit.NIDIS = 1;  // Disable all normalinterrupts
  INTENABLEH = 0;
  INTENABLEL = 0; // all interrupts are disable
  INTTYPEH = 0;
  INTTYPEL = 0;   // all interrupt lines are routed to the normal interrupt
  NIMASK = 0x10;  // Do not disable any normal interrupts
}
