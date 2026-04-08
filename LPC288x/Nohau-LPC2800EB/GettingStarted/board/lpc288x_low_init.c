/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lpc288x_low_init.c
 *    Description : Low level init
 *
 *    History :
 *    1. Date        : 6, November 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <assert.h>
#include "lpc288x_low_init.h"

#pragma section=".intvec"
#ifdef IFLASH_BOOT
// The mark of valid iFlash programme
#pragma section=".FlashValidMark"
#pragma location=".FlashValidMark"
__root const unsigned int flash_valid_marker = 0xAA55AA55;
#endif

#pragma data_alignment=4096
IntrVecTable_t  IrqTable[31] = {0};

/*************************************************************************
 * Function Name: LPC288x_SectionClock
 * Parameters: volatile long * pReg, SectionInClockSelect_t Mode
 *
 * Return: void
 *
 * Description: This function is used for change of the section input clock
 *
 *************************************************************************/
void LPC288x_SectionClockSelect(volatile unsigned long * pReg, Int32U Mode)
{
#define ST_OFFSET   ((unsigned long *)&SYSSSR - (unsigned long *)&SYSSCR)
#define CH1_OFFSET  ((unsigned long *)&SYSFSR1 - (unsigned long *)&SYSSCR)
#define CH2_OFFSET  ((unsigned long *)&SYSFSR2 - (unsigned long *)&SYSSCR)
unsigned long Channel = (*(pReg+ST_OFFSET) & 0x3) ^ 0x3;  // get unused channel
  // Apply new clock source setting
  if(Channel & 0x1)
  {
    *(pReg+CH1_OFFSET) = Mode;
  }
  else
  {
    *(pReg+CH2_OFFSET) = Mode;
  }
  // change the channel
  *pReg = Channel;
}

