/*
 * File:	start.c
 * Purpose:	Kinetis start up routines.
 *
 * Notes:
 */

#include "start_m4.h"
#include "common.h"

extern void write_vtor (int vtor);
__no_init uint32_t SystemCoreClock;

void SystemCoreClockUpdate (void);

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then
 * branches to the main process.
 */
//typedef void (*vector_entry)(void);
//extern vector_entry  __vector_table[];

int __low_level_init (void)
{
  int n,pclk;
  extern uint32 __VECTOR_TABLE[];
  extern uint32 __VECTOR_RAM[];

  /* Copy the vector table to RAM */
  if (__VECTOR_RAM != __VECTOR_TABLE)
  {
      for (n = 0; n < (0x400/sizeof(__VECTOR_RAM[0])); n++)
          __VECTOR_RAM[n] = __VECTOR_TABLE[n];
  }

  /* Point the VTOR to the new copy of the vector table */
  write_vtor((uint32)__VECTOR_RAM);

#if (CLOCK_SETUP == 0)
  // using PLL1 PFD3
  //Enable CCM, SRC, and more
  CCM->CCGR4 |= CCM_CCGR4_CG10(1); // WKPU ungate
  CCM->CCGR4 |= CCM_CCGR4_CG11(1); // CCM ungate
  CCM->CCGR4 |= CCM_CCGR4_CG12(1); // GPC ungate
  CCM->CCGR4 |= CCM_CCGR4_CG14(1); // SRC ungate
  CCM->CCGR3 |= CCM_CCGR3_CG0(1);  // ANADIG ungate

  //Enable IOMUX
  CCM->CCGR2 |= CCM_CCGR2_CG8(1);  // IOMUX Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG9(1);  // Port A Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG10(1); // Port B Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG11(1); // Port C Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG12(1); // Port D Controller ungate
  CCM->CCGR2 |= CCM_CCGR2_CG13(1); // Port E Controller ungate

  // Setup the clocks to run in synchronous mode
  CCM->CLPCR &= ~CCM_CLPCR_FXOSC_PWRDWN_MASK;
  while(!(CCM_CSR_FXOSC_RDY_MASK & CCM->CSR));
  CCM->CCSR |= CCM_CCSR_FAST_CLK_SEL_MASK;

  /* enable PLLs in Anadig */
  ANADIG->PLL1_CTRL=0x00002001;
  while(!(ANADIG_PLL1_CTRL_LOCK_MASK & ANADIG->PLL1_CTRL));//wait until lock
  ANADIG->PLL2_CTRL=0x00002001;
  while(!(ANADIG_PLL2_CTRL_LOCK_MASK & ANADIG->PLL2_CTRL));//wait until lock
  ANADIG->PLL4_CTRL=0x00002031; //PLL4
  while(!(ANADIG_PLL4_CTRL_LOCK_MASK & ANADIG->PLL4_CTRL));//wait until lock
  ANADIG->PLL5_CTRL=0x00002001; //PLL5
  while(!(ANADIG_PLL5_CTRL_LOCK_MASK & ANADIG->PLL5_CTRL));//wait until lock
  ANADIG->PLL6_CTRL=0x00002028; //PLL6
  while(!(ANADIG_PLL6_CTRL_LOCK_MASK & ANADIG->PLL6_CTRL));//wait until lock

  //Use 396MHz CA5, 400 MHz DDR, 132MHz CM4, and 66MHz bus
  CCM->CCSR=0x0003FF24; //PLL1 uses PLL1_PFD3, enable all PLL1 and PLL2, select Fast Clock, and sys_clock_sel use PLL1
  CCM->CACRR=0x00000810; //ARM_DIV=0 (div by 1), BUS_DIV=2 (div by 3), ipg_div value is 1 (div by 2)
#endif

  SystemCoreClockUpdate();

  pclk  = SystemCoreClock / (((CCM->CACRR & CCM_CACRR_BUS_CLK_DIV_MASK) >> CCM_CACRR_BUS_CLK_DIV_SHIFT) + 1);
  pclk /= (((CCM->CACRR & CCM_CACRR_IPG_CLK_DIV_MASK) >> CCM_CACRR_IPG_CLK_DIV_SHIFT) + 1);

  uart_init(pclk, TERMINAL_BAUD);

  /* Identify CPU type and configuration */
  cpu_identify();

  return 1;
}

/********************************************************************/
/*!
 * \brief   Vybrid Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 */
