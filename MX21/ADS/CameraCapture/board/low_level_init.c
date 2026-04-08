/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : low_level_init.c
 *    Description : low level init module - AITC, MUX, DBG UART
 *
 *    History :
 *    1. Date        : May 14, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 *
 **************************************************************************/
#include <limits.h>
#include "arm_comm.h"
#include "board.h"

#include <NXP/iomc9328mx21.h>

#pragma segment=".intvec"

#define INTR_TBL_BASE_ADDR  0xFFFFFF00

/*************************************************************************
 * Function Name: getchar
 * Parameters: none
 *
 * Return: int
 *
 * Description: Receive char by UART 1
 *
 *************************************************************************/
int getchar(void)
{
  while(!USR1_1_bit.RRDY);
  return(URXD_1_bit.RX_DATA);
}

/*************************************************************************
 * Function Name: putchar
 * Parameters: int c
 *
 * Return: int
 *
 * Description: Send char by UART 1
 *
 *************************************************************************/
int putchar(int c)
{
  while(!USR1_1_bit.TRDY);
  UTXD_1_bit.TX_DATA = c;
  return(c);
}

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
Int32U Num   = UART1_BAUD_RATE * 16;
Int32U Denum = FCLK / PERDIV1;

  // AHB-Lite IP Interface
  AIPI1_PSR0 = 0x00040304;
  AIPI1_PSR1 = 0xFFFBFCFB;
  AIPI2_PSR0 = 0x3FFC0000;
  AIPI2_PSR1 = 0xFFFFFFFF;

  // Peripheral Access in supervision only
  AIPI1_PAR = AIPI2_PAR = 0xFFFFFFFF;

  // EIM
  EIM_CNF_bit.BCM = 0;
  // CS0 Initialization Burst Flash
  CS0U = 0x00000701;
  CS0L = 0x00000E01;
  // CS1 Initialization
  CS1U = 0x00002000;
  CS1L = 0x11118501;

  // Set PER1 clk dividers
  PCDR1_bit.PCLK_DIV1 = PERDIV1-1;

#ifndef SDRAM_EXEC
	// FCLK = 266MHz, HCLK = 133 MHz
	CSCR_bit.PRESC   = 0x0;
	CSCR_bit.BCLKDIV = 0x1;
	CSCR_bit.MPEN = 0x1;      // MPLL Enable
	CSCR_bit.SPEN = 0x1;      // Serial Peripheral PLL Enable
	
	// configure MPCTL for 266 MHz
	MPCTL0 = 0x007B1C73;
	// configure SPLL for 240 MHz
	SPCTL0 = 0x03811C89;
	
	// restart the MPCTL
	CSCR_bit.MPLL_RESTART = 1;		
	// restart the SPLL
	CSCR_bit.SPLL_RESTART = 1;		

  // SDRAM
  // CSD0 Initialization (SDRAM)  16Mx16x2 IAM=0 CSD0 CL3
  SDCTL0 = 0x92120300;  // Set Precharge Command
  // A10 - High Issue Precharge all Command
  *(volatile Int32U *)(SDRAM_PRECHARGE_ALL + SDRAM_BASE_ADDRESS) = 0;
  SDCTL0 = 0xA2120300;  // Set AutoRefresh Command
  //  AutoRefresh x8
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  *(volatile Int32U *)(SDRAM_BASE_ADDRESS) = 0;
  // Set mode register
  SDCTL0 = 0xB2120300;  // Set Mode Register
  *(Int32U *)(SDRAM_BASE_ADDRESS + SDRAM_MODE_REG) = 0; // Issue Mode Register Command
//  Set to Normal Mode
//  From the spec of the SDRAM
//  1. tRCD = 19ns minimum  -> RCD = 3 clk (SDCLK=133MHz) -> SRCD = 11b
//  2. tRP  = 19ns minimum  -> RP  = 3 clk (SDCLK=133MHz) -> SRP  = 0b
//  3. tRC  = 65ns minimum  -> RC  = 9 clk (SDCLK=133MHz) -> SRC  = 1001b
//  4. refresh rate = 8192rows/64ms -> SREFR = 11b
  SDCTL0 = 0x8212F339;
#endif // SDRAM_EXEC

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
  INTCNTL_bit.ABFEN = 0;  // ABFLAG bit is normal
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
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+       IRQV) = (Int32U)__sfb(".intvec")+IRQV;
  // FIQ
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+       FIQV) = (Int32U)__sfb(".intvec")+FIQV;
  // Undef
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 + UNDEFV) = (Int32U)__sfb(".intvec")+UNDEFV;
  // SWI
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 +   SWIV) = (Int32U)__sfb(".intvec")+SWIV;
  // Prefetch abort
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 +PABORTV) = (Int32U)__sfb(".intvec")+PABORTV;
  // Data abort
  *(Int32U *)((INTR_TBL_BASE_ADDR-IRQV)+ 4 +DABORTV) = (Int32U)__sfb(".intvec")+DABORTV;

  // DBG UART Init
  // Uart1 Clock enable
  PCCR0_bit.UART1_EN = 1;
  // Uart disable
  UCR1_1 = 0x0;
  // Assign pins RxD and TxD
  PTE_GIUS_bit.PIN12 = 0;PTE_GPR_bit.PIN12 = 0;
  PTE_GIUS_bit.PIN13 = 0;PTE_GPR_bit.PIN13 = 0;
  // Set PER clk dividers
  PCDR1_bit.PCLK_DIV1 = PERDIV1-1;
  // Div 1
  UFCR_1_bit.RFDIV = 5;
  UFCR_1_bit.TXTL  = 2;
  // 8 bit, No Parity, 1 stop, TX Enable, RX Enable
  UCR3_1 = UCR4_1 = 0;
  UCR2_1 = 0x4026;
  // Init Serial DBG 115200 PERCLK1 - MPLL_CLK / 16 - 16.625MHz
  while ((Num > USHRT_MAX) || (Denum > USHRT_MAX))
  {
    Num >>=1; Denum>>=1;
  }
  // update of baud only when write in this order
  UBIR_1 = Num   - 1;
  UBMR_1 = Denum - 1;
  // UART Enable
  UCR1_1 |= 0x1;

}