#ifdef XSDRAM_INIT
/*************************************************************************
 * Function Name: Dly_us
 * Parameters: Int32U Dly [us]
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
static void Dly_us (Int32U Dly)
{
  INT_REQ5 = 0x04010000;    // enable interrupt
  T0CTRL = 0;               // stop counting
  T0LOAD = 12 * Dly;        // load period
  T0CLR = 0;                // clear timer pending interrupt
  T0CTRL_bit.TENAB = 1;     // enable counting
  while(!INT_PENDING_bit.TIMER0);
  T0CTRL_bit.TENAB = 0;     // stop counting
}
#endif // XSDRAM_INIT

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
  // Disable WDT
  WDT_TCR_bit.CE = 0;

  // Disable and reset cache
  CACHE_PAGE_CTRL = 0;
  CACHE_SETTINGS = 1;
  CACHE_SETTINGS_bit.CACHE_RST = 0;
  while(CACHE_RST_STAT_bit.CACHE_STATUS);  // wait until reset complete

#ifdef IFLASH_BOOT
  // Set flash read wait states
  F_WAIT_bit.WAIT_STATES = 4;
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  // map iflash memory on address 0x00000000
  ADDRESS_PAGE_0 = (Int32U)__segment_begin(".intvec")>>21;
  CACHE_PAGE_CTRL_bit.PAGE_0_ENA = 1;
#endif // IFLASH_BOOT

#ifdef XFLASH_BOOT
  // map xflash CS0 memory on address 0x00000000
  ADDRESS_PAGE_0 = (Int32U)__segment_begin(".intvec")>>21;
  CACHE_PAGE_CTRL_bit.PAGE_0_ENA = 1;
  // 16 bit, CS active low, LBn should be asserted low so that the memory
  // drives both lanes of the bus.
  EMCSTATICCONFIG0 = 0x00000081;
  EMCSTATICCONFIG1 = 0x00000081;
  EMCSTATICCONFIG2 = 0x00000081;
  // Flash read access delay
  EMCSTATICWAITRD0 = P2C(FLASH_TAC);
  EMCSTATICWAITRD1 = P2C(FLASH_TAC);
  EMCSTATICWAITRD2 = P2C(FLASH_TAC);
#endif

  // Set PLL and clocks
  OSCEN_bit.RUN = 1;

  // Init all section to fast clock
  LPC288x_SectionClockSelect(&SYSSCR ,HF_Osc);
  LPC288x_SectionClockSelect(&APB0SCR,HF_Osc);
  LPC288x_SectionClockSelect(&APB1SCR,HF_Osc);
  LPC288x_SectionClockSelect(&APB3SCR,HF_Osc);
  LPC288x_SectionClockSelect(&DCDCSCR,HF_Osc);
  LPC288x_SectionClockSelect(&RTCSCR ,HF_Osc);
  LPC288x_SectionClockSelect(&MCISCR ,HF_Osc);
  LPC288x_SectionClockSelect(&UARTSCR,HF_Osc);
  LPC288x_SectionClockSelect(&DAIOSCR,HF_Osc);
  LPC288x_SectionClockSelect(&DAISCR ,HF_Osc);

  // Init PLL
  if(LPMSEL)
  {
    LPPDN  = 1;           // stop the main PLL
    LPFIN  = HF_Osc;      // fast osc input clock
    LPMSEL = (unsigned int)(PLL_MUL-1);   // set PLL multiplier
    LPPSEL = PLL_DIV;     // set PLL divider
    LPMBYP = 0;           // disable bypass of PLL CCO
    LPDBYP = 0;           // disable bypass of post divider
    LPPDN  = 0;           // start the main PLL
    // wait until PLL lock frequency
    while(!LPLOCK);

    // Select Main PLL clock of system state
    LPC288x_SectionClockSelect(&SYSSCR,MainPLL);
  }

  // Init interrupt controller
  MMIOESR0_bit.ESR_EN = 0;          // disable fractional divider
  MMIORES = 1;
  INT_VECTOR0 = (Int32U)IrqTable;   // set interrupt vectors table
  INT_VECTOR1 = 0;
  INT_PRIOMASK0 = INT_PRIOMASK1 = 0;
  for(pInt32U pReg = (pInt32U)&INT_REQ1; pReg <= (pInt32U)&INT_REQ29; ++pReg)
  {
    *pReg = 0x3E000000;
  }

#ifdef XSDRAM_INIT
  // assign GPIO to EMC
  MODE0_0 = 0xFFFFFFFF;
  MODE0_1 = 0x000FFFFF;
  MODE1_0 = 0x00000000;
  MODE1_1 = 0x00000000;
  // enable EMC clk
  // disable fractional divider
  EMCESR0_bit.ESR_EN = EMCESR1_bit.ESR_EN = 0;
  // release reset of the EMC
  EMCRES = 1;
  // Init SDRAM controller and memory
  EMCCONTROL = 1; // Enable EMC
  EMCDYNAMICREADCONFIG = 1; // Command delayed strategy, using AHBHCLKDELAY
  EMCMISC = 0;
  EMCDYNAMICRASCAS_bit.CAS = 2;
  EMCDYNAMICRASCAS_bit.RAS = 2;
  EMCDYNAMICRP = P2C(SDRAM_TRP);
  EMCDYNAMICRAS = P2C(SDRAM_TRAS);
  EMCDYNAMICSREX = P2C(SDRAM_TXSR);
  EMCDYNAMICAPR = SDRAM_TAPR;
  EMCDYNAMICDAL = SDRAM_TDAL;
  EMCDYNAMICWR = SDRAM_TWR;
  EMCDYNAMICRC = P2C(SDRAM_TRC);
  EMCDYNAMICRFC = P2C(SDRAM_TRFC);
  EMCDYNAMICXSR = P2C(SDRAM_TXSR);
  EMCDYNAMICRRD = P2C(SDRAM_TRRD);
  EMCDYNAMICMRD = SDRAM_TMRD;
  EMCDYNAMICCONFIG = 0x00000480;        // 12 row, 9 - col, SDRAM
  // JEDEC General SDRAM Initialization Sequence
  // DELAY to allow power and clocks to stabilize ~100 us
  // NOP
  EMCDYNAMICCONTROL = 0x4183;
  Dly_us(200);
  // PALL
  EMCDYNAMICCONTROL_bit.SDRAMINI = 2;
  EMCDYNAMICREFRESH = 1;
  for(volatile Int32U i= 128; i; --i); // > 128 clk
  EMCDYNAMICREFRESH = P2C(SDRAM_REFRESH) >> 4;
  // COMM
  EMCDYNAMICCONTROL_bit.SDRAMINI = 1;
  // Burst 8, Sequential, CAS-2
  volatile unsigned short Dummy = *(volatile unsigned short *)(SDRAM_BASE_ADDR | (0x23UL << 12));
  // NORM
  EMCDYNAMICCONTROL = 0x4000;
  EMCDYNAMICCONFIG_bit.BUF_EN = 1;
#endif  // XSDRAM_INIT

  return(1);
}