void cpu_identify(void)
{
  printf("\n\n**********************\n");
  printf("Freescale Vybrid\n");

  if((MSCM->CPxTYPE>>MSCM_CPxTYPE_Personality_SHIFT)==0x434135)
  {
    printf("  A5 Core Running\n");
  }
  else if((MSCM->CPxTYPE>>MSCM_CPxTYPE_Personality_SHIFT)==0x434D34)
  {
    printf("  M4 Core Running\n");
    printf("  Core Clk Freq: %d kHz\n", SystemCoreClock/(((CCM->CACRR & CCM_CACRR_BUS_CLK_DIV_MASK) >> CCM_CACRR_BUS_CLK_DIV_SHIFT) + 1));
  }

  printf("  Rev #: 1.%d\n",(MSCM->CPxTYPE&MSCM_CPxTYPE_rYpZ_MASK)-1);
  printf("  Core number: %d\n",MSCM->CPxNUM);
  printf("  Number of cores: %d\n",MSCM->CPxCOUNT+1); //Add 1 to get # of cores

  printf("\n");
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void)
{
  uint8_t sys_clk_select;  /* Variable to store output clock frequency of the MCG module */
  uint8_t temp_select;
  uint32_t MFI = 0;
  uint32_t PLLout = 0;

  uint32_t PLL2_NUM=ANADIG->PLL2_NUM;
  uint32_t PLL2_DENOM=ANADIG->PLL2_DENOM;
  uint32_t PLL1_NUM=ANADIG->PLL1_NUM;
  uint32_t PLL1_DENOM=ANADIG->PLL1_DENOM;

  sys_clk_select = (CCM->CCSR & CCM_CCSR_SYS_CLK_SEL_MASK);

  switch (sys_clk_select)
  {
    case 0:  // CASE 0:  FAST Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      SystemCoreClock = 24000;
      break;
    case 1:  // CASE 1:  SLOW Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      SystemCoreClock = 32;
      break;
    case 2:  // CASE 2:  PLL2 PFD Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      temp_select = (CCM->CCSR & CCM_CCSR_PLL2_PFD_CLK_SEL_MASK);
      MFI = 20 + (2 * (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_DIV_SELECT_MASK));
      switch (temp_select)
      {
        case 0:
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            SystemCoreClock = 24000;
          else
            SystemCoreClock = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          break;
        case 1:  //CASE 1!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
           PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD1_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD1_FRAC_SHIFT);
          break;
        case 2:   //CASE 2!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
         SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD2_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD2_FRAC_SHIFT);
         break;
        case 3:  //CASE 3!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
           PLLout = 24000;
          else
           PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
         SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD3_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD3_FRAC_SHIFT);
         break;
        case 4:  //CASE 4!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL2_PFD & ANADIG_PLL2_PFD_PFD4_FRAC_MASK)>>ANADIG_PLL2_PFD_PFD4_FRAC_SHIFT);
          break;
        default:
          break;
      }
    case 3:  // CASE 3:  PLL2 Main Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      if (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_BYPASS_MASK)
        SystemCoreClock = 24000;
      else
      {
        MFI = 20 + (2 * (ANADIG->PLL2_CTRL & ANADIG_PLL2_CTRL_DIV_SELECT_MASK));
        SystemCoreClock = (24000*(MFI + ((PLL2_NUM)/(PLL2_DENOM))));
      }
      break;
    case 4:  // CASE 4:  PLL1 PFD Clock!!! ///////////////////////////////////////////////////////////////////////////////////////
      temp_select = (CCM->CCSR & CCM_CCSR_PLL1_PFD_CLK_SEL_MASK)>>CCM_CCSR_PLL1_PFD_CLK_SEL_SHIFT ;
      MFI = 20 + (2 * (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_DIV_SELECT_MASK));
      switch (temp_select)
      {
        case 0:
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            SystemCoreClock = 24000;
          else
            SystemCoreClock = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          break;
        case 1:  //CASE 1!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL2_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD1_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD1_FRAC_SHIFT);
          break;
        case 2:   //CASE 2!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD2_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD2_FRAC_SHIFT);
          break;
        case 3:  //CASE 3!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18)/ ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD3_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD3_FRAC_SHIFT);
          break;
        case 4:  //CASE 4!!! ///////////////////////////////////////////////////////////////////////////////////
          if (ANADIG->PLL1_CTRL & ANADIG_PLL1_CTRL_BYPASS_MASK)
            PLLout = 24000;
          else
            PLLout = (24000*(MFI + ((PLL1_NUM)/(PLL1_DENOM))));
          SystemCoreClock = (PLLout * 18) / ((ANADIG->PLL1_PFD & ANADIG_PLL1_PFD_PFD4_FRAC_MASK)>>ANADIG_PLL1_PFD_PFD4_FRAC_SHIFT);
          break;
        default:
          break;
      }
      break;
    case 5:
      SystemCoreClock = 480000;
      break;
    default:
      break;
  }
}
