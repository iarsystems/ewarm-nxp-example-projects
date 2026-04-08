/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sdram_64M_32bit_drv.c
 *    Description : SDRAM driver K4S561632E-TC(L)75 16MBx16bitx2
 *
 *    History :
 *    1. Date        : 29, October 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/

#include <stdio.h>
#include "includes.h"

#define SYS_FREQ  96MHZ

extern Int32U SDRAM_BASE_ADDR;

#if     SYS_FREQ == (96MHZ)
#define SDRAM_PERIOD          10.4  // 96MHz
#elif   SYS_FREQ == (72MHZ)
#define SDRAM_PERIOD          13.8  // 72MHz
#elif   SYS_FREQ == (57MHZ)
#define SDRAM_PERIOD          17.4  // 57.6MHz
#elif SYS_FREQ == (48MHZ)
#define SDRAM_PERIOD          20.8  // 48MHz
#elif SYS_FREQ == (36MHZ)
#define SDRAM_PERIOD          27.8  // 36MHz
#elif SYS_FREQ == (24MHZ)
#define SDRAM_PERIOD          41.7  // 24MHz
#elif SYS_FREQ == (12MHZ)
#define SDRAM_PERIOD          83.3  // 24MHz
#else
#error Frequency not defined
#endif

#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(Int32U)((Flo32)Period/SDRAM_PERIOD))+1)

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TAPR            1
#define SDRAM_TDAL            3
#define SDRAM_TWR             3
#define SDRAM_TRC             65
#define SDRAM_TRFC            66
#define SDRAM_TXSR            67
#define SDRAM_TRRD            15
#define SDRAM_TMRD            3

/*************************************************************************
 * Function Name: SDRAM_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: SDRAM controller and memory init
 *
 *************************************************************************/
void SDRAM_Init (void)
{
  /* Enable EMC clock*/
  PCONP_bit.PCEMC = 1;
  /*The EMC uses the same clock as the CPU*/
  EMCCLKSEL = 0;
  /*Assign pins to SDRAM controller*/
  IOCON_P2_16 = 0x21;
  IOCON_P2_17 = 0x21;
  IOCON_P2_18 = 0x21;
  IOCON_P2_20 = 0x21;
  IOCON_P2_24 = 0x21;
  IOCON_P2_28 = 0x21;
  IOCON_P2_29 = 0x21;
  IOCON_P2_30 = 0x21;
  IOCON_P2_31 = 0x21;
  IOCON_P3_00 = 0x21;
  IOCON_P3_01 = 0x21;
  IOCON_P3_02 = 0x21;
  IOCON_P3_03 = 0x21;
  IOCON_P3_04 = 0x21;
  IOCON_P3_05 = 0x21;
  IOCON_P3_06 = 0x21;
  IOCON_P3_07 = 0x21;
  IOCON_P3_08 = 0x21;
  IOCON_P3_09 = 0x21;
  IOCON_P3_10 = 0x21;
  IOCON_P3_11 = 0x21;
  IOCON_P3_12 = 0x21;
  IOCON_P3_13 = 0x21;
  IOCON_P3_14 = 0x21;
  IOCON_P3_15 = 0x21;
  IOCON_P3_16 = 0x21;
  IOCON_P3_17 = 0x21;
  IOCON_P3_18 = 0x21;
  IOCON_P3_19 = 0x21;
  IOCON_P3_20 = 0x21;
  IOCON_P3_21 = 0x21;
  IOCON_P3_22 = 0x21;
  IOCON_P3_23 = 0x21;
  IOCON_P3_24 = 0x21;
  IOCON_P3_25 = 0x21;
  IOCON_P3_26 = 0x21;
  IOCON_P3_27 = 0x21;
  IOCON_P3_28 = 0x21;
  IOCON_P3_29 = 0x21;
  IOCON_P3_30 = 0x21;
  IOCON_P3_31 = 0x21;
  IOCON_P4_00 = 0x21;
  IOCON_P4_01 = 0x21;
  IOCON_P4_02 = 0x21;
  IOCON_P4_03 = 0x21;
  IOCON_P4_04 = 0x21;
  IOCON_P4_05 = 0x21;
  IOCON_P4_06 = 0x21;
  IOCON_P4_07 = 0x21;
  IOCON_P4_08 = 0x21;
  IOCON_P4_09 = 0x21;
  IOCON_P4_10 = 0x21;
  IOCON_P4_11 = 0x21;
  IOCON_P4_12 = 0x21;
  IOCON_P4_13 = 0x21;
  IOCON_P4_14 = 0x21;
  IOCON_P4_25 = 0x21;
  /*Init SDRAM controller*/
  EMCDLYCTL_bit.CMDDLY = 0x8;
  /*Set data read delay*/
  EMCDLYCTL_bit.FBCLKDLY = 0x8;  
  /**/
  EMCDLYCTL_bit.CLKOUT0DLY = 0x8;  
  //EMCDLYCTL_bit.CLKOUT1DLY = 0x0;  
  EMCControl      = 1;         // enable EMC
  EMCDynamicReadConfig = 1;
  EMCDynamicRasCas0_bit.CAS = 3;
  EMCDynamicRasCas0_bit.RAS = 3;
  EMCDynamictRP = P2C(SDRAM_TRP);
  EMCDynamictRAS = P2C(SDRAM_TRAS);
  EMCDynamictSREX = P2C(SDRAM_TXSR);
  EMCDynamictAPR = SDRAM_TAPR;
  EMCDynamictDAL = SDRAM_TDAL+P2C(SDRAM_TRP);
  EMCDynamictWR = SDRAM_TWR;
  EMCDynamictRC = P2C(SDRAM_TRC);
  EMCDynamictRFC = P2C(SDRAM_TRFC);
  EMCDynamictXSR = P2C(SDRAM_TXSR);
  EMCDynamictRRD = P2C(SDRAM_TRRD);
  EMCDynamictMRD = SDRAM_TMRD;
  EMCDynamicConfig0 = 0x0004680;        // 13 row, 9 - col, SDRAM
  // JEDEC General SDRAM Initialization Sequence
  // DELAY to allow power and clocks to stabilize ~100 us
  // NOP
  EMCDynamicControl = 0x0183;
  for(volatile Int32U i = 200*30; i;i--);
  // PALL
  EMCDynamicControl_bit.I = 2;
  EMCDynamicRefresh = 1;
  for(volatile Int32U i= 256; i; --i); // > 128 clk
   EMCDynamicRefresh = P2C(SDRAM_REFRESH) >> 4;
  // COMM
  EMCDynamicControl_bit.I = 1;
  // Burst 4, Sequential, CAS-3
  volatile unsigned long Dummy = *(volatile unsigned short *)
                                  ((uint32_t)&SDRAM_BASE_ADDR + (0x32UL << (13)));
  // NORM
  EMCDynamicControl = 0x0000;
  EMCDynamicConfig0_bit.B = 1;
  for(volatile Int32U i = 100000; i;i--);
}
