/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : low_level_init.c
 *    Description : low level init module - trim OSC amplitude
 *    set MPLL and SPLL 266MHz and 300MHz, init CS0 (NOR), CS5 (PSRAM) and
 *    CSD0 (DDR RAM), init MUX and AITC
 *
 *    History :
 *    1. Date        : May 1, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 *
 **************************************************************************/
#include "arm_comm.h"

#include <NXP/iomcimx27.h>

#pragma segment=".intvec"

#define INTR_TBL_BASE_ADDR  0xFFFFFF00

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
  // AHB-Lite IP Interface
  AIPI1_PSR0 = 0x00040304;
  AIPI1_PSR1 = 0xFFFBFCFB;
  AIPI2_PSR0 = 0x3FFC0000;
  AIPI2_PSR1 = 0xFFFFFFFF;
  AIPI1_PAR  = 0xFFFFFFFF;
  AIPI2_PAR  = 0xFFFFFFFF;

  // Init CS0  (NOR)    0xC0000000 - 0xC1FFFFFF
  CSCR0U = 0x0000CC03;
  CSCR0L = 0xa0330D01;
  CSCR0A = 0x00220800;

  // Init CS5  (PSRAM)  0xD6000000 - 0xD0FFFFFF
  CSCR5U = 0x0000dcf6;
  CSCR5L = 0x444a4541;
  CSCR5A = 0x44443302;

  // Configure CS4 (CPLD) 0xD4000000 – 0xD5FFFFFF
  CSCR4U = 0x0000DCF6;
  CSCR4L = 0x444A4541;
  CSCR4A = 0x44443302;

	// configure peripheral enables for PCCR0
  PCCR0 = 0;
	// configure peripheral enables for PCCR1
  PCCR1 = 0x00580780;
  // Set Peripheral Clock Divider Registers
  PCDR0 = 0x23C83403;
  PCDR1 = 0x03030303;

#ifndef DDRRAM_EXEC

  if(0x00331C23 != MPCTL0)
  {
    // 26MHz OSC adjust
    CSCR_bit.OSC26M_DIV1P5 = 0;
    CSCR_bit.OSC26M_DIS    = 0;
    // wait some time for OSC stabilization
    // depend of OSC type and load capacitors
    for(volatile Int32U i = 100000; i; --i);
    // trim the OSC amplitude
    OSC26MCTL_bit.AGC = 0x3F;
    do
    {
      // delay 30.5 us
      for(volatile Int32U i = 1000; i; --i);
      if(OSC26MCTL_bit.OSC26M_PEAK == 0)
      {
        OSC26MCTL_bit.AGC -= 4;
        break;
      }
    }
    while(--OSC26MCTL_bit.AGC > 4);

    // Set maximum clock divistion
    CSCR_bit.ARM_DIV = 0x3;
    CSCR_bit.AHB_DIV = 0x3;
    CSCR_bit.ARMSRC  = 0x0;   // PLL postdivider 3/2
    CSCR_bit.MCU_SEL = 0x1;   // Ext. 26MHz OSC
    CSCR_bit.SP_SEL  = 0x1;   // Ext. 26MHz OSC

    // configure MPCTL for 399 MHz
    MPCTL0 = 0x00331C23;

    // restart MPCTL
    // after changing the operating
    // frequencies
    CSCR_bit.MPLL_RESTART = 1;		

    // FCLK = 266Hz, HCLK = 133 MHz
    CSCR_bit.ARM_DIV = 0x0;
    CSCR_bit.AHB_DIV = 0x1;

    // Init DDRRAM 0xA0000000 - 0xA7FFFFFF
    // DDR 4x16Mx16 x 2
    // Row Address = 14
    // Col Address = 10
    // ESDMISC: enable DDR mode
    ESDMISC = 0x00000004;
    // ESDCFG0: timing config
    ESDCFG0 = 0x006AC73A;
    // ESDCTL0: Precharge Mode
    ESDCTL0 = 0x92100000;
    // issue Precharge All command
    *(volatile unsigned int *)0xA0000400 = 0;
    // ESDCTL0: AutoRefresh Mode
    ESDCTL0 = 0xA2100000;
    // issue Autorefresh command
    *(volatile unsigned int *)0xA0000000 = 0;
    *(volatile unsigned int *)0xA0000000 = 0;
    // ESDCTL0: Load Mode register Mode
    ESDCTL0 = 0xB2100000;
    // issue Load Mode Register command
    *(volatile unsigned char *)0xA0000033 = 0;
    *(volatile unsigned char *)0xA1000000 = 0;
    // ESDCTL0: Normal Mode
    // Row Address  = 13
    // Col Address  = 10
    // Data width   = 32-bit
    // Refresh Rate = 8192/64ms
    // Burst Length = 8
    // - 2 : 32 bit mode
    // - 1 : 16 bit - D[15:00]
    // - 0 : 16 bit - D[31:16]
    ESDCTL0 = 0x82226080;
    // issue normal access
    *(volatile unsigned int *)0xA0000000 = 0;
    // ESDMISC: Reset Delay Line Measurement
    ESDMISC = 0x0000000C;
	}

#endif // DDRRAM_EXEC

  // MUX
  PCSR = 0x00000003;  // Set high priority masters (0,1)
  // init Slave General Purpose Control Register
  SGPCR0 = SGPCR1 = SGPCR2 = 0x00000100;
  // init Master Priority Register
  MPR0 = MPR1 = MPR2 =  0x00543210;
  // init Master General Purpose Control Register
  MGPCR0 = MGPCR1 = MGPCR2 = 2;

  // AITC
  INTCNTL_bit.FIDIS = 1;  // Disable all fast interrupts
  INTCNTL_bit.NIDIS = 1;  // Disable all normal interrupts
  INTCNTL_bit.NIAD  = 0;  // Disregard the normal interrupt flag
  INTCNTL_bit.FIAD  = 1;  // Fast interrupt flag increases the
                          // bus arbitration priority
  INTCNTL_bit.MD    = 0;  // Interrupt vector table located in
                          // high memory from 0xFFFF_FF00 to 0xFFFF_FFFF
  INTENABLEH = 0;
  INTENABLEL = 0; // all interrupts are disable
  INTTYPEH = 0;
  INTTYPEL = 0;   // all interrupt lines are routed to the normal interrupt
  NIMASK = 0x10;  // Do not disable any normal interrupts

  // Init Interrupt vector table
  // IRQ
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+        IRQV) = (Int32U)__sfb(".intvec")+IRQV;
  // FIQ
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+        FIQV) = (Int32U)__sfb(".intvec")+FIQV;
  // Undef
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 +  UNDEFV) = (Int32U)__sfb(".intvec")+UNDEFV;
  // SWI
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 +    SWIV) = (Int32U)__sfb(".intvec")+SWIV;
  // Prefetch abort
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 + PABORTV) = (Int32U)__sfb(".intvec")+PABORTV;
  // Data abort
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 + DABORTV) = (Int32U)__sfb(".intvec")+DABORTV;
}
