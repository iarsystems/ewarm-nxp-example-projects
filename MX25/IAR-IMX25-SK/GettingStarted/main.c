/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : October 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It shows basic use
 * of I/O, system initialization (PLL, MAX, AIPI, EMI, MMU, AITC) and timers.
 *
 *  Set the jumpers and the switches regarding the IAR KSK-i.MX25 User’s Manual
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_i2c1.h"
#include "drv_MC34704.h"

static volatile Int32U Gpt1Counter;

/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
static
void GPT1_Handler(void)
{
  // Clear interrupt flag
  GPT1SR_bit.OF1 = 1;
  ++Gpt1Counter;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main(void)
{
Int32U LedState = -1ul;

  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

  // LEDs Init
  IOMUXC_SW_MUX_CTL_PAD_A14_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A15_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A16_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A17_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A18_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A19_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A20_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A21_bit.MUX_MODE = ALT5_MUX_MODE;
  LED_PORT |= LED_MASK;
  LED_DIR  |= LED_MASK;

  // Init AITC
  ASIC_Init();

  // Init GTIM
  Gpt1Counter = 0;
  // Enable GPT1 clock
  CGCR0_bit.ipg_per_gpt  = 1;
  CGCR1_bit.ipg_clk_gpt1 = 1;

  // Disable timer
  GPT1CR = 0;
  // Disable interrupt
  GPT1IR_bit.OF1IE = 0;
  // Reset Timer1
  GPT1CR_bit.SWR = 1;
  while(GPT1CR_bit.SWR);
  // ipg_clk_highfreq - PERCLK5
  GPT1CR_bit.CLKSRC= 2;
  // reset counters
  GPT1CR_bit.ENMOD = 1;
  // Restart counter
  GPT1CR_bit.FRR = 0;

  // Init the Timer's period
  GPT1PR_bit.PRESCALER = 0;
  GPT1OCR1 = HCLK / ((PCDR1_bit.PERDIV5+1) * GPT1_TICK_PER_SEC);
  // Init GPT1 interrupt
  ASIC_SetupIntr(GPT1_Handler,FALSE,INT_GPT1,1);
  ASIC_EnableIntSource(INT_GPT1);
  // Enable GPT1
  GPT1CR_bit.EN = 1;
  // Enable Oput compare 1 interrupt
  GPT1IR_bit.OF1IE = 1;

  I2C1_Init();

  // Enable interrupts
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();
  __enable_interrupt();

  // MC34704 init
  MC34704_Init();

  while(1)
  {
    if(Gpt1Counter >= 4)
    {
      Gpt1Counter = 0;
      Int32U tmp = LED_PORT & ~LED_MASK;
      tmp = LedState & LED_MASK;
      LED_PORT = tmp;
      --LedState;
    }
  }
}
